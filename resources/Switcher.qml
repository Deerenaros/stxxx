import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Item {
    GridLayout {
        property var items: [];
        property var selected: null;

        id: switcher

        anchors.fill: parent

        columnSpacing: 5
        rowSpacing: 5

        rows: 8
        columns: 8

        Component.onCompleted: {
            for(var c = 0; c < switcher.columns; c++) {
                items.push([])
                for(var r = 0; r < switcher.rows; r++) {
                    var component = Qt.createComponent("SwitchItem.qml")
                    if(component !== null && component.status === Component.Ready) {
                        var attrs = {
                            text: (r+1 > c ? c + " " + (r+1) : "#"),
                            i: c, j: r+1,
                            clickable: c != 0 && r != 0 && r+1 > c,
                            selected: false,

                            bg: "gray",
                            textColor: "black",
                        }

                        if(c === 0 || r === 0) {
                            attrs.bg = "black"
                            attrs.textColor = "white"

                            if(c === 0 && r === 0) {
                                attrs.text = qsTr("PINS")
                            } else if(c != 0) {
                                attrs.text = c.toString()
                            } else {
                                attrs.text = (r+1).toString()
                            }
                        }

                        items[items.length-1].push(component.createObject(switcher, attrs))
                    } else if(component !== null && component.status === Component.Error) {
                        console.debug(component.errorString())
                    }
                }
            }
        }

        Connections {
            target: devicesModel
            onSwitcherSignal: {
                switcher.items[a][b-1].text = ac + "/" + dc
                // switcher.items[a][b-1].toggle()
            }
        }
    }
}
