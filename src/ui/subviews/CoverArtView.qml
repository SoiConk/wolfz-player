import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

Rectangle {
    id: coverArtRoot
    color: "#181825" // Nền tối đồng bộ với MainContent (Catppuccin Crust)

    // Khởi tạo Object để kéo dữ liệu bài hát từ Cache RAM C++
    property var songInfo: null

    // Thuộc tính kiểm tra xem có bài hát nào đang chạy thực tế không
    readonly property bool hasValidSong: typeof playerController !== "undefined" && playerController.currentSong !== ""

    // Lắng nghe tín hiệu khi luồng C++ quét xong bài hát ngầm để ép QML cập nhật lại ảnh/chữ
    Connections {
        target: typeof playerController !== "undefined" ? playerController : null

        // Khi nhận tín hiệu songReady từ PlayerController
        function onSongReady(song) {
            if (playerController.currentSong === song.path) {
                // Cập nhật lại đối tượng songInfo lấy từ MetadataManager
                coverArtRoot.songInfo = playerController.metadataManager.getSongInfo(playerController.currentSong)
            }
        }
    }

    // Khối bọc đĩa nhạc hình vuông
    Rectangle {
        id: coverContainer

        // Luôn co giãn theo 60% của cạnh ngắn nhất màn hình để tạo tỉ lệ cân đối giữa các máy tính
        width: Math.min(parent.width, parent.height) * 0.9
        height: width
        anchors.centerIn: parent

        color: "#313244"      // Màu nền đĩa khi chưa load được ảnh
        radius: 12            // Bo góc đĩa nhạc 12px hiện đại, mềm mại giống Spotify/Apple Music
        border.color: "#45475a"
        border.width: 1

        // BẮT BUỘC: Cắt mọi góc cạnh thừa của các phần tử con bên trong theo đúng bán kính radius (bo góc ảnh)
        clip: true

        Image {
            id: coverImage
            anchors.fill: parent
            anchors.margins: 2 // Tạo khoảng đệm nhỏ so với phần viền ngoài border
            fillMode: Image.PreserveAspectCrop

            source: "qrc:/image/assets/images/defaultCoverArt.png";
        }
    }
}