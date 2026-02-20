#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderdrawabstr.h"

SVGBuilderDrawAbstr::SVGBuilderDrawAbstr(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilder(parent_node, data, parm, context, mixing){
}

void SVGBuilderDrawAbstr::build(){
if (DEBUG_MODE  >  2) printf("SVGBuilderDrawAbstr::build begin imgDraw->Count=%u\n", imgDraw->Count);

    unsigned short imageCount = imgDraw->Count;

    char* p = (char*) &(imgDraw->Image);

    for (int j = 0; j < imageCount; ++j){
        IMGDECOR* imgDecor = (IMGDECOR*) p;
        char* parm = p + sizeof(IMGDECOR);
if (DEBUG_MODE  >  2) printf("SVGBuilderDrawAbstr::build begin imgDecor->Number=%u\n", imgDecor->Number);
        buildImage(parm, imgDecor->Number);
        p += imgDecor->Length;
    }

if (DEBUG_MODE  >  2) printf("SVGBuilderDrawAbstr::build end\n");
}

void SVGBuilderDrawAbstr::buildImage(char* parm, unsigned short type){
    SVGBuilder::buildImage(parm, type, data, context, getMixing());
}

