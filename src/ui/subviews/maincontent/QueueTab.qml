import Blueberry_Wolfz 1.0

import "qrc:/Blueberry_Wolfz/src/ui/components"

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: queueTabRoot
    Layout.fillWidth: true
    Layout.fillHeight: true

    signal itemSelected(int requestedIndex)

    Text {
        text: "EMPTY"
        color: "#6c7086"
        font.pixelSize: 40
        anchors.centerIn: parent
        visible: queueListView.count === 0
    }

    ListView {
        id: queueListView

        anchors.fill: parent
        anchors.margins: 10
        spacing: 4
        clip: true

        model: QueueModel

        delegate: ItemDelegate {
            id: queueItem
            width: queueListView.width
            height: 45

            background: Rectangle {
                color: (index === PlayerController.currentIndex)
                       ? "#2e3148"
                       : (queueItem.hovered ? "#252538" : "transparent")
                radius: 4
                border.color: (index === PlayerController.currentIndex) ? "#f38ba8" : "transparent"
                border.width: 1
            }

            contentItem: RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: 10

                // Play Icon
                Text {
                    text: "▶"
                    color: "#f38ba8"
                    font.pixelSize: 11
                    visible: index === PlayerController.currentIndex
                    Layout.alignment: Qt.AlignVCenter
                }

                // Minicover
                ImageRounded {
                    size: 40
                    source: ShowInfo.miniCoverPath(Number(modelData))
                            || "qrc:/Blueberry_Wolfz/src/ui/assets/images/defaultCoverArt.png"
                    radius: 4
                }

                // Name
                Text {
                    text: ShowInfo.title(Number(modelData))
                    color: (index === PlayerController.currentIndex) ? "#f38ba8" : "#cdd6f4"
                    font.bold: index === PlayerController.currentIndex
                    font.pixelSize: 13
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                }

                // Duration
                Text {
                    text: ShowInfo.duration(Number(modelData))

                    color: "#a6adc8"

                    font.pixelSize: 12

                    Layout.alignment:
                        Qt.AlignVCenter
                }
            }

            onClicked: {
                queueTabRoot.onItemSelected(index)
            }
        }

        ScrollBar.vertical: ScrollBar {
            id: queueScrollBar

            policy: ScrollBar.AsNeeded

            width: 8

            contentItem: Rectangle {
                implicitWidth: 8
                radius: 4
                color: "#6c7086"

                opacity: queueScrollBar.pressed ? 0.9 : 0.5
            }

            background: Rectangle {
                color: "transparent"
            }
        }
    }
}