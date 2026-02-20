#ifndef SVGBUILDERDOTSHIFT_H
#define SVGBUILDERDOTSHIFT_H

#include "svgbuildertype.h"
#include "imgtype.h"

class SVGBuilderDotShift: public SVGBuilderType{
public:
    SVGBuilderDotShift(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    virtual void build();
protected:
    void modifyPoints(IMGVECTPOINT* pPoint, unsigned int* pCount, char* imageParm, int index);
};

#endif // SVGBUILDERDOTSHIFT_H
