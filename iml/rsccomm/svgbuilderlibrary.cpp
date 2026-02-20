#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "svgbuilderlibrary.h"
#include "debug.h"
#include <QLibrary>

SVGBuilderLibrary::SVGBuilderLibrary(
    QDomElement* parent_node,
    const POLYDATAEX * data,
    const char * parm,
    const IMGCONTEXT * context,
        VECTORMIX* mixing) :SVGBuilder(parent_node, data, parm, context, mixing){

    imglibrary = (IMGLIBRARY*) parm;
}

void SVGBuilderLibrary::build(){
if (DEBUG_MODE  >  2) printf("JNI SVGBuilderLibrary::build begin\n");
    LONG code = imglibrary->Code;   ///< Код DLL (IML).
    LONG dllIndex = imglibrary->Index;   ///< Индекс DLL в списке библиотек.
    LONG functionCode = imglibrary->Function;   ///< Код функции.
    IMGDRAW* imgDraw = &(imglibrary->Draw);   ///< Параметры.

#if defined(__linux__)
   #if defined(__igis__)
     const char* libraryName = "librscarmy_igis.so";
   #else
     const char* libraryName = "librscarmy.so";
   #endif
#else
    const char* libraryName = "rscarmy.dll";
#endif

    QLibrary hLibrary("rscarmy");

    typedef long int (WINAPI * PAINT_IMAGE)(long int type,
                                   const POLYDATAEX* data,
                                   const IMGDRAW * parm,
                                   const IMGCONTEXT * context);
    PAINT_IMAGE imlPaintImage;
    imlPaintImage = (PAINT_IMAGE) hLibrary.resolve("imlPaintImage");
    if (!imlPaintImage){
         char* message = new char[255];
         sprintf(message, "SVGBuilderLibrary::build method 'imlPaintImage' not found");
         throw MyException(message);
     }

    typedef void (WINAPI * SVG_DESC_APPEND_IMG)( const IMGCONTEXT * context,
                                                   long int type);
    SVG_DESC_APPEND_IMG svgDescAppendImg;
    svgDescAppendImg = (SVG_DESC_APPEND_IMG) hLibrary.resolve("svgDescAppendImg");
    if (!svgDescAppendImg){
         char* message = new char[255];
         sprintf(message, "SVGBuilderLibrary::build method 'svgDescAppendImg' not found");
         throw MyException(message);
     }

    typedef void (WINAPI * SVG_DESC_LEAVE_NODE)( const IMGCONTEXT * context,
                                                   long int type);
    SVG_DESC_LEAVE_NODE svgDescLeaveNode;
    svgDescLeaveNode = (SVG_DESC_LEAVE_NODE) hLibrary.resolve("svgDescLeaveNode");
    if (!svgDescLeaveNode){
         char* message = new char[255];
         sprintf(message, "SVGBuilderLibrary::build method 'svgDescLeaveNode' not found");
         throw MyException(message);
     }

     if (DEBUG_MODE  >  3) printf("JSVGBuilderLibrary::build before imlPaintImage\n");
     svgDescAppendImg(context, functionCode);
     imlPaintImage(functionCode, data, imgDraw, context);
     svgDescLeaveNode(context, functionCode);
     if (DEBUG_MODE  >  3) printf("SVGBuilderLibrary::build after imlPaintImage\n");

     hLibrary.unload();

if (DEBUG_MODE  >  2) printf("JNI SVGBuilderLibrary::build end\n");
}
