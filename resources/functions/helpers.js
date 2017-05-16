//    resources/functions/helpers.qml is part of STx
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

function create(path, attrs, parent, success) {
    var component = Qt.createComponent(path)
    if(component !== null && component.status === Component.Ready) {
        var o = component.createObject(parent, attrs);
        if(o !== null && success && {}.toString.call(success) === '[object Function]') {
            success(o);
        } return o;
    } else if(component !== null && component.status === Component.Error) {
        console.debug(component.errorString())
    }

    return null;
}
