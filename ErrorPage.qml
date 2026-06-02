import QtQuick
import QtQuick.Controls

Item {
    height: parent ? parent.height : 0
    width:  parent ? parent.width  : 0

    required property bool manyDevices
    required property var  deviceList

    Component.onCompleted: {
        console.log(">>> " + deviceList.length)
        console.log(">>> " + deviceList)
    }

    Rectangle {
        id: bg
        anchors.fill: parent
        color: "red"
    }

    Label {
        id: errorTitle
        anchors.top:  parent.top
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.topMargin: 100
        color: "white"
        font.bold: true
        font.pointSize: 24
        text: manyDevices ? qsTr("Too many devices connected") : qsTr("Some error")
    }

    ListView {
        id: filesListView
        clip: true
        anchors.fill: parent
        anchors.topMargin: 100 + errorTitle.height + 50
        anchors.bottomMargin: 20
        anchors.leftMargin: 20
        anchors.rightMargin: 20
        spacing: 10

        Component {
            id: contactsDelegate
            Rectangle {
                id: wrapper
                width: parent.width
                height: contactInfo.height
                color: "orange"
                Text {
                    id: contactInfo
                    text: index + ": " + modelData
                    color: "white"
                    font.bold: true
                    font.pointSize: 12
                }
            }
        }

        model: deviceList
        delegate: contactsDelegate
    }
}
