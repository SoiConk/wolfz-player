#include "Queue.h"

Queue& Queue::getInstance()
{
    static Queue instance;
    return instance;
}

const QStringList &Queue::getList() const
{
    return list;
}

const QString &Queue::getPath(int index) const {
    return list[index];
}

void Queue::add(const QString &path)
{
    list = { path };
    index = 0;

    emit changed();
}

void Queue::addFolder(const QStringList &listPath)
{
    if (listPath == list)
        return;
    list = listPath;
    index = 0;

    emit changed();
}

QString Queue::current() const
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
            return true;
        } else {
            return false;
        }
    }

    index--;
    return true;
}

bool Queue::next()
{
    if (index < 0) return false;

    if (index == list.size() - 1) {
        if (isLoop) {
            index = 0;
            return true;
        } else {
            return false;
        }
    }

    index++;
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