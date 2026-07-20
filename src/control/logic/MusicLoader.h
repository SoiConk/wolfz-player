#ifndef MUSICLOADER_H
#define MUSICLOADER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtQml>

enum class AudioFormat : int {
    Unknown = 0,
    WMA  = 1,
    MP3  = 2,
    M4A  = 3,
    FLAC = 4
};

class MusicLoader : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(QUrl lastFolder READ lastFolder NOTIFY lastFolderChanged)

public:
    explicit MusicLoader(QObject *parent = nullptr);

    Q_INVOKABLE void openFile(const QString &filePath);
    Q_INVOKABLE void openFolder(const QString &folderPath);

    QUrl lastFolder() const;
    Q_INVOKABLE void loadId(qint64 songId);
    Q_INVOKABLE void openPlaylist(qint64 playlistId, int index);

signals:
    void fileLoaded(const QString &paths);
    void folderLoaded(const QStringList &paths);
    void lastFolderChanged();

private:
    int rankFilter(const QString& suffix);
    QStringList filterList(const QFileInfoList& files);
private:
    QUrl folder;
};

#endif // MUSICLOADER_H
