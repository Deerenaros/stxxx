//    resources/MultiText.qml is part of STx
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

ColumnLayout {
    id: layout

    property string text: ""
    property int hAlignment: Text.AlignHCenter
    property int vAlignment: Text.AlignVCenter
    property string color: "black"

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
