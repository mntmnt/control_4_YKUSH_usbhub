import QtQuick
import QtQuick.Controls
import QtQuick.Window
import com.project.USBSwitch

Item {
    id: root

    readonly property color uncheckedButtonColor:    "#ADADAD"
    readonly property color checkedConnectedColor:   "#2AA63E"
    readonly property color checkedUnconnectedColor: "salmon"
    readonly property int controlsSpacing: 20
    readonly property real controlCardWidth: Math.max(140, Math.min(220, width))
    readonly property real minimumButtonDiameter: 7 * Math.max(Screen.pixelDensity, 96 / 25.4)
    readonly property real fitScale: Math.min(
        1,
        width / Math.max(1, portsFlow.childrenRect.width),
        height / Math.max(1, portsFlow.childrenRect.height)
    )
    readonly property real contentScale: Math.max(
        Math.min(1, minimumButtonDiameter / Math.max(1, port1off.width)),
        fitScale
    )

    clip: true

    function updateState(port, enabled) {
        console.assert(port === 1 || port === 2 || port === 3, "port must be 1, 2 or 3");

        const portsOff = [port1off, port2off, port3off]
        const portsOn = [port1on, port2on, port3on]
        const index = (port - 1) % 3

        portsOff[index].checked = !enabled
        portsOn[index].checked = enabled
    }

    Flow {
        id: portsFlow
        flow: Flow.TopToBottom
        width: childrenRect.width
        height: root.height
        scale: root.contentScale
        transformOrigin: Item.TopLeft
        x: Math.max(0, (root.width - (childrenRect.width * scale)) / 2)
        y: Math.max(0, (root.height - (childrenRect.height * scale)) / 2)
        spacing: controlsSpacing

        Item {
            width: controlCardWidth
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
            width: controlCardWidth
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
            width: controlCardWidth
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
