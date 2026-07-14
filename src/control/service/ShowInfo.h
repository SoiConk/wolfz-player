#ifndef SHOWINFO_H
#define SHOWINFO_H

#include "data/model/Song.h"

#include <QObject>
#include <QtQml>
#include <QList>
#include <QHash>

class QueueModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    enum Roles {
        SongIdRole = Qt::UserRole + 1
    };

    explicit QueueModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void reload();

private:
    QList<qint64> list;
};

class HistoryModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    enum Roles {
        SongIdRole = Qt::UserRole + 1
    };

    explicit HistoryModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void reload();

private:
    QList<qint64> list;
};

class ShowInfo : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON
private:
    struct Meta {
        QString title;
        QString artist;
        int duration;
        QString miniCoverPath;
    };
    mutable QHash<qint64, SongLite> cache;

    const SongLite& getOrCreate(qint64 songId) const;

public:
    explicit ShowInfo(QObject *parent = nullptr);
    Q_INVOKABLE QString title(qint64 songId) const;
    Q_INVOKABLE QString artist(qint64 songId) const;
    Q_INVOKABLE QString duration(qint64 songId) const;
    Q_INVOKABLE QString miniCoverPath(qint64 songId) const;
    Q_INVOKABLE QString coverPath(qint64 songId) const;
};

#endif