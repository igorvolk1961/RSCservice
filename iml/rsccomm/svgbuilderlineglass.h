#ifndef IMGBUILDERLINEGLASS_H
#define IMGBUILDERLINEGLASS_H

#include "svgbuildertype.h"
#include "imgtype.h"

class SVGBuilderLineGlass: public SVGBuilderType{
public:
    SVGBuilderLineGlass(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    void build();
protected:
};

#endif // IMGBUILDERLINEGLASS_H
