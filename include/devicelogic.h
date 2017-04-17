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

#ifndef DEVICELOGIC_H
#define DEVICELOGIC_H

#include <QAbstractSeries>
#include <QXYSeries>
#include <QAreaSeries>
#include <QLineSeries>

#include "betterdebug.h"
#include "processor.h"

QT_CHARTS_USE_NAMESPACE

class DeviceLogic : public Processor
{
    const double SWITCH_ACDC_SCALE = 10.;
    const double BATTERY_SCALE = 100./5;
    static const int AMPLIFIER_LENGTH = 256;
    static const int AMPLIFIER_BYTE_SHIFT = 110;

public:

    DeviceLogic();

    void process(Device&, Starting&);
    void process(Device&, Battery&);
    void process(Device&, Amplifier&);
    void process(Device&, Switch&);
    void process(Device&, Receiver&);
    void process(Device&, NLD&);
    void process(Device&, FDR&);
    void process(Device&, Flashing&);

    void process(Device&, QString&);

protected:
    cvoid value(const size_t, cvoid, Device*);

private:
    Pins m_stop = Pins(7, 8);

    bool m_automate = false;
    QVector<QPointF> m_amplifier;
    QLineSeries m_spectrum;
    QXYSeries *m_ampl = nullptr;
    QAreaSeries *m_spec = nullptr;
};

#endif // DEVICELOGIC_H
