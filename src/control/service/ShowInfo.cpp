#include "control/service/MetadataManager.h"
#include "data/playlist/Queue.h"
#include "data/playlist/History.h"
#include "ShowInfo.h"

#include <QDebug>

QueueModel::QueueModel(QObject* parent) : QAbstractListModel(parent)
{
    qDebug() << "QueueModel created";
    reload();
    connect(&Queue::getInstance(), &Queue::changed, &MetadataManager::getInstance(), &MetadataManager::setQueue);
    connect(&Queue::getInstance(), &Queue::changed, this, &QueueModel::reload);
}

int QueueModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return list.size();
}

QVariant QueueModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= list.size())
        return {};

    if (role == SongIdRole)
        return QVariant::fromValue(list[index.row()]);

    return {};
}


QHash<int, QByteArray> QueueModel::roleNames() const
{
    return {
        {SongIdRole, "songId"}
    };
}


void QueueModel::reload()
{
    QList<qint64> newList = Queue::getInstance().getList();

    beginResetModel();

    list = newList;

    endResetModel();
}

HistoryModel::HistoryModel(QObject* parent) : QAbstractListModel(parent)
{
    reload();
    connect(&History::getInstance(), &History::changedHistory, this, &HistoryModel::reload);
    connect(&History::getInstance(), &History::changedHistory, &MetadataManager::getInstance(), &MetadataManager::setHistory);
}

int HistoryModel::rowCount(const QModelIndex& parent) const
{
    if (parent.isValid())
        return 0;

    return list.size();
}

QVariant HistoryModel::data(const QModelIndex& index, int role) const
{
    if (!index.isValid() || index.row() >= list.size())
        return {};

    if (role == SongIdRole)
        return QVariant::fromValue(list[index.row()]);

    return {};
}


QHash<int, QByteArray> HistoryModel::roleNames() const
{
    return {
        {SongIdRole, "songId"}
    };
}


void HistoryModel::reload()
{
    QList<qint64> newList = History::getInstance().getAll();

    beginResetModel();

    list = newList;

    endResetModel();
}

ShowInfo::ShowInfo(QObject* parent) : QObject(parent)
{
}

const SongLite& ShowInfo::getOrCreate(qint64 songId) const {
    if (!cache.contains(songId))
    {
        cache.insert(songId, MetadataManager::getInstance().getMetadata(songId));
    }
    return cache[songId];
}

QString ShowInfo::title(qint64 songId) const
{
    return getOrCreate(songId).title;
}

QString ShowInfo::artist(qint64 songId) const
{
    return getOrCreate(songId).artist;
}

QString ShowInfo::duration(qint64 songId) const
{
    qint64 ms = getOrCreate(songId).duration;
    qint64 totalSeconds = ms / 1000;

    qint64 minutes = totalSeconds / 60;
    qint64 seconds = totalSeconds % 60;

    return QString("%1:%2")
        .arg(minutes)
        .arg(seconds, 2, 10, QChar('0'));
}

QString ShowInfo::miniCoverPath(qint64 songId) const
{
    QString path = getOrCreate(songId).miniCoverPath;
    return !path.isEmpty() ? "file:///" + path : "";
}

QString ShowInfo::coverPath(qint64 songId) const
{
    QString path = MetadataManager::getInstance().getCoverPath(songId);
    return !path.isEmpty() ? "file:///" + path : "";
}