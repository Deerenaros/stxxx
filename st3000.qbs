import qbs 1.0

CppApplication {
    files: [
        "include/application.h",
        "include/betterdebug.h",
        "include/device.h",
        "include/devicesmodel.h",
        "main.cpp",
        "qml.qrc",
        "resources/Amplifier.qml",
        "resources/DeviceView.qml",
        "resources/DevicesList.qml",
        "resources/FDR.qml",
        "resources/FDRAutomate.qml",
        "resources/FDRControls.qml",
        "resources/FDRItem.qml",
        "resources/FDRItems.qml",
        "resources/FDRResults.qml",
        "resources/FDRSignal.qml",
        "resources/MultiText.qml",
        "resources/NLD.qml",
        "resources/OffMode.qml",
        "resources/Oscilloscope.qml",
        "resources/Pin.qml",
        "resources/PinSelector.qml",
        "resources/Receiver.qml",
        "resources/Settings.qml",
        "resources/StatusBar.qml",
        "resources/SwitchItem.qml",
        "resources/Switcher.qml",
        "resources/main.qml",
        "source/application.cpp",
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
    // cpp.staticLibraries: ["Qt5PlatformSupport", "opengl32", "qwindows", "imm32", "winmm", "Ws2_32"]
}
