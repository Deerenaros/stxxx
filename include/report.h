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
//    along with Foobar.  If not, see <http://www.gnu.org/licenses/>.

#ifndef REPORT_H
#define REPORT_H

#include <QtXlsx/QtXlsx>

#include "processor.h"
#include "betterdebug.h"

class Report : public Processor, private QXlsx::Document
{
    permanent unsigned MAX_FDR_DATASET = 10;
    permanent unsigned FDR_HEADER_SIZE = 2;

public:
    Report(QString file);

    void process(Device&, Starting&);
    void process(Device&, Battery&);
    void process(Device&, Amplifier&);
    void process(Device&, Switch&);
    void process(Device&, Receiver&);
    void process(Device&, NLD&);
    void process(Device&, FDR&);
    void process(Device&, Flashing&);

protected:
    cvoid value(const size_t, cvoid);

private:
    QString m_file;
    QXlsx::Document m_report;
};

#endif // REPORT_H
