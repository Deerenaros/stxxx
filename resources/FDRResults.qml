import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4


Item {
    id: resultsRoot

    property string header: ""
    property bool active: false
    property bool busy: false

    Layout.fillWidth: true
    Layout.fillHeight: true

    ListModel {
        id: resultModel
    }

    ColumnLayout {
        Rectangle {
            Layout.preferredHeight: 25
            Layout.fillWidth: true
            Text {
                text: (busy ? "..." : header)
                font.bold: active

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter

                MouseArea {
                    anchors.fill: parent

                    onClicked: {
                        if(!active) {
                            active = true
                            resultsRoot.parent.currentSet.active = false
                            resultsRoot.parent.currentSet = resultsRoot
                            devicesModel.retake()
                        }
                    }
                }
            }
        }

        TableView {
            id: results
            Layout.fillWidth: true
            Layout.fillHeight: true

            TableViewColumn {
                role: "n"
                title: qsTr("NN")
                width: results.width/5 - 1
                delegate: Item {
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: styleData.textColor
                        elide: styleData.elideMode
                        text: "#" + styleData.value
                    }
                }
            }
            TableViewColumn {
                role: "len"
                title: qsTr("Length")
                width: 2*results.width/5 - 1
                delegate: Item {
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: styleData.textColor
                        elide: styleData.elideMode
                        text: styleData.value + "m"
                    }
                }
            }
            TableViewColumn {
                role: "lvl"
                title: qsTr("Signal Level")
                width: 2*results.width/5 - 1
                delegate: Item {
                    Text {
                        anchors.verticalCenter: parent.verticalCenter
                        anchors.horizontalCenter: parent.horizontalCenter
                        color: styleData.textColor
                        elide: styleData.elideMode
                        text: styleData.value + "dB"
                    }
                }
            }

            model: resultModel

            Connections {
                target: devicesModel
                onFdrSignal: {
                    if(active) {
                        switch(what) {
                        case 0:
                            busy = true
                            resultModel.clear()
                            break
                        case 1:
                            busy = false
                            resultModel.append({
                                "n": (results.model.count+1).toString(),
                                "lvl": lvl.toString(),
                                "len": len.toString(),
                            })
                            break
                        }
                    }
                }
            }
        }
    }
}
