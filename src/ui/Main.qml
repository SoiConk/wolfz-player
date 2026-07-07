import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "views"

ApplicationWindow {
    id: root
    width: 1020
    height: 680
    visible: true
    minimumWidth: 500
    minimumHeight: 580
    title: "Blueberry Wolfz"

    ColumnLayout {
        anchors.fill: parent
        spacing: 0

        TopBar {
            id: topBar
            Layout.fillWidth: true
            Layout.preferredHeight: 60
            Layout.minimumHeight: 60
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
            Layout.minimumHeight: 90
            Layout.maximumHeight: 90
        }
    }
}