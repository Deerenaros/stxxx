#ifndef DEVICESMODEL_H
#define DEVICESMODEL_H

#include <QVariant>
#include <QVariantList>
#include <QAbstractItemModel>
#include <QMap>
#include <QPair>

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

    static const int COLUMN_COUNT = 1;
    static const int AMPLIFIER_BYTE_SHIFT = 110;
    static const int SWITCH_MODE = 3;
    const double BATTERY_SCALE = 100./5;

    const QMap<uint16_t, QString> SUPPORTED_PIDS = {
        { 0x6015, "ST300" }
    };

    Q_PROPERTY(int current READ getCurrent WRITE setCurrent NOTIFY currentChanged)
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
    int getCount() const;

Q_SIGNALS:
signals:
    void amplifierSignal(int count);
    void switcherSignal(int input, int output, double ac, double dc);
    void statusSignal(double charge, bool isCharging);

public slots:
    void setCurrent(int);
    void setModeForCurrent(char);
    void specifyModeForCurrent();
    void setSeries(QAbstractSeries*);

protected:
    QHash<int, QByteArray> roleNames() const;

private slots:
    void _packetRX(Device*, QByteArray*);

private:
    QByteArray _nextSwitch(qint8 in, qint8 out);

    void _toStatus(QByteArray&);
    void _toSwitch(QByteArray&);
    void _toAmplifier(QByteArray&);
    void _toReciever(QByteArray&);
    void _toNLD(QByteArray&);
    void _toFDR(QByteArray&);
    void _toBattery(const QByteArray &);

    QPair<quint8, quint8> m_waitingSwitch;
    int m_currentDevice = -1;
    char m_currentMode = 0x00;
    QVector<QPointF> m_amplifier;
    QQuickView *m_appViewer = nullptr;
    QXYSeries *m_series = nullptr;
    QList<Device*> m_devices;

signals:
    int currentChanged();
    void countChanged();
};

#endif // DEVICESMODEL_H
