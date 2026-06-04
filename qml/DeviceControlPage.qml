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
            portControlsArea.updateState(port, enabled)
        }

        function onErrorReported(message) { // Signal handler for mySignal
            lastRestatus.text = qsTr("ERROR: ") + message;
            lastRestatus.color = "#FB2C36";
        }
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

    PortControlsArea {
        id: portControlsArea
        anchors.top: infoLabel.bottom
        anchors.bottom: lastRestatus.top
        anchors.left: parent.left
        anchors.right: parent.right
        anchors.topMargin: 20
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20
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
