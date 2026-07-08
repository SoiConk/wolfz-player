import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Item {
    id: aboutPageRoot

    anchors.fill: parent
    anchors.margins: 10

    Rectangle {
        id: card
        anchors.centerIn: parent

        width: parent.width * 0.9
        height: parent.height * 0.9

        readonly property bool isLarge: card.width > 950 && card.height > 600

        readonly property int titleSize: isLarge ? 72 : 36

        readonly property int bodySize: isLarge ? 36 : 18

        readonly property int mainSpacing: isLarge ? 40 : 20

        readonly property int innerSpacing: isLarge ? 20 : 10

        readonly property int imageSize: isLarge ? 300 : 150

        color: "#24273a"
        radius: 24
        border.color: "#494d64"
        border.width: 1

        RowLayout {
            anchors.fill: parent
            anchors.margins: card.mainSpacing
            spacing: card.mainSpacing

            ColumnLayout {
                Layout.fillWidth: true
                Layout.fillHeight: true
                spacing: card.innerSpacing
                Layout.alignment: Qt.AlignVCenter

                Text {
                    text: "<a href='https://github.com/SoiConk/wolfz-player.git' style='color: #8195ea;
                        text-decoration: none;'>Blueberry Wolfz</a>"
                    font.pixelSize: card.titleSize
                    font.bold: true
                    textFormat: Text.RichText
                    onLinkActivated: Qt.openUrlExternally(link)
                }

                Text {
                    text: "Music player by <a href='https://github.com/SoiConK' style='color: #faec74;
                        text-decoration: none;'>Sói Con</a>"
                    color: "#a5adcb"
                    font.pixelSize: card.bodySize
                    textFormat: Text.RichText
                    onLinkActivated: Qt.openUrlExternally(link)
                }

                Item { height: card.innerSpacing }

                Column {
                    spacing: card.innerSpacing
                    Repeater {
                        model: ["- Simple music player"
                            , "- Queue and history"
                            , "- Metadata (WIP)"
                            , "- Library and playlist (WIP)"
                            , "- Lyric (WIP)"]
                        Text {
                            text: modelData
                            color: "#a5adcb"
                            font.pixelSize: card.bodySize
                        }
                    }
                }
            }

            ColumnLayout {
                Layout.preferredWidth: card.imageSize
                Layout.alignment: Qt.AlignCenter
                spacing: card.innerSpacing
                visible: card.width > 455

                Rectangle {
                    Layout.preferredWidth: card.imageSize
                    Layout.preferredHeight: card.imageSize
                    color: "transparent"

                    Image {
                        anchors.fill: parent
                        source: "qrc:/Blueberry_Wolfz/src/ui/assets/images/about.png"
                        sourceSize.height: card.imageSize
                        sourceSize.width: card.imageSize
                        fillMode: Image.PreserveAspectCrop
                        asynchronous: true
                        smooth: true
                    }
                }
                Text {
                    text: "Thank you!"
                    color: "#8195ea"
                    font.pixelSize: card.bodySize
                    font.bold: true
                    Layout.alignment: Qt.AlignHCenter
                    style: Text.Outline
                    styleColor: "#ffffff"
                }
            }
        }
    }
}