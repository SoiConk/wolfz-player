#ifndef MUSICLOADER_H
#define MUSICLOADER_H

#include <QObject>
#include <QString>
#include <QStringList>
#include <QtQml>

class MusicLoader : public QObject {
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

public:
    explicit MusicLoader(QObject *parent = nullptr);

    Q_INVOKABLE void openFile(const QString &filePath);
    Q_INVOKABLE void openFolder(const QString &folderPath);

signals:
    void fileLoaded(const QString &paths);
    void folderLoaded(const QStringList &paths);
};

#endif // MUSICLOADER_H
