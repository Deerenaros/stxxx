//    source/device.cpp is part of STx
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

#include "betterdebug.h"
#include "device.h"

Device::Device(const QString& name, const QSerialPortInfo &portInfo, QObject *parent)
    : QThread(parent)
    , m_name(name)
    , m_portInfo(portInfo)
{
    m_port = nullptr;
    m_buff = new QByteArray();
}

Device::~Device() {
    delete m_buff;

    for(auto buff: m_queuedBuff) {
        delete buff;
    }

    if(m_port != nullptr) {
        m_port->close();
    }
}

QString Device::name() const {
    return m_name;
}

QString Device::did() const {
    return QString("0x%2 0x%3")
        .arg(m_portInfo.vendorIdentifier(), 4, 16, QChar('0'))
        .arg(m_portInfo.productIdentifier(), 4, 16, QChar('0'));
}

bool Device::isReady() const {
    return m_port != nullptr && m_port->isOpen();
}

void Device::open() {
    m_mutex.lock();

    if(m_port == nullptr) {
        m_port = new QSerialPort(m_portInfo, this);
    }

    bool ok = true;
    ok &= m_port->open(QIODevice::ReadWrite);
    ok &= m_port->setBaudRate(921600);
    ok &= m_port->setDataTerminalReady(true);
    ok &= m_port->setRequestToSend(true);
    ok &= m_port->setParity(QSerialPort::NoParity);
    ok &= m_port->setStopBits(QSerialPort::OneStop);
    ok &= m_port->setDataBits(QSerialPort::Data8);
    ok &= m_port->setFlowControl(QSerialPort::NoFlowControl);

    this->start();

    m_mutex.unlock();
}

quint8 Device::checkESC(quint8 byte) {
    switch(byte) {
    case START:
        return 0xDC;
    case END:
        return 0xDD;
    case ESC:
        return 0xDE;
    }

    return 0;
}

void Device::write(const QByteArray &data) {
    QString str2;
    QByteArray str("\xC0\x00", 2);
    quint8 crc = 0, ind = str.count(), tmp;
    str2 = QString("%1:").arg(data.count());
    str[ind++] = static_cast<quint8>(data.count());
    tmp = checkESC(str[ind-1]);

    if (tmp > 0) {
        str[ind-1] = ESC;
        str[ind++] = tmp;
    }

    for (int i = 0; i < data.count(); i++) {
        str2 += QString("0x%1 ").arg((quint8)data.at(i), 2, 16, QChar('0'));

        if (i == 0) {
            str2 += QString("(%1)  ").arg(data.at(i));
        }

        crc += data[i];
        tmp = checkESC(data.at(i));
        if (tmp > 0) {
            str[ind++] = ESC;
            str[ind++] = tmp;
        } else {
            str[ind++] = data.at(i);
        }
    }

    tmp = checkESC(crc);
    if (tmp > 0) {
        str[ind++] = ESC;
        str[ind++] = tmp;
    } else {
        str[ind++] = crc;
    }

    str[ind++] = END;

    m_mutex.lock();
    m_port->write(str, ind);
    m_mutex.unlock();

    debug << DMark("writen") << "<< " << str2;
}

void Device::run() {
    for(;;) {
        m_mutex.lock();
        auto data = (isReady() ? m_port->readAll() : QByteArray());
        m_mutex.unlock();

        QByteArray &buff = *m_buff;
        for(char ch: data) {

            if (step == 3) {
                strBuf += QString("0x%1 ").arg((quint8)ch, 2, 16, QChar('0'));
            }

            if (ch == (char)0xc0) {
                step = 1; // приняли начало пакета - ждем размер
                esc = false;
                continue;
            }

            if (ch == (char)0xc1) { // конец пакета
                if (step == 5) {
                    step = 0;
                    debug << DMark("read") << ">> " << strBuf;
                    m_mutex.lock();
                    emit packetRead(this, new QByteArray(buff));
                    m_mutex.unlock();
                    buff.clear();
                    strBuf.clear();
                }
                continue;
            }

            if (ch == (char)0xdb) {
                esc = true;
                continue;
            }

            if (esc) {
                if (ch == (char)0xdc) {
                    ch = (char)0xc0;
                } else if (ch == (char)0xdd) {
                    ch = (char)0xc1;
                } else if (ch == (char)0xde) {
                    ch = (char)0xdb;
                }

                if (step == 3) {
                    //strBuf += "(" + QByteArray::number((quint8)ch, 16).toUpper() + ") ";
                }
                esc = false;
            }

            if (step == 1) { // старший байт размера пакета
                rxCNT = quint8(ch << 8);
                step = 2;
            } else if (step == 2) { // младший байт размера пакета
                rxCNT += quint8(ch);
                cnt = 0;
                crc = 0;
                step = 3;
                strBuf += QString("%1: ").arg(rxCNT);
            } else if (step == 3) {
                if (cnt == 0) {
                    strBuf += QString("(%1)  ").arg(ch);
                }
                buff[cnt] = (quint8)ch;
                crc += (quint8)ch;
                cnt++;
                if (cnt == rxCNT) {
                    step = 4;
                }
            } else if (step == 4) {
                if (crc == (quint8)ch) {
                    buff[rxCNT] = 0;
                    step = 5;
                } else {
                    step = 0;
                    strBuf = strBuf;
                    fatal << DMark("read") << ">> " << strBuf;
                }
            }
        }

        this->msleep(SLEEP_DELAY);
    }
}

void Device::close() {
    if(this->isRunning()) {
        m_mutex.lock();
        m_port->close();
        this->terminate();
        m_mutex.unlock();
    }
}

static quint32 size, send, step, progr=0, cnt;

void Device::flash(QString fileName) {
    QByteArray str;
    quint32 tmp;
    quint8 i;

    m_firmware.setFileName(fileName.remove("file:///"));
    if(!m_firmware.open(QIODevice::ReadOnly)) {
        emit deviceError(m_firmware.errorString(), Device::FIRMWARE);
    }

    QDataStream in;
    in.setDevice(&m_firmware);
    in.setByteOrder(QDataStream::LittleEndian);

    QDataStream out(&str, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out << (quint8)'D';
    in >> i; out << i; in >> i; out << i; in >> i; out << i; // 300
    in >> i; out << i; in >> i; out << i;    // Версия
    in >> i; out << i; // кол-во файлов внутри
    in >> tmp;  // Размер
    size = tmp;
    out << tmp;
    in >> i; out << i; // признак наличия MCU
    in >> tmp; out << tmp; // размер MCU
    write(str);
    debug << size << " would be written";
    send = 0;
    step = size/99;  // шаг для прогресса
    progr = 0;
    cnt = 0;
}

void Device::flash() {
    QDataStream in;
    in.setDevice(&m_firmware);
    in.setByteOrder(QDataStream::LittleEndian);

    QByteArray str;
    QDataStream out(&str, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);
    out << (quint8)'D';
    quint8 tmp8;
    for (int i=0; i<128; i++) { // по 128 байт (всегда, даже если лишние будут)
        in >> tmp8;
        out << tmp8;
        send++;
    }

    write(str);
    cnt += 128; // счетчик для прогресса
    if (cnt >= step) // увеличиваем и отправляем прогресс
    {
        while (cnt >= step)
        {
            cnt -= step;
            if (progr<100) progr++;
        }

        write(QByteArray({'P', (qint8)progr}));
    }
}
