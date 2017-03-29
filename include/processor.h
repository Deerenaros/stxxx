#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "device.h"
#include "packets.h"

class DeviceModel;

class Processor
{
public:
    void setModel(DeviceModel* m) {model = m;}

    virtual void process(Device&, Starting&) = 0;
    virtual void process(Device&, Battery&) = 0;
    virtual void process(Device&, Amplifier&) = 0;
    virtual void process(Device&, Switch&) = 0;
    virtual void process(Device&, Receiver&) = 0;
    virtual void process(Device&, NLD&) = 0;
    virtual void process(Device&, FDR&) = 0;
    virtual void process(Device&, Flashing&) = 0;

protected:
    DeviceModel* model;
};

#endif // PROCESSOR_H
