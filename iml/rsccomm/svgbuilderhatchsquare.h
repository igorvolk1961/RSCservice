#ifndef SVGBUILDERHATCHSQUARE_H
#define SVGBUILDERHATCHSQUARE_H

#include "svgbuildertype.h"
#include "imgtype.h"

class SVGBuilderHatchSquare: public SVGBuilderType{
public:
    SVGBuilderHatchSquare(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    void build();
protected:
};
#endif // SVGBUILDERHATCHSQUARE_H
