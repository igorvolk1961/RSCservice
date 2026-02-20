#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderdraw.h"
#include "mapgdi.h"

SVGBuilderDraw::SVGBuilderDraw(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderDrawAbstr(parent_node, data, parm, context, mixing){

    imgDraw = (IMGDRAW*) parm;
}
