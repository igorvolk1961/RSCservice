#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuildersquarevector.h"

SVGBuilderSquareVector::SVGBuilderSquareVector(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
}

void SVGBuilderSquareVector::build(){
//    printf("JNI SVGBuilderSquareVector::build begin\n");

    IMGSQUAREVECTOR* imgSquareVector =  (IMGSQUAREVECTOR*)parm;
    QDomElement* current_node = context->svg_desc->getCurrent_node();
    current_node->setAttribute("StepHor", imgSquareVector->StepHor * context->PixelX);
    current_node->setAttribute("StepVer", imgSquareVector->StepVer * context->PixelX);

    size_t imageSize = imgSquareVector->Length;

    unsigned char imageFunc = IMG_SQUAREVECTOR;
    unsigned char imageType = VT_SQUARE;

    buildSubobjects(imageSize, imageFunc, imageType, parm, 0);

//    printf("JNI SVGBuilderSquareVector::build end\n");
}
