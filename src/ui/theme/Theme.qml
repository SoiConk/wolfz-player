pragma Singleton

import Blueberry_Wolfz 1.0
import QtQuick

QtObject {
    id: root

    property QtObject darkTheme: DarkTheme {}
    property QtObject currentTheme: darkTheme

    readonly property color background: currentTheme.background
    readonly property color surface: currentTheme.surface
    readonly property color surfaceHover: currentTheme.surfaceHover

    readonly property color text: currentTheme.text
    readonly property color subtext: currentTheme.subtext

    readonly property color primary: currentTheme.primary
    readonly property color accent: currentTheme.accent

    readonly property color border: currentTheme.border

    readonly property color confirm: currentTheme.confirm
    readonly property color deny: currentTheme.deny
    readonly property color layer: currentTheme.layer

    function setTheme(theme) {
        currentTheme = theme
    }
}