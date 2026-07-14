#ifndef HISTORY_H
#define HISTORY_H

#include <QStringList>
#include <QObjectList>

class History : public QObject
{
    Q_OBJECT

public:
    static History& getInstance();

    void add(qint64 songId);
    void addList(QList<qint64> list);
    QList<qint64> getAll() const;

private:
    History() = default;
    ~History() = default;

    History(const History&) = delete;
    History(History&&) = delete;
    History& operator=(const History&) = delete;
    History& operator=(History&&) = delete;

    QList<qint64> list;
    int maxSize = 10;

signals:
    void changedHistory(QList<qint64> list);
};

#endif