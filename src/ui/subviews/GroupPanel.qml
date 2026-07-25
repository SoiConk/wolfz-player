import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: gproot
    color: Theme.surface
    border.color: Theme.border
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

                CustomTabButton {
                    text: "History"
                    active: gproot.currentGroupTab === 0
                    onClicked: gproot.currentGroupTab = 0
                }

                Rectangle {
                    height: 30
                    width: 1
                    color: Theme.border
                }

                CustomTabButton {
                    text: "Queue"
                    active: gproot.currentGroupTab === 1
                    onClicked: gproot.currentGroupTab = 1
                }

                Rectangle {
                    height: 30
                    width: 1
                    color: Theme.border
                }

                CustomTabButton {
                    text: "Lyrics"
                    active: gproot.currentGroupTab === 2
                    onClicked: gproot.currentGroupTab = 2
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
            currentIndex: gproot.currentGroupTab
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