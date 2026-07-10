#ifndef HISTORY_H
#define HISTORY_H

#include <QStringList>

class History
{
public:
    static History& getInstance();

    void add(const QString &file);
    QStringList getAll() const;

private:
    History() = default;

    History(const History&) = delete;
    History& operator=(const History&) = delete;

    QStringList list;
    int maxSize = 10;
};

#endif