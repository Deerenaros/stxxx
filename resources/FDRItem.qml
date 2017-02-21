import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

Item {
    id: switchItem

    property real len: 0
    property real lvl: 0
    property int first: 0
    property int second: 0

    property string textColor: "black"
    property bool busy: false
    property var measurments: []
    property var colors: ["silver", "gainsboro"]
    property var pairColors: ["tan", "wheat"]
    property real g: lvl/parent.maxlvl

    Layout.fillHeight: true
    Layout.fillWidth: true

    function isPair(a, b) {
        return (a === 1 && b === 2)
            || (a === 3 && b === 6)
            || (a === 4 && b === 5)
            || (a === 7 && b === 8)
    }

    Component.onCompleted: {
        if(isPair(first, second)) {
            colors = pairColors
        }
    }

    LinearGradient {
        anchors.fill: parent
        start: Qt.point(0, 0)
        end: Qt.point(parent.width, 0)

        gradient: Gradient {
            GradientStop {
                position: 0
                color: switchItem.colors[0]
            }

            GradientStop {
                position: Math.min(0.99, g)
                color: switchItem.colors[0]
            }

            GradientStop {
                position: Math.min(0.999, g + 0.001)
                color: switchItem.colors[1]
            }

            GradientStop {
                position: 1
                color: switchItem.colors[1]
            }
        }
    }

    MultiText {
        id: txt

        vAlignment: Text.Center
        text: (busy ? "..." : first + "-" + second + "\n" + len + "m, " + lvl + "dB")
        color: textColor
        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        anchors.fill: parent

        onClicked: {
            devicesModel.automate = !devicesModel.automate
        }
    }
}
