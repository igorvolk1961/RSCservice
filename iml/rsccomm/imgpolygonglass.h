#ifndef IMGPOLYGONGLASS_H
#define IMGPOLYGONGLASS_H

#include "imgsquare.h"

class ImgPolygonGlass:public ImgSquare{
public:
    ImgPolygonGlass(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
    ImgPolygonGlass(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
           double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
protected:
    double getOpacity();
};

#endif // IMGPOLYGONGLASS_H
