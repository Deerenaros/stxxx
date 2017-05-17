//    source/storemodel.cpp is part of STx
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


#include "modelstore.h"

constexpr const char *ModelStore::ROLENAMES[];

ModelStore::ModelStore(QObject *parent)
    : QAbstractItemModel(parent)
{
    for(auto role = std::begin(ROLENAMES); role != std::end(ROLENAMES); role++) {
        m_roles[hash(*role)]  = QByteArray(*role);
    }
}

QModelIndex ModelStore::index(int r, int c, const QModelIndex&) const {
    return createIndex(r, c);
}

QModelIndex ModelStore::parent(const QModelIndex&) const {
    return QModelIndex();
}

int ModelStore::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 2;
}

int ModelStore::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 4;
}

QVariant ModelStore::data(const QModelIndex &index, int role) const {
    return QString("(%1, %2), %3").arg(index.row()).arg(index.column()).arg(QString(m_roles[role]));
}

QHash<int, QByteArray> ModelStore::roleNames() const {
    return m_roles;
}
