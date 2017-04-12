//    main.cpp is part of STx
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


//#define DEBUG_TO_STD
#define DEBUG_TO_NET

/// \todo Рефлектометр: графика {deadline: "04/13/17"}
/// \todo Создание отчёта и сохранение результатов {deadline: "04/13/17"}
/// \todo Статическая линковка {deadline: "never"}
/// \todo Ещё Меньше Лапши, МЕНЬШЕ! {deadline: "never"}

// #include <QtPlugin>
// Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

#include "application.h"
#include "betterdebug.h"

void qBetterDebug(QtMsgType type, const QMessageLogContext &context, const QString &_dbg) {
    QString dbg = _dbg;
#ifdef DEBUG_TO_NET
    static QTcpSocket sock;
    static bool init = true;
    if(init) {
        init = false;
        sock.connectToHost(QHostAddress("127.0.0.1"), 61293, QTcpSocket::ReadWrite);
        if(!sock.waitForConnected(1000) || !sock.open(QTcpSocket::ReadWrite)) {
            fprintf(stderr, "ERROR! %s\n", sock.errorString().toLocal8Bit().data());
            fflush(stderr);
            return;
        }
    }
#endif

    QJsonObject msg;

    switch (type) {
    case QtMsgType::QtFatalMsg:
        msg.insert("type", "fatal");
        break;
    case QtMsgType::QtInfoMsg:
        msg.insert("type", "info");
        break;
    case QtMsgType::QtDebugMsg:
        msg.insert("type", "debug");
        break;
    case QtMsgType::QtCriticalMsg:
        msg.insert("type", "critical");
        break;
    default:
        break;
    }

    if(dbg.contains('`')) {
        msg.insert("mark", dbg.split('`').first());
        msg.insert("msg", dbg.split('`').last());
    } else {
        msg.insert("mark", "...");
        msg.insert("msg", dbg);
    }

    msg.insert("file", context.file);
    msg.insert("line", context.line);
    msg.insert("fn", context.function);


#ifdef DEBUG_TO_STD
    fprintf(stderr, "%s\n", dbg.replace("`", ":\t").toLocal8Bit().data());
    fflush(stderr);
#endif

#ifdef DEBUG_TO_NET
    init = (sock.write(QJsonDocument(msg).toJson(QJsonDocument::Compact) + "`") == -1);
    if(!init) {
        sock.flush();
    }
#endif
}


int main(int argc, char *argv[]) {
    qInstallMessageHandler(qBetterDebug);
    Application app(argc, argv);
    return app.exec();
}
