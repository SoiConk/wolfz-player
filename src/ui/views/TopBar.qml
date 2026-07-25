import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import QtCore
import QtQuick.Effects

Rectangle {
    id: topBarRoot
    color: Theme.surface

    signal switchToHistory()
    // Select file or folder dialog
    FileDialog {
        id: fileDialog
        title: "Select Audio file"
        currentFolder: MusicLoader.lastFolder
        nameFilters: [
            "Audio files (*.mp3 *.flac *.wav *.m4a *.mp4 *.wma *.asf)",
            "MP3 files (*.mp3)",
            "FLAC files (*.flac)",
            "WAV files (*.wav)",
            "M4A files (*.m4a *.mp4)",
            "ASF files (*.wma *.asf)"
        ]
        onAccepted: {
            var path = selectedFile.toString();
            if (path.startsWith("file:///")) {
                path = path.substring(8);
            }
            MusicLoader.openFile(path);
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Select Audio folder"
        currentFolder: MusicLoader.lastFolder
        onAccepted: {
            var path = selectedFolder.toString();
            if (path.startsWith("file:///")) {
                path = path.substring(8);
            }
            MusicLoader.openFolder(path);
            switchToHistory();
        }
    }

    // UI
    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 15

        // App Logo and Name
        RowLayout {
            spacing: 10
            Rectangle {
                width: 30
                height: 30
                radius: 12
                color: "transparent"

                Image {
                    id: appLogo
                    sourceSize.width: 30
                    sourceSize.height: 30
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectCrop
                    source: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/images/appLogo.png"
                    smooth: true
                }
            }
            Text {
                text: "Blueberry Wolfz"
                color: "#8195ea"
                font.pixelSize: 16
                font.bold: true
            }
        }

        // Item for spacer
        Item { Layout.fillWidth: true }

        RowLayout {
            spacing: 12

            // Select File Button
            Button {
                id: btnSelectFile
                flat: true
                implicitHeight: 32

                contentItem: Text {
                    text: "Select File"
                    color: btnSelectFile.hovered ? Theme.accent : Theme.text
                    font.pixelSize: 13
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                background: Rectangle {
                    color: btnSelectFile.pressed ? Theme.border : (btnSelectFile.hovered ? Theme.surfaceHover : Theme.background)
                    border.color: Theme.border
                    border.width: 1
                    radius: 6
                }

                onClicked: fileDialog.open()
            }

            // Select Folder Button
            Button {
                id: btnSelectFolder
                flat: true
                implicitHeight: 32

                contentItem: Text {
                    text: "Select Folder"
                    color: btnSelectFolder.hovered ? Theme.accent : Theme.text
                    font.pixelSize: 13
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                background: Rectangle {
                    color: btnSelectFolder.pressed ? Theme.border : (btnSelectFolder.hovered ? Theme.surfaceHover : Theme.background)
                    border.color: Theme.border
                    border.width: 1
                    radius: 6
                }

                onClicked: folderDialog.open()
            }

            // Small spacer
            Item { Layout.preferredWidth: 4 }

            // Your avatar
            Rectangle {
                width: 36
                height: 36
                radius: 18
                Layout.alignment: Qt.AlignVCenter
                clip: true

                ImageRounded {
                    source: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/images/profileIcon.png"
                    sourceSize: 36
                }
            }
        }
    }

    // Bottom line
    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: Theme.border
    }
}