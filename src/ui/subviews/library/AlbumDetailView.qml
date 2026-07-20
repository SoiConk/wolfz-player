import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs

Item {
    id: root

    property var  albumId
    property StackView stackView

    Connections {
        target: ShowInfo
        function onReloadAlbumCache() {
            refreshTrigger++
        }
    }

    PlaylistModel {
        id: playlistModel
        playlistId: root.albumId
    }

    DialogManager {
        id: dialogManagerADV
        property int playlistId: -1
    }

    FileDialog {
        id: coverFileDialog
        title: "Select New Cover"
        currentFolder: MusicLoader.lastFolder
        nameFilters: [
            "All files (*.png *.jpg *.jpeg *.webp *.gif)",
            "Media Files (*.png *.jpg *.jpeg *.webp)",
            "GIF files (*.gif)"
        ]

        onAccepted: {
            var path = selectedFile.toString();
            if (path.startsWith("file:///")) {
                path = path.substring(8);
            }
            playlistService.setCover(root.albumId, path);
            stackView.replace("qrc:/qt/qml/Blueberry_Wolfz/src/ui/subviews/library/AlbumDetailView.qml", {
                albumId: root.albumId,
                stackView: stackView
                }, StackView.Immediate
            )
        }
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 10
        anchors.margins: 10

        RowLayout  {
            spacing: 10
            Layout.fillWidth: true
            Layout.preferredHeight: 250
            Layout.maximumHeight: 250
            Layout.minimumHeight: 250
            Layout.alignment: Qt.AlignTop

            Rectangle {
                width: 250
                height: 250
                radius: 8
                color: "#313244"

                ImageRounded {
                    source: ShowInfo.albumCoverPath(Number(albumId))
                    sourceSize: 250
                }

                HoverHandler {
                    id: coverHover
                }

                Rectangle {
                    anchors.fill: parent
                    color: "#b0000000"
                    radius: parent.radius-2
                    opacity: coverHover.hovered ? 1.0 : 0.0
                    visible: opacity > 0.0

                    Behavior on opacity {
                        NumberAnimation { duration: 200 }
                    }

                    IconButton {
                        iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/addAlbum.svg"
                        iconSize: 150
                        anchors.centerIn: parent
                        opacity: coverHover.hovered ? 0.6 : 0.0
                        visible: opacity > 0.0
                        onClicked: {
                            coverFileDialog.open()
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: 5
                Layout.alignment: Qt.AlignTop

                RowLayout {
                    Layout.fillWidth: true

                    Item { Layout.fillWidth: true }

                    Rectangle {
                        height: 45
                        width: 100
                        radius: 6
                        color: "transparent"
                        border.color: "#45475a"
                        border.width: 1

                        CustomButton {
                            iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/back.svg"
                            text: "Back"
                            anchors.fill: parent
                            onClicked: root.stackView.pop()
                        }
                    }
                }

                ColumnLayout {
                    spacing: 10
                    Layout.fillWidth: true

                    Text {
                        text: ShowInfo.name(Number(albumId))
                        color: "#cdd6f4"
                        font.pixelSize: 55
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }

                    Text {
                        text: ShowInfo.durations(Number(albumId))
                        color: "#a6adc8"
                        font.pixelSize: 35
                        Layout.fillWidth: true
                        elide: Text.ElideRight
                    }
                }

                Item { Layout.fillHeight: true }

                RowLayout {
                    height: 30
                    IconButton {
                        iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/buttonPlay.svg"
                        onClicked: MusicLoader.openPlaylist(albumId, 0)
                    }

                    IconButton {
                        iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/buttonRemove.svg"
                        onClicked: {
                            dialogManagerADV.playlistId = Number(albumId)
                            dialogManagerADV.openDeletePlaylist(dialogManagerADV.playlistId, (albumId) => {
                                playlistService.removePlaylist(albumId)
                                root.stackView.pop()
                            })
                        }
                    }

                    Item { Layout.fillWidth: true }
                }
            }
        }

        RowLayout {
            spacing: 10
            Layout.fillWidth: true

            Text {
                text: "#"
                Layout.preferredWidth: 50
                horizontalAlignment: Text.AlignHCenter
                color: "#a6adc8"
                font.pixelSize: 18
            }

            Text {
                text: "Song"
                Layout.fillWidth: true
                color: "#a6adc8"
                font.pixelSize: 18
            }


            Text {
                Layout.alignment: Qt.AlignVCenter
                horizontalAlignment: Text.AlignHCenter
                verticalAlignment: Text.AlignVCenter
                text: "Duration"
                Layout.preferredWidth: 80
                visible: parent.width > 550
                color: "#a6adc8"
                font.pixelSize: 18
            }

            Item { Layout.preferredWidth: 40 }

            Item { Layout.preferredWidth: 5 }
        }

        Rectangle {
            color: "#45475a"
            Layout.fillWidth: true
            height: 1
        }

        ListView {
            id: listView
            Layout.fillWidth: true
            Layout.fillHeight: true
            model: playlistModel
            clip: true

            delegate: Item {
                id: listItem
                width: listView.width
                height: 60

                HoverHandler {
                    id: hover
                }

                property bool hovered: hover.hovered

                RowLayout {
                    anchors.fill: parent
                    spacing: 10

                    // ===== # / play =====
                    Item {
                        Layout.preferredWidth: 50
                        Layout.fillHeight: true

                        Text {
                            anchors.centerIn: parent
                            text: index + 1
                            visible: !listItem.hovered
                            color: "#cdd6f4"
                            font.pixelSize: 18
                        }

                        IconButton {
                            iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/buttonPlay.svg"
                            anchors.centerIn: parent
                            visible: listItem.hovered
                            onClicked: {
                                MusicLoader.openPlaylist(Number(albumId), index)
                            }
                        }
                    }

                    RowLayout {
                        Layout.fillWidth: true
                        spacing: 10

                        Rectangle {
                            width: 50
                            height: 50
                            radius: 6
                            color: "#313244"
                            ImageRounded {
                                source: ShowInfo.miniCoverPath(Number(modelData))
                                sourceSize: 50
                            }
                        }

                        ColumnLayout {
                            Layout.fillWidth: true
                            spacing: 2
                            Layout.alignment: Qt.AlignVCenter

                            Text {
                                text: ShowInfo.title(Number(modelData))
                                color: "#cdd6f4"
                                font.pixelSize: 26
                                font.bold: false
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                            Text {
                                text: ShowInfo.artist(Number(modelData))
                                color: "#a6adc8"
                                font.pixelSize: 16
                                font.italic: true
                                elide: Text.ElideRight
                                Layout.fillWidth: true
                            }
                        }
                    }

                    // ===== Duration =====
                    Text {
                        Layout.alignment: Qt.AlignVCenter
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                        text: ShowInfo.duration(Number(modelData))
                        Layout.preferredWidth: 80
                        visible: listView.width > 550
                        color: "#a6adc8"
                        font.pixelSize: 18
                    }

                    // ===== Cột cuối (icon giống #) =====
                    Item {
                        Layout.preferredWidth: 40
                        Layout.fillHeight: true

                        IconButton {
                            iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/buttonMoreList.svg"
                            anchors.centerIn: parent
                            visible: listItem.hovered
                            onClicked: {
                                console.log("A")
                            }
                        }
                    }

                    Item { Layout.preferredWidth: 5 }
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: playlistScrollBar

                policy: ScrollBar.AsNeeded

                width: 10

                contentItem: Rectangle {
                    implicitWidth: 10
                    radius: 4
                    color: "#6c7086"

                    opacity: playlistScrollBar.pressed ? 0.9 : 0.5
                }

                background: Rectangle {
                    color: "transparent"
                }
            }
        }
    }
    /*
    onAlbumIdChanged: {
        if (albumId !== undefined && albumId !== null) {
            songModel.setAlbum(albumId)
        }
    }
    */
}