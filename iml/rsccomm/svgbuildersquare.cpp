#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuildersquare.h"
#include "imgsquare.h"

SVGBuilderSquare::SVGBuilderSquare(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
}

void SVGBuilderSquare::build(){
//    printf("JNI SVGBuilderSquare::build begin\n");

    size_t imageSize = sizeof(IMGSQUARE);
    unsigned char imageFunc = IMG_SQUARE;
    unsigned char imageType = VT_SQUARE;

//    printf("JNI SVGBuilderSquare::build data->Count=%u\n", data->Count);

    buildSubobjects(imageSize, imageFunc, imageType, parm, 0);

//    printf("JNI SVGBuilderSquare::build end\n");
}

