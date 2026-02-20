#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderlineshift.h"
//#include "exception.h"
#include <math.h>

SVGBuilderLineShift::SVGBuilderLineShift(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
}

void SVGBuilderLineShift::build(){
//    printf("JNI SVGBuilderLineShift::build begin\n");

    IMGLINESHIFT* imgLineShift = (IMGLINESHIFT*) parm;

    QDomElement* current_node = context->svg_desc->getCurrent_node();
    current_node->setAttribute("Shift", imgLineShift->Shift * context->PixelX);

    size_t imageSize = imgLineShift->Head.Length - sizeof(IMGDECHEAD);
    unsigned char imageFunc = imgLineShift->Head.Number;
    unsigned char imageType = VT_LINE;
    char* imageParm = (char*) &(imgLineShift->Parm);

    buildSubobjects(imageSize, imageFunc, imageType, imageParm, 0);

//    printf("JNI SVGBuilderLineShift::build end\n");
}

void SVGBuilderLineShift::updateParm(ImgType* imgType){
    IMGLINESHIFT* imgLineShift = (IMGLINESHIFT*) parm;
    char* pImgLine = (char*) &(imgLineShift->Parm[0]);
    imgType->setParm(pImgLine);
}

void SVGBuilderLineShift::modifyPoints(IMGVECTPOINT* pPoint, unsigned int* pCount, char* imageParm, int index){

    IMGLINESHIFT* imgLineShift = (IMGLINESHIFT*) parm;
    double offset = imgLineShift->Shift * context->PixelX;
    buildOffsetPoints(pPoint, *pCount, offset,
                         NULL, NULL);
}

