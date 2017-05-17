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
            role: "fdr12"
            title: "1-2"
        }

        TableViewColumn {
            role: "fdr13"
            title: "1-3"
        }

        TableViewColumn {
            role: "fdr14"
            title: "1-4"
        }

        TableViewColumn {
            role: "fdr15"
            title: "1-5"
        }

        TableViewColumn {
            role: "fdr16"
            title: "1-6"
        }

        TableViewColumn {
            role: "fdr17"
            title: "1-7"
        }

        TableViewColumn {
            role: "fdr18"
            title: "1-8"
        }

        TableViewColumn {
            role: "fdr23"
            title: "2-3"
        }

        TableViewColumn {
            role: "fdr24"
            title: "2-4"
        }

        TableViewColumn {
            role: "fdr25"
            title: "2-5"
        }

        TableViewColumn {
            role: "fdr26"
            title: "2-6"
        }

        TableViewColumn {
            role: "fdr27"
            title: "2-7"
        }

        TableViewColumn {
            role: "fdr28"
            title: "2-8"
        }

        TableViewColumn {
            role: "fdr34"
            title: "3-4"
        }

        TableViewColumn {
            role: "fdr35"
            title: "3-5"
        }

        TableViewColumn {
            role: "fdr36"
            title: "3-6"
        }

        TableViewColumn {
            role: "fdr37"
            title: "3-7"
        }

        TableViewColumn {
            role: "fdr38"
            title: "3-8"
        }

        TableViewColumn {
            role: "fdr45"
            title: "4-5"
        }

        TableViewColumn {
            role: "fdr46"
            title: "4-6"
        }

        TableViewColumn {
            role: "fdr47"
            title: "4-7"
        }

        TableViewColumn {
            role: "fdr48"
            title: "4-8"
        }

        TableViewColumn {
            role: "fdr56"
            title: "5-6"
        }

        TableViewColumn {
            role: "fdr57"
            title: "5-7"
        }

        TableViewColumn {
            role: "fdr58"
            title: "5-8"
        }

        TableViewColumn {
            role: "fdr67"
            title: "6-7"
        }

        TableViewColumn {
            role: "fdr68"
            title: "6-8"
        }

        TableViewColumn {
            role: "fdr78"
            title: "7-8"
        }

        model: report.model
    }
}
