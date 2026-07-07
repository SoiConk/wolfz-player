#include "Queue.h"
#include <QDir>
#include <QFileInfoList>

const QStringList Queue::getList(){
    return list;
}

void Queue::add(const QString &file)
{
    list.clear();
    list.append(file);
    index = 0;
}

void Queue::addFolder(const QString &folderPath)
{
    QDir dir(folderPath);

    QStringList filters;
    filters << "*.mp3" << "*.wav";

    QFileInfoList files = dir.entryInfoList(filters, QDir::Files, QDir::Name);
    QStringList tempList;

    for (const QFileInfo &file : std::as_const(files)) {
        tempList.append(file.absoluteFilePath());
    }

    if (!tempList.isEmpty()){
        list = tempList;
        index = 0;
    }
}

QString Queue::current() const
{
    if (index < 0 || index >= list.size())
        return "";

    return list[index];
}

QString Queue::next()
{
    if (list.isEmpty()) return "";

    index++;

    if (index >= list.size()) {
        if (loopMode == LoopMode::LoopAll)
            index = 0;
        else {
            index = list.size() - 1;
            return "";
        }
    }

    return current();
}

QString Queue::previous()
{
    if (list.isEmpty()) return "";

    index--;

    if (index < 0) {
        if (loopMode == LoopMode::LoopAll)
            index = list.size() - 1;
        else {
            index = 0;
            return "";
        }
    }

    return current();
}

bool Queue::isEmpty() const
{
    return list.isEmpty();
}

void Queue::setLoopMode(LoopMode mode)
{
    loopMode = mode;
}

LoopMode Queue::getLoopMode() const
{
    return loopMode;
}

void Queue::setIndex(int i)
{
    if (i >= 0 && i < list.size())
        index = i;
}

int Queue::getIndex() const
{
    return index;
}

int Queue::size() const
{
    return list.size();
}