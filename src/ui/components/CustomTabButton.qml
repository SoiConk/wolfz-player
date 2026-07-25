import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property string text: ""
    property bool active: false

    signal clicked()

    Layout.fillWidth: active
    Layout.preferredWidth: active ? 120 : 60
    Layout.fillHeight: true
    clip: true

    MouseArea {
        id: mouse
        anchors.fill: parent
        hoverEnabled: true
        onClicked: root.clicked()
    }

    Text {
        anchors.centerIn: parent
        text: root.text
        font.pixelSize: 16

        color: root.active ? Theme.accent
                           : mouse.containsMouse ? Theme.text : Theme.subtext
        font.bold: root.active
    }

    Rectangle {
        id: indicator
        anchors.bottom: parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter

        height: 3
        radius: 2
        color: Theme.accent

        width: root.active ? parent.width : 0

        opacity: root.active ? 1 : 0

        Behavior on width {
            NumberAnimation { duration: 150; easing.type: Easing.OutCubic }
        }
        Behavior on opacity {
            NumberAnimation { duration: 120 }
        }
    }
}