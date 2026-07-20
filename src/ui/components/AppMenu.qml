import QtQuick
import QtQuick.Controls

Menu {
    id: root

    property color bgColor: "#1e1e2e"
    property color borderColor: "#313244"
    property color hoverColor: "#313244"
    property color textColor: "#cdd6f4"

    padding: 8

    width: 240

    background: Rectangle {
        color: root.bgColor
        radius: 10
        border.color: root.borderColor
    }

    delegate: MenuItem {
        id: item

        implicitWidth: parent ? parent.width - root.padding * 2 : 224
        implicitHeight: 42

        leftPadding: 16
        rightPadding: 16

        background: Rectangle {
            radius: 6
            anchors.fill: parent
            anchors.margins: 2
            color: (item.hovered || item.highlighted) ? root.hoverColor : "transparent"
        }

        contentItem: Text {
            text: item.text
            color: root.textColor

            font.pixelSize: 16
            font.bold: true

            verticalAlignment: Text.AlignVCenter
            horizontalAlignment: Text.AlignLeft
            elide: Text.ElideRight
        }
    }
}