#include "Song.h"

Song::Song(const QString& path,
           qint64 songId,
           const QString& title,
           const QString& artist,
           qint64 duration,
           qint64 artworkId)
            : path(path),
              songId(songId),
              title(title),
              artist(artist),
              duration(duration),
              artworkId(artworkId)
{}

TrackReference::TrackReference(qint64 songId) : songId(songId)
{}

SongArtwork::SongArtwork(   qint64 artworkId,
                            const QString& miniCoverPath,
                            const QString& coverPath)
                            : artworkId(artworkId),
                              miniCoverPath(miniCoverPath),
                              coverPath(coverPath)
{}

SongLite::SongLite(const QString& title,
         const QString& artist,
         qint64 duration,
         const QString& miniCoverPath)
        : title(title)
        , artist(artist)
        , duration(duration)
        , miniCoverPath(miniCoverPath)
{}