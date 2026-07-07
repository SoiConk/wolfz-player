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

    Slider {
        id: topProgressBar
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right
        height: 6
        from: 0
        to: playerController.duration
        value: playerController.position

        onMoved: playerController.setPosition(value)

        background: Rectangle {
            height: 4
            color: "#313244"
            Rectangle {
                width: topProgressBar.visualPosition * parent.width
                height: parent.height
                color: "#f38ba8"
            }
        }
        handle: Rectangle { visible: false }
    }

    RowLayout {
        anchors.fill: parent
        anchors.topMargin: 0
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 0

        RowLayout {
            Layout.fillWidth: true
            Layout.minimumWidth: 120
            spacing: 12
            Layout.alignment: Qt.AlignLeft

            Rectangle {
                id: miniCoverContainer
                width: 45
                height: 45
                radius: 4
                color: "#313244"
                Layout.alignment: Qt.AlignVCenter

                Image {
                    id: miniCoverArt
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectCrop
                    source: "qrc:/image/assets/images/defaultCoverArt.png"
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                spacing: 2
                Layout.alignment: Qt.AlignVCenter

                Text {
                    text: "Unknown"
                    color: "#cdd6f4"
                    font.pixelSize: 20
                    font.bold: true
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
                Text {
                    text: "None"
                    color: "#a6adc8"
                    font.pixelSize: 12
                    font.italic: true
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                }
            }
        }

        RowLayout {
            Layout.fillWidth: false
            Layout.alignment: Qt.AlignCenter
            spacing: 5

            Button {
                text: "⏮"; font.pixelSize: 20; flat: true
                onClicked: playerController.playPrevious()
            }

            Button {
                text: playerController.isPlaying ? "⏸" : "▶"
                font.pixelSize: 30
                flat: true
                onClicked: playerController.togglePlay()
            }

            Button {
                text: "⏭"; font.pixelSize: 20; flat: true
                onClicked: playerController.playNext()
            }

            Text {
                text: playerBarRoot.formatTime(playerController.position) + " / " + playerBarRoot.formatTime(playerController.duration)
                color: "#a6adc8"
                font.pixelSize: 16
                Layout.leftMargin: 5
            }
        }

        RowLayout {
            Layout.fillWidth: false
            Layout.alignment: Qt.AlignRight
            spacing: 2

            Item { Layout.fillWidth: true }

            property bool showVolumeSlider: playerBarRoot.width > 600
            property bool showExtraButtons: playerBarRoot.width > 750

            Button {
                id: loopButton
                flat: true
                font.pixelSize: 20
                text: playerController.loopMode === 1 ? "🔂" : "🔁"
                opacity: playerController.loopMode !== 0 ? 1.0 : 0.4
                onClicked: playerController.cycleLoopMode()
            }

            Button {
                id: muteButton
                flat: true
                font.pixelSize: 20
                text: {
                    let vol = playerController.volume;
                    if (vol === 0) return "🔇";
                    else if (vol < 50) return "🔉";
                    else return "🔊";
                }
                onClicked: playerController.toggleMute()
            }

            Slider {
                id: volumeSlider
                from: 0
                to: 100
                value: playerController.volume
                Layout.preferredWidth: 150
                Layout.leftMargin: 5
                Layout.alignment: Qt.AlignVCenter
                onMoved: playerController.volume = value
                visible: parent.showVolumeSlider
            }

            Button {
                id: moreOptionsButton
                flat: true
                font.pixelSize: 18
                text: "•••"
                Layout.leftMargin: 5
                visible: parent.showExtraButtons
            }
        }
    }
}