import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Controls

ScrollBar {
    id: root

    policy: ScrollBar.AsNeeded

    width: hovered || pressed ? 10 : 6

    contentItem: Rectangle {
        implicitWidth: root.width
        radius: 5
        color: Theme.subtext

        opacity: root.pressed ? 0.9
                              : root.hovered ? 0.7 : 0.2
        Behavior on opacity {
            NumberAnimation { duration: 150 }
        }
        Behavior on implicitWidth {
            NumberAnimation { duration: 150 }
        }
    }

    background: Rectangle {
        color: "transparent"
    }
}