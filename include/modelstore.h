//    include/storemodel.h is part of STx
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

#ifndef STOREMODEL_H
#define STOREMODEL_H

#include <QAbstractItemModel>
#include "hash.h"

class ModelStore : public QAbstractItemModel
{
    static constexpr const char *ROLENAMES[] = {
        // FDR's Roles
        "fdr12", "fdr13", "fdr14", "fdr15", "fdr16", "fdr17", "fdr18", "fdr23", "fdr24", "fdr25", "fdr26", "fdr27", "fdr28", "fdr34", "fdr35", "fdr36", "fdr37", "fdr38", "fdr45", "fdr46", "fdr47", "fdr48", "fdr56", "fdr57", "fdr58", "fdr67", "fdr68", "fdr78",
    };

public:
    ModelStore(QObject *parent=nullptr);

    QModelIndex index(int, int, const QModelIndex&) const;
    QModelIndex parent(const QModelIndex&) const;

    enum Role {
        Number=Qt::UserRole+1,
        Length,
        Level
    };

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;

private:
    QHash<int, QByteArray> m_roles;
};

#endif // STOREMODEL_H
