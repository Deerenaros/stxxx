import QtQuick 2.0

Rectangle {
    id: devices

    signal choosed(int index)

    ListView {
        id: devicesList
        anchors.fill: parent
        anchors.margins: 5

        Component {
            id: deviceDelegate
            Rectangle {
                id: wrapper
                width: 80
                height: 40
                color: ListView.isCurrentItem ? "black" : "white"
                Column {
                    Text {
                        color: wrapper.ListView.isCurrentItem ? "white" : "black"
                        font.pointSize: 10
                        text: "<b>" + name + "</b>"
                    }
                    Text {
                        color: wrapper.ListView.isCurrentItem ? "white" : "black"
                        font.capitalization: Font.AllUppercase
                        font.pointSize: 8
                        text: did
                    }
                }

                MouseArea {
                    z: 100500
                    anchors.fill: parent
                    onClicked: {
                        devicesList.currentIndex = index
                        choosed(index)
                    }
                }
            }
        }

        model: devicesModel

        delegate: deviceDelegate
        highlight: Rectangle {
            width: parent.width
            color: "lightsteelblue"
        }
    }
}
