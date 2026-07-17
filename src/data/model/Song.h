#ifndef SONG_H
#define SONG_H

#include <QString>
#include <QtGlobal>
#include <QList>

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
                      qint64 duration,
                      const QString& miniCoverPath);
};

struct Album
{
    qint64 albumId;
    QList<qint64> songIdList;
    qint64 totalDuration;
    QString albumCoverPath;
    Album() = default;
    explicit Album(qint64 albumId,
                   const QList<qint64> songIdList,
                   qint64 totalDuration,
                   const QString& albumCoverPath);
};

#endif // SONG_H