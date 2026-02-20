#ifndef SVGBUILDERDASH_H
#define SVGBUILDERDASH_H

#include "svgbuildertype.h"
#include "imgtype.h"
#include "imgsection.h"

class SVGBuilderDash: public SVGBuilderType{
public:
    SVGBuilderDash(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);
    ~SVGBuilderDash();

    void build();
protected:
    void modifyPoints(IMGVECTPOINT* points, unsigned int* pCount, char* imageParm, int index);
    size_t getDescLength(size_t imageSize, DRAWPOINT* points, int pointCount, int index);
    ImgType* createImgType(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data, unsigned char imageFunc,
                           int index, const IMGCONTEXT* context);
private:
    ImgSection** imgSections;
    unsigned int dashSum;
    unsigned int lineCount;

};

#endif // SVGBUILDERDASH_H
