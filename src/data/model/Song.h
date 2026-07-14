#ifndef SONG_H
#define SONG_H

#include <QString>
#include <QtGlobal>

struct Song
{
    QString path;
    qint64 songId = -1;
    QString title;
    QString artist;
    qint64 duration = 0;
    qint64 artworkId = -1;

    Song() = default;
    explicit Song(const QString& path,
         qint64 songId,
         const QString& title,
         const QString& artist,
         qint64 duration,
         qint64 artworkId);
};

struct TrackReference
{
    qint64 songId = -1;

    TrackReference() = default;
    explicit TrackReference(qint64 songId);
};

struct SongArtwork
{
    qint64 artworkId = -1;
    QString miniCoverPath;
    QString coverPath;

    SongArtwork() = default;
    explicit SongArtwork(qint64 artworkId,
                const QString& miniCoverPath,
                const QString& coverPath);
};

struct SongLite
{
    QString title;
    QString artist;
    qint64 duration = 0;
    QString miniCoverPath;
    SongLite() = default;
    explicit SongLite(const QString& title,
                      const QString& artist,
                      const qint64 duration,
                      const QString& miniCoverPath);
};

#endif // SONG_H