#ifndef QUEUE_H
#define QUEUE_H

#include <QList>
#include <QObject>

class Queue : public QObject
{
    Q_OBJECT

public:
    static Queue& getInstance();

    const QList<qint64> &getList() const;
    qint64 getId(int index) const;

    void add(qint64 id);
    void addList(const QList<qint64> &listId);

    qint64 current() const;
    bool next();
    bool previous();

    void setLoop(bool isLoop);

    bool isEmpty() const;

    bool setIndex(int i);
    int getIndex() const;

    int size() const;

signals:
    void changedIndex(qint64 index);
    void changed(QList<qint64> list);

private:
    Queue() = default;

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    QList<qint64> list;
    int index = -1;
    bool isLoop = false;
};

#endif