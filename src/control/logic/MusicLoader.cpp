#include "MusicLoader.h"
#include <data/playlist/Queue.h>

#include <QFileInfo>
#include <QDir>

MusicLoader::MusicLoader(QObject *parent) :QObject(parent)
{
    connect(this, &MusicLoader::fileLoaded, &Queue::getInstance(), &Queue::add);

    connect(this, &MusicLoader::folderLoaded, &Queue::getInstance(), &Queue::addFolder);
}

void MusicLoader::openFile(const QString &filePath)
{
    if (filePath.isEmpty())
        return;
    emit fileLoaded(filePath);
}

void MusicLoader::openFolder(const QString &folderPath)
{
    if (folderPath.isEmpty())
        return;

    QDir directory(folderPath);

    QStringList filters;
    filters << "*.mp3" << "*.wav";

    QFileInfoList files = directory.entryInfoList(filters, QDir::Files, QDir::Name);

    QStringList tempList;

    for (const QFileInfo &file : std::as_const(files)) {
        tempList.append(file.absoluteFilePath());
    }

    if (tempList.isEmpty()){
        return;
    }
    emit folderLoaded(tempList);
}