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
            devicesModel.setSeries(amplOscilloscope.series(0))
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
