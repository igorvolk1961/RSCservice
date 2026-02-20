#ifndef HMAPRECORD_H
#define HMAPRECORD_H

#include "maptype.h"
#include <QDateTime>

class HmapRecord{
public:
    HmapRecord(HMAP hmap);
    ~HmapRecord();

    HMAP getHmap();

private:
    HMAP hmap;
    QDateTime lastUseDateTime;
};

#endif // HMAPRECORD_H
