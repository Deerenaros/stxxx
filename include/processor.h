//    include/processor.h is part of STx
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

#ifndef PROCESSOR_H
#define PROCESSOR_H

#include "device.h"
#include "packets.h"
#include "hash.h"
#include "model.h"


typedef const void * cvoid;

template<typename TO, typename FROM>
TO just_cast(FROM val) {
    return TO(val);
}

class Processor : public QObject
{
    Q_OBJECT

public:
    Processor(Model& model)
        : QObject(&model)
        , model(model)
    {}

    permanent cvoid EVENT   = reinterpret_cast<cvoid>(0xE0000000);
    permanent cvoid NOTHING = reinterpret_cast<cvoid>(0xE0000001);
    permanent cvoid TAKE    = reinterpret_cast<cvoid>(0xE0000002);

    virtual void process(Device&, Starting&) = 0;
    virtual void process(Device&, Battery&) = 0;
    virtual void process(Device&, Amplifier&) = 0;
    virtual void process(Device&, Switch&r) = 0;
    virtual void process(Device&, Receiver&) = 0;
    virtual void process(Device&, NLD&) = 0;
    virtual void process(Device&, FDR&) = 0;
    virtual void process(Device&, Flashing&) = 0;


protected:
    virtual cvoid handle(const size_t, cvoid, Device*) = 0;

    Model& model;

public:
    template<typename T>
    T handle(const char *name, T data, Device *dev = nullptr) {
        cvoid ret = handle(hash(name), reinterpret_cast<cvoid>(data), dev);
        return just_cast<T>(ret);
    }
};

#endif // PROCESSOR_H
