#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QQuickView>

#include "devicesmodel.h"

class Application : public QApplication
{
    Q_OBJECT

    static constexpr const char* RESOURCE_MAIN = "qrc:/resources/main.qml";
    static constexpr const char* APPLICATION_TITLE = "ST Viewer";
#ifdef Q_OS_WIN
    static constexpr const char* IMPORT_PATH = "%1/../../../../%2";
#else
    static constexpr const char* IMPORT_PATH "%1/../../../%2"
#endif

public:
    explicit Application(int&, char**);
signals:
public slots:
private:
    QQuickView viewer;
    DevicesModel model;
};

#endif // APPLICATION_H
