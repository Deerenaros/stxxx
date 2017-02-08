import QtQuick 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.1

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
