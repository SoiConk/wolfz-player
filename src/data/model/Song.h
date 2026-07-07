#ifndef SONG_H
#define SONG_H

#include <QString>
#include <QImage>

struct Song {
    QString title;
    QString artist;
    QString path;
    qint64 duration = 0;
    QImage cover;

    Song() = default;
    Song(const QString& path,
         const QString& title,
         const QString& artist,
         qint64 duration,
         const QImage& cover);
};

#endif // SONG_H