//    resources/FDRSignal.qml is part of STx
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
import QtCharts 2.1

Rectangle {
    Layout.fillHeight: true
    Layout.fillWidth: true

    ChartView {
        id: chart

        title: "Line"
        anchors.fill: parent
        antialiasing: true

        ValueAxis {
            id: axisY
            min: 0
            max: 255
        }

        ValueAxis {
            id: axisX
            titleText: "meters"
            min: 0
            max: 520
        }

        LineSeries {
            name: "spectrum"
            axisX: axisX
            axisY: axisY
            useOpenGL: false
        }

        Component.onCompleted: {
            console.log(chart.series(0))
            devicesModel.properties.fdr.series = chart.series(0)
            devicesModel.setSpectrum(chart.series(0))
        }
    }
}
