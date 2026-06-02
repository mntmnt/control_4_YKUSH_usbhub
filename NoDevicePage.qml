import QtQuick
import QtQuick.Controls

Item {
    height: parent ? parent.height : 0
    width:  parent ? parent.width  : 0

    Rectangle {
        id: bg
        anchors.fill: parent
        color: "red"
    }

    Label {
        anchors.top:  parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 100
        color: "white"
        font.bold: true
        font.pointSize: 24
        text: qsTr("No Device")
    }

    BusyIndicator {
        id: busyIndicator
        anchors.centerIn: parent
    }

}
