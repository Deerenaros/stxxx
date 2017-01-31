#include "betterdebug.h"
#include "devicesmodel.h"

#include <QAreaSeries>
#include <QQuickView>

QT_CHARTS_USE_NAMESPACE

Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

DevicesModel::DevicesModel(QQuickView *appViewer, QObject *parent)
    : QAbstractItemModel(parent)
    , m_waitingSwitch(QPair<quint8, quint8>(0, 0))
    , m_appViewer(appViewer)
{
    qRegisterMetaType<QAbstractSeries*>();
    qRegisterMetaType<QAbstractAxis*>();

    for(const QSerialPortInfo &info: QSerialPortInfo::availablePorts()) {
        debug << DMark("found") << QString("%1 0x%2 0x%3")
                    .arg(info.description())
                    .arg(info.vendorIdentifier(), 4, 16, QChar('0'))
                    .arg(info.productIdentifier(), 4, 16, QChar('0'));

        if(SUPPORTED_PIDS.contains(info.productIdentifier())) {
            QString name = SUPPORTED_PIDS[info.productIdentifier()];
            auto dev = new Device(name, info, this);
            m_devices.append(dev);
            connect(dev, &Device::packetRead, this, &DevicesModel::_packetRX);
            dev->start();
        }
    }

    if(m_devices.count() > 0) {
        m_currentDevice = 0;
    }
}

DevicesModel::~DevicesModel() {
    for(Device *dev: m_devices) {
        dev->wait();
    }
}

int DevicesModel::rowCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return m_devices.count();
}

int DevicesModel::columnCount(const QModelIndex & parent) const {
    Q_UNUSED(parent);
    return COLUMN_COUNT;
}

QModelIndex DevicesModel::index(int row, int column, const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return createIndex(row, column);
}

QModelIndex DevicesModel::parent(const QModelIndex &child) const {
    Q_UNUSED(child);
    return QModelIndex();
}

QVariant DevicesModel::data(const QModelIndex & index, int role) const {
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

int DevicesModel::getCurrent() const {
    return m_currentDevice;
}

int DevicesModel::getCount() const {
    return m_devices.count();
}

bool DevicesModel::getReady() const {
    return m_devices[m_currentDevice]->isReady();
}

Device& DevicesModel::currentDevice() {
    return *m_devices[m_currentDevice];
}

void DevicesModel::setCurrent(int current) {
    m_currentDevice = current;
    emit currentChanged();
}

void DevicesModel::setModeForCurrent(char mode) {
    if(mode) {
        if(!currentDevice().isReady()) {
            currentDevice().open();
        }

        if(mode > 1) {
            mode += 1;
        }

        char buff[] = {'R', mode};
        currentDevice().write(QByteArray(buff, 2));
    } else {
        currentDevice().close();
        emit statusSignal(-1, -1);
    }
}

void DevicesModel::specifyModeForCurrent() {
}

void DevicesModel::setSeries(QAbstractSeries* series) {
    m_series = static_cast<QXYSeries*>(series);
}

QHash<int, QByteArray> DevicesModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[NameRole] = "name";
    roles[DIdRole] = "did";
    return roles;
}

QByteArray DevicesModel::_nextSwitch(qint8 in, qint8 out) {
    if(in == 0 || out == 0) {
        in = 1;
        out = 2;
    } else if(out >= 8) {
        in += 1;
        out = 2;
    } else {
        out += 1;
    }

    if(in > 7) {
        in = 1;
        out = 2;
    }

    char buff[] = {'I', 1, in, out};
    m_waitingSwitch = QPair<quint8, quint8>(in, out);
    return QByteArray(buff, 4);
}

void DevicesModel::_toStatus(QByteArray& bytes) {
    _toBattery(bytes.mid(5, 3));
}

void DevicesModel::_toBattery(const QByteArray &bytes) {
    emit statusSignal(BATTERY_SCALE*bytes[1], bytes[2] == 1);
}

void DevicesModel::_toSwitch(QByteArray& bytes) {
    auto data = bytes.mid(1, 6).data();

    qint8 input = *reinterpret_cast<qint8*>(data+4);
    qint8 output = *reinterpret_cast<qint8*>(data+5);
    double dc = *reinterpret_cast<qint16*>(data+0) / 10;
    double ac = *reinterpret_cast<qint16*>(data+2) / 10;
    debug << DMark("switch") << QString("dc: %1 ac: %2 in: %3 out: %4").arg(dc).arg(ac).arg(input).arg(output);

    if(QPair<quint8, quint8>(input, output) == m_waitingSwitch) {
        currentDevice().write(_nextSwitch(input, output));
        emit switcherSignal(input, output, ac, dc);
    }
}

void DevicesModel::_toAmplifier(QByteArray& bytes) {
    int i = 0, k = m_amplifier.size();
    auto data = bytes.mid(2, bytes.length() - 6);
    for(auto byte = data.rbegin(); byte != data.rend(); byte++) {
        m_amplifier.insert(k, QPointF(i--, (*byte) - AMPLIFIER_BYTE_SHIFT));
    }

    int size = m_amplifier.size();
    if(size > 1024) {
        m_amplifier.remove(0, size - 1024);
    }

    for(i = 0; i < m_amplifier.size() - data.size(); i++) {
        m_amplifier[i].rx() -= data.size();
    }

    if(m_series != nullptr) {
        m_series->replace(m_amplifier);
    }

    emit amplifierSignal(size);
}

void DevicesModel::_toReciever(QByteArray& bytes) {
    Q_UNUSED(bytes);
}

void DevicesModel::_toNLD(QByteArray& bytes) {
    Q_UNUSED(bytes);
}

void DevicesModel::_toFDR(QByteArray& bytes) {
    Q_UNUSED(bytes);
}

void DevicesModel::_packetRX(Device* sender, QByteArray* packet) {
    int devIndex = m_devices.indexOf(sender);
    Q_UNUSED(devIndex);

    switch (packet->at(0)) {
    case 'O':
        _toAmplifier(*packet);
        break;
    case 'V':
        _toSwitch(*packet);
        break;
    case 'S':
        _toStatus(*packet);
        break;
    case 'B':
        _toBattery(*packet);
        break;
    case 'T':
        _toFDR(*packet);
        break;
    case 'N':
        _toNLD(*packet);
        break;
    case 'C':
        _toReciever(*packet);
        break;
    default:
        break;
    }

    delete packet;
}
