#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuildervectorabstr.h"
#include <math.h>

#include "imgtype.h"
#include "imgline.h"
#include "imgdot.h"
#include "imgsquare.h"
#include "imgvectortext.h"
#include "imgvectortextun.h"
#include "myexception.h"

SVGBuilderVectorAbstr::SVGBuilderVectorAbstr(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilder(parent_node, data, parm, context, mixing){
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::SVGBuilderVectorAbstr begin\n");

    int pointCount = data->PolyCounts[0];

    if (pointCount > 1) {
if (DEBUG_MODE > 3) printf("SVGBuilderVectorAbstr::SVGBuilderVectorAbstr x0=%d y0=%d x1=%d y1=%d \n", data->Points[0].x, data->Points[0].y, data->Points[1].x, data->Points[1].y);
        double sx = data->Points[1].x - data->Points[0].x;
        double sy = data->Points[1].y - data->Points[0].y;
        double r = sqrt(sx * sx + sy * sy);
        this->rotation = acos(sx / r) * 180 / M_PI;
        if (sy < 0) {
          this->rotation = -this->rotation;
        }
    } else {
        this->rotation = 0;
    }
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::SVGBuilderVectorAbstr end\n");
}

void SVGBuilderVectorAbstr::init(){
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::init begin\n");

if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::init imgvectorex->HLine1=%u\n", imgvectorex->HLine1);
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::init imgvectorex->HLine2=%u\n", imgvectorex->HLine2);
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::init imgvectorex->VLine1=%u\n", imgvectorex->HLine1);
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::init imgvectorex->VLine2=%u\n", imgvectorex->VLine2);
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::init imgvectorex->HSize=%u\n", imgvectorex->HSize);
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::init imgvectorex->VSize=%u\n", imgvectorex->VSize);

    if (imgvectorex->HSize==0 || (imgvectorex->HLine2 - imgvectorex->HLine1)==0){
        ve_v_factor_x = 1.;
    } else {
        ve_v_factor_x = 1. * imgvectorex->HSize / (imgvectorex->HLine2 - imgvectorex->HLine1) ;
    }

    if (imgvectorex->VSize==0 || (imgvectorex->VLine2 - imgvectorex->VLine1)==0){
        ve_v_factor_y = 1.;
    } else {
        ve_v_factor_y = 1. * imgvectorex->VSize / (imgvectorex->VLine2 - imgvectorex->VLine1) ;
    }

    posH = imgvectorex->PosH;
    posV = imgvectorex->PosV;
    scale_coeff_x = context->Scale * context->PixelX * ve_v_factor_x;
    scale_coeff_y = context->Scale * context->PixelX  * ve_v_factor_y;
    scale_coeff_thick = context->Scale * context->PixelX;

//    ImgBorder* imgBorder = context->imgBorder;
//    if ((imgBorder->factor_x == null) ||
//        (imgBorder->factor_x == null)){
//        if (ve_v_factor_x) {
//          imgBorder->factor_x = ve_v_factor_x;
//        }
//        if (ve_v_factor_y) {
//          imgBorder->factor_y = ve_v_factor_y;
//        }
//    } else {
//        scale_coeff_x *= ve_v_factor_x/ imgBorder->factor_x;
//        scale_coeff_y *= ve_v_factor_y/ imgBorder->factor_y;
//        scale_coeff_thick *= fmax(ve_v_factor_x, ve_v_factor_y)/ fmax(imgBorder->factor_x, imgBorder->factor_y);
//    }

if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::SVGBuilderVectorAbstr ve_v_factor_x=%0.4f\n", ve_v_factor_x);
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::SVGBuilderVectorAbstr ve_v_factor_y=%0.4f\n", ve_v_factor_y);
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::SVGBuilderVectorAbstr scale_coeff_x=%0.4f\n", scale_coeff_x);
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::SVGBuilderVectorAbstr scale_coeff_y=%0.4f\n", scale_coeff_y);
if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::SVGBuilderVectorAbstr scale_coeff_thick=%0.4f\n", scale_coeff_thick);

if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::init end\n");
}

void SVGBuilderVectorAbstr::build(){
    if (imgvectorex->Count == 0) return;

if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::build begin\n");

    LONG descListSize = imgvectorex->Count;
    IMGPOLYDESC** descList = new IMGPOLYDESC* [descListSize];
\
    int  PosV     = imgvectorex->PosV;
    int  PosH     = imgvectorex->PosH;
    int  VLine1   = imgvectorex->VLine1;
    int  VLine2   = imgvectorex->VLine2;
    int  VSize    = imgvectorex->VSize;
    int  HLine1   = imgvectorex->HLine1;
    int  HLine2   = imgvectorex->HLine2;
    int  HSize    = imgvectorex->HSize;
//    printf("SVGBuilderVectorAbstr::build PosV=%d\n", PosV);
//    printf("SVGBuilderVectorAbstr::build PosH=%d\n", PosH);
//    printf("SVGBuilderVectorAbstr::build VLine1=%d\n", VLine1);
//    printf("SVGBuilderVectorAbstr::build VLine2=%d\n", VLine1);
//    printf("SVGBuilderVectorAbstr::build VSize=%d\n", VSize);
//    printf("SVGBuilderVectorAbstr::build HLine1=%d\n", VLine1);
//    printf("SVGBuilderVectorAbstr::build HLine2=%d\n", VLine1);
//    printf("SVGBuilderVectorAbstr::build HSize=%d\n", VSize);

    char* p = (char*)&(imgvectorex->Desc);
    for (LONG j = 0; j < descListSize; ++j){
        IMGPOLYDESC* desc = (IMGPOLYDESC*)p;
        descList[j] = desc;
        p += 2 * sizeof(unsigned char) + sizeof(unsigned short);
        p += desc->Length;
        ULONG pointCount = (ULONG) *p;
        p += sizeof(ULONG);
        p += pointCount * sizeof(IMGVECTPOINT);
    }

    const char* symbolTag = getSymbolTag();
    QDomElement the_node =  parent_node->appendChild(parent_node->ownerDocument().createElement(symbolTag)).toElement();
    current_node =  &the_node;
    if (*symbolTag == 'g'){
        current_node->setAttribute("id", context->svg_desc->getCurrent_id());
        if (imgvectorex->Static) {
            current_node->setAttribute("rsc:Static", imgvectorex->Static);
        }
        if (imgvectorex->Mirror) {
            current_node->setAttribute("rsc:Mirror", imgvectorex->Mirror);
        }
        if (imgvectorex->NoPress) {
            current_node->setAttribute("rsc:NoPress", imgvectorex->NoPress);
        }
        if (imgvectorex->Scale) {
            current_node->setAttribute("rsc:Scale", imgvectorex->Scale);
        }
        if (imgvectorex->Centre) {
            current_node->setAttribute("rsc:Centre", imgvectorex->Centre);
        }
    }

    try {
      for (int j=0; j<descListSize; ++j){
        IMGPOLYDESC* desc = descList[j];
        addShape(desc);
      }
    } catch(MyException myException) {
        delete[] descList;
        throw MyException(myException);
    } catch(const char* message) {
        delete[] descList;
        throw MyException(message);
    } catch(...) {
        delete[] descList;
        throw MyException("SVGBuilderVectorAbstr::build unknown exxeption");
    }

    if (rotation != 0){
        char* transform = new char[255];
        sprintf(transform, "rotate(%0.2f %0.2f %0.2f)", rotation, dx, dy);
        current_node->setAttribute("transform", transform);
        getImgBorder()->rotate(rotation, dx, dy);
        delete[] transform;
    }

    delete[] descList;

if (DEBUG_MODE > 2) printf("SVGBuilderVectorAbstr::build end\n");
}

const char* SVGBuilderVectorAbstr::getSymbolTag() {
    return "g";
}

void SVGBuilderVectorAbstr::addShape(IMGPOLYDESC* desc){
if (DEBUG_MODE > 3) printf("SVGBuilderVectorAbstr::addShape begin\n");
    unsigned char image = desc->Image;

    VECTORMIX* mixing = getMixing();
    ImgType* imgType = NULL;
    switch (image) {
      case IMG_LINE:{
        imgType = new ImgLine(desc, hrsc, data, dx, dy, posH, posV,
                            scale_coeff_x, scale_coeff_y, scale_coeff_thick, mixing, context);
        break;
      }
      case IMG_DOT:{
        imgType = new ImgDot(desc, hrsc, data, dx, dy, posH, posV,
                          scale_coeff_x, scale_coeff_y, scale_coeff_thick, mixing, context);
        break;
      }
      case IMG_SQUARE:{
        imgType = new ImgSquare(desc, hrsc, data, dx, dy, posH, posV,
                          scale_coeff_x, scale_coeff_y, scale_coeff_thick, mixing, context);
        break;
      }
    case IMG_VECTORTEXT:
    case IMG_VECTORTEXTUN:{
      char unicode = ((IMGVECTOREX*) parm)->Unicode;
      imgType = new ImgVectorText(desc, hrsc, data, dx, dy, posH, posV,
                      scale_coeff_x, scale_coeff_y, scale_coeff_thick, mixing, context,
                                    unicode);
        break;
      }
    }

    if (imgType) {
        imgType->buildSvgElement(current_node, getImgBorder());
        delete imgType;
    } else {
        if (image) {
          char* message = new char[255];
          sprintf(message, "UNKNOWN SVGBuilderVectorAbstr::addShape unknown image %d", image);
          throw MyException(message);
        } else {
          if (DEBUG_MODE > 0) printf("SVGBuilderVectorAbstr::addShape image == 0\n");
        }
    }
if (DEBUG_MODE > 3) printf("SVGBuilderVectorAbstr::addShape end\n");
}


