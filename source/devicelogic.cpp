//    include/devicelogic.h is part of STx
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

#include "devicelogic.h"

#include "devicesmodel.h"

DeviceLogic::DeviceLogic() {
}

void DeviceLogic::process(Device &dev, Starting& data) {
    process(dev, data.battery);

    emit model->pinsChanged(data.pins.a, data.pins.b);

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

    qint8 input = data.pins.a;
    qint8 output = data.pins.b;
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

    if(m_ampl != nullptr) {
        model->series.amplifier->replace(m_amplifier);
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
//    min: 200.0
//    max: 205000.6

    static quint8 hi;
    if(data.submode == FDR::SPECTRUM) {
        qdebug("fdr") << "spectrum";
        if(data.number == 0) {
            m_spectrum.clear();
            hi = 0;
        }

        int i = m_spectrum.count();
        for(auto byte = std::begin(data.spectrum); byte != std::end(data.spectrum); byte++, i++) {
            if(m_spectrum.count() && byte) {
                m_spectrum.append(QPointF(i/2.1f, m_spectrum.at(m_spectrum.count()-1).y()));
            }

            m_spectrum.append(QPointF(i/2.1f, *byte));
            hi = (*byte > hi ? *byte : hi);
        }

        if(data.number == 3) {
            model->series.fdr->setUpperSeries(&m_spectrum);
        }
    } else if(data.submode == FDR::OK) {
        if(model->property("automate").toBool() && !dev.current.next().is(m_stop)) {
            dev.setPins(dev.current.next());
        }

        if(data.size > 0) {
            QList<QPair<double, int>> list;
            int a = data.pins.a, b = data.pins.b;

            for(qint8 i = 0; i < data.size; i++) {
                list << QPair<double, int>(data.measments[i].len / 10., data.measments[i].lvl);
            }

            for(auto &item: list) {
                emit model->fdrSignal(1, a, b, item.first, item.second);
            }
            emit model->fdrSpectrum(0.8*list.first().first, 1.2*list.last().first, 1.1*hi);

            qSort(list.begin(), list.end(), [](QPair<double, int> a, QPair<double, int> b) {
                return a.second > b.second;
            });
            emit model->fdrSignal(2, a, b, list.first().first, list.first().second);
        } else {
            emit model->fdrSignal(-1, data.pins.a, data.pins.b, 0, 0);
        }
    } else if(data.submode == FDR::START) {
        emit model->fdrSignal(0, data.pins.a, data.pins.b, 0, 0);
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

cvoid DeviceLogic::handle(const size_t name, cvoid p, Device *dev) {
    switch(name) {
    case "automate"_h:
        if(p != TAKE) {
            m_automate = (p != reinterpret_cast<cvoid>(false));
            if(m_automate) {
                m_stop = dev->current;
                qdebug("fdr") << "stop auto @" << m_stop.a << "," << m_stop.b;
            }
        }

        return reinterpret_cast<cvoid>(m_automate);
    default:
        return NOTHING;
    }
}
