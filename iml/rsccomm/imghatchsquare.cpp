#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imghatchsquare.h"
#include "imgline.h"
#include "imgdot.h"

ImgHatchSquare::ImgHatchSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                 double dx, double dy, double posH, double posV,
                 double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                                 VECTORMIX* mixing, const IMGCONTEXT* context):ImgPatternSquare(desc, hrsc, data, dx, dy, posH, posV,
                                                     scale_coeff_x, scale_coeff_y, scale_coeff_thick, mixing, context){
}
ImgHatchSquare::ImgHatchSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                               double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                               VECTORMIX* mixing, const IMGCONTEXT* context):ImgPatternSquare(desc, hrsc, data, 0.,0.,0.,0.,
                                                                                         scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                                         mixing, context){
}

void ImgHatchSquare::buildPattern(const char* patternId, QDomElement &pattern_node){
        IMGHATCHSQUARE* imgSquare = (IMGHATCHSQUARE*) parm;

        char* p = parm;
        p += sizeof(IMGHATCHSQUARE);
        IMGLINE* imgLine = (IMGLINE*)p;

        QDomElement line_node = pattern_node.appendChild(pattern_node.ownerDocument().createElement("line")).toElement();
        line_node.setAttribute("x1", "0");
        line_node.setAttribute("y1", "0");
        line_node.setAttribute("x2", "0");
        line_node.setAttribute("y2", "10");

        char* s_color = makeColorString(getPatternStrokeColor(), false);
        double lineThick = imgLine->Thick * LINE_THICK_FACTOR * scale_coeff_thick;

        char* s_lineThick = new char[255];
        sprintf(s_lineThick, "%0.2f", lineThick);

    //    printf("ImgHatchSquare::buildPattern s_color=%s\n", s_color);
    //    printf("ImgHatchSquare::buildPattern s_lineThick=%s\n", s_lineThick);
        line_node.setAttribute("stroke", s_color);
        line_node.setAttribute("stroke-width",  s_lineThick);

//        curentNode.setAttribute("Thick", lineThick);

        delete[] s_color;
        delete[] s_lineThick;

        if (imgSquare->Number == IMG_DOT){
          setDashArray(&line_node);
        }

        double step = imgSquare->Step * scale_coeff_thick;

        QDomElement* curentNode = context->svg_desc->getCurrent_node();
        curentNode->setAttribute("Step", step);

        char width_s[255];
        sprintf(width_s, "%0.2f", step);
        pattern_node.setAttribute("width", width_s);

        pattern_node.setAttribute("height", "10");
}

