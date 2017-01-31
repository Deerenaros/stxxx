#ifndef BETTERDEBUG_H
#define BETTERDEBUG_H

#include <QDebug>
#include <QHostAddress>
#include <QTcpSocket>
#include <QJsonObject>
#include <QJsonDocument>

#include <stdio.h>
#include <stdlib.h>

class DMark : public QString {
public:
    DMark(QString mark) : QString(mark) {}

    friend QDebug operator << (QDebug debug, const DMark &mark) {
        QDebugStateSaver saver(debug);
        debug << static_cast<QString>(mark) << "`";
        return debug;
    }
};

#define debug qDebug().nospace().noquote()
#define critical qCritical().nospace().noquote()

#endif
