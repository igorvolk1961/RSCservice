#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imgsquare.h"
#include "mapgdi.h"
#include <math.h>

ImgSquare::ImgSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                 double dx, double dy, double posH, double posV,
                 double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                     VECTORMIX* mixing, const IMGCONTEXT* context):ImgType(desc, hrsc, data, dx, dy, posH, posV,
                                                     scale_coeff_x, scale_coeff_y, scale_coeff_thick, mixing, context){
}
ImgSquare::ImgSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                     double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                     VECTORMIX* mixing, const IMGCONTEXT* context):ImgType(desc, hrsc, data, 0.,0.,0.,0.,
                                                                               scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                               mixing, context){
}

ULONG ImgSquare::getFillColor(){
    IMGSQUARE* imgSquare = (IMGSQUARE*) parm;
    if (mixing && (mixing->Color != (unsigned int)-1) &&
            ((getRGB(imgSquare->Color, false) != 0xffffff) ||
             ((getRGB(imgSquare->Color, false) == 0xffffff) && (mixing->Transparency != 100)))
        ) {
      return mixing->Color;
    } else {
      return imgSquare->Color;
    }
}

ULONG ImgSquare::getStrokeColor(){
    return (ULONG)-1;
}


double ImgSquare::getOpacity(){
    IMGSQUARE* imgSquare = (IMGSQUARE*) parm;
    if (mixing && (getRGB(imgSquare->Color, false) == 0xffffff)) {
//        printf("ImgSquare::getOpacity mixing->Transparency=%d\n", mixing->Transparency);
        double result;
        result = mixing->Transparency / 100.;  // Похоже, авторы путают Transparency и Opacity
        return result;
    } else {
//        printf("ImgSquare::getOpacity 1\n");
      return 1.;
    }
}

char* ImgSquare::getFill(){
    char* result = makeColorString(getFillColor(), false);
    return result;
}

char* ImgSquare::getStroke(){
    char* result = makeColorString(getStrokeColor(), false);
    return result;
}

void ImgSquare::buildSvgElement(QDomElement* parent_node, ImgBorder* imgBorder){
    if (DEBUG_MODE > 2) printf("ImgSquare::buildSvgElement begin\n");
    if (*pPointCount == 0) {
        if (DEBUG_MODE > 2) printf("ImgSquare::buildSvgElement end (no points)\n");
        return;
    }
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

    char* s_fill = getFill();
    char* s_stroke = getStroke();

    shape_node.setAttribute("fill", s_fill);
    shape_node.setAttribute("fill-opacity", getOpacity());
    shape_node.setAttribute("stroke", s_stroke);

    delete[] s_fill;
    delete[] s_stroke;
    if (DEBUG_MODE > 2) printf("ImgSquare::buildSvgElement end\n");
}
