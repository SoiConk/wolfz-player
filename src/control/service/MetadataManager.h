#ifndef METADATAMANAGER_H
#define METADATAMANAGER_H

#include <data/model/Song.h>

#include <QObject>
#include <QHash>
#include <QStringList>
#include <QtSql/QSqlDatabase>

class MetadataManager : public QObject
{
    Q_OBJECT

public:
    static MetadataManager& getInstance();

    void processFile(const QString& file);
    void processFolder(const QStringList& list);

    QString appDataPath() const;
    QString appLocalDataPath() const;

    QString getPathById(qint64 songId);
    QList<qint64> getQueue() const;
    void setQueue(QList<qint64> queue);
    QList<qint64> getHistory() const;
    void setHistory(QList<qint64> history);
    void cleanMissingFiles();
    SongLite getMetadata(qint64 songId) const;
    QString getCoverPath(qint64 songId) const;
    void addSongId(qint64 songId);

signals:
    void songReady(qint64 songId);
    void queueReady(QList<qint64> listId);

private:
    explicit MetadataManager();
    ~MetadataManager();

    void initDatabase();
    bool existsInDB(const QString& path, qint64 &songId);
    qint64 insertSong(const QString& path);

    QString queryPathFromDB(qint64 songId);

    void extractMetadata(const QString& path,
                         QString &title,
                         QString &artist,
                         qint64 &duration);

    qint64 extractArtworks(const QString& path, QSqlDatabase &db);

private:
    MetadataManager(const MetadataManager&) = delete;
    MetadataManager& operator=(const MetadataManager&) = delete;

    QSqlDatabase database;

    QHash<qint64, QString> pathCache;
};

#endif // METADATAMANAGER_H