#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderdrawmix.h"

SVGBuilderDrawMix::SVGBuilderDrawMix(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderDrawAbstr(parent_node, data, parm, context, mixing){

    imgDraw = ((IMGDRAWMIX*) parm)->Draw;
    this->mixing = &(((IMGDRAWMIX*) parm)->Mixing);
}
