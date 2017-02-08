import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1
import QtGraphicalEffects 1.0

Item {
    id: switchItem

    property string text: ""
    property string textColor: "black"
    property bool busy: true
    property bool clickable: true
    property int c: 0
    property int r: 0
    property var measurments: []
    property var colors: ["tan", "wheat"]
    property real lvl: 0.0
    property real g: lvl/parent.maxlvl

    Layout.fillHeight: true
    Layout.fillWidth: true
    Layout.columnSpan: 1
    Layout.rowSpan: 1

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
                position: (g >= 0.99 ? 0.999 : g)
                color: switchItem.colors[0]
            }

            GradientStop {
                position: (g >= 0.99 ? 1 : g + 0.001)
                color: switchItem.colors[1]
            }

            GradientStop {
                position: 1
                color: switchItem.colors[1]
            }
        }
    }

    Text {
        id: txt

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
