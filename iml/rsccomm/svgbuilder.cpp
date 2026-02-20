#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuildervectorex.h"
#include "svgbuildervectormix.h"
#include "svgbuilderline.h"
#include "svgbuilderdot.h"
#include "svgbuildersquare.h"
#include "svgbuildertext.h"
#include "svgbuilderpolygonglass.h"
#include "svgbuilderlineglass.h"
#include "svgbuilderdraw.h"
#include "svgbuilderlibrary.h"
#include "svgbuilderdrawmix.h"
#include "svgbuilderlineshift.h"
#include "svgbuilderdotshift.h"
#include "svgbuildervectornode.h"
#include "svgbuilderdecorate.h"
#include "svgbuilderdash.h"
#include "svgbuilderhatchsquare.h"
#include "svgbuildersquarevector.h"
//#include "exception.h"
#include "myexception.h"

#include <climits>


SVGBuilder::SVGBuilder(QDomElement* parent_node,
                       const POLYDATAEX * data,
                       const char * parm,
                       const IMGCONTEXT * context,
                       VECTORMIX* mixing){

    this->baseLineLength_px = context->baseLineLength_px;

    DRAWPOINT point;
    if ((data->Count != 0) &&
        (data->PolyCounts[0] != 0)) {
      point = data->Points[0];
    } else {
        point = NULL;
    }
    this->dx = point.x;
    this->dy = point.y;

    this->svg_node = context->svg_desc->getSvg_node();
    this->parent_node = parent_node;
    this->data = data;
    this->parm = parm;

    this->hrsc = data->MapRsc;
    this->context = context;

    this->mixing = mixing;
}

SVGBuilder::~SVGBuilder(){
}

void SVGBuilder::build(){
}

ImgBorder* SVGBuilder::getImgBorder(){
    return context->imgBorder;
}

void SVGBuilder::buildImage(const char* parm, unsigned short type,
                            const POLYDATAEX * data,
                            const IMGCONTEXT * context,
                            VECTORMIX* mixing){
    SVGBuilder* svgBuilder = NULL;
    QDomElement* parent_node = ((IMGCONTEXT*)context)->svg_desc->getG_node();
    switch (type) {
    case IMG_VECTOREX:{
        context->svg_desc->appendImg("IMG_VECTOREX");
        svgBuilder = new SVGBuilderVectorEx(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_VECTORMIX:{
      context->svg_desc->appendImg("IMG_VECTORMIX");
      svgBuilder = new SVGBuilderVectorMix(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_LINE:{
      context->svg_desc->appendImg("IMG_LINE");
      svgBuilder = new SVGBuilderLine(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_DOT:{
      context->svg_desc->appendImg("IMG_DOT");
      svgBuilder = new SVGBuilderDot(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_SQUARE:{
      context->svg_desc->appendImg("IMG_SQUARE");
      svgBuilder = new SVGBuilderSquare(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_TEXT:{
      context->svg_desc->appendImg("IMG_TEXT");
      svgBuilder = new SVGBuilderText(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_POLYGONGLASS:{
      context->svg_desc->appendImg("IMG_POLYGONGLASS");
      svgBuilder = new SVGBuilderPolygonGlass(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_LINEGLASS:{
      context->svg_desc->appendImg("IMG_LINEGLASS");
      svgBuilder = new SVGBuilderLineGlass(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_DRAW:{
      context->svg_desc->appendImg("IMG_DRAW");
      svgBuilder = new SVGBuilderDraw(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_DRAWMIX:{
      context->svg_desc->appendImg("IMG_DRAWMIX");
      svgBuilder = new SVGBuilderDrawMix(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_LIBRARY :{
      context->svg_desc->appendImg("IMG_LIBRARY");
      svgBuilder = new SVGBuilderLibrary(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_LINESHIFT:{
      context->svg_desc->appendImg("IMG_LINESHIFT");
      svgBuilder = new SVGBuilderLineShift(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_DOTSHIFT:{
      context->svg_desc->appendImg("IMG_DOTSHIFT");
      svgBuilder = new SVGBuilderDotShift(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_VECTORNODE:{
      context->svg_desc->appendImg("IMG_VECTORNODE");
      svgBuilder = new SVGBuilderVectorNode(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_DECORATE:{
      context->svg_desc->appendImg("IMG_DECORATE");
      svgBuilder = new SVGBuilderDecorate(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_DASH:{
      context->svg_desc->appendImg("IMG_DASH");
      svgBuilder = new SVGBuilderDash(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_HATCHSQUARE:{
      context->svg_desc->appendImg("IMG_HATCHSQUARE");
      svgBuilder = new SVGBuilderHatchSquare(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    case IMG_SQUAREVECTOR:{
      context->svg_desc->appendImg("IMG_SQUAREVECTOR");
      svgBuilder = new SVGBuilderSquareVector(parent_node, data, parm, (IMGCONTEXT*)context, mixing);
      break;
    }
    default: {
     char* message = new char[255];
     sprintf(message, "UNKNOWN SVGBuilder::buildImage unknown function: %d", type);
     throw MyException(message);
    }
    }
    if (svgBuilder) {
        try{
           svgBuilder->build();
           context->imgBorder->expand(svgBuilder->getImgBorder());
           context->svg_desc->leaveCurrentNode();
        } catch (MyException myException) {
            delete svgBuilder;
            throw MyException(myException);
        } catch (char* message){
             delete svgBuilder;
             throw MyException(message);
        } catch (...){
             delete svgBuilder;
             throw MyException("SVGBuilder::buildImage unknown exception");
        }
        delete svgBuilder;
    }
}


VECTORMIX*   SVGBuilder::getMixing(){
    return mixing;
}
