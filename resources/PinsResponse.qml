//    resources/PINsResponse.qml is part of STx
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
import QtQuick.Controls 1.1
import QtQuick.Window 2.1


Item {
    width: 400
    height: parent.height

    property var pins: []

    GridLayout {
        anchors.fill: parent

        id: pinslayout
        property real maxlvl: 1

        flow: GridLayout.TopToBottom

        rows: 14

        rowSpacing: 1
        columnSpacing: 1

        Component.onCompleted: {
            for(var i = 1; i <= 7; i++) {
                pins.push([])
                for(var j = i+1; j <= 8; j++) {
                    var component = Qt.createComponent("FDRItem.qml")
                    if(component !== null && component.status === Component.Ready) {
                        pins[pins.length-1].push(component.createObject(pinslayout, {first: i, second: j}))
                    } else if(component !== null && component.status === Component.Error) {
                        console.debug(component.errorString())
                    }
                }
            }
        }
    }

    Connections {
        target: devicesModel
        onFdrSignal: {
            if(a > b) {
                var t = a; a = b; b = t;
            }

            switch(what) {
            case 0:
                get(a, b).busy = true
                get(a, b).measurments = []
                break
            case 1:
                get(a, b).measurments.push({"lvl": lvl, "len": len})
                break
            case 2:
                pinslayout.maxlvl = Math.max(pinslayout.maxlvl, lvl)
                get(a, b).busy = false
                get(a, b).lvl = lvl
                get(a, b).len = len
                break
            }

             console.log(get(a, b).g)
        }
    }

    Component.onCompleted: {
        // devicesModel.automate = true
    }

    function get(i, j) {
        if(pins[i-1] && pins[i-1][j-i-1]) {
            return pins[i-1][j-i-1];
        } return null;
    }
}
