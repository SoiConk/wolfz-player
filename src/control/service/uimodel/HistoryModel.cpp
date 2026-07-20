#include "HistoryModel.h"

#include "data/playlist/History.h"

HistoryModel::HistoryModel(QObject* parent) : QAbstractListModel(parent)
{
    reload();
    connect(&History::getInstance(), &History::changedHistory, this, &HistoryModel::reload);
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

