#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuildertype.h"
#include "imgline.h"
#include "imgdot.h"
#include "imgsquare.h"
#include "imgpolygonglass.h"
#include "imglineglass.h"
#include "imghatchsquare.h"
#include "imgsquarevector.h"
#include "imgvectorex.h"
#include "imgvectormix.h"
#include "imgvectortext.h"
#include <math.h>

SVGBuilderType::SVGBuilderType(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilder(parent_node, data, parm, context, mixing){
}

void SVGBuilderType::build(){
}

ImgType* SVGBuilderType::createImgType(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data, unsigned char imageFunc,
                                       int index, const IMGCONTEXT* context){
    if (DEBUG_MODE  >  2) printf("SVGBuilderType::createImgType begin\n");
    double scale_coeff_thick = context->PixelX;
    double scale_coeff_x;
    double scale_coeff_y;
    if (context->inImgVector) {
        scale_coeff_x = context->PixelX;
        scale_coeff_y = context->PixelX;
    } else {
        scale_coeff_x = 1;
        scale_coeff_y = 1;
    }
    VECTORMIX* mixing = getMixing();

    ImgType* imgType;
    switch (imageFunc) {
    case IMG_LINE:
    {
        context->svg_desc->appendImg("IMG_LINE");
        imgType = new ImgLine(desc, hrsc, data,
                              scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                              mixing, context);
        break;
    }
    case IMG_DOT:
    {
        context->svg_desc->appendImg("IMG_DOT");
        imgType = new ImgDot(desc, hrsc, data,
                             scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                             mixing, context);
        break;
    }
    case IMG_SQUARE:
    {
        context->svg_desc->appendImg("IMG_SQUARE");
        imgType = new ImgSquare(desc, hrsc, data,
                                scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                mixing, context);
        break;
    }
    case IMG_POLYGONGLASS:
    {
        context->svg_desc->appendImg("IMG_POLYGONGLASS");
        imgType = new ImgPolygonGlass(desc, hrsc, data,
                                      scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                      mixing, context);
        break;
    }
    case IMG_LINEGLASS:
    {
        context->svg_desc->appendImg("IMG_LINEGLASS");
        imgType = new ImgLineGlass(desc, hrsc, data,
                                   scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                   mixing, context);
        break;
    }
    case IMG_HATCHSQUARE:
    {
        context->svg_desc->appendImg("IMG_HATCHSQUARE");
        imgType = new ImgHatchSquare(desc, hrsc, data,
                                     scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                     mixing, context);
        break;
    }
    case IMG_SQUAREVECTOR:
    {
        context->svg_desc->appendImg("IMG_SQUAREVECTOR");
        imgType = new ImgSquareVector(desc, hrsc, data,
                                      scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                      mixing, context);
        break;
    }
    case IMG_VECTOREX:
    {
        context->svg_desc->appendImg("IMG_VECTOREX");
        imgType = new ImgVectorex(desc, hrsc, data,
                                  scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                  mixing, context);
        break;
    }
    case IMG_VECTORMIX:
    {
        context->svg_desc->appendImg("IMG_VECTORMIX");
        imgType = new ImgVectorMix(desc, hrsc, data,
                                   scale_coeff_x, scale_coeff_y, scale_coeff_thick,
                                   mixing, context);
        break;
    }
    default:
        char* message = new char[255];
        sprintf(message, "UNKNOWN SVGBuilderType::createImgType unknown image %d", imageFunc);
        throw MyException(message);
    }

    updateParm(imgType);

    if (DEBUG_MODE  >  2) printf("SVGBuilderType::createImgType end\n");
    return imgType;
}

void SVGBuilderType::updateParm(ImgType* imgType){
}

void SVGBuilderType::buildSubobjects(size_t imageSize, unsigned char imageFunc, unsigned char imageType, const char* imageParm, int index){
if (DEBUG_MODE  >  2) printf("SVGBuilderType::buildSubobjects begin data->Count=%u\n", data->Count);

        int* polyCounts = data->PolyCounts;
        DRAWPOINT* points = data->Points;
        char** texts = data->Text;
        for (int j = 0; j < data->Count; ++j) {
          int pointCount = polyCounts[j];
          if (imageType == VT_TEXT){
              if (texts){
                buildText(pointCount, points, texts[j], imageParm);
              }
          } else {
if (DEBUG_MODE  >  2) printf("SVGBuilderType::build pointCount=%u\n", pointCount);
            buildSubobject(imageSize, imageFunc, imageType, pointCount, points, imageParm, index);
          }
          points += pointCount;
        }

if (DEBUG_MODE  >  2) printf("SVGBuilderType::buildSubobjects end\n");
}

size_t SVGBuilderType::getDescLength(size_t imageSize, DRAWPOINT* points, int pointCount, int index){
    if (DEBUG_MODE  >  2) printf("SVGBuilderType::getDescLength begin\n");
    if (DEBUG_MODE  >  2) printf("SVGBuilderType::getDescLength imageSize=%u pointCount=%u \n", imageSize, pointCount);
    size_t  result = sizeof(IMGPOLYDESC)
            - sizeof(unsigned int) + imageSize
            + sizeof(IMGVECTPOINT) * (pointCount - 1);
    if (DEBUG_MODE  >  2) printf("SVGBuilderType::getDescLength end\n");
    return result;
}

void SVGBuilderType::buildText(int pointCount, DRAWPOINT* points, char* text,
                               const char* imageParm){
    if (DEBUG_MODE  >  2) printf("SVGBuilderType::buildText begin\n");
    if (!pointCount) {
        if (DEBUG_MODE  >  2) printf("SVGBuilderType::buildText end (no points)\n");
        return;
    }

    if (DEBUG_MODE  >  2) printf("SVGBuilderType::buildText text=%s\n", text);

    size_t textLength = strlen(text) + 1;
    size_t descLength = sizeof(IMGPOLYDESC)
                - sizeof(unsigned int) + sizeof(IMGVECTORTEXT)
                - 32 + textLength
                + sizeof(IMGVECTPOINT) * (pointCount - 1);
    char* pDesc = new char[descLength];
    IMGPOLYDESC* desc = (IMGPOLYDESC*) pDesc;

    size_t imageSize = sizeof(IMGTEXT);
    desc->Length = sizeof(IMGVECTORTEXT)
            - 32 + textLength;
    desc->Type = VT_TEXT;
    desc->Image = IMG_VECTORTEXT;
    char* imageParm1 = (char*)&(desc->Parm);
    memcpy(imageParm1, imageParm, imageSize);
    char* p = imageParm1;
    p += imageSize;
    int* pCode = (int*) p;
    *pCode = 0;
    p += sizeof(int);
    char* title = (char*) p;

    strcpy(title, text);
    p += textLength;
    unsigned int* pCount = (unsigned int*) p;
    *pCount = pointCount;
    p += sizeof(unsigned int);
    IMGVECTPOINT* pPoint = (IMGVECTPOINT*) p;
    for (int j=0; j < pointCount; ++j){
        pPoint[j].Hor = points[j].x;
        pPoint[j].Ver = points[j].y;
        if (DEBUG_MODE>4) printf("SVGBuilderType::buildText j=%u points[j].x=%d points[j].y=%d\n", j, points[j].x, points[j].y);
    }

    HRSC hrsc = context->hrsc;
    double scale_coeff_thick = context->PixelX;
    VECTORMIX* mixing = getMixing();

    context->svg_desc->appendImg("IMG_VECTORTEXT");
    ImgVectorText* imgType = new ImgVectorText(desc, hrsc, data,
          scale_coeff_thick,
          mixing, context, 1);

    QDomElement the_node =  parent_node->appendChild(parent_node->ownerDocument().createElement("g")).toElement();
    char index_s[16];
    sprintf(index_s, "%u", context->svg_desc->getCurrent_id());
    the_node.setAttribute("id", index_s);
    current_node =  &the_node;

    imgType->buildSvgElement(current_node, getImgBorder());
    context->svg_desc->leaveCurrentNode();

    delete imgType;
    delete[] pDesc;

    if (DEBUG_MODE  >  2) printf("SVGBuilderType::buildText end\n");
}

void SVGBuilderType::buildSubobject(size_t imageSize, unsigned char imageFunc, unsigned char imageType,
                                    int pointCount, DRAWPOINT* points, const char* imageParm, int index){
if (DEBUG_MODE  >  2) printf("SVGBuilderType::buildSubobject begin\n");
    if (!pointCount) {
        if (DEBUG_MODE  >  2) printf("SVGBuilderType::buildSubobject end (no points)\n");
        return;
    }

    size_t descLength = getDescLength(imageSize, points, pointCount, index);

    char* pDesc = new char[descLength];
    IMGPOLYDESC* desc = (IMGPOLYDESC*) pDesc;

    desc->Length = imageSize;
    desc->Type = imageType;
    desc->Image = imageFunc;
    char* imageParm1 = (char*)&(desc->Parm);
    memcpy(imageParm1, imageParm, imageSize);
    char* p = imageParm1;
    p += imageSize;
    unsigned int* pCount = (unsigned int*) p;
    *pCount = pointCount;
    p += sizeof(unsigned int);
    IMGVECTPOINT* pPoint = (IMGVECTPOINT*) p;
    for (int j=0; j < pointCount; ++j){
        pPoint[j].Hor = points[j].x;
        pPoint[j].Ver = points[j].y;
    }

    try {
        modifyPoints(pPoint, pCount, imageParm1, index);
    } catch (MyException myException) {
        delete[] pDesc;
        throw MyException(myException);
    } catch (const char* message) {
        delete[] pDesc;
        throw MyException(message);
    } catch (...) {
        delete[] pDesc;
        throw MyException("SVGBuilderType::buildSubobject unknown exception");
    }

    HRSC hrsc = context->hrsc;
    ImgType* imgType;
    try {
      imgType = createImgType(desc, hrsc, data, imageFunc, index, context);
    } catch (MyException myException) {
        delete[] pDesc;
        throw MyException(myException);
    } catch (const char* message) {
        delete[] pDesc;
        throw MyException(message);
    } catch (...) {
        delete[] pDesc;
        throw MyException("SVGBuilderType::buildSubobject unknown exception");
    }

    QDomElement the_node =  parent_node->appendChild(parent_node->ownerDocument().createElement("g")).toElement();
    char index_s[16];
    sprintf(index_s, "%u", context->svg_desc->getCurrent_id());
    the_node.setAttribute("id", index_s);
    current_node =  &the_node;
    try {
       imgType->buildSvgElement(current_node, getImgBorder());
       context->svg_desc->leaveCurrentNode();
    } catch (MyException myException) {
        delete imgType;
        delete[] pDesc;
        throw MyException(myException);
    } catch (const char* message) {
        delete imgType;
        delete[] pDesc;
        throw MyException(message);
    } catch (...) {
        delete imgType;
        delete[] pDesc;
        throw MyException("SVGBuilderType::buildSubobject unknown exception");
    }
    delete imgType;
    delete[] pDesc;

   if (DEBUG_MODE  >  2) printf("SVGBuilderType::buildSubobject end\n");
}

void SVGBuilderType::modifyPoints(IMGVECTPOINT* pPoint, unsigned int* pCount, char* imageParm, int index){
}

void SVGBuilderType::calcDashPoints(unsigned int shift, unsigned int blank, unsigned dash, unsigned int lineCount,
                                    IMGVECTPOINT* points, unsigned int* pCount){
    unsigned int pointCount = *pCount;

    IMGVECTPOINT* qPoint = new IMGVECTPOINT[lineCount];
    double currLen = 0.;
    double remainder = shift * context->PixelX;
    double sectionLength1 = (blank + dash) * context->PixelX;
    double sectionLength = remainder;
    bool first = true;
    unsigned int qPointCount = 0;
    if (DEBUG_MODE  >  2) printf("SVGBuilderType::calcDashPoints pointCount=%u\n", pointCount);
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

        while (currLen < lineLength) {
            qPoint[qPointCount].Hor = x0 + dx * currLen / lineLength;
            qPoint[qPointCount].Ver = y0 + dy * currLen / lineLength;
            ++qPointCount;
            sectionLength = sectionLength1;
            currLen += sectionLength;
        }
        remainder = currLen - lineLength;
        currLen = 0;
    }
    if (DEBUG_MODE  >  2) printf("SVGBuilderType::calcDashPoints qPointCount=%u\n", qPointCount);

    for (int j = 0; j < qPointCount; ++j) {
        points[j].Hor = qPoint[j].Hor;
        points[j].Ver = qPoint[j].Ver;
    }
    delete[] qPoint;
    *pCount = qPointCount;
}


void SVGBuilderType::buildOffsetPoints(IMGVECTPOINT* inPoints, size_t n, double offset,
                     IMGVECTPOINT* prevP, IMGVECTPOINT* lastP) {

//    printf("SVGBuilderType::buildOffsetPoints begin n=%u\n", n);

  IMGVECTPOINT* points = new IMGVECTPOINT[n];

  PointPair* prevOp0 = NULL;
  PointPair* prevOp1 = NULL;
  size_t i = 0;
  for (size_t j = 0; j <= n; j++){
    IMGVECTPOINT* p;
    if (j < n) {
      p = inPoints + j;
    } else
    if (lastP) {
      p = lastP;
    } else {
      break;
    }
    if (!prevP){
      prevP = p;
      continue;
    }
    PointPair* op0 = getOffsetPoints(prevP, p, 0,
                offset , false);
    PointPair* op1 = getOffsetPoints(p, prevP, 0,
                offset, true);
    IMGVECTPOINT* cp;
    if (prevOp0 && prevOp1){
      cp = getCrossPoint(
               &(op0->p1), &(op1->p1),
               &(prevOp0->p1), &(prevOp1->p1));
    } else {
      cp = new IMGVECTPOINT();
      cp->Hor = op0->p1.Hor;
      cp->Ver = op0->p1.Ver;
    }
    if (j > 0){
        points[i].Hor = cp->Hor;
        points[i].Ver = cp->Ver;
        ++i;
    }
    delete cp;
    prevP = p;
    delete prevOp0;
    delete prevOp1;
    prevOp0 = op0;
    prevOp1 = op1;
  }
  if (!lastP && prevOp1){
    points[i].Hor = prevOp1->p1.Hor;
    points[i].Ver = prevOp1->p1.Ver;
  }
  delete prevOp0;
  delete prevOp1;

  for (size_t i = 0; i < n; ++i){
      inPoints[i].Hor = points[i].Hor;
      inPoints[i].Ver = points[i].Ver;
  }
  delete[] points;
//  printf("SVGBuilderType::buildOffsetPoints end\n");
}


PointPair* SVGBuilderType::getOffsetPoints(IMGVECTPOINT* p0, IMGVECTPOINT* p1, double sx, double sy, bool flip){
  double dx = p1->Hor - p0->Hor;
  double dy = p1->Ver - p0->Ver;
  double l = sqrt(dx * dx + dy * dy);
  double cosA = dx / l;
  double sinA = dy / l;
  double x0 = p0->Hor + sx * cosA;
  double y0 = p0->Ver + sx * sinA;
  PointPair* op = new PointPair();
  if (flip){
    op->p1.Hor = (int)(x0 + sy * sinA);
    op->p1.Ver = (int)(y0 - sy * cosA);
    op->p0.Hor = (int)(x0 - sy * sinA);
    op->p0.Ver = (int)(y0 + sy * cosA);
  } else {
    op->p0.Hor = (int)(x0 + sy * sinA);
    op->p0.Ver = (int)(y0 - sy * cosA);
    op->p1.Hor = (int)(x0 - sy * sinA);
    op->p1.Ver = (int)(y0 + sy * cosA);
  }
  return op;
}

double SVGBuilderType::getDistance(IMGVECTPOINT* p0, IMGVECTPOINT* p1){
  double dx = p1->Hor - p0->Hor;
  double dy = p1->Ver - p0->Ver;
  double d = sqrt(dx * dx + dy * dy);
  return d;
}

IMGVECTPOINT* SVGBuilderType::getCrossPoint(IMGVECTPOINT* p00, IMGVECTPOINT* p10, IMGVECTPOINT* p01, IMGVECTPOINT* p11){
  double dx0 = p10->Hor - p00->Hor;
  double dy0 = p10->Ver - p00->Ver;
  double dx1 = p11->Hor - p01->Hor;
  double dy1 = p11->Ver - p01->Ver;
  if (dx0 == 0){
    if (dx1 == 0){
      throw MyException("parallel vertical lines have no crosspoint");
    }
    double d1 = dy1 / dx1;
    IMGVECTPOINT* cp = new IMGVECTPOINT();
    cp->Hor = p00->Hor;
    cp->Ver = (int)(p01->Ver + (cp->Hor - p01->Hor) * d1);
    return cp;
  }
  if (dx1 == 0){
    IMGVECTPOINT* cp = new IMGVECTPOINT();
    cp->Hor = p01->Hor;
    cp->Ver = (int)(p00->Ver + (cp->Hor - p00->Hor) * dx0);
    return cp;
  }
  double d0 = dy0 / dx0;
  double d1 = dy1 / dx1;
  double d = dy0 * dx1 - dy1 * dx0;
  if (d) {
    IMGVECTPOINT* cp = new IMGVECTPOINT();
    cp->Hor  = (int)(((p00->Hor * dy0 * dx1- p01->Hor * dy1 * dx0) +
             (p01->Ver - p00->Ver) * dx0 * dx1) / d);
    cp->Ver = (int)(p00->Ver + (cp->Hor - p00->Hor) * d0);
    return cp;
  } else {
    if ((getDistance(p00, p11) < 2) ||
        (getDistance(p00, p01) < 2)) {
        IMGVECTPOINT* cp = new IMGVECTPOINT();
        cp->Hor = p00->Hor;
        cp->Ver = p00->Ver;
        return cp;
    } else
    if ((getDistance(p10, p11) < 2) ||
        (getDistance(p10, p01) < 2)) {
        IMGVECTPOINT* cp = new IMGVECTPOINT();
        cp->Hor = p10->Hor;
        cp->Ver = p10->Ver;
        return cp;
    } else {
      return NULL;
    }
  }
}
