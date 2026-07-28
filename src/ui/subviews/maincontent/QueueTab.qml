import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: queueTabRoot
    Layout.fillWidth: true
    Layout.fillHeight: true

    signal itemSelected(int requestedIndex)

    QueueModel {
        id: queueModel
    }

    DialogManager {
        id: dialogManagerQueue
    }

    RowLayout {
        id: queueHeader
        anchors.top: parent.top
        anchors.left: parent.left
        anchors.right: parent.right

        anchors.leftMargin: 10
        anchors.rightMargin: 10
        anchors.topMargin: 5
        anchors.bottomMargin: 5
        height: 50


        CustomButton {
            iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/addQueue.svg"
            text: "Queue"

            onClicked: queueMenu.popup()
        }
    }

    AppMenu {
        id: songContextMenu
        property int songId: -1

        MenuItem {
            text: "Add Song to Playlist"
            onTriggered: {
                    dialogManagerQueue.openChoosePlaylist(songContextMenu.songId, (album, sId) => {
                    playlistService.addSongToPlaylist(album, Number(sId))
                })
            }
        }

        MenuItem {
            text: "Add Song to New Playlist"
            onTriggered: {
                    dialogManagerQueue.openCreatePlaylist(songContextMenu.songId, (name, sId) => {
                    playlistService.addSongToNewPlaylist(name, Number(sId))
                })
            }
        }
    }

    AppMenu {
        id: queueMenu

        MenuItem {
            text: "Add Queue to Playlist"
            onTriggered: {
                dialogManagerQueue.openChoosePlaylist(-1, (albumId, songId) => {
                    playlistService.addQueueToPlaylist(albumId)
                })
            }
        }

        MenuItem {
            text: "Save as New Playlist"
            onTriggered: {
                dialogManagerQueue.openCreatePlaylist(-1, (name, songId) => {
                    playlistService.addQueueToNewPlaylist(name)
                })
            }
        }
    }

    Text {
        text: "EMPTY"
        color: Theme.accent
        font.pixelSize: 40
        anchors.centerIn: parent
        visible: queueListView.count === 0
    }

    ListView {
        id: queueListView

        anchors.fill: parent
        anchors.topMargin: 60
        anchors.leftMargin: 10
        anchors.rightMargin: 5
        spacing: 4
        clip: true
        reuseItems: true

        model: queueModel

        delegate: ItemDelegate {
            id: queueItem
            width: queueListView.width - 10
            height: 50

            anchors.rightMargin: 10

            background: Rectangle {
                color: (index === PlayerController.currentIndex)
                       ? Theme.surfaceHover
                       : (queueItem.hovered ? Theme.surfaceHover : "transparent")
                radius: 4
                border.color: (index === PlayerController.currentIndex) ? Theme.accent : "transparent"
                border.width: 1
            }

            contentItem: RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: 10

                // Minicover
                Rectangle {
                    id: miniCoverContainer
                    color: "transparent"

                    width: 40
                    height: width
                    radius: 4
                    ImageRounded {
                        source: ShowInfo.miniCoverPath(Number(modelData))
                        sourceSize: 40
                        cache: true
                    }
                }

                // Name
                Text {
                    text: ShowInfo.title(Number(modelData))
                    color: (index === PlayerController.currentIndex) ? Theme.accent : Theme.text
                    font.bold: index === PlayerController.currentIndex
                    font.pixelSize: 15
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                }

                // Duration
                Item {
                    width: 30
                    Layout.fillHeight: true

                    Text {
                        id: durationText

                        text: ShowInfo.duration(Number(modelData))

                        color: Theme.subtext

                        font.pixelSize: 13

                        anchors.centerIn: parent
                        visible: !queueItem.hovered
                    }

                    IconButton {
                        id: moreBtn
                        anchors.centerIn: parent
                        iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/buttonMoreList.svg"
                        iconSize: 20

                        visible: queueItem.hovered

                        onClicked: {
                            songContextMenu.songId = Number(modelData)
                            songContextMenu.popup()
                        }
                    }
                }
            }

            onClicked: {
                queueTabRoot.itemSelected(index)
            }
        }

        ScrollBar.vertical: CustomScrollBar {}
    }
}