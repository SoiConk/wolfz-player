#ifndef PLAYLIST_H
#define PLAYLIST_H

#include <QObject>

class Playlist {

public:
    explicit Playlist(qint64 playlistId = -1);

    void addSong(qint64 songId);
    void addSongList(const QList<qint64>& listSongId);
    void addNewList(const QString& name);
    void load(qint64 albumId);
    void removeSong(qint64 songId);
    void moveSong(int from, int to);
    void removePlaylist();

    const QList<qint64>& songs() const;
    int size() const;
    qint64 getId() const;

private:
    QList<qint64> list;
    qint64 playlistId;
};
#endif // PLAYLIST_H
