#include "Song.h"

Song::Song(const QString& path,
           const QString& title,
           const QString& artist,
           qint64 duration,
           const QImage& cover)
    : title(title),
    artist(artist),
    path(path),
    duration(duration),
    cover(cover)
{
}