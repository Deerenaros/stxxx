import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    property string text: ""
    property int index: 0
    property bool active: index === parent.parent.currentIndex
    property var clicked: null

    height: parent.parent.height/(parent.parent.count)
    width: 120
    color: active || index < 0 ? "lightgray" : Qt.rgba(0.20784314,  0.21176471,  0.21568627, 1)

    border.width: index < 0 ? 0 : 1
    border.color: active ? "black" : "white"

    Text {
        anchors.verticalCenter: parent.verticalCenter
        anchors.horizontalCenter: parent.horizontalCenter
        text: parent.text
        color: active || index < 0 ? "black" : "white"
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            if(index < 0) {
                if(clicked !== null) {
                    clicked();
                }
            } else {
                parent.parent.parent.currentIndex = index
            }
        }
    }
}
