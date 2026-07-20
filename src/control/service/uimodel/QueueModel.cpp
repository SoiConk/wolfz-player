#include "QueueModel.h"

#include "data/playlist/Queue.h"

QueueModel::QueueModel(QObject* parent) : QAbstractListModel(parent)
{
    qDebug() << "QueueModel created";
    reload();
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
