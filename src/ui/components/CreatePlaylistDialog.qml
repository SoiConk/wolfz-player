import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Controls

Dialog {
    id: root
    modal: true
    focus: true
    width: 440
    anchors.centerIn: Overlay.overlay

    property int songId: -1
    signal submit(string name, int songId)

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
            text: "Create Playlist"
            color: Theme.confirm
            font.pixelSize: 24
            font.bold: true
        }

        TextField {
            id: input
            width: parent.width - content.padding * 2
            height: 44
            font.pixelSize: 15
            placeholderText: "Enter playlist name"
            color: Theme.text
            placeholderTextColor: Theme.subtext

            background: Rectangle {
                radius: 6
                color: Theme.background
                border.color: input.focus ? Theme.confirm : Theme.border
            }
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
                text: "Create"
                width: 90
                height: 40
                enabled: input.text.length > 0

                background: Rectangle {
                    radius: 6
                    color: !parent.enabled ? Theme.surfaceHover
                         : parent.pressed ? Theme.confirm
                         : parent.hovered ? Theme.text
                         : Theme.confirm
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
                    root.submit(input.text, root.songId)
                    root.close()
                }
            }
        }
        Keys.onEscapePressed: root.close()
    }

    onOpened: {
        input.text = ""
        input.forceActiveFocus()
    }
}