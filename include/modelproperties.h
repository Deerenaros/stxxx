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

#define PROPERTY(type, name) \
protected:\
    type m_##name;\
private:\
    Q_PROPERTY(type name MEMBER m_##name NOTIFY name##Changed)


class ModelProperties : public QObject {
    Q_OBJECT

    PROPERTY(bool, automate)
    PROPERTY(int, current)
    PROPERTY(int, fdr_set)

public:
    ModelProperties(QObject *parent = nullptr) : QObject(parent) {}

signals:
    void automateChanged();
    void currentChanged();
    void fdr_setChanged();
};

#undef PROPERTY

#endif // MODELPROPERTIES_H
