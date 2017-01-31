import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: switchItem

    color: "black"

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
