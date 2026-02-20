#ifndef IMGSECTION_H
#define IMGSECTION_H

#include "imgtype.h"
#include "mapgdi.h"

class ImgSection{
public:
    ImgSection(unsigned int imageFunc, size_t imageSize, const char* imageParm, unsigned int shift, unsigned int dash);
    ~ImgSection();
public:
    unsigned int imageFunc;
    size_t imageSize;
    const char* imageParm;
    unsigned int shift;
    unsigned int dash;
    unsigned int blank;

};

#endif // IMGSECTION_H
