#include "Song.h"

Song::Song(const QString& path,
           qint64 songId,
           const QString& title,
           const QString& artist,
           qint64 duration,
           qint64 artworkId) :
    path(path),
    songId(songId),
    title(title),
    artist(artist),
    duration(duration),
    artworkId(artworkId)
{}

SongCover::SongCover(const QString& hash,
                         const QString& miniCoverPath,
                         const QString& coverPath) :
    hash(hash),
    miniCoverPath(miniCoverPath),
    coverPath(coverPath)
{}

SongInfo::SongInfo(const QString& title,
                   const QString& artist,
                   qint64 duration) :
    title(title),
    artist(artist),
    duration(duration)
{}

SongShowInfo::SongShowInfo(const QString& title,
                           const QString& artist,
                           qint64 duration,
                           const QString& miniCoverPath) :
    title(title),
    artist(artist),
    duration(duration),
    miniCoverPath(miniCoverPath)
{}

Album::Album(qint64 albumId,
             const QList<qint64> title,
             const QList<qint64> songIdList,
             qint64 totalDuration,
             const QString& albumCoverPath
             ) :
    albumId(albumId),
    title(title),
    songIdList(songIdList),
    totalDuration(totalDuration),
    albumCoverPath(albumCoverPath)
{}