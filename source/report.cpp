//    source/report.cpp is part of STx
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
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#include <utility>
#include <tuple>

#include "report.h"

#include "devicesmodel.h"

constexpr unsigned Report::MAX_FDR_DATASET;

Report::Report(QString file)
    : m_file(file)
    , m_report(file)
{
}

void Report::process(Device &dev, Starting &s) {
    Q_UNUSED(dev);
    Q_UNUSED(s);
}

void Report::process(Device &dev, Battery &b) {
    Q_UNUSED(dev);
    Q_UNUSED(b);
}

void Report::process(Device &dev, Amplifier &a) {
    Q_UNUSED(dev);
    Q_UNUSED(a);
}

void Report::process(Device &dev, Switch &sw) {
    Q_UNUSED(dev);
    Q_UNUSED(sw);
}

void Report::process(Device &dev, Receiver &rx) {
    Q_UNUSED(dev);
    Q_UNUSED(rx);
}

void Report::process(Device &dev, NLD &nld) {
    Q_UNUSED(dev);
    Q_UNUSED(nld);
}

void Report::process(Device &dev, FDR &data) {
    Q_UNUSED(dev);
    Q_UNUSED(data);

    if(!m_report.selectSheet(ReportModel::Sheets::FDR)) {
        m_report.addSheet(ReportModel::Sheets::FDR);
    }

    // m_report.sheet(ReportModel::Sheets::FDR)

    if(data.submode == FDR::OK) {
        quint8 a = data.pins.a, b = data.pins.b, n = data.pins.N - 1;
        // sum of arithmetic progression's members with shift by second pin, doubled
        int col = ( (a-1)*(2*n-(a-2))/2 ) + ( b-a ); col = col*2 - 1;
        QVariantMap fdr = qvariant_cast<QVariantMap>(model->getProperties().value("fdr"));
        qdebug("fdr") << fdr.value("set").toInt();
        int offset = (fdr.value("set").toInt() == 1
                      ? 0
                      : FDR_HEADER_SIZE + MAX_FDR_DATASET);

        //m_report.mergeCells(QXlsx::CellRange(offset + 1, col, offset + 1, col+1));

        QXlsx::Format fmtTitle;
        fmtTitle.setHorizontalAlignment(QXlsx::Format::AlignHCenter);
        m_report.setRowFormat(1, fmtTitle);

        QXlsx::Format fmtLength;
        fmtLength.setNumberFormat("#.#\"m\"");

        m_report.write(offset + 1,   col, QString("%1~%2").arg(a).arg(b));
        m_report.write(offset + 2,   col, "Length");
        m_report.write(offset + 2, col+1, "Level");

        for(size_t i = 1; i < std::min(MAX_FDR_DATASET, unsigned(data.size + 1)); i++) {
            m_report.write(offset + i+FDR_HEADER_SIZE,   col, data.measments[i-1].len/10., fmtLength);
            m_report.write(offset + i+FDR_HEADER_SIZE, col+1, data.measments[i-1].lvl);
        }
    }
}

void Report::process(Device &dev, Flashing &flash) {
    Q_UNUSED(dev);
    Q_UNUSED(flash);
}

void Report::push(Device &dev) {
    Q_UNUSED(dev);
    qdebug("report") << (m_report.save()
                            ? "report saved successfully"
                            : "error during saving report");
}

cvoid Report::handle(const size_t id, cvoid p, Device *dev) {
    Q_UNUSED(p);
    Q_UNUSED(dev);

    quint16 c, r;
    switch(id) {
    case "report"_h:
        if(p == EVENT) {
            push(*dev);
            return NOTHING;
        }

        c = (int(p) | 0xFFFF0000) >> 16;
        r = int(p) | 0x0000FFFF;
        m_lastRq = m_report.read(c, r).toString();
        return reinterpret_cast<cvoid>(&m_lastRq);
    case "report_model"_h:
        m_lastMl = ReportModel(m_report.sheet(QString(p)));
        return reinterpret_cast<cvoid>(&m_lastMl);
    default:
        return NOTHING;
    }
}
