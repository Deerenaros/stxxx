//    include/betterdebug.h is part of STx
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

#define qdebug(x) qDebug().nospace().noquote() << DMark(x)
#define qfatal(x) qCritical().nospace().noquote() << DMark(x)

#endif
