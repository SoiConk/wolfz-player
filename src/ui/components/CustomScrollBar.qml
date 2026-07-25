import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Controls

ScrollBar {
    id: root

    policy: ScrollBar.AsNeeded

    width: 10

    contentItem: Rectangle {
        implicitWidth: root.width
        radius: 5
        color: Theme.subtext

        opacity: root.pressed ? 0.9 : 0.5
    }

    background: Rectangle {
        color: "transparent"
    }
}