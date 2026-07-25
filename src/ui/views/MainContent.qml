import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: mainContentRoot
    color: Theme.background

    // Property get from SideBar through Main.qml
    property int currentPageIndex: 0

    readonly property bool isCollaped: width < 800

    Loader {
        anchors.fill: parent

        sourceComponent: {
            switch(currentPageIndex) {
                case 0:
                    return homeComponent
                case 1:
                    return libraryComponent
                case 2:
                    return aboutComponent
                default: return homeComponent
            }
        }
    }

    Component {
        id: homeComponent
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
                Layout.preferredWidth: mainContentRoot.width * 0.35
                Layout.minimumWidth: 320
                Layout.maximumWidth: 450
                visible: !mainContentRoot.isCollaped
            }
        }
    }

    Component {
        id: libraryComponent
        Library {
            id: libraryViewPage
        }
    }

    Component {
        id: aboutComponent
        About {
            id: aboutViewPage
        }
    }

}