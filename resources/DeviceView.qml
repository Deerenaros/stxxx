import QtQuick 2.0
import QtQuick.Controls 2.0
import QtQuick.Layouts 1.0

GridLayout {
    id: deviceView

    columnSpacing: 0
    rowSpacing: 0

    rows: 3
    columns: 1

    property var openedDevices: createArray(devicesModel.count, 0)

    TabBar {
        id: bar

        Layout.preferredHeight: 60
        Layout.fillWidth: true
        Layout.column: 1
        Layout.columnSpan: 1
        Layout.row: 1
        Layout.rowSpan: 1

        currentIndex: tabs.currentIndex

        TabButton {
            text: qsTr("OFFLINE")
        }

        TabButton {
            text: qsTr("Settings")
        }

        TabButton {
            text: qsTr("Switch")
        }

        TabButton {
            text: qsTr("Amplifier")
        }

        TabButton {
            text: qsTr("Receiver")
        }

        TabButton {
            text: qsTr("NLD")
        }

        TabButton {
            text: qsTr("FDR")
        }
    }

    SwipeView {
        id: tabs

        Layout.fillHeight: true
        Layout.fillWidth: true
        Layout.column: 1
        Layout.columnSpan: 1
        Layout.row: 2
        Layout.rowSpan: 1

        focus: false
        focusPolicy: Qt.NoFocus
        currentIndex: bar.currentIndex

        onCurrentIndexChanged: {
            openedDevices[devicesModel.current] = currentIndex
            devicesModel.setModeForCurrent(currentIndex)
        }

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
    }

    StatusBar {
        Layout.topMargin: 10
        Layout.preferredHeight: 25
        Layout.fillWidth: true

        Layout.column: 1
        Layout.columnSpan: 1
        Layout.row: 3
        Layout.rowSpan: 1
    }

    function createArray(len, item) {
        var arr = [];
        for(var i = 0; i < len; i++) {
            arr.push(item)
        }
        return arr;
    }

    function myChangeIndex(index) {
        bar.currentIndex = openedDevices[index];
    }
}
