import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

ScrollView {
    property var listmodel: []

    Layout.fillWidth: true
    Layout.preferredHeight: 60
    Layout.columnSpan: 8
    Layout.column: 0
    Layout.row: 8

    ListView {
        spacing: 5

        orientation: Qt.Horizontal
        model: listmodel

        delegate: Rectangle {
            width: 113
            color: "gray"

            anchors.bottom: parent.bottom
            anchors.top: parent.top

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                text: len + "m, " + level + "dB"
                color: "black"
            }
        }
    }
}
