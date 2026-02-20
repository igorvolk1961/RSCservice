#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderpolygonglass.h"
#include "imgpolygonglass.h"

SVGBuilderPolygonGlass::SVGBuilderPolygonGlass(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
}

void SVGBuilderPolygonGlass::build(){
if (DEBUG_MODE > 2) printf("JNI SVGBuilderPolygonGlass::build begin\n");

    size_t imageSize = sizeof(IMGPOLYGONGLASS);
    unsigned char imageFunc = IMG_POLYGONGLASS;
    unsigned char imageType = VT_SQUARE;

if (DEBUG_MODE > 3) printf("JNI SVGBuilderPolygonGlass::build data->Count=%u\n", data->Count);

    buildSubobjects(imageSize, imageFunc, imageType, parm, 0);

if (DEBUG_MODE > 2) printf("JNI SVGBuilderPolygonGlass::build end\n");
}

