#include "MusicLoader.h"
#include <data/playlist/Queue.h>
#include <control/service/MetadataManager.h>

#include <QFileInfo>
#include <QDir>
#include <QStandardPaths>
#include <QHash>
#include <QCollator>



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

    QStringList filters = {"*.mp3", "*.flac", "*.wav", "*.m4a", "*.mp4", "*.wma", "*.asf"};;

    QFileInfoList files = directory.entryInfoList(filters, QDir::Files);

    QStringList tempList;

    QFileInfo bestFile;
    QString currentBase;

    QCollator collator;
    collator.setNumericMode(true);

    std::sort(files.begin(), files.end(), [&collator](const QFileInfo &a, const QFileInfo &b) {
        return collator.compare(a.completeBaseName(), b.completeBaseName()) < 0;
    });

    for (const QFileInfo &file : std::as_const(files)) {
        QString base = file.completeBaseName();

        if (base != currentBase) {
            if (!bestFile.fileName().isEmpty())
                tempList.append(bestFile.absoluteFilePath());

            bestFile = file;
            currentBase = base;
        } else {
            if (rankFilter(file.suffix()) > rankFilter(bestFile.suffix())) {
                bestFile = file;
            }
        }
    }

    if (!bestFile.fileName().isEmpty())
        tempList.append(bestFile.absoluteFilePath());

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

int MusicLoader::rankFilter(const QString& suffix)
{
    static const QHash<QString, int> rank = {
        {"flac", 4},
        {"m4a",  3},
        {"mp4", 3},
        {"wav", 2},
        {"mp3",  1},
        {"wma", 0},
        {"asf",  0}
    };

    return rank.value(suffix.toLower(), 0);
}

void MusicLoader::openPlaylist(qint64 playlistId, int index)
{
    if (playlistId < 0) return;
    Queue& instance = Queue::getInstance();
    QList<qint64> list = MetadataManager::getInstance().getSongList(playlistId);

    if (list == instance.getList() && index == instance.getIndex()) {
        return;
    }

    if (list != instance.getList()) {
        instance.addList(list);
    }

    instance.setIndex(index);
}