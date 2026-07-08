import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property url iconSource: ""
    property int iconSize: 30
    property color hoverColor: "#313244"

    signal clicked()

    width: iconSize + 20
    height: iconSize + 20

    Rectangle {
        anchors.fill: parent
        radius: width / 2
        color: parent.hoverColor
        opacity: mouseArea.pressed ? 0.8 : 0

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

        opacity: mouseArea.pressed ? 0.6 : 1.0
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }
}