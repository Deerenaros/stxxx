//    include/modelstore.h is part of STx
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
#include <QList>
#include <QSqlDatabase>

#include "utils/hash.h"

// Much better but more harder would be
// if this one inherit QAbstractItemModel
class Store {
public:
    virtual QString prefix() = 0;
    virtual QStringList roles() = 0;
    virtual QStringList modes() = 0;
};


// Trying to wrap SQL-related queries
// with simply and static interface
class SQLHeap {
public:
    SQLHeap() {
        m_db = QSqlDatabase::addDatabase("QSQLITE", "heap");
        m_db.setDatabaseName(":memory:");
    }

    void set(int id, QString table, QString value);

private:
    QSqlDatabase m_db;
};

struct Stores {
    class FDRMerge : public Store {
    public:
        static constexpr int BREAK = 6;

        virtual QString prefix() { return "fdr"; }
        virtual QStringList roles() {
            return QStringList{"12", "13", "14", "15", "16", "17", "18", "23", "24", "25", "26", "27", "28",
                               "34", "35", "36", "37", "38", "45", "46", "47", "48", "56", "57", "58", "67", "68", "78"};
        }
        virtual QStringList modes() { return QStringList(); }
    };

    class FDRLeft : public Store {
    public:

        virtual QString prefix() { return "fdrl"; }
        virtual QStringList roles() {
            return QStringList{"12", "13", "14", "15", "16", "17", "18", "23", "24", "25", "26", "27", "28",
                               "34", "35", "36", "37", "38", "45", "46", "47", "48", "56", "57", "58", "67", "68", "78"};
        }
        virtual QStringList modes() { return QStringList{"m", "db"}; }
    };

    class FDRRight : public Store {
    public:

        virtual QString prefix() { return "fdrr"; }
        virtual QStringList roles() {
            return QStringList{"12", "13", "14", "15", "16", "17", "18", "23", "24", "25", "26", "27", "28",
                               "34", "35", "36", "37", "38", "45", "46", "47", "48", "56", "57", "58", "67", "68", "78"};
        }
        virtual QStringList modes() { return QStringList{"m", "db"}; }
    };
};


// For best design where need to divide whole model into
// pieces of attached stores die to impossible set different
// lengths for different roles.
// This class determine QML-related interface in mean
// that here we are storing all data and bind them with
// QML tables and other related views.
class ModelStore : public QAbstractItemModel
{
public:
    ModelStore(QObject *parent=nullptr);

    // QAbstractItemModel's interface
    QModelIndex index(int, int, const QModelIndex&) const;
    QModelIndex parent(const QModelIndex&) const;

    QHash<int, QByteArray> roleNames() const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    QVariant data(const QModelIndex &index, int role) const;
    // end of QAbstractItemModel's interface


    // own interface to get-set data by key... Would be good to think about better way
    QVariant get(QString store, QString key, int i) const;
    void set(QString store, QVariant key, int i, QVariant data);
private:
    QList<Store*> m_stores;
    QHash<int, QByteArray> m_roles;

    SQLHeap m_heap;
};


#endif // STOREMODEL_H
