//    resources/Report/Main.qml is part of STx
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
    function create(path, attrs, parent) {
        var component = Qt.createComponent(path)
        if(component !== null && component.status === Component.Ready) {
            return component.createObject(parent, attrs);
        } else if(component !== null && component.status === Component.Error) {
            console.debug(component.errorString())
        }

        return null;
    }

    id: nld
    property string name: "none"
    property var attrs: {
        "fdr": {
            "rows": 10,
            "cols": 10,
        },
        "default": {
            "rows": 1,
            "cols": 2,
        }
    }

    Component {
        id: column
        TableViewColumn{width: 100 }
    }

    TableView {
        id: table

        anchors.fill: parent

        Component.onCompleted: {
            var a = attrs[name] || attrs["default"]
            console.log(name)
            for(var c = 0; c < a.cols; c++) {
                var b = {"role": c.toString(), "title": c.toString()}
                column.createObject(table, b)
                console.log("fuck" + c);
            }
        }

        model: []//deviceView.reportModel(name)
    }
}
