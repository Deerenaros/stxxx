import QtQuick 2.0
import QtQuick.Layouts 1.1

Item {
    id: fdr

    property var win: null

    GridLayout {
        id: results

        anchors.fill: parent

        columnSpacing: 15
        rowSpacing: 15

        rows: 2
        columns: 4

        PinSelector {
            Layout.column: 0
            Layout.row: 0
        }

        FDRResults {
            header: "first set"

            Layout.column: 1
            Layout.row: 0
        }

        FDRResults {
            header: "second set"

            Layout.column: 2
            Layout.row: 0
        }

        FDRSignalPreview {
            Layout.column: 3
            Layout.row: 0
        }

        FDRVelocitySlider {
            Layout.column: 0
            Layout.columnSpan: 4
            Layout.row: 1
        }

        Connections {
            target: devicesModel
            onFdrSignal: {
                switch(what) {
                case 5:
                    return
                case 4:
                    break
                case 3:
                    break
                case 2:
                case 1:
                    break
                case 0:
                    break
                case -1:
                    break
                }
            }
        }

        function setContent(a, b, content) {
        }
    }

    function automate() {
        var component = Qt.createComponent("FDRAutomate.qml");
        if(component !== null && component.status === Component.Ready) {
            win = component.createObject(fdr);
            win.show();
        } else if(component !== null && component.status === Component.Error) {
            console.debug(component.errorString())
        }
    }
}
