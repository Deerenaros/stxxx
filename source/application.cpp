#include "application.h"

#include <QStandardPaths>
#include <QQmlContext>
#include <QQmlEngine>
#include <QDir>

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , viewer()
    , model(&viewer)
{
    QString extraImportPath = QString(IMPORT_PATH).arg(QGuiApplication::applicationDirPath(), QString::fromLatin1("qml"));
    viewer.engine()->addImportPath(extraImportPath);
    viewer.rootContext()->setContextProperty("devicesModel", &model);
    viewer.setTitle(APPLICATION_TITLE);
    viewer.setSource(QUrl(RESOURCE_MAIN));
    viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    viewer.show();

    connect(viewer.engine(), &QQmlEngine::quit, &viewer, &QWindow::close);
    connect(this, &QApplication::aboutToQuit, &model, &DevicesModel::closeAll);
}
