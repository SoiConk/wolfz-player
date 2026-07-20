#include "Song.h"

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

AlbumInfo::AlbumInfo(const QString& name,
                     qint64 duration,
                     const QString& albumCoverPath
                     ) :
    name(name),
    duration(duration),
    albumCoverPath(albumCoverPath)
{}