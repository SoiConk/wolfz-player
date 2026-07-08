/*
    Using:
    CustomButton {
        text:
        iconSource:
        isActive:
        isCollapsed:
        onClicked:
    }
*/

import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property url iconSource: ""
    property string text: ""
    property bool isActive: false
    property bool isCollapsed: false

    signal clicked()

    Layout.fillWidth: true
    implicitHeight: 45

    Rectangle {
        anchors.fill: parent
        radius: 6
        color: root.isActive ? "#313244" : (mouseArea.containsMouse ? "#252538" : "transparent")

        Behavior on color { ColorAnimation { duration: 100 } }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: root.isCollapsed ? 0 : 12

            opacity: root.isActive ? 1.0 : (mouseArea.containsMouse ? 0.9 : 0.6)
            Behavior on opacity { NumberAnimation { duration: 100 } }

            Image {
                id: iconImg
                source: root.iconSource
                sourceSize.width: 25
                sourceSize.height: 25
                fillMode: Image.PreserveAspectFit
                smooth: true

                Layout.alignment: root.isCollapsed ? Qt.AlignCenter : Qt.AlignLeft
            }

            Text {
                text: root.text
                font.pixelSize: 20
                font.bold: root.isActive
                color: "#cdd6f4"

                visible: !root.isCollapsed
                Layout.fillWidth: true
                verticalAlignment: Text.AlignVCenter
            }
        }
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor
        onClicked: root.clicked()
    }
}