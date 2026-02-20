#ifndef SVGBUILDERSQUAREVECTOR_H
#define SVGBUILDERSQUAREVECTOR_H

#include "svgbuildertype.h"
#include "imgtype.h"

class SVGBuilderSquareVector: public SVGBuilderType{
public:
    SVGBuilderSquareVector(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    void build();
protected:
};
#endif // SVGBUILDERSQUAREVECTOR_H
