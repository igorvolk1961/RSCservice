#ifndef IMGHATCHSQUARE_H
#define IMGHATCHSQUARE_H

#include "imgpatternsquare.h"

class ImgHatchSquare:public ImgPatternSquare{
public:
    ImgHatchSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
    ImgHatchSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                   double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
           VECTORMIX* mixing, const IMGCONTEXT* context);
protected:
   void buildPattern(const char* patternId, QDomElement &pattern_node);
};

#endif // IMGHATCHSQUARE_H

