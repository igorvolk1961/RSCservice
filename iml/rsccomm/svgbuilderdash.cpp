#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderdash.h"

#include "imgline.h"
#include "imgmarkerline.h"
#include "imgdot.h"
//#include "exception.h"
#include <math.h>
#include <cstdlib>

SVGBuilderDash::SVGBuilderDash(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){
    if (DEBUG_MODE  >  2) printf("SVGBuilderDash::SVGBuilderDash begin\n");

    IMGDASH* imgDash = (IMGDASH*) parm;
    imgSections = new ImgSection*[imgDash->Count];
    char* p = (char*) &(imgDash->Section);

    dashSum = 0;
    for (int j = 0; j < imgDash->Count; ++j){
        IMGSECTION* section = (IMGSECTION*) p;
        size_t imageSize = section->Length - (sizeof(IMGSECTION) - sizeof(unsigned int));
        const char* imageParm = (char*) section + (sizeof(IMGSECTION) - sizeof(unsigned int));
        unsigned int dash = section->Base; // * scale_coeff_thick;
        ImgSection* imgSection = new ImgSection(section->Number, imageSize, imageParm, dashSum, dash);
        imgSections[j] = imgSection;
        dashSum += dash;
        p += section->Length;
    }

    for (int j = 0; j < imgDash->Count; ++j){
        ImgSection* imgSection = imgSections[j];
        imgSection->blank = dashSum - imgSection->dash;
        if (DEBUG_MODE  >  2) printf("SVGBuilderDash::SVGBuilderDash imgSection->dash=%u imgSection->blank=%u imgSection->imageFunc=%u\n", imgSection->dash, imgSection->blank, imgSection->imageFunc);
    }
    if (DEBUG_MODE  >  2) printf("SVGBuilderDash::SVGBuilderDash end\n");
}

SVGBuilderDash::~SVGBuilderDash(){
    IMGDASH* imgDash = (IMGDASH*) parm;
    for (int j = 0; j < imgDash->Count; ++j){
      delete imgSections[j];
    }
    delete[] imgSections;
}


void SVGBuilderDash::build(){
if (DEBUG_MODE  >  2) printf("SVGBuilderDash::build begin\n");

    IMGDASH* imgDash = (IMGDASH*) parm;
//    QDomElement* current_node = context->svg_desc->getCurrent_node();
//    current_node->setAttribute("Last", imgDash->Last);
//    current_node->setAttribute("Border", imgDash->Border);

    for (int j = 0; j < imgDash->Count; ++j){
      ImgSection* imgSection = imgSections[j];
      unsigned char imageFunc = imgSection->imageFunc;
      if (imageFunc != IMG_EMPTY) {

        unsigned char imageType = VT_LINE;
        size_t imageSize;
        if (imageFunc == IMG_LINE) {
          imageFunc = IMG_DOT;
          imageSize = sizeof(IMGDOT);
          if (DEBUG_MODE  >  2) {
              IMGLINE* imgLine = (IMGLINE*)imgSection->imageParm;
              printf("SVGBuilderDash::build imgLine->Thick=%u\n", imgLine->Thick);
          }
        } else {
          imageSize = imgSection->imageSize;
        }
        const char* imageParm = imgSection->imageParm;

        current_node = context->svg_desc->appendSection();
        current_node->setAttribute("dash", imgSection->dash * context->PixelX);
        current_node->setAttribute("blank", imgSection->blank * context->PixelX);
        current_node->setAttribute("shift", imgSection->shift * context->PixelX);

        buildSubobjects(imageSize, imageFunc, imageType, imageParm, j);

        context->svg_desc->leaveCurrentNode();
      }
    }
if (DEBUG_MODE  >  2) printf("SVGBuilderDash::build end\n");
}


size_t SVGBuilderDash::getDescLength(size_t imageSize, DRAWPOINT* points, int pointCount, int index){
    if (DEBUG_MODE  >  2) printf("SVGBuilderDash::getDescLength begin\n");
    size_t result;
    ImgSection*  imgSection = imgSections[index];
    if (imgSection->imageFunc == IMG_LINE) {
        lineCount = 1;
        result = sizeof(IMGPOLYDESC)
                - sizeof(unsigned int) + sizeof(IMGDOT)
                + sizeof(IMGVECTPOINT) * pointCount;
    } else {
        double lineLenghtSum = 0;
        for (int m=0; m<pointCount-1; ++m) {
            double x0 = points[m].x;
            double y0 = points[m].y;
            double x1 = points[m + 1].x;
            double y1 = points[m + 1].y;
            double dx = x1 - x0;
            double dy = y1 - y0;
            double lineLength = sqrt(dx * dx + dy * dy);
            lineLenghtSum += lineLength;
        }
      lineCount = (int) (lineLenghtSum / (dashSum * context->PixelX )) + 1;
      if (lineCount < pointCount){
          lineCount = pointCount;
      }

      result = sizeof(IMGPOLYDESC)
            - sizeof(unsigned int) + imageSize
            + sizeof(IMGVECTPOINT) * (lineCount + 1);
    }
    if (DEBUG_MODE  >  2) printf("SVGBuilderDash::getDescLength end\n");
    return result;
}

void SVGBuilderDash::modifyPoints(IMGVECTPOINT* pPoint, unsigned int* pCount, char* imageParm, int index){
    if (DEBUG_MODE  >  2) printf("SVGBuilderDash::modifyPoints begin\n");
    unsigned int pointCount = *pCount;
    if (!pointCount) return;

    IMGVECTPOINT* points =  pPoint;

    ImgSection*  imgSection = imgSections[index];
    if (imgSection->imageFunc == IMG_EMPTY) {
        return;
    }

    if (imgSection->imageFunc == IMG_LINE) {
        double currLen = 0.;
        double remainder = imgSection->shift * context->PixelX;
        bool first = true;

        for (int m = 0; m < pointCount - 1; ++m){
            double x0 = points[m].Hor;
            double y0 = points[m].Ver;
            double x1 = points[m + 1].Hor;
            double y1 = points[m + 1].Ver;
            double dx = x1 - x0;
            double dy = y1 - y0;
            double lineLength = sqrt(dx * dx + dy * dy);
            if (first){
              if (remainder < lineLength){
                currLen = remainder;
              } else {
                remainder -= lineLength;
                continue;
              }
            } else {
                first = false;
            }
            pPoint[0].Hor = x0 + dx * currLen / lineLength;
            pPoint[0].Ver = y0 + dy * currLen / lineLength;
            break; //Корректируем только первую точку
         }
    } else {
      calcDashPoints(imgSection->shift, imgSection->blank, imgSection->dash, lineCount, points, pCount);
    }
    if (DEBUG_MODE  >  2) printf("SVGBuilderDash::modifyPoints end\n");
}

ImgType* SVGBuilderDash::createImgType(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data, unsigned char imageFunc,
                                       int index, const IMGCONTEXT* context){
    if (DEBUG_MODE  >  2) printf("SVGBuilderDash::createImgType begin\n");
    ImgSection*  imgSection = imgSections[index];
    double scale_coeff_thick = context->PixelX * context->Scale;
    double scale_coeff_x = context->Scale;
    double scale_coeff_y = context->Scale;
    VECTORMIX* mixing = getMixing();

    ImgType* imgType = NULL;
    switch (imageFunc) {
    case IMG_DOT:
    {
        IMGDOT* imgDot = (IMGDOT*) &(desc->Parm);
        imgDot->Dash = imgSection->dash;
        imgDot->Blank = imgSection->blank;
        desc->Length = sizeof(IMGDOT);
        context->svg_desc->appendImg("IMG_DOT");
        imgType = new ImgDot(desc, hrsc, data,
              scale_coeff_x, scale_coeff_y, scale_coeff_thick,
              mixing, context);
        break;
    }
    case IMG_VECTOREX:
    case IMG_VECTORMIX:
    {
        int marker_id = std::rand();
        char markerId[32];
        sprintf(markerId, "marker_%d", marker_id);

        if (DEBUG_MODE  >  2) printf("SVGBuilderDash::createImgType desc->Image=%u desc->Length=%u\n", desc->Image, desc->Length);
        char* p = (char*)&(desc->Parm);
        p += desc->Length;
        ULONG* pPointCount = (ULONG*) p;
        if (DEBUG_MODE  >  2) printf("SVGBuilderDash::createImgType *pPointCount=%u\n", *pPointCount);

        context->svg_desc->appendImg("IMG_MARKERLINE");
        ImgMarkerLine* imgMarkerLine = new ImgMarkerLine(desc, hrsc, data,
              scale_coeff_thick,
              mixing, context);
        imgMarkerLine->init(markerId, imageFunc,
                            ImgMarkerLine::MARKER_START |
                            ImgMarkerLine::MARKER_END |
                            ImgMarkerLine::MARKER_MID);
        imgType = imgMarkerLine;
        break;
    }
    default:
        char* message = new char[255];
        sprintf(message, "UNKNOWN SVGBuilderType::createImgType unknown image %u", imageFunc);
        throw MyException(message);
    }

    updateParm(imgType);

    if (DEBUG_MODE  >  2) printf("SVGBuilderDash::createImgType end\n");
    return imgType;
}
