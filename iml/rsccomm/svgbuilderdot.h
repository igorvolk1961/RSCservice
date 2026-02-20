#ifndef SVGBUILDERDOT_H
#define SVGBUILDERDOT_H

#include "svgbuilderline.h"
#include "imgtype.h"

class SVGBuilderDot: public SVGBuilderLine{
public:
    SVGBuilderDot(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    void build();
protected:
};

#endif // SVGBUILDERDOT_H
