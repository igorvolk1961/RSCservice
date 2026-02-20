#ifndef IMGUSE_H
#define IMGUSE_H

#include "imgline.h"

class ImgMarkerLine:public ImgLine{
public:
    static const int MARKER_START = 1;
    static const int MARKER_END = 2;
    static const int MARKER_MID = 4;

    ImgMarkerLine(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
            double scale_coeff_thick,
            VECTORMIX* mixing, const IMGCONTEXT * context);
    ~ImgMarkerLine();
    void init(const char* markerId, unsigned char imageFunc, int markerKind);

    void buildSvgElement(QDomElement* parent_node, ImgBorder* imgBorder);
private:
    IMGPOLYDESC* desc;
    char* markerId;
    unsigned char imageFunc;
    int markerKind;
};

#endif // IMGUSE_H
