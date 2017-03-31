//    include/packets.h is part of STx
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

#ifndef PACKETS_H
#define PACKETS_H

#include <QtGlobal>

#pragma pack(push, 1)
#define MAX_PACKET_SIZE 256
#define permanent static constexpr

typedef qint8 mode;

struct Stuff {
    permanent char START = (char)0xc0;
    permanent char END = (char)0xc1;
    permanent char ESC = (char)0xdb;
    permanent char ESC_START = (char)0xdc;
    permanent char ESC_END = (char)0xdd;
    permanent char ESC_ESC = (char)0xde;
};

struct Modes {
    permanent char BATTERY = 'B';
    permanent char FLASHING = 'D';
    permanent char PROGRESS = 'P';
    permanent char STARTING = 'S';
    permanent char SWITCH = 'V';
    permanent char AMPL = 'O';
    permanent char RX = 'C';
    permanent char FDR = 'T';
    permanent char NLD = 'N';
};

struct Constants {
    static constexpr quint8 usec = 0;
    static constexpr quint8 msec = 1;

    static constexpr quint8 uV = 0;
    static constexpr quint8 mV = 1;
    static constexpr quint8 dV = 2;

    static constexpr quint8 AM = 0;
    static constexpr quint8 FM = 1;

    static constexpr quint8 lowFQ  = 0;
    static constexpr quint8 wideFQ = 1;
};

struct Flashing {
    quint8 status;
};

struct Cable {
    quint8 cableType;           ///< cable's type
    quint8 cableMask;           ///< cable's using pins' mask
};

struct Battery {
    quint8 charge;              ///< Current charge (0~5)
    quint8 isCharging;          ///< Is currently charging
};

struct Starting {
    struct Settings {
        quint8 hours;       ///< hours (0~23)
        quint8 minutes;     ///< minutes (0~59)
        quint8 year;        ///< second half of year's digits (16, 17, 18, etc)
        quint8 month;       ///< month (0~12)
        quint8 day;         ///< day (0~31)
    };

    struct Switch {
        quint8 modeFrom;    ///< mode from entered to switcher
        Cable  info;
    };

    struct Amplifier {
        quint8  vShift;     ///< voltage shift plus 26V
        quint8  amplify;    ///< current amplify 0-9: 0, 5, 10, 20, 30, 40, 50, 60, 25dB
        quint8  isSpectr;   ///< is spectr on
        quint16 dt;         ///< time for division (X-axis)
        quint8  tRate;      ///< time rate (0 - usec, 1 - msec)
        quint16 dU;         ///< voltage for division
        quint8  URate;      ///< voltage rate (0 - uV, 1 - mV, 2 - dV)
        quint8  dc;         ///< direct current value
        quint8  ac;         ///< alternating current value
        quint8  isTable;    ///< is table mode
        Cable   info;
    };

    struct Receiver {
        quint16 startFrq;   ///< starting frequency
        quint16 stopFrq;    ///< stop frequency
        quint16 markFrq;    ///< marked frequency
        quint8  stepFrq;    ///< step of frequency
        quint8  module;     ///< modulation type (0 - AM, 1 - FM)
        quint8  isFixedFrq; ///< Is frequency being fixed
        quint8  isOscFixed; ///< Is osciloscope being fixed
        quint8  isSpcFixed; ///< Is spectr being fixed
        quint8  amplifyCoef;///< Coef. of signal amplifing
        quint8  isDiffMode; ///< Is diff mode
        quint16 dt;         ///< time for division (X-axis)
        quint8  tRate;      ///< time rate (0 - usec, 1 - msec)
        quint8  modeFrom;   ///< mode from entered to receiver
        quint8  detector;   ///< detector (deprecated, not using)
        quint8  range;      ///< range of freqs (0 is below 30 MHz, 1 is below 150 MHz)
    };

    struct FDR {
        struct Coef {
            quint8 integer;
            quint8 hundredth;
        } coef;             ///< velocity factor of signal over cable

        Cable cable;
    };

    struct NLD {
        quint8 amplitude;   ///< Signal's amplitude (0~4)

        Cable info;
    };

    quint8  currentMode;         ///< Current working mode
    quint8  line1;               ///< Current first line (0~8)
    quint8  line2;               ///< Current second line (0~8)
    quint8  isSwitcher;          ///< Is switcher on
    quint8  isHighVoltage;       ///< Is voltage excessed
    Battery battery;

    union {
        Starting::Settings settings;
        Starting::Switch swtch;
        Starting::Amplifier amplifier;
        Starting::Receiver receiver;
        Starting::NLD nld;
        Starting::FDR fdr;
    };
};

struct Amplifier {
    struct Oscillogramme {
        quint8 swing;       ///< signal swing (hi - low)
        quint8 rate;        ///< 1 for mV or 2 for dV
        quint8 data[192];   ///< oscilorame
    };

    struct Spectre {
        quint8 part;        ///< first (0) or second (1) part
        quint8 data[128];   ///< spectr data
    };

    quint8 submode;

    union {
        Amplifier::Oscillogramme oscillogramme;

        Amplifier::Spectre spectr;
    };
};

struct Switch {
    quint16 dc;
    quint16 ac;
    quint8  line1;
    quint8  line2;
};

struct Receiver {
    struct Spectre {
        quint8 number;
        quint8 data[100];
    };

    struct Search {
        quint8 measure;     ///< measures of freq (1) or levels (2)
        quint8 count;
        quint8 signls[MAX_PACKET_SIZE];
    };

    struct Oscillogramme {
        quint8 part;        ///< first or second part of data (0 or 1)
        quint8 data[160];
    };

    quint8 submode;

    union {
        Receiver::Spectre spectr;

        Receiver::Search search;

        Receiver::Oscillogramme oscillogramme;
    };
};

struct NLD {
    quint8 red;                 ///< red level (0~250)
    quint8 blue;                ///< blue level (0~250)
    quint8 white;               ///< white level (0~250)
    quint8 line1;
    quint8 line2;
    quint8 signalLevel;         ///< Signal's level (0~4)
};

struct FDR {
    enum _submodes {
        START = 0,
        OK = 1,
        TABLE = 2,
    };

    struct Measure {
        quint16 len;
        quint8  lvl;
    };

    quint8 submode;
    quint8 size;
    quint8 line1;
    quint8 line2;

    union {
        FDR::Measure measure;

        FDR::Measure measments[MAX_PACKET_SIZE];
    };
};

struct Packet {
    mode mod;
    union Misc {
        Starting starting;
        Battery battery;
        Amplifier amplifier;
        Switch swtch;
        Receiver rx;
        NLD nld;
        FDR fdr;
        Flashing flash;
    } data;
};
#pragma pack(pop)

#endif // PACKETS_H
