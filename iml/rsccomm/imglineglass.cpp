#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imglineglass.h"

ImgLineGlass::ImgLineGlass(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                 double dx, double dy, double posH, double posV,
                 double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                                 VECTORMIX* mixing, const IMGCONTEXT* context):ImgLine(desc, hrsc, data, dx, dy, posH, posV,
                                                     scale_coeff_x, scale_coeff_y, scale_coeff_thick, mixing, context){
}
ImgLineGlass::ImgLineGlass(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                           double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                           VECTORMIX* mixing, const IMGCONTEXT* context):ImgLine(desc, hrsc, data, 0.,0.,0.,0.,
                                                                                     scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                                     mixing, context){
}

double ImgLineGlass::getOpacity(){
    IMGLINEGLASS* imgLineGlass = (IMGLINEGLASS*) parm;
    int opacity = imgLineGlass->Transparency;

    double result;
    if (mixing && (mixing->Transparency != -1)) {
if (DEBUG_MODE>2) printf("imgLineGlass::getOpacity mixing->Transparency=%d\n", mixing->Transparency);
        result = mixing->Transparency / 100.;  // Похоже, авторы путают Transparency и Opacity
    } else {
if (DEBUG_MODE>2) printf("imgLineGlass::getOpacity 1\n");
      result =  opacity / 100.;
    }
    return result;
}


