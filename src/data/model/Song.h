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

struct SongCover
{
    QString hash;
    QString miniCoverPath;
    QString coverPath;

    SongCover() = default;
    explicit SongCover(const QString& hash,
                const QString& miniCoverPath,
                const QString& coverPath);
};

struct SongInfo
{
    QString title;
    QString artist;
    qint64 duration = 0;
    SongInfo() = default;
    explicit SongInfo(const QString& title,
                      const QString& artist,
                      qint64 duration);
};

struct SongShowInfo
{
    QString title;
    QString artist;
    qint64 duration = 0;
    QString miniCoverPath;
    SongShowInfo() = default;
    explicit SongShowInfo(const QString& title,
                      const QString& artist,
                      qint64 duration,
                      const QString& miniCoverPath);
};

struct Album
{
    qint64 albumId;
    QString title;
    QList<qint64> songIdList;
    qint64 totalDuration;
    QString albumCoverPath;
    Album() = default;
    explicit Album(qint64 albumId,
                   const QList<qint64> title,
                   const QList<qint64> songIdList,
                   qint64 totalDuration,
                   const QString& albumCoverPath);
};

#endif // SONG_H