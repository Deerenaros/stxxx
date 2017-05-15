//    resources/Report/FDR.qml is part of STx
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
import QtQuick.Controls 1.4

Item {
    id: report

    property var model: []

    anchors.fill: parent

    TableView {
        id: table

        anchors.fill: parent

        TableViewColumn {
            role: "1"
            title: "1-2"
        }

        TableViewColumn {
            role: "2"
            title: "1-3"
        }

        TableViewColumn {
            role: "3"
            title: "1-4"
        }

        TableViewColumn {
            role: "4"
            title: "1-5"
        }

        TableViewColumn {
            role: "5"
            title: "1-6"
        }

        TableViewColumn {
            role: "6"
            title: "1-7"
        }

        TableViewColumn {
            role: "7"
            title: "1-8"
        }

        TableViewColumn {
            role: "8"
            title: "2-3"
        }

        TableViewColumn {
            role: "9"
            title: "2-4"
        }

        TableViewColumn {
            role: "10"
            title: "2-5"
        }

        TableViewColumn {
            role: "11"
            title: "2-6"
        }

        TableViewColumn {
            role: "12"
            title: "2-7"
        }

        TableViewColumn {
            role: "13"
            title: "2-8"
        }

        TableViewColumn {
            role: "14"
            title: "3-4"
        }

        TableViewColumn {
            role: "15"
            title: "3-5"
        }

        TableViewColumn {
            role: "16"
            title: "3-6"
        }

        TableViewColumn {
            role: "17"
            title: "3-7"
        }

        TableViewColumn {
            role: "18"
            title: "3-8"
        }

        TableViewColumn {
            role: "19"
            title: "4-5"
        }

        TableViewColumn {
            role: "20"
            title: "4-6"
        }

        TableViewColumn {
            role: "21"
            title: "4-7"
        }

        TableViewColumn {
            role: "22"
            title: "4-8"
        }

        TableViewColumn {
            role: "23"
            title: "5-6"
        }

        TableViewColumn {
            role: "24"
            title: "5-7"
        }

        TableViewColumn {
            role: "25"
            title: "5-8"
        }

        TableViewColumn {
            role: "26"
            title: "6-7"
        }

        TableViewColumn {
            role: "27"
            title: "6-8"
        }

        TableViewColumn {
            role: "28"
            title: "7-8"
        }

        model: report.model
    }
}
