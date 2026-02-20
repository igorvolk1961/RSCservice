#ifndef IMGSQUARE_H
#define IMGSQUARE_H

#include "imgtype.h"
#include "mapgdi.h"

class ImgSquare:public ImgType{
public:
    ImgSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT* context);
    ImgSquare(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
              double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT* context);
    void buildSvgElement(QDomElement* parent_node, ImgBorder* imgBorder);
protected:
    virtual ULONG getFillColor();
    virtual ULONG getStrokeColor();
    virtual char* getFill();
    virtual char* getStroke();
    virtual double getOpacity();
};
#endif // IMGSQUARE_H
