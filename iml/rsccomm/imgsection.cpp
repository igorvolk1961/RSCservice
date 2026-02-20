#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imgsection.h"

ImgSection::ImgSection(unsigned int imageFunc, size_t imageSize, const char* imageParm, unsigned int shift, unsigned int dash){
    this->imageFunc = imageFunc;
    this->imageSize = imageSize;
    this->imageParm = imageParm;
    this->shift = shift;
    this->dash = dash;
    if (DEBUG_MODE  >  2) printf("ImgSection::ImgSection shift=%u dash=%u imageFunc=%u\n",shift,  dash, imageFunc);
}

ImgSection::~ImgSection(){
}
