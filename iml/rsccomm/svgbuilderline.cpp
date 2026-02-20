#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderline.h"
#include "imgline.h"

SVGBuilderLine::SVGBuilderLine(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
}

void SVGBuilderLine::build(){
//    printf("JNI SVGBuilderLine::build begin\n");

    size_t imageSize = sizeof(IMGLINE);
    unsigned char imageFunc = IMG_LINE;
    unsigned char imageType = VT_LINE;

    buildSubobjects(imageSize, imageFunc, imageType, parm, 0);

//    printf("JNI SVGBuilderLine::build end\n");
}

