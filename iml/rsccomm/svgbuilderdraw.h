#ifndef SVGBUILDERDRAW_H
#define SVGBUILDERDRAW_H

#include "svgbuilderdrawabstr.h"

class SVGBuilderDraw: public SVGBuilderDrawAbstr {
public:
    SVGBuilderDraw(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);
};

#endif // SVGBUILDERDRAW_H
