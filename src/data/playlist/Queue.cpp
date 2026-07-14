#include "Queue.h"

Queue& Queue::getInstance()
{
    static Queue instance;
    return instance;
}

const QList<qint64> &Queue::getList() const
{
    return list;
}

qint64 Queue::getId(int index) const {
    return list[index];
}

void Queue::add(qint64 id)
{
    list = { id };
    index = 0;

    emit changedIndex(index);
    emit changed(list);
}

void Queue::addList(const QList<qint64> &listId)
{
    if (listId == list)
        return;
    list = listId;
    index = 0;

    emit changedIndex(index);
    emit changed(list);
}

qint64 Queue::current() const
{
    if (index < 0 || index >= list.size())
        return {};
    return list[index];
}

bool Queue::previous()
{
    if (index < 0) return false;

    if (index == 0) {
        if (isLoop) {
            index = list.size() - 1;
            emit changedIndex(index);
            return true;
        } else {
            return false;
        }
    }

    index--;
    emit changedIndex(index);
    return true;
}

bool Queue::next()
{
    if (index < 0) return false;

    if (index == list.size() - 1) {
        if (isLoop) {
            index = 0;
            emit changedIndex(index);
            return true;
        } else {
            return false;
        }
    }

    index++;
    emit changedIndex(index);
    return true;
}

void Queue::setLoop(bool isLoop)
{
    this->isLoop = isLoop;
}

bool Queue::isEmpty() const
{
    return list.isEmpty();
}

bool Queue::setIndex(int i)
{
    if (i == index)
        return false;
    index = i;
    emit changedIndex(index);
    return true;
}

int Queue::getIndex() const
{
    return index;
}

int Queue::size() const
{
    return list.size();
}