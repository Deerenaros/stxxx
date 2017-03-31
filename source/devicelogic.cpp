#include "devicelogic.h"

#include "devicesmodel.h"

DeviceLogic::DeviceLogic()
{

}

void DeviceLogic::process(Device &dev, Starting& data) {
    process(dev, data.battery);

    emit model->pinsChanged(data.line1, data.line2);

    auto &s = data.settings;
    switch(data.currentMode) {
    case 1:
        emit model->dateSignal(s.hours, s.minutes, s.year, s.month, s.day);
        break;
    }
}

void DeviceLogic::process(Device &dev, Battery& data) {
    Q_UNUSED(dev);

    emit model->statusSignal(BATTERY_SCALE*data.charge, data.isCharging == 1);
}

void DeviceLogic::process(Device &dev, Switch& data) {
    Q_UNUSED(dev);

    qint8 input = data.line1;
    qint8 output = data.line2;
    double dc = data.ac / SWITCH_ACDC_SCALE;
    double ac = data.dc / SWITCH_ACDC_SCALE;

    qdebug("switch") << QString("dc: %1 ac: %2 in: %3 out: %4").arg(dc).arg(ac).arg(input).arg(output);

    emit model->switcherSignal(input, output, ac, dc);

//    if(m_auto && _haveToContinueSwitch(QPair<qint8, qint8>(input, output))) {
//        dev.write(_nextSwitch(input, output));
//        m_waitingReset = SWITCH_PACKETS_TO_RESET;
//    } else if(m_auto) {
//        m_waitingReset += 1;
//    }
}

void DeviceLogic::process(Device &dev, Amplifier& data) {
    Q_UNUSED(dev);

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

    emit model->amplifierSignal(size);
}

void DeviceLogic::process(Device &dev, Receiver& data) {
    Q_UNUSED(data);
    Q_UNUSED(dev);
}

void DeviceLogic::process(Device &dev, NLD& data) {
    Q_UNUSED(data);
    Q_UNUSED(dev);
}

void DeviceLogic::process(Device &dev, FDR& data) {
    Q_UNUSED(dev);

    if(data.submode != FDR::START) {
        if(automate && data.line1 != char(7) && data.line2 != char(8)) {
            // dev.write(_nextSwitch(data.line1, data.line2));
            // emit m_model->pinsChanged(m_waitingSwitch.first, m_waitingSwitch.second);
        }

        if(data.size > 0) {
            QList<QPair<double, int>> list;
            int a = data.line1, b = data.line2;

            for(qint8 i = 0; i < data.size; i++) {
                list << QPair<double, int>(data.measments[i].len / 10., data.measments[i].lvl);
            }

            for(auto &item: list) {
                emit model->fdrSignal(1, a, b, item.first, item.second);
            }

            qSort(list.begin(), list.end(), [](QPair<double, int> a, QPair<double, int> b) {
                return a.second > b.second;
            });

            emit model->fdrSignal(2, a, b, list.first().first, list.first().second);
        } else {
            emit model->fdrSignal(-1, data.line1, data.line2, 0, 0);
        }
    } else {
        emit model->fdrSignal(0, data.line1, data.line2, 0, 0);
    }
}

void DeviceLogic::process(Device &dev, Flashing& flash) {
    switch (flash.status) {
    case 1:
        qdebug("flash") << "not complete";
        break;
    case 7:
        /* complete */
        break;
    default:
        dev.flash();
        break;
    }
}
