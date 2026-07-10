#ifndef PLAYERCONTROLLER_H
#define PLAYERCONTROLLER_H

#include <data/player/MusicPlayer.h>

#include <QObject>
#include <QtQml>

class PlayerController : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    QML_SINGLETON

    Q_PROPERTY(bool isPlaying READ isPlayingState NOTIFY playingChanged)

    Q_PROPERTY(int volume READ getVolume WRITE setVolume NOTIFY volumeChanged)

    Q_PROPERTY(qint64 position READ position WRITE setPosition NOTIFY positionChanged)
    Q_PROPERTY(qint64 duration READ duration NOTIFY durationChanged)

    Q_PROPERTY(LoopMode loopMode READ getLoopMode NOTIFY loopModeChanged)

public:
    enum class LoopMode {
        NoLoop,
        LoopOne,
        LoopAll
    };
    Q_ENUM(LoopMode)

    explicit PlayerController(QObject *parent = nullptr);

    /*
    SongPlaying Group
    */
    Q_INVOKABLE void playPrevious();
    Q_INVOKABLE void togglePlay();
    Q_INVOKABLE void playNext();
    Q_INVOKABLE void playAt(int index);
    QString currentSong() const;
    int currentIndex() const;
    bool isPlayingState() const;

    /*
    Volume Group
    */
    Q_INVOKABLE void setVolume(int value);
    Q_INVOKABLE void toggleMute();
    int getVolume() const;

    /*
    Timestamp Group
    */
    Q_INVOKABLE void setPosition(qint64 position);
    qint64 position() const;
    qint64 duration() const;

    /*
    Loop Group
    */
    Q_INVOKABLE void cycleLoopMode();
    LoopMode getLoopMode() const;

signals:
    /*
    SongPlaying Group
    */
    void playingChanged();

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
    void loopModeChanged(PlayerController::LoopMode loopMode);

private slots:
    void handlePlayerStateChanged(QMediaPlayer::PlaybackState state);

private:
    /*
    SongPlaying Group
    */
    MusicPlayer *player;
    void playCurrent();
    void handleEndOfMedia();
    bool isPlaying = false;

    /*
    Volume Group
    */
    int volume = 100;
    int previousVolume = 100;

    /*
    Loop Group
    */
    LoopMode loopMode = LoopMode::NoLoop;

};

#endif