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
//    along with STx.  If not, see <http://www.gnu.org/licenses/>.


#define DEBUG_TO_STD
//#define DEBUG_TO_NET


// Try to static build, need prooper rebuild full Qt
// #include <QtPlugin>
// Q_IMPORT_PLUGIN(QWindowsIntegrationPlugin)

#include "application.h"
#include "utils/betterdebug.h"


// Here writing message to stdout or/with socket
// This is great idea to make another way to write
// debug messages into file with auto upload into server.
// Could save much time when debuging with customers.
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

    // stdout
#ifdef DEBUG_TO_STD
    fprintf(stderr, "%s\n", dbg.replace("`", ":\t").toLocal8Bit().data());
    fflush(stderr);
#endif

    // socket
#ifdef DEBUG_TO_NET
    QJsonObject msg;

    // building JSON-message
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

    // adding mark (custom) and message type (debug/critical/info/etc)
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

    // writing
    init = (sock.write(QJsonDocument(msg).toJson(QJsonDocument::Compact) + "`") == -1);
    if(!init) {
        // if OK, flushing
        sock.flush();
    }
#endif
}


int main(int argc, char *argv[]) {
    // Better debug message writer. Could double output
    // into socket by enabling DEBUG_TO_NET macro
    qInstallMessageHandler(qBetterDebug);
    // Entry point in "source/application.cpp"
    Application app(argc, argv);
    return app.exec();
}
