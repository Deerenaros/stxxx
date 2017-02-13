import QtQuick 2.0

Rectangle {
    width: 100
    height: 20

    color: Qt.rgba(0.20784314,  0.21176471,  0.21568627, 1)

    property int first: 0
    property int second: 0

    Text {
        text: first + "-" + second

        color: "white"

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }
}
