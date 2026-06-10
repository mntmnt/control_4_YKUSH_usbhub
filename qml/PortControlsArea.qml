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
    readonly property int portCount: 3
    readonly property int buttonRowSpacing: 12
    readonly property real referenceButtonWidth: Math.max(64, (controlCardWidth - buttonRowSpacing) / 2)
    readonly property real fitScale: Math.min(
        1,
        width / Math.max(1, portsFlow.childrenRect.width),
        height / Math.max(1, portsFlow.childrenRect.height)
    )
    readonly property real contentScale: Math.max(
        Math.min(1, minimumButtonDiameter / Math.max(1, referenceButtonWidth)),
        fitScale
    )

    clip: true

    function updateState(port, enabled) {
        const portIsAvailable = port === 1 || port === 2 || port === 3;
        console.assert(portIsAvailable, "port must be 1, 2 or 3");
        if (! portIsAvailable ) {
            console.warn("unavailable port passed: " + port);
        }

        const item = portsRepeater.itemAt((port - 1) % root.portCount)
        if (! item) {
            return
        }

        item.offButton.checked = !enabled
        item.onButton.checked = enabled
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

        Repeater {
            id: portsRepeater
            model: root.portCount

            delegate: Item {
                id: portCard

                required property int index
                readonly property int portNumber: index + 1
                property alias offButton: offBtn
                property alias onButton: onBtn

                width: controlCardWidth
                height: portColumn.implicitHeight

                Column {
                    id: portColumn
                    anchors.horizontalCenter: parent.horizontalCenter
                    spacing: 12

                    Label {
                        text: qsTr("Port %1").arg(portCard.portNumber)
                        width: parent.parent.width
                        horizontalAlignment: Text.AlignHCenter
                        wrapMode: Text.Wrap
                        color: "white"
                        font.pointSize: 20
                        font.bold: true
                    }

                    Row {
                        anchors.horizontalCenter: parent.horizontalCenter
                        spacing: root.buttonRowSpacing

                        RoundButton {
                            id: offBtn
                            text: qsTr("OFF")
                            width: Math.max(64, (parent.parent.parent.width - parent.spacing) / 2)
                            height: width
                            checkable: true
                            checked: false
                            font.pointSize: width < 72 ? 14 : 18
                            onClicked: USBSwitch.disableUsbPort(portCard.portNumber)

                            palette.button: checked ? checkedUnconnectedColor : uncheckedButtonColor
                        }

                        RoundButton {
                            id: onBtn
                            text: qsTr("ON")
                            width: Math.max(64, (parent.parent.parent.width - parent.spacing) / 2)
                            height: width
                            checkable: true
                            checked: false
                            font.pointSize: width < 72 ? 14 : 18
                            onClicked: USBSwitch.enableUsbPort(portCard.portNumber)

                            palette.button: checked ? checkedConnectedColor : uncheckedButtonColor
                        }
                    }
                }
            }
        }
    }
}
