#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderdecorate.h"
#include "imgline.h"
#include "imgdot.h"
#include "imgmarkerline.h"
//#include "exception.h"
#include <math.h>

SVGBuilderDecorate::SVGBuilderDecorate(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilderType(parent_node, data, parm, context, mixing){

    decors = new IMGDECOR*[7];
}

SVGBuilderDecorate::~SVGBuilderDecorate(){
    delete[] decors;
}

void SVGBuilderDecorate::build(){
if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::build begin\n");

    IMGDECORATE* imgDecorate = (IMGDECORATE*) parm;
    char* p = (char*) &(imgDecorate->Base);

    for (int j = 0; j < 7; ++j){
        IMGDECOR* decor = (IMGDECOR*) p;
        decors[j] = decor;
        p += decor->Length;
    }

    for (int j = 0; j < 7; ++j){
      IMGDECOR* decor = decors[j];
      unsigned char imageFunc = decor->Number;
      if (imageFunc != IMG_EMPTY) {
        size_t imageSize = decor->Length - sizeof(IMGDECOR);
        unsigned char imageType = VT_LINE;
        const char* imageParm = (char*) decor + sizeof(IMGDECOR);

        buildSubobjects(imageSize, imageFunc, imageType, imageParm, j);
      }
    }

if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::build end\n");
}

size_t SVGBuilderDecorate::getDescLength(size_t imageSize, DRAWPOINT* points, int pointCount, int index){
    switch (index){
    case 0:{ // base
      return sizeof(IMGPOLYDESC)
            - sizeof(unsigned int) + imageSize
            + sizeof(IMGVECTPOINT) * (pointCount  * 2 - 1);
    }
    case 5: // first
    case 6:{ // second
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
      IMGDECORATE* imgDecorate = (IMGDECORATE*) parm;
      imgDecorate->First.Length;
      lineCount = (int) (lineLenghtSum / ((imgDecorate->First.Length + imgDecorate->Second.Length) * context->PixelX )) + 1;
      if (lineCount < pointCount){
          lineCount = pointCount;
      }

      return sizeof(IMGPOLYDESC)
            - sizeof(unsigned int) + imageSize
            + sizeof(IMGVECTPOINT) * (lineCount + 1);
    }
    default:
        return SVGBuilderType::getDescLength(imageSize, points, pointCount, index);
    }
}

void SVGBuilderDecorate::modifyPoints(IMGVECTPOINT* pPoint, unsigned int* pCount, char* imageParm, int index){
    unsigned int pointCount = *pCount;
    if (!pointCount) return;
    if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::modifyPoints begin\n");

    IMGDECORATE* imgDecorate = (IMGDECORATE*) parm;

    switch (index){
    case 0:{ // base
if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::modifyPoints Base\n");

        IMGDECORHEAD* head = &(imgDecorate->Head);
        if ((head->Left > 0) ||
            (head->Right > 0)){

          IMGVECTPOINT* qPoint = new IMGVECTPOINT[pointCount * 2];
          for (int j=0; j < pointCount - 1; ++j){
            long int p0x = pPoint[j].Hor;
            long int p0y = pPoint[j].Ver;
            long int p1x = pPoint[j + 1].Hor;
            long int p1y = pPoint[j + 1].Ver;
            long int q0x = p0x;
            long int q0y = p0y;
            long int q1x = p1x;
            long int q1y = p1y;
            int gap0;
            if ((head->Begin > 0)) {
                gap0 = head->Begin;
            } else {
                gap0 = 0;
            }
            if (gap0 < head->Left) {
                gap0 = head->Left;
            }
            int gap1;
            if ((j == pointCount - 2) & (head->End > 0)) {
                gap1 = head->End;
            } else {
                gap1 = 0;
            }
            if (gap1 < head->Right) {
                gap1 = head->Right;
            }
            if (gap0 || gap1) {
              double dx = p1x - p0x;
              double dy = p1y - p0y;
              double len = sqrt(dx * dx + dy * dy);
              if (gap0) {
                q0x = (int)(p0x + dx * gap0 / len);
                q0y = (int)(p0y + dy * gap0 / len);
              }
              if (gap1) {
                q0x = (int)(p1x - dx * gap1 / len);
                q0y = (int)(p1y - dy * gap1 / len);
              }
            }
            qPoint[2 * j].Hor = q0x;
            qPoint[2 * j].Ver = q0y;
            qPoint[2 * j + 1].Hor = q1x;
            qPoint[2 * j + 1].Ver = q1y;
          }
          for (int j = 0; j < pointCount; ++j){
              pPoint[2 * j].Hor = qPoint[2 * j].Hor;
              pPoint[2 * j].Ver = qPoint[2 * j].Ver;
              pPoint[2 * j + 1].Hor = qPoint[2 * j + 1].Hor;
              pPoint[2 * j + 1].Ver = qPoint[2 * j + 1].Ver;
          }
          delete[] qPoint;
        } else
        if ((head->Begin > 0) ||
            (head->End > 0)) {
            int gap0 = head->Begin;
            if (gap0) {
              long int p0x = pPoint[0].Hor;
              long int p0y = pPoint[0].Ver;
              long int p1x = pPoint[1].Hor;
              long int p1y = pPoint[1].Ver;
              double dx = p1x - p0x;
              double dy = p1y - p0y;
              double len = sqrt(dx * dx + dy * dy);
              pPoint[0].Hor = (int)(p0x + dx * gap0 / len);
              pPoint[0].Ver = (int)(p0y + dy * gap0 / len);
            }
            int gap1 = head->End;
            if (gap1) {
              long int p0x = pPoint[pointCount - 1].Hor;
              long int p0y = pPoint[pointCount - 1].Ver;
              long int p1x = pPoint[pointCount - 2].Hor;
              long int p1y = pPoint[pointCount - 2].Ver;
              double dx = p1x - p0x;
              double dy = p1y - p0y;
              double len = sqrt(dx * dx + dy * dy);
              pPoint[pointCount - 1].Hor = (int)(p0x + dx * gap0 / len);
              pPoint[pointCount - 1].Ver = (int)(p0y + dy * gap0 / len);
            }

        }
        break;
    }
    case 1:{ // begin
if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::modifyPoints BEGIN\n");
        break;
    }
    case 2:{ // end
if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::modifyPoints END\n");
        break;
    }
    case 3:{ // left
if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::modifyPoints Left\n");
       if (imgDecorate->Left.Length){ // TODO shift point by imgDecorate->Left.Length / 2
       }
       break;
    }
    case 4:{ // right
if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::modifyPoints Right\n");
       if (imgDecorate->Rigth.Length){ // TODO shift point by imgDecorate->Right.Length / 2
       }
       break;
    }
    case 5:{ // first
if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::modifyPoints first\n");
        calcDashPoints(0, imgDecorate->Second.Length, imgDecorate->First.Length, lineCount, pPoint, pCount);
        break;
    }
    case 6:{ // second
if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::modifyPoints second\n");
        calcDashPoints(imgDecorate->First.Length, imgDecorate->First.Length, imgDecorate->Second.Length, lineCount, pPoint, pCount);
        break;
    }
    default:{
        char* message = new char[255];
        sprintf(message, "UNKNOWN SVGBuilderDecorate::modifyPoints unknown decor index %u", index);
        throw MyException(message);
    }
    }
    if (DEBUG_MODE  >  2) printf("SVGBuilderDecorate::modifyPoints end\n");
}

ImgType* SVGBuilderDecorate::createImgType(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data, unsigned char imageFunc,
                                           int index, const IMGCONTEXT* context){
    if (DEBUG_MODE  >  2) printf("SVGBuilderDash::createImgType begin\n");
    double scale_coeff_thick = context->PixelX;
    VECTORMIX* mixing = getMixing();

    ImgType* imgType = NULL;
    switch (index){
    case 0: //base
        imgType = SVGBuilderType::createImgType(desc, hrsc, data, imageFunc, index, context);
        break;
    default:
    {
        int markerKind;
        switch (index){
        case 1: //begin
            markerKind = ImgMarkerLine::MARKER_START;
            break;
        case 2: //end
            markerKind = ImgMarkerLine::MARKER_END;
            break;
        default:
            markerKind = ImgMarkerLine::MARKER_START |
                    ImgMarkerLine::MARKER_END |
                    ImgMarkerLine::MARKER_MID;
        }

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
        imgMarkerLine->init(markerId, imageFunc, markerKind);
        imgType = imgMarkerLine;

        updateParm(imgType);

        break;
    }
    }

    if (DEBUG_MODE  >  2) printf("SVGBuilderDash::createImgType end\n");
    return imgType;
}
