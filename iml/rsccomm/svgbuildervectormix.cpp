#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuildervectormix.h"

SVGBuilderVectorMix::SVGBuilderVectorMix(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderVectorAbstr(parent_node, data, parm, context, mixing){

if (DEBUG_MODE  >  2) printf("SVGBuilderVectorMix::SVGBuilderVectorMix begin\n");
    imgvectorex = ((IMGVECTORMIX*)parm)->Vector;
    this->mixing =  &(((IMGVECTORMIX*)parm)->Mixing);
    init();
if (DEBUG_MODE  >  2) printf("SVGBuilderVectorMix::SVGBuilderVectorMix end\n");
}

void SVGBuilderVectorMix::build(){
if (DEBUG_MODE  >  2) printf("SVGBuilderVectorMix::build begin\n");
    SVGBuilderVectorAbstr::build();
if (DEBUG_MODE  >  2) printf("SVGBuilderVectorMix::build end\n");
}
