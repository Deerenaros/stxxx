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
//    along with STx.  If not, see <http://www.gnu.org/licenses/>.

import QtQuick 2.0
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

    id: reportRoot

    property string name: "none"
    property var components: {
        "fdr": "FDR.qml",
    }

    Component.onCompleted: {
        if(name in components) {
            create(components[name], {"model": devicesModel.reportModel(name)}, reportRoot)
        } else {
            create("../Placeholder.qml", {}, reportRoot)
        }
    }
}
