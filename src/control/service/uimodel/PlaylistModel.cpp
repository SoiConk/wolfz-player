#include "PlaylistModel.h"
#include "control/service/MetadataManager.h"

PlaylistModel::PlaylistModel(QObject* parent) : QAbstractListModel(parent)
{
    connect(&MetadataManager::getInstance(), &MetadataManager::playlistUpdate, this, &PlaylistModel::reload);
}

int PlaylistModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;
    return playlist.size();
}

QVariant PlaylistModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return {};

    const QList<qint64>& list = getList();

    if (index.row() >= list.size()) return {};

    if (role == SongIdRole)
        return list[index.row()];

    return {};
}

QHash<int, QByteArray> PlaylistModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[SongIdRole] = "songId";
    return roles;
}

void PlaylistModel::load(qint64 playlistId)
{
    if (this->playlistId() == playlistId) return;

    beginResetModel();

    playlist.load(playlistId);

    endResetModel();

    emit playlistChanged();
}

void PlaylistModel::reload()
{
    beginResetModel();

    playlist.load(playlist.getId());

    endResetModel();
}

const QList<qint64>& PlaylistModel::getList() const
{
    return playlist.songs();
}

qint64 PlaylistModel::playlistId() const
{
    return playlist.getId();
}