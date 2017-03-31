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
    LOCKED

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
    str2 = QString("%1: ").arg(data.count(), 3);
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

    LOCKEDX( m_port->write(str, ind) );

    qdebug("writen") << "<< " << str2;
}

void Device::run() {
    for(;;) {
        LOCKEDX( auto data = (isReady() ? m_port->readAll() : QByteArray()) );
        QByteArray &buff = *m_buff;
        for(char ch: data) {
            if (step == 3) {
                strBuf += QString("0x%1 ").arg((quint8)ch, 2, 16, QChar('0'));
            }

            if (ch == Stuff::START) {
                step = 1;
                esc = false;
                continue;
            }

            if (ch == Stuff::END) {
                if (step == 5) {
                    step = 0;
                    qdebug("read") << ">> " << strBuf;

                    clearPacket();
                    m_packet = new QByteArray(buff);
                    emit packetRead(this, reinterpret_cast<Packet*>(m_packet->data()));

                    buff.clear();
                    strBuf.clear();
                }

                this->msleep(SLEEP_DELAY);
                continue;
            }

            if (ch == Stuff::ESC) {
                esc = true;
                continue;
            }

            if (esc) {
                switch(ch) {
                case Stuff::ESC_START:
                    ch = Stuff::START;
                    break;
                case Stuff::ESC_END:
                    ch = Stuff::END;
                    break;
                case Stuff::ESC_ESC:
                    ch = Stuff::ESC;
                    break;
                }

                esc = false;
            }

            if (step == 1) { // старший байт размера пакета
                rxCNT = quint8(ch << 8);
                step = 2;
            } else if (step == 2) { // младший байт размера пакета
                rxCNT += quint8(ch);
                count = 0;
                crc = 0;
                step = 3;
                strBuf += QString("%1: ").arg(rxCNT, 3);
            } else if (step == 3) {
                if (count == 0) {
                    strBuf += QString("(%1)  ").arg(ch);
                }
                buff[count] = (quint8)ch;
                crc += (quint8)ch;
                count++;
                if (count == rxCNT) {
                    step = 4;
                }
            } else if (step == 4) {
                if (crc == (quint8)ch) {
                    buff[rxCNT] = 0;
                    step = 5;
                } else {
                    step = 0;
                    strBuf = strBuf;
                    qfatal("read") << ">> " << strBuf;
                }
            }
        }

        this->msleep(SLEEP_DELAY);
    }
}

void Device::clearPacket() {
    LOCKED

    if(m_packet != nullptr) {
        delete m_packet;
        m_packet = nullptr;
    }
}

void Device::close() {
    LOCKED

    if(this->isRunning()) {
        m_port->close();
        this->terminate();
    }
}

static quint32 size, send, step, progress=0, cnt;


/// \todo Добавить таймаут на подверждение прошивки
/// \todo Уменьшить размер пакета до 100
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

    out << static_cast<quint8>(Modes::FLASHING);

    in >> i; out << i; in >> i; out << i; in >> i; out << i; // 300
    in >> i; out << i; in >> i; out << i;    // Версия
    in >> i; out << i; // кол-во файлов внутри
    in >> tmp;  // Размер
    size = tmp;
    out << tmp;
    in >> i; out << i; // признак наличия MCU
    in >> tmp; out << tmp; // размер MCU
    write(str);
    qdebug("flash") << size << " would be written";
    send = 0;
    step = size/99;  // шаг для прогресса
    progress = 0;
    count = 0;
}

void Device::flash() {
    QDataStream in;
    in.setDevice(&m_firmware);
    in.setByteOrder(QDataStream::LittleEndian);

    QByteArray str;
    QDataStream out(&str, QIODevice::WriteOnly);
    out.setByteOrder(QDataStream::LittleEndian);

    out << static_cast<quint8>(Modes::FLASHING);

    quint8 tmp8;
    for (int i=0; i < 128; i++) { // по 128 байт (всегда, даже если лишние будут)
        in >> tmp8;
        out << tmp8;
        send++;
    }

    write(str);

    count += 128; // счетчик для прогресса
    if (count >= step) // увеличиваем и отправляем прогресс
    {
        while (count >= step)
        {
            count -= step;
            if (progress < 100) progress++;
        }

        write(QByteArray({Modes::PROGRESS, (qint8)progress}));
    }
}
