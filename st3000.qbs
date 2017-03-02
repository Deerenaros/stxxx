import qbs 1.0

CppApplication {
    files: [
        "include/application.h",
        "include/betterdebug.h",
        "include/device.h",
        "include/devicesmodel.h",
        "include/packets.h",
        "main.cpp",
        "qml.qrc",
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
