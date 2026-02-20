#ifndef IMGLINEGLASS_H
#define IMGLINEGLASS_H

#include "imgline.h"

class ImgLineGlass:public ImgLine{
public:
    ImgLineGlass(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
    ImgLineGlass(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
protected:
    double getOpacity();
};

#endif // IMGLINEGLASS_H
