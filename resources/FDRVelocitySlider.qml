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
        }

        Text {
            Layout.fillHeight: true
            Layout.preferredWidth: 50

            text: slider.value.toString().substring(0, 4)
        }
    }
}
