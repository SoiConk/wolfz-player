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

import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Effects

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
        color: root.isActive ? Theme.surfaceHover
                             : (mouseArea.containsMouse ? Theme.background : "transparent")

        Behavior on color { ColorAnimation { duration: 100 } }

        RowLayout {
            anchors.fill: parent
            anchors.leftMargin: 10
            anchors.rightMargin: 10
            spacing: root.isCollapsed ? 0 : 12

            opacity: root.isActive ? 1.0 : (mouseArea.containsMouse ? 0.9 : 0.6)
            Behavior on opacity { NumberAnimation { duration: 100 } }

            Item {
                Layout.preferredWidth: 25
                    Layout.preferredHeight: 25
                    Layout.alignment: root.isCollapsed ? Qt.AlignCenter : Qt.AlignLeft
                Image {
                    id: iconImg
                    source: root.iconSource
                    fillMode: Image.PreserveAspectFit
                    smooth: true
                    visible: false
                }

                MultiEffect {
                    anchors.fill: iconImg
                    source: iconImg
                    colorization: 1.0
                    colorizationColor: Theme.subtext
                }
            }

            Text {
                text: root.text
                font.pixelSize: 20
                font.bold: root.isActive
                color: Theme.text

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