import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: queueTabRoot
    Layout.fillWidth: true
    Layout.fillHeight: true

    property var queueList: []
    property int currentPlayingIndex: -1

    signal itemSelected(int index)

    Text {
        text: "EMPTY"
        color: "#6c7086"
        font.pixelSize: 40
        anchors.centerIn: parent
        visible: queueListView.count === 0
    }

    ListView {
        id: queueListView
        anchors.fill: parent
        anchors.margins: 10
        spacing: 4
        clip: true
        model: queueTabRoot.queueList

        delegate: ItemDelegate {
            id: queueItem
            width: queueListView.width
            height: 45

            background: Rectangle {
                color: (index === queueTabRoot.currentPlayingIndex)
                       ? "#2e3148"
                       : (queueItem.hovered ? "#252538" : "transparent")
                radius: 4
                border.color: (index === queueTabRoot.currentPlayingIndex) ? "#f38ba8" : "transparent"
                border.width: 1
            }

            contentItem: RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: 10

                // Play Icon
                Text {
                    text: "▶"
                    color: "#f38ba8"
                    font.pixelSize: 11
                    visible: index === queueTabRoot.currentPlayingIndex
                    Layout.alignment: Qt.AlignVCenter
                }

                // File's name
                Text {
                    text: {
                        var str = modelData;
                        return str.substring(str.lastIndexOf("/") + 1);
                    }
                    color: (index === queueTabRoot.currentPlayingIndex) ? "#f38ba8" : "#cdd6f4"
                    font.bold: index === queueTabRoot.currentPlayingIndex
                    font.pixelSize: 13
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                }
            }

            onClicked: queueTabRoot.itemSelected(index)
        }
    }
}