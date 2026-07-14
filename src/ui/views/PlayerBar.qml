import Blueberry_Wolfz 1.0

import "qrc:/Blueberry_Wolfz/src/ui/components"

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: playerBarRoot
    color: "#11111b"

    function formatTime(ms) {
            if (ms <= 0) return "00:00";
            let totalSeconds = Math.floor(ms / 1000);
            let minutes = Math.floor(totalSeconds / 60);
            let seconds = totalSeconds % 60;
            return String(minutes).padStart(2, '0') + ":" + String(seconds).padStart(2, '0');
    }
    property bool isPlaying: PlayerController.currentSong !== -1

    ProgressBar {
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 6
        from: 0
        to: PlayerController.duration
        value: PlayerController.position

        updateOnDrag: false

        onReleased: (newValue) => PlayerController.setPosition(newValue)
    }

    RowLayout {
        anchors.fill: parent
        anchors.topMargin: 0
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 0

        RowLayout {
            Layout.fillWidth: true
            spacing: 12
            Layout.alignment: Qt.AlignLeft

            property bool showText: playerBarRoot.width > 550
            Layout.minimumWidth: showText ? 120 : 60
            Layout.preferredWidth: playerBarRoot.width * 0.4
            Layout.maximumWidth: playerBarRoot.width * 0.4

            Rectangle {
                id: miniCoverContainer
                width: 45
                height: 45
                radius: 4
                color: "#313244"
                Layout.alignment: Qt.AlignVCenter

                ImageRounded {
                    size: 45
                    source: ShowInfo.miniCoverPath(PlayerController.currentSong)
                            || "qrc:/Blueberry_Wolfz/src/ui/assets/images/defaultCoverArt.png"
                    radius: 4
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                Layout.alignment: Qt.AlignVCenter
                visible: parent.showText

                Text {
                    text: playerBarRoot.isPlaying ? ShowInfo.title(PlayerController.currentSong)
                                    : "Unknown"
                    color: "#cdd6f4"
                    font.pixelSize: 24
                    font.bold: true
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
                Text {
                    text: playerBarRoot.isPlaying ? ShowInfo.artist(PlayerController.currentSong)
                                    : "None"
                    color: "#a6adc8"
                    font.pixelSize: 14
                    font.italic: true
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
            }
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.alignment: Qt.AlignLeft
            spacing: 5

            IconButton {
                iconSource: "qrc:/Blueberry_Wolfz/src/ui/assets/icons/buttonPrevious.svg";
                onClicked: PlayerController.playPrevious()
            }

            IconButton {
                iconSource: PlayerController.isPlaying ? "qrc:/Blueberry_Wolfz/src/ui/assets/icons/buttonPause.svg"
                                                       : "qrc:/Blueberry_Wolfz/src/ui/assets/icons/buttonPlay.svg"
                iconSize: 40
                onClicked: PlayerController.togglePlay()
            }

            IconButton {
                iconSource: "qrc:/Blueberry_Wolfz/src/ui/assets/icons/buttonNext.svg"
                onClicked: PlayerController.playNext()
            }

            Text {
                text: playerBarRoot.formatTime(PlayerController.position)
                      + " / " + playerBarRoot.formatTime(PlayerController.duration)
                color: "White"
                font.pixelSize: 16
                Layout.leftMargin: 5
            }
        }

        Item { Layout.fillWidth: true }

        RowLayout {
            Layout.alignment: Qt.AlignRight
            spacing: 2

            Item { Layout.fillWidth: true }

            property bool showVolumeSlider: playerBarRoot.width > 680
            property bool showExtraButton: playerBarRoot.width > 750

            IconButton {
                iconSource: PlayerController.loopMode === 0 ? "qrc:/Blueberry_Wolfz/src/ui/assets/icons/buttonNoLoop.svg"
                    : PlayerController.loopMode === 1 ? "qrc:/Blueberry_Wolfz/src/ui/assets/icons/buttonLoopOne.svg"
                                                      : "qrc:/Blueberry_Wolfz/src/ui/assets/icons/buttonLoopAll.svg"

                onClicked: PlayerController.cycleLoopMode()
            }

            IconButton {
                iconSource: {
                    let vol = PlayerController.volume;
                    if (vol === 0) return "qrc:/Blueberry_Wolfz/src/ui/assets/icons/volumeMuted.svg";
                    else if (vol < 50) return "qrc:/Blueberry_Wolfz/src/ui/assets/icons/volumeMid.svg";
                    else return "qrc:/Blueberry_Wolfz/src/ui/assets/icons/volumeFull.svg";
                }
                onClicked: PlayerController.toggleMute()
            }

            ProgressBar {
                id: volumeSlider
                Layout.preferredWidth: 150
                Layout.alignment: Qt.AlignVCenter
                Layout.fillWidth: false
                visible: parent.showVolumeSlider

                from: 0
                to: 100
                value: PlayerController.volume

                alwaysShowHandle: true
                updateOnDrag: true

                onMoved: (newValue) => PlayerController.volume = newValue
                onReleased: (newValue) => PlayerController.volume = newValue
            }

            IconButton {
                iconSource: "qrc:/Blueberry_Wolfz/src/ui/assets/icons/buttonMore.svg"
                Layout.leftMargin: 8
                visible: parent.showExtraButton
            }
        }
    }
}