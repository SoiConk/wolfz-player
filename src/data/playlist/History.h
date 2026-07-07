#ifndef HISTORY_H
#define HISTORY_H

#include <QStringList>

class History
{
public:
    void add(const QString &file);
    QStringList getAll() const;

private:
    QStringList list;
    int maxSize = 10;
};

#endif