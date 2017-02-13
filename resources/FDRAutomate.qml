import QtQuick 2.0
import QtQuick.Window 2.1
Window {
    width: 300
    height: 200

    Rectangle {
        anchors.fill: parent
        color: "lightgray"

        Text {
            anchors.centerIn: parent
            text: qsTr("FDR Automate")
        }
    }
}
