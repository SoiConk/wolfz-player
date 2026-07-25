#include "History.h"

History& History::getInstance()
{
    static History instance;
    return instance;
}

void History::add(qint64 songId)
{
    listSong.removeAll(songId);
    listSong.prepend(songId);

    if (listSong.size() > maxSizeList)
        listSong.removeLast();
    emit changedHistory(listSong);
}

void History::addList(const QList<qint64>& listSongId)
{
    if (listSongId.empty())
        return;
    listSong = (listSongId.size() <= maxSizeList) ? listSongId : listSongId.first(maxSizeList);
    emit changedHistory(listSong);
}

QList<qint64> History::getAll() const
{
    return listSong;
}

void History::addPlaylist(qint64 playlistId)
{
    listPlaylist.removeAll(playlistId);
    listPlaylist.prepend(playlistId);

    if (listPlaylist.size() > maxPlaylistHistory)
        listPlaylist.removeLast();

    emit changedPlaylistHistory(listPlaylist);
}


void History::addPlaylistList(const QList<qint64>& listPlaylistId)
{
    if (listPlaylistId.isEmpty())
        return;

    listPlaylist = (listPlaylistId.size() <= maxSizeList) ? listPlaylistId : listPlaylistId.first(maxSizeList);

    emit changedPlaylistHistory(listPlaylist);
}


QList<qint64> History::getAllPlaylist() const
{
    return listPlaylist;
}