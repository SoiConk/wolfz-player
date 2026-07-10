#include "PlayerController.h"
#include <data/playlist/Queue.h>
#include "data/playlist/History.h"

PlayerController::PlayerController(QObject *parent) : QObject(parent) {
    player = new MusicPlayer(this);

    // Song add
    connect(&Queue::getInstance(), &Queue::changed, this, &PlayerController::playCurrent);

    // Timestamp
    connect(player, &MusicPlayer::positionChanged, this, &PlayerController::positionChanged);

    connect(player, &MusicPlayer::durationChanged, this, &PlayerController::durationChanged);

    // Auto play
    connect(player, &MusicPlayer::mediaFinished, this, &PlayerController::handleEndOfMedia);

    //Toggle play
    connect(player, &MusicPlayer::stateChanged, this, &PlayerController::handlePlayerStateChanged);
}

/*
SongPlay Group
*/
void PlayerController::playPrevious()
{
    if (Queue::getInstance().previous())
        playCurrent();
}

void PlayerController::togglePlay()
{
    int index = currentIndex();
    if (index < 0)
        return;

    player->toggle();
}

void PlayerController::playNext()
{
    if (Queue::getInstance().next())
        playCurrent();
}

void PlayerController::playAt(int index)
{
    if (Queue::getInstance().setIndex(index))
        playCurrent();
}

QString PlayerController::currentSong() const
{
    return Queue::getInstance().getPath(currentIndex());
}

int PlayerController::currentIndex() const
{
    return Queue::getInstance().getIndex();
}

bool PlayerController::isPlayingState() const
{
    return isPlaying;
}


void PlayerController::handlePlayerStateChanged(QMediaPlayer::PlaybackState state)
{
    bool playing = (state == QMediaPlayer::PlayingState);

    if (isPlaying != playing) {
        isPlaying = playing;
        emit playingChanged();
    }
}

void PlayerController::playCurrent()
{
    QString file = Queue::getInstance().current();
    if (file.isEmpty())
        return;
    player->load(file);
    player->play();

    History::getInstance().add(file);
}

void PlayerController::handleEndOfMedia()
{
    if (loopMode == LoopMode::LoopOne) {
        playCurrent();
        return;
    }

    playNext();
}

/*
Timestamp Group
*/
void PlayerController::setPosition(qint64 position)
{
    player->setPosition(position);
}

qint64 PlayerController::position() const
{
    return player->position();
}

qint64 PlayerController::duration() const
{
    return player->duration();
}

/*
Volume Group
*/
void PlayerController::setVolume(int value)
{
    if (volume != value) {
        volume = value;

        float vol = value / 100.0f;
        player->setVolume(vol);

        emit volumeChanged();
    }
}

void PlayerController::toggleMute()
{
    if (volume > 0) {
        previousVolume = volume;
        setVolume(0);
    } else {
        setVolume(previousVolume);
    }
}

int PlayerController::getVolume() const
{
    return volume;
}

/*
Loop Group
*/
void PlayerController::cycleLoopMode()
{
    switch (loopMode) {
    case LoopMode::NoLoop:
        loopMode = LoopMode::LoopAll;
        Queue::getInstance().setLoop(true);
        break;
    case LoopMode::LoopAll:
        loopMode = LoopMode::LoopOne;
        Queue::getInstance().setLoop(false);
        break;
    case LoopMode::LoopOne:
        loopMode = LoopMode::NoLoop;
        Queue::getInstance().setLoop(false);
        break;
    }

    emit loopModeChanged(loopMode);
}

PlayerController::LoopMode PlayerController::getLoopMode() const
{
    return loopMode;
}