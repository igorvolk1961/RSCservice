#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imgdot.h"

ImgDot::ImgDot(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
               double dx, double dy, double posH, double posV,
               double scale_coeff_x, double scale_coeff_y,
               double scale_coeff_thick,
               VECTORMIX* mixing, const IMGCONTEXT* context):ImgLine(desc, hrsc, data, dx, dy, posH, posV,
                                                     scale_coeff_x, scale_coeff_y,
                                                     scale_coeff_thick, mixing, context){
}

ImgDot::ImgDot(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
               double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
               VECTORMIX* mixing, const IMGCONTEXT* context):ImgLine(desc, hrsc, data, 0.,0.,0.,0.,
                                                                         scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                         mixing, context){
    IMGDOT* imgDot = (IMGDOT*) parm;
    if (DEBUG_MODE > 2) printf("ImgDot::ImgDot imgDot->Color=%u imgDot->Thick=%u imgDot->Dash=%u imgDot->Blank=%u\n", imgDot->Color, imgDot->Thick, imgDot->Dash, imgDot->Blank);
}

void ImgDot::setDashArray(QDomElement* node){
    IMGDOT* imgDot = (IMGDOT*) parm;

    if (DEBUG_MODE > 2) printf("ImgDot::setDashArray imgDot->Color=%u imgDot->Thick=%u imgDot->Dash=%u imgDot->Blank=%u scale_coeff_thick=%0.4f\n", imgDot->Color, imgDot->Thick, imgDot->Dash, imgDot->Blank, scale_coeff_thick);


    double dash_size = imgDot->Dash * scale_coeff_thick;
    double blank_size = imgDot->Blank * scale_coeff_thick;
//    QDomElement curentNode = context->svg_desc->getCurrent_node();
//    curentNode.setAttribute("Dash", dash_size);
//    curentNode.setAttribute("Blank", blank_size);

    char s_dasharray[32];
    sprintf(s_dasharray, "%0.2f,%0.2f", dash_size, blank_size);
    node->setAttribute("stroke-dasharray", s_dasharray);
}
