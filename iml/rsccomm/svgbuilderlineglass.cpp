#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderlineglass.h"
#include "imglineglass.h"

SVGBuilderLineGlass::SVGBuilderLineGlass(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
}

void SVGBuilderLineGlass::build(){
if (DEBUG_MODE > 2) printf("JNI SVGBuilderLineGlass::build begin\n");
    size_t imageSize = sizeof(IMGLINEGLASS);
    unsigned char imageFunc = IMG_LINEGLASS;
    unsigned char imageType = VT_LINE;

if (DEBUG_MODE > 3) printf("JNI SVGBuilderLineGlass::build data->Count=%u\n", data->Count);

    buildSubobjects(imageSize, imageFunc, imageType, parm, 0);

if (DEBUG_MODE > 2) printf("JNI SVGBuilderLineGlass::build end\n");
}

