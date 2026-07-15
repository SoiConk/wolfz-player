import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: groupPanelRoot
    color: "#1e1e2e"
    border.color: "#313244"
    border.width: 1

    property int currentGroupTab: 1 // 0: History, 1: Queue, 2: Lyrics

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        // TabBar (History | Queue | Lyrics)
        Item {
            Layout.fillWidth: true
            Layout.minimumHeight: 50
            Layout.maximumHeight: 50

            RowLayout {
                anchors.fill: parent
                spacing: 0

                Button {
                    Layout.fillWidth: true; Layout.fillHeight: true; flat: true
                    contentItem: Text {
                        text: "History"
                        color: groupPanelRoot.currentGroupTab === 0 ? "#f38ba8" : "#a6adc8"
                        font.bold: groupPanelRoot.currentGroupTab === 0
                        font.pixelSize: 13
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: groupPanelRoot.currentGroupTab = 0
                }

                Button {
                    Layout.fillWidth: true; Layout.fillHeight: true; flat: true
                    contentItem: Text {
                        text: "Queue"
                        color: groupPanelRoot.currentGroupTab === 1 ? "#f38ba8" : "#a6adc8"
                        font.bold: groupPanelRoot.currentGroupTab === 1
                        font.pixelSize: 13
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: groupPanelRoot.currentGroupTab = 1
                }

                Button {
                    Layout.fillWidth: true; Layout.fillHeight: true; flat: true
                    contentItem: Text {
                        text: "Lyrics"
                        color: groupPanelRoot.currentGroupTab === 2 ? "#f38ba8" : "#a6adc8"
                        font.bold: groupPanelRoot.currentGroupTab === 2
                        font.pixelSize: 13
                        horizontalAlignment: Text.AlignHCenter
                        verticalAlignment: Text.AlignVCenter
                    }
                    onClicked: groupPanelRoot.currentGroupTab = 2
                }
            }
        }

        // Line
        Rectangle { Layout.fillWidth: true; Layout.preferredHeight: 1; color: "#313244" }

        // Content zone
        StackLayout {
            id: contentStack
            Layout.fillWidth: true
            Layout.fillHeight: true
            currentIndex: groupPanelRoot.currentGroupTab
            clip: true

             // index 0
            HistoryTab {
                id: historyTab
                Layout.fillWidth: true
                Layout.fillHeight: true

                onPlayRequested: (idSong) => MusicLoader.loadId(idSong)
            }

            // index 1
            QueueTab {
                id: queueTab
                Layout.fillWidth: true
                Layout.fillHeight: true

                onItemSelected: (index) => PlayerController.playAt(index)
            }

            // index 2
            LyricsTab {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }
}