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
}