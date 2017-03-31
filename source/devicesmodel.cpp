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
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.



#include <QAreaSeries>
#include <QQuickView>
#include <QList>

#include "betterdebug.h"
#include "devicesmodel.h"

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DeviceModel::DeviceModel(QQuickView *appViewer, QObject *parent)
    : QAbstractItemModel(parent)
    , m_waitingSwitch(QPair<quint8, quint8>(0, 0))
    , m_appViewer(appViewer)
{
    for(const QSerialPortInfo &info: QSerialPortInfo::availablePorts()) {
        qdebug("found") << QString("%1 0x%2 0x%3")
                    .arg(info.description())
                    .arg(info.vendorIdentifier(), 4, 16, QChar('0'))
                    .arg(info.productIdentifier(), 4, 16, QChar('0'));

        if(SUPPORTED_PIDS.contains(info.productIdentifier())) {
            QString name = SUPPORTED_PIDS[info.productIdentifier()];
            auto dev = new Device(name, info, this);
            m_devices.append(dev);

            connect(dev, &Device::packetRead, this, &DeviceModel::_packetRX);
            connect(dev, &Device::deviceError, this, &DeviceModel::deviceError);
        }
    }

    if(m_devices.count() > 0) {
        m_currentDevice = 0;
    }
}

DeviceModel::~DeviceModel() {
    for(Device *dev: m_devices) {
        dev->close();
    }
}

int DeviceModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_devices.count();
}

int DeviceModel::columnCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

QModelIndex DeviceModel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return createIndex(row, column);
}

QModelIndex DeviceModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

QVariant DeviceModel::data(const QModelIndex & index, int role) const {
    if (index.row() < 0 || index.row() >= m_devices.count()) {
        return QVariant();
    }

    switch(role) {
    case NameRole:
        return m_devices[index.row()]->name();
    case DIdRole:
        return m_devices[index.row()]->did();
    }

    return QVariant();
}

int DeviceModel::getCurrent() const {
    return m_currentDevice;
}

int DeviceModel::getCount() const {
    return m_devices.count();
}

bool DeviceModel::getAuto() const {
    return m_auto;
}

bool DeviceModel::isReady() const {
    return m_currentDevice >= 0
        && m_currentDevice < m_devices.size()
        && m_devices[m_currentDevice] != nullptr
        && m_devices[m_currentDevice]->isReady();
}

void DeviceModel::closeAll() {
    for(Device *dev: m_devices) {
        dev->close();
    }
}

Device& DeviceModel::currentDevice() {
    return *m_devices[m_currentDevice];
}

void DeviceModel::setCurrent(int current) {
    if(m_currentDevice != current) {
        m_currentDevice = current;
        emit currentChanged();
    }
}

void DeviceModel::retake() {
    currentDevice().write(QByteArray("R\x07"));
}

void DeviceModel::report() {
    // Report creating...
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

        char buff[] = {'R', mode};
        currentDevice().write(QByteArray(buff, 2));
        _specifyOnModeChange(mode);
    } else {
        currentDevice().close();
        emit statusSignal(-1, -1);
    }
}

void DeviceModel::specifyMode() {
}

void DeviceModel::setSeries(QAbstractSeries* series) {
    // m_series = static_cast<QXYSeries*>(series);
}

void DeviceModel::setPins(int a, int b) {
    char buff[] = {'I', char(1), char(a), char(b)};
    currentDevice().write(QByteArray(buff, 4));
}

void DeviceModel::setDate(qint8 hours, qint8 min, qint8 year, qint8 month, qint8 day) {
    char time[] = {'T', 1, hours, min};
    char date[] = {'T', 2, day, month, year};

    currentDevice().write(QByteArray(time, 4));
    currentDevice().write(QByteArray(date, 5));
}

void DeviceModel::setAuto(bool automate) {
    if(automate != m_auto) {
        m_auto = automate;
        // currentDevice().write(_nextSwitch(0, 0));
        emit autoChanged();
    }
}

void DeviceModel::setVelocityFactor(double factor) {
    if(isReady()) {
        char buff[] = {'F', 1, char(factor), char(100*(factor-quint8(factor)))};
        currentDevice().write(QByteArray(buff, 4));
    }
}

void DeviceModel::flashCurrent(QString fileName) {
    if(isReady()) {
        currentDevice().flash(fileName);
    }
}

void DeviceModel::deviceError(QString error, Device::Error type) {
    emit firmwareError(error);
}

QHash<int, QByteArray> DeviceModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DIdRole] = "did";
    return roles;
}

void DeviceModel::_specifyOnModeChange(char mode) {
    switch(mode) {
    case 2:
        m_auto = true;
        setPins(1, 2);
        break;
    case 5:
        currentDevice().write(QByteArray("S\x0c\x01", 3));
        break;
    }
}

void DeviceModel::bind(Processor *p) {
    m_processors.append(p);
    p->setModel(this);
}

void DeviceModel::_packetRX(Device* dev, Packet* packet) {
    for(Processor *p: m_processors) {
        switch (packet->mod) {
        case Modes::FLASHING:
            p->process(*dev, packet->data.flash);
            break;
        case Modes::AMPL:
            p->process(*dev, packet->data.amplifier);
            break;
        case Modes::SWITCH:
            p->process(*dev, packet->data.swtch);
            break;
        case Modes::STARTING:
            p->process(*dev, packet->data.starting);
            break;
        case Modes::BATTERY:
            p->process(*dev, packet->data.battery);
            break;
        case Modes::FDR:
            p->process(*dev, packet->data.fdr);
            break;
        case Modes::NLD:
            p->process(*dev, packet->data.nld);
            break;
        case Modes::RX:
            p->process(*dev, packet->data.rx);
            break;
        default:
            break;
        }
    }
}
