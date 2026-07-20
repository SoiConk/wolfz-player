#include "control/service/MetadataManager.h"
#include "ShowInfo.h"

#include <QDebug>

ShowInfo::ShowInfo(QObject* parent) : QObject(parent)
{
    connect(&MetadataManager::getInstance(), &MetadataManager::clearInfoCache, this, &ShowInfo::clearCache);
    connect(&MetadataManager::getInstance(), &MetadataManager::clearedAlbumCache, this, &ShowInfo::clearAlbumCache);
    connect(this, &ShowInfo::reloadAlbumCache, &MetadataManager::getInstance(), &MetadataManager::reloadAlbum);
}

const SongShowInfo& ShowInfo::getOrCreate(qint64 songId) const
{
    SongShowInfo* info = cache.object(songId);
    if (info != nullptr)
    {
        return *info;
    }

    SongShowInfo data = MetadataManager::getInstance().getMetadata(songId);
    cache.insert(songId, new SongShowInfo(data));
    return *cache.object(songId);
}

QString ShowInfo::title(qint64 songId) const
{
    return getOrCreate(songId).title;
}

QString ShowInfo::artist(qint64 songId) const
{
    return getOrCreate(songId).artist;
}

QString ShowInfo::duration(qint64 songId) const
{
    qint64 ms = getOrCreate(songId).duration;
    qint64 totalSeconds = ms / 1000;

    qint64 minutes = totalSeconds / 60;
    qint64 seconds = totalSeconds % 60;

    return QString("%1:%2")
        .arg(minutes)
        .arg(seconds, 2, 10, QChar('0'));
}

QString ShowInfo::miniCoverPath(qint64 songId) const
{
    QString path = getOrCreate(songId).miniCoverPath;
    return !path.isEmpty() ? "file:///" + path : "";
}

QString ShowInfo::coverPath(qint64 songId) const
{
    QString path = MetadataManager::getInstance().getCoverPath(songId);
    return !path.isEmpty() ? "file:///" + path : "";
}

void ShowInfo::clearCache()
{
    cache.clear();
}

const AlbumInfo& ShowInfo::getOrCreateAlbum(qint64 albumId) const
{
    AlbumInfo* info = albumCache.object(albumId);
    if (info != nullptr)
    {
        return *info;
    }

    AlbumInfo data = MetadataManager::getInstance().getAlbumInfo(albumId);
    albumCache.insert(albumId, new AlbumInfo(data));
    return *albumCache.object(albumId);
}

QString ShowInfo::name(qint64 albumId) const
{
    return getOrCreateAlbum(albumId).name;
}

QString ShowInfo::durations(qint64 albumId) const
{
    qint64 ms = getOrCreateAlbum(albumId).duration;
    qint64 totalSeconds = ms / 1000;

    qint64 hours = totalSeconds / 3600;
    qint64 minutes = (totalSeconds % 3600) / 60;
    qint64 seconds = totalSeconds % 60;

    if (hours > 0) {
        return QString("%1:%2:%3")
        .arg(hours)
            .arg(minutes, 2, 10, QChar('0'))
            .arg(seconds, 2, 10, QChar('0'));
    }

    return QString("%1:%2")
        .arg(minutes)
        .arg(seconds, 2, 10, QChar('0'));
}

QString ShowInfo::albumCoverPath(qint64 albumId) const
{
    QString path = getOrCreateAlbum(albumId).albumCoverPath;
    return !path.isEmpty() ? "file:///" + path : "";
}

void ShowInfo::clearAlbumCache()
{
    albumCache.clear();
    emit reloadAlbumCache();
}