import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Item {
    Layout.preferredHeight: 30
    Layout.fillWidth: true

    RowLayout {
        anchors.fill: parent

        Slider {
            id: slider

            Layout.fillHeight: true
            Layout.fillWidth: true

            value: 1.25
            minimumValue: 0.3
            maximumValue: 3.1

            onPressedChanged: {
                if(!pressed) {
                    devicesModel.setVelocityFactor(value >= 1 ? value : 1.0/value)
                }
            }

            onValueChanged: {
                if(!pressed) {
                    devicesModel.setVelocityFactor(value >= 1 ? value : 1.0/value)
                }

                spin.value = value
            }
        }

        SpinBox {
            id: spin

            Layout.fillHeight: true
            Layout.preferredWidth: 50

            value: 1.25
            decimals: 2
            stepSize: 0.01

            minimumValue: 0.3
            maximumValue: 3.1

            onValueChanged: {
                slider.value = value
            }
        }
    }
}
