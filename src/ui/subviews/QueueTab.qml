import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: queueTabRoot
    Layout.fillWidth: true
    Layout.fillHeight: true

    // Dữ liệu nhận từ GroupPanel kết nối với C++
    property var queueList: []
    property int currentPlayingIndex: -1

    // Tín hiệu phát ra khi người dùng click vào một bài trong hàng đợi để chuyển bài
    signal itemSelected(int index)

    // Giao diện khi hàng đợi trống
    Text {
        text: "🎵 Hàng đợi trống"
        color: "#6c7086"
        font.pixelSize: 13
        anchors.centerIn: parent
        visible: queueListView.count === 0
    }

    // Danh sách hiển thị hàng đợi phát nhạc
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

            // Đổi màu nền nếu là bài hát đang phát hoặc khi hover chuột qua
            background: Rectangle {
                color: (index === queueTabRoot.currentPlayingIndex)
                       ? "#2e3148" // Màu nổi bật cho bài đang phát
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

                // Icon Trạng thái (Chỉ hiện ở bài đang phát)
                Text {
                    text: "▶"
                    color: "#f38ba8"
                    font.pixelSize: 11
                    visible: index === queueTabRoot.currentPlayingIndex
                    Layout.alignment: Qt.AlignVCenter
                }

                // Tên file bài hát
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

            onClicked: {
                queueTabRoot.itemSelected(index)
            }
        }
    }
}