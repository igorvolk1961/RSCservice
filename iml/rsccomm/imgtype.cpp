#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imgtype.h"
#include "palette.h"
#include <math.h>

ImgType::ImgType(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                 double dx, double dy, double posH, double posV,
                 double scale_coeff_x, double scale_coeff_y,
                 double scale_coeff_thick,
                 VECTORMIX* mixing, const IMGCONTEXT* context){
    if (DEBUG_MODE > 3) printf("ImgType::ImgType begin\n");

if (DEBUG_MODE > 3) printf("ImgType::ImgType desc->Image=%u desc->Length=%u\n", desc->Image, desc->Length);
    char* p = (char*)&(desc->Parm);
    p += desc->Length;
    pPointCount = (ULONG*) p;
if (DEBUG_MODE > 3) printf("ImgType::ImgType pPointCount=%u\n", *pPointCount);
    if (*pPointCount) {
      p += sizeof(ULONG);
      points = (IMGVECTPOINT*)p;
      if (DEBUG_MODE > 3) printf("ImgType::ImgType points[0].Hor=%d points[0].Ver=%d\n", points[0].Hor, points[0].Ver);
      if (*pPointCount > 1){
if (DEBUG_MODE > 3) printf("ImgType::ImgType points[1].Hor=%d points[1].Ver=%d\n", points[1].Hor, points[1].Ver);
      }
    } else {
       points = NULL;
    }

    this->hrsc = hrsc;
    this->data = data;
    this->parm  = (char*)&desc->Parm;
    this->type = desc->Type;
    this->dx = dx;
    this->dy= dy;
    this->posH = posH;
    this->posV = posV;
    this->scale_coeff_x = scale_coeff_x;
    this->scale_coeff_y = scale_coeff_y;
    this->scale_coeff_thick = scale_coeff_thick * context->Scale;
    this->mixing = mixing;
    this->context = context;

if (DEBUG_MODE > 3) printf("ImgType::ImgType end\n");
}

ImgType::~ImgType(){
}

ULONG ImgType::getFillColor(){
    return 0;
}

void ImgType::buildSvgElement(QDomElement* parent_node, ImgBorder* b){
    if (DEBUG_MODE  >  2) printf("ImgType::buildSvgElement ERROR\n");
}

IMGVECTPOINT* ImgType::getPoints(){
    return points;
}

void ImgType::setParm(char* parm){
    this->parm = parm;
}

ULONG ImgType::getRGB(ULONG color, bool rgb) {
    ULONG flag = color & IMGC_INDEX;
    ULONG indx = color & IMGC_CLEARCOLOR;
//    printf("ImgType::getRGB flag=%u indx=%u\n", flag, indx);
    if (color == IMGC_TRANSPARENT){
//        printf("ImgType::getRGB result=%u\n", 0xffffff);
        return 0xffffff;
    }
    ULONG result;
    if (flag == 0) {
        result = color;
    } else {
        result = getPaletteColor(indx) ;
    }

    if (!rgb) {
        int tmp = (result & 0x00ff00);
        tmp |= (result & 0x0000ff) << 16;
        tmp |= (result & 0xff0000) >> 16;
        result = tmp;
    }
//    printf("ImgType::getRGB result=%u\n", result);
    return result;
}

char* ImgType::makeColorString(ULONG color, bool rgb){
    char* s_color1 = new char[255];
    if (color == (ULONG)-1){
        sprintf(s_color1, "none");
    } else {
      ULONG Color1 = getRGB(color, rgb);
      sprintf(s_color1, "#%06x", Color1);
    }
    return s_color1;
}

void ImgType::makePath(QDomElement* shape_node, ImgBorder* b){
    if (DEBUG_MODE > 2) printf("ImgType::makePath begin\n");
    switch (type) {
        case VT_SQUARE: //0  Площадь
        case VT_LINE: //1  Линия
            //           case VT_FREE:      //12  Цепочка не определена
        {
        if (DEBUG_MODE > 2) printf("ImgType::makePath VT_SQUARE or VT_LINE\n");
            std::string path_value;
            double x = null;
            double y = null;
if (DEBUG_MODE > 3) printf("ImgType::makePath *pPointCount=%u\n", *pPointCount);
            for (ULONG j = 0; j < *pPointCount; ++j) {
                IMGVECTPOINT ap = points[j];
if (DEBUG_MODE > 4) printf("ImgType::makePath ap.Hor=%d ap.Ver=%d\n", ap.Hor, ap.Ver);
                if (x == null) {
                    path_value += "M";
                } else {
                    path_value += " L";
                }
                x = ap.Hor * scale_coeff_x + dx;
                y = ap.Ver * scale_coeff_y + dy;
                b->update(x, y);
                char s_x[32];
                sprintf(s_x, "%0.1f", x);
                char s_y[32];
                sprintf(s_y, "%0.1f", y);
                path_value.append(s_x);
                path_value += " ";
                path_value.append(s_y);
            } //for (int j = 1; j <= pointCount; ++j)
            if (type == VT_SQUARE) {
                path_value += "z";
            }
            shape_node->setAttribute("d", path_value.c_str());
            break;
        }
        case VT_ARC: //66  Цепочка типа дуга
        case VT_PIE: //67  Сектор (площадь)
        case VT_ARC_REVERT: //70  Цепочка типа дуга
        {
        if (DEBUG_MODE > 2) printf("ImgType::makePath some curve\n");
//        printf("JNI ImgType::makePath posH=%0.02f posV=%0.02f\n", posH, posV);
//        printf("JNI ImgType::makePath Hor=%d Ver=%d\n", points[0].Hor, points[0].Ver);
//        printf("JNI ImgType::makePath Hor=%d Ver=%d\n", points[1].Hor, points[1].Ver);
//        printf("JNI ImgType::makePath Hor=%d Ver=%d\n", points[2].Hor, points[2].Ver);
//        printf("JNI ImgType::makePath Hor=%d Ver=%d\n", points[3].Hor, points[3].Ver);

            double xc = (points[0].Hor - posH) * scale_coeff_x + dx;
            double yc = (points[0].Ver - posV) * scale_coeff_y + dy;
            double r =   points[1].Hor * scale_coeff_x;
            double x0 = (points[2].Hor - posH) * scale_coeff_x + dx;
            double y0 = (points[2].Ver - posV) * scale_coeff_y + dy;
            double x2 = (points[3].Hor - posH) * scale_coeff_x + dx;
            double y2 = (points[3].Ver - posV) * scale_coeff_y + dy;
//            printf("JNI ImgType::makePath x0=%0.02f y0=%0.02f\n", x0, y0);
//            printf("JNI ImgType::makePath x2=%0.02f y2=%0.02f\n", x2, y2);
//            printf("JNI ImgType::makePath xc=%0.02f yc=%0.02f\n", xc, yc);

            updateImgBorderBySector(b, xc, yc, r, x2, y2, x0, y0);

            std::string path_value = calcCurve(r, xc, yc, x2, y2, x0, y0, true);
            char s_xc[32];
            sprintf(s_xc, "%0.1f", xc);
            char s_yc[32];
            sprintf(s_yc, "%0.1f", yc);
            if (type == VT_PIE){
                path_value += " L";
                path_value.append(s_xc);
                path_value += " ";
                path_value.append(s_yc);
                path_value.append(" z");
            }
            shape_node->setAttribute("d", path_value.c_str());
            break;
        }
        default:
        {
        } // остальные типы обработаны ранее
    } // switch (oldType)
    if (DEBUG_MODE > 2) printf("ImgType::makePath end\n");
}

void ImgType::makeEllipse(QDomElement* shape_node, ImgBorder* b){
    IMGVECTPOINT p = points[0];
    double ph = p.Hor;
    double pv = p.Ver;
    double cx = (ph - posH) * scale_coeff_x + dx;
    double cy = (pv - posV) * scale_coeff_y + dy;
    double rx;
    double ry;
    if (*pPointCount > 1) {
        rx = points[1].Hor * scale_coeff_x;
        ry = points[1].Ver * scale_coeff_y;
    } else {
        rx = 0.f;
        ry = 0.f;
    }
    b->maxX = fmax(b->maxX, cx + ry);
    b->maxY = fmax(b->maxY, cy + ry);
    b->minX = fmin(b->minX, cx - ry);
    b->minY = fmin(b->minY, cy - ry);
    char s_cx[32];
    sprintf(s_cx, "%0.2f", cx);
    shape_node->setAttribute("cx", s_cx);
    char s_cy[32];
    sprintf(s_cy, "%0.2f", cy);
    shape_node->setAttribute("cy", s_cy);
    if ((type == VT_ROUND) ||
        (type == VT_CIRCLE)) {
        char s_r[32];
        sprintf(s_r, "%0.2f", ry);
        shape_node->setAttribute("r", s_r);
    } else {
        char s_rx[32];
        sprintf(s_rx, "%0.2f", rx);
        shape_node->setAttribute("rx", s_rx);
        char s_ry[32];
        sprintf(s_ry, "%0.2f", ry);
        shape_node->setAttribute("ry", s_ry);
    }
}

std::string ImgType::calcCurve(double r, double xc, double yc, double x0, double y0, double x2, double y2, bool first){
    double r0 = sqrt((x0-xc)*(x0-xc) + (y0-yc)*(y0-yc));
    double r2 = sqrt((x2-xc)*(x2-xc) + (y2-yc)*(y2-yc));
    double dr0 = abs(r0 - r) / r;
    double dr2 = abs(r2 - r) / r;
    if (dr0 > dr2) {
        correctArcCoords(x0, y0, x2, y2, xc, yc, r, r0);
    } else {
        correctArcCoords(x2, y2, x0, y0, xc, yc, r, r2);
    }

    r0 = sqrt((x0-xc)*(x0-xc) + (y0-yc)*(y0-yc));
    r2 = sqrt((x2-xc)*(x2-xc) + (y2-yc)*(y2-yc));
    double cos0 = (x0 - xc) / r0;
    double sin0 = (y0 - yc) / r0;
    double cos2 = (x2 - xc) / r2;
    double sin2 = (y2 - yc) / r2;
    double cos20 = cos2 * cos0 + sin2*sin0;
    double sin20 = sin2 * cos0 - cos2*sin0;

    double start_angle = acos(cos0);
    if (sin0 < 0) {
        start_angle = -start_angle;
    }

    double sweep_angle = acos(cos20);
    if (sin20 < 0) {
        sweep_angle = -sweep_angle;
    }

    std::string result;

    double curve[8];
    double d_sweep_angle = M_PI / 2;
    int N;
    if (abs(sweep_angle) <= d_sweep_angle) {
        N  = 1;
        d_sweep_angle = sweep_angle;
    } else {
        N = (int)ceil(abs(sweep_angle) / d_sweep_angle);
        d_sweep_angle = sweep_angle / N;
    }
    for (int m=0; m<N; ++ m){
        arc_to_bezier(xc, yc, r, r,
                start_angle, d_sweep_angle,
                curve);
        for (int j = 0; j < 8; ++j) {
            if (m == 0) {
                if (j == 0) {
                    if (first) {
                        result += " M ";
                    } else {
                        result +=" L ";
                    }
                } else
                if (j == 2) {
                    result += " C ";
                } else {
                    result += " ";
                }
            } else {
                if (j < 2) {
                    continue;
                } else
                if (j == 2) {
                    result += " C ";
                } else {
                    result += " ";
                }
            }
            char s_c[32];
            sprintf(s_c, "%0.2f", curve[j]);
            result += s_c;
        }
        start_angle += d_sweep_angle;
    }
    return result;
}

void ImgType::arc_to_bezier(double xc, double yc, double rx, double ry,
                            double start_angle, double sweep_angle, double* curve) {
    double x0 = cos(sweep_angle / 2.0);
    double y0 = sin(sweep_angle / 2.0);
    double tx = (1.0 - x0) * 4.0 / 3.0;
    double ty = y0 - tx * x0 / y0;
    double px[4];
    double py[4];
    px[0] = x0;
    py[0] = -y0;
    px[1] = x0 + tx;
    py[1] = -ty;
    px[2] = x0 + tx;
    py[2] = ty;
    px[3] = x0;
    py[3] = y0;

    double sn = sin(start_angle + sweep_angle / 2.0);
    double cs = cos(start_angle + sweep_angle / 2.0);
    for (int i = 0; i < 4; i++) {
        curve[i * 2] = xc + rx * (px[i] * cs - py[i] * sn);
        curve[i * 2 + 1] = yc + ry * (px[i] * sn + py[i] * cs);
    }

}

void ImgType::correctArcCoords(double x0, double y0, double x2, double y2,
                               double xc, double yc, double r, double r0) {
    if (abs(x0-xc) < abs(y0-yc)){
        double y0p = +sqrt(r*r - (x0-xc)*(x0-xc)) + yc;
        double y0m = -sqrt(r*r - (x0-xc)*(x0-xc)) + yc;
        if (abs(y0p-y0) < abs(y0m-y0)){
            y0 = y0p;
        } else {
            y0 = y0m;
        }
    } else {
        double x0p = +sqrt(r*r - (y0-yc)*(y0-yc)) + xc;
        double x0m = -sqrt(r*r - (y0-yc)*(y0-yc)) + xc;
        if (abs(x0p-x0) < abs(x0m-x0)){
            x0 = x0p;
        } else {
            x0 = x0m;
        }
    }
}


void ImgType::updateImgBorderBySector(ImgBorder* b, double xc, double yc, double r,
                                      double x0, double y0, double x1, double y1){
    double dx0 = x0 - xc;
    double dy0 = y0 - yc;
    double dx1 = x1 - xc;
    double dy1 = y1 - yc;
//    if (type == VT_ARC_REVERT) {
//                double t;
//                t = dx0; dx0 = dx1; dx1 = t;
//                t = dy0; dy0 = dy1; dy1 = t;
//    }
    if ((dx0*dx1) < 0){
                if (dx0 >= 0){
                    if (b->maxY < yc + r){
                        b->maxY = yc + r;
                    }
                } else {
                    if (b->minY > yc - r){
                        b->minY = yc - r;
                    }
                }
    }
    if ((dy0*dy1) < 0){
                if (dy0 >= 0){
                    if (b->maxX < xc + r){
                        b->maxX = xc + r;
                    }
                } else {
                    if (b->minX > xc - r){
                        b->minX = xc - r;
                    }
                }
    }
    if (b->minX > x0)
        b->minX = x0;
    if (b->maxX < x0)
        b->maxX = x0;
    if (b->minY > y0)
        b->minY = y0;
    if (b->maxY < y0)
        b->maxY = y0;
    if (b->minX > x1)
        b->minX = x1;
    if (b->maxX < x1)
        b->maxX = x1;
    if (b->minY > y1)
        b->minY = y1;
    if (b->maxY < y1)
        b->maxY = y1;

}
