#include "MetadataManager.h"

#include <taglib.h>
#include <taglib/fileref.h>
#include <taglib/audioproperties.h>
#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>

#include <QDir>
#include <QFileInfo>
#include <QStandardPaths>
#include <QVariant>
#include <QDebug>

#include <QImage>
#include <QCryptographicHash>

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

    if (existsInDB(path, songId)) {
        //emit metadataReady();
        emit songReady(songId);
        return;
    }

    QString title, artist;
    qint64 duration = 0;

    songId = insertSong(path);

    if (songId >= 0)
        pathCache.insert(songId, path);

    //emit metadataReady();
    emit songReady(songId);
}

void MetadataManager::processFolder(const QStringList& list)
{
    QList<qint64> listId;
    QList<qint64> tempIds;
    QStringList tempPaths;

    database.transaction();
    QSqlQuery query(database);
    query.prepare(R"(
        INSERT INTO songs (path, title, artist, duration, artworkId)
        VALUES (?, ?, ?, ?, NULL)
    )");

    for (const QString& path : list) {
        qint64 songId = -1;

        if (existsInDB(path, songId)) {
            listId.append(songId);
            pathCache.insert(songId, path);
        }
        else {
            QString tempTitle = QFileInfo(path).completeBaseName();
            query.bindValue(0, path);
            query.bindValue(1, tempTitle);
            query.bindValue(2, "Unknown Artist");
            query.bindValue(3, 0);

            if (query.exec()) {
                songId = query.lastInsertId().toLongLong();
                listId.append(songId);
                pathCache.insert(songId, path);

                tempIds.append(songId);
                tempPaths.append(path);
            } else {
                qDebug() << "False Insert:" << query.lastError();
            }
        }
    }
    database.commit();

    if (!tempIds.isEmpty()) {
        QList<qint64> ids = tempIds;
        QStringList paths = tempPaths;

        QThreadPool::globalInstance()->start([=]() {
            QString connName = QString("worker_%1")
            .arg((quintptr)QThread::currentThreadId());

            QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", connName);
            db.setDatabaseName(appDataPath() + "/songs.db");

            if (!db.open()) {
                qDebug() << db.lastError();;
                return;
            }

            QSqlQuery updateQuery(db);

            updateQuery.prepare(R"(
                UPDATE songs
                SET title = ?, artist = ?, duration = ?, artworkId = ?
                WHERE songId = ?
            )");

            db.transaction();

            for (int i = 0; i < ids.size(); ++i) {
                qint64 id = ids[i];
                QString path = paths[i];

                QString title, artist;
                qint64 duration = 0;

                extractMetadata(path, title, artist, duration);
                qint64 artworkId = extractArtworks(path, db);

                updateQuery.bindValue(0, title);
                updateQuery.bindValue(1, artist);
                updateQuery.bindValue(2, duration);
                if (artworkId == -1)
                    updateQuery.bindValue(3, QVariant());
                else
                    updateQuery.bindValue(3, artworkId);
                updateQuery.bindValue(4, id);

                if (!updateQuery.exec()) {
                    qDebug() << "False in Metadataupdated:" << id << updateQuery.lastError();
                }
            }
            db.commit();
            updateQuery.finish();
            db.close();
            updateQuery = QSqlQuery();
            db = QSqlDatabase();
            QSqlDatabase::removeDatabase(connName);
            emit queueReady(listId);
            //emit metadataReady();
        });
    } else {
        emit queueReady(listId);
        //emit metadataReady();
    }
}


QString MetadataManager::appDataPath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppDataLocation);
}

QString MetadataManager::appLocalDataPath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}

QString MetadataManager::getPathById(qint64 songId)
{
    auto it = pathCache.find(songId);
    if (it != pathCache.end())
        return it.value();

    QString path = queryPathFromDB(songId);

    if (!path.isEmpty())
        pathCache.insert(songId, path);

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

    if (query.next())
        return query.value(0).toString();

    return "";
}

bool MetadataManager::existsInDB(const QString& path, qint64 &songId)
{
    QSqlQuery query(database);
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

    QString title, artist;
    qint64 duration = 0;

    extractMetadata(path, title, artist, duration);
    qint64 artworkId = extractArtworks(path, database);

    query.prepare(R"(
        INSERT INTO songs (path, title, artist, duration, artworkId)
        VALUES (?, ?, ?, ?, ?)
    )");

    query.addBindValue(path);
    query.addBindValue(title);
    query.addBindValue(artist);
    query.addBindValue(duration);
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

void MetadataManager::extractMetadata(const QString& path,
                                      QString &title,
                                      QString &artist,
                                      qint64 &duration)
{
    artist = "Unknown";
    duration = 0;
    #if defined(Q_OS_WIN)
        TagLib::FileRef f(path.toStdWString().c_str());
    #else
        TagLib::FileRef f(QFile::encodeName(path).constData());
    #endif

    if (f.isNull() || !f.tag()) {
        title = QFileInfo(path).completeBaseName();
        return;
    }

    TagLib::Tag *tag = f.tag();
    if (!tag->title().isEmpty())
        title = QString::fromUtf8(tag->title().to8Bit(true).c_str());
    else
        title = QFileInfo(path).completeBaseName();

    if (!tag->artist().isEmpty())
        artist = QString::fromUtf8(tag->artist().to8Bit(true).c_str());

    if (f.audioProperties()) {
        duration = static_cast<qint64>(f.audioProperties()->lengthInMilliseconds());
    }
}

qint64 MetadataManager::extractArtworks(const QString& path, QSqlDatabase &db)
{
    QByteArray imageData;

    #if defined(Q_OS_WIN)
        TagLib::MPEG::File mpegFile(path.toStdWString().c_str());
    #else
        TagLib::MPEG::File mpegFile(QFile::encodeName(path).constData());
    #endif

    if (mpegFile.isValid() && mpegFile.ID3v2Tag()) {
        TagLib::ID3v2::Tag *tag = mpegFile.ID3v2Tag();
        TagLib::ID3v2::FrameList list = tag->frameListMap()["APIC"];
        qDebug() << "APIC count:" << list.size();
        if (!list.isEmpty()) {
            for (auto *frame : list) {
                auto pic = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frame);
                if (pic) {
                    imageData = QByteArray(pic->picture().data(), pic->picture().size());
                }
            }
        }
    }

    if (imageData.isEmpty()) return -1;

    QString hash = QCryptographicHash::hash(imageData, QCryptographicHash::Md5).toHex();

    QSqlQuery query(db);
    query.prepare("SELECT artworkId FROM artworks WHERE hash = ?");
    query.addBindValue(hash);
    if (query.exec() && query.next()) {
        return query.value(0).toLongLong();
    }

    QImage img;
    if (!img.loadFromData(imageData)) {
        qDebug() << "Image size:" << imageData.size();
        return -1;
    }

    QString coverDir = appLocalDataPath() + "/covers/full/";
    QString miniDir = appLocalDataPath() + "/covers/mini/";

    QDir().mkpath(coverDir);
    QDir().mkpath(miniDir);

    QString coverPath = coverDir + hash + "_full.jpg";
    QString miniCoverPath = miniDir + hash + "_mini.jpg";

    qint64 size = img.width() < img.height() ? img.width() : img.height();
    quint64 resize = size < 850 ? size : 850;
    QImage mainCover = img.scaled(resize, resize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage miniCover = img.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    mainCover.save(coverPath, "JPG", 90);
    miniCover.save(miniCoverPath, "JPG", 85);

    query.prepare(R"(
        INSERT INTO artworks (hash, miniCoverPath, coverPath)
        VALUES (?, ?, ?)
    )");
    query.addBindValue(hash);
    query.addBindValue(miniCoverPath);
    query.addBindValue(coverPath);

    if (query.exec()) {
        return query.lastInsertId().toLongLong();
    }
    qDebug() << "INSERT ERROR:" << query.lastError();
    return -1;
}

/// Attempt to clean file
/*
void MetadataManager::cleanMissingFiles()
{
    if (!database.isOpen())
        return;

    struct MissingSong
    {
        qint64 songId;
        qint64 artworkId;
        QString path;
        QString coverPath;
        QString miniCoverPath;
    };

    QList<MissingSong> missingList;

    QSqlQuery query(database);

    query.prepare(R"(
        SELECT
            songs.songId,
            songs.path,
            songs.artworkId,
            artworks.coverPath,
            artworks.miniCoverPath

        FROM songs

        LEFT JOIN artworks
        ON songs.artworkId = artworks.artworkId
    )");

    if(!query.exec())
    {
        qDebug()
        << "Missing scan failed:"
        << query.lastError();

        return;
    }

    while(query.next())
    {
        qint64 songId =
            query.value("songId")
                .toLongLong();


        QString path =
            query.value("path")
                .toString();


        if(!QFileInfo::exists(path))
        {
            MissingSong item;

            item.songId = songId;

            item.artworkId =
                query.value("artworkId")
                    .toLongLong();

            item.path = path;

            item.coverPath =
                query.value("coverPath")
                    .toString();

            item.miniCoverPath =
                query.value("miniCoverPath")
                    .toString();

            missingList.append(item);
        }
    }

    if(missingList.isEmpty())
    {
        qDebug()
        << "No missing files";

        return;
    }

    database.transaction();

    QSqlQuery deleteSong(database);

    deleteSong.prepare(
        "DELETE FROM songs WHERE songId=?"
        );

    for(const MissingSong& item : missingList)
    {
        if(!item.coverPath.isEmpty())
        {
            QFile::remove(
                item.coverPath
                );
        }

        if(!item.miniCoverPath.isEmpty())
        {
            QFile::remove(
                item.miniCoverPath
                );
        }

        deleteSong.bindValue(
            0,
            item.songId
            );


        if(!deleteSong.exec())
        {
            qDebug()
            << "Delete song failed:"
            << deleteSong.lastError();
        }


        pathCache.remove(
            item.songId
            );
    }
    database.commit();

    query.prepare(R"(
        SELECT artworkId,
               coverPath,
               miniCoverPath

        FROM artworks

        WHERE artworkId NOT IN
        (
            SELECT artworkId
            FROM songs
            WHERE artworkId IS NOT NULL
        )
    )");

    if(!query.exec())
    {
        qDebug()
        << query.lastError();

        return;
    }
    QList<qint64> removeIds;

    while(query.next())
    {
        qint64 id =
            query.value(0)
                .toLongLong();

        QString cover =
            query.value(1)
                .toString();

        QString mini =
            query.value(2)
                .toString();

        QFile::remove(cover);
        QFile::remove(mini);

        removeIds.append(id);
    }

    if(removeIds.isEmpty())
        return;

    QSqlQuery del(database);
    del.prepare(
        "DELETE FROM artworks WHERE artworkId=?"
        );

    for(qint64 id : removeIds)
    {
        del.bindValue(
            0,
            id
            );

        if(!del.exec())
        {
            qDebug()
            << "Artwork delete failed:"
            << del.lastError();
        }
    }

    qDebug()
        << "Removed unused artworks:"
        << removeIds.size();

    qDebug()
        << "Removed missing songs:"
        << missingList.size();
}
*/

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

SongLite MetadataManager::getMetadata(qint64 songId) const
{
    SongLite meta;

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
        return SongLite(
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