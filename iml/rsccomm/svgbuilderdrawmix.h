#ifndef SVGBUILDERDRAWMIX_H
#define SVGBUILDERDRAWMIX_H

#include "svgbuilderdrawabstr.h"

class SVGBuilderDrawMix: public SVGBuilderDrawAbstr {
public:
    SVGBuilderDrawMix(
        QDomElement* parent_node,
        const POLYDATAEX * data,
        const char * parm,
        const IMGCONTEXT * context,
            VECTORMIX* mixing);
};


#endif // SVGBUILDERDRAWMIX_H
