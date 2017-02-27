import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

GridLayout {
    id: deviceView

    columnSpacing: 0
    rowSpacing: 0

    rows: 2
    columns: 2

    Rectangle {
        color: "white"
        z: 100500

        Layout.bottomMargin: 0
        Layout.fillHeight: true
        Layout.preferredWidth: 130
        Layout.column: 0
        Layout.columnSpan: 1
        Layout.row: 0
        Layout.rowSpan: 2

        ListView {
            id: bar
            x: 5
            y: 50
            width: parent.width - 10
            height: parent.height - 100

            interactive: false

            currentIndex: tabs.currentIndex

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
                    index: 2
                    text: qsTr("Switch")
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
                    index: 7
                    text: qsTr("Report")
                }

                TabBtn {
                    index: -1
                    text: qsTr("To Report")
                }
            }

            model: vimodel
        }
    }

    SwipeView {
        id: tabs

        interactive: false

        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.column: 1
        Layout.columnSpan: 1
        Layout.row: 0
        Layout.rowSpan: 2

        currentIndex: bar.currentIndex

        OffMode {
        }

        Settings {
        }

        Switcher {
        }

        Amplifier {
        }

        Receiver {
        }

        NLD {
        }

        FDR {
        }

        NLD {
        }
    }
}
