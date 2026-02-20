#ifndef IMGTYPE_H
#define IMGTYPE_H

#include <string>
#include <climits>
#include "myexception.h"
#include "imlservex.h"
#include <QtXml>
#include "mapgdi.h"
#include "imgborder.h"
#include "debug.h"

#define MIN_LINE_THICK 0.1
#define MM_IN_PIX 0.28
#define MKM_IN_PIX 280.
#define M_IN_PIX 0.000280
#define LINE_THICK_FACTOR 1.
#define TEXT_HEIGHT_FACTOR 1.

#define null INT_MIN
#define ULONG unsigned int

class ImgType {
public:
    ImgType(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick, VECTORMIX* mixing, const IMGCONTEXT * context);
    virtual ~ImgType();
    virtual void buildSvgElement(QDomElement* parent_node, ImgBorder* imgBorder);
    void setParm(char* parm);
    IMGVECTPOINT* getPoints();
protected:
    char* makeColorString(ULONG Color, bool rgb);
    void makePath(QDomElement* shape_node, ImgBorder* imgBorder);
    void makeEllipse(QDomElement* shape_node, ImgBorder* imgBorder);
    ULONG getRGB(ULONG color, bool rgb);
protected:
    virtual ULONG getFillColor();
private:
    std::string calcCurve(double r, double xc, double yc, double x0, double y0, double x2, double y2, bool closed);
    void correctArcCoords(double x0, double y0, double x2, double y2,
                          double xc, double yc, double r, double r0);
    void arc_to_bezier(double xc, double yc, double rx, double ry,
                       double start_angle, double sweep_angle, double* curve);
    void updateImgBorderBySector(ImgBorder* imgBorder, double xc, double yc, double r,
                                 double x0, double y0, double x1, double y1);

protected:
    HRSC hrsc;
    const POLYDATAEX * data;
    double dx;
    double dy;
    double posH;
    double posV;
    double scale_coeff_x;
    double scale_coeff_y;
    double scale_coeff_thick;
    char*  parm;
    unsigned char type;
    ULONG* pPointCount;
    IMGVECTPOINT* points;
    VECTORMIX* mixing;
    const IMGCONTEXT* context;
};

#endif // IMGTYPE_H
