#include "History.h"

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