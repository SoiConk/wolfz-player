import Blueberry_Wolfz

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import QtCore
import QtQuick.Effects

Rectangle {
    id: topBarRoot
    color: "#1e1e2e"

    // Select file or folder dialog
    FileDialog {
        id: fileDialog
        title: "Select Audio file"
        currentFolder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
        nameFilters: ["Audio files (*.mp3 *.wav)"]
        onAccepted: {
            var path = selectedFile.toString();

            if (path.startsWith("file:///")) {
                path = path.substring(8);
            }
            PlayerController.openFile(path);
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Select Audio folder"
        currentFolder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
        onAccepted: {
            var path = selectedFolder.toString();
            if (path.startsWith("file:///")) {
                path = path.substring(8);
            }
            PlayerController.openFolder(path);
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
                color: "#1e1e2e"

                Image {
                    id: appLogo
                    sourceSize.width: 30
                    sourceSize.height: 30
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectCrop
                    source: "qrc:/Blueberry_Wolfz/src/ui/assets/images/appLogo.png"
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

        // Cụm bên phải: Gồm phần 3 và phần 2 sát nhau
        RowLayout {
            spacing: 12

            // Select File Button
            Button {
                id: btnSelectFile
                flat: true
                implicitHeight: 32

                contentItem: Text {
                    text: "Select File"
                    color: btnSelectFile.hovered ? "#f5e0dc" : "#cdd6f4"
                    font.pixelSize: 13
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                background: Rectangle {
                    color: btnSelectFile.pressed ? "#414356" : (btnSelectFile.hovered ? "#313244" : "#181825")
                    border.color: "#313244"
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
                    color: btnSelectFolder.hovered ? "#f5e0dc" : "#cdd6f4"
                    font.pixelSize: 13
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                background: Rectangle {
                    color: btnSelectFolder.pressed ? "#414356" : (btnSelectFolder.hovered ? "#313244" : "#181825")
                    border.color: "#313244"
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
                border.color: "#b4befe"
                border.width: 1
                Layout.alignment: Qt.AlignVCenter
                clip: true

                Image {
                    id: miniCoverProfile
                    sourceSize.width: 35
                    sourceSize.height: 35
                    anchors.fill: parent
                    anchors.margins: parent.border.width
                    fillMode: Image.PreserveAspectCrop
                    source: "qrc:/Blueberry_Wolfz/src/ui/assets/images/profileIcon.png"

                    smooth: true
                    visible: false
                }

                MultiEffect {
                    source: miniCoverProfile
                    anchors.fill: miniCoverProfile
                    maskEnabled: true
                    maskSource: mask
                }

                Rectangle {
                    id: mask
                    width: miniCoverProfile.width
                    height: miniCoverProfile.height
                    radius: 17
                    color: "black"
                    layer.enabled: true
                    visible: false
                    antialiasing: true
                }
            }
        }
    }

    // Bottom line
    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: "#313244"
    }
}