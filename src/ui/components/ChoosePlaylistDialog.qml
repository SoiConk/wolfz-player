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
        color: Theme.surface
        radius: 12
        border.color: Theme.border
    }

    Column {
        id: content
        anchors.fill: parent
        padding: 24
        spacing: 16

        Text {
            text: "Select Playlist"
            color: Theme.text
            font.pixelSize: 24
            font.bold: true
        }

        Rectangle {
            width: parent.width - content.padding * 2
            height: 280
            color: Theme.background
            radius: 8
            border.color: Theme.border
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
                            color: delegateItem.hovered ? Theme.surfaceHover : "transparent"
                        }

                        contentItem: Text {
                            text: ShowInfo.name(Number(modelData))
                            color: Theme.text
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

                ScrollBar.vertical: CustomScrollBar {}
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