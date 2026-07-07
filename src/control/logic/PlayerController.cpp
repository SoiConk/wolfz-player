#include "PlayerController.h"
#include <QFileInfo>
#include <QDir>

PlayerController::PlayerController(QObject *parent)
    : QObject(parent)
{
    player = new MusicPlayer(this);

    resetLoopMode();

    // Timestamp
    connect(player, &MusicPlayer::positionChanged,
            this, &PlayerController::positionChanged);

    connect(player, &MusicPlayer::durationChanged,
            this, &PlayerController::durationChanged);

    // Auto play
    connect(player, &MusicPlayer::mediaFinished,
            this, &PlayerController::handleEndOfMedia);

    //Toggle play
    connect(player, &MusicPlayer::stateChanged,
            this, &PlayerController::handlePlayerStateChanged);
}

/*
OpenPath Group
*/
void PlayerController::openFile(const QString &filePath)
{
    if (filePath.isEmpty())
        return;

    queue.add(filePath);

    emit queueUpdated(queue.getList());
    emit indexChanged(queue.getIndex());

    playCurrent();
}

void PlayerController::openFolder(const QString &folderPath)
{
    if (folderPath.isEmpty())
        return;

    queue.addFolder(folderPath);
    if (queue.isEmpty())
        return;

    emit queueUpdated(queue.getList());
    emit indexChanged(queue.getIndex());

    playCurrent();
}

/*
SongPlay Group
*/
void PlayerController::playPrevious()
{
    QString prev = queue.previous();
    if (prev.isEmpty())
        return;

    playCurrent();
}

void PlayerController::togglePlay()
{
    if (queue.isEmpty())
        return;

    player->toggle();
}

void PlayerController::playNext()
{
    QString next = queue.next();
    if (next.isEmpty())
        return;

    playCurrent();
}

QString PlayerController::currentSong() const
{
    return queue.current();
}

int PlayerController::currentIndex() const
{
    return queue.getIndex();
}

bool PlayerController::isPlaying() const {
    return m_isPlaying;
}


void PlayerController::handlePlayerStateChanged(QMediaPlayer::PlaybackState state)
{
    bool playing = (state == QMediaPlayer::PlayingState);

    if (m_isPlaying != playing) {
        m_isPlaying = playing;
        emit playingChanged();
    }
}

void PlayerController::playCurrent()
{
    QString file = queue.current();
    if (file.isEmpty())
        return;

    player->load(file);
    player->play();

    history.add(file);
    emit historyUpdated(history.getAll());


    emit songChanged(file);
    emit indexChanged(queue.getIndex());
}

void PlayerController::handleEndOfMedia()
{
    if (queue.getLoopMode() == LoopMode::LoopOne) {
        playCurrent();
        return;
    }

    QString next = queue.next();

    if (next.isEmpty())
        return;

    playCurrent();
}

/*
Timestamp Group
*/
void PlayerController::setPosition(qint64 position)
{
    player->setPosition(position);
}

qint64 PlayerController::position() const {
    return player->position();
}

qint64 PlayerController::duration() const {
    return player->duration();
}

/*
Volume Group
*/
void PlayerController::setVolume(int value)
{
    if (m_volume != value) {
        m_volume = value;

        float vol = value / 100.0f;
        player->setVolume(vol);

        emit volumeChanged();
    }
}

void PlayerController::toggleMute()
{
    if (m_volume > 0) {
        m_previousVolume = m_volume;
        setVolume(0);
    } else {
        setVolume(m_previousVolume);
    }
}

int PlayerController::volume() const {
    return m_volume;
}

/*
Loop Group
*/
void PlayerController::resetLoopMode()
{
    queue.setLoopMode(LoopMode::NoLoop);
    emit loopModeChanged(PlayerController::NoLoop);
}

void PlayerController::cycleLoopMode()
{
    LoopMode mode = queue.getLoopMode();

    switch (mode) {
    case LoopMode::NoLoop:  mode = LoopMode::LoopAll; break;
    case LoopMode::LoopAll: mode = LoopMode::LoopOne; break;
    case LoopMode::LoopOne: mode = LoopMode::NoLoop; break;
    }

    queue.setLoopMode(mode);
    emit loopModeChanged(static_cast<PlayerController::QmlLoopMode>(mode));
}

PlayerController::QmlLoopMode PlayerController::loopMode() const {
    return static_cast<QmlLoopMode>(queue.getLoopMode());
}