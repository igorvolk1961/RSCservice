#ifndef IMGDOT_H
#define IMGDOT_H

#include "imgline.h"

class ImgDot:public ImgLine{
public:
    ImgDot(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
    ImgDot(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
           double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
protected:
    void setDashArray(QDomElement* node);
};

#endif // IMGDOT_H
