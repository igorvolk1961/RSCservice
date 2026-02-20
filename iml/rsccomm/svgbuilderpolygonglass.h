#ifndef IMGBUILDERPOLYGONGLASS_H
#define IMGBUILDERPOLYGONGLASS_H

#include "svgbuildertype.h"
#include "imgtype.h"

class SVGBuilderPolygonGlass: public SVGBuilderType{
public:
    SVGBuilderPolygonGlass(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    void build();
protected:
};

#endif // IMGBUILDERPOLYGONGLASS_H
