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
        color: "#1e1e2e"
        radius: 12
        border.color: "#313244"
    }

    Column {
        id: content
        spacing: 20
        padding: 24
        width: parent.width

        Text {
            text: "Create Playlist"
            color: "#cdd6f4"
            font.pixelSize: 24
            font.bold: true
        }

        TextField {
            id: input
            width: parent.width - content.padding * 2
            height: 44
            font.pixelSize: 15
            placeholderText: "Enter playlist name"
            color: "#cdd6f4"
            placeholderTextColor: "#6c7086"

            background: Rectangle {
                radius: 6
                color: "#181825"
                border.color: input.focus ? "#b4befe" : "#313244"
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

            Button {
                text: "Create"
                width: 90
                height: 40
                enabled: input.text.length > 0

                background: Rectangle {
                    radius: 6
                    color: !parent.enabled ? "#313244"
                         : parent.pressed ? "#a6adc8"
                         : parent.hovered ? "#cdd6f4"
                         : "#b4befe"
                }

                contentItem: Text {
                    text: parent.text
                    color: "#1e1e2e"
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