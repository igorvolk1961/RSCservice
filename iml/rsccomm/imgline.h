#ifndef IMGLINE_H
#define IMGLINE_H

#include "imgtype.h"
#include "mapgdi.h"

class ImgLine:public ImgType{
public:
    ImgLine(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT * context);
    ImgLine(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT * context);
    void buildSvgElement(QDomElement* parent_node, ImgBorder* imgBorder);
protected:
    virtual void setDashArray(QDomElement* node);
    virtual double getOpacity();
protected:
    ULONG getStrokeColor();
};

#endif // IMGLINE_H
