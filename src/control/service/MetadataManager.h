#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include <data/model/Song.h>
#include <control/service/ExtractMetadata.h>

#include <QObject>
#include <QHash>
#include <QStringList>
#include <QtSql/QSqlDatabase>

class MetadataManager : public QObject
{
    Q_OBJECT

public:
    static MetadataManager& getInstance();

    void processFile(const QString& file);
    void processFolder(const QStringList& list);

    QString getPathById(qint64 songId);

    QList<qint64> getQueue() const;
    void setQueue(QList<qint64> queue);
    QList<qint64> getHistory() const;
    void setHistory(QList<qint64> history);

    SongShowInfo getMetadata(qint64 songId) const;
    QString getCoverPath(qint64 songId) const;
    void addSongId(qint64 songId);
signals:
    void songReady(qint64 songId);
    void queueReady(QList<qint64> listId);
    void clearInfoCache();

public:
    qint64 addNewPlaylist(const QString& name);
    void addSongPlaylist(qint64 playlistId, qint64 songId);
    void addSongList(qint64 playlistId, const QList<qint64>& songList);
    QList<qint64> getSongList(qint64 playlistId);
    void removeSong(qint64 playlistId, qint64 songId);
    void moveSong(qint64 playlistId, int from, int to);
    void removePlaylist(qint64 playlistId);
    void setPlaylistCover(qint64 playlistId, const QString& coverPath);
    QList<qint64> getAlbum() const;
    AlbumInfo getAlbumInfo(qint64 playlistId) const;
    void reloadAlbum();

signals:
    void albumUpdate();
    void clearedAlbumCache();
    void playlistUpdate();

private:
    explicit MetadataManager();
    ~MetadataManager();

    void initDatabase();
    QString appDataPath() const;

    bool existsInDB(const QString& path, qint64 &songId, QSqlDatabase& db);
    qint64 insertSong(const QString& path);

    QString queryPathFromDB(qint64 songId);

    SongInfo getSongInfo(const QString& path);
    qint64 getArtworkId(const QString& path, QSqlDatabase &db);

    void removeMissingSong(qint64 songId);

private:
    MetadataManager(const MetadataManager&) = delete;
    MetadataManager& operator=(const MetadataManager&) = delete;

    QSqlDatabase database;

    QHash<qint64, QString> pathCache;

    ExtractMetadata metadata;
};

#endif // METADATAMANAGER_H