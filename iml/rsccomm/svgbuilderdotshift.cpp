#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderdotshift.h"
//#include "exception.h"
#include <math.h>


SVGBuilderDotShift::SVGBuilderDotShift(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
}

void SVGBuilderDotShift::build(){
//    printf("JNI SVGBuilderDotShift::build begin\n");

    IMGDOTSHIFT* imgDotShift = (IMGDOTSHIFT*) parm;
    QDomElement* current_node = context->svg_desc->getCurrent_node();
    current_node->setAttribute("Shift", imgDotShift->Shift * context->PixelX);

    size_t imageSize = sizeof(IMGDOT);
    unsigned char imageFunc = IMG_DOT;
    unsigned char imageType = VT_LINE;
    const char* imageParm = parm;

    buildSubobjects(imageSize, imageFunc, imageType, imageParm, 0);

//    printf("JNI SVGBuilderDotShift::build end\n");
}


void SVGBuilderDotShift::modifyPoints(IMGVECTPOINT* pPoint, unsigned int* pCount, char* imageParm, int index){

    IMGDOTSHIFT* imgDotShift = (IMGDOTSHIFT*) parm;
    double offset = imgDotShift->Shift * context->PixelX;
    buildOffsetPoints(pPoint, *pCount, offset,
                         NULL, NULL);
}

