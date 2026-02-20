#ifndef IMGVECTORTEXTUN_H
#define IMGVECTORTEXTU_H

#include "imgvectortext.h"

class ImgVectorTextUn:public ImgVectorText{
public:
    ImgVectorTextUn(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT* context, char unicod);
    ImgVectorTextUn(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT* context, char unicod);
};

#endif // IMGVECTORTEXTUN_H
