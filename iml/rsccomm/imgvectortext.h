#ifndef IMGVECTORTEXT_H
#define IMGVECTORTEXT_H

#include "imgtype.h"
#include "mapgdi.h"

class ImgVectorText:public ImgType{
public:
    ImgVectorText(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double dx, double dy, double posH, double posV,
            double scale_coeff_x, double scale_coeff_y,
            double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT* context,
                  char unicode);
    ImgVectorText(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT* context,
                  char unicode);
    void buildSvgElement(QDomElement* parent_node, ImgBorder* imgBorder);
private:
    void makeTextStyle(QDomElement &text_node);
//    void getTextMetric(const char* text, double fontSize, double* textWidth, double* textHeight);
    void getTextMetric(const char* text, double* textWidth, double* textHeight);
protected:
    virtual void getText(char* text, int bufferLen, int semCode, char* title);
    ULONG getFillColor();
protected:
    char unicode;
};

#endif // IMGVECTORTEXT_H
