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
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.


#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QString>
#include <QSerialPortInfo>
#include <QByteArray>
#include <QSerialPort>
#include <QMutex>
#include <QThread>
#include <QQueue>
#include <QFile>

#include <QMessageBox>

class Device
        : public QThread
{
    Q_OBJECT

    static const quint8 START = 0xC0;
    static const quint8 END   = 0xC1;
    static const quint8 ESC   = 0xDB;

    static const unsigned long SLEEP_DELAY = 25;

    static quint8 checkESC (quint8 byte);

public:
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

signals:
    void packetRead(Device* sender, QByteArray* data);
    void deviceError(QString error, Error type = FIRMWARE);

protected:
    void run();

private:
    QString strBuf;
    bool esc = false;
    quint8 step = 0, crc = 0;
    quint16 rxCNT = 0, cnt = 0;

private:
    QMutex m_mutex;

    QFile m_firmware;

    QString m_name;
    QQueue<QByteArray*> m_queuedBuff;
    QByteArray* m_buff = nullptr;
    QSerialPort* m_port = nullptr;
    const QSerialPortInfo m_portInfo;
};

#endif // DEVICE_H
