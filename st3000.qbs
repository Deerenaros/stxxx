import qbs 1.0

CppApplication {
    files: [
        "include/application.h",
        "include/betterdebug.h",
        "include/device.h",
        "include/devicesmodel.h",
        "include/packets.h",
        "include/qml_packets.h",
        "main.cpp",
        "qml.qrc",
        "resources/Amplifier.qml",
        "resources/DeviceView.qml",
        "resources/DevicesList.qml",
        "resources/FDR.qml",
        "resources/FDRControls.qml",
        "resources/FDRItem.qml",
        "resources/FDRItems.qml",
        "resources/FDRResults.qml",
        "resources/FDRSignal.qml",
        "resources/MultiText.qml",
        "resources/NLD.qml",
        "resources/OffMode.qml",
        "resources/Oscilloscope.qml",
        "resources/PINSelector.qml",
        "resources/PINsResponse.qml",
        "resources/Pin.qml",
        "resources/Receiver.qml",
        "resources/Settings.qml",
        "resources/StatusBar.qml",
        "resources/SwitchItem.qml",
        "resources/Switcher.qml",
        "resources/TabBtn.qml",
        "resources/functions/helpers.js",
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
