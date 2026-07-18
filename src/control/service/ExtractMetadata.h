#ifndef EXTRACTMETADATA_H
#define EXTRACTMETADATA_H

#include <data/model/Song.h>

#include <QString>
#include <QByteArray>

class ExtractMetadata
{
public:
    ExtractMetadata();
    SongInfo getInfo(const QString& path);
    SongCover getCover(const QString& path);
private:
    QString appLocalDataPath() const;

    QByteArray handleMP3Cover(const QString& path);
    QByteArray handleFLACCover(const QString& path);
    QByteArray handleMP4Cover(const QString& path);
    QByteArray handleASFCover(const QString& path);
    SongCover hashSave(const QByteArray& imageData);
};

#endif // EXTRACTMETADATA_H
