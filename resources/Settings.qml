//    resources/Settings.qml is part of STx
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
import QtQuick.Dialogs 1.2

Item {
    ColumnLayout {
        Calendar {
            id: calendar
        }

        SpinBox {
            id: _hours

            minimumValue: 0
            maximumValue: 23
        }

        SpinBox {
            id: minutes

            minimumValue: 0
            maximumValue: 59
        }

        Connections {
            target: devicesModel
            onDateSignal: {
                _hours.value = hours
                minutes.value = min
                calendar.selectedDate = new Date(year+2000, month-1, day);
            }
        }

        Button {
            text: qsTr("Update")

            onClicked: {
                flashDialog.open()
            }
        }

        FileDialog {
            id: flashDialog
            title: qsTr("Update firmware");
            folder: shortcuts.home

            selectMultiple: false
            selectFolder: false
            nameFilters: [qsTr("Binary firmware") + "(*.bin)"]

            onAccepted: {
                devicesModel.flashCurrent(flashDialog.fileUrl.toString())
            }
        }
    }

    MessageDialog {
        id: fileError
        title: "File read error"
        text: ""
        onAccepted: {
        }
    }

    Connections {
        target: devicesModel
        onFirmwareError: {
            fileError.text = error
            fileError.visible = true
        }
    }

    Connections {
        target: calendar
        onSelectedDateChanged: {
            update()
        }
    }
    Connections {
        target: _hours
        onEditingFinished: {
            update()
        }
    }
    Connections {
        target: minutes
        onEditingFinished: {
            update()
        }
    }

    function update () {
        var date = calendar.selectedDate
        devicesModel.setDate(_hours.value, minutes.value, date.getFullYear()-2000, date.getMonth() + 1, date.getDate())
    }
}
