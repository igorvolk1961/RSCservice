#ifndef IMGBUILDERLINE_H
#define IMGBUILDERLINE_H

#include "svgbuildertype.h"
#include "imgtype.h"


class SVGBuilderLine: public SVGBuilderType{
public:
    SVGBuilderLine(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    virtual void build();
protected:
};

#endif // IMGBUILDERLINE_H
