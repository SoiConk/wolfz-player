import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: historyTabRoot
    Layout.fillWidth: true
    Layout.fillHeight: true

    property var historyList: []

    signal playRequested(string filePath)

    Text {
        text: "📜 Lịch sử phát trống"
        color: "#6c7086"
        font.pixelSize: 13
        anchors.centerIn: parent
        visible: historyListView.count === 0
    }

    ListView {
        id: historyListView
        anchors.fill: parent
        anchors.margins: 10
        spacing: 4
        clip: true
        model: historyTabRoot.historyList

        delegate: ItemDelegate {
            id: historyItem
            width: historyListView.width
            height: 40

            background: Rectangle {
                color: historyItem.hovered ? "#252538" : "transparent"
                radius: 4
            }

            contentItem: RowLayout {
                anchors.fill: parent
                anchors.leftMargin: 10
                anchors.rightMargin: 10
                spacing: 10

                Text {
                    text: "↩"
                    color: "#a6adc8"
                    font.pixelSize: 14
                    Layout.alignment: Qt.AlignVCenter
                }

                Text {
                    text: {
                        var str = modelData;
                        return str.substring(str.lastIndexOf("/") + 1);
                    }
                    color: "#cdd6f4"
                    font.pixelSize: 13
                    elide: Text.ElideRight
                    Layout.fillWidth: true
                    Layout.alignment: Qt.AlignVCenter
                }
            }

            onClicked: {
                historyTabRoot.playRequested(modelData)
            }
        }
    }
}