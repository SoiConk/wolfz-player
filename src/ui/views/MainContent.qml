import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: mainContentRoot
    color: "#1e1e2e"

    // Property get from SideBar through Main.qml
    property int currentPageIndex: 0
    property alias groupPanel: groupPanelRoot

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
                id: groupPanelRoot
                Layout.fillHeight: true
                Layout.preferredWidth: parent.width * 0.35
                Layout.minimumWidth: 320
                Layout.maximumWidth: 450
                visible: !isCollaped
            }
        }

        Library {
            id: libraryViewPage
        }

        About {
            id: aboutViewPage
        }
    }
}