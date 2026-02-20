#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderdot.h"
#include "imgdot.h"


SVGBuilderDot::SVGBuilderDot(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderLine(parent_node, data, parm, context, mixing){
}

void SVGBuilderDot::build(){
//    printf("JNI SVGBuilderDot::build begin\n");

    size_t imageSize = sizeof(IMGDOT);
    unsigned char imageFunc = IMG_DOT;
    unsigned char imageType = VT_LINE;

    buildSubobjects(imageSize, imageFunc, imageType, parm, 0);

//    printf("JNI SVGBuilderDot::build end\n");
}
