#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imgpatternsquare.h"
#include "imgline.h"
#include "imgdot.h"

ImgPatternSquare::ImgPatternSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                 double dx, double dy, double posH, double posV,
                 double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                                 VECTORMIX* mixing, const IMGCONTEXT* context):ImgSquare(desc, hrsc, data, dx, dy, posH, posV,
                                                     scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                                             mixing, context){
}
ImgPatternSquare::ImgPatternSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                                   double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                                 VECTORMIX* mixing, const IMGCONTEXT* context):ImgSquare(desc, hrsc, data,0.,0.,0.,0.,
                                                                                             scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                                             mixing, context){
}

ULONG ImgPatternSquare::getFillColor(){
    return (ULONG)-1;
}

ULONG ImgPatternSquare::getStrokeColor(){
    return (ULONG)-1;
}

ULONG ImgPatternSquare::getPatternStrokeColor(){
    IMGHATCHSQUARE* imgSquare = (IMGHATCHSQUARE*) parm;
    char* p = (char*)imgSquare;
    p += sizeof(IMGHATCHSQUARE);
    IMGLINE* imgLine = (IMGLINE*) p;
    if (mixing && (mixing->Color != -1) &&
            ((getRGB(imgLine->Color, false) != 0xffffff) ||
             ((getRGB(imgLine->Color, false) == 0xffffff) && (mixing->Transparency != 100)))
        ) {
      return mixing->Color;
    } else {
      return imgLine->Color;
    }
}

void ImgPatternSquare::buildPattern(const char* patternId, QDomElement &pattern_node){
}

char* ImgPatternSquare::getFill(){
    char* patternId = "ppp";

    QDomElement defs_node = context->svg_desc->getSvg_node()->firstChildElement("defs");
    if (defs_node.isNull()) {
        defs_node = context->svg_desc->getSvg_node()->appendChild(context->svg_desc->getSvg_node()->ownerDocument().createElement("defs")).toElement();
    }
    QDomElement pattern_node = defs_node.firstChildElement("pattern");

    if (pattern_node.isNull()) {
        pattern_node = defs_node.appendChild(defs_node.ownerDocument().createElement("pattern")).toElement();
        pattern_node.setAttribute("id", patternId);
        pattern_node.setAttribute("patternUnits", "userSpaceOnUse");

        IMGHATCHSQUARE* imgSquare = (IMGHATCHSQUARE*) parm;

//        QDomElement* curentNode = context->svg_desc->getCurrent_node();

        char* patternTransform_s = new char[255];
        sprintf(patternTransform_s, "rotate(%d 0 0)", imgSquare->Angle);
        pattern_node.setAttribute("patternTransform", patternTransform_s);
        delete[] patternTransform_s;

        buildPattern(patternId, pattern_node);
    }

    char* result = new char[255];
    sprintf(result, "url(#%s)", patternId);
    return result;
}

void ImgPatternSquare::setDashArray(QDomElement* node){
    char* p = parm;
    p += sizeof(IMGHATCHSQUARE);
    IMGDOT* imgDot = (IMGDOT*) p;

    if (DEBUG_MODE > 2) printf("ImgPatternSquare::setDashArray imgDot->Color=%u imgDot->Thick=%u imgDot->Dash=%u imgDot->Blank=%u scale_coeff_thick=%0.4f\n", imgDot->Color, imgDot->Thick, imgDot->Dash, imgDot->Blank, scale_coeff_thick);

    double dash_size = imgDot->Dash * scale_coeff_thick;
    double blank_size = imgDot->Blank * scale_coeff_thick;

//    QDomElement curentNode = context->svg_desc->getCurrent_node();
//    curentNode.setAttribute("Dash", dash_size);
//    curentNode.setAttribute("Blank", blank_size);

    char s_dasharray[32];
    sprintf(s_dasharray, "%0.2f,%0.2f", dash_size, blank_size);
    node->setAttribute("stroke-dasharray", s_dasharray);
}

