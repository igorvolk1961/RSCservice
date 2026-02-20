#ifndef IMGBUILDERTEXT_H
#define IMGBUILDERTEXT_H

#include "svgbuildertype.h"
#include "imgtype.h"


class SVGBuilderText: public SVGBuilderType{
public:
    SVGBuilderText(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    virtual void build();
protected:
};

#endif // IMGBUILDERTEXT_H
