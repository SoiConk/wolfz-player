#ifndef HISTORY_H
#define HISTORY_H

#include <QStringList>
#include <QObject>

class History : public QObject
{
    Q_OBJECT

public:
    static History& getInstance();

    void add(qint64 songId);
    void addList(const QList<qint64>& listSongId);
    QList<qint64> getAll() const;

    void addPlaylist(qint64 playlistId);
    void addPlaylistList(const QList<qint64>& listPlaylistId);
    QList<qint64> getAllPlaylist() const;

private:
    History() = default;
    ~History() = default;

    History(const History&) = delete;
    History(History&&) = delete;
    History& operator=(const History&) = delete;
    History& operator=(History&&) = delete;

    QList<qint64> listSong;
    int maxSizeList = 15;

    QList<qint64> listPlaylist;
    int maxPlaylistHistory = 10;

signals:
    void changedHistory(const QList<qint64>& list);

    void changedPlaylistHistory(const QList<qint64>& list);
};

#endif