import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    property var pins: [];

    GridLayout {
        id: pinslayout

        flow: GridLayout.TopToBottom

        rows: 8

        Component.onCompleted: {
            for(var i = 0; i < pinslayout.rows-1; i++) {
                pins.push([])
                for(var j = i+1; j < pinslayout.rows; j++) {
                    var component = Qt.createComponent("Pin.qml")
                    if(component !== null && component.status === Component.Ready) {
                        pins[pins.length-1].push(component.createObject(component, {first: i, second: j}))
                    } else if(component !== null && component.status === Component.Error) {
                        console.debug(component.errorString())
                    }
                }
            }
        }
    }
}
