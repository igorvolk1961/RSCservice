#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuildervectornode.h"

#include "imgline.h"
#include "imgmarkerline.h"
//#include "exception.h"
#include <math.h>

SVGBuilderVectorNode::SVGBuilderVectorNode(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
    if (DEBUG_MODE  >  2) printf("SVGBuilderVectorNode::SVGBuilderDash begin\n");

    if (DEBUG_MODE  >  2) printf("SVGBuilderVectorNode::SVGBuilderDash end\n");
}

SVGBuilderVectorNode::~SVGBuilderVectorNode(){
}

void SVGBuilderVectorNode::build(){
if (DEBUG_MODE  >  2) printf("SVGBuilderVectorNode::build begin\n");

        unsigned char imageFunc = IMG_VECTOREX;
        unsigned char imageType = VT_LINE;
        IMGVECTOREX* imgVectorex = (IMGVECTOREX*) parm;
        size_t imageSize = imgVectorex->Length;
        const char* imageParm = parm;

        buildSubobjects(imageSize, imageFunc, imageType, imageParm, 0);

        if (DEBUG_MODE  >  2) printf("SVGBuilderVectorNode::build end\n");
}

ImgType* SVGBuilderVectorNode::createImgType(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data, unsigned char imageFunc,
                                             int index, const IMGCONTEXT* context){
    if (DEBUG_MODE  >  2) printf("SVGBuilderVectorNode::createImgType begin\n");
    int marker_id = std::rand();
    char markerId[32];
    sprintf(markerId, "marker_%d", marker_id);

    double scale_coeff_thick = context->PixelX;
        VECTORMIX* mixing = getMixing();

        if (DEBUG_MODE  >  2) printf("SVGBuilderVectorNode::createImgType desc->Image=%u desc->Length=%u\n", desc->Image, desc->Length);
        char* p = (char*)&(desc->Parm);
        p += desc->Length;
        ULONG* pPointCount = (ULONG*) p;
        if (DEBUG_MODE  >  2) printf("SVGBuilderVectorNode::createImgType *pPointCount=%u\n", *pPointCount);

        context->svg_desc->appendImg("IMG_MARKERLINE");
        ImgMarkerLine* imgMarkerLine = new ImgMarkerLine(desc, hrsc, data,
              scale_coeff_thick,
              mixing, context);
        imgMarkerLine->init(markerId, imageFunc,
                            ImgMarkerLine::MARKER_START |
                            ImgMarkerLine::MARKER_END |
                            ImgMarkerLine::MARKER_MID);
        updateParm(imgMarkerLine);

    if (DEBUG_MODE  >  2) printf("SVGBuilderVectorNode::createImgType end\n");
    return imgMarkerLine;
}
