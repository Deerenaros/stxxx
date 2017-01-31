import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4

Item {
    GridLayout {
        property var items: [];

        id: switcher

        anchors.fill: parent

        columnSpacing: 5
        rowSpacing: 5

        rows: 8
        columns: 8

        Component.onCompleted: {
            for(var c = 0; c < switcher.rows; c++) {
                items.push([])
                for(var r = 0; r < switcher.columns; r++) {
                    var component = Qt.createComponent("SwitchItem.qml")
                    if(component !== null) {
                        var attrs = {
                            "Layout.column": c,
                            "Layout.row": r,
                            text: (c+1) + " " + r,

                            color: "gray",
                            textColor: "black",
                        }

                        if(c == 0 && r == 0) {
                            attrs.text = qsTr("INPUTS\\OUTPUTS")
                            attrs.color = "black"
                            attrs.textColor = "white"
                        } else if(c == 0 || r == 0) {
                            attrs.color = "black"
                            attrs.textColor = "white"

                            if(c != 0) {
                                attrs.text = (c+1).toString()
                            } else {
                                attrs.text = r.toString()
                            }
                        }

                        items[items.length-1].push(component.createObject(switcher, attrs))
                    }
                }
            }
        }

        Connections {
            target: devicesModel
            onSwitcherSignal: {
                switcher.items[output-1][input].text = ac + "/" + dc
            }
        }
    }
}
