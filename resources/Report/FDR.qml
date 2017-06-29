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
            role: "fdr:12"
            title: "1-2"
            width: 75
        }

        TableViewColumn {
            role: "fdr:13"
            title: "1-3"
            width: 75
        }

        TableViewColumn {
            role: "fdr:14"
            title: "1-4"
            width: 75
        }

        TableViewColumn {
            role: "fdr:15"
            title: "1-5"
            width: 75
        }

        TableViewColumn {
            role: "fdr:16"
            title: "1-6"
            width: 75
        }

        TableViewColumn {
            role: "fdr:17"
            title: "1-7"
            width: 75
        }

        TableViewColumn {
            role: "fdr:18"
            title: "1-8"
            width: 75
        }

        TableViewColumn {
            role: "fdr:23"
            title: "2-3"
            width: 75
        }

        TableViewColumn {
            role: "fdr:24"
            title: "2-4"
            width: 75
        }

        TableViewColumn {
            role: "fdr:25"
            title: "2-5"
            width: 75
        }

        TableViewColumn {
            role: "fdr:26"
            title: "2-6"
            width: 75
        }

        TableViewColumn {
            role: "fdr:27"
            title: "2-7"
            width: 75
        }

        TableViewColumn {
            role: "fdr:28"
            title: "2-8"
            width: 75
        }

        TableViewColumn {
            role: "fdr:34"
            title: "3-4"
            width: 75
        }

        TableViewColumn {
            role: "fdr:35"
            title: "3-5"
            width: 75
        }

        TableViewColumn {
            role: "fdr:36"
            title: "3-6"
            width: 75
        }

        TableViewColumn {
            role: "fdr:37"
            title: "3-7"
            width: 75
        }

        TableViewColumn {
            role: "fdr:38"
            title: "3-8"
            width: 75
        }

        TableViewColumn {
            role: "fdr:45"
            title: "4-5"
            width: 75
        }

        TableViewColumn {
            role: "fdr:46"
            title: "4-6"
            width: 75
        }

        TableViewColumn {
            role: "fdr:47"
            title: "4-7"
            width: 75
        }

        TableViewColumn {
            role: "fdr:48"
            title: "4-8"
            width: 75
        }

        TableViewColumn {
            role: "fdr:56"
            title: "5-6"
            width: 75
        }

        TableViewColumn {
            role: "fdr:57"
            title: "5-7"
            width: 75
        }

        TableViewColumn {
            role: "fdr:58"
            title: "5-8"
            width: 75
        }

        TableViewColumn {
            role: "fdr:67"
            title: "6-7"
            width: 75
        }

        TableViewColumn {
            role: "fdr:68"
            title: "6-8"
            width: 75
        }

        TableViewColumn {
            role: "fdr:78"
            title: "7-8"
            width: 75
        }

        model: devicesModel
    }
}
