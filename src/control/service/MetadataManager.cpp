#include "MetadataManager.h"

#include <QSqlQuery>
#include <QSqlError>

#include <QtConcurrent>

MetadataManager& MetadataManager::getInstance()
{
    static MetadataManager instance;
    return instance;
}

MetadataManager::MetadataManager() : QObject(nullptr)
{
    initDatabase();
}

MetadataManager::~MetadataManager()
{
    if (database.isOpen())
        database.close();
}

void MetadataManager::initDatabase()
{
    database = QSqlDatabase::addDatabase("QSQLITE", "main_connection");

    QString appDataPath = this->appDataPath();
    QDir dir(appDataPath);

    if (!dir.exists())
    {
        dir.mkpath(".");
    }

    database.setDatabaseName(appDataPath + "/songs.db");

    if (!database.open()) {
        qDebug() << "DB open failed:" << database.lastError();
        return;
    }

    QSqlQuery query(database);
    query.exec("PRAGMA journal_mode=WAL;");
    query.exec("PRAGMA foreign_keys = ON;");

    query.exec(R"(
    CREATE TABLE IF NOT EXISTS artworks (
        artworkId INTEGER PRIMARY KEY AUTOINCREMENT,
        hash TEXT UNIQUE,
        miniCoverPath TEXT,
        coverPath TEXT
    );
    )");

    query.exec(R"(
        CREATE TABLE IF NOT EXISTS songs (
            songId INTEGER PRIMARY KEY AUTOINCREMENT,
            path TEXT UNIQUE,
            title TEXT,
            artist TEXT,
            duration INTEGER,
            artworkId INTEGER,
            FOREIGN KEY (artworkId)
                REFERENCES artworks(artworkId)
        );
    )");

    query.exec(R"(CREATE TABLE IF NOT EXISTS queue(
            id INTEGER PRIMARY KEY,
            songId INTEGER
        );
    )");

    query.exec(R"(CREATE TABLE IF NOT EXISTS history(
            id INTEGER PRIMARY KEY,
            songId INTEGER
        );
    )");

    query.exec(R"(CREATE TABLE IF NOT EXISTS playlists (
            playlistId INTEGER PRIMARY KEY AUTOINCREMENT,
            name TEXT,
            duration INTEGER,
            albumCoverPath TEXT
        );
    )");

    query.exec(R"(CREATE TABLE IF NOT EXISTS playlist_songs (
            id INTEGER PRIMARY KEY AUTOINCREMENT,
            position INTEGER,
            playlistId INTEGER,
            songId INTEGER,
            FOREIGN KEY (playlistId) REFERENCES playlists(playlistId)
        );
    )");

    query.exec("CREATE INDEX IF NOT EXISTS idx_path ON songs(path);");
}

void MetadataManager::processFile(const QString& path)
{
    qint64 songId = -1;

    if (existsInDB(path, songId, database)) {
        emit songReady(songId);
        return;
    }

    songId = insertSong(path);

    if (songId >= 0)
        pathCache.insert(songId, path);

    emit songReady(songId);
}

void MetadataManager::processFolder(const QStringList& list)
{
    processFile(list[0]);

    qint64 firstId = -1;
    if (!existsInDB(list[0], firstId, database))
        return;
    QThreadPool::globalInstance()->start([=]() {
        QString connName = QString("worker_%1").arg((quintptr)QThread::currentThreadId());

        QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
        db.setDatabaseName(appDataPath() + "/songs.db");

        if (!db.open()) {
            qDebug() << db.lastError();;
            return;
        }

        QSqlQuery insertQuery(db);
        QList<qint64> listId;
        listId.append(firstId);

        insertQuery.prepare(R"(
            INSERT INTO songs (path, title, artist, duration, artworkId)
            VALUES (?, ?, ?, ?, ?)
        )");

        bool success = true;

        db.transaction();

        for (int i = 1; i < list.size(); i++) {
            const QString& path = list[i];
            qint64 songId = -1;

            if (existsInDB(path, songId, db)) {
                listId.append(songId);
            } else {
                SongInfo song = getSongInfo(path);
                qint64 artworkId = getArtworkId(path, db);

                insertQuery.bindValue(0, path);
                insertQuery.bindValue(1, song.title);
                insertQuery.bindValue(2, song.artist);
                insertQuery.bindValue(3, song.duration);
                if (artworkId == -1)
                    insertQuery.bindValue(4, QVariant());
                else
                    insertQuery.bindValue(4, artworkId);

                if (insertQuery.exec()) {
                    songId = insertQuery.lastInsertId().toLongLong();
                    listId.append(songId);
                } else {
                    qDebug() << "Insert fail:" << insertQuery.lastError();
                    success = false;
                    break;
                }
            }

            QMetaObject::invokeMethod(this, [=]() {
                pathCache.insert(songId, path);
            }, Qt::QueuedConnection);
        }
        if (success)
            db.commit();
        else
            db.rollback();

        insertQuery.finish();
        insertQuery = QSqlQuery();
        db.close();
        db = QSqlDatabase();
        QSqlDatabase::removeDatabase(connName);

        if (success) {
        QMetaObject::invokeMethod(this, [=]() {
            emit queueReady(listId);
        }, Qt::QueuedConnection);
        } else {
            QMetaObject::invokeMethod(this, [=]() {
                pathCache.clear();
            }, Qt::QueuedConnection);
        }
    });
}


QString MetadataManager::appDataPath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString MetadataManager::getPathById(qint64 songId)
{
    if (pathCache.contains(songId)) {
        QString cachedPath = pathCache.value(songId);
        return cachedPath;
    }

    QString path = queryPathFromDB(songId);


    if (!path.isEmpty()) {
        pathCache.insert(songId, path);
    }

    return path;
}

QString MetadataManager::queryPathFromDB(qint64 songId)
{
    QSqlQuery query(database);
    query.prepare("SELECT path FROM songs WHERE songId = ?");
    query.addBindValue(songId);

    if (!query.exec()) {
        qDebug() << "Query failed:" << query.lastError();
        return "";
    }

    if (query.next()) {
        QString path = query.value(0).toString();

        if (!QFile::exists(path)) {
            removeMissingSong(songId);
            return "";
        }

        return path;
    }

    return "";
}

bool MetadataManager::existsInDB(const QString& path, qint64 &songId, QSqlDatabase& db)
{
    QSqlQuery query(db);
    query.prepare("SELECT songId FROM songs WHERE path = ?");
    query.addBindValue(path);

    if (!query.exec()) {
        qDebug() << "Exists query failed:" << query.lastError();
        return false;
    }

    if (query.next()) {
        songId = query.value(0).toLongLong();
        return true;
    }

    return false;
}

qint64 MetadataManager::insertSong(const QString& path)
{
    QSqlQuery query(database);

    SongInfo song = getSongInfo(path);
    qint64 artworkId = getArtworkId(path, database);

    query.prepare(R"(
        INSERT INTO songs (path, title, artist, duration, artworkId)
        VALUES (?, ?, ?, ?, ?)
    )");

    query.addBindValue(path);
    query.addBindValue(song.title);
    query.addBindValue(song.artist);
    query.addBindValue(song.duration);
    if (artworkId == -1)
        query.addBindValue(QVariant());
    else
        query.addBindValue(artworkId);

    if (!query.exec()) {
        qDebug() << "Insert failed:" << query.lastError();
        return -1;
    }

    return query.lastInsertId().toLongLong();
}

SongInfo MetadataManager::getSongInfo(const QString& path)
{
    return metadata.getInfo(path);
}

qint64 MetadataManager::getArtworkId(const QString& path, QSqlDatabase &db)
{
    SongCover songCover = metadata.getCover(path);
    if (songCover.hash.isEmpty()) {
        return -1;
    }
    QSqlQuery query(db);
    query.prepare("SELECT artworkId FROM artworks WHERE hash = ?");
    query.addBindValue(songCover.hash);
    if (query.exec() && query.next()) {
        return query.value(0).toLongLong();
    }

    query.prepare(R"(
        INSERT INTO artworks (hash, miniCoverPath, coverPath)
        VALUES (?, ?, ?)
    )");
    query.addBindValue(songCover.hash);
    query.addBindValue(songCover.miniCoverPath);
    query.addBindValue(songCover.coverPath);

    if (query.exec()) {
        return query.lastInsertId().toLongLong();
    }
    qDebug() << "INSERT ERROR:" << query.lastError();
    return -1;
}

QList<qint64> MetadataManager::getQueue() const
{
    QList<qint64> result;

    QSqlQuery query(database);
    query.exec("SELECT songId FROM queue ORDER BY id ASC");

    while (query.next()) {
        result.append(query.value(0).toLongLong());
    }

    return result;
}

void MetadataManager::setQueue(QList<qint64> queue)
{
    QSqlQuery query(database);

    database.transaction();
    query.exec("DELETE FROM queue");

    query.prepare("INSERT INTO queue(songId) VALUES (?)");

    for (qint64 id : queue) {
        query.bindValue(0, id);
        query.exec();
    }
    database.commit();
}

QList<qint64> MetadataManager::getHistory() const
{
    QList<qint64> result;

    QSqlQuery query(database);
    query.exec("SELECT songId FROM history ORDER BY id ASC");

    while (query.next()) {
        result.append(query.value(0).toLongLong());
    }

    return result;
}

void MetadataManager::setHistory(QList<qint64> history)
{
    QSqlQuery query(database);

    database.transaction();
    query.exec("DELETE FROM history");

    query.prepare("INSERT INTO history(songId) VALUES (?)");

    for (qint64 id : history) {
        query.bindValue(0, id);
        query.exec();
    }
    database.commit();
}

SongShowInfo MetadataManager::getMetadata(qint64 songId) const
{
    SongShowInfo meta;

    QSqlQuery query(database);

    query.prepare(R"(
        SELECT
            songs.title,
            songs.artist,
            songs.duration,
            artworks.miniCoverPath
        FROM songs
        LEFT JOIN artworks
            ON songs.artworkId = artworks.artworkId
        WHERE songs.songId = ?
    )");

    query.addBindValue(songId);

    if (query.exec() && query.next())
    {
        return SongShowInfo(
            query.value(0).toString(),
            query.value(1).toString(),
            query.value(2).toLongLong(),
            query.value(3).toString());
    }

    return {};
}

QString MetadataManager::getCoverPath(qint64 songId) const
{
    QSqlQuery query(database);

    query.prepare(R"(
    SELECT artworks.coverPath
    FROM songs
    LEFT JOIN artworks
        ON songs.artworkId = artworks.artworkId
    WHERE songs.songId = ?
    )");

    query.addBindValue(songId);

    if (query.exec() && query.next()) {
        QString coverPath = query.value(0).toString();
        return coverPath;
    }
    return {};
}

void MetadataManager::addSongId(qint64 songId)
{
    emit songReady(songId);
}

void MetadataManager::removeMissingSong(qint64 songId)
{
    QSqlQuery query(database);

    query.prepare("SELECT path, artworkId FROM songs WHERE songId = ?");
    query.addBindValue(songId);

    if (!query.exec() || !query.next())
        return;

    QString path = query.value(0).toString();
    QVariant artworkVar = query.value(1);

    if (QFile::exists(path))
        return;

    qDebug() << "File missing, removing:" << path;

    database.transaction();

    query.prepare("DELETE FROM playlist_songs WHERE songId = ?");
    query.addBindValue(songId);
    query.exec();

    query.prepare("DELETE FROM queue WHERE songId = ?");
    query.addBindValue(songId);
    query.exec();

    query.prepare("DELETE FROM history WHERE songId = ?");
    query.addBindValue(songId);
    query.exec();

    query.prepare("DELETE FROM songs WHERE songId = ?");
    query.addBindValue(songId);
    query.exec();

    if (!artworkVar.isNull()) {
        qint64 artworkId = artworkVar.toLongLong();

        query.prepare("SELECT COUNT(*) FROM songs WHERE artworkId = ?");
        query.addBindValue(artworkId);

        if (query.exec() && query.next()) {
            int count = query.value(0).toInt();

            if (count == 0) {
                QSqlQuery artQuery(database);
                artQuery.prepare("SELECT miniCoverPath, coverPath FROM artworks WHERE artworkId = ?");
                artQuery.addBindValue(artworkId);

                QString miniPath, coverPath;
                if (artQuery.exec() && artQuery.next()) {
                    miniPath = artQuery.value(0).toString();
                    coverPath = artQuery.value(1).toString();
                }

                QSqlQuery delArt(database);
                delArt.prepare("DELETE FROM artworks WHERE artworkId = ?");
                delArt.addBindValue(artworkId);
                delArt.exec();

                if (!miniPath.isEmpty())
                    QFile::remove(miniPath);
                if (!coverPath.isEmpty())
                    QFile::remove(coverPath);
            }
        }
    }

    database.commit();

    pathCache.remove(songId);
    emit playlistUpdate();
    emit clearInfoCache();
}

qint64 MetadataManager::addNewPlaylist(const QString& name)
{
    QSqlQuery query(database);
    query.prepare(R"(INSERT INTO playlists(name, duration, albumCoverPath)
        VALUES (?, 0, NULL)
    )");
    query.addBindValue(name);

    if (!query.exec()) {
        qDebug() << query.lastError();
        return -1;
    }

    emit clearedAlbumCache();;
    return query.lastInsertId().toLongLong();
}

void MetadataManager::addSongPlaylist(qint64 playlistId, qint64 songId)
{
    QSqlQuery query(database);

    query.prepare("SELECT MAX(position) FROM playlist_songs WHERE playlistId = ?");
    query.addBindValue(playlistId);

    int pos = 0;
    if (query.exec() && query.next())
        pos = query.value(0).toInt() + 1;

    query.prepare(R"(
        INSERT INTO playlist_songs(playlistId, songId, position)
        VALUES (?, ?, ?)
    )");

    query.addBindValue(playlistId);
    query.addBindValue(songId);
    query.addBindValue(pos);

    query.exec();

    query.prepare("SELECT duration FROM songs WHERE songId = ?");
    query.addBindValue(songId);

    if (!query.exec() || !query.next())
        return;

    int duration = query.value(0).toInt();

    QSqlQuery update(database);
    update.prepare(R"(
        UPDATE playlists
        SET duration = duration + ?
        WHERE playlistId = ?
    )");
    update.addBindValue(duration);
    update.addBindValue(playlistId);

    if (!update.exec()) {
        qDebug() << "update duration failed:" << update.lastError().text();
    }
    emit playlistUpdate();
}

void MetadataManager::addSongList(qint64 playlistId, const QList<qint64>& songList)
{
    QSqlQuery query(database);

    database.transaction();

    query.prepare("SELECT MAX(position) FROM playlist_songs WHERE playlistId = ?");
    query.addBindValue(playlistId);

    int pos = 0;
    if (query.exec() && query.next())
        pos = query.value(0).toInt() + 1;

    query.prepare(R"(
        INSERT INTO playlist_songs(playlistId, songId, position)
        VALUES (?, ?, ?)
    )");

    int totalDuration = 0;
    for (qint64 songId : songList)
    {
        query.bindValue(0, playlistId);
        query.bindValue(1, songId);
        query.bindValue(2, pos++);
        query.exec();

        QSqlQuery d(database);
        d.prepare("SELECT duration FROM songs WHERE songId = ?");
        d.addBindValue(songId);

        if (d.exec() && d.next())
            totalDuration += d.value(0).toInt();
    }

    QSqlQuery update(database);
    update.prepare("UPDATE playlists SET duration = duration + ? WHERE playlistId = ?");
    update.addBindValue(totalDuration);
    update.addBindValue(playlistId);
    update.exec();

    database.commit();
    emit playlistUpdate();
}

QList<qint64> MetadataManager::getSongList(qint64 playlistId)
{
    QList<qint64> result;

    QSqlQuery query(database);
    query.prepare(R"(
        SELECT songId FROM playlist_songs
        WHERE playlistId = ?
        ORDER BY position ASC
    )");

    query.addBindValue(playlistId);

    if (query.exec())
    {
        while (query.next())
            result.append(query.value(0).toLongLong());
    }

    return result;
}

void MetadataManager::removeSong(qint64 playlistId, qint64 songId)
{
    QSqlQuery query(database);
    database.transaction();

    query.prepare(R"(
        SELECT position, s.duration
        FROM playlist_songs ps
        JOIN songs s ON ps.songId = s.songId
        WHERE ps.playlistId = ? AND ps.songId = ?
    )");
    query.addBindValue(playlistId);
    query.addBindValue(songId);

    int pos = -1;
    int duration = 0;

    if (query.exec() && query.next()) {
        pos = query.value(0).toInt();
        duration = query.value(1).toInt();
    }

    query.prepare(R"(
        DELETE FROM playlist_songs
        WHERE playlistId = ? AND songId = ?
    )");

    query.addBindValue(playlistId);
    query.addBindValue(songId);
    query.exec();

    QSqlQuery update(database);
    update.prepare("UPDATE playlists SET duration = duration - ? WHERE playlistId = ?");
    update.addBindValue(duration);
    update.addBindValue(playlistId);
    update.exec();

    query.prepare(R"(
        UPDATE playlist_songs
        SET position = position - 1
        WHERE playlistId = ? AND position > ?
    )");
    query.addBindValue(playlistId);
    query.addBindValue(pos);
    query.exec();

    database.commit();
    emit playlistUpdate();
}

void MetadataManager::moveSong(qint64 playlistId, int from, int to)
{
    QList<qint64> list = getSongList(playlistId);

    if (from < 0 || from >= list.size() || to < 0 || to >= list.size())
        return;

    list.move(from, to);

    QSqlQuery query(database);
    database.transaction();

    query.prepare("DELETE FROM playlist_songs WHERE playlistId = ?");
    query.addBindValue(playlistId);
    query.exec();

    query.prepare(R"(
        INSERT INTO playlist_songs(playlistId, songId, position)
        VALUES (?, ?, ?)
    )");

    for (int i = 0; i < list.size(); i++)
    {
        query.bindValue(0, playlistId);
        query.bindValue(1, list[i]);
        query.bindValue(2, i);
        query.exec();
    }

    database.commit();
    emit playlistUpdate();
}

void MetadataManager::removePlaylist(qint64 playlistId)
{
    QSqlQuery query(database);

    database.transaction();

    QSqlQuery deleteSongs(database);
    deleteSongs.prepare(R"(
        DELETE FROM playlist_songs
        WHERE playlistId = ?
    )");
    deleteSongs.addBindValue(playlistId);

    if (!deleteSongs.exec()) {
        qDebug() << "removePlaylist - deleteSongs failed:" << deleteSongs.lastError().text();
        database.rollback();
        return;
    }

    query.prepare(R"(
        DELETE FROM playlists
        WHERE playlistId = ?
    )");
    query.addBindValue(playlistId);

    if (!query.exec()) {
        qDebug() << "removePlaylist failed:" << query.lastError().text();
        database.rollback();
        return;
    }

    database.commit();
    emit clearedAlbumCache();;
}

void MetadataManager::setPlaylistCover(qint64 playlistId, const QString& coverPath)
{
    QSqlQuery query(database);
    query.prepare(R"(
        UPDATE playlists
        SET albumCoverPath = ?
        WHERE playlistId = ?
    )");

    query.addBindValue(coverPath);
    query.addBindValue(playlistId);

    if (!query.exec()) {
        qDebug() << "setPlaylistCover failed:" << query.lastError().text();
    }
    emit playlistUpdate();
    emit clearedAlbumCache();;
}

QList<qint64> MetadataManager::getAlbum() const
{
    QSqlQuery query(database);
    query.prepare(R"(
        SELECT playlistId FROM playlists
        ORDER BY playlistId ASC
    )");
    QList<qint64> playlistIds;
    if (query.exec()) {
        if (query.size() > 0) {
            playlistIds.reserve(query.size());
        }

        while (query.next()) {
            qint64 id = query.value(0).toLongLong();
            playlistIds.append(id);
        }
    } else {
        qWarning() << "Lỗi select playlistId:" << query.lastError().text();
    }
    return playlistIds;
}

AlbumInfo MetadataManager::getAlbumInfo(qint64 playlistId) const
{
    SongShowInfo meta;

    QSqlQuery query(database);

    query.prepare(R"(
        SELECT
            playlists.name,
            playlists.duration,
            playlists.albumCoverPath
        FROM playlists
        WHERE playlistId = ?
    )");

    query.addBindValue(playlistId);

    if (query.exec() && query.next())
    {
        return AlbumInfo(
            query.value(0).toString(),
            query.value(1).toInt(),
            query.value(2).toString());
    }

    return {};
}

void MetadataManager::reloadAlbum()
{
    emit albumUpdate();
}