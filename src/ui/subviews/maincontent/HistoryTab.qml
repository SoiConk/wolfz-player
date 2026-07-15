import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: historyTabRoot
    Layout.fillWidth: true
    Layout.fillHeight: true

    signal playRequested(int songId)

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
        spacing: 4
        clip: true
        model: HistoryModel

        delegate: ItemDelegate {
            id: historyItem
            width: historyListView.width
            height: 45

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

                // Play icon
                Text {
                    text: "↩"
                    color: index === 0 ? "#f38ba8" : "#a6adc8"
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignVCenter
                }

                // Minicover
                ImageRounded {
                    size: 40
                    source: ShowInfo.miniCoverPath(Number(modelData))
                            || "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/images/defaultCoverArt.png"
                    radius: 4
                }

                // Name
                Text {
                    text: ShowInfo.title(Number(modelData))
                    color: index === 0 ? "#f38ba8" : "#cdd6f4"
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
                historyTabRoot.playRequested(Number(modelData))
            }
        }

        ScrollBar.vertical: ScrollBar {
            id: historyScrollBar

            policy: ScrollBar.AsNeeded

            width: 8

            contentItem: Rectangle {
                implicitWidth: 8
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