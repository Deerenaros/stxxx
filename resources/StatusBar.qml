//    resources/StatusBar.qml is part of STx
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
