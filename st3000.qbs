import qbs 1.0

CppApplication {
    files: [
        "include/betterdebug.h",
        "include/device.h",
        "include/devicesmodel.h",
        "main.cpp",
        "qml.qrc",
        "resources/Amplifier.qml",
        "resources/DeviceView.qml",
        "resources/DevicesList.qml",
        "resources/FDR.qml",
        "resources/NLD.qml",
        "resources/OffMode.qml",
        "resources/Oscilloscope.qml",
        "resources/Receiver.qml",
        "resources/Settings.qml",
        "resources/StatusBar.qml",
        "resources/SwitchItem.qml",
        "resources/Switcher.qml",
        "resources/main.qml",
        "source/device.cpp",
        "source/devicesmodel.cpp",
    ]

    Depends {
        name: "Qt"
        submodules: ["charts", "qml", "quick", "serialport", "network"]
    }

    Depends {
        name: "cpp"
    }

    Group {
        fileTagsFilter: product.type
        qbs.install: true
    }

    cpp.includePaths: ["./include"]
}
