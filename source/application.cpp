//    source/application.cpp is part of STx
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
    connect(this, &QApplication::aboutToQuit, &model, &DeviceModel::closeAll);
}
