import QtQuick
import Qt.labs.platform as Labs

Labs.SystemTrayIcon {
    id: tray

    property var targetWindow: null

    visible: available
    icon.source: "qrc:/YepkitUSBSwitch/UsbIcon.ico"
    tooltip: targetWindow ? targetWindow.title : ""

    onActivated: function(reason) {
        if (targetWindow && reason === Labs.SystemTrayIcon.DoubleClick)
            targetWindow.toggleVisibility()
    }

    menu: Labs.Menu {
        Labs.MenuItem {
            text: tray.targetWindow && tray.targetWindow.visible ? qsTr("Hide") : qsTr("Show")
            onTriggered: if (tray.targetWindow) tray.targetWindow.toggleVisibility()
        }
        Labs.MenuItem { separator: true }
        Labs.MenuItem {
            text: qsTr("Quit")
            onTriggered: Qt.quit()
        }
    }
}