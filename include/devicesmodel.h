//    include/devicesmodel.cpp is part of STx
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

#ifndef DEVICESMODEL_H
#define DEVICESMODEL_H

#include <QObject>
#include <QVariant>


#include <QVariantList>
#include <QAbstractItemModel>
#include <QMap>
#include <QPair>
#include <QThread>

QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

#include "processor.h"
#include "model.h"


class DeviceModel : public Model
{
    Q_OBJECT

    // some constants
    permanent int COLUMN_COUNT = 1;
    permanent int SWITCH_MODE = 3;
    permanent int SWITCH_PACKETS_TO_RESET = -5;

    // Have to flash device to, for example, 0x72B0 PID
    permanent quint16 SUPPORTED_VID = 0x0403;
    const QMap<qint16, QString> SUPPORTED_PIDS = {
        { 0x6015, "ST300" }
    };

public:

    explicit DeviceModel(QQuickView *appViewer, QObject *parent = 0);
    ~DeviceModel();

    Device& currentDevice() const;
    void bind(Processor*);


public slots:
    bool isReady() const;

    void closeAll();
    void setCurrent(int);
    void toReport();
    void retake();
    void setMode(char);
    void setAmpl(QAbstractSeries*);
    void setSpectrum(QAbstractSeries*);
    void setPins(int, int);
    void setVelocityFactor(double factor);
    void flashCurrent(QString);
    void deviceError(QString, Device::Error);

    void setDate(qint8 hours, qint8 min, qint8 year, qint8 month, qint8 day);

private slots:
    void _packetRX(Device*, Packet *packet);

private:
    // Great idea to throw out this method
    template <typename T>
    void _broadcast(const char *name, T ptr, Device *dev = nullptr) {
        for(Processor *p: m_processors) {
            p->handle(name, ptr, dev);
        }
    }

    // And this one too, but they are need currently
    // only for one button 'auto' in order to prooper
    // determine stop-pins
    // Rewrite to Qt's signal-slot
    template <typename T>
    T _request(const char *name, cvoid additional = Processor::TAKE) {
        cvoid ret = Processor::NOTHING;
        for(Processor *p: m_processors) {
            ret = p->handle(name, additional, &currentDevice());
            if(ret != Processor::NOTHING) {
                break;
            }
        }

        return just_cast<T>(ret);
    }

    void _specifyOnModeChange(char);


    QPair<qint8, qint8> m_waitingSwitch;
    int m_waitingReset = SWITCH_PACKETS_TO_RESET;
    int m_current = -1;
    char m_currentMode = 0x00;
    QQuickView *m_appViewer = nullptr;
    QList<Device*> m_devices;
    QList<Processor*> m_processors;
};

#endif // DEVICESMODEL_H
