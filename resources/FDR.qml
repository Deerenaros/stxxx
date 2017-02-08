import QtQuick 2.0
import QtCharts 2.1
import QtQuick.Layouts 1.1

Item {
    id: fdr


    GridLayout {
        property var items: []
        property var last: null

        property var diags: ["gray", "gray"]
        property var heads: ["black", "black"]
        property var pairs: ["tan", "wheat"]
        property var others: ["silver", "gainsboro"]
        property real maxlvl: 0.1

        id: results

        anchors.fill: parent

        columnSpacing: 5
        rowSpacing: 5

        rows: 9
        columns: 8


        ListModel {
            id: selectedFDR
        }

        function isPair(a, b) {
            return (a === 1 && b === 2) ||
                   (a === 3 && b === 6) ||
                   (a === 4 && b === 5) ||
                   (a === 7 && b === 8);
        }

        Component.onCompleted: {
            for(var c = 0; c < results.columns; c++) {
                items.push([])
                for(var r = 0; r < results.rows-1; r++) {
                    var component = Qt.createComponent("FDRItem.qml")
                    if(component !== null && component.status === Component.Ready) {
                        var attrs = {
                            colors: (isPair(c, r+1) ? pairs : (r == c-1 ? diags : others)),
                            textColor: "black",
                            "c": c,
                            "r": r,
                        }

                        if(c === 0 || r === 0) {
                            attrs.busy = false
                            attrs.colors = heads
                            attrs.textColor = "white"
                            attrs.clickable = false

                            if(c === 0 && r === 0) {
                                attrs.text = qsTr("PINS")
                            } else if(c !== 0) {
                                attrs.text = c.toString()
                            } else {
                                attrs.text = (r+1).toString()
                            }
                        }

                        items[items.length-1].push(component.createObject(results, attrs))
                    } else if(component !== null && component.status === Component.Error) {
                        console.debug(component.errorString())
                    }
                }
            }

            var expand = Qt.createComponent("FDRItems.qml")
            if(expand !== null) {
                expand.createObject(results, {"listmodel": selectedFDR});
            }
        }

        Connections {
            target: devicesModel
            onFdrSignal: {
                switch(what) {
                case 5:
                    results.items[b][a-1].measurments.push({"len": len, "level": level})
                    return
                case 4:
                    results.setContent(b, a, "Hi Noise")
                    break
                case 3:
                    results.setContent(b, a, "Hi Voltage")
                    break
                case 2:
                case 1:
                    results.items[b][a-1].measurments = [{"len": len, "level": level}]
                    results.setContent(b, a, len + "m, " + level + "dB")
                    results.items[b][a-1].lvl = level
                    results.maxlvl = (results.maxlvl > level ? results.maxlvl : level)
                    break
                case 0:
                    results.setContent(b, a, "...")
                    break
                case -1:
                    results.setContent(b, a, "No Signal")
                    break
                }

                results.items[b][a-1].busy = false
                // results.items[b][a-1].color = "red"
                if(results.last !== null &&  results.last !== results.items[b][a-1]) {
                    results.last.colors = (results.isPair(results.last.c, results.last.r+1) ? results.pairs : results.others)
                }

                results.last = results.items[b][a-1]
            }
        }

        function onMouseChoose(c, r) {
            selectedFDR.clear()
            for(var i = 0; i < results.items[c][r].measurments.length; i++) {
                selectedFDR.append(results.items[c][r].measurments[i])
            }
        }

        function setContent(b, a, txt) {
            results.items[b][a-1].text = txt
            if(b > 1 && a < 8) {
                // results.items[b-1][a].text = txt
            }
        }
    }
}
