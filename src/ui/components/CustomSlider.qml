import QtQuick
import QtQuick.Layouts

Item {
    id: root

    property real value: 0.0
    property real from: 0.0
    property real to: 1.0

    property color progressColor: "#f38ba8"
    property color backgroundColor: "#313244"

    property bool alwaysShowHandle: false

    property bool updateOnDrag: false

    signal moved(real newValue)
    signal released(real newValue)

    Layout.fillWidth: true
    implicitHeight: 16

    readonly property real currentDisplayValue: mouseArea.pressed ? mouseArea.visualValue : root.value
    readonly property real factor: (to > from) ? (currentDisplayValue - from) / (to - from) : 0.0

    Rectangle {
        id: bgTrack
        anchors.left: parent.left; anchors.right: parent.right; anchors.verticalCenter: parent.verticalCenter
        height: mouseArea.containsMouse ? 6 : 4
        radius: height / 2
        color: root.backgroundColor
        Behavior on height { NumberAnimation { duration: 100 } }

        Rectangle {
            id: progressTrack
            height: parent.height
            width: bgTrack.width * root.factor
            radius: parent.radius
            color: root.progressColor
        }
    }

    Rectangle {
        id: handle
        width: 22; height: 22; radius: 11; color: "#ffffff"
        anchors.verticalCenter: bgTrack.verticalCenter
        x: (bgTrack.width * root.factor) - (width / 2)
        visible: root.alwaysShowHandle || mouseArea.containsMouse
    }

    MouseArea {
        id: mouseArea
        anchors.fill: parent
        hoverEnabled: true
        cursorShape: Qt.PointingHandCursor

        property real visualValue: 0.0

        function calculateValue(mouseX) {
            let percentage = Math.max(0.0, Math.min(1.0, mouseX / width));
            return root.from + percentage * (root.to - root.from);
        }

        onPressed: (mouse) => {
            visualValue = calculateValue(mouse.x)
            if (root.updateOnDrag) {
                root.moved(visualValue);
            }
        }

        onPositionChanged: (mouse) => {
            if (mouse.buttons & Qt.LeftButton) {
                visualValue = calculateValue(mouse.x)
                if (root.updateOnDrag) {
                    root.moved(visualValue);
                }
            }
        }

        onReleased: (mouse) => {
            let finalValue = calculateValue(mouse.x);
            root.released(finalValue);
        }
    }
}