#ifndef PLAYLISTSERVICE_H
#define PLAYLISTSERVICE_H

#include <QtQml>

class PlaylistService : public QObject
{
    Q_OBJECT
    QML_ELEMENT

public:
    explicit PlaylistService(QObject* parent = nullptr);
    Q_INVOKABLE void addSongToPlaylist(qint64 playlistId, qint64 songId);
    Q_INVOKABLE void addSongToNewPlaylist(const QString& name, qint64 songId);
    Q_INVOKABLE void createPlaylist(const QString& name);
    Q_INVOKABLE void createPlaylistFromQueue(const QString& name);
    Q_INVOKABLE void addQueueToPlaylist(qint64 playlistId);
    Q_INVOKABLE void removeSong(qint64 playlistId, qint64 songId);
    Q_INVOKABLE void moveSong(qint64 playlistId, int from, int to);
    Q_INVOKABLE void removePlaylist(qint64 playlistId);
    Q_INVOKABLE void setCover(qint64 playlistId, const QString &filePath);
};

class AlbumModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum Roles {
        PlaylistIdRole = Qt::UserRole + 1
    };
    explicit AlbumModel(QObject* parent = nullptr);
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void reload();

private:
    QList<qint64> list;
};

#endif // PLAYLISTSERVICE_H
