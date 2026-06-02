import QtQuick
import QtQuick.Controls
import com.project.USBSwitch

Item {
    height: parent ? parent.height : 0
    width:  parent ? parent.width  : 0

    Connections {
        target: USBSwitch
        function onPortStateUpdated(port, enabled, message) { // Signal handler for mySignal
            lastRestatus.text = "Port-" + port + " " + (enabled ? qsTr("ON") : qsTr("OFF"));
            lastRestatus.color = "white";
            updateState(port, enabled)
        }
        function onErrorReported(message) { // Signal handler for mySignal
            lastRestatus.text = qsTr("ERROR: ") + message;
            lastRestatus.color = "#FB2C36";
        }
    }

    function updateState(port, enabled) {
        console.assert(port === 1 || port === 2 || port === 3, "port must be 1, 2 or 3");

        let portsOff = [port1off, port2off, port3off]
        let portsOn  = [port1on,  port2on,  port3on ]
        let index = (port-1) % 3

        portsOff[index].checked = !enabled
        portsOn[index].checked = enabled
    }

    Rectangle {
        id: bg
        anchors.fill: parent
        color: "#2c3e50"
    }

    Label {
        id: infoLabel
        anchors.top:  parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 20
        color: "white"
        font.bold: true
        font.pointSize: 19
        text: USBSwitch.deviceInfo
    }

    Grid {
        anchors.centerIn: parent
        columns: 3
        spacing: 20

        Label {
            text: qsTr("Port 1")
            font.pointSize: 20
            width: 80
            height: 80
            color: "white"
            verticalAlignment: Qt.AlignVCenter
        }

        RoundButton {
            id: port1off
            // icon.name: "stop"
            // icon.width: 32
            // icon.height: 32
            text: qsTr("OFF")
            width: 80
            height: 80
            checkable: true
            checked: false
            font.pointSize: 18
            onClicked: USBSwitch.disableUsbPort(1)

            palette.button: checked ? "salmon" : "lightgrey"
        }

        RoundButton {
            id: port1on
            // icon.name: "stop"
            // icon.width: 32
            // icon.height: 32
            text: qsTr("ON")
            width: 80
            height: 80
            checkable: true
            checked: false
            font.pointSize: 18
            onClicked: USBSwitch.enableUsbPort(1)

            palette.button: checked ? "#2AA63E" : "lightgrey"
        }

        //-------
        Label {
            text: qsTr("Port 2")
            font.pointSize: 20
            width: 80
            height: 80
            color: "white"
            verticalAlignment: Qt.AlignVCenter
        }

        RoundButton {
            id: port2off
            // icon.name: "stop"
            // icon.width: 32
            // icon.height: 32
            text: qsTr("OFF")
            width: 80
            height: 80
            checkable: true
            checked: false
            font.pointSize: 18
            onClicked: USBSwitch.disableUsbPort(2)

            palette.button: checked ? "salmon" : "lightgrey"
        }

        RoundButton {
            id: port2on
            // icon.name: "stop"
            // icon.width: 32
            // icon.height: 32
            text: qsTr("ON")
            width: 80
            height: 80
            checkable: true
            checked: false
            font.pointSize: 18
            onClicked: USBSwitch.enableUsbPort(2)

            palette.button: checked ? "#2AA63E" : "lightgrey"
        }

        //-------3
        Label {
            text: qsTr("Port 3")
            font.pointSize: 20
            width: 80
            height: 80
            color: "white"
            verticalAlignment: Qt.AlignVCenter
        }

        RoundButton {
            id: port3off
            // icon.name: "stop"
            // icon.width: 32
            // icon.height: 32
            text: qsTr("OFF")
            width: 80
            height: 80
            checkable: true
            checked: false
            font.pointSize: 18
            onClicked: USBSwitch.disableUsbPort(3)

            palette.button: checked ? "salmon" : "lightgrey"
        }

        RoundButton {
            id: port3on
            // icon.name: "stop"
            // icon.width: 32
            // icon.height: 32
            text: qsTr("ON")
            width: 80
            height: 80
            checkable: true
            checked: false
            font.pointSize: 18
            onClicked: USBSwitch.enableUsbPort(3)

            palette.button: checked ? "#2AA63E" : "lightgrey"
        }


    }

    Label {
        id: lastRestatus
        anchors.bottom:  parent.bottom
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.bottomMargin: 20
        color: "white"
        font.bold: true
        font.pointSize: 12
        text: ""
    }

}
