#include "MusicLoader.h"
#include <data/playlist/Queue.h>
#include <control/service/MetadataManager.h>

#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>

MusicLoader::MusicLoader(QObject *parent) :QObject(parent)
{
    folder = QUrl::fromLocalFile(QStandardPaths::writableLocation(QStandardPaths::MusicLocation));

    connect(&MetadataManager::getInstance(), &MetadataManager::songReady, &Queue::getInstance(), &Queue::add);
    connect(&MetadataManager::getInstance(), &MetadataManager::queueReady, &Queue::getInstance(), &Queue::addList);
}

void MusicLoader::openFile(const QString &filePath)
{
    if (filePath.isEmpty())
        return;

    MetadataManager::getInstance().processFile(filePath);
}

void MusicLoader::openFolder(const QString &folderPath)
{
    if (folderPath.isEmpty())
        return;

    QDir directory(folderPath);

    folder = QUrl::fromLocalFile(directory.absolutePath());
    emit lastFolderChanged();

    QStringList filters;
    filters << "*.mp3";

    QFileInfoList files = directory.entryInfoList(filters, QDir::Files, QDir::Name);

    QStringList tempList;

    for (const QFileInfo &file : std::as_const(files)) {
        tempList.append(file.absoluteFilePath());
    }

    if (tempList.isEmpty()){
        return;
    }

    MetadataManager::getInstance().processFolder(tempList);
}

QUrl MusicLoader::lastFolder() const
{
    return folder;
}

void MusicLoader::loadId(qint64 songId)
{
    MetadataManager::getInstance().addSongId(songId);
}