#ifndef SXFINFORECORD_H
#define SXFINFORECORD_H

//#include "mapapi.h"
#include <QDateTime>

class SxfInfoRecord{
public:
    SxfInfoRecord();
    ~SxfInfoRecord();

    MAPREGISTEREX* getMapReg();
    LISTREGISTER* getSheetReg(bool update = true);   
    long int getEPSG();
    void setEPSG(long int epsg);
private:
    MAPREGISTEREX* mapreg;
    LISTREGISTER* sheetreg;
    long int epsg;
    QDateTime lastUseDateTime;
};

#endif // SXFINFORECORD_H
