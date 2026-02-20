#ifndef SVGBUILDERDECORATE_H
#define SVGBUILDERDECORATE_H

#include "svgbuildertype.h"
#include "imgtype.h"

class SVGBuilderDecorate: public SVGBuilderType{
public:
    SVGBuilderDecorate(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);
    ~SVGBuilderDecorate();

    virtual void build();
protected:
    void modifyPoints(IMGVECTPOINT* points, unsigned int* pCount, char* imageParm, int index);
    size_t getDescLength(size_t imageSize, DRAWPOINT* points, int pointCount, int index);
    ImgType* createImgType(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data, unsigned char imageFunc,
                           int index, const IMGCONTEXT* context);
private:
   IMGDECOR** decors;
   unsigned int lineCount;
};

#endif // SVGBUILDERDECORATE_H
