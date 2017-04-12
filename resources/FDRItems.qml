//    resources/FDRItems.qml is part of STx
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
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

ScrollView {
    property var listmodel: []

    Layout.fillWidth: true
    Layout.preferredHeight: 60
    Layout.columnSpan: 8
    Layout.column: 0
    Layout.row: 8

    ListView {
        spacing: 5

        orientation: Qt.Horizontal
        model: listmodel

        delegate: Rectangle {
            width: 113
            color: "gray"

            anchors.bottom: parent.bottom
            anchors.top: parent.top

            Text {
                anchors.verticalCenter: parent.verticalCenter
                anchors.horizontalCenter: parent.horizontalCenter

                text: len + "m, " + level + "dB"
                color: "black"
            }
        }
    }
}
