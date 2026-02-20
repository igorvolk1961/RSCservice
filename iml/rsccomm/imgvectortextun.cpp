#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "rsc.h"
#include "imgvectortextun.h"
#include "imgtype.h"
#include "mapgdi.h"
#include "imlserv0.h"
#include "imlservex.h"
#include <math.h>

ImgVectorTextUn::ImgVectorTextUn(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
                             VECTORMIX* mixing, const IMGCONTEXT* context, char unicod):ImgVectorText(desc, hrsc, data, dx, dy, posH, posV,
                                              scale_coeff_x, scale_coeff_y, scale_coeff_thick, mixing, context, unicod){
}

ImgVectorTextUn::ImgVectorTextUn(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double scale_coeff_thick,
                             VECTORMIX* mixing, const IMGCONTEXT* context, char unicod):ImgVectorText(desc, hrsc, data, 0., 0., 0., 0., 1.,1.,
                                                                                       scale_coeff_thick, mixing, context, unicod){
}
