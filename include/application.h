//    include/application.h is part of STx
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

public slots:
    void onEvent();

signals:
public slots:
private:
    QQuickView m_viewer;
    DeviceModel m_model;


};

#endif // APPLICATION_H
