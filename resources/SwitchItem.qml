import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: switchItem

    property string text: ""
    property string textColor: "black"

    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.columnSpan: 1
    Layout.rowSpan: 1

    Text {
        id: txt

        text: parent.text
        color: parent.textColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
}
