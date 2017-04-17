//    resources/FDRResults.qml is part of STx
//
//    STx is free software: you can redistribute it and/or modify
//    it under the terms of the GNU General Public License as published by
//    the Free Software Foundation, either version 3 of the License, or
//    (at your option) any later version.
//
//    STx is distributed in the hope that it will be useful,
//    but WITHOUT ANY WARRANTY; without even the implied warranty of
//    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
//    GNU General Public License for more details.
//
//    You should have received a copy of the GNU General Public License
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4


Item {
    id: resultsRoot

    property int set: 0
    property bool active: false
    property bool busy: false

    ListModel {
        id: resultModel
    }

    ColumnLayout {
        Rectangle {
            Layout.preferredHeight: 25
            Layout.fillWidth: true
            Text {
                text: (busy ? "..." : qsTr("SET #") + set)
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
                            devicesModel.properties.fdr.set = resultsRoot.set
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
