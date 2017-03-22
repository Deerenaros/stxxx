//    resources/Pin.qml is part of STx
//
//    STx is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    STx is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.


import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: pin

    property int first: 0
    property int second: 0
    property var value: null
    property bool usable: true
    property var unselectColor: Qt.rgba(0.20784314,  0.21176471,  0.21568627, 1)
    property var selectColor: "lightgray"

    width: 100
    Layout.preferredHeight: 14
    Layout.fillHeight: true

    color: (usable ? unselectColor: "darkslategrey")

    Text {
        id: txt

        text: (value ? value : first + "-" + second)

        color: (usable ? "white" : "lightgray")

        anchors.horizontalCenter: parent.horizontalCenter
        anchors.verticalCenter: parent.verticalCenter
    }

    MouseArea {
        anchors.fill: parent
        onClicked: {
            select()
        }
    }

    function select () {
        if(usable) {
            if(parent.parent.selected) {
                parent.parent.selected.deselect()
            }

            if(devicesModel.automate) {
                devicesModel.automate = false
            }

            devicesModel.setPins(first, second)

            txt.color = "black"
            pin.color = pin.selectColor

            parent.parent.selected = pin
        }
    }

    function deselect() {
        txt.color = "white"
        pin.color = Qt.rgba(0.20784314,  0.21176471,  0.21568627, 1)
    }
}
