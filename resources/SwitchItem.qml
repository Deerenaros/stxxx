import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: switchItem

    property string text: ""
    property string textColor: "black"
    property string bg: "black"
    property string selectedbg: "red"
    property int i: 0
    property int j: 0
    property bool clickable: false
    property bool selected: false

    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.columnSpan: 1
    Layout.rowSpan: 1

    color: (selected ? selectedbg : bg)

    Text {
        id: txt

        color: textColor
        text: parent.text
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    function toggle() {
        if(parent.selected !== null) {
            parent.selected.selected = false
        }

        parent.selected = switchItem
        devicesModel.setPins(i, j)
        selected = true
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            if(clickable) {
                toggle()
            }
        }
    }
}
