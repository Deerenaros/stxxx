//    resources/Report/Main.qml is part of STx
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
import QtQuick.Controls 1.4

Item {
    id: nld
    property string name: "none"

    TableView {
        anchors.fill: parent

        TableViewColumn {
            role: "n"
            title: qsTr("Number")
        }

        TableViewColumn {
            role: "len"
            title: qsTr("Length")
        }

        TableViewColumn {
            role: "lvl"
            title: qsTr("Signal Level")
        }

        model: []//deviceView.reportModel(name)
    }
}
