import QtQuick
import QtQuick.Effects

Item {
    id: root
    property url source: ""
    property real radius: -1

    anchors.fill: parent

    readonly property real effectiveRadius: {
        if (radius >= 0)
            return radius

        if (parent && parent.radius !== undefined)
            return parent.radius

        return 0
    }

    Image {
        id: image
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
        radius: root.effectiveRadius
        color: "black"
        layer.enabled: true
        visible: false
        antialiasing: true
    }
}