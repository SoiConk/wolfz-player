import Blueberry_Wolfz 1.0

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
        color: "transparent"
        radius: 12

        ImageRounded {
            source: ShowInfo.coverPath(PlayerController.currentSong)
            sourceSize: 832
            showDefaultOnEmpty: true
        }
    }
}