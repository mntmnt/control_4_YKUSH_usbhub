import QtQuick
import QtQuick.Controls
import com.project.USBSwitch

Item {
    readonly property color uncheckedButtonColor:    "#ADADAD"
    readonly property color checkedConnectedColor:   "#2AA63E"
    readonly property color checkedUnconnectedColor: "salmon"
    readonly property int controlsOuterMargin: 20
    readonly property int controlsSpacing: 20
    readonly property real controlCardWidth: Math.max(140, Math.min(220, width - (controlsOuterMargin * 2)))

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
        width: Math.max(0, (parent ? parent.width : 0) - 40)
        horizontalAlignment: Text.AlignHCenter
        wrapMode: Text.Wrap
        maximumLineCount: 3
        elide: Text.ElideRight
        clip: true
        color: "white"
        font.bold: true
        font.pointSize: 19
        text: USBSwitch.deviceInfo
    }

    Item {
        id: controlsArea
        anchors.top: infoLabel.bottom
        anchors.bottom: lastRestatus.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.leftMargin: controlsOuterMargin
        anchors.rightMargin: controlsOuterMargin
        clip: true

        Flow {
            id: portsFlow
            flow: Flow.TopToBottom
            width: childrenRect.width
            height: controlsArea.height
            x: (controlsArea.width - width) / 2
            y: (controlsArea.height - childrenRect.height) / 2
            spacing: controlsSpacing

            Item {
                width: Math.max(140, controlCardWidth)
                height: port1Column.implicitHeight

            Column {
                id: port1Column
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 12

                Label {
                    text: qsTr("Port 1")
                    width: parent.parent.width
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.Wrap
                    color: "white"
                    font.pointSize: 20
                    font.bold: true
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12

                    RoundButton {
                        id: port1off
                        text: qsTr("OFF")
                        width: Math.max(64, (parent.parent.parent.width - parent.spacing) / 2)
                        height: width
                        checkable: true
                        checked: false
                        font.pointSize: width < 72 ? 14 : 18
                        onClicked: USBSwitch.disableUsbPort(1)

                        palette.button: checked ? checkedUnconnectedColor : uncheckedButtonColor
                    }

                    RoundButton {
                        id: port1on
                        text: qsTr("ON")
                        width: Math.max(64, (parent.parent.parent.width - parent.spacing) / 2)
                        height: width
                        checkable: true
                        checked: false
                        font.pointSize: width < 72 ? 14 : 18
                        onClicked: USBSwitch.enableUsbPort(1)

                        palette.button: checked ? checkedConnectedColor : uncheckedButtonColor
                    }
                }
            }
        }

            Item {
                width: Math.max(140, controlCardWidth)
                height: port2Column.implicitHeight

            Column {
                id: port2Column
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 12

                Label {
                    text: qsTr("Port 2")
                    width: parent.parent.width
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.Wrap
                    color: "white"
                    font.pointSize: 20
                    font.bold: true
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12

                    RoundButton {
                        id: port2off
                        text: qsTr("OFF")
                        width: Math.max(64, (parent.parent.parent.width - parent.spacing) / 2)
                        height: width
                        checkable: true
                        checked: false
                        font.pointSize: width < 72 ? 14 : 18
                        onClicked: USBSwitch.disableUsbPort(2)

                        palette.button: checked ? checkedUnconnectedColor : uncheckedButtonColor
                    }

                    RoundButton {
                        id: port2on
                        text: qsTr("ON")
                        width: Math.max(64, (parent.parent.parent.width - parent.spacing) / 2)
                        height: width
                        checkable: true
                        checked: false
                        font.pointSize: width < 72 ? 14 : 18
                        onClicked: USBSwitch.enableUsbPort(2)

                        palette.button: checked ? checkedConnectedColor : uncheckedButtonColor
                    }
                }
            }
        }

            Item {
                width: Math.max(140, controlCardWidth)
                height: port3Column.implicitHeight

            Column {
                id: port3Column
                anchors.horizontalCenter: parent.horizontalCenter
                spacing: 12

                Label {
                    text: qsTr("Port 3")
                    width: parent.parent.width
                    horizontalAlignment: Text.AlignHCenter
                    wrapMode: Text.Wrap
                    color: "white"
                    font.pointSize: 20
                    font.bold: true
                }

                Row {
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12

                    RoundButton {
                        id: port3off
                        text: qsTr("OFF")
                        width: Math.max(64, (parent.parent.parent.width - parent.spacing) / 2)
                        height: width
                        checkable: true
                        checked: false
                        font.pointSize: width < 72 ? 14 : 18
                        onClicked: USBSwitch.disableUsbPort(3)

                        palette.button: checked ? checkedUnconnectedColor : uncheckedButtonColor
                    }

                    RoundButton {
                        id: port3on
                        text: qsTr("ON")
                        width: Math.max(64, (parent.parent.parent.width - parent.spacing) / 2)
                        height: width
                        checkable: true
                        checked: false
                        font.pointSize: width < 72 ? 14 : 18
                        onClicked: USBSwitch.enableUsbPort(3)

                        palette.button: checked ? checkedConnectedColor : uncheckedButtonColor
                    }
                }
            }
            }
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
