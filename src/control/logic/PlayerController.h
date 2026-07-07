#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include "data/player/MusicPlayer.h"
#include "data/playlist/Queue.h"
#include "data/playlist/History.h"
#include "data/model/LoopMode.h"

#include <QObject>

class PlayerController : public QObject
{
    Q_OBJECT

    Q_PROPERTY(QString currentSong READ currentSong NOTIFY songChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY indexChanged)
    Q_PROPERTY(bool isPlaying READ isPlaying NOTIFY playingChanged)

    Q_PROPERTY(int volume READ volume WRITE setVolume NOTIFY volumeChanged)

    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)

    Q_PROPERTY(QmlLoopMode loopMode READ loopMode NOTIFY loopModeChanged)

public:
    enum QmlLoopMode {
        NoLoop = static_cast<int>(LoopMode::NoLoop),
        LoopOne = static_cast<int>(LoopMode::LoopOne),
        LoopAll = static_cast<int>(LoopMode::LoopAll)
    };
    Q_ENUM(QmlLoopMode)

    explicit PlayerController(QObject *parent = nullptr);

    /*
    Openpath Group
    */
    Q_INVOKABLE void openFile(const QString &filePath);
    Q_INVOKABLE void openFolder(const QString &folderPath);

    /*
    SongPlaying Group
    */
    Q_INVOKABLE void playPrevious();
    Q_INVOKABLE void togglePlay();
    Q_INVOKABLE void playNext();
    QString currentSong() const;
    int currentIndex() const;
    bool isPlaying() const;

    /*
    Volume Group
    */
    Q_INVOKABLE void setVolume(int value);
    Q_INVOKABLE void toggleMute();
    int volume() const;

    /*
    Timestamp Group
    */
    Q_INVOKABLE void setPosition(qint64 position);
    qint64 position() const;
    qint64 duration() const;

    /*
    Loop Group
    */
    Q_INVOKABLE void resetLoopMode();
    Q_INVOKABLE void cycleLoopMode();
    QmlLoopMode loopMode() const;

signals:
    /*
    SongPlaying Group
    */
    void songChanged(const QString &path);
    void indexChanged(int index);
    void playingChanged();
    void queueUpdated(const QStringList &list);
    void historyUpdated(QStringList list);

    /*
    Volume Group
    */
    void volumeChanged();

    /*
    Timestamp Group
    */
    void positionChanged(qint64);
    void durationChanged(qint64);

    /*
    Loop Group
    */
    void loopModeChanged(PlayerController::QmlLoopMode mode);

private slots:
    void handlePlayerStateChanged(QMediaPlayer::PlaybackState state);

private:
    /*
    SongPlaying Group
    */
    MusicPlayer *player;
    void playCurrent();
    void handleEndOfMedia();
    bool m_isPlaying = false;
    Queue queue;
    History history;

    /*
    Volume Group
    */
    int m_volume = 100;
    int m_previousVolume = 100;

};

#endif