#ifndef SXFINFO_H
#define SXFINFO_H

#include <QString>
#include <QByteArray>
#include "sxfinforecord.h"

class SxfInfo {
public:
    SxfInfo(const QString &filePath);
    void parsePassport(QByteArray &outputBody);
    void getPassport(HMAP hmap, SxfInfoRecord *sxfInfoRecord, bool transformToPseudoMercator);
private:
    void getRstDescription(HMAP hmap, SxfInfoRecord *sxfInfoRecord);
private:
    QString filePath;
};

#endif // SXFINFO_H
