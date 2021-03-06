//    include/device.h is part of STx
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

#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QString>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QSerialPort>
#include <QThread>
#include <QQueue>
#include <QFile>

#include <QMessageBox>

#include "packets.h"
#include "locked.h"


/// \todo Rework to make write and read parallel
class Device
        : public QThread
{
    Q_OBJECT
    LOCKABLE

    static const quint8 START = 0xC0;
    static const quint8 END   = 0xC1;
    static const quint8 ESC   = 0xDB;

    static const unsigned long SLEEP_DELAY = 25;

    static quint8 checkESC (quint8 byte);

signals:
    void pinsChanged(Device*);

public:
    Pins current;

    enum Error {
        FIRMWARE
    };

    friend class DevicesModel;

    Device(const QString& name, const QSerialPortInfo& portInfo, QObject* parent = nullptr);
    ~Device();

    QString name() const;
    QString did() const;
    bool isReady() const;

    void open();
    void close();
    void flash(QString);
    void flash();

    void write(const QByteArray &data);
    QByteArray read();

    void setPins(Pins pins);

signals:
    void packetRead(Device* sender, Packet* data);
    void deviceError(QString error, Error type = FIRMWARE);

public slots:
    void clearPacket();

protected:
    void run();

private:
    void _process(Packet *p);

    QString strBuf;
    bool esc = false;
    quint8 step = 0, crc = 0;
    quint16 rxCNT = 0, count = 0;

private:
    QFile m_firmware;

    QString m_name;
    QQueue<QByteArray*> m_queuedBuff;
    QByteArray* m_packet = nullptr;
    QByteArray* m_buff = nullptr;
    QSerialPort* m_port = nullptr;
    const QSerialPortInfo m_portInfo;
};

#endif // DEVICE_H
