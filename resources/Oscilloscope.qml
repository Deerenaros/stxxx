import QtQuick 2.0
import QtCharts 2.1

ChartView {
    id: oscilloscope
    antialiasing: true

    animationOptions: ChartView.NoAnimation
    theme: ChartView.ChartThemeLight

    ValueAxis {
        id: axisY
        min: -110
        max: +110
    }

    ValueAxis {
        id: axisX
        min: -1024
        max: 0
    }

    Connections {
        target: devicesModel


        onAmplifierSignal: {
            axisX.min = -1024
            axisX.max = 0
        }
    }

    LineSeries {
        id: lineSeries
        name: "signal"
        axisX: axisX
        axisY: axisY
        useOpenGL: false
    }

    MouseArea {
        id: mouse
        z: 100500

        anchors.fill: parent

        hoverEnabled: true

        property int prevX
        property int prevY

        onPositionChanged: {
//            if(pressed) {
//                var dx = (prevX - mouseX)
//                var dy = (prevY - mouseY) * Math.log(axisY.max + 1) / 10

//                if(axisY.max + dy > 1) {
//                    axisY.max += dy
//                } else {
//                    axisY.max = +1
//                }

//                axisY.min = -axisY.max
//            }

//            prevX = mouseX; prevY = mouseY
        }
    }

    function updateMinMax(count) {
        axisX.min = 1024 - count;
        axisX.max = count;
    }

    function createAxis(min, max) {
        return Qt.createQmlObject("import QtQuick 2.0; import QtCharts 2.0; ValueAxis { min: " + min + "; max: " + max + " }", chartView);
    }
}
