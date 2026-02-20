#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imgline.h"
#include "mapgdi.h"
#include <math.h>

ImgLine::ImgLine(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                 double dx, double dy, double posH, double posV,
                 double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                 VECTORMIX* mixing, const IMGCONTEXT* context):ImgType(desc, hrsc, data, dx, dy, posH, posV,
                                                     scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                           mixing, context){
    if (DEBUG_MODE > 4) printf("ImgLine::ImgLine begin/end\n");
}

ImgLine::ImgLine(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                 double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                 VECTORMIX* mixing, const IMGCONTEXT* context):ImgType(desc, hrsc, data, 0.,0.,0.,0.,
                                                                           scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                           mixing, context){
    if (DEBUG_MODE > 4) printf("ImgLine::ImgLine_1 begin/end\n");
}

ULONG ImgLine::getStrokeColor(){
    IMGLINE* imgLine = (IMGLINE*) parm;
    if (mixing && (mixing->Color != -1)) {
        return mixing->Color;
    } else {
        return imgLine->Color;
    }
}

void ImgLine::setDashArray(QDomElement* node){
}

double ImgLine::getOpacity(){
    return 1;
}

void ImgLine::buildSvgElement(QDomElement* parent_node, ImgBorder* imgBorder){
if (DEBUG_MODE > 2) printf("ImgLine::buildSvgElement begin\n");
   if (*pPointCount == 0) {
       if (DEBUG_MODE > 2) printf("ImgLine::buildSvgElement end (no points)\n");
       return;
   }

    IMGLINE* imgLine = (IMGLINE*) parm;
    unsigned int thick = imgLine->Thick;    
    if (DEBUG_MODE > 2) printf("ImgLine::buildSvgElement thick=%u\n", thick);

    QDomElement shape_node;
    if ((type == VT_ROUND) ||
        (type == VT_CIRCLE) ||
        (type == VT_ELLIPSE) ||
        (type == VT_OVAL)) {
        if ((type == VT_ROUND) ||
            (type == VT_CIRCLE)) {
            shape_node =  parent_node->appendChild(parent_node->ownerDocument().createElement("circle")).toElement();
        } else {
            shape_node = parent_node->appendChild(parent_node->ownerDocument().createElement("ellipse")).toElement();
        }
        makeEllipse(&shape_node, imgBorder);
    } else {
        shape_node = parent_node->appendChild(parent_node->ownerDocument().createElement("path")).toElement();
        makePath(&shape_node, imgBorder);
    }
    setDashArray(&shape_node);

    char* s_color = makeColorString(getStrokeColor(), false);    

    double lineThick = thick * LINE_THICK_FACTOR * scale_coeff_thick;
    imgBorder->maxThick = fmax(imgBorder->maxThick, lineThick);

    double opacity = getOpacity();
    char s_opacity[256];
    sprintf(s_opacity, "%0.2f", opacity);

    char s_lineThick[255];
    sprintf(s_lineThick, "%0.2f", lineThick);    

//    QDomElement* curentNode = context->svg_desc->getCurrent_node();
//    curentNode->setAttribute("Thick", lineThick);

    shape_node.setAttribute("fill", "none");
    shape_node.setAttribute("fill-opacity", 0.);
    shape_node.setAttribute("stroke-width", s_lineThick);
    shape_node.setAttribute("stroke-opacity", s_opacity);
    shape_node.setAttribute("stroke", s_color);

    delete[] s_color;

if (DEBUG_MODE > 2) printf("ImgLine::buildSvgElement end\n");
}
