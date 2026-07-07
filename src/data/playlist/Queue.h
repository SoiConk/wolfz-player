#ifndef QUEUE_H
#define QUEUE_H

#include "data/model/LoopMode.h"

#include <QStringList>

class Queue
{
public:
    const QStringList getList();
    void add(const QString &file);
    void addFolder(const QString &folderPath);

    QString current() const;
    QString next();
    QString previous();

    bool isEmpty() const;

    void setLoopMode(LoopMode mode);
    LoopMode getLoopMode() const;

    void setIndex(int i);
    int getIndex() const;

    int size() const;

private:
    QStringList list;
    int index = -1;
    LoopMode loopMode = LoopMode::NoLoop;
};

#endif