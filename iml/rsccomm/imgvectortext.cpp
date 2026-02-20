#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "rsc.h"
#include "imgvectortext.h"
#include "imgtype.h"
#include "mapgdi.h"
#include "imlserv0.h"
#include "imlservex.h"
#include "utils.h"
#include "semdef.h"
#include <math.h>
//#ifndef __G_DEPRECATED_THREAD_H__
//#define __G_DEPRECATED_THREAD_H__
//#endif
//#include "pango/pango-layout.h"
//#include "pango/pangoft2.h"
//#include "pango/pango-font.h"

ImgVectorText::ImgVectorText(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
                             VECTORMIX* mixing, const IMGCONTEXT* context,
                             char unicode):ImgType(desc, hrsc, data, dx, dy, posH, posV,
                                              scale_coeff_x, scale_coeff_y, scale_coeff_thick, mixing, context){
    this->unicode = unicode;
}

ImgVectorText::ImgVectorText(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double scale_coeff_thick,
                             VECTORMIX* mixing, const IMGCONTEXT* context,
                             char unicode):ImgType(desc, hrsc, data, 0., 0., 0., 0., 1.,1.,
                                                                                       scale_coeff_thick, mixing, context){
    this->unicode = unicode;
}

void ImgVectorText::getText(char* text, int bufferLen, int semCode, char* title){

    if (semCode == 0 && title[0] == '#') {
        char title1[32];
        memset(title1, 0, 32);
        for (int j = 0; j < 15; ++j){
            char c = title[(j + 1) * 2];
            if (c >= '0' && c <= '9') {
                title1[j] = c;
            } else {
                break;
            }
        }
        semCode = atoi(title1);
//        semCode = _Wtoi(title + 2, 2);
    }

    if (semCode != 0) {
        SEMANTIC* semn = (SEMANTIC *)(data->Semantic);

        RSCSEMANTICEX sem;
        int res = imlGetRscSemanticExByCode(hrsc, &sem, semCode);
        if (!res) {
            return;
        }

        int semType = sem.Type;
        if ((semType == SEMBASECHAR) ||
            (semType == SEMBASESHORT) ||
            (semType == SEMBASELONG) ||
            (semType == SEMBASEDOUBLE) ||
            (semType == SEMBASELONG4)) {
          long classNumber = 0;
          int res = imlGetSemanticLong(semn, semCode,
                                      1, &classNumber);
          if (!res) {
              classNumber = (long) sem.Default;
          }
          if (classNumber) {
            const char* classText = imlGetRscSemanticClassificatorName(hrsc, semCode, classNumber);
            TRsc* rsc = (TRsc*)(void*)context->hrsc;
            #if defined(__linux__)
              int CP_UTF8 = 0; // внутри не используется
            #endif
            rsc->convertTo(classText, text, bufferLen, TRsc::CONV_CP1251, CP_UTF8);
          }
        } else {
            int res = imlGetSemanticString(semn, semCode, 1, text, bufferLen);
            if (!res) {
                return;
            }
        }
    } else {
        if (unicode) {
          strcpy(text, title);
        } else {
            TRsc* rsc = (TRsc*)(void*)context->hrsc;
            #if defined(__linux__)
              int CP_UTF8 = 0; // внутри не используется
              rsc->convertTo(title, text, bufferLen, TRsc::CONV_CP1251, CP_UTF8);
            #else
              rsc->convertTo(title, text, bufferLen, TRsc::CONV_CP1251, 1251);
            #endif
        }
    }
}

void ImgVectorText::buildSvgElement(QDomElement* parent_node, ImgBorder* border){
    if (DEBUG_MODE  >  2) printf("ImgVectorText::buildSvgElement begin\n");
    if (*pPointCount == 0) {
        if (DEBUG_MODE > 2) printf("ImgVectorText::buildSvgElement end (no points)\n");
        return;
    }
    IMGVECTORTEXT* vt = (IMGVECTORTEXT*) parm;

    LONG semCode = vt->Code;
    int bufferLen = 1024;
    char text[bufferLen];
    memset(text, 0, bufferLen);
    getText(text, bufferLen, semCode, vt->Title);

    size_t textSize = strlen(text);
    if ((textSize == 0) ||
        ((textSize == 1) && (text[0] == 0))) {
        return;
    }

    QDomElement text_node = parent_node->appendChild(parent_node->ownerDocument().createElement("text")).toElement();
    text_node.appendChild(text_node.ownerDocument().createTextNode(text));

    if (DEBUG_MODE>1) printf("ImgVectorText::buildSvgElement t->Text.Flag=%u vt->Text.Height=%u vt->Text.Align=%u\n", vt->Text.Flag, vt->Text.Height, vt->Text.Align);
    double fontSize;
    if ((vt->Text.Flag & IMGTEXT_SCALE) == 0){
        fontSize = vt->Text.Height;
    } else {
        fontSize = 1800;
    }
    fontSize *= scale_coeff_thick;
    if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement fontSize=%0.02f scale_coeff_thick=%0.4f\n", fontSize, scale_coeff_thick);

    double textWidth;
    double textHeight;
    getTextMetric(text, &textWidth, &textHeight);
    textWidth *= fontSize;
    textHeight *= fontSize;

    double x0;
    double y0;
    double x1;
    double y1;
    if (*pPointCount == 2) {
      IMGVECTPOINT point0 = points[0];
      x0 = point0.Hor * scale_coeff_x + dx;
      y0 = point0.Ver * scale_coeff_y + dy;
      IMGVECTPOINT point1 = points[1];
      x1 = point1.Hor * scale_coeff_x + dx;
      y1 = point1.Ver * scale_coeff_y + dy;
    } else {
        double minX = INFINITE;
        double minY = INFINITE;
        double maxX = -INFINITE;
        double maxY = -INFINITE;
        for (int j = 0; j < *pPointCount; ++j){
            IMGVECTPOINT point = points[j];
            double x = point.Hor * scale_coeff_x;
            double y = point.Ver * scale_coeff_y;
            if (minX > x) {
                minX = x;
            }
            if (minY > y) {
                minY = y;
            }
            if (maxX < x) {
                maxX = x;
            }
            if (maxY < y) {
                maxY = y;
            }
        }
        x0 = minX;
        x1 = maxX;
        y0 = minY + textHeight;
        y1 = y0;
    }

    std::string class_attr = "IMG_VECTORTEXT SEM_";
    char s_semCode[32];
    sprintf(s_semCode, "%d", semCode);
    class_attr += s_semCode;

    text_node.setAttribute("class", class_attr.c_str());
    double len = sqrt((x1 - x0) * (x1 - x0) + (y1 - y0) * (y1 - y0));
    double cosA = (x1 - x0) / len;
    double sinA = (y1 - y0) / len;

    bool revert = false;
    if (cosA < 0) {  // текст никогда не переворачиваем
        cosA = -cosA;
        double x_tmp = x0;
        double y_tmp = y0;
        x0 = x1;
        y0 = y1;
        x1 = x_tmp;
        y1 = y_tmp;
        revert = true;
    }
    if (fabs(sinA) > 0.001) {
        double angle = acos(cosA) * 180. / M_PI;
        if (y1 < y0) {
            angle = -angle;
        }
        char s_angle[32];
        sprintf(s_angle, "%0.02f", angle);
        text_node.setAttribute("rotate", s_angle);
    } else {
        cosA = 1;
        sinA = 0;
    }
    if (vt->Text.Flag == 1) {
        char s_textWidth[32];
        sprintf(s_textWidth, "%0.02f", len);
        text_node.setAttribute("textLength", s_textWidth);
    }


    if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement textWidth=%u textHeight=%u\n", textWidth, textHeight);

    if ((vt->Text.Flag & IMGTEXT_SCALE) != 0){
        textHeight *= len / textWidth;
        textWidth = len;
    }
    double x_min, y_min, x_max, y_max, x_pos, y_pos;

    unsigned short align = vt->Text.Align;
    // ВАЖНО!!! Проверять условия в указанном порядке
    if ((align & FA_CENTER) == FA_CENTER) {
        text_node.setAttribute("text-anchor", "middle");
        x_pos = 0.5 * (x0 + x1);
        x_min = x_pos - 0.5 * textWidth;
        x_max = x_pos + 0.5 * textWidth;
        if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 5\n");
    } else
    if ((align & FA_RIGHT) == FA_RIGHT) {
        if (!revert) {
            text_node.setAttribute("text-anchor", "end");
            x_pos = x1;
            x_min = x_pos - textWidth;
            x_max = x_pos;
            if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 1\n");
          } else {
            text_node.setAttribute("text-anchor", "start");
            x_pos = x0;
            x_min = x_pos;
            x_max = x_pos + textWidth;
            if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 2\n");
          }
    } else {
//    if ((align & FA_LEFT) == FA_LEFT)
        if (!revert) {
            text_node.setAttribute("text-anchor", "start");
            x_pos = x0;
            x_min = x0;
            x_max = x0 + textWidth;;
            if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 3\n");
        } else {
            text_node.setAttribute("text-anchor", "end");
            x_pos = x1;
            x_min = x1 - textWidth;
            x_max = x1;
            if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 4\n");
        }
    }

     y_pos = y0;
     if ((align & FA_MIDDLE) == FA_MIDDLE) {
         text_node.setAttribute("dominant-baseline", "middle");
         y_min = y_pos - textHeight;
         y_max = y_pos + textHeight;
         if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 11\n");
     } else
     if ((align & FA_BASELINE) == FA_BASELINE) {
         text_node.setAttribute("dominant-baseline", "baseline");
         y_min = y_pos - 1.5 * textHeight;
         y_max = y_pos + 0.5 * textHeight;
         if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 12\n");
     } else
     if ((align & FA_BOTTOM) == FA_BOTTOM) {
         if (!revert) {
           text_node.setAttribute("dominant-baseline", "baseline");
           y_min = y_pos - 1.5 * textHeight;
           y_max = y_pos + 0.5 * textHeight;
           if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 9\n");
         } else {
             text_node.setAttribute("dominant-baseline", "hanging");
             y_min = y0 - 0.5 * textHeight;
             y_max = y0 + 1.5 * textHeight;
             if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 10\n");
         }
     } else {
//    if ((align & FA_TOP) == FA_TOP)
        if (!revert) {
            text_node.setAttribute("dominant-baseline", "hanging");
            y_min = y_pos - 0.5 * textHeight;
            y_max = y_pos + 1.5 * textHeight;
            if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 7\n");
        } else {
            text_node.setAttribute("dominant-baseline", "baseline");
            y_min = y_pos - 1.5 * textHeight;
            y_max = y_pos + 0.5 * textHeight;
            if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement 8\n");
        }
    }

    if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement x_min=%0.2f y_min=%0.2f\n", x_min, y_min);
    if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement x_max=%0.2f y_max=%0.2f\n", x_max, y_max);
    if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement cosA=%0.2f sinA=%0.2f\n", cosA, sinA);

    border->update(x0, y0);
    border->update(x1, y1);
    double x_0 = cosA * x_min - sinA * y_min;
    double y_0 = sinA * x_min + cosA * y_min;
    border->update(x_0, y_0);
    double x_1 = cosA * x_min - sinA * y_max;
    double y_1 = sinA * x_min + cosA * y_max;
    border->update(x_1, y_1);
    double x_2 = cosA * x_max - sinA * y_max;
    double y_2 = sinA * x_max + cosA * y_max;
    border->update(x_2, y_2);
    double x_3 = cosA * x_max - sinA * y_min;
    double y_3 = sinA * x_max + cosA * y_min;
    border->update(x_3, y_3);

    if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement x_0=%0.2f y_0=%0.2f\n", x_0, y_0);
    if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement x_1=%0.2f y_1=%0.2f\n", x_1, y_1);
    if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement x_2=%0.2f y_2=%0.2f\n", x_2, y_2);
    if (DEBUG_MODE>4) printf("ImgVectorText::buildSvgElement x_3=%0.2f y_3=%0.2f\n", x_3, y_3);

    char s_x_pos[32];
    sprintf(s_x_pos, "%0.02f", x_pos);
    text_node.setAttribute("x", s_x_pos);
    char s_y_pos[32];
    sprintf(s_y_pos, "%0.02f", y_pos);
    text_node.setAttribute("y", s_y_pos);

    char s_height[32];
    sprintf(s_height, "%0.02f", fontSize);
    text_node.setAttribute("font-size", s_height);

    //  unsigned char  Horizontal  ;   // Признак горизонтального расположения (0/1)


    makeTextStyle(text_node);

    ULONG imgColor = vt->Text.Color;
    char* s_color = makeColorString(imgColor, false);
    text_node.setAttribute("fill", s_color);
    delete[] s_color;

    if (DEBUG_MODE  >  2) printf("ImgVectorText::buildSvgElement end\n");
}

ULONG ImgVectorText::getFillColor(){
    if (mixing && (mixing->Color != INT_MIN)) {
      return mixing->Color;
    } else {
      IMGVECTORTEXT* vt = (IMGVECTORTEXT*) parm;
      return vt->Text.Color;
    }
}

void ImgVectorText::makeTextStyle(QDomElement &text_node){
    IMGVECTORTEXT* vt = (IMGVECTORTEXT*) parm;

    text_node.setAttribute("font-family", "Arial");

    unsigned short weight = vt->Text.Weight;
    if (weight != 0) { // Вес шрифта, толщина контура (FW_THIN, ...)
        char s_weight[32];
        sprintf(s_weight, "%u", weight);
        text_node.setAttribute("font-weight", s_weight);
    }

    if (vt->Text.Italic != 0) { // Признак наклона символов (0/1)
        text_node.setAttribute("font-style", "italic");
    }
    if (vt->Text.Wide == UNIW_NARROW) {
        text_node.setAttribute("font-stretch", "narrower");
    } else
    if (vt->Text.Wide == UNIW_WIDE) {
        text_node.setAttribute("font-stretch", "wider");
    }
}

double glifWidths[] = {
0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50,
0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50, 0.50,
0.21, 0.23, 0.27, 0.42, 0.42, 0.67, 0.76, 0.14, 0.25, 0.25, 0.29, 0.44, 0.21, 0.25, 0.21, 0.21,
0.42, 0.42, 0.42, 0.42, 0.42, 0.42, 0.42, 0.42, 0.42, 0.42, 0.21, 0.21, 0.44, 0.44, 0.44, 0.42,
0.76, 0.50, 0.50, 0.54, 0.54, 0.50, 0.46, 0.58, 0.54, 0.21, 0.38, 0.50, 0.42, 0.61, 0.54, 0.58,
0.50, 0.58, 0.54, 0.50, 0.45, 0.54, 0.50, 0.75, 0.49, 0.49, 0.46, 0.21, 0.21, 0.21, 0.34, 0.42,
0.25, 0.42, 0.42, 0.38, 0.42, 0.42, 0.21, 0.42, 0.42, 0.17, 0.17, 0.38, 0.17, 0.63, 0.42, 0.42,
0.42, 0.42, 0.25, 0.38, 0.21, 0.42, 0.37, 0.53, 0.36, 0.37, 0.37, 0.25, 0.20, 0.25, 0.44, 0.34,
0.42, 0.38, 0.34, 0.37, 0.62, 0.36, 0.43, 0.39, 0.60, 0.62, 0.47, 0.54, 0.39, 0.38, 0.56, 0.41,
0.50, 0.49, 0.50, 0.41, 0.51, 0.50, 0.69, 0.45, 0.54, 0.54, 0.44, 0.49, 0.61, 0.54, 0.58, 0.54,
0.50, 0.54, 0.45, 0.48, 0.57, 0.49, 0.55, 0.50, 0.69, 0.70, 0.59, 0.66, 0.49, 0.54, 0.76, 0.54,
0.42, 0.43, 0.40, 0.27, 0.44, 0.42, 0.50, 0.34, 0.42, 0.42, 0.33, 0.44, 0.52, 0.41, 0.42, 0.41
};

//void ImgVectorText::getTextMetric(const char* text, double fontSize, double* textWidth, double* textHeight){
//    PangoFontMap* pfm = pango_ft2_font_map_new();
//    PangoContext* pc = pango_font_map_create_context(pfm);
//    PangoLayout* pl = pango_layout_new(pc);
//    PangoFontDescription* pfd;
//    try {
//       pango_layout_set_markup(pl, text, -1);
//       pfd = pango_font_description_from_string("Arial");
//       pango_font_description_set_absolute_size(pfd, fontSize * PANGO_SCALE);
//       pango_layout_set_font_description(pl, pfd);
//       pango_layout_get_pixel_size(pl, &textWidth, &textHeight);
//    } catch (...) {
//        pango_font_description_free(pfd);
//        g_object_unref(pl);
//        g_object_unref(pc);
//        g_object_unref(pfm);
//        throw MyException("ImgVectorText::buildSvgElement unknown exception");
//    }
//    pango_font_description_free(pfd);
//    g_object_unref(pl);
//    g_object_unref(pc);
//    g_object_unref(pfm);
//}

void ImgVectorText::getTextMetric(const char* text, double* textWidth, double* textHeight){
    size_t len = strlen(text);
    const char* c = text;
    double width = 0;
    int j = 0;
    while (j < len) {
        int i ;
        if (*c < 0) {
            ++c;
            ++j;
            i = (*c)  & 0X000000ff;
        } else {
            i = *c;
        }
        double glifWidth = glifWidths[i];
        width += glifWidth;
        ++c;
        ++j;
    }
    *textWidth = width + (len - 1) * 0.14; // межсимвольный промежуток
    *textHeight = 0.84;
}
