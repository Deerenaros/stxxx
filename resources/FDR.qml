//    resources/FDR.qml is part of STx
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

        FDRResults {
            id: firstSet

            set: 1
            active: true

            Layout.column: 0
            Layout.row: 0
        }

        FDRResults {
            id: secondSet

            set: 2
            active: false

            Layout.column: 1
            Layout.row: 0
        }

        FDRControls {
            Layout.column: 2
            Layout.row: 0
        }

        FDRSignal {
            Layout.column: 0
            Layout.columnSpan: 3
            Layout.row: 1
        }

        PINSelector {
            Layout.column: 3
            Layout.row: 0
            Layout.rowSpan: 2

            values: true
        }

        function onAutomateSet(a, b, content) {
            // devicesModel.automate = false
            main.event();

            firstSet.resultModel.clear()
        }
    }
}
