#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imgvectormix.h"
#include "svgbuildervectormix.h"

ImgVectorMix::ImgVectorMix(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
        double dx, double dy, double posH, double posV,
        double scale_coeff_x, double scale_coeff_y,
        double scale_coeff_thick,
        VECTORMIX* mixing, const IMGCONTEXT* context):ImgType(desc, hrsc, data, dx, dy, posH, posV,
                                                    scale_coeff_x, scale_coeff_y, scale_coeff_thick, mixing, context){

}

ImgVectorMix::ImgVectorMix(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                           double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                           VECTORMIX* mixing, const IMGCONTEXT* context):ImgType(desc, hrsc, data, 0.,0.,0.,0.,
                                                                                     scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                                     mixing, context){
}

void ImgVectorMix::buildSvgElement(QDomElement* parent_node, ImgBorder* imgBorder){
if (DEBUG_MODE > 2) printf("ImgVectorMix:buildSvgElement begin\n");
    POLYDATAEX * data1 = new POLYDATAEX();
    memcpy(data1, data, sizeof(POLYDATAEX));
    data1->Count = 1;
    data1->PolyCounts = (int*)pPointCount;
    data1->Points = (DRAWPOINT*)points;
    SVGBuilderVectorMix* builder = new SVGBuilderVectorMix(
                parent_node, data1, parm, context, mixing);
    builder->build();
    imgBorder->expand(builder->getImgBorder());
    delete data1;
    delete builder;
if (DEBUG_MODE > 2) printf("ImgVectorMix:buildSvgElement end\n");
}
