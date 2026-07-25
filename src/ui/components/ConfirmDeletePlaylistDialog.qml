import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    modal: true
    focus: true
    width: 440
    anchors.centerIn: Overlay.overlay

    property int albumId: -1
    property string playlistName: ShowInfo.name(Number(albumId))
    signal confirm(int albumId)

    background: Rectangle {
        color: Theme.surface
        radius: 12
        border.color: Theme.border
    }

    Column {
        id: content
        spacing: 20
        padding: 24
        width: parent.width

        Text {
            text: "Delete Playlist"
            color: Theme.deny
            font.pixelSize: 22
            font.bold: true
        }

        Text {
            width: parent.width - content.padding * 2
            text: "Are you sure you want to delete \"" + playlistName + "\"?"
            color: Theme.text
            font.pixelSize: 15
            wrapMode: Text.WordWrap
        }

        Text {
            text: "This action cannot be undone."
            color: Theme.subtext
            font.pixelSize: 13
        }

        Row {
            anchors.right: parent.right
            anchors.rightMargin: content.padding
            spacing: 12

            Button {
                text: "Cancel"
                width: 90
                height: 40

                background: Rectangle {
                    color: parent.hovered ? Theme.surfaceHover : Theme.background
                    radius: 6
                }

                contentItem: Text {
                    text: parent.text
                    color: Theme.subtext
                    font.pixelSize: 14
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: root.close()
            }

            Button {
                text: "Delete"
                width: 90
                height: 40

                background: Rectangle {
                    radius: 6
                    color: parent.pressed ? Theme.deny
                         : parent.hovered ? Theme.subtext
                         : Theme.deny
                }

                contentItem: Text {
                    text: parent.text
                    color: Theme.surface
                    font.pixelSize: 14
                    font.bold: true
                    anchors.fill: parent
                    horizontalAlignment: Text.AlignHCenter
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    root.confirm(albumId)
                    root.close()
                }
            }
        }
        Keys.onEscapePressed: root.close()
    }
}