#include "History.h"

History& History::getInstance()
{
    static History instance;
    return instance;
}

void History::add(const QString &file)
{
    list.removeAll(file);
    list.prepend(file);

    if (list.size() > maxSize)
        list.removeLast();
}

QStringList History::getAll() const
{
    return list;
}