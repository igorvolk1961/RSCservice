#ifndef IMGVECTOREX_H
#define IMGVECTOREX_H

#include "imgtype.h"
#include "mapgdi.h"

class ImgVectorex:public ImgType{
public:
    ImgVectorex(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT* context);
    ImgVectorex(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT* context);
    void buildSvgElement(QDomElement* parent_node, ImgBorder* imgBorder);
protected:
};
#endif // IMGVECTOREX_H
