#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "device.h"
#include "packets.h"
#include "hash.h"

class DeviceModel;

typedef const void * cvoid;

template<typename TO, typename FROM>
TO just_cast(FROM val) {
    return TO(val);
}

class Processor
{
public:
    permanent cvoid NOTHING = "";
    permanent cvoid TAKE    = "";

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
    virtual cvoid value(const size_t, cvoid) = 0;

    DeviceModel* model;

public:
    template<typename T>
    T value(const char *name, T data) {
        cvoid ret = value(hash(name), reinterpret_cast<cvoid>(data));
        return just_cast<T>(ret);
    }
};

#endif // PROCESSOR_H
