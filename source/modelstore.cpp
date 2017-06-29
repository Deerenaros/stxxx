//    source/modelstore.cpp is part of STx
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

#include "utils/betterdebug.h"

ModelStore::ModelStore(QObject *parent)
    : QAbstractItemModel(parent)
{
    m_stores.append(new Stores::FDRMerge());
    m_stores.append(new Stores::FDRLeft());
    m_stores.append(new Stores::FDRRight());
}

QModelIndex ModelStore::index(int r, int c, const QModelIndex&) const {
    return createIndex(r, c);
}

QModelIndex ModelStore::parent(const QModelIndex&) const {
    return QModelIndex();
}

int ModelStore::rowCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 13;
}

int ModelStore::columnCount(const QModelIndex &parent) const {
    Q_UNUSED(parent);
    return 1;
}

QVariant ModelStore::data(const QModelIndex &index, int role) const {
    QStringList keys = QString(m_roles[role]).split(':');
    return get(keys[0], keys[1], index.row());
}

QHash<int, QByteArray> ModelStore::roleNames() const {
    QHash<int, QByteArray> &roles = *const_cast<QHash<int, QByteArray>*>(&m_roles);
    qdebug("model") << "init";

    if(m_roles.empty()) {
        for(Store* s: m_stores) {
            for(auto role: s->roles()) {
                if(s->modes().size()) {
                    for(auto mode: s->modes()) {
                        auto mixed = QString("%1:%2:%3").arg(s->prefix()).arg(role).append(mode);
                        roles[qHash(mixed)] = mixed.toLatin1();
                    }
                } else {
                    auto mixed = QString("%1:%2").arg(s->prefix()).arg(role);
                    roles[qHash(mixed)] = mixed.toLatin1();
                }
            }
        }
    }

    return roles;
}

QVariant ModelStore::get(QString store, QString key, int i) const {
    Q_UNUSED(store);
    Q_UNUSED(key);
    if(i == Stores::FDRMerge::BREAK) {
        return QString("");
    }

    return QString("%1m (%2)").arg(1.5*i, 0, 'f', 1).arg(1.5*i, 0, 'f', 0);
}
