#ifndef DEVICESMODEL_H
#define DEVICESMODEL_H

#include <QVariant>
#include <QVariantList>
#include <QAbstractItemModel>
#include <QMap>
#include <QPair>
#include <QThread>

#include <QAbstractSeries>
#include <QXYSeries>

#include "device.h"


QT_BEGIN_NAMESPACE
class QQuickView;
QT_END_NAMESPACE

QT_CHARTS_USE_NAMESPACE

class DevicesModel
        : public QAbstractItemModel
{
    Q_OBJECT
#pragma pack(push, 1)
    struct ReceiverHeader {
        qint16 start;
        qint16 stop;
        qint16 mark;
        qint8 step;
        qint8 type;
        qint8 oscFixed;
        qint8 spcFixed;
        qint8 amplify;
        qint8 diffMode;
        qint16 dt;
        qint8 rate;
        qint8 modeFrom;
        qint8 detector;

        enum {
            AM, FM
        };

        enum {
            US, MS
        };
    };
#pragma pack(pop)

    static const int COLUMN_COUNT = 1;
    static const int AMPLIFIER_BYTE_SHIFT = 110;
    static const int SWITCH_MODE = 3;
    static const int SWITCH_PACKETS_TO_RESET = -5;
    static const int AMPLIFIER_LENGTH = 256;
    const double SWITCH_ACDC_SCALE = 10.;
    const double BATTERY_SCALE = 100./5;
    const QMap<uint16_t, QString> SUPPORTED_PIDS = {
        { 0x6015, "ST300" }
    };

    Q_PROPERTY(int current READ getCurrent WRITE setCurrent NOTIFY currentChanged)
    Q_PROPERTY(bool automate READ getAuto WRITE setAuto NOTIFY autoChanged)
    Q_PROPERTY(bool isReady READ getReady)
    Q_PROPERTY(int count READ getCount NOTIFY countChanged)

public:
    explicit DevicesModel(QQuickView *appViewer, QObject *parent = 0);
    ~DevicesModel();

    enum DevicesRoles {
        NameRole = Qt::UserRole + 1,
        DIdRole
    };

    int rowCount(const QModelIndex & parent = QModelIndex()) const;
    int columnCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex & index, int role = Qt::DisplayRole) const;
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;
    QModelIndex parent(const QModelIndex &child = QModelIndex()) const;

    Device& currentDevice();
    int getCurrent() const;
    bool getReady() const;
    bool getAuto() const;
    int getCount() const;

Q_SIGNALS:
signals:
    void amplifierSignal(int count);
    void switcherSignal(int a, int b, double ac, double dc);
    void statusSignal(double charge, bool isCharging);
    void dateSignal(int hours, int min, int year, int month, int day);
    void fdrSignal(int what, int a, int b, double len, unsigned lvl);
    void pinsChanged(int a, int b);

    int currentChanged();
    void countChanged();
    void autoChanged();

public slots:
    void closeAll();
    void setCurrent(int);
    void setAuto(bool);
    void setModeForCurrent(char);
    void specifyModeForCurrent();
    void setSeries(QAbstractSeries*);
    void setPins(int, int);
    void setVelocityFactor(double factor);

    void setDate(qint8 hours, qint8 min, qint8 year, qint8 month, qint8 day);

protected:
    QHash<int, QByteArray> roleNames() const;

private slots:
    void _packetRX(Device*, QByteArray*);

private:
    bool _haveToContinueSwitch(QPair<qint8, qint8>);
    QByteArray _nextSwitch(qint8, qint8);

    void _specifyOnModeChange(char);

    void _toStatus(QByteArray&);
    void _toSwitch(QByteArray&);
    void _toAmplifier(QByteArray&);
    void _toReciever(QByteArray&);
    void _toNLD(QByteArray&);
    void _toFDR(QByteArray&);
    void _toBattery(const QByteArray &);

    QPair<qint8, qint8> m_waitingSwitch;
    int m_waitingReset = SWITCH_PACKETS_TO_RESET;
    int m_currentDevice = -1;
    char m_currentMode = 0x00;
    QVector<QPointF> m_amplifier;
    QQuickView *m_appViewer = nullptr;
    QXYSeries *m_series = nullptr;
    QList<Device*> m_devices;

    bool m_auto = false;
};

#endif // DEVICESMODEL_H
