#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imgsquarevector.h"
#include "imgvectorex.h"

ImgSquareVector::ImgSquareVector(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                 double dx, double dy, double posH, double posV,
                 double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                                 VECTORMIX* mixing, const IMGCONTEXT* context):ImgPatternSquare(desc, hrsc, data, dx, dy, posH, posV,
                                                     scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                                                    mixing, context){
}
ImgSquareVector::ImgSquareVector(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                                 double scale_coeff_x, double scale_coeff_y, double scale_coeff_thick,
                                 VECTORMIX* mixing, const IMGCONTEXT* context):ImgPatternSquare(desc, hrsc, data,0.,0.,0.,0.,
                                                                                                    scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                                                                                    mixing, context){
}

void ImgSquareVector::buildPattern(const char* patternId, QDomElement &pattern_node){
        IMGSQUAREVECTOR* imgSquareVector = (IMGSQUAREVECTOR*) parm;

        IMGVECTOREX* imgVectorex = (IMGVECTOREX*) &(imgSquareVector->Mark);

        size_t imageSize = imgVectorex->Length;
        size_t descLength = sizeof(IMGPOLYDESC)
                    - sizeof(unsigned int) + imageSize;
        char* pDesc = new char[descLength];
        IMGPOLYDESC* desc = (IMGPOLYDESC*) pDesc;

        desc->Length = imageSize;
        desc->Type = VT_LINE;
        desc->Image = IMG_VECTOREX;
        char* p = (char*)&(desc->Parm);
        memcpy(p, imgVectorex, imageSize);
        p += imageSize;
        int* pCount = (int*)p;
        *pCount = 1;
        p += sizeof(unsigned int);
        IMGVECTPOINT* point = (IMGVECTPOINT*)p;
        point->Hor = 0;
        point->Ver = 0;

        ImgType* imgType = new ImgVectorex(desc, hrsc, data,
                                  1., 1., scale_coeff_thick,
                                  mixing, context);

        ImgBorder* imgBorder = context->imgBorder;
        double old_minX = imgBorder->minX;
        double old_minY = imgBorder->minY;
        double old_maxX = imgBorder->maxX;
        double old_maxY = imgBorder->maxY;
        double old_maxThick = imgBorder->maxThick;
        imgBorder->init();

        imgType->buildSvgElement(&pattern_node, imgBorder);

        imgBorder->minX = old_minX;
        imgBorder->minY = old_minY;
        imgBorder->maxX = old_maxX;
        imgBorder->maxY = old_maxY;
        imgBorder->maxThick = old_maxThick;

        delete imgType;
        delete[] pDesc;

        double width = imgSquareVector->StepHor * scale_coeff_thick;
        char width_s[255];
        sprintf(width_s, "%0.2f", width);
        pattern_node.setAttribute("width", width_s);

        double height = imgSquareVector->StepVer * scale_coeff_thick;
        char height_s[255];
        sprintf(height_s, "%0.2f", height);
        pattern_node.setAttribute("height", height_s);

//        QDomElement curentNode = context->svg_desc->getCurrent_node();
//        curentNode.setAttribute("StepHor", width);
//        curentNode.setAttribute("StepVer", height);

}

