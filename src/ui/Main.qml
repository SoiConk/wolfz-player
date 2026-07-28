import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

ApplicationWindow {
    id: root
    width: 1020
    height: 600
    visible: true
    minimumWidth: 500
    minimumHeight: 580
    title: "Blueberry Wolfz"

    AlbumModel {
        id: albumModel
    }

    PlaylistService {
        id: playlistService
    }

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TopBar {
            id: topBar
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            Layout.maximumHeight: 60
        }

        RowLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true
            spacing: 0

            SideBar {
                id: sideBar
                Layout.fillHeight: true
                property bool shouldShow: parent.width > 600

                Layout.preferredWidth: shouldShow ? 200 : 70
            }

            MainContent {
                id: mainContent
                Layout.fillWidth: true
                Layout.fillHeight: true
                currentPageIndex: sideBar.currentMenuIndex
            }
        }

        PlayerBar {
            id: playerBar
            Layout.fillWidth: true
            Layout.preferredHeight: 90
            Layout.maximumHeight: 90
        }
    }

    Shortcut {
        sequence: "Space"

        onActivated: {
            PlayerController.togglePlay()
        }
    }

    Shortcut {
        sequence: "Ctrl+Right"

        onActivated: {
            PlayerController.playNext()
        }
    }

    Shortcut {
        sequence: "Ctrl+Left"

        onActivated: {
            PlayerController.playPrevious()
        }
    }

    Shortcut {
        sequence: "Ctrl+R"

        onActivated: {
            PlayerController.cycleLoopMode()
        }
    }

    Shortcut {
        sequence: "Ctrl+Up"

        onActivated: {
            PlayerController.setVolumeByStep(5)
        }
    }

    Shortcut {
        sequence: "Ctrl+Down"

        onActivated: {
            PlayerController.setVolumeByStep(-5)
        }
    }

    Shortcut {
        sequence: "Ctrl+M"

        onActivated: {
            PlayerController.toggleMute()
        }
    }
}