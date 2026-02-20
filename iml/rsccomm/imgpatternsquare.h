#ifndef IMGPATTERNSQUARE_H
#define IMGPATTERNSQUARE_H

#include "imgsquare.h"

class ImgPatternSquare:public ImgSquare{
public:
    ImgPatternSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
    ImgPatternSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
           double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
protected:
    ULONG getFillColor();
    ULONG getStrokeColor();
    char* getFill();
    ULONG getPatternStrokeColor();
    void setDashArray(QDomElement* node);
protected:
    virtual void buildPattern(const char* patternId, QDomElement &pattern_node);
};

#endif // IMGPATTERNSQUARE_H
