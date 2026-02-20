#ifndef IMGSQUAREVECTOR_H
#define IMGSQUAREVECTOR_H

#include "imgpatternsquare.h"

class ImgSquareVector:public ImgPatternSquare{
public:
    ImgSquareVector(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
    ImgSquareVector(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                    double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
protected:
    void buildPattern(const char* patternId, QDomElement &pattern_node);
};

#endif // IMGSQUAREVECTOR_H
