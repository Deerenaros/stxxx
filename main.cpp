//#define DEBUG_TO_STD
#define DEBUG_TO_NET

// TODO: Рефлектометр/нелинейник
// TODO: Сохранение результатов
// TODO: Создание отчёта
// TODO: лаааааааааааааааааааапшааааааааааааааааа

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
