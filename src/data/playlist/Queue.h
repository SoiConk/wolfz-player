#ifndef QUEUE_H
#define QUEUE_H

#include <QStringList>
#include <QObject>

class Queue : public QObject
{
    Q_OBJECT

public:
    static Queue& getInstance();

    const QStringList &getList() const;
    const QString &getPath(int index) const;

    void add(const QString &path);
    void addFolder(const QStringList &folderPath);

    QString current() const;
    bool next();
    bool previous();

    void setLoop(bool isLoop);

    bool isEmpty() const;

    bool setIndex(int i);
    int getIndex() const;

    int size() const;

signals:
    void changed();

private:
    Queue() = default;

    Queue(const Queue&) = delete;
    Queue& operator=(const Queue&) = delete;

    QStringList list;
    int index = -1;
    bool isLoop = false;
};

#endif