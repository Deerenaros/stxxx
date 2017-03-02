#ifndef PACKETS_H
#define PACKETS_H

#include <QtGlobal>

#pragma pack(push, 1)
#define MAX_PACKET_SIZE 256

struct Modes {
    enum _modes {
        STARTING = 'S',
        SWITCH = 'V',
        AMPL = 'O',
        AMPLspc = 'C',
        AMPLtbl = 'V',
        RXspc = 'C',
        RXosc = 'O',
        RXsrch = 'T',
        FDR = 'T',
        FDRan = 'A',
        NLD = 'N',
    };
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

struct Cable {
    quint8 cableType;           ///< cable's type
    quint8 cableMask;           ///< cable's using pins' mask
};

struct Battery {
    quint8 charge;              ///< Current charge (0~5)
    quint8 isCharging;          ///< Is currently charging
};

struct Starting {
    quint8  currentMode;         ///< Current working mode
    quint8  line1;               ///< Current first line (0~8)
    quint8  line2;               ///< Current second line (0~8)
    quint8  isSwitcher;          ///< Is switcher on
    quint8  isHighVoltage;       ///< Is voltage excessed
    Battery battery;

    union {
        struct {
            quint8 hours;       ///< hours (0~23)
            quint8 minutes;     ///< minutes (0~59)
            quint8 year;        ///< second half of year's digits (16, 17, 18, etc)
            quint8 month;       ///< month (0~12)
            quint8 day;         ///< day (0~31)
        } settings;

        struct {
            quint8 modeFrom;    ///< mode from entered to switcher
            Cable  info;
        } swtch;

        struct {
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
        } amplifier;

        struct {
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
        } receiver;

        struct {
            quint8 amplitude;   ///< Signal's amplitude (0~4)

            Cable info;
        };

        struct {
            struct {
                quint8 integer;
                quint8 hundredth;
            } coef;             ///< velocity factor of signal over cable

            Cable info;
        } fdr;
    };
};

struct Amplifier {
    quint8 submode;
    union {
        struct {
            quint8 data[192];   ///< oscilorame
            quint8 swing;       ///< signal swing (hi - low)
            quint8 rate;        ///< 1 for mV or 2 for dV
        } oscilograme;

        struct {
            quint8 part;        ///< first (0) or second (1) part
            quint8 data[128];   ///< spectr data
        } spectr;
    };
};

struct Switch {
    quint16 dc;
    quint16 ac;
    quint8  line1;
    quint8  line2;
};

struct Reciver {
    quint8 submode;
    union {
        struct {
            quint8 number;
            quint8 data[100];
        } spectr;

        struct {
            quint8 measure;     ///< measures of freq (1) or levels (2)
            quint8 count;
            quint8 signls[MAX_PACKET_SIZE];
        } search;

        struct {
            quint8 part;        ///< first or second part of data (0 or 1)
            quint8 data[160];
        } osciloscope;
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

    quint8 submode;
    quint8 size;
    quint8 line1;
    quint8 line2;

    union {
        struct {
            quint16 len;
            quint8  lvl;
        } measure;

        struct {
            quint16 len;
            quint8  lvl;
        } measments[MAX_PACKET_SIZE];
    };
};

struct Packet {
    quint8  mode;
    union Misc {
        Starting starting;
        Battery battery;
        Amplifier amplifier;
        Switch swtch;
        Reciver rx;
        NLD nld;
        FDR fdr;
    } data;
};
#pragma pack(pop)

#endif // PACKETS_H
