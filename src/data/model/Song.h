#ifndef SONG_H
#define SONG_H

#include <QString>
#include <QtGlobal>
#include <QList>

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

struct AlbumInfo
{
    QString name;
    qint64 duration;
    QString albumCoverPath;
    AlbumInfo() = default;
    explicit AlbumInfo(const QString& name,
                       qint64 duration,
                       const QString& albumCoverPath);
};

#endif // SONG_H