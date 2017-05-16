//    resources/TabBtn.qml is part of STx
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
//    along with STx.  If not, see <http://www.gnu.org/licenses/>.

import QtQuick 2.0
import QtQuick.Layouts 1.1

Rectangle {
    id: me

    property string text: ""
    property int index: -1
    property bool active: index === parent.parent.currentIndex
    property var fired: null
    property string offColor: "lightgray"
    property string onColor: Qt.rgba(0.20784314,  0.21176471,  0.21568627, 1)

    height: parent.parent.height/(parent.parent.count)
    width: 120
    color: active || index < 0 ? offColor : onColor

    border.width: index < 0 ? 0 : 1
    border.color: "white"// active ? onColor : offColor

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
                    me.fired();
                }
            } else {
                parent.parent.parent.currentIndex = index
                devicesModel.setMode(index)
            }
        }
    }
}
