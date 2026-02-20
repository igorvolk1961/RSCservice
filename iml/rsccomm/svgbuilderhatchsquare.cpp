#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderhatchsquare.h"

SVGBuilderHatchSquare::SVGBuilderHatchSquare(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
}

void SVGBuilderHatchSquare::build(){
//    printf("JNI SVGBuilderHatchSquare::build begin\n");

    IMGHATCHSQUARE* imgHatchSquare =  (IMGHATCHSQUARE*)parm;
    QDomElement* current_node = context->svg_desc->getCurrent_node();
    current_node->setAttribute("Step", imgHatchSquare->Step * context->PixelX);

    size_t imageSize = imgHatchSquare->Length;

    unsigned char imageFunc = IMG_HATCHSQUARE;
    unsigned char imageType = VT_SQUARE;

    buildSubobjects(imageSize, imageFunc, imageType, parm, 0);

//    printf("JNI SVGBuilderHatchSquare::build end\n");
}
