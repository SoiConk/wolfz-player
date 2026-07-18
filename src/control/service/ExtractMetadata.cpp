#include "ExtractMetadata.h"

#include <QFileInfo>
#include <QStandardPaths>
#include <QDir>

#include <QImage>
#include <QCryptographicHash>

#include <taglib.h>
#include <taglib/fileref.h>
#include <taglib/audioproperties.h>

#include <taglib/mpegfile.h>
#include <taglib/id3v2tag.h>
#include <taglib/attachedpictureframe.h>

#include <taglib/flacfile.h>
#include <taglib/flacpicture.h>

#include <taglib/mp4file.h>
#include <taglib/mp4tag.h>
#include <taglib/mp4item.h>

#include <taglib/asffile.h>
#include <taglib/asftag.h>
#include <taglib/asfpicture.h>

ExtractMetadata::ExtractMetadata() {}

SongInfo ExtractMetadata::getInfo(const QString& path)
{
#if defined(Q_OS_WIN)
    TagLib::FileRef f(path.toStdWString().c_str());
#else
    TagLib::FileRef f(QFile::encodeName(path).constData());
#endif
    SongInfo song("","Unknown",0);

    if (!f.isNull() && f.tag()) {
        TagLib::Tag *tag = f.tag();
        if (!tag->title().isEmpty()) {
            song.title = QString::fromUtf8(tag->title().to8Bit(true).c_str());
        }

        if (!tag->artist().isEmpty()) {
            song.artist = QString::fromUtf8(tag->artist().to8Bit(true).c_str());
        }

        if (f.audioProperties()) {
            song.duration = static_cast<qint64>(f.audioProperties()->lengthInMilliseconds());
        }
    }

    if (song.title.isEmpty()) {
        song.title = QFileInfo(path).completeBaseName();
    }

    return song;
}

SongCover ExtractMetadata::getCover(const QString& path)
{
    QString tail = QFileInfo(path).suffix().toLower();
    QByteArray imageData;

    if (tail == "mp3")
        return hashSave(handleMP3Cover(path));
    if (tail == "flac")
        return hashSave(handleFLACCover(path));
    if (tail == "m4a" || tail == "mp4")
        return hashSave(handleMP4Cover(path));
    if (tail == "wma" || tail == "asf")
        return hashSave(handleASFCover(path));
    return {};
}

QByteArray ExtractMetadata::handleMP3Cover(const QString& path) {
#if defined(Q_OS_WIN)
    TagLib::MPEG::File mpegFile(path.toStdWString().c_str());
#else
    TagLib::MPEG::File mpegFile(QFile::encodeName(path).constData());
#endif
    QByteArray imageData;
    if (mpegFile.isValid() && mpegFile.ID3v2Tag()) {
        TagLib::ID3v2::Tag *tag = mpegFile.ID3v2Tag();
        TagLib::ID3v2::FrameList frames = tag->frameListMap()["APIC"];
        if (!frames.isEmpty()) {
            for (TagLib::ID3v2::Frame *frame : frames) {
                TagLib::ID3v2::AttachedPictureFrame *picFrame = dynamic_cast<TagLib::ID3v2::AttachedPictureFrame*>(frame);
                if (picFrame) {
                    if (picFrame->type() == TagLib::ID3v2::AttachedPictureFrame::FrontCover) {
                        imageData = QByteArray(picFrame->picture().data(), picFrame->picture().size());
                        break;
                    }
                    if (imageData.isEmpty()) {
                        imageData = QByteArray(picFrame->picture().data(), picFrame->picture().size());
                        if (!imageData.isEmpty())
                            break;
                    }
                }
            }
        }
    }
    return imageData;
}

QByteArray ExtractMetadata::handleFLACCover(const QString& path) {
#if defined(Q_OS_WIN)
    TagLib::FLAC::File flacFile(path.toStdWString().c_str());
#else
    TagLib::FLAC::File flacFile(QFile::encodeName(path).constData());
#endif
    QByteArray imageData;
    if (flacFile.isValid()) {
        TagLib::List<TagLib::FLAC::Picture*> pictureList = flacFile.pictureList();
        if (!pictureList.isEmpty()) {
            for (TagLib::FLAC::Picture *pic : pictureList) {
                if (pic) {
                    if (pic->type() == TagLib::FLAC::Picture::FrontCover) {
                        imageData = QByteArray(pic->data().data(), pic->data().size());
                        break;
                    }
                    if (imageData.isEmpty()) {
                        imageData = QByteArray(pic->data().data(), pic->data().size());
                        if (!imageData.isEmpty())
                            break;
                    }
                }
            }
        }
    }
    return imageData;
}

QByteArray ExtractMetadata::handleMP4Cover(const QString& path) {
#if defined(Q_OS_WIN)
    TagLib::MP4::File mp4File(path.toStdWString().c_str());
#else
    TagLib::MP4::File mp4File(QFile::encodeName(path).constData());
#endif
    QByteArray imageData;
    if (mp4File.isValid() && mp4File.tag()) {
        TagLib::MP4::Tag *tag = mp4File.tag();
        const TagLib::MP4::ItemMap &itemMap = tag->itemMap();
        if (itemMap.contains("covr")) {
            TagLib::MP4::Item coverItem = itemMap["covr"];
            TagLib::MP4::CoverArtList coverList = coverItem.toCoverArtList();
            if (!coverList.isEmpty()) {
                for (const TagLib::MP4::CoverArt &cover : coverList) {
                    imageData = QByteArray(cover.data().data(), cover.data().size());
                    break;
                }
            }
        }
    }
    return imageData;
}

QByteArray ExtractMetadata::handleASFCover(const QString& path)
{
#if defined(Q_OS_WIN)
    TagLib::ASF::File asfFile(path.toStdWString().c_str());
#else
    TagLib::ASF::File asfFile(QFile::encodeName(path).constData());
#endif
    QByteArray imageData;
    if (asfFile.isValid() && asfFile.tag()) {
        TagLib::ASF::Tag *tag = asfFile.tag();
        TagLib::ASF::AttributeList attributeList = tag->attributeListMap()["WM/Picture"];
        if (!attributeList.isEmpty()) {
            for (const TagLib::ASF::Attribute &attr : attributeList) {
                TagLib::ASF::Picture asfPic = attr.toPicture();
                if (asfPic.isValid() && !asfPic.picture().isEmpty()) {
                    if (asfPic.type() == TagLib::ASF::Picture::FrontCover) {
                        imageData = QByteArray(asfPic.picture().data(), asfPic.picture().size());
                        break;
                    }
                    if (imageData.isEmpty()) {
                        imageData = QByteArray(asfPic.picture().data(), asfPic.picture().size());
                        if (!imageData.isEmpty())
                            break;
                    }
                }
            }
        }
    }
    return imageData;
}

SongCover ExtractMetadata::hashSave(const QByteArray& imageData)
{
    SongCover cover{};
    QImage img;
    if (!img.loadFromData(imageData)) {
        qDebug() << "Image size:" << imageData.size();
        return cover;
    }

    QString hash = QCryptographicHash::hash(imageData, QCryptographicHash::Md5).toHex();
    cover.hash = hash;

    QString coverDir = appLocalDataPath() + "/covers/full/";
    QString miniDir = appLocalDataPath() + "/covers/mini/";

    QDir().mkpath(coverDir);
    QDir().mkpath(miniDir);

    QString coverPath = coverDir + hash + "_full.jpg";
    QString miniCoverPath = miniDir + hash + "_mini.jpg";
    cover.coverPath = coverPath;
    cover.miniCoverPath = miniCoverPath;

    if (QFile::exists(coverPath) && QFile::exists(miniCoverPath)) {
        return cover;
    }

    qint64 size = img.width() < img.height() ? img.width() : img.height();
    quint64 resize = size < 832 ? size : 832;
    QImage mainCover = img.scaled(resize, resize, Qt::KeepAspectRatio, Qt::SmoothTransformation);
    QImage miniCover = img.scaled(60, 60, Qt::KeepAspectRatio, Qt::SmoothTransformation);

    mainCover.save(coverPath, "JPG", 85);
    miniCover.save(miniCoverPath, "JPG", 75);

    return cover;
}

QString ExtractMetadata::appLocalDataPath() const
{
    return QStandardPaths::writableLocation(QStandardPaths::AppLocalDataLocation);
}