import QtQuick
import QtQuick.Controls
import com.project.USBSwitch

ApplicationWindow {
    id: window
    width:  640
    height: 700
    visible: true
    title: qsTr("Yepkit USB Upstream Switch")

    function toggleVisibility() {
        if (window.visible && window.visibility !== Window.Minimized) {
            window.hide()
        } else {
            window.show()
            window.raise()
            window.requestActivate()
        }
    }

    onClosing: function(close) {
        // Hide to tray instead of quitting when the tray is available.
        if (trayLoader.item && trayLoader.item.available) {
            close.accepted = false
            // Just in case postpone it to the event loop. To avoid issues on Wayland
            Qt.callLater(function() { window.visible = false; })
        }
    }

    Loader {
        id: trayLoader
        active: systemTraySupport
        source: systemTraySupport ? "SystemTray.qml" : ""

        onLoaded: item.targetWindow = window
    }

    StackView {
        readonly property string defaultPage: "NoDevicePage.qml"
        property string currentName: defaultPage

        id: contentFrame
        anchors.fill: parent
        initialItem: defaultPage

        function showQmlPage(qmlFile : string, properties) {
            if ( qmlFile !== currentName ) {
                replace(qmlFile, properties)
                currentName = qmlFile
            } else {
                Object.assign(currentItem, properties)
            }
        }
    }

    Connections {
        target: USBSwitch

        function onDeviceConnected() {
            contentFrame.showQmlPage("DeviceControlPage.qml")
        }

        function onDeviceDisconnected() {
            contentFrame.showQmlPage("NoDevicePage.qml");
        }

        function onErrorTooMuch(deviceList) {
            contentFrame.showQmlPage("ErrorPage.qml", {"manyDevices": true, deviceList, errorMessage: null});
        }


        function onErrorConnectionFailed(errorMessage) {
            contentFrame.showQmlPage("ErrorPage.qml", {"manyDevices": false, "deviceList": null, errorMessage});
        }
    }
}
