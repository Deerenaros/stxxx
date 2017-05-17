//    include/modelproperties.h is part of STx
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

#ifndef MODELPROPERTIES_H
#define MODELPROPERTIES_H

#include <QObject>

#include <QAbstractSeries>
#include <QXYSeries>
#include <QAreaSeries>

QT_CHARTS_USE_NAMESPACE
Q_DECLARE_METATYPE(QAbstractSeries *)
Q_DECLARE_METATYPE(QAbstractAxis *)

#include "modelstore.h"


#define PROPERTY(type, name) \
protected:\
    type m_##name;\
private:\
    Q_PROPERTY(type name MEMBER m_##name NOTIFY name##Changed)


class Model : public ModelStore {
    Q_OBJECT

    PROPERTY(bool, automate)
    PROPERTY(int, current)
    PROPERTY(int, fdr_set)

public:
    struct {
        QAreaSeries *fdr;
        QXYSeries *amplifier;
    } series; ///< pointers to chart's series

    Model(QObject *parent = nullptr) : ModelStore(parent) {}

    QVariant operator[] (const char *n) {
        return this->property(n);
    }

signals:
    void automateChanged();
    void currentChanged();
    void fdr_setChanged();

    void pinsChanged(quint8 a, quint8 b);
    void dateSignal(int hours, int min, int year, int month, int day);
    void amplifierSignal(int count);
    void switcherSignal(int a, int b, double ac, double dc);
    void statusSignal(double charge, bool isCharging);
    void fdrSignal(int what, int a, int b, double len, unsigned lvl);
    void firmwareError(QString error);
    void processed();
    void propertiesChanged();
    void fdrSpectrum(double left, double right, double hi);
};

#undef PROPERTY

#endif // MODELPROPERTIES_H
