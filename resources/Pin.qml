import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: pin

    property int first: 0
    property int second: 0
    property bool usable: true

    width: 100
    Layout.preferredHeight: 14
    Layout.fillHeight: true

    color: (usable ? Qt.rgba(0.20784314,  0.21176471,  0.21568627, 1) : "darkslategrey")

    Text {
        id: txt

        text: first + "-" + second

        color: (usable ? "white" : "lightgray")

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
        if(usable) {
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
    }

    function deselect() {
        txt.color = "white"
        pin.color = Qt.rgba(0.20784314,  0.21176471,  0.21568627, 1)
    }
}
