#include "report.h"

Report::Report(QString file)
    : m_file(file)
{
}

void Report::process(Device &dev, Starting &s) {
}

void Report::process(Device &dev, Battery &b) {
}

void Report::process(Device &dev, Amplifier &a) {
}

void Report::process(Device &dev, Switch &sw) {
}

void Report::process(Device &dev, Receiver &rx) {
}

void Report::process(Device &dev, NLD &nld) {
}

void Report::process(Device &dev, FDR &fdr) {
}

void Report::process(Device &dev, Flashing &flash) {
}
