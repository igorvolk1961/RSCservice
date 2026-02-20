#ifndef SVGBUILDERVECTORMIX_H
#define SVGBUILDERVECTORMIX_H

#include "svgbuildervectorabstr.h"
#include "imgtype.h"

class SVGBuilderVectorMix: public SVGBuilderVectorAbstr{
public:
    SVGBuilderVectorMix(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);

    virtual void build();
};

#endif // SVGBUILDERVECTORMIX_H
