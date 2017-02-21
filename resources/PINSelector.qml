import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

Item {
    property var pins: []
    property var selected: null

    Layout.fillWidth: true
    Layout.fillHeight: true

    ColumnLayout {
        spacing: 15

        GridLayout {
            id: pinslayout

            flow: GridLayout.TopToBottom

            rows: 14

            rowSpacing: 1
            columnSpacing: 1

            Component.onCompleted: {
                for(var i = 1; i <= 7; i++) {
                    pins.push([])
                    for(var j = i+1; j <= 8; j++) {
                        var component = Qt.createComponent("Pin.qml")
                        if(component !== null && component.status === Component.Ready) {
                            pins[pins.length-1].push(component.createObject(pinslayout, {first: i, second: j}))
                        } else if(component !== null && component.status === Component.Error) {
                            console.debug(component.errorString())
                        }
                    }
                }
            }
        }

        Button {
            text: "Button"

            onClicked: {
                if(parent.parent.parent.parent.automate) {
                    parent.parent.parent.parent.automate();
                } else {
                    console.debug("automate does not exist in " + parent.parent.parent.parent + " (" + automate + ")")
                }
            }
        }
    }

    Connections {
        target: devicesModel

        onPinsChanged: {
            if(get(a, b)) {
                get(a, b).select()
            }
        }
    }

    function get(i, j) {
        if(pins[i-1] && pins[i-1][j-i-1]) {
            return pins[i-1][j-i-1];
        } return null;
    }
}
