import "../components"

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: coverArtRoot
    color: "#181825"

    Rectangle {
        id: coverContainer

        width: Math.min(parent.width, parent.height) * 0.9
        height: width
        anchors.centerIn: parent
        color: "#313244"
        radius: 12
        border.color: "#45475a"
        border.width: 1

        ImageRounded {
            anchors.fill: parent
            anchors.margins: 2
            size: Math.min((parent.width - 4) / 50 * 50, (parent.height - 4) / 50 * 50, 850)
            source: "qrc:/Blueberry_Wolfz/src/ui/assets/images/defaultCoverArt.png"
            radius: 12
        }
    }
}