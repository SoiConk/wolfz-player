import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Controls

Item {
    id: root

    StackView {
        id: stack
        anchors.fill: parent
        clip: true

        initialItem: AlbumGridPage {
            onOpenAlbum: function(albumId) {
                stack.push("qrc:/qt/qml/Blueberry_Wolfz/src/ui/subviews/library/AlbumDetailView.qml", {
                    albumId: albumId,
                    stackView: stack
                })
            }
        }
    }
}