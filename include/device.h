#ifndef DEVICE_H
#define DEVICE_H

#include <QObject>
#include <QString>
#include <QSerialPortInfo>
#include <QSerialPort>
#include <QMutex>
#include <QThread>
#include <QQueue>

class Device
        : public QThread
{
    Q_OBJECT

    static const qint8 START = qint8(0xC0);
    static const qint8 END   = qint8(0xC1);
    static const qint8 ESC   = qint8(0xDB);

    static const unsigned long SLEEP_DELAY = 25;

    static quint8 checkESC (quint8 byte);

public:
    friend class DevicesModel;

    Device(const QString& name, const QSerialPortInfo& portInfo, QObject* parent = nullptr);
    ~Device();

    QString name() const;
    QString did() const;
    bool isReady() const;

    void open();
    void close();

    void write(const QByteArray &data);
    QByteArray read();

signals:
    void packetRead(Device* sender, QByteArray* data);

protected:
    void run();

private:
    QString strBuf;
    bool esc = false;
    quint8 step = 0, crc = 0;
    quint16 rxCNT = 0, cnt = 0;

private:
    QMutex m_mutex;

    QString m_name;
    QQueue<QByteArray*> m_queuedBuff;
    QByteArray* m_buff = nullptr;
    QSerialPort* m_port = nullptr;
    const QSerialPortInfo m_portInfo;
};

#endif // DEVICE_H
