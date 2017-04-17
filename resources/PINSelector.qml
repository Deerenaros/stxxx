//    resources/PINSelector.qml is part of STx
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
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1


Item {
    function create(path, attrs, parent) {
        var component = Qt.createComponent(path)
        if(component !== null && component.status === Component.Ready) {
            return component.createObject(parent, attrs);
        } else if(component !== null && component.status === Component.Error) {
            console.debug(component.errorString())
        }

        return null;
    }


    property var pins: []
    property var selected: null
    property bool values: false
    property int maxlvl: 1

    Layout.fillWidth: true
    Layout.fillHeight: true

    GridLayout {
        id: pinslayout

        anchors.fill: parent

        flow: GridLayout.LeftToRight

        columns: 2//values ? 2 : 1

        rowSpacing: 1

        Component.onCompleted: {
            var a, o, f = 0;
            for(var i = 1; i <= 7; i++) {
                pins.push([])
                for(var j = i+1; j <= 8; j++) {
                    a = {first: i, second: j};
                    pins[pins.length-1].push(create("Pin.qml", a, pinslayout));
                    if(values) {
                        create("FDRItem.qml", a, pinslayout);
                    }
                }
            }

            pins.push([])
            for(var k = 1; k <= 8; k++) {
                a = {first: k, second: 9, usable: false};
                pins[pins.length-1].push(create("Pin.qml", a, pinslayout));
                if(values) {
                    create("FDRItem.qml", a, pinslayout);
                }
            }
        }
    }

    Connections {
        target: devicesModel

        onPinsChanged: {
            if(get(a, b)) {
                get(a, b).select()
            }
        }
    }

    function get(i, j) {
        if(pins[i-1] && pins[i-1][j-i-1]) {
            return pins[i-1][j-i-1];
        } return null;
    }
}
