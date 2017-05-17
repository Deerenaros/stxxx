//    source/devicesmodel.cpp is part of STx
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

#include <QQuickView>
#include <QList>
#include <QVariantMap>

#include "betterdebug.h"
#include "devicesmodel.h"



DeviceModel::DeviceModel(QQuickView *appViewer, QObject *parent)
    : Model(parent)
    , m_waitingSwitch(QPair<quint8, quint8>(0, 0))
    , m_appViewer(appViewer)
{
    m_fdr_set = 1;

    for(const QSerialPortInfo &info: QSerialPortInfo::availablePorts()) {
        qdebug("found") << QString("%1 0x%2 0x%3")
                    .arg(info.description())
                    .arg(info.vendorIdentifier(), 4, 16, QChar('0'))
                    .arg(info.productIdentifier(), 4, 16, QChar('0'));

        quint16 vid = info.vendorIdentifier();
        quint16 pid = info.productIdentifier();
        if(SUPPORTED_VID == vid && SUPPORTED_PIDS.contains(pid)) {
            QString name = SUPPORTED_PIDS[info.productIdentifier()];
            auto dev = new Device(name, info, this);
            m_devices.append(dev);

            connect(dev, &Device::packetRead, this, &DeviceModel::_packetRX);
            connect(dev, &Device::deviceError, this, &DeviceModel::deviceError);
            connect(dev, &Device::pinsChanged, [this](Device *dev) {
                emit pinsChanged(dev->current.a, dev->current.b);
            });
        }
    }

    if(m_devices.count() > 0) {
        m_current = 0;
    }
}

DeviceModel::~DeviceModel() {
    for(Device *dev: m_devices) {
        dev->close();
    }
}

bool DeviceModel::isReady() const {
    return m_current >= 0
        && m_current < m_devices.size()
        && m_devices[m_current] != nullptr
        && m_devices[m_current]->isReady();
}

void DeviceModel::closeAll() {
    for(Device *dev: m_devices) {
        dev->close();
    }
}

Device& DeviceModel::currentDevice() const {
    return *m_devices[m_current];
}

void DeviceModel::setCurrent(int current) {
    if(m_current != current) {
        m_current = current;
        emit currentChanged();
    }
}

void DeviceModel::toReport() {
    _broadcast("report", Processor::EVENT, &currentDevice());
}

void DeviceModel::retake() {
    char buff[] = {Modes::Tx::MODE, 7};
    currentDevice().write(QByteArray(buff, 2));
}

void DeviceModel::setMode(char mode) {
    qdebug("mode") << int(mode);

    if(mode) {
        if(!currentDevice().isReady()) {
            currentDevice().open();
        }

        if(mode > 1) {
            mode += 1;
        }

        char buff[] = {Modes::Tx::MODE, mode};
        currentDevice().write(QByteArray(buff, 2));
        _specifyOnModeChange(mode);
    } else {
        currentDevice().close();
        emit statusSignal(-1, -1);
    }
}

void DeviceModel::setSpectrum(QAbstractSeries* series) {
    this->series.fdr = static_cast<QAreaSeries*>(series);
}

void DeviceModel::setAmpl(QAbstractSeries* series) {
    this->series.amplifier = static_cast<QXYSeries*>(series);
}

void DeviceModel::setPins(int a, int b) {
    char buff[] = {Modes::Tx::SWITCH, char(1), char(a), char(b)};
    currentDevice().write(QByteArray(buff, 4));
}

void DeviceModel::setDate(qint8 hours, qint8 min, qint8 year, qint8 month, qint8 day) {
    char time[] = {Modes::Tx::SETS, 1, hours, min};
    char date[] = {Modes::Tx::SETS, 2, day, month, year};

    currentDevice().write(QByteArray(time, 4));
    currentDevice().write(QByteArray(date, 5));
}


void DeviceModel::setVelocityFactor(double factor) {
    if(isReady()) {
        char buff[] = {Modes::Tx::FDR, 1, char(factor), char(100*(factor-quint8(factor)))};
        currentDevice().write(QByteArray(buff, 4));
    }
}

void DeviceModel::flashCurrent(QString fileName) {
    if(isReady()) {
        currentDevice().flash(fileName);
    }
}

void DeviceModel::deviceError(QString error, Device::Error type) {
    Q_UNUSED(type);
    emit firmwareError(error);
}

void DeviceModel::_specifyOnModeChange(char mode) {
    switch(mode) {
    case 2:
        // were is no more 'switch'
        break;
    case 5:
        // range choosing
        currentDevice().write(QByteArray("S\x0c\x01", 3));
        break;
    }
}

void DeviceModel::bind(Processor *p) {
    m_processors.append(p);
}

void DeviceModel::_packetRX(Device* dev, Packet* packet) {
    for(Processor *p: m_processors) {
        switch (packet->mod) {
        case Modes::Rx::FLASHING:
            p->process(*dev, packet->data.flash);
            break;
        case Modes::Rx::AMPL:
            p->process(*dev, packet->data.amplifier);
            break;
        case Modes::Rx::SWITCH:
            p->process(*dev, packet->data.swtch);
            break;
        case Modes::Rx::STARTING:
            p->process(*dev, packet->data.starting);
            break;
        case Modes::Rx::BATTERY:
            p->process(*dev, packet->data.battery);
            break;
        case Modes::Rx::FDR:
            p->process(*dev, packet->data.fdr);
            break;
        case Modes::Rx::NLD:
            p->process(*dev, packet->data.nld);
            break;
        case Modes::Rx::RX:
            p->process(*dev, packet->data.rx);
            break;
        default:
            break;
        }
    }
}
