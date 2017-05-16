//    resources/FDR/Plot.qml is part of STx
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
import QtCharts 2.1

Rectangle {


    ChartView {
        id: chart

        title: ""
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

        AreaSeries {
            id: spectrum
            name: "spectrum"
            axisX: axisX
            axisY: axisY
            useOpenGL: false
        }

        MouseArea {
            anchors.fill: parent;

            onPressed: {
                var pos = chart.mapToValue(Qt.point(mouseX, mouseY), spectrum)
                chart.title = pos.x.toFixed(1) + "m: " + pos.y.toFixed(1)
            }

            onMouseXChanged: {
                var pos = chart.mapToValue(Qt.point(mouseX, mouseY), spectrum)
                chart.title = pos.x.toFixed(1) + "m: " + pos.y.toFixed(1)
            }

            onReleased: {
                chart.title = ""
            }
        }

        Component.onCompleted: {
            devicesModel.setSpectrum(chart.series(0))
        }

        Connections {
            target: devicesModel
            onFdrSpectrum: {
                axisX.min = 0.8*left;
                axisX.max = 1.2*right;
                axisY.max = hi;
            }
        }
    }
}
