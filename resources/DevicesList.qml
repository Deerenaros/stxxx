//    resources/DevicesList.qml is part of STx
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

Rectangle {
    id: devices

    signal choosed(int index)

    ListView {
        id: devicesList
        anchors.fill: parent
        anchors.margins: 5

        Component {
            id: deviceDelegate
            Rectangle {
                id: wrapper
                width: 80
                height: 40
                color: ListView.isCurrentItem ? "black" : "white"
                Column {
                    Text {
                        color: wrapper.ListView.isCurrentItem ? "white" : "black"
                        font.pointSize: 10
                        text: "<b>" + name + "</b>"
                    }
                    Text {
                        color: wrapper.ListView.isCurrentItem ? "white" : "black"
                        font.capitalization: Font.AllUppercase
                        font.pointSize: 8
                        text: did
                    }
                }

                MouseArea {
                    z: 100500
                    anchors.fill: parent
                    onClicked: {
                        devicesList.currentIndex = index
                        choosed(index)
                    }
                }
            }
        }

        model: devicesModel

        delegate: deviceDelegate
        highlight: Rectangle {
            width: parent.width
            color: "lightsteelblue"
        }
    }
}
