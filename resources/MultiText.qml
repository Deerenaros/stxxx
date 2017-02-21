import QtQuick 2.0
import QtQuick.Layouts 1.1

ColumnLayout {
    id: layout

    property string text: ""
    property int hAlignment: Text.AlignHCenter
    property int vAlignment: Text.AlignVCenter
    property var color: "black"

    anchors.fill: parent

    Repeater {
        id: repeater
        model: text.split(/[\r\n]/g);

        Text {
            text: repeater.model[index]
            color: layout.color
            horizontalAlignment: hAlignment
            verticalAlignment: vAlignment
            Layout.fillWidth: true
        }
    }
}
