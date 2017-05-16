//    resources/DevicesView.qml is part of STx
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
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

import "FDR" as FDR
import "NLD" as NLD
import "Report" as Report

RowLayout {
    id: deviceView

    property bool isReportViewing: false


    Rectangle {
        color: "white"
        z: 100500

        Layout.bottomMargin: 0
        Layout.fillHeight: true
        Layout.preferredWidth: 130

        ListView {
            id: bar
            x: 5
            y: 50
            width: parent.width - 10
            height: parent.height - 100

            interactive: false

            currentIndex: 0

            VisualItemModel {
                id: vimodel

                TabBtn {
                    index: 0
                    text: qsTr("OFFLINE")
                }

                TabBtn {
                    index: 1
                    text: qsTr("Settings")
                }

                TabBtn {
                    index: 3
                    text: qsTr("Amplifier")
                }

                TabBtn {
                    index: 4
                    text: qsTr("Receiver")
                }

                TabBtn {
                    index: 5
                    text: qsTr("NLD")
                }

                TabBtn {
                    index: 6
                    text: qsTr("FDR")
                }

                TabBtn {
                    id: report
                    text: qsTr("Report")

                    fired: function() {
                        isReportViewing = !isReportViewing
                        report.offColor = isReportViewing ? "red" : "lightgray"
                    }
                }

                TabBtn {
                    text: qsTr("To Report")

                    fired: function() {
                        devicesModel.toReport()
                    }
                }
            }

            model: vimodel
        }
    }

    SwipeView {
        id: tabs

        interactive: false
        visible: !deviceView.isReportViewing

        Layout.fillHeight: true
        Layout.fillWidth: true

        currentIndex: bar.currentIndex

        OffMode {
        }

        Settings {
        }

        Settings {
        }

        Amplifier {
        }

        Receiver {
        }

        NLD.Main {
        }

        FDR.Main {
        }
    }

    SwipeView {
        id: reports

        interactive: false
        visible: deviceView.isReportViewing

        Layout.fillHeight: true
        Layout.fillWidth: true

        currentIndex: bar.currentIndex

        OffMode {
        }

        Report.Main {
            name: "settings"
        }

        Report.Main {
            name: "settings"
        }

        Report.Main {
            name: "amplifier"
        }

        Report.Main {
            name: "receiver"
        }

        Report.Main {
            name: "nld"
        }

        Report.Main {
            name: "fdr"
        }
    }
}
