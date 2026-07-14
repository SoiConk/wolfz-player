import "qrc:/Blueberry_Wolfz/src/ui/components"

import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: sideBarRoot
    color: "#181825"

    // Property for Main.qml to control which content to show (0: Home, 1: Library, 2: About)
    property int currentMenuIndex: 0

    // Mini sidebar check
    readonly property bool isCollapsed: width < 100

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 5
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 8

        //Home
        CustomButton {
            text: "Home"
            iconSource: "qrc:/Blueberry_Wolfz/src/ui/assets/icons/home.svg"
            isActive: sideBarRoot.currentMenuIndex === 0
            isCollapsed: sideBarRoot.isCollapsed
            onClicked: sideBarRoot.currentMenuIndex = 0
        }

        // Library
        CustomButton {
            text: "Library"
            iconSource: "qrc:/Blueberry_Wolfz/src/ui/assets/icons/library.svg"
            isActive: sideBarRoot.currentMenuIndex === 1
            isCollapsed: sideBarRoot.isCollapsed
            onClicked: sideBarRoot.currentMenuIndex = 1
        }

        //About
        CustomButton {
            text: "About"
            iconSource: "qrc:/Blueberry_Wolfz/src/ui/assets/icons/about.svg"
            isActive: sideBarRoot.currentMenuIndex === 2
            isCollapsed: sideBarRoot.isCollapsed
            onClicked: sideBarRoot.currentMenuIndex = 2
        }

        // Line
        Rectangle {
            Layout.fillWidth: true
            Layout.preferredHeight: 1
            color: "#313244"
            Layout.topMargin: 10
            Layout.bottomMargin: 10
            visible: !sideBarRoot.isCollapsed
        }

        // Recent Library
        ScrollView {
            Layout.fillWidth: true
            Layout.fillHeight: true
            clip: true
            visible: !sideBarRoot.isCollapsed

            ColumnLayout {
                anchors.fill: parent
                spacing: 4

                Text {
                    text: "RECENT PLAYLIST"
                    font.pixelSize: 11
                    font.bold: true
                    color: "#585b70"
                    Layout.leftMargin: 8
                    Layout.bottomMargin: 5
                }

                Repeater {
                    model: []
                    delegate: Button {
                        Layout.fillWidth: true
                        Layout.preferredHeight: 35
                        flat: true

                        contentItem: Text {
                            text: modelData
                            color: "#a6adc8"
                            font.pixelSize: 13
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }

                        onHoveredChanged: {
                            contentItem.color = hovered ? "#cdd6f4" : "#a6adc8"
                        }
                    }
                }
            }
        }

        Item { Layout.fillHeight: sideBarRoot.isCollapsed }
    }

    Rectangle {
        anchors.right: parent.right
        height: parent.height
        width: 1
        color: "#313244"
    }
}