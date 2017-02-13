import QtQuick 2.0
import QtQuick.Layouts 1.1
import QtQuick.Controls 1.4


Item {
    property string header: ""

    Layout.fillWidth: true
    Layout.fillHeight: true

    ColumnLayout {
        Rectangle {
            Layout.preferredHeight: 25
            Layout.fillWidth: true
            Text {
                text: header

                anchors.horizontalCenter: parent.horizontalCenter
                anchors.verticalCenter: parent.verticalCenter
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
            }
            TableViewColumn {
                role: "len"
                title: qsTr("Length")
                width: 2*results.width/5 - 1
            }
            TableViewColumn {
                role: "lvl"
                title: qsTr("Signal Level")
                width: 2*results.width/5 - 1
            }

            model: ListModel {
                ListElement {
                    n: "#1"
                    len: "100m"
                    lvl: "40db"
                }
            }

            itemDelegate: Item {
                Text {
                    anchors.verticalCenter: parent.verticalCenter
                    anchors.horizontalCenter: parent.horizontalCenter
                    color: styleData.textColor
                    elide: styleData.elideMode
                    text: styleData.value
                }
            }
        }
    }

}
