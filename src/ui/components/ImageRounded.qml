/*
    Using:
    ImageRounded {
        size:
        source:
        radius:
    }
*/

import QtQuick
import QtQuick.Effects

Item {
    id: root
    property url source: ""
    property real radius: 0
    property int size: 0

    width: size
    height: size

    Image {
        id: image
        sourceSize.width: root.size
        sourceSize.height: root.size
        anchors.fill: parent
        source: root.source
        fillMode: Image.PreserveAspectCrop
        visible: false
        smooth: true
        asynchronous: true
        cache: false
    }

    MultiEffect {
        source: image
        anchors.fill: image
        maskEnabled: true
        maskSource: mask
    }

    Rectangle {
        id: mask
        width: image.width
        height: image.height
        radius: root.radius
        color: "black"
        layer.enabled: true
        visible: false
        antialiasing: true
    }
}