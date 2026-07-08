import QtQuick
import QtQuick.Layouts

Item {
    id: lyricsTabRoot

    ColumnLayout {
        anchors.centerIn: parent
        spacing: 10
        width: parent.width * 0.8

        Text {
            text: "🎤"
            font.pixelSize: 32
            Layout.alignment: Qt.AlignHCenter
            opacity: 0.5
        }

        Text {
            text: "WIP"
            color: "#6c7086"
            font.pixelSize: 13
            horizontalAlignment: Text.AlignHCenter
            wrapMode: Text.WordWrap
            Layout.fillWidth: true
        }
    }
}