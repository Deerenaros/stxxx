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


#include "betterdebug.h"
#include "devicesmodel.h"

#include <QAreaSeries>
#include <QQuickView>
#include <QList>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DeviceModel::DeviceModel(QQuickView *appViewer, QObject *parent)
    : QAbstractItemModel(parent)
    , m_waitingSwitch(QPair<quint8, quint8>(0, 0))
    , m_appViewer(appViewer)
{
    for(const QSerialPortInfo &info: QSerialPortInfo::availablePorts()) {
        debug << DMark("found") << QString("%1 0x%2 0x%3")
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

void DeviceModel::setMode(char mode) {
    debug << DMark("mode") << int(mode);

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
    m_series = static_cast<QXYSeries*>(series);
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
        currentDevice().write(_nextSwitch(0, 0));
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

QByteArray DeviceModel::_nextSwitch(qint8 a, qint8 b) {
    if(b == 0 || a == 0) {
        a = 1;
        b = 2;
    } else if(b >= 8) {
        a += 1;
        b = a+1;
    } else {
        b += 1;
    }

    if(a > 7) {
        a = 1;
        b = 2;
    }

    char buff[] = {'I', 1, a, b};
    m_waitingSwitch = QPair<qint8, qint8>(a, b);
    return QByteArray(buff, 4);
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

void DeviceModel::_process(Starting& data) {
    _process(data.battery);

    emit pinsChanged(data.line1, data.line2);

    switch(data.currentMode) {
    case 1:
        emit dateSignal(data.settings.hours,
                        data.settings.minutes,
                        data.settings.year,
                        data.settings.month,
                        data.settings.day);
        break;
    }
}

void DeviceModel::_process(Battery& data) {
    emit statusSignal(BATTERY_SCALE*data.charge, data.isCharging == 1);
}

bool DeviceModel::_haveToContinueSwitch(QPair<qint8, qint8> inout) {
    bool result = !(m_waitingSwitch.first || m_waitingSwitch.second);
    result |= m_waitingReset || inout == m_waitingSwitch;
    return result;
}

void DeviceModel::_process(Switch& data) {
    qint8 input = data.line1;
    qint8 output = data.line2;
    double dc = data.ac / SWITCH_ACDC_SCALE;
    double ac = data.dc / SWITCH_ACDC_SCALE;

    debug << DMark("switch") << QString("dc: %1 ac: %2 in: %3 out: %4").arg(dc).arg(ac).arg(input).arg(output);

    emit switcherSignal(input, output, ac, dc);

    if(m_auto && _haveToContinueSwitch(QPair<qint8, qint8>(input, output))) {
        currentDevice().write(_nextSwitch(input, output));
        m_waitingReset = SWITCH_PACKETS_TO_RESET;
    } else if(m_auto) {
        m_waitingReset += 1;
    }
}

void DeviceModel::_process(Amplifier& data) {
    int i = 0, k = m_amplifier.size();

    for(auto byte = std::begin(data.oscillogramme.data); byte != std::end(data.oscillogramme.data); byte++) {
        m_amplifier.insert(k, QPointF(i--, (*byte) - AMPLIFIER_BYTE_SHIFT));
    }

    int size = m_amplifier.size();
    if(size > 1024) {
        m_amplifier.remove(0, size - AMPLIFIER_LENGTH);
    }

    for(i = 0; i < int(m_amplifier.size() - sizeof(data.oscillogramme.data)); i++) {
        m_amplifier[i].rx() -= sizeof(data.oscillogramme.data);
    }

    if(m_series != nullptr) {
        m_series->replace(m_amplifier);
    }

    emit amplifierSignal(size);
}

void DeviceModel::_process(Receiver& data) {
    Q_UNUSED(data);
}

void DeviceModel::_process(NLD& data) {
    Q_UNUSED(data);
}

void DeviceModel::_process(FDR& data) {
    if(data.submode != FDR::START) {
        if(m_auto && data.line1 != char(7) && data.line2 != char(8)) {
            currentDevice().write(_nextSwitch(data.line1, data.line2));
            emit pinsChanged(m_waitingSwitch.first, m_waitingSwitch.second);
        }

        if(data.size > 0) {
            QList<QPair<double, int>> list;
            int a = data.line1, b = data.line2;

            for(qint8 i = 0; i < data.size; i++) {
                list << QPair<double, int>(data.measments[i].len / 10., data.measments[i].lvl);
            }

            for(auto &item: list) {
                emit fdrSignal(1, a, b, item.first, item.second);
            }

            qSort(list.begin(), list.end(), [](QPair<double, int> a, QPair<double, int> b) {
                return a.second > b.second;
            });

            emit fdrSignal(2, a, b, list.first().first, list.first().second);
        } else {
            emit fdrSignal(-1, data.line1, data.line2, 0, 0);
        }
    } else {
        emit fdrSignal(0, data.line1, data.line2, 0, 0);
    }
}

void DeviceModel::_process(Flashing& flash) {
    switch (flash.status) {
    case 1:
        debug << DMark("flash") << "not complete";
        break;
    case 7:
        /* complete */
        break;
    default:
        currentDevice().flash();
        break;
    }
}

void DeviceModel::_packetRX(Device* sender, QByteArray* packet) {
    int devIndex = m_devices.indexOf(sender);
    Q_UNUSED(devIndex);
    auto& _packet = *reinterpret_cast<Packet*>(packet->data());

    switch (_packet.mod) {
    case Modes::FLASHING:
        _process(_packet.data.flash);
        break;
    case Modes::AMPL:
        _process(_packet.data.amplifier);
        break;
    case Modes::SWITCH:
        _process(_packet.data.swtch);
        break;
    case Modes::STARTING:
        _process(_packet.data.starting);
        break;
    case Modes::BATTERY:
        _process(_packet.data.battery);
        break;
    case Modes::FDR:
        _process(_packet.data.fdr);
        break;
    case Modes::NLD:
        _process(_packet.data.nld);
        break;
    case Modes::RX:
        _process(_packet.data.rx);
        break;
    default:
        break;
    }

    delete packet;
}
