import "../subviews"

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: mainContentRoot
    color: "#1e1e2e"

    // Property get from SideBar through Main.qml
    property int currentPageIndex: 0

    readonly property bool isCollaped: width < 800

    StackLayout {
        anchors.fill: parent
        currentIndex: mainContentRoot.currentPageIndex

        // Home
        RowLayout {
            id: homeViewPage
            spacing: 0

            // Image
            CoverArtView {
                Layout.fillWidth: true
                Layout.fillHeight: true
            }

            // GroupBox
            GroupPanel {
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.35
                Layout.minimumWidth: 320
                Layout.maximumWidth: 450
                visible: !isCollaped
            }
        }

        // Library
        Rectangle {
            id: libraryViewPage
            color: "#1e1e2e"

            Text {
                text: "📚 YOUR LIBRARY\n(WIP)"
                color: "#cdd6f4"
                font.pixelSize: 20
                horizontalAlignment: Text.AlignHCenter
                anchors.centerIn: parent
            }
        }

        // About
        Rectangle {
            id: aboutViewPage
            color: "#11111b"

            Text {
                text: "ℹ️ MUSIC PLAYER v1.0\nMade with Qt QML"
                color: "#a6adc8"
                font.pixelSize: 16
                horizontalAlignment: Text.AlignHCenter
                anchors.centerIn: parent
            }
        }
    }
}