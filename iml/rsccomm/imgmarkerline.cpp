#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imgmarkerline.h"
#include "imgvectorex.h"
#include "imgvectormix.h"
#include "imgvectortext.h"
#include <map>
#include <math.h>
#include <string.h>

ImgMarkerLine::ImgMarkerLine(IMGPOLYDESC* desc, HRSC hrsc, const POLYDATAEX * data,
                 double scale_coeff_thick,
                 VECTORMIX* mixing, const IMGCONTEXT* context):ImgLine(desc, hrsc, data,
                                                                           1., 1., scale_coeff_thick, mixing, context){
    this->desc = desc;

    char* p = (char*)&(desc->Parm);
    p += desc->Length;
    pPointCount = (ULONG*) p;
    p += sizeof(ULONG);
    points = (IMGVECTPOINT*)p;

    this->markerId = NULL;
if (DEBUG_MODE > 3) printf("ImgMarkerLine::ImgMarkerLine desc->Image=%u desc->Length=%u\n", desc->Image, desc->Length);
if (DEBUG_MODE > 3) printf("ImgMarkerLine::ImgMarkerLine *pPointCount=%u\n", *pPointCount);

}
ImgMarkerLine::~ImgMarkerLine(){
    if (this->markerId) {
        delete[] this->markerId;
    }
}

void ImgMarkerLine::init(const char* markerId, unsigned char imageFunc, int markerKind){
    this->markerId = new char[strlen(markerId)  + 1];
    strcpy(this->markerId, markerId);
    this->imageFunc = imageFunc;
    this->markerKind = markerKind;
}

void ImgMarkerLine::buildSvgElement(QDomElement* parent_node, ImgBorder* imgBorder){
if (DEBUG_MODE > 2) printf("ImgMarkerLine::buildSvgElement begin\n");

    QDomElement defs_node = context->svg_desc->getSvg_node()->firstChildElement("defs");
    if (defs_node == QDomElement()) {
        defs_node = context->svg_desc->getSvg_node()->appendChild(context->svg_desc->getSvg_node()->ownerDocument().createElement("defs")).toElement();
    }
    QDomNodeList marker_nodes = defs_node.elementsByTagName("marker");
    QDomElement marker_node = QDomElement();
    for (int j = 0; j < marker_nodes.count(); ++j){
        QDomElement marker_node_ = marker_nodes.item(j).toElement();
        if (marker_node_.attribute("id") ==  markerId){
           marker_node = marker_node_;
           return;
        }
    }

    if (marker_node.isNull()) {
        marker_node = defs_node.appendChild(defs_node.ownerDocument().createElement("marker")).toElement();
        marker_node.setAttribute("id", markerId);
        marker_node.setAttribute("orient", "auto");

        char* p = (char*)&(desc->Parm);
        p += desc->Length;
        unsigned int* pCount = (unsigned int*) p;
        unsigned int oldPCount = *pCount;
        int oldHor = points[0].Hor;
        int oldVer = points[0].Ver;

        *pCount = 1;
        points[0].Hor = 0;
        points[0].Ver = 0;

        ImgType* imgType;
        switch (imageFunc) {
        case IMG_VECTOREX:{
      if (DEBUG_MODE > 3) printf("ImgMarkerLine::ImgMarkerLine desc->Image=%u desc->Length=%u\n", desc->Image, desc->Length);
      if (DEBUG_MODE > 3) printf("ImgMarkerLine::ImgMarkerLine *pPointCount=%u\n", *pPointCount);


          context->svg_desc->appendImg("IMG_VECTOREX");
          imgType = new ImgVectorex(desc, hrsc, data,
                                    1., 1.,scale_coeff_thick,
                                    mixing, context);
          break;
        }
        case IMG_VECTORMIX:{
          context->svg_desc->appendImg("IMG_VECTORMIX");
          imgType = new ImgVectorMix(desc, hrsc, data,
                                    1., 1., scale_coeff_thick,
                                    mixing, context);
          break;
        }
        default:{
            char* message = new char[255];
            sprintf(message, "UNKNOWN SVGBuilderDash::buildSvgElement unknown decor imageFunc %u", imageFunc);
            throw MyException(message);
        }
        }

        ImgBorder* imgBorder = context->imgBorder;
        double old_minX = imgBorder->minX;
        double old_minY = imgBorder->minY;
        double old_maxX = imgBorder->maxX;
        double old_maxY = imgBorder->maxY;
        double old_maxThick = imgBorder->maxThick;
        imgBorder->init();

        imgType->buildSvgElement(&marker_node, imgBorder);
        context->svg_desc->leaveCurrentNode();
        delete imgType;

        *pCount = oldPCount;
        points[0].Hor = oldHor;
        points[0].Ver = oldVer;

        double relUnitsToPix = 1.;
        imgBorder->addViewBox(&marker_node, relUnitsToPix, true);

        double dx = imgBorder->maxX - imgBorder->minX;
        double dy = imgBorder->maxY - imgBorder->minY;
        double d = sqrt(dx * dx + dy * dy);

        imgBorder->minX = old_minX;
        imgBorder->minY = old_minY;
        imgBorder->maxX = old_maxX;
        imgBorder->maxY = old_maxY;
        imgBorder->maxThick = fmax(old_maxThick, d);
    }

    QDomElement shape_node = parent_node->appendChild(parent_node->ownerDocument().createElement("path")).toElement();

    makePath(&shape_node, imgBorder);

    std::string markerUrl_str = "url(#";
    markerUrl_str.append(markerId);
    markerUrl_str.append(")");
    const char* markerUrl = markerUrl_str.c_str();
    if (markerKind & MARKER_START){
      shape_node.setAttribute("marker-start", markerUrl);
    }
    if (markerKind & MARKER_END){
      shape_node.setAttribute("marker-end", markerUrl);
    }
    if (markerKind & MARKER_MID){
      shape_node.setAttribute("marker-mid", markerUrl);
    }
    shape_node.setAttribute("fill", "none");

if (DEBUG_MODE > 2) printf("ImgMarkerLine::buildSvgElement end\n");
}
