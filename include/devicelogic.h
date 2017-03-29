#ifndef DEVICELOGIC_H
#define DEVICELOGIC_H

#include <QAbstractSeries>
#include <QXYSeries>

#include "processor.h"
#include "betterdebug.h"

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

private:
    bool m_auto = false;

    QVector<QPointF> m_amplifier;
    QXYSeries *m_series = nullptr;
};

#endif // DEVICELOGIC_H
