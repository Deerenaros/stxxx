//    include/qml_packets.h is part of STx
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


#ifndef QML_PACKETS_H
#define QML_PACKETS_H

#include "packets.h"
#include "qobjectdefs.h"

#define WORST_PROPERTY(type, name) \
    private: Q_PROPERTY(type name READ get_##name WRITE set_##name)\
    public:\
        void set_##name(type value) { m_me.name = value; }\
        type get_##name() { return m_me.name; }

#define WORST_READONLY(type, name) \
    private: Q_PROPERTY(type name READ get_##name)\
    public:\
        type get_##name() { return m_me.name; }

#define WORST_INIT(type, pretype) \
    public: type(pretype &me) : m_me(me) {} \
    private: pretype &m_me;

#define WORST_ARRAY(type, pretype, size) \
    public: type(pretype *me) {m_me = me; m_size = size;} \
    private: pretype *m_me; size_t m_size;

struct QCable {
    Q_GADGET

    WORST_PROPERTY(quint8, cableType)
    WORST_PROPERTY(quint8, cableMask)

    WORST_INIT(QCable, Cable)
};

struct QBattery {
    Q_GADGET

    WORST_PROPERTY(quint8, charge)
    WORST_PROPERTY(quint8, isCharging)

    WORST_INIT(QBattery, Battery)
};

struct QStarting {
    Q_GADGET

    WORST_PROPERTY(quint8, currentMode)
public:
    quint8  currentMode;
    quint8  line1;
    quint8  line2;
    quint8  isSwitcher;
    quint8  isHighVoltage;
    Battery battery;

    struct Settings {
        quint8 hours;
        quint8 minutes;
        quint8 year;
        quint8 month;
        quint8 day;
    } settings;

    struct Switch {
        quint8 modeFrom;
        Cable  info;
    } swtch;

    struct Amplifier {
        quint8  vShift;
        quint8  amplify;
        quint8  isSpectr;
        quint16 dt;
        quint8  tRate;
        quint16 dU;
        quint8  URate;
        quint8  dc;
        quint8  ac;
        quint8  isTable;
        Cable   info;
    } amplifier;

    struct Receiver {
        quint16 startFrq;
        quint16 stopFrq;
        quint16 markFrq;
        quint8  stepFrq;
        quint8  module;
        quint8  isFixedFrq;
        quint8  isOscFixed;
        quint8  isSpcFixed;
        quint8  amplifyCoef;
        quint8  isDiffMode;
        quint16 dt;
        quint8  tRate;
        quint8  modeFrom;
        quint8  detector;
        quint8  range;
    } receiver;

    struct {
        quint8 amplitude;

        Cable info;
    };

    struct {
        struct Coef {
            quint8 integer;
            quint8 hundredth;
        } coef;

        Cable cable;
    } fdr;

    WORST_INIT(QStarting, Starting)
};

struct QAmplifier {
public:
    quint8 submode;
    struct Oscillogramme {
        quint8 data[192];
        quint8 swing;
        quint8 rate;
    } oscillogramme;

    struct Spectre {
        quint8 part;
        quint8 data[128];
    } spectr;

    WORST_INIT(QAmplifier, Amplifier)
};

struct QSwitch {
public:
    quint16 dc;
    quint16 ac;
    quint8  line1;
    quint8  line2;

    WORST_INIT(QSwitch, Switch)
};

struct QReceiver {
public:
    quint8 submode;

    struct Spectre {
        quint8 number;
        quint8 data[100];
    } spectr;

    struct Search {
        quint8 measure;
        quint8 count;
        quint8 signls[MAX_PACKET_SIZE];
    } search;

    struct Oscillogramme {
        quint8 part;
        quint8 data[160];
    } oscillogramme;

    WORST_INIT(QReceiver, Receiver)
};

struct QNLD {
public:
    quint8 red;
    quint8 blue;
    quint8 white;
    quint8 line1;
    quint8 line2;
    quint8 signalLevel;

    WORST_INIT(QNLD, NLD)
};

struct QFDR_Measure {
    Q_GADGET

    WORST_PROPERTY(quint16, len)
    WORST_PROPERTY(quint8,  lvl)

    WORST_INIT(QFDR_Measure, FDR::Measure)
};

struct QFDR {
    Q_GADGET

    WORST_PROPERTY(quint8, submode)
    WORST_PROPERTY(quint8, size)
    WORST_PROPERTY(quint8, line1)
    WORST_PROPERTY(quint8, line2)

    WORST_INIT(QFDR, FDR)
};

#undef Q_STRUCT
#undef WORST_PROPERTY

#endif // QML_PACKETS_H
