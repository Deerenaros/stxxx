import QtQuick 2.7
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

Item {
    id: main
    width: 1024
    height: 640
    property bool sourceLoaded: false

    onWidthChanged: {
        // TODO: let's improve redraw on resize
    }

    ColumnLayout {
        anchors.fill: parent

        DeviceView {
            id: device

            Layout.fillHeight: true
            Layout.fillWidth: true
        }

        StatusBar {}
    }
}
