import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    modal: true
    focus: true

    width: 440
    height: 450
    anchors.centerIn: Overlay.overlay

    property int songId: -1

    signal playlistSelected(int albumId, int songId)

    background: Rectangle {
        color: "#1e1e2e"
        radius: 12
        border.color: "#313244"
    }

    Column {
        id: content
        anchors.fill: parent
        padding: 24
        spacing: 16

        Text {
            text: "Select Playlist"
            color: "#cdd6f4"
            font.pixelSize: 22
            font.bold: true
        }

        Rectangle {
            width: parent.width - content.padding * 2
            height: 280
            color: "#181825"
            radius: 8
            border.color: "#313244"
            clip: true

            ListView {
                id: playlistView
                anchors.fill: parent
                anchors.margins: 10
                anchors.rightMargin: 5
                spacing: 4
                model: albumModel

                boundsBehavior: Flickable.StopAtBounds

                delegate: Item {
                    width: ListView.view.width
                    height: 45
                    property bool isAddItem: index === playlistView.count - 1

                    ItemDelegate {
                        id: delegateItem
                        anchors.fill: parent
                        anchors.rightMargin: 10
                        visible: !isAddItem

                        background: Rectangle {
                            radius: 4
                            color: delegateItem.hovered ? "#252538" : "transparent"
                        }

                        contentItem: Text {
                            text: ShowInfo.name(Number(modelData))
                            color: "#cdd6f4"
                            font.pixelSize: 15
                            font.bold: true
                            verticalAlignment: Text.AlignVCenter
                            elide: Text.ElideRight
                        }

                        onClicked: {
                            root.playlistSelected(Number(modelData), root.songId)
                            root.close()
                        }
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

        Row {
            anchors.right: parent.right
            anchors.rightMargin: content.padding

            Button {
                text: "Cancel"
                width: 90
                height: 40

                background: Rectangle {
                    color: parent.hovered ? "#313244" : "#181825"
                    radius: 6
                }

                contentItem: Text {
                    text: parent.text
                    color: "#a6adc8"
                    font.pixelSize: 14
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: root.close()
            }
        }
        Keys.onEscapePressed: root.close()
    }
}