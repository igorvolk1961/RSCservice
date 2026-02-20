#ifndef SVGBUILDERSQUARE_H
#define SVGBUILDERSQUARE_H

#include "svgbuildertype.h"
#include "imgtype.h"

class SVGBuilderSquare: public SVGBuilderType{
public:
    SVGBuilderSquare(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    virtual void build();
protected:
};

#endif // SVGBUILDERSQUARE_H
