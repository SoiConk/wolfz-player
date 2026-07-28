import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Effects

Item {
    id: root
    property url source: ""
    property bool showDefaultOnEmpty: false
    property real radius: -1
    property bool isAnimated: source.toString().endsWith(".gif")
    property int sourceSize: 0
    property bool cache: false

    anchors.fill: parent

    readonly property bool useRectangle: source.toString() === "" && !showDefaultOnEmpty

    readonly property real effectiveRadius: {
        if (radius >= 0)
            return radius

        if (parent && parent.radius !== undefined)
            return parent.radius

        return 0
    }

    Loader {
        id: contentLoader
        anchors.fill: parent

        sourceComponent: {
            if (useRectangle) {
                return rectangleComponent
            }
            return root.isAnimated ? gifComponent : imageComponent
        }
    }

    Component {
        id: imageComponent
        Item {
            Image {
                id: image
                anchors.fill: parent
                source: root.source.toString() !== "" ? root.source
                                                      : "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/images/defaultCoverArt.png"
                fillMode: Image.PreserveAspectCrop
                visible: false
                smooth: true
                asynchronous: true
                cache: root.cache
                sourceSize.width: root.sourceSize
                sourceSize.height: undefined
            }

            MultiEffect {
                source: image
                anchors.fill: image
                maskEnabled: true
                maskSource: mask
                maskThresholdMin: 0.5
                maskSpreadAtMin: 1.0
            }

            Rectangle {
                id: mask
                width: image.width
                height: image.height
                radius: root.effectiveRadius
                color: "black"
                layer.enabled: true
                layer.smooth: true
                visible: false
                antialiasing: true
            }
        }
    }

    Component {
        id: gifComponent
        Item {
            AnimatedImage {
                id: gif
                anchors.fill: parent
                source: root.source
                fillMode: AnimatedImage.PreserveAspectCrop
                visible: false
                smooth: true
                asynchronous: true
                cache: false
                sourceSize.width: root.sourceSize
                sourceSize.height: undefined
            }

            MultiEffect {
                source: gif
                anchors.fill: gif
                maskEnabled: true
                maskSource: mask

                maskThresholdMin: 0.5
                maskSpreadAtMin: 1.0
            }

            Rectangle {
                id: mask
                width: gif.width
                height: gif.height
                radius: root.effectiveRadius
                color: "black"
                layer.enabled: true
                layer.smooth: true
                visible: false
                antialiasing: true
            }
        }
    }

    Component {
        id: rectangleComponent
        Rectangle {
            radius: root.effectiveRadius
            color: Theme.layer
        }
    }
}