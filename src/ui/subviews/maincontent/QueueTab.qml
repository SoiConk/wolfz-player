import Blueberry_Wolfz 1.0

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
        anchors.rightMargin: 5
        spacing: 4
        clip: true

        model: QueueModel

        delegate: Item {
            width: ListView.view.width
            height: 45

            ItemDelegate {
                id: queueItem

                anchors.fill: parent
                anchors.rightMargin: 10

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

                    // Minicover
                    Rectangle {
                        id: miniCoverContainer

                        width: 40
                        height: width
                        radius: 4
                        ImageRounded {
                            source: ShowInfo.miniCoverPath(Number(modelData))
                                    || "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/images/defaultCoverArt.png"
                        }
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
        }

        ScrollBar.vertical: ScrollBar {
            id: queueScrollBar

            policy: ScrollBar.AsNeeded

            width: 10

            contentItem: Rectangle {
                implicitWidth: 10
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