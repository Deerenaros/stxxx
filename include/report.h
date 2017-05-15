//    include/report.h is part of STx
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

#ifndef REPORT_H
#define REPORT_H

#include <QtXlsx/QtXlsx>
#include <QVariantMap>

#include "processor.h"
#include "betterdebug.h"


class ReportModel : public QAbstractTableModel {
public:
    ReportModel(QXlsx::Document *doc, QObject *parent=nullptr) : QAbstractTableModel(parent), doc(doc) {}

    struct Sheets {
        permanent char FDR[] = "fdr";
    };
protected:
    QXlsx::Document* doc;
};


struct ReportModels {
    class FDR : public ReportModel {
    public:
        FDR(QXlsx::Document *doc, QObject *parent=nullptr) : ReportModel(doc, parent) {}

        enum Role {
            Number=Qt::UserRole,
            Length,
            Level
        };

        QHash<int, QByteArray> roleNames() const;
        int rowCount(const QModelIndex &parent) const;
        int columnCount(const QModelIndex &parent) const;
        QVariant data(const QModelIndex &index, int role) const;
    };
};


class Report : public Processor
{
    permanent unsigned MAX_FDR_DATASET = 10;
    permanent unsigned FDR_HEADER_SIZE = 2;

public:
    Report(QString, QObject *parent=nullptr);

    void process(Device&, Starting&);
    void process(Device&, Battery&);
    void process(Device&, Amplifier&);
    void process(Device&, Switch&);
    void process(Device&, Receiver&);
    void process(Device&, NLD&);
    void process(Device&, FDR&);
    void process(Device&, Flashing&);

    void push(Device&);

protected:
    cvoid handle(const size_t, cvoid, Device*);

private:
    QString m_file;
    QXlsx::Document m_report;
    QString m_lastRq;
    QHash<size_t, ReportModel*> m_models;
};

#endif // REPORT_H
