import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import QtQuick.Dialogs
import QtCore

Rectangle {
    id: topBarRoot
    color: "#1e1e2e" // Màu nền tối Catppuccin

    // -------------------------------------------------------------
    // HỘP THOẠI DUYỆT FILE & FOLDER (Hỗ trợ Qt 6)
    // -------------------------------------------------------------
    FileDialog {
        id: fileDialog
        title: "Chọn tệp tin âm thanh"
        currentFolder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
        nameFilters: ["Audio files (*.mp3 *.wav)"]
        onAccepted: {
            var path = selectedFile.toString();

            if (path.startsWith("file:///")) {
                path = path.substring(8);
            }
            playerController.openFile(path);
        }
    }

    FolderDialog {
        id: folderDialog
        title: "Chọn thư mục nhạc"
        currentFolder: StandardPaths.writableLocation(StandardPaths.MusicLocation)
        onAccepted: {
            var path = selectedFolder.toString();
            if (path.startsWith("file:///")) {
                path = path.substring(8);
            }
            playerController.openFolder(path);
        }
    }

    // -------------------------------------------------------------
    // GIAO DIỆN THANH TOPBAR
    // -------------------------------------------------------------
    RowLayout {
        anchors.fill: parent
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 15

        // Phần 1: Tên và Logo tạm thời
        RowLayout {
            spacing: 10
            Rectangle {
                width: 30
                height: 30
                radius: 12
                color: "#1e1e2e"

                Image {
                    id: appLogo
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectCrop
                    source: "qrc:/image/assets/images/appLogo.png"
                    smooth: true
                    mipmap: true
                }
            }
            Text {
                text: "Blueberry Wolfz"
                color: "#cdd6f4"
                font.pixelSize: 16
                font.bold: true
            }
        }

        Item { Layout.fillWidth: true } // Spacer đẩy các cụm chức năng sang phải

        // Cụm bên phải: Gồm phần 3 và phần 2 sát nhau
        RowLayout {
            spacing: 12

            // Phần 3: Nút Open File (Được custom style đẹp mắt)
            Button {
                id: btnOpenFile
                flat: true
                implicitHeight: 32

                contentItem: Text {
                    text: "📁 Open File"
                    color: btnOpenFile.hovered ? "#f5e0dc" : "#cdd6f4"
                    font.pixelSize: 13
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                background: Rectangle {
                    color: btnOpenFile.pressed ? "#414356" : (btnOpenFile.hovered ? "#313244" : "#181825")
                    border.color: "#313244"
                    border.width: 1
                    radius: 6
                }

                onClicked: fileDialog.open()
            }

            // Nút Open Folder
            Button {
                id: btnOpenFolder
                flat: true
                implicitHeight: 32

                contentItem: Text {
                    text: "📂 Open Folder"
                    color: btnOpenFolder.hovered ? "#f5e0dc" : "#cdd6f4"
                    font.pixelSize: 13
                    font.bold: true
                    verticalAlignment: Text.AlignVCenter
                    horizontalAlignment: Text.AlignHCenter
                }

                background: Rectangle {
                    color: btnOpenFolder.pressed ? "#414356" : (btnOpenFolder.hovered ? "#313244" : "#181825")
                    border.color: "#313244"
                    border.width: 1
                    radius: 6
                }

                onClicked: folderDialog.open()
            }

            // Khoảng cách nhỏ giữa nút bấm và Avatar
            Item { Layout.preferredWidth: 4 }

            // Phần 2: Avatar cá nhân tạm thời
            Rectangle {
                width: 32
                height: 32
                radius: 16
                border.color: "#b4befe"
                border.width: 1
                Layout.alignment: Qt.AlignVCenter
                clip: true

                Image {
                    id: miniCoverProfile
                    anchors.fill: parent
                    anchors.margins: parent.border.width
                    fillMode: Image.PreserveAspectCrop
                    source: "qrc:/image/assets/images/profileIcon.png"

                    smooth: true
                    mipmap: true
                }
            }
        }
    }

    // Đường kẻ đáy TopBar
    Rectangle {
        anchors.bottom: parent.bottom
        width: parent.width
        height: 1
        color: "#313244"
    }
}