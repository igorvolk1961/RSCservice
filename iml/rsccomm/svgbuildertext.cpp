#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuildertext.h"
#include "imgline.h"

SVGBuilderText::SVGBuilderText(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
}

void SVGBuilderText::build(){
if (DEBUG_MODE > 2) printf("JNI SVGBuilderText::build begin\n");

    size_t imageSize = sizeof(IMGTEXT);
    unsigned char imageFunc = IMG_TEXT;
    unsigned char imageType = VT_TEXT;

    buildSubobjects(imageSize, imageFunc, imageType, parm, 0);

if (DEBUG_MODE > 2) printf("JNI SVGBuilderLine::build end\n");
}

