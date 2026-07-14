#include "History.h"

History& History::getInstance()
{
    static History instance;
    return instance;
}

void History::add(qint64 songId)
{
    list.removeAll(songId);
    list.prepend(songId);

    if (list.size() > maxSize)
        list.removeLast();
    emit changedHistory(list);
}

void History::addList(QList<qint64> list)
{
    if (list.empty())
        return;
    this->list = list;
    emit changedHistory(list);
}

QList<qint64> History::getAll() const
{
    return list;
}