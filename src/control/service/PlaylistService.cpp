#include "PlaylistService.h"

#include "data/playlist/Playlist.h"
#include "control/service/MetadataManager.h"
#include "data/playlist/Queue.h"
#include "ShowInfo.h"

PlaylistService::PlaylistService(QObject* parent)
    : QObject(parent)
{
}

void PlaylistService::addSongToPlaylist(qint64 playlistId, qint64 songId)
{
    if (playlistId < 0 || songId < 0) return;

    Playlist playlist(playlistId);
    playlist.addSong(songId);
}

void PlaylistService::addSongToNewPlaylist(const QString& name, qint64 songId)
{
    if (name.isEmpty() || songId < 0) return;

    Playlist playlist;
    playlist.addNewList(name);
    playlist.addSong(songId);
}

void PlaylistService::createPlaylist(const QString& name)
{
    if (name.isEmpty()) return;

    Playlist playlist;
    playlist.addNewList(name);
}

void PlaylistService::addQueueToNewPlaylist(const QString& name)
{
    if (name.isEmpty()) return;

    Playlist playlist;
    playlist.addNewList(name);
    playlist.addSongList(Queue::getInstance().getList());
}

void PlaylistService::addQueueToPlaylist(qint64 playlistId)
{
    if (playlistId < 0) return;

    Playlist playlist(playlistId);
    playlist.addSongList(Queue::getInstance().getList());
}

void PlaylistService::removeSong(qint64 playlistId, qint64 songId)
{
    if (playlistId < 0 || songId < 0) return;

    Playlist playlist(playlistId);
    playlist.removeSong(songId);
}

void PlaylistService::moveSong(qint64 playlistId, int from, int to)
{
    if (playlistId < 0 || from == to) return;

    Playlist playlist(playlistId);
    playlist.moveSong(from, to);
}

void PlaylistService::removePlaylist(qint64 playlistId)
{
    if (playlistId < 0) return;

    Playlist playlist(playlistId);
    playlist.removePlaylist();
}

void PlaylistService::setCover(qint64 playlistId, const QString &filePath)
{
    if (playlistId < 0) return;
    MetadataManager::getInstance().setPlaylistCover(playlistId, filePath);
}

AlbumModel::AlbumModel(QObject* parent)
    : QAbstractListModel(parent)
{
    reload();
    connect(&MetadataManager::getInstance(),&MetadataManager::albumUpdate, this, &AlbumModel::reload);
}

int AlbumModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return list.size();
}

QVariant AlbumModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid() || index.row() >= list.size())
        return {};

    if (role == PlaylistIdRole)
        return list.at(index.row());

    return {};
}

QHash<int, QByteArray> AlbumModel::roleNames() const
{
    return {
        { PlaylistIdRole, "playlistId" }
    };
}

void AlbumModel::reload()
{
    beginResetModel();

    list = MetadataManager::getInstance().getAlbum();
    list.append(-1);

    endResetModel();
}