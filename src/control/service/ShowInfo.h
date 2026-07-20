#ifndef SHOWINFO_H
#define SHOWINFO_H

#include "data/model/Song.h"

#include <QtQml>
#include <QList>
#include <QHash>
#include <QCache>

class ShowInfo : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
private:
    mutable QCache<qint64, SongShowInfo> cache {1000};
    mutable QCache<qint64, AlbumInfo> albumCache {200};

    const SongShowInfo& getOrCreate(qint64 songId) const;
    const AlbumInfo& getOrCreateAlbum(qint64 albumId) const;

    void clearCache();
    void clearAlbumCache();

public:
    explicit ShowInfo(QObject *parent = nullptr);
    Q_INVOKABLE QString title(qint64 songId) const;
    Q_INVOKABLE QString artist(qint64 songId) const;
    Q_INVOKABLE QString duration(qint64 songId) const;
    Q_INVOKABLE QString miniCoverPath(qint64 songId) const;
    Q_INVOKABLE QString coverPath(qint64 songId) const;

    Q_INVOKABLE QString name(qint64 albumId) const;
    Q_INVOKABLE QString durations(qint64 albumId) const;
    Q_INVOKABLE QString albumCoverPath(qint64 albumId) const;

signals:
    void reloadAlbumCache();
};

#endif