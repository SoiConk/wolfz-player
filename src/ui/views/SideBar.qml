import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: sideBarRoot
    color: Theme.background

    // Property for Main.qml to control which content to show (0: Home, 1: Library, 2: About)
    property int currentMenuIndex: 0

    // Mini sidebar check
    readonly property bool isCollapsed: width < 100

    PlaylistHistoryModel {
        id: playlistHistoryModel
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 8

        //Home
        CustomButton {
            text: "Home"
            iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/home.svg"
            isActive: sideBarRoot.currentMenuIndex === 0
            isCollapsed: sideBarRoot.isCollapsed
            onClicked: sideBarRoot.currentMenuIndex = 0
        }

        // Library
        CustomButton {
            text: "Library"
            iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/library.svg"
            isActive: sideBarRoot.currentMenuIndex === 1
            isCollapsed: sideBarRoot.isCollapsed
            onClicked: sideBarRoot.currentMenuIndex = 1
        }

        //About
        CustomButton {
            text: "About"
            iconSource: "qrc:/qt/qml/Blueberry_Wolfz/src/ui/assets/icons/about.svg"
            isActive: sideBarRoot.currentMenuIndex === 2
            isCollapsed: sideBarRoot.isCollapsed
            onClicked: sideBarRoot.currentMenuIndex = 2
        }

        // Line Border
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: Theme.border
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            visible: !sideBarRoot.isCollapsed
        }

        // Recent Library
        ListView {
            Layout.fillWidth: true
            Layout.fillHeight: true

            clip: true
            spacing: 4
            visible: !sideBarRoot.isCollapsed

            header: Text {
                text: "RECENT PLAYLIST"
                font.pixelSize: 11
                font.bold: true
                color: Theme.subtext
                height: 25
                leftPadding: 8
            }

            model: playlistHistoryModel

            delegate: ItemDelegate {
                id: playlistHistoryItem

                width: ListView.view.width - 10
                height: 50
                anchors.rightMargin: 10
                hoverEnabled: true

                background: Rectangle {
                    color: playlistHistoryItem.hovered ? Theme.surfaceHover : "transparent"
                    radius: 4
                    border.color: playlistHistoryItem.hovered ? Theme.border : "transparent"
                    border.width: 1
                }

                contentItem: Text {
                    text: ShowInfo.name(Number(modelData))
                    color: Theme.text
                    font.pixelSize: 15
                    elide: Text.ElideRight
                    verticalAlignment: Text.AlignVCenter
                }

                onClicked: {
                    MusicLoader.openPlaylist(Number(modelData), 0)
                }
            }

            ScrollBar.vertical: CustomScrollBar {}

        }

        Item { Layout.fillHeight: sideBarRoot.isCollapsed }
    }

    Rectangle {
        anchors.right: parent.right
        height: parent.height
        width: 1
        color: Theme.border
    }
}