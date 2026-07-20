import Blueberry_Wolfz 1.0

import QtQuick
import QtQuick.Controls

Item {
    id: manager

    property var currentCreateCallback: null
    property var currentDeleteCallback: null
    property var currentChooseCallback: null

    CreatePlaylistDialog {
        id: createDialog
        onSubmit: (name, songId) => {
            if (manager.currentCreateCallback) {
                manager.currentCreateCallback(name, songId)
            }
        }
    }

    ConfirmDeletePlaylistDialog {
        id: deleteDialog
        onConfirm: (albumId) => {
            if (manager.currentDeleteCallback) {
                manager.currentDeleteCallback(albumId)
            }
        }
    }

    ChoosePlaylistDialog {
        id: chooseDialog
        onPlaylistSelected: (albumId, songId) => {
            if (manager.currentChooseCallback) {
                manager.currentChooseCallback(albumId, songId)
            }
        }
    }

    function openCreatePlaylist(songId, callback) {
        manager.currentCreateCallback = callback
        createDialog.songId = songId
        createDialog.open()
    }

    function openDeletePlaylist(albumId, callback) {
        manager.currentDeleteCallback = callback
        deleteDialog.albumId = albumId
        deleteDialog.open()
    }

    function openChoosePlaylist(songId, callback) {
        manager.currentChooseCallback = callback
        chooseDialog.songId = songId
        chooseDialog.open()
    }
}