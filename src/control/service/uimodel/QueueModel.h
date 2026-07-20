#ifndef QUEUEMODEL_H
#define QUEUEMODEL_H

#include <QtQml>

class QueueModel : public QAbstractListModel
{
    Q_OBJECT
    QML_ELEMENT

public:
    enum Roles {
        SongIdRole = Qt::UserRole + 1
    };

    explicit QueueModel(QObject* parent = nullptr);

    int rowCount(const QModelIndex& parent = QModelIndex()) const override;

    QVariant data(const QModelIndex& index, int role = Qt::DisplayRole) const override;

    QHash<int, QByteArray> roleNames() const override;

    void reload();

private:
    QList<qint64> list;
};

#endif // QUEUEMODEL_H
