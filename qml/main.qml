import QtQuick
import QtQuick.Controls
import com.project.USBSwitch

ApplicationWindow {
    width:  640
    height: 700
    visible: true
    title: qsTr("Yepkit USB Upstream Switch")

    StackView {
        id: contentFrame
        anchors.fill: parent
        initialItem: "NoDevicePage.qml"
    }

    Connections {
        target: USBSwitch

        function onDeviceConnected() {
            contentFrame.replace("DeviceControlPage.qml")
        }

        function onDeviceDisconnected() {
            contentFrame.replace("NoDevicePage.qml");
        }

        function onErrorTooMuch(deviceList) {
            contentFrame.replace("ErrorPage.qml", {"manyDevices": true, "deviceList": deviceList});
        }
    }
}
