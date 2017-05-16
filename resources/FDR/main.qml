//    resources/FDR/Main.qml is part of STx
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

import "../PairSelector.qml" as PairSelector

Item {
    id: fdr

    property var win: null

    GridLayout {
        id: results

        property var currentSet: firstSet

        anchors.fill: parent

        columnSpacing: 15
        rowSpacing: 15

        rows: 2
        columns: 4

        Results {
            id: firstSet

            set: 1
            active: true

            Layout.column: 0
            Layout.row: 0

            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Results {
            id: secondSet

            set: 2
            active: false

            Layout.column: 1
            Layout.row: 0

            Layout.fillWidth: true
            Layout.fillHeight: true
        }

        Controls {
            Layout.column: 2
            Layout.row: 0

            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        Plot {
            Layout.column: 0
            Layout.columnSpan: 3
            Layout.row: 1

            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        PairSelector {
            Layout.column: 3
            Layout.row: 0
            Layout.rowSpan: 2

            values: true
        }
    }
}
