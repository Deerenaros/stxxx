import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Item {
    Layout.fillHeight: true
    Layout.fillWidth: true

    ColumnLayout {
        anchors.fill: parent

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

        Button {
            text: devicesModel.automate ? qsTr("Stop") : qsTr("Scan")

            onClicked: {
                devicesModel.automate = !devicesModel.automate
            }
        }
    }
}
