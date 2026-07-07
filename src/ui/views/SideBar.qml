import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: sideBarRoot
    color: "#181825"

    // property for Main.qml to control which content to show (0: Home, 1: Library, 2: About)
    property int currentMenuIndex: 0

    // Mini sidebar check
    readonly property bool isCollapsed: width < 100

    ColumnLayout {
        anchors.fill: parent
        anchors.topMargin: 20
        anchors.leftMargin: 10
        anchors.rightMargin: 10
        spacing: 8

        //Home
        Button {
            id: homeBtn
            Layout.fillWidth: true
            Layout.preferredHeight: 45
            flat: true

            background: Rectangle {
                color: sideBarRoot.currentMenuIndex === 0 ? "#313244" : "transparent"
                radius: 6
            }

            contentItem: RowLayout {
                spacing: sideBarRoot.isCollapsed ? 0 : 12

                Text {
                    text: "🏠"
                    font.pixelSize: 18
                    color: sideBarRoot.currentMenuIndex === 0 ? "#cdd6f4" : "#a6adc8"
                    Layout.alignment: sideBarRoot.isCollapsed ? Qt.AlignCenter : Qt.AlignLeft
                }
                Text {
                    text: "Home"
                    font.pixelSize: 14
                    font.bold: sideBarRoot.currentMenuIndex === 0
                    color: sideBarRoot.currentMenuIndex === 0 ? "#cdd6f4" : "#a6adc8"

                    visible: !sideBarRoot.isCollapsed

                    Layout.fillWidth: true
                }
            }
            onClicked: sideBarRoot.currentMenuIndex = 0
        }

        // Library
        Button {
            id: libraryBtn
            Layout.fillWidth: true
            Layout.preferredHeight: 45
            flat: true
            background: Rectangle {
                color: sideBarRoot.currentMenuIndex === 1 ? "#313244" : "transparent"
                radius: 6
            }
            contentItem: RowLayout {
                spacing: sideBarRoot.isCollapsed ? 0 : 12
                Text {
                    text: "📚"
                    font.pixelSize: 18
                    color: sideBarRoot.currentMenuIndex === 1 ? "#cdd6f4" : "#a6adc8"
                    Layout.alignment: sideBarRoot.isCollapsed ? Qt.AlignCenter : Qt.AlignLeft
                }
                Text {
                    text: "Library"
                    font.pixelSize: 14
                    font.bold: sideBarRoot.currentMenuIndex === 1
                    color: sideBarRoot.currentMenuIndex === 1 ? "#cdd6f4" : "#a6adc8"

                    visible: !sideBarRoot.isCollapsed

                    Layout.fillWidth: true
                }
            }
            onClicked: sideBarRoot.currentMenuIndex = 1
        }

        // About
        Button {
            id: aboutBtn
            Layout.fillWidth: true
            Layout.preferredHeight: 45
            flat: true
            background: Rectangle {
                color: sideBarRoot.currentMenuIndex === 2 ? "#313244" : "transparent"
                radius: 6
            }
            contentItem: RowLayout {
                spacing: sideBarRoot.isCollapsed ? 0 : 12
                Text {
                    text: "ℹ️"
                    font.pixelSize: 18
                    color: sideBarRoot.currentMenuIndex === 2 ? "#cdd6f4" : "#a6adc8"
                    Layout.alignment: sideBarRoot.isCollapsed ? Qt.AlignCenter : Qt.AlignLeft
                }
                Text {
                    text: "About"
                    font.pixelSize: 14
                    font.bold: sideBarRoot.currentMenuIndex === 2
                    color: sideBarRoot.currentMenuIndex === 2 ? "#cdd6f4" : "#a6adc8"

                    visible: !sideBarRoot.isCollapsed

                    Layout.fillWidth: true
                }
            }
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