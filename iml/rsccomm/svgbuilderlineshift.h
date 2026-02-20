#ifndef SVGBUILDERLINESHIFT_H
#define SVGBUILDERLINESHIFT_H

#include "svgbuildertype.h"
#include "imgtype.h"

class SVGBuilderLineShift: public SVGBuilderType{
public:
    SVGBuilderLineShift(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    virtual void build();
protected:
    void modifyPoints(IMGVECTPOINT* pPoint, unsigned int* pCount, char* imageParm, int index);
    void updateParm(ImgType* imgType);
};

#endif // SVGBUILDERLINESHIFT_H
