//    resources/Amplifier.qml is part of STx
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
import QtCharts 2.1
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

GridLayout {
    id: amplifier

    rows: 2
    columns: 2

    Oscilloscope {
        id: amplOscilloscope

        Layout.row: 0
        Layout.column: 1

        Layout.fillHeight: true
        Layout.fillWidth: true

        Component.onCompleted: {
            devicesModel.setAmpl(amplOscilloscope.series(0))
        }

        Connections {
            target: devicesModel


            onAmplifierSignal: {
                if(count > 1024) {
                    amplOscilloscope.updateMinMax(count)
                }
            }
        }
    }

    Slider {
        Layout.row: 0
        Layout.column: 0
        Layout.preferredWidth: 20
        Layout.fillHeight: true

        orientation: Qt.Vertical
    }

    Slider {
        Layout.row: 1
        Layout.column: 1
        Layout.fillWidth: true
        Layout.preferredHeight: 20
    }
}
