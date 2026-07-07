#include "MusicPlayer.h"
#include <QUrl>

MusicPlayer::MusicPlayer(QObject *parent)
    : QObject(parent)
{
    player = new QMediaPlayer(this);
    audioOutput = new QAudioOutput(this);

    player->setAudioOutput(audioOutput);

    connect(player, &QMediaPlayer::positionChanged,
            this, &MusicPlayer::positionChanged);

    connect(player, &QMediaPlayer::durationChanged,
            this, &MusicPlayer::durationChanged);

    connect(player, &QMediaPlayer::playbackStateChanged,
            this, &MusicPlayer::stateChanged);

    connect(player, &QMediaPlayer::mediaStatusChanged, this,
            [this](QMediaPlayer::MediaStatus status){
                if (status == QMediaPlayer::EndOfMedia)
                    emit mediaFinished();
            });
}

void MusicPlayer::load(const QString &filePath)
{
    player->setSource(QUrl::fromLocalFile(filePath));
    player->setPosition(0);
}

void MusicPlayer::play()
{
    player->play();
}

void MusicPlayer::pause()
{
    player->pause();
}

void MusicPlayer::toggle()
{
    if (player->playbackState() == QMediaPlayer::PlayingState)
        pause();
    else
        play();
}

void MusicPlayer::setVolume(float volume)
{
    volume = std::clamp(volume, 0.0f, 1.0f);
    audioOutput->setVolume(volume);
}

void MusicPlayer::setPosition(qint64 position)
{
    player->setPosition(position);
}

qint64 MusicPlayer::duration() const
{
    return player->duration();
}

qint64 MusicPlayer::position() const
{
    return player->position();
}