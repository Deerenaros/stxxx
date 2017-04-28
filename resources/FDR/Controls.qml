//    resources/FDR/Controls.qml is part of STx
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


    RowLayout {
        anchors.fill: parent

        ColumnLayout {
            Button {
                text: "Load"
            }

            Button {
                text: "Save"
            }

            Button {
                text: "Compare"
            }
        }

        ColumnLayout {
            Button {
                text: devicesModel.automate ? qsTr("Stop") : qsTr("Scan")

                onClicked: {
                    var auto = !devicesModel.automate
                    devicesModel.automate = auto
                    if(auto) {
                        devicesModel.retake()
                    }
                }
            }

            Button {
                text: "Analysis"
            }

            SpinBox {
                id: spin

                Layout.preferredWidth: 50

                value: 1.5
                decimals: 2
                stepSize: 0.01

                minimumValue: 0.3
                maximumValue: 3.1

                onValueChanged: {
                    devicesModel.setVelocityFactor(value >= 1 ? value : 1.0/value)
                }
            }
        }
    }
}
