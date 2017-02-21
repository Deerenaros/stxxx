import QtQuick 2.0

Rectangle {
    id: pin

    width: 100
    height: 20

    color: Qt.rgba(0.20784314,  0.21176471,  0.21568627, 1)

    property int first: 0
    property int second: 0

    Text {
        id: txt

        text: first + "-" + second

        color: "white"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            select()
        }
    }

    function select () {
        if(parent.parent.parent.selected) {
            parent.parent.parent.selected.deselect()
        }

        if(!devicesModel.automate) {
            devicesModel.setPins(first, second)
        }

        txt.color = "black"
        pin.color = "lightgray"

        parent.parent.parent.selected = pin
    }

    function deselect() {
        txt.color = "white"
        pin.color = Qt.rgba(0.20784314,  0.21176471,  0.21568627, 1)
    }
}
