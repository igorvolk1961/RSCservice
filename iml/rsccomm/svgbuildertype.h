#ifndef SVGBUILDERTYPE_H
#define SVGBUILDERTYPE_H

#include "svgbuilder.h"
#include "imgtype.h"

class PointPair{
public:
    IMGVECTPOINT p0;
    IMGVECTPOINT p1;
};

class SVGBuilderType: public SVGBuilder{
public:
    SVGBuilderType(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    virtual void build();
protected:
    void buildSubobjects(size_t imageSize, unsigned char imageFunc, unsigned char imageType, const char* imageParm, int index);

    virtual void buildText(int pointCount, DRAWPOINT* points, char* text,
                                   const char* imageParm);
    virtual void buildSubobject(size_t imageSize, unsigned char imageFunc, unsigned char imageType,
                                        int pointCount, DRAWPOINT* points, const char* imageParm, int index);
    virtual size_t getDescLength(size_t imageSize, DRAWPOINT* points, int pointCount, int index);
    virtual void modifyPoints(IMGVECTPOINT* points, unsigned int* pCount, char* imageParm, int index);
    virtual ImgType* createImgType(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data, unsigned char imageFunc,
                                   int index, const IMGCONTEXT* context);
    virtual void updateParm(ImgType* imgType);
protected:
    void calcDashPoints(unsigned int shift, unsigned int blank, unsigned dash, unsigned int lineCount,
                        IMGVECTPOINT* pPoint, unsigned int* pCount);
protected:
    static void buildOffsetPoints(IMGVECTPOINT* inPoints, size_t n, double offset,
                           IMGVECTPOINT* prevP, IMGVECTPOINT* lastP);
    static PointPair* getOffsetPoints(IMGVECTPOINT* p0, IMGVECTPOINT* p1, double sx, double sy, bool flip);
    static double getDistance(IMGVECTPOINT* p0, IMGVECTPOINT* p1);
    static IMGVECTPOINT* getCrossPoint(IMGVECTPOINT* p00, IMGVECTPOINT* p10, IMGVECTPOINT* p01, IMGVECTPOINT* p11);
};

#endif // SVGBUILDERTYPE_H
