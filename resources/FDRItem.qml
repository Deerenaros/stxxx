import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Rectangle {
    id: switchItem

    property string text: "fre"
    property string textColor: "black"
    property bool busy: true
    property bool clickable: true
    property int c: 0
    property int r: 0
    property var measurments: []

    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.columnSpan: 1
    Layout.rowSpan: 1

    Text {
        id: txt
        visible: !busy

        text: parent.text
        color: parent.textColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            if(clickable) {
                parent.parent.onMouseChoose(c, r)
            }
        }
    }
}
