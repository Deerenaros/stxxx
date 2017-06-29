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
//    along with STx.  If not, see <http://www.gnu.org/licenses/>.

#include "application.h"

#include <QStandardPaths>
#include <QQmlContext>
#include <QQmlEngine>
#include <QDir>

#include "report.h"
#include "devicelogic.h"

Application::Application(int &argc, char **argv)
    : QApplication(argc, argv)
    , m_viewer()
    , m_model(&m_viewer)
{
    // Cheking application directory
    qdebug("app") << "Current directory is " << QDir::currentPath();

    // binding model's processors (controllers in MVC)
    m_model.bind(new DeviceLogic(m_model));
    m_model.bind(new Report("report.xlsx", m_model));

    // Updating QML engine
    QString extraImportPath = QString(IMPORT_PATH).arg(QGuiApplication::applicationDirPath(), QString::fromLatin1("qml"));
    m_viewer.engine()->addImportPath(QRC_PATH);
    m_viewer.engine()->addImportPath(QML_PATH);
    m_viewer.engine()->addImportPath(extraImportPath);
    m_viewer.rootContext()->setContextProperty("devicesModel", &m_model);
    m_viewer.setTitle(APPLICATION_TITLE);
    m_viewer.setSource(QUrl(RESOURCE_MAIN));
    m_viewer.setResizeMode(QQuickView::SizeRootObjectToView);
    m_viewer.show(); // and showing window

    // Making close window works prooper
    connect(m_viewer.engine(), &QQmlEngine::quit, &m_viewer, &QWindow::close);
    connect(this, &QApplication::aboutToQuit, &m_model, &DeviceModel::closeAll);
}

void Application::onEvent() {
    // do not mind
    qdebug("application") << "event!";
}
