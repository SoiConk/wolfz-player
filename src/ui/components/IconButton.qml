import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

Item {
    id: root

    property url iconSource: ""
    property int iconSize: 30
    property color hoverColor: Theme.surfaceHover

    signal clicked()

    width: iconSize + 20
    height: iconSize + 20

    Rectangle {
        anchors.fill: parent
        radius: root.width / 2
        color: parent.hoverColor
        opacity: mouseArea.pressed ? 0.8
                                   : mouseArea.containsMouse ? 0.4 : 0
    }

    Image {
        id: iconImage
        anchors.centerIn: parent

        width: root.iconSize
        height: root.iconSize

        sourceSize.width: root.iconSize
        sourceSize.height: root.iconSize

        source: root.iconSource
        fillMode: Image.PreserveAspectFit
        smooth: true
        visible: false

        opacity: mouseArea.pressed ? 0.6 : 1.0
    }

    MultiEffect {
        anchors.fill: iconImage
        source: iconImage
        colorization: 1.0
        colorizationColor: Theme.primary
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        propagateComposedEvents: false
        preventStealing: true
        cursorShape: Qt.PointingHandCursor
        onClicked: (mouse) => {
            mouse.accepted = true
            root.clicked()
        }
    }
}