import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: historyTabRoot
    Layout.fillWidth: true
    Layout.fillHeight: true

    signal playRequested(int songId)
    HistoryModel {
            id: historyModel
    }

    Text {
        text: "EMPTY"
        color: "#6c7086"
        font.pixelSize: 40
        anchors.centerIn: parent
        visible: historyListView.count === 0
    }

    ListView {
        id: historyListView
        anchors.fill: parent
        anchors.margins: 10
        anchors.rightMargin: 5
        spacing: 4
        clip: true
        model: historyModel

        delegate: Item {
            width: ListView.view.width
            height: 50

            ItemDelegate {
                id: historyItem

                anchors.fill: parent
                anchors.rightMargin: 10

                background: Rectangle {
                    color: index === 0
                        ? "#2e3148"
                        : historyItem.hovered ? "#252538" : "transparent"
                    radius: 4
                    border.color: index === 0 ? "#f38ba8" : "transparent"
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
                            sourceSize: 40
                        }
                    }

                    // Name
                    Text {
                        text: ShowInfo.title(Number(modelData))
                        color: index === 0 ? "#f38ba8" : "#cdd6f4"
                        font.bold: index === 0
                        font.pixelSize: 15
                        elide: Text.ElideRight
                        Layout.fillWidth: true
                        Layout.alignment: Qt.AlignVCenter
                    }

                    // Duration
                    Text {
                        text: ShowInfo.duration(Number(modelData))

                        color: "#a6adc8"

                        font.pixelSize: 13

                        Layout.alignment:
                            Qt.AlignVCenter
                    }
                }

                onClicked: {
                    historyTabRoot.playRequested(Number(modelData))
                }
            }
        }

        ScrollBar.vertical: ScrollBar {
            id: historyScrollBar

            policy: ScrollBar.AsNeeded

            width: 10

            contentItem: Rectangle {
                implicitWidth: 10
                radius: 4
                color: "#6c7086"

                opacity: historyScrollBar.pressed ? 0.9 : 0.5
            }

            background: Rectangle {
                color: "transparent"
            }
        }
    }
}