#ifndef PLAYLISTMODEL_H
#define PLAYLISTMODEL_H

#include "data/playlist/Playlist.h"

#include <QtQml>


class PlaylistModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

    Q_PROPERTY(qint64 playlistId READ playlistId WRITE load NOTIFY playlistChanged)

public:
    enum Roles {
        SongIdRole = Qt::UserRole + 1
    };

    explicit PlaylistModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    QVariant data(const QModelIndex &index, int role) const override;

    QHash<int, QByteArray> roleNames() const override;

    void load(qint64 playlistId);
    void reload();

    const QList<qint64>& getList() const;

    qint64 playlistId() const;

signals:
    void playlistChanged();

private:
    Playlist playlist;

    void fetchFromDB();
};
#endif // PLAYLISTMODEL_H
