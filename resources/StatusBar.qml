import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: switchItem

    Layout.topMargin: 0
    Layout.preferredHeight: 25
    Layout.fillWidth: true

    color: Qt.rgba(0.20784314,  0.21176471,  0.21568627, 1)

    Text {
        id: txt
        text: "offline"

        color: "white"

        anchors.right: switchItem.right
        anchors.verticalCenter: parent.verticalCenter
    }

    Connections {
        target: devicesModel

        onStatusSignal: {
            if(charge < 0) {
                txt.text = "offline"
            } else {
                txt.text = (isCharging ? "charging" : "discharging")
                txt.text += ": " + charge + "%"
            }
        }
    }
}
