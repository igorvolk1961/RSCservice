#ifndef SVGBUILDERVECTOREX_H
#define SVGBUILDERVECTOREX_H

#include "svgbuildervectorabstr.h"
#include "imgtype.h"

class SVGBuilderVectorEx: public SVGBuilderVectorAbstr{
public:
    SVGBuilderVectorEx(
        QDomElement* parent_node,
        const POLYDATAEX * data,
		const char * parm,
        const IMGCONTEXT * context,
        VECTORMIX* mixing);
};

#endif
