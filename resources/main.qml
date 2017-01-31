import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    id: main
    width: 1024
    height: 600
    property bool sourceLoaded: false

    onWidthChanged: {
        // TODO: let's improve redraw on resize
    }

    RowLayout {
        id: rootLayout

        anchors.fill: parent
        spacing: 0

        DevicesList {
            id: devices
            z: 100

            Layout.fillHeight: true
            Layout.preferredWidth: 90

            onChoosed: {
                device.myChangeIndex(index)
                devicesModel.current = index
            }
        }

        DeviceView {
            id: device

            Layout.fillHeight: true
            Layout.fillWidth: true
        }
    }
}
