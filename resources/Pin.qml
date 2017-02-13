import QtQuick 2.0

Rectangle {
    property int first: 0
    property int second: 0

    Text {
        text: first + "-" + second
    }
}
