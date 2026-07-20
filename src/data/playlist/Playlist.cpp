#include "Playlist.h"

#include "control/service/MetadataManager.h"

Playlist::Playlist(qint64 playlistId) : playlistId(playlistId)
{}

void Playlist::addSong(qint64 songId)
{
    if (songId == -1) return;
    list.append(songId);
    MetadataManager::getInstance().addSongPlaylist(playlistId, songId);
}

void Playlist::addSongList(const QList<qint64>& listSongId)
{
    if (listSongId.isEmpty()) return;
    list.append(listSongId);
    MetadataManager::getInstance().addSongList(playlistId, listSongId);
}

void Playlist::addNewList(const QString& name)
{
    playlistId = MetadataManager::getInstance().addNewPlaylist(name);
}

void Playlist::load(qint64 albumId)
{
    if (albumId == -1)
        return;
    list = MetadataManager::getInstance().getSongList(albumId);
    playlistId = albumId;
}

void Playlist::removeSong(qint64 songId)
{
    list.removeAt(list.indexOf(songId));
    MetadataManager::getInstance().removeSong(playlistId, songId);
}

void Playlist::moveSong(int from, int to){
    list.move(from, to);
    MetadataManager::getInstance().moveSong(playlistId, from, to);
}

void Playlist::removePlaylist()
{
    if (playlistId < 0) return;
    MetadataManager::getInstance().removePlaylist(playlistId);
    list.clear();
    playlistId = -1;
}

const QList<qint64>& Playlist::songs() const
{
    return list;
}

int Playlist::size() const
{
    return list.size();
}

qint64 Playlist::getId() const
{
    return playlistId;
}