#ifndef REPORT_H
#define REPORT_H

#include <QtXlsx/QtXlsx>

#include "processor.h"

class Report : public Processor, private QXlsx::Document
{
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
