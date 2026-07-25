import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: historyTabRoot
    Layout.fillWidth: true
    Layout.fillHeight: true

    signal playRequested(int songId)

    HistoryModel {
            id: historyModel
    }

    DialogManager {
        id: dialogManagerHistory
    }

    Text {
        text: "EMPTY"
        color: Theme.accent
        font.pixelSize: 40
        anchors.centerIn: parent
        visible: historyListView.count === 0
    }

    ListView {
        id: historyListView
        anchors.fill: parent
        anchors.margins: 10
        anchors.rightMargin: 5
        spacing: 4
        clip: true
        model: historyModel

        delegate: ItemDelegate {
            id: historyItem

            width: historyListView.width - 10
            height: 50
            anchors.rightMargin: 10

            background: Rectangle {
                color: index === 0
                    ? Theme.surfaceHover
                    : historyItem.hovered ? Theme.surfaceHover : "transparent"
                radius: 4
                border.color: index === 0 ? Theme.accent : "transparent"
                border.width: 1
            }

            AppMenu {
                id: songContextMenu
                property int songId: -1

                MenuItem {
                    text: "Add Song to Playlist"
                    onTriggered: {
                            dialogManagerHistory.openChoosePlaylist(songContextMenu.songId, (album, sId) => {
                            playlistService.addSongToPlaylist(album, Number(sId))
                        })
                    }
                }

                MenuItem {
                    text: "Add Song to New Playlist"
                    onTriggered: {
                            dialogManagerHistory.openCreatePlaylist(songContextMenu.songId, (name, sId) => {
                            playlistService.addSongToNewPlaylist(name, Number(sId))
                        })
                    }
                }
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
                    }
                }

                // Name
                Text {
                    text: ShowInfo.title(Number(modelData))
                    color: index === 0 ? Theme.accent : Theme.text
                    font.bold: index === 0
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
                        visible: !historyItem.hovered
                    }

                    IconButton {
                        id: moreBtn
                        anchors.centerIn: parent
                        iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/buttonMoreList.svg"
                        iconSize: 20

                        visible: historyItem.hovered

                        onClicked: {
                            songContextMenu.songId = Number(modelData)
                            songContextMenu.popup()
                        }
                    }
                }
            }

            onClicked: {
                historyTabRoot.playRequested(Number(modelData))
            }

        }
        ScrollBar.vertical: CustomScrollBar {}
    }
}