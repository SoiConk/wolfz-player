import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: root
    Layout.fillWidth: true
    Layout.fillHeight: true

    signal openAlbum(var albumId)

    DialogManager {
        id: localDialogManager
    }

    Item {
        anchors.fill: parent
        anchors.topMargin: 24
        anchors.leftMargin: 15
        anchors.rightMargin: 5

        GridView {
            id: grid
            anchors.fill: parent
            clip: true
            boundsBehavior: Flickable.StopAtBounds
            rightMargin: 15

            cellHeight: 264

            cellWidth: {
                var availableWidth = width - rightMargin
                var idealItemWidth = 180

                var columns = Math.floor(availableWidth / idealItemWidth)
                if (columns <= 1) return availableWidth

                return availableWidth / columns
            }

            model: albumModel

            delegate: Item {
                id: delegateRoot
                width: grid.cellWidth
                height: 240

                property bool hovered: false
                property bool isAddItem: index === grid.count - 1

                Column {
                    width: 180
                    height: 240
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 10

                    Rectangle {
                        id: coverBox
                        width: 180
                        height: 180
                        radius: 12
                        color: "#313244"
                        opacity: !isAddItem ? 1 : delegateRoot.hovered ? 1 : 0.4

                        Loader {
                            id: addItemLoader
                            anchors.fill: parent
                            sourceComponent: {
                                return isAddItem ? addItemComponent : albumCoverComponent
                            }
                        }

                        Component {
                            id: albumCoverComponent
                            ImageRounded {
                                anchors.fill: parent
                                source: ShowInfo.albumCoverPath(Number(modelData))
                                sourceSize: 180
                            }
                        }
                        Component {
                            id: addItemComponent
                            IconButton {
                                iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/addAlbum.svg"
                                iconSize: 110
                                visible: isAddItem
                                anchors.centerIn: parent
                            }
                        }

                        MouseArea {
                            anchors.fill: parent
                            hoverEnabled: true

                            onEntered: delegateRoot.hovered = true
                            onExited: delegateRoot.hovered = false

                            onClicked: {
                                if (isAddItem) {
                                    localDialogManager.openCreatePlaylist(-1, (name) => {
                                        playlistService.createPlaylist(name)
                                    })
                                } else {
                                    root.openAlbum(modelData)
                                }
                            }
                        }
                    }

                    Text {
                        text: isAddItem ? "Add Playlist" : ShowInfo.name(Number(modelData))
                        color: "#cdd6f4"
                        font.pixelSize: 18
                        font.bold: true
                        elide: Text.ElideRight
                        width: parent.width
                        horizontalAlignment: isAddItem ? Text.AlignHCenter : Text.AlignLeft
                    }
                }
            }

            ScrollBar.vertical: ScrollBar {
                id: queueScrollBar

                policy: ScrollBar.AsNeeded
                width: 10

                contentItem: Rectangle {
                    implicitWidth: 10
                    radius: 4
                    color: "#6c7086"
                    opacity: queueScrollBar.pressed ? 0.9 : 0.5
                }

                background: Rectangle {
                    color: "transparent"
                }
            }
        }
    }
}