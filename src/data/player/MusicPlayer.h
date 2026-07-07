#ifndef MUSICPLAYER_H
#define MUSICPLAYER_H

#include <QObject>
#include <QMediaPlayer>
#include <QAudioOutput>

class MusicPlayer : public QObject
{
    Q_OBJECT

public:
    explicit MusicPlayer(QObject *parent = nullptr);

    void load(const QString &filePath);
    void play();
    void pause();
    void toggle();

    void setVolume(float volume);
    void setPosition(qint64 position);

    qint64 duration() const;
    qint64 position() const;

signals:
    void positionChanged(qint64);
    void durationChanged(qint64);
    void stateChanged(QMediaPlayer::PlaybackState);
    void mediaFinished();

private:
    QMediaPlayer *player;
    QAudioOutput *audioOutput;
};

#endif