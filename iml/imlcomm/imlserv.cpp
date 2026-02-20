/****** imlserv.cpp ************ Belenkov O.V.   ****** 23/01/22 ***
******* imlserv.cpp ************ Panteleeva N.A. ****** 07/12/17 ***
******* imlserv.cpp ************ Shabakov D.A.   ****** 07/10/19 ***
******* imlserv.cpp ************ Korjilov A.     ****** 17/11/14 ***
******* imlserv.cpp ************ Kruzhkov A.E.   ****** 28/11/18 ***
******* imlserv.cpp ************ Savelov A.S.    ****** 27/01/21 ***
******* imlserv.cpp ************ Konon V.N.      ****** 02/06/16 ***
******* imlserv.cpp ************ Ganyushin K.A.  ****** 12/10/18 ***
******* imlserv.cpp ************ Zheleznyakov V.A.***** 04/10/19 ***
******* imlserv.cpp ************ Vitsko D.A.     ****** 24/07/20 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2022              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*     ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ОТОБРАЖЕНИЯ ПОЛЬЗОВАТЕЛЬСКИХ     *
*                  ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ                      *
*                                                                  *
*******************************************************************/

#include "imlserv.h"

#ifndef MAPGDI_H
  #include "mapgdi.h"
#endif

#include "math.h"
#include <stdio.h>

#ifdef WIN32API
 #include <tchar.h>
#endif

#define RGB24BGR24(color) (((color&0x000000FF)<<16) |  \
                           ((color&0x0000FF00)    ) |  \
                           ((color&0x00FF0000)>>16))


typedef long int (WINAPI * PAINTIMAGE)(const POLYDATA4D * data, long int type, const char * parm, const IMGCONTEXT * context);
typedef long int (WINAPI * TESTVISIBLE)(const POLYDATAEX * data, long int type, const char * parm, const IMGCONTEXT * context);
typedef long int (WINAPI * PREPAREBORDER)(const POLYDATAEX * data, long int type, const char * parm, const IMGCONTEXT * context,
                                          FRAME * border);
typedef char * (WINAPI * DOS2WIN)(char * string, long int size);

typedef char * (WINAPI * KOI82WIN)(char * string, long int size);

typedef const char * (WINAPI * GETPATHSHELL)();

typedef long int (WINAPI * GETTRUECOLOR)(const IMGCONTEXT * context, long int color);

typedef long int (WINAPI * GETSEMANTIC)(HRSC hRsc, RSCSEMANTICEX * semtype, long int code);

typedef long int (WINAPI * GETINCODE) (HRSC hRsc,long int excode, long int local, long int number);

typedef long int (WINAPI * GETLANGUAGE)();

typedef long int (WINAPI * GETVECTORSIZE)(IMGVECTOREX * vector,IMAGESIZE* imagesize, char * semantic, HRSC rsc);

typedef char * (WINAPI * GETOBJECTPARAMETERS)(HRSC hRsc,long int incode);

typedef char * (WINAPI * GETPRINTOBJECTPARAMETERS)(HRSC hRsc,long int incode);

typedef long int (WINAPI * GETTITLEBORDER)(const POLYDATAEX * data, const IMGTEXT * parm, const IMGCONTEXT * context);
typedef long int (WINAPI * GETOBJECTFUNCTION)(HRSC hRsc,long int incode);

typedef double (WINAPI * GETMAPREALDISTANCE)(HMAP hmap, DOUBLEPOINT *point1, DOUBLEPOINT *point2);
typedef void (WINAPI * GETMAPPICTURETOPLANE)(HMAP hmap, double * x, double * y);

typedef const char * (WINAPI * GETSEMANTICRECORD)(HOBJ hobj);
typedef const char * (WINAPI * GETOBJECTKEY)(HOBJ hobj);
typedef long int (WINAPI * MAPOBJECTKEY)(HOBJ hobj);
typedef long int (WINAPI * MAPGETRSCOBJECTWCODEUN)(HRSC hRsc,long int incode, WCHAR * wcode, long int size);
typedef long int (WINAPI * MAPGETRSCSEMANTICCLACCIFICATORFULLNAMEUN)(HRSC hRsc, long int code, long int value, WCHAR * buffer, int size);

typedef long int (WINAPI * GETOBJECTNUMBER)(HOBJ hobj);
typedef long int (WINAPI * ISOBJECTLOADING)(HOBJ hobj);
typedef HMAP (WINAPI * GETOBJECTDOCIDENT)(HOBJ hobj);

typedef HSITE (WINAPI * GETOBJECTSITEIDENT)(HMAP hMap,HOBJ hobj);
typedef long int (WINAPI * GETSITESCALE)(HMAP hMap, HSITE hsite);

typedef long int (WINAPI * GETLISTNUMBER)(HOBJ hobj);
typedef HOBJ (WINAPI * CREATEOBJECT)(HMAP hMap,long int sheetnumber);
typedef void (WINAPI * FREEOBJECT)(HOBJ info);
typedef long int (WINAPI * GETLISTFRAMEOBJECT)(HMAP hmap, long int list,HOBJ hobj);

typedef long int (WINAPI * GETSITESHEETNAMEUN)(HMAP hMap, HSITE hSite, long int list, WCHAR *name, long int size);
typedef long int (WINAPI * GETSITENOMENCLATUREUN)(HMAP hMap, HSITE hSite, int list, WCHAR *name, long int size);

typedef long int (WINAPI * MAPNOMENCLATUREUN)(HOBJ info,WCHAR *name, long int size);
typedef long int (WINAPI * MAPLISTNAMEUN)(HOBJ info, WCHAR *name, long int size);
typedef long int (WINAPI * MAPOBJECTMAPSCALE)(HOBJ info);

typedef long int (WINAPI * MAPOBJECTEXCODE)(HOBJ info);
typedef long int (WINAPI * MAPOBJECTCODE)(HOBJ info);
typedef HRSC (WINAPI * MAPGETRSCIDENTBYOBJECT)(HOBJ info);

typedef double (WINAPI * GETOBJECTSQUARE)(HOBJ hobj);
typedef double (WINAPI * GETOBJECTLENGTH)(HOBJ hobj);
typedef long int (WINAPI * ISALIASNAMEUN)(const WCHAR * name);
typedef HSITE (WINAPI * GETCURRENTVIEWSITEEX)(HMAP hmap, HPAINT hsite);
typedef long int (WINAPI * READSITEDOCUMENTUN)(HMAP hMap, HSITE hSite, const WCHAR *alias, WCHAR *name, int size);

typedef long int (WINAPI * GETSITEFILENAMEUN)(HMAP hmap, HSITE hsite, WCHAR *name, int size);
typedef long int (WINAPI * BUILDLONGNAMEEX)(const WCHAR * templ, const WCHAR * name, WCHAR * target, long int size);

typedef long int (WINAPI * GETSEMANTICEX)(HOBJ hobj, RSCSEMANTICEX * semtype, long int semcode);
typedef HMAP (WINAPI * OPENDATAUN)(const WCHAR * name, int mode);
typedef HMAP (WINAPI * OPENDATAREF)(HMAP hmap, HSITE hsite, const WCHAR * name, int mode, long int * error);
typedef void (WINAPI * CLOSEDATA)(HMAP hMap);
typedef int (WINAPI * ISETVIEWSCALEPRO)(HMAP hmap, long int *x, long int *y, float scale, HPAINT hPaint);
typedef int (WINAPI * ISETVIEWSCALEPRINT)(HMAP hmap, float scale, HPAINT hPaint, double * horpix, double * verpix);
typedef int (WINAPI * GETMAPSCALE)(HMAP hmap);
typedef double (WINAPI * GETREALSHOWSCALE)(HMAP hmap);
typedef void (WINAPI * GETPICTURESIZE)(long int *width, long int *height, HPAINT hpaint);
typedef int (WINAPI * GEOWGS84TOPLANE3D)(HMAP hmap, double *Bx, double *Ly, double *H);
typedef void (WINAPI * PLANETOPICTUREPRO)(double *x, double *y, HPAINT hPaint);
typedef int (WINAPI * PAINT95PRO)(HMAP hmap, HDC hdc, int erase, RECT *rect, int alpha, HPAINT hPaint);
typedef int (WINAPI * PAINTTOXIMAGEPRO)(HMAP hmap, XIMAGEDESC *imagedesc, long int erase, long int x, long int y, RECT *rect, HPAINT hPaint);

typedef int (WINAPI * SETVIEWTYPEPRO)(HMAP hmap, long int type, HPAINT hPaint);
typedef int (WINAPI * SETSITETRANSPARENT)(HMAP hmap, HSITE hsite, int transparent);
typedef COLORREF (WINAPI * SETBACKCOLOR)(HMAP hmap, COLORREF color);
typedef int (WINAPI * ISSITEFROMSERVER)(HMAP hMap, HSITE hSite);
typedef HMAP (WINAPI * GETMAPHANDLE)(HPAINT);
typedef long int (WINAPI * SETSEMANTIC)(HOBJ info, long int code, const WCHAR *value, long int size);
typedef long int (WINAPI* GETRSCSEMANTICNAMEUN)(HRSC hRsc, long int code, WCHAR* name, long int size);

typedef HPAINT (WINAPI * CREATEPAINTCONTROL)(HMAP hmap);
typedef HPAINT (WINAPI * CREATEPAINTCONTROLEX)(HMAP hmap, HPAINT hpaint);
typedef void (WINAPI * FREEPAINTCONTROL)(HPAINT hPaint);
typedef long int (WINAPI * ISDIAGNOSTICS)();
typedef void  (WINAPI * WRITETODIAGNOSTICSLOG)(const WCHAR * message, const WCHAR * messageex, long int type);

typedef const char * (WINAPI * GETDOCUMENTFROMSITZ)(HPAINT hpaint, const WCHAR * name, long int * size, long int * error);

typedef void (WINAPI * FREEDOCUMENTFROMSITZ)(const char * memory);

char MapLibrary[MAX_PATH_LONG] = "mapacces64.dll";   // Имя библиотеки ядра

HINSTANCE     hMapLibrary   = (HINSTANCE)0;
PAINTIMAGE    PaintImage    = (PAINTIMAGE)0;
TESTVISIBLE   TestVisible   = (TESTVISIBLE)0;
PREPAREBORDER PrepareBorder = (PREPAREBORDER)0;
DOS2WIN       imlDos2Win    = (DOS2WIN)0;
KOI82WIN      imlKoi82Win   = (KOI82WIN)0;
GETPATHSHELL  imlGetPathShell = (GETPATHSHELL)0;
GETTRUECOLOR  GetTrueColor  = (GETTRUECOLOR)0;
GETSEMANTIC   GetRscSemanticEx = (GETSEMANTIC)0;
GETINCODE     GetRscObjectIncode = (GETINCODE)0;
GETLANGUAGE   GetMapLanguage = (GETLANGUAGE)0;
GETVECTORSIZE GetRscVectorSize = (GETVECTORSIZE)0;
GETOBJECTPARAMETERS  GetRscObjectParameters =  (GETOBJECTPARAMETERS)0;
GETPRINTOBJECTPARAMETERS  GetRscPrintObjectParameters = (GETPRINTOBJECTPARAMETERS)0;
GETTITLEBORDER GetTitleBorder = (GETTITLEBORDER)0;
GETOBJECTFUNCTION GetRscObjectFunction = (GETOBJECTFUNCTION)0;
GETMAPREALDISTANCE GetMapRealDistance = (GETMAPREALDISTANCE) 0;
GETMAPPICTURETOPLANE GetMapPictureToPlane = (GETMAPPICTURETOPLANE)0;
GETSEMANTICRECORD GetSemanticRecord = (GETSEMANTICRECORD)0;
GETOBJECTKEY GetObjectKey = (GETOBJECTKEY)0;
MAPOBJECTKEY MapObjectKey = (MAPOBJECTKEY)0;
MAPGETRSCSEMANTICCLACCIFICATORFULLNAMEUN GetSmClsNameUn = (MAPGETRSCSEMANTICCLACCIFICATORFULLNAMEUN)0;

GETOBJECTNUMBER GetObjectNumber = (GETOBJECTNUMBER)0;
ISOBJECTLOADING IsObjectLoading = (ISOBJECTLOADING)0;
GETOBJECTDOCIDENT GetObjectDocIdent = (GETOBJECTDOCIDENT)0;
GETOBJECTSITEIDENT GetObjectSiteIdent = (GETOBJECTSITEIDENT)0;
GETSITESCALE GetSiteScale = (GETSITESCALE)0;

GETLISTNUMBER GetListNumber = (GETLISTNUMBER)0;
CREATEOBJECT CreateObject = (CREATEOBJECT)0;
FREEOBJECT FreeObject = (FREEOBJECT)0;
GETLISTFRAMEOBJECT GetListFrameObject = (GETLISTFRAMEOBJECT)0;

GETSITESHEETNAMEUN GetSiteSheetNameUn = (GETSITESHEETNAMEUN)0;
GETSITENOMENCLATUREUN GetSiteNomenclatureUn = (GETSITENOMENCLATUREUN)0;

MAPNOMENCLATUREUN GetNomenclatureUn = (MAPNOMENCLATUREUN)0;
MAPLISTNAMEUN GetListNameUn = (MAPLISTNAMEUN)0;
MAPOBJECTMAPSCALE GetObjectMapScale = (MAPOBJECTMAPSCALE)0;

MAPGETRSCOBJECTWCODEUN GetObjectWCodeUn = (MAPGETRSCOBJECTWCODEUN)0;
MAPOBJECTEXCODE  GetObjectExcode =  (MAPOBJECTEXCODE)0;
MAPOBJECTCODE GetObjectIncode = (MAPOBJECTCODE)0;
MAPGETRSCIDENTBYOBJECT GetRsc = (MAPGETRSCIDENTBYOBJECT)0;
GETOBJECTSQUARE GetObjectSquare = (GETOBJECTSQUARE)0;
GETOBJECTSQUARE GetObjectMapSquare = (GETOBJECTSQUARE)0;
GETOBJECTLENGTH GetObjectLength = (GETOBJECTLENGTH)0;
ISALIASNAMEUN IsAliasNameUn = (ISALIASNAMEUN)0;
GETCURRENTVIEWSITEEX GetCurrentViewSite = (GETCURRENTVIEWSITEEX)0;
READSITEDOCUMENTUN ReadSiteDocumentUn = (READSITEDOCUMENTUN)0;
GETSITEFILENAMEUN GetSiteFileNameUn = (GETSITEFILENAMEUN)0;
BUILDLONGNAMEEX BuildLongNameEx = (BUILDLONGNAMEEX)0;

GETSEMANTICEX GetSemanticEx = (GETSEMANTICEX)0;

OPENDATAUN OpenDataUn = (OPENDATAUN)0;
OPENDATAREF OpenDataRef = (OPENDATAREF)0;
CLOSEDATA CloseData = (CLOSEDATA)0;
ISETVIEWSCALEPRO SetViewScalePro = (ISETVIEWSCALEPRO)0;
ISETVIEWSCALEPRINT SetViewScalePrint = (ISETVIEWSCALEPRINT)0;
GETMAPSCALE GetMapScale = (GETMAPSCALE)0;
GETREALSHOWSCALE GetRealShowScale = (GETREALSHOWSCALE)0;
GETPICTURESIZE GetPictureSize = (GETPICTURESIZE)0;
GEOWGS84TOPLANE3D GeoWGS84ToPlane3D = (GEOWGS84TOPLANE3D)0;
PLANETOPICTUREPRO PlaneToPicturePro = (PLANETOPICTUREPRO)0;
PAINT95PRO Paint95Pro = (PAINT95PRO)0;
PAINTTOXIMAGEPRO PaintToXImagePro = (PAINTTOXIMAGEPRO)0;
SETVIEWTYPEPRO SetViewTypePro = (SETVIEWTYPEPRO)0;
SETSITETRANSPARENT SetSiteTransparent = (SETSITETRANSPARENT)0;
SETBACKCOLOR SetBackColor = (SETBACKCOLOR)0;
ISSITEFROMSERVER IsSiteFromServer = (ISSITEFROMSERVER)0;
GETMAPHANDLE GetMapHandle = (GETMAPHANDLE)0;
SETSEMANTIC  SetSemantic = (SETSEMANTIC)0;
GETRSCSEMANTICNAMEUN GetRscSemanticNameUn = (GETRSCSEMANTICNAMEUN)0;

CREATEPAINTCONTROL CreatePaintControl = (CREATEPAINTCONTROL)0;
CREATEPAINTCONTROLEX CreatePaintControlEx = (CREATEPAINTCONTROLEX)0;
FREEPAINTCONTROL FreePaintControl = (FREEPAINTCONTROL)0;
ISDIAGNOSTICS IsDiagnostics = (ISDIAGNOSTICS)0;
WRITETODIAGNOSTICSLOG WriteToDiagnosticsLog = (WRITETODIAGNOSTICSLOG)0;

GETDOCUMENTFROMSITZ GetDocumentFromSitz  = (GETDOCUMENTFROMSITZ)0;
FREEDOCUMENTFROMSITZ FreeDocumentFromSitz = (FREEDOCUMENTFROMSITZ)0;

// Счетчик ссылок
static int imlRefCount = 0;

#if !defined(LINUXAPI) && defined(GDILIB)
#include <GdiPlus.h>
using namespace Gdiplus;

int GdiPresent = 0;

extern "C"
{
int WINAPI GDIInit()
{
 return 1;
}
}

#endif

extern "C"
{
#ifdef SHAPELIB
  void CloseMapListForSite(HSITE hsite);
  void _IMLAPI CloseImlLibrary(void);
#endif

// ------------------------------------------------------------------
// Уведомление о закрытии классификатора в ядре
// Позволяет освободить ресурсы, которые были выделены для отображения
// программируемых знаков для заданного классификатора карты
// ------------------------------------------------------------------
_IMLIMP void _IMLAPI imlFreeDataForSite(HSITE hsite)
{
  // Вставить код при необходимости
#ifdef SHAPELIB
  CloseMapListForSite(hsite);
#endif
}

// ------------------------------------------------------------------
//  Отключить библиотеку ядра ГИС
// ------------------------------------------------------------------
#ifdef IMLCONTROLCLOSE
void _IMLAPI CloseImlLibrary(void);
#endif

#if defined(_M_X64) || defined(BUILD_DLL64)
_IMLIMP void _IMLAPI imlCloseGisLibrary64()
#else
_IMLIMP void _IMLAPI imlCloseGisLibrary()
#endif
{
  imlRefCount--;

  if (imlRefCount > 0)
    return;

  imlRefCount = 0;

  if (hMapLibrary)
  {
#ifdef SHAPELIB
    // Освободить ресурсы алгоритмов отображения
    CloseImlLibrary();
#endif
    if (hMapLibrary == 0)
      return;

#ifndef LINUXAPI
    FreeLibrary(hMapLibrary);
#else
    dlclose((void*)hMapLibrary);
#endif
    hMapLibrary = (HINSTANCE)0;

    PaintImage = (PAINTIMAGE)0;
    TestVisible = (TESTVISIBLE)0;
    PrepareBorder = (PREPAREBORDER)0;
    imlDos2Win = (DOS2WIN)0;
    imlKoi82Win = (KOI82WIN)0;
    imlGetPathShell = (GETPATHSHELL)0;
    GetTrueColor = (GETTRUECOLOR)0;
    GetRscSemanticEx = (GETSEMANTIC)0;
    GetRscObjectIncode = (GETINCODE)0;
    GetMapLanguage = (GETLANGUAGE)0;
    GetRscVectorSize = (GETVECTORSIZE)0;
    GetRscObjectParameters = (GETOBJECTPARAMETERS)0;
    GetRscPrintObjectParameters = (GETPRINTOBJECTPARAMETERS)0;
    GetTitleBorder = (GETTITLEBORDER)0;
    GetRscObjectFunction = (GETOBJECTFUNCTION)0;
    GetMapRealDistance = (GETMAPREALDISTANCE)0;
    GetMapPictureToPlane = (GETMAPPICTURETOPLANE)0;
    GetSemanticRecord = (GETSEMANTICRECORD)0;
    GetObjectKey = (GETOBJECTKEY)0;
    MapObjectKey = (MAPOBJECTKEY)0;
    GetSmClsNameUn =
      (MAPGETRSCSEMANTICCLACCIFICATORFULLNAMEUN)0;
    GetObjectNumber = (GETOBJECTNUMBER)0;
    IsObjectLoading = (ISOBJECTLOADING)0;
    GetObjectDocIdent = (GETOBJECTDOCIDENT)0;
    GetObjectSiteIdent = (GETOBJECTSITEIDENT)0;
    GetSiteScale = (GETSITESCALE)0;

    GetListNumber = (GETLISTNUMBER)0;
    CreateObject = (CREATEOBJECT)0;
    FreeObject = (FREEOBJECT)0;
    GetListFrameObject = (GETLISTFRAMEOBJECT)0;
    GetSiteSheetNameUn = (GETSITESHEETNAMEUN)0;
    GetSiteNomenclatureUn = (GETSITENOMENCLATUREUN)0;

    GetNomenclatureUn = (MAPNOMENCLATUREUN)0;
    GetListNameUn = (MAPLISTNAMEUN)0;
    GetObjectMapScale = (MAPOBJECTMAPSCALE)0;

    GetObjectWCodeUn = (MAPGETRSCOBJECTWCODEUN)0;
    GetObjectExcode = (MAPOBJECTEXCODE)0;
    GetObjectIncode = (MAPOBJECTCODE)0;
    GetRsc = (MAPGETRSCIDENTBYOBJECT)0;
    GetObjectSquare = (GETOBJECTSQUARE)0;
    GetObjectLength = (GETOBJECTLENGTH)0;
    GetObjectMapSquare = (GETOBJECTSQUARE)0;
    IsAliasNameUn = (ISALIASNAMEUN)0;
    GetCurrentViewSite = (GETCURRENTVIEWSITEEX)0;
    ReadSiteDocumentUn = (READSITEDOCUMENTUN)0;
    GetSiteFileNameUn = (GETSITEFILENAMEUN)0;
    BuildLongNameEx = (BUILDLONGNAMEEX)0;
    GetSemanticEx = (GETSEMANTICEX)0;
    OpenDataUn = (OPENDATAUN)0;
    OpenDataRef = (OPENDATAREF)0;
    CloseData = (CLOSEDATA)0;
    SetViewScalePro = (ISETVIEWSCALEPRO)0;
    SetViewScalePrint = (ISETVIEWSCALEPRINT)0;
    GetMapScale = (GETMAPSCALE)0;
    GetRealShowScale = (GETREALSHOWSCALE)0;
    GetPictureSize = (GETPICTURESIZE)0;
    GeoWGS84ToPlane3D = (GEOWGS84TOPLANE3D)0;
    PlaneToPicturePro = (PLANETOPICTUREPRO)0;
    Paint95Pro = (PAINT95PRO)0;
    PaintToXImagePro = (PAINTTOXIMAGEPRO)0;
    SetViewTypePro = (SETVIEWTYPEPRO)0;
    SetSiteTransparent = (SETSITETRANSPARENT)0;
    SetBackColor = (SETBACKCOLOR)0;
    IsSiteFromServer = (ISSITEFROMSERVER)0;
    GetMapHandle = (GETMAPHANDLE)0;
    SetSemantic = (SETSEMANTIC)0;
    GetRscSemanticNameUn = (GETRSCSEMANTICNAMEUN)0;
    CreatePaintControl = (CREATEPAINTCONTROL)0;
    CreatePaintControlEx = (CREATEPAINTCONTROLEX)0;
    FreePaintControl = (FREEPAINTCONTROL)0;
    IsDiagnostics = (ISDIAGNOSTICS)0;
    WriteToDiagnosticsLog = (WRITETODIAGNOSTICSLOG)0;
    GetDocumentFromSitz = (GETDOCUMENTFROMSITZ)0;
    FreeDocumentFromSitz = (FREEDOCUMENTFROMSITZ)0;
  }
}

long int imgPaintImage(const POLYDATA4D * data, long int type, const char * parm, const IMGCONTEXT * context);


// ------------------------------------------------------------------
//  Подключиться к библиотеке ядра
// (вызывается автоматически при каждом открытии классификатора)
// ------------------------------------------------------------------

#if defined(_M_X64) || defined(BUILD_DLL64)
_IMLIMP long int _IMLAPI imlOpenGisLibrary64(const char * libname)
#else
_IMLIMP long int _IMLAPI imlOpenGisLibrary(const char * libname)
#endif
{
    if ((libname == 0) || (*libname == 0) || (strlen(libname) < 4))
      return 0;

    if ((hMapLibrary != 0) && (strcmp(MapLibrary, libname) == 0))
    {
      imlRefCount++;
      return 1;
    }

    if (hMapLibrary != 0)
    {
      // В одном приложении разные MapLibrary не могут быть
      return 0;
    }

    StrCopy(MapLibrary, libname, sizeof(MapLibrary));

#ifndef LINUXAPI
    hMapLibrary = LoadLibraryA(MapLibrary);
#else
    hMapLibrary = (HINSTANCE)dlopen(MapLibrary, RTLD_LAZY);
#endif

    if (hMapLibrary == 0)
      return 0;

#ifndef LINUXAPI
     PaintImage = (PAINTIMAGE)
        GetProcAddress(hMapLibrary, "imgPaintImageEx");

     TestVisible = (TESTVISIBLE)
        GetProcAddress(hMapLibrary, "imgTestVisibleEx");

     PrepareBorder = (PREPAREBORDER)
        GetProcAddress(hMapLibrary, "imgPrepareBorderEx");

     imlDos2Win     = (DOS2WIN)
        GetProcAddress(hMapLibrary, "mapDos2Win");

     imlGetPathShell  = (GETPATHSHELL)
        GetProcAddress(hMapLibrary, "mapGetPathShell");

     GetTrueColor     = (GETTRUECOLOR)
        GetProcAddress(hMapLibrary, "imgGetTrueColorByColor");

     GetRscSemanticEx = (GETSEMANTIC)
        GetProcAddress(hMapLibrary, "mapGetRscSemanticExByCode");

     GetRscObjectIncode = (GETINCODE)
        GetProcAddress(hMapLibrary, "mapGetRscObjectCodeByNumber");
     GetMapLanguage = (GETLANGUAGE)
        GetProcAddress(hMapLibrary, "mapGetMapAccessLanguage");
     GetRscVectorSize = (GETVECTORSIZE)
        GetProcAddress(hMapLibrary, "mapGetVectorSizeEx");
     GetRscObjectParameters = (GETOBJECTPARAMETERS)
        GetProcAddress(hMapLibrary, "mapGetRscObjectParameters");
     GetRscPrintObjectParameters = (GETPRINTOBJECTPARAMETERS)
        GetProcAddress(hMapLibrary, "mapGetRscPrintObjectParameters");
     GetTitleBorder = (GETTITLEBORDER)
        GetProcAddress(hMapLibrary, "imgGetTitleBorder");
     GetRscObjectFunction = (GETOBJECTFUNCTION)
        GetProcAddress(hMapLibrary, "mapGetRscObjectFunction");
     GetMapRealDistance = (GETMAPREALDISTANCE)
        GetProcAddress(hMapLibrary, "mapRealDistance");
     GetMapPictureToPlane = (GETMAPPICTURETOPLANE)
        GetProcAddress(hMapLibrary, "mapPictureToPlane");
     GetSemanticRecord = (GETSEMANTICRECORD)
        GetProcAddress(hMapLibrary, "imgGetSemanticRecord");
     GetObjectKey = (GETOBJECTKEY)
        GetProcAddress(hMapLibrary, "imgGetObjectKey");
     MapObjectKey = (MAPOBJECTKEY)
        GetProcAddress(hMapLibrary, "mapObjectKey");
     GetSmClsNameUn = (MAPGETRSCSEMANTICCLACCIFICATORFULLNAMEUN)
        GetProcAddress(hMapLibrary, "mapGetRscSemanticClassificatorFullNameUn");

     GetObjectNumber = (GETOBJECTNUMBER)
        GetProcAddress(hMapLibrary, "mapGetObjectNumber");
     IsObjectLoading = (ISOBJECTLOADING)
        GetProcAddress(hMapLibrary, "mapIsObjectLoading");

     GetObjectDocIdent = (GETOBJECTDOCIDENT)
        GetProcAddress(hMapLibrary, "mapGetObjectDocIdent");
     GetObjectSiteIdent = (GETOBJECTSITEIDENT)
        GetProcAddress(hMapLibrary, "mapGetObjectSiteIdent");
     GetSiteScale = (GETSITESCALE)
        GetProcAddress(hMapLibrary, "mapGetSiteScale");

     GetListNumber = (GETLISTNUMBER)
        GetProcAddress(hMapLibrary, "mapGetListNumber");
     CreateObject = (CREATEOBJECT)
        GetProcAddress(hMapLibrary, "mapCreateObject");
     FreeObject = (FREEOBJECT)
        GetProcAddress(hMapLibrary, "mapGetSiteListFrameObject");
     GetListFrameObject = (GETLISTFRAMEOBJECT)
        GetProcAddress(hMapLibrary, "mapGetSiteListFrameObject");
     GetSiteSheetNameUn = (GETSITESHEETNAMEUN)
         GetProcAddress(hMapLibrary, "mapGetSiteSheetNameUn");
     GetSiteNomenclatureUn = (GETSITENOMENCLATUREUN)
         GetProcAddress(hMapLibrary, "mapGetSiteNomenclatureUn");

     GetNomenclatureUn = (MAPNOMENCLATUREUN)
         GetProcAddress(hMapLibrary, "mapNomenclatureUn");
     GetListNameUn = (MAPLISTNAMEUN)
         GetProcAddress(hMapLibrary, "mapListNameUn");
     GetObjectMapScale = (MAPOBJECTMAPSCALE)
         GetProcAddress(hMapLibrary, "mapObjectMapScale");

     GetObjectWCodeUn = (MAPGETRSCOBJECTWCODEUN)
            GetProcAddress(hMapLibrary, "mapGetRscObjectWCodeUn");
     GetObjectExcode =  (MAPOBJECTEXCODE)
            GetProcAddress(hMapLibrary, "mapObjectExcode");
     GetObjectIncode = (MAPOBJECTCODE)
            GetProcAddress(hMapLibrary, "mapObjectCode");
     GetRsc = (MAPGETRSCIDENTBYOBJECT)
            GetProcAddress(hMapLibrary, "mapGetRscIdentByObject");
      GetObjectSquare = (GETOBJECTSQUARE)
        GetProcAddress(hMapLibrary, "mapConventionalSquare");
     GetObjectMapSquare = (GETOBJECTSQUARE)
        GetProcAddress(hMapLibrary, "mapSquareInMap");
     GetObjectLength = (GETOBJECTLENGTH)
        GetProcAddress(hMapLibrary, "mapLength");
     IsAliasNameUn = (ISALIASNAMEUN)
        GetProcAddress(hMapLibrary, "mapIsAliasNameUn");
     GetCurrentViewSite = (GETCURRENTVIEWSITEEX)
        GetProcAddress(hMapLibrary, "mapGetCurrentViewSiteEx");
     ReadSiteDocumentUn = (READSITEDOCUMENTUN)
        GetProcAddress(hMapLibrary, "mapReadSiteDocumentUn");
     GetSiteFileNameUn = (GETSITEFILENAMEUN)
        GetProcAddress(hMapLibrary, "mapGetSiteFileNameUn");
     BuildLongNameEx = (BUILDLONGNAMEEX)
        GetProcAddress(hMapLibrary, "mapBuildLongNameEx");
     GetSemanticEx = (GETSEMANTICEX)
        GetProcAddress(hMapLibrary, "mapSemanticDescribeEx");
     OpenDataUn = (OPENDATAUN)
        GetProcAddress(hMapLibrary, "mapOpenDataUn");
     OpenDataRef = (OPENDATAREF)
        GetProcAddress(hMapLibrary, "mapOpenDataRef");
     CloseData  = (CLOSEDATA)
        GetProcAddress(hMapLibrary, "mapCloseData");
     SetViewScalePro  = (ISETVIEWSCALEPRO)
        GetProcAddress(hMapLibrary, "mapSetViewScalePro");
     SetViewScalePrint  = (ISETVIEWSCALEPRINT)
        GetProcAddress(hMapLibrary, "mapSetViewScalePrint");
     GetMapScale  = (GETMAPSCALE)
        GetProcAddress(hMapLibrary, "mapGetMapScale");
     GetRealShowScale  = (GETREALSHOWSCALE)
        GetProcAddress(hMapLibrary, "mapGetRealShowScale");
     GetPictureSize  = (GETPICTURESIZE)
        GetProcAddress(hMapLibrary, "mapGetPictureSizeEx");
     GeoWGS84ToPlane3D = (GEOWGS84TOPLANE3D)
        GetProcAddress(hMapLibrary, "mapGeoWGS84ToPlane3D");
     PlaneToPicturePro = (PLANETOPICTUREPRO)
        GetProcAddress(hMapLibrary, "mapPlaneToPicturePro");
     Paint95Pro = (PAINT95PRO)
        GetProcAddress(hMapLibrary, "mapPaint95Pro");
     PaintToXImagePro = (PAINTTOXIMAGEPRO)
        GetProcAddress(hMapLibrary, "mapPaintToXImagePro");
     SetViewTypePro = (SETVIEWTYPEPRO)
        GetProcAddress(hMapLibrary, "mapSetViewTypePro");
     SetSiteTransparent = (SETSITETRANSPARENT)
        GetProcAddress(hMapLibrary, "mapSetSiteTransparent");
     SetBackColor = (SETBACKCOLOR)
        GetProcAddress(hMapLibrary, "mapSetBackColor");

     IsSiteFromServer = (ISSITEFROMSERVER)
        GetProcAddress(hMapLibrary, "mapIsSiteFromServer");

     GetMapHandle = (GETMAPHANDLE)
        GetProcAddress(hMapLibrary, "mapGetPaintControlMapHandle");
     SetSemantic = (SETSEMANTIC)
        GetProcAddress(hMapLibrary,"mapAppendSemanticUnicode");
     GetRscSemanticNameUn = (GETRSCSEMANTICNAMEUN)
       GetProcAddress(hMapLibrary, "mapGetRscSemanticNameUn");
     CreatePaintControl = (CREATEPAINTCONTROL)
         GetProcAddress(hMapLibrary,"mapCreatePaintControl");
     CreatePaintControlEx = (CREATEPAINTCONTROLEX)
         GetProcAddress(hMapLibrary,"mapCreatePaintControlEx");
     FreePaintControl = (FREEPAINTCONTROL)
         GetProcAddress(hMapLibrary,"mapFreePaintControl");
     IsDiagnostics = (ISDIAGNOSTICS)
         GetProcAddress(hMapLibrary,"mapIsDiagnostics");
     WriteToDiagnosticsLog = (WRITETODIAGNOSTICSLOG)
         GetProcAddress(hMapLibrary,"mapWriteToDiagnosticsLog");
     GetDocumentFromSitz = (GETDOCUMENTFROMSITZ)
         GetProcAddress(hMapLibrary,"mapGetDocumentFromSitzEx");
     FreeDocumentFromSitz = (FREEDOCUMENTFROMSITZ)
         GetProcAddress(hMapLibrary,"mapFreeDocumentFromSitz");
#else
     PaintImage = (PAINTIMAGE)
        dlsym((void *)hMapLibrary, "imgPaintImageEx");

     TestVisible = (TESTVISIBLE)
        dlsym((void *)hMapLibrary, "imgTestVisibleEx");

     PrepareBorder = (PREPAREBORDER)
        dlsym((void *)hMapLibrary, "imgPrepareBorderEx");

     imlDos2Win     = (DOS2WIN)
        dlsym((void *)hMapLibrary, "mapDos2Win");

     imlKoi82Win     = (KOI82WIN)
        dlsym((void *)hMapLibrary, "mapKoi82Win");

     imlGetPathShell = (GETPATHSHELL)
        dlsym((void *)hMapLibrary, "mapGetPathShell");

     GetTrueColor     = (GETTRUECOLOR)
        dlsym((void *)hMapLibrary, "imgGetTrueColorByColor");

     GetRscSemanticEx = (GETSEMANTIC)
        dlsym((void *)hMapLibrary, "mapGetRscSemanticExByCode");

     GetRscObjectIncode = (GETINCODE)
        dlsym((void *)hMapLibrary, "mapGetRscObjectCodeByNumber");
     GetMapLanguage = (GETLANGUAGE)
        dlsym((void *)hMapLibrary, "mapGetMapAccessLanguage");
     GetRscVectorSize = (GETVECTORSIZE)
        dlsym((void *)hMapLibrary, "mapGetVectorSizeEx");
     GetRscObjectParameters = (GETOBJECTPARAMETERS)
        dlsym((void *)hMapLibrary, "mapGetRscObjectParameters");
     GetRscPrintObjectParameters = (GETPRINTOBJECTPARAMETERS)
        dlsym((void *)hMapLibrary, "mapGetRscPrintObjectParameters");
     GetTitleBorder = (GETTITLEBORDER)
        dlsym((void *)hMapLibrary, "imgGetTitleBorder");
     GetRscObjectFunction = (GETOBJECTFUNCTION)
         dlsym((void *)hMapLibrary, "mapGetRscObjectFunction");
     GetMapRealDistance = (GETMAPREALDISTANCE)
         dlsym((void *)hMapLibrary, "mapRealDistance");
     GetMapPictureToPlane = (GETMAPPICTURETOPLANE)
         dlsym((void *)hMapLibrary, "mapPictureToPlane");
     GetSemanticRecord = (GETSEMANTICRECORD)
         dlsym((void *)hMapLibrary, "imgGetSemanticRecord");
     GetObjectKey = (GETOBJECTKEY)
         dlsym((void *)hMapLibrary, "imgGetObjectKey");
     MapObjectKey = (MAPOBJECTKEY)
         dlsym ((void *)hMapLibrary, "mapObjectKey");
     GetSmClsNameUn = (MAPGETRSCSEMANTICCLACCIFICATORFULLNAMEUN)
         dlsym ((void *)hMapLibrary, "mapGetRscSemanticClassificatorFullNameUn");

     GetObjectNumber = (GETOBJECTNUMBER)
        dlsym ((void *)hMapLibrary, "mapGetObjectNumber");
     IsObjectLoading = (ISOBJECTLOADING)
        dlsym ((void *)hMapLibrary, "mapIsObjectLoading");

     GetObjectDocIdent = (GETOBJECTDOCIDENT)
        dlsym ((void *)hMapLibrary, "mapGetObjectDocIdent");
     GetObjectSiteIdent = (GETOBJECTSITEIDENT)
        dlsym ((void *)hMapLibrary, "mapGetObjectSiteIdent");
     GetSiteScale = (GETSITESCALE)
        dlsym ((void *)hMapLibrary, "mapGetSiteScale");

     GetListNumber = (GETLISTNUMBER)
        dlsym ((void *)hMapLibrary, "mapGetListNumber");
     CreateObject = (CREATEOBJECT)
        dlsym ((void *)hMapLibrary, "mapCreateObject");
     FreeObject = (FREEOBJECT)
        dlsym ((void *)hMapLibrary, "mapGetSiteListFrameObject");
     GetListFrameObject = (GETLISTFRAMEOBJECT)
        dlsym ((void *)hMapLibrary, "mapGetSiteListFrameObject");

     GetSiteSheetNameUn = (GETSITESHEETNAMEUN)
         dlsym ((void *)hMapLibrary,"mapGetSiteSheetNameUn");
     GetSiteNomenclatureUn = (GETSITENOMENCLATUREUN)
          dlsym ((void *)hMapLibrary, "mapGetSiteNomenclatureUn");

     GetNomenclatureUn = (MAPNOMENCLATUREUN)
        dlsym ((void *)hMapLibrary, "mapNomenclatureUn");
     GetListNameUn = (MAPLISTNAMEUN)
        dlsym ((void *)hMapLibrary, "mapListNameUn");
     GetObjectMapScale = (MAPOBJECTMAPSCALE)
        dlsym ((void *)hMapLibrary, "mapObjectMapScale");

     GetObjectWCodeUn = (MAPGETRSCOBJECTWCODEUN)
         dlsym ((void *)hMapLibrary, "mapGetRscObjectWCodeUn");
     GetObjectExcode =  (MAPOBJECTEXCODE)
         dlsym ((void *)hMapLibrary, "mapObjectExcode");
     GetObjectIncode = (MAPOBJECTCODE)
          dlsym ((void *)hMapLibrary, "mapObjectCode");
     GetRsc = (MAPGETRSCIDENTBYOBJECT)
          dlsym ((void *)hMapLibrary, "mapGetRscIdentByObject");
      GetObjectSquare = (GETOBJECTSQUARE)
         dlsym((void *)hMapLibrary, "mapSquare");
     GetObjectMapSquare = (GETOBJECTSQUARE)
         dlsym((void *)hMapLibrary, "mapSquareInMap");
     GetObjectLength = (GETOBJECTLENGTH)
         dlsym((void *)hMapLibrary, "mapLength");
     IsAliasNameUn = (ISALIASNAMEUN)
         dlsym((void *)hMapLibrary, "mapIsAliasNameUn");
     GetCurrentViewSite = (GETCURRENTVIEWSITEEX)
         dlsym((void *)hMapLibrary, "mapGetCurrentViewSiteEx");
     ReadSiteDocumentUn = (READSITEDOCUMENTUN)
        dlsym((void *)hMapLibrary, "mapReadSiteDocumentUn");
     GetSiteFileNameUn = (GETSITEFILENAMEUN)
        dlsym((void *)hMapLibrary, "mapGetSiteFileNameUn");
     BuildLongNameEx = (BUILDLONGNAMEEX)
        dlsym((void *)hMapLibrary, "mapBuildLongNameEx");
     GetSemanticEx = (GETSEMANTICEX)
         dlsym((void *)hMapLibrary, "mapSemanticDescribeEx");

     OpenDataUn = (OPENDATAUN)
        dlsym((void *)hMapLibrary, "mapOpenDataUn");
     OpenDataRef = (OPENDATAREF)
        dlsym((void *)hMapLibrary, "mapOpenDataRef");
     CloseData  = (CLOSEDATA)
        dlsym((void *)hMapLibrary, "mapCloseData");
     SetViewScalePro  = (ISETVIEWSCALEPRO)
        dlsym((void *)hMapLibrary, "mapSetViewScalePro");
     SetViewScalePrint  = (ISETVIEWSCALEPRINT)
        dlsym((void *)hMapLibrary, "mapSetViewScalePrint");
     GetMapScale  = (GETMAPSCALE)
        dlsym((void *)hMapLibrary, "mapGetMapScale");
     GetRealShowScale  = (GETREALSHOWSCALE)
        dlsym((void *)hMapLibrary, "mapGetRealShowScale");
     GetPictureSize  = (GETPICTURESIZE)
        dlsym((void *)hMapLibrary, "mapGetPictureSizeEx");
     GeoWGS84ToPlane3D = (GEOWGS84TOPLANE3D)
        dlsym((void *)hMapLibrary, "mapGeoWGS84ToPlane3D");
     PlaneToPicturePro = (PLANETOPICTUREPRO)
        dlsym((void *)hMapLibrary, "mapPlaneToPicturePro");
     Paint95Pro = (PAINT95PRO)
        dlsym((void *)hMapLibrary, "mapPaint95Pro");
     PaintToXImagePro = (PAINTTOXIMAGEPRO)
        dlsym((void *)hMapLibrary, "mapPaintToXImagePro");
     SetViewTypePro = (SETVIEWTYPEPRO)
        dlsym((void *)hMapLibrary, "mapSetViewTypePro");
     SetSiteTransparent = (SETSITETRANSPARENT)
        dlsym((void *)hMapLibrary, "mapSetSiteTransparent");
     SetBackColor = (SETBACKCOLOR)
        dlsym((void *)hMapLibrary, "mapSetBackColor");
     IsSiteFromServer = (ISSITEFROMSERVER)
        dlsym((void *)hMapLibrary, "mapIsSiteFromServer");
     GetMapHandle = (GETMAPHANDLE)
        dlsym((void *)hMapLibrary, "mapGetPaintControlMapHandle");
     SetSemantic = (SETSEMANTIC)
        dlsym((void *)hMapLibrary, "mapAppendSemanticUnicode");
     GetRscSemanticNameUn = (GETRSCSEMANTICNAMEUN)
       dlsym((void*)hMapLibrary, "mapGetRscSemanticNameUn");
     CreatePaintControl = (CREATEPAINTCONTROL)
         dlsym((void *)hMapLibrary,"mapCreatePaintControl");
     CreatePaintControlEx = (CREATEPAINTCONTROLEX)
         dlsym((void *)hMapLibrary,"mapCreatePaintControlEx");
     FreePaintControl = (FREEPAINTCONTROL)
         dlsym((void *)hMapLibrary,"mapFreePaintControl");
     IsDiagnostics = (ISDIAGNOSTICS)
         dlsym((void *)hMapLibrary,"mapIsDiagnostics");
     WriteToDiagnosticsLog = (WRITETODIAGNOSTICSLOG)
         dlsym((void *)hMapLibrary,"mapWriteToDiagnosticsLog");
     GetDocumentFromSitz = (GETDOCUMENTFROMSITZ)
         dlsym((void *)hMapLibrary,"mapGetDocumentFromSitzEx");
     FreeDocumentFromSitz = (FREEDOCUMENTFROMSITZ)
         dlsym((void *)hMapLibrary,"mapFreeDocumentFromSitz");
#endif   // LINUXAPI

     imlRefCount++;

     if ((TestVisible && PaintImage) == 0)
       {
#if defined(_M_X64) || defined(BUILD_DLL64)
         imlCloseGisLibrary64();
#else
         imlCloseGisLibrary();
#endif
         return 0;
       }

   return 1;
}

} // extern "C"

/* Volkov I.A. BEGIN */
//// ------------------------------------------------------------------
//// Обработать графический объект в соответствии с видом операции
//// test    - вид операции (0 - рисовать, 1 - определить попадание в область,
////                         2 - определить габариты графического объекта),
//// data    - координаты графического объекта,
//// type    - тип графического объекта (см. mapgdi.h : IMG_LINE...),
//// parm    - параметры рисуемого объекта (см. mapgdi.h : IMGLINE...),
//// context - вспомогательные параметры отображения (см. mapgdiex.h),
//// border  - рассчитанные габариты с учетом их предыдущего значения !
//// При ошибке возвращает ноль
//// ------------------------------------------------------------------
//int _fastcall imgPrepareImage(const POLYDATA4D* data, int type, const char* parm,
//                              const IMGCONTEXT* context, int test, FRAME* border)
//{
//  if (test == 0)
//  {
//    if (PaintImage)
//      return PaintImage(data, type, parm, context);
//    return 0;
//  }

//  if (test == PREPARE_TEST)
//  {
//    if (TestVisible)
//      return TestVisible(data, type, parm, context);
//    return 0;
//  }

//  if (PrepareBorder)
//    return PrepareBorder(data, type, parm, context, border);

//  return 0;
//}

//int _fastcall imgPrepareImage(const POLYDATAEX * data, int type, const char * parm,
//                              const IMGCONTEXT * context, int test, FRAME * border)
//{
//  POLYDATA4D data4D;
//  if (data == 0)
//    return 0;
//  memcpy(&data4D, data, sizeof(POLYDATAEX));

//  return imgPrepareImage(&data4D, type, parm, context, test, border);
//}
/* Volkov I.A. END */

// ------------------------------------------------------------------
// Проверить попадание графического объекта в отображаемую область
// type - тип функции отображения, включенной в IMLLIST,
// data - координаты линии в области памяти от
//        верхнего левого угла;
// parm - параметры отображения (MAPGDI.H);
// context - дополнительные параметры отображения.
// Возвращаемое значение :
//  0  - объект не виден (слишком мелкий масштаб
//       или объект не виден по координатам ...),
//       или код объекта не поддерживается;
//  +1 - объект отображен.
// При ошибке возвращает ноль
// ------------------------------------------------------------------
int _fastcall imgTestImage(const POLYDATAEX * data, int type, const char * parm,
                           const IMGCONTEXT * context)
{
   if (TestVisible)
     return TestVisible(data, type, parm, context);
   return 0;
}

// ------------------------------------------------------------------
// Преобразовать текстовую строку из кодировки OEM (DOS) в ANSI (WINDOWS)
// При ошибке возвращает 0
// ------------------------------------------------------------------
char * _fastcall imgDos2Win(char * string, long int size)
{
  if (imlDos2Win)
    return imlDos2Win(string, size);
  return 0;
}

// ------------------------------------------------------------------
// Преобразовать текстовую строку из кодировки KOI8 (UNIX) в ANSI (WINDOWS)
// При ошибке возвращает 0
// ------------------------------------------------------------------
char * _fastcall imgKoi82Win(char * string, long int size)
{
  if (imlKoi82Win)
    return imlKoi82Win(string, size);
  return 0;
}

// ------------------------------------------------------------------
// Путь к exe
// ------------------------------------------------------------------
const char * _fastcall imgGetPathShell()
{
  if (imlGetPathShell)
    return imlGetPathShell();
  return "";
}

/* Volkov I.A. BEGIN */
//// ------------------------------------------------------------------
//// Вычисление расстояния между двумя точками на местности
//// hmap   - идентификатор открытых данных
//// point1, point2 - координаты точек в метрах на местности
//// Для вычисления расстояния координаты пересчитываюся
//// в проекцию топографической карты с установкой осевого
//// меридиана в центре отрезка
//// При ошибке возвращает 0
//// ------------------------------------------------------------------
//double _fastcall imgRealDistance(HMAP hmap, DOUBLEPOINT *point1, DOUBLEPOINT *point2)
//{
//  if (GetMapRealDistance)
//    return (GetMapRealDistance(hmap,point1,point2));
//  return 0.0;
//}

//// ------------------------------------------------------------------
//// Преобразование из пикселов в изображении в координаты
//// на местности в метрах
//// Применение :
//// xmet = xdis; ymet = ydis;
//// mapPictureToPlane(xmet,ymet);
//// hmap - идентификатор открытых данных
//// x,y  - преобразуемые координаты
//// ------------------------------------------------------------------
//void _fastcall imgPictureToPlane(HMAP hmap, double *x, double *y)
//{
//  if (GetMapPictureToPlane)
//    GetMapPictureToPlane(hmap, x, y);
//}
/* Volkov I.A. END */

// ------------------------------------------------------------------
// Запросить рамку подписи
// data    - координаты линии в области памяти от
//           верхнего левого угла,
// parm    - параметры подписи (см.MAPGDI.H)
// context - текущие условия отображения,
// При успешном выполнении в области координат, на которую указывает
// параметр data, записываются 5 точек замкнутого полигона,
// соответствующего габаритам подписи с учетом межстрочного и
// межсимвольного интервала. При установке в параметрах подписи
// свойства закрашивания фона используются эти же габариты.
// При ошибке возвращает ноль
// ------------------------------------------------------------------
int _fastcall imgGetTitleBorder(const POLYDATAEX * data, const IMGTEXT * parm, const IMGCONTEXT * context)
{
  if (GetTitleBorder)
    return GetTitleBorder(data, parm, context);
  return 0;
}

// Заполнить структуру описания семантической характеристики
// по коду семантики
// code - внешний код семантики
// При ошибке возвращает ноль

/* Volkov I.A. BEGIN */
//int _fastcall imlGetRscSemanticExByCode(HRSC hRsc, RSCSEMANTICEX * semtype, int code)
//{
//  if (GetRscSemanticEx)
//    return GetRscSemanticEx(hRsc, semtype, code);
//  return 0;
//}

//// ---------------------------------------------------------
//// Найти внутренний код объекта по коду,локализации и номеру в аналогичных
//// code - внешний код семантики
//// При ошибке возвращает ноль
//// ---------------------------------------------------------
//int _fastcall imlGetRscObjectIncode(HRSC hRsc, long int excode, long int local, long int number)
//{
//  if (GetRscObjectIncode)
//    return GetRscObjectIncode(hRsc,excode,local,number);
//  return 0;
//}

//// ---------------------------------------------------------
//// Запросить язык сообщений
//// 1 - английский, 2 - русский, ... (см. Maptype.h)
//// ---------------------------------------------------------
//int _fastcall imlGetMapAccessLanguage()
//{
//   if (GetMapLanguage)
//      return GetMapLanguage();
//   return 1;
//}
/* Volkov I.A. END */

// ---------------------------------------------------------
// Запросить является ли имя идентификатором данных на Сервере
// Если да, то возвращает ненулевое значение (1 - устаревший
// формат без имени сервера, 2 - содержит имя сервера)
// ---------------------------------------------------------
int _fastcall imlIsAliasNameUn(const WCHAR *name)
{
  if (IsAliasNameUn)
    return IsAliasNameUn(name);
  return 0;
}

// ---------------------------------------------------------
// Запросить идентификатор текущей отображаемой карты
// (при запросе в момент отображения из вспомогательной библиотеки -
// считаем, что один HMAP применяется в одном потоке отображения)
// При ошибке возвращает ноль
// ---------------------------------------------------------
HSITE _fastcall imlGetCurrentViewSiteEx(HMAP hmap, HPAINT hpaint)
{
  if (GetCurrentViewSite)
    return GetCurrentViewSite(hmap, hpaint);
  return 0;
}

// ---------------------------------------------------------
// Считать документ на сервере
// hMap   - идентификатор открытой основной карты
// hSite  - идентификатор открытой пользовательской карты
// alias  - алиас документа на сервере (может храниться в семантике
//          объекта карты, начинается со строки "HOST#")
// name   - полный путь к считанному документу, строка заполняется
//          автоматически при считывании документа, имя документа и
//          дата редактирования устанавливаются такими, какими они были
//          при записи в mapSaveSiteDocument.
// size   - размер буфера в байтах для записи пути (не менее 520 байт)
// При успешном выполнении возвращает имя считанного файла документа в поле name
// При ошибке возвращает ноль
// ---------------------------------------------------------
int _fastcall imlReadSiteDocumentUn(HMAP hmap, HSITE hsite, const WCHAR *alias, WCHAR *name, int size)
{
  if (ReadSiteDocumentUn)
    return ReadSiteDocumentUn(hmap, hsite, alias, name, size);
  return 0;
}

// ---------------------------------------------------------
// Определить имя файла пользовательской карты
// по ее идентификатору
// hMap  - идентификатор открытой карты
// hSite - идентификатор открытой пользовательской карты
// При ошибке возвращает пустую строку
// ---------------------------------------------------------
int _fastcall imlGetSiteFileNameUn(HMAP hmap, HSITE hsite, WCHAR *name, int size)
{
  if (GetSiteFileNameUn)
    return GetSiteFileNameUn(hmap, hsite, name, size);
  return 0;
}

// ---------------------------------------------------------
// Построить "длинное" имя файла (полный путь к файлу)
// templ - эталонный путь, относительно которого строится полный путь;
// templ - путь к библиотекам приложения (mapGetPathShell)
// name - короткий путь к файлу; например, имя файла
// path - "длинное" имя файла (указатель на массив для размещения результата)
// size - размер выделенного буфера (path) в байтах
// При ошибке возвращает ноль
// ---------------------------------------------------------
int _fastcall imlBuildLongNameEx(const WCHAR *templ, const WCHAR *name, WCHAR *path, long int size)
{
  if (BuildLongNameEx)
    return BuildLongNameEx(templ, name, path, size);
  return 0;
}

/* Volkov I.A. BEGIN */
//// ---------------------------------------------------------
//// Запросить размеры векторного объекта
//// semantic - указатель на семантику объекта, если подпись зависит от семантики
//// При ошибке возвращает ноль
//// ---------------------------------------------------------
//int _fastcall imlGetRscVectorSize(IMGVECTOREX * vector,IMAGESIZE* imagesize,
//                                  char * semantic, HRSC rsc)
//{
//  if (GetRscVectorSize)
//    return GetRscVectorSize(vector, imagesize, semantic, rsc);
//  return 0;
//}

//// ---------------------------------------------------------
//// Запросить экранные параметры объекта по incode
//// При ошибке возвращает ноль
//// ---------------------------------------------------------
//char * _fastcall imlGetRscObjectParameters(HRSC hRsc,long int incode)
//{
//  if (GetRscObjectParameters)
//    return GetRscObjectParameters(hRsc,incode);
//  return 0;
//}

//// ---------------------------------------------------------
//// Запросить принтерные параметры объекта по incode
//// При ошибке возвращает ноль
//// ---------------------------------------------------------
//char * _fastcall imlGetRscPrintObjectParameters(HRSC hRsc,long int incode)
//{
//  if (GetRscPrintObjectParameters)
//    return GetRscPrintObjectParameters(hRsc,incode);
//  return 0;
//}

//// ---------------------------------------------------------
//// Запросить экранную функцию объекта по incode
//// При ошибке возвращает ноль
//// ---------------------------------------------------------
// int _fastcall imlGetRscObjectFunction(HRSC hRsc,long int incode)
//{
//  if (GetRscObjectFunction)
//    return GetRscObjectFunction(hRsc,incode);
//  return 0;
//}

//// ------------------------------------------------------------------
////  Определить значение цвета в формате RGB по значению цвета
////  в параметрах отображения
//// ------------------------------------------------------------------
//long int imgGetTrueColorByColor(const IMGCONTEXT * context, long int color)
//{
//  // Это индекс цвета из палитры карты ?
//  if ((color & IMGC_INDEX) == 0)
//    {
//      // Это чистый цвет
//      return color;
//    }

//  if (GetTrueColor)
//    return RGB24BGR24(GetTrueColor(context, color));

//  return 0;
//}
/* Volkov I.A. END */

// ------------------------------------------------------------------
// Запросить адрес записи семантики объекта
// При ошибке возвращает ноль
// ------------------------------------------------------------------
const char * imlGetSemanticRecord(HOBJ hobj)
{
  if (GetSemanticRecord)
    return GetSemanticRecord(hobj);
  return 0;
}


// ------------------------------------------------------------------
// Запросить строку с ключом объекта
// При ошибке возвращает ноль
// ------------------------------------------------------------------
const char * imlGetObjectKey(HOBJ hobj)
{
  if (GetObjectKey)
    return GetObjectKey(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Запросить строку с буквенно - цифровым кодом объекта по incode
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetObjectWCode(HRSC hRsc, long int incode, WCHAR* wcode, long int size)
{
  if (GetObjectWCodeUn)
    return GetObjectWCodeUn(hRsc, incode, wcode, size);
  return 0;
}

// ------------------------------------------------------------------
// Запросить код объекта
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetObjectExcode(HOBJ hobj)
{
  if (GetObjectExcode)
    return GetObjectExcode(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Запросить внутренний код объекта
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetObjectIncode(HOBJ hobj)
{
  if (GetObjectIncode)
    return GetObjectIncode(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Запросить HRSC
// При ошибке возвращает ноль
// ------------------------------------------------------------------
HRSC imlGetRsc(HOBJ hobj)
{
  if (GetRsc)
    return GetRsc(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Запросить строку с уникальным номером объекта на карте
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlMapObjectKey(HOBJ hobj)
{
  if (MapObjectKey)
    return MapObjectKey(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Запросить строку с номером объекта в листе
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlIsObjectLoading(HOBJ hobj)
{
  if (IsObjectLoading)
    return IsObjectLoading(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Запросить выполняется ли загрузка объекта из базы данных
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetObjectNumber(HOBJ hobj)
{
  if (GetObjectNumber)
    return GetObjectNumber(hobj);
  return 0;
}


// ------------------------------------------------------------------
// Определить идентификатор открытого документа
// для заданного объекта
// При ошибке возвращает ноль
// ------------------------------------------------------------------
HMAP imlGetObjectDocIdent(HOBJ hobj)
{
  if (GetObjectDocIdent)
    return GetObjectDocIdent(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Определить идентификатор открытой пользовательской карты
// для заданного объекта
// hMap  - идентификатор открытой основной карты
// hObj  - идентификатор объекта пользовательской карты
// При ошибке возвращает ноль
// ------------------------------------------------------------------
HSITE imlGetObjectSiteIdent(HMAP hMap,HOBJ hobj)
{
  if (GetObjectSiteIdent)
    return GetObjectSiteIdent(hMap,hobj);
  return 0;
}

// ------------------------------------------------------------------
// Запросить базовый масштаб карты
// hMap   - идентификатор открытой основной карты
// hSite  - идентификатор открытой пользовательской карты
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetSiteScale(HMAP hMap, HSITE hSite)
{
  if (GetSiteScale)
    return GetSiteScale(hMap, hSite);
  return 0;
}

// ------------------------------------------------------------------
// Запросить номер листа для объекта
// info - идентификатор объекта карты в памяти
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetListNumber(HOBJ hobj)
{
  if (GetListNumber)
    return GetListNumber(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Cоздать пустой объект векторной карты
// ------------------------------------------------------------------
HOBJ imlCreateObject(HMAP hMap,long int sheetnumber)
{
  if (CreateObject)
    return CreateObject(hMap,sheetnumber);
  return 0;
}

// ------------------------------------------------------------------
// Удалить объект векторной карты
// При ошибке возвращает ноль
// ------------------------------------------------------------------
void imlFreeObject(HOBJ hobj)
{
  if (FreeObject)
    return FreeObject(hobj);
  return ;
}

// ------------------------------------------------------------------
// Запросить объект "Рамка листа"
// hmap - идентификатор открытых данных
// list - номер листа (c 1)
// info - идентификатор объекта карты в памяти
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetListFrameObject(HMAP hmap, long int list,HOBJ hobj)
{
  if (GetListFrameObject)
    return GetListFrameObject(hmap, list,hobj);
  return 0;
}

// ------------------------------------------------------------------
// Запросить имя листа по его номеру (number)
// hMap  - идентификатор открытых данных
// hSite - идентификатор открытой пользовательской карты
// list  - номер листа карты
// name  - адрес буфера для результата запроса
// size  - размер буфера в байтах
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetSiteSheetName(HMAP hMap, HSITE hSite, long int list, WCHAR *name, long int size)
{
  if (GetSiteSheetNameUn)
    return GetSiteSheetNameUn(hMap,hSite, list,name,size);
  return 0;
}

// ------------------------------------------------------------------
// Запросить номенклатуру листа по его номеру
// Номенклатура листа применяется для поиска в функции mapSeekObject
// hMap  - идентификатор открытых данных
// hSite - идентификатор открытой пользовательской карты
// list  - номер листа карты
// name  - адрес буфера для результата запроса
// size  - размер буфера в байтах
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetSiteNomenclature(HMAP hMap, HSITE hSite, long int list, WCHAR *name, long int size)
{
   if (GetSiteNomenclatureUn)
    return GetSiteNomenclatureUn(hMap,hSite, list,name,size);
  return 0;
}

// ------------------------------------------------------------------
// Запросить имя листа для объекта
// hobj - идентификатор объекта карты в памяти
// name  - адрес буфера для результата запроса
// size  - размер буфера в байтах
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetListNameUn(HOBJ hobj,WCHAR *name, long int size)
{
  if (GetListNameUn)
     return GetListNameUn(hobj,name,size);
  return 0;
}

// ------------------------------------------------------------------
// Запросить номенклатуру листа для объекта
// hobj - идентификатор объекта карты в памяти
// name  - адрес буфера для результата запроса
// size  - размер буфера в байтах
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetNomenclatureUn(HOBJ hobj,WCHAR *name, long int size)
{
  if (GetNomenclatureUn)
     return GetNomenclatureUn(hobj,name,size);
  return 0;
}

// ------------------------------------------------------------------
// Запросить базовый масштаб карты
// hobj - идентификатор объекта карты в памяти
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetObjectMapScale(HOBJ hobj)
{
  if (GetObjectMapScale)
     return GetObjectMapScale(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Запросить строку с символьным значением имени классификатора семантики по
// коду семантики, числовому значению классификатора
// При ошибке возвращает ноль
// ------------------------------------------------------------------
long int imlGetSmClsNameUn(HRSC hRsc, long int code, long int value, WCHAR * buffer, int size)
{
  if (GetSmClsNameUn)
    return GetSmClsNameUn(hRsc,code,value,buffer,size);
  return 0;
}

// ------------------------------------------------------------------
// Площадь объекта
// При ошибке возвращает ноль
// ------------------------------------------------------------------
double imlGetObjectSquare(HOBJ hobj)
{
  if (GetObjectSquare)
    return GetObjectSquare(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Площадь объекта
// При ошибке возвращает ноль
// ------------------------------------------------------------------
double imlGetObjectMapSquare(HOBJ hobj)
{
  if (GetObjectMapSquare)
    return GetObjectMapSquare(hobj);
  return imlGetObjectSquare(hobj);
}

// ------------------------------------------------------------------
// Площадь объекта
// При ошибке возвращает ноль
// ------------------------------------------------------------------
double imlGetObjectLength(HOBJ hobj)
{
  if (GetObjectLength)
    return GetObjectLength(hobj);
  return 0;
}

// ------------------------------------------------------------------
// Запросить точность поля значения семантики
// При ошибке возвращает ноль
// ------------------------------------------------------------------
int imlGetSemanticDecimal(HOBJ hobj, int semcode)
{
  if (GetSemanticEx)
  {
    RSCSEMANTICEX semtype;
    if (GetSemanticEx(hobj, &semtype, semcode) != 0)
      return semtype.Decimal;
  }
  return 0;
}


  /*********************************************************
  *                                                        *
  *        ОБРАБОТКА ГРАФИЧЕСКИХ ПАРАМЕТРОВ ОБ' ЕКТА       *
  *                                                        *
  *********************************************************/

#define DRAWIDENT 0x7FFF7FFE

// ------------------------------------------------------------------
// Запросить адрес описания примитива
// по последовательному номеру (начиная с 1)
// При ошибке возвращает 0
// ------------------------------------------------------------------
const IMGDECOR * _fastcall imlDrawAddress(const IMGDRAW * draw, int number)
{
  if ((draw == 0) || (draw->Ident != DRAWIDENT))
    return 0;

  if ((number < 1) || (number > draw->Count))
    return 0;

  char * point = (char *) &(draw->Image);
  if (number == 1)
    return (const IMGDECOR *)point;

  while (number-- >1)
   {
     int length = ((IMGDECOR *)point)->Length;
     if (length <= 0)
       return 0;
     point += length;
   }

  return (const IMGDECOR *)point;
}

// ------------------------------------------------------------------
// Запросить функцию отображения примитива
// по последовательному номеру (начиная с 1)
// При ошибке возвращает 0
// ------------------------------------------------------------------
int _fastcall imlDrawType(const IMGDRAW * draw, int number)
{
  const IMGDECOR * decor = imlDrawAddress(draw,number);
  if (decor)
    return decor->Number;
  return 0;
}

// ------------------------------------------------------------------
// Запросить адрес параметров примитива
// по последовательному номеру (начиная с 1)
// При ошибке возвращает 0
// ------------------------------------------------------------------
const char * _fastcall imlDrawParm(const IMGDRAW * draw, int number)
{
  const IMGDECOR * decor = imlDrawAddress(draw,number);
  if (decor)
    return (const char *)((char *)decor + sizeof(IMGDECOR));
  return 0;
}

  /*********************************************************
  *                                                        *
  *     ПОСТРОЕНИЕ ТИПОВЫХ ГЕОМЕТРИЧЕСКИХ ПРИМИТИВОВ       *
  *                                                        *
  *********************************************************/

#define CIRCLECOUNT 180

// -------------------------------------------------------
// Построить метрику окружности заданного радиуса
// Заполняем метрику по адресу data (резервируется CIRCLECOUNT точек)
// center - центр окружности в координатах устройства вывода
// radius - радиус окружности в точках устройства
// data   - адрес структуры, описывающей выходные данные
// При ошибке возвращает ноль
// --------------------------------------------------------
int _fastcall imlBuildCircle(DRAWPOINT* center, long radius, POLYDATAEX* data)
{
  int i, ii;
  DRAWPOINT   dpoint[CIRCLECOUNT];
  DRAWPOINT* points;               // Выходные координаты
  DRAWPOINT* dpointex;

  DRAWPOINT* dpoints = &dpoint[0];
  int count = 0;                    // Число точек

  if ((data && center) == 0)
    return 0;

  points = (DRAWPOINT*)(data->Points);

  data->Count = 1;

  if (radius < 1)
  {
    points->X = center->X;
    points->Y = center->Y;
    return (*(data->PolyCounts) = 1);
  }

  if (radius == 1)
  {
    points->X = center->X;
    points->Y = center->Y + 1;
    points++;
    points->X = center->X + 1;
    points->Y = center->Y;
    points++;
    points->X = center->X;
    points->Y = center->Y - 1;
    points++;
    points->X = center->X - 1;
    points->Y = center->Y;
    points++;
    points->X = center->X;
    points->Y = center->Y + 1;
    return (*(data->PolyCounts) = 5);
  }

  // Заполняем восьмую часть
  if (radius <= 10)
  {
    long temp;
    int x = count;
    int y = radius;
    long radius2 = radius * radius;
    while (y > x)
    {
      dpoints->X = x;
      dpoints->Y = y;
      count++;
      dpoints++;

      temp = radius2 - count * count;
      if (temp <= 0)
        break;

      x = count;
      y = (sqrt((double)temp) + 0.3);
    }
  }
  else
  {
    int OCTPOINT;
    double multi;
    if (radius <= 50)
      OCTPOINT = (radius + 2) / 4;      // 18
    else
      OCTPOINT = 15;

    multi = M_PI / (OCTPOINT * 4.0);
    for (i = 0; i <= OCTPOINT; i++)
    {
      dpoints->X = (int)((double)radius * sin(multi * (double)i) + 0.4);
      dpoints->Y = (int)((double)radius * cos(multi * (double)i) + 0.4);
      dpoints++;
    }

    count = OCTPOINT + 1;
  }

  ii = count;
  if (ii >= 1)
  {
    // симметрично размножим четверть
    dpointex = &dpoint[ii];
    while (ii-- > 0)
    {
      dpointex--;
      dpoints->X = dpointex->Y;
      dpoints->Y = dpointex->X;
      dpoints++;
      count++;
    }
  }

  // половина
  ii = count;
  dpointex = &dpoint[ii];
  while (ii-- > 0)
  {
    dpointex--;
    dpoints->X = dpointex->X;
    dpoints->Y = -dpointex->Y;
    dpoints++;
    count++;
  }

  // вся
  ii = count;
  dpointex = &dpoint[ii];
  while (ii-- > 0)
  {
    dpointex--;
    dpoints->X = -dpointex->X;
    dpoints->Y = dpointex->Y;
    dpoints++;
    count++;
  }

  // Переписываем метрику с учетом центра окружности
  dpoints = &dpoint[0];
  for (i = 0; i < count; i++)
  {
    points->X = (int)(dpoints->X + center->X);
    points->Y = (int)(dpoints->Y + center->Y);
    points++;
    dpoints++;
  }

  // Записать число точек
  return (*(data->PolyCounts) = count);
}

// ------------------------------------------------------------
// Построить эллипс
// Заполняем метрику по адресу data (резервируется 120 точек)
// center  - центр эллипса в координатах устройства вывода
// radius  - радиус эллипса в точках устройства по горизонтали
// radiusv - радиус эллипса в точках устройства по вертикали
// data    - адрес структуры, описывающей выходные данные
// При ошибке возвращает ноль
// ------------------------------------------------------------
int _fastcall imlBuildEllipse(DRAWPOINT* center, long radius, long radiusv, POLYDATAEX* data)
{
  int i, ii;
  double scale;
  DRAWPOINT* points;            // Выходные координаты
  DOUBLEPOINT* dpointex;
  DOUBLEPOINT dpoint[CIRCLECOUNT];
  DOUBLEPOINT* dpoints;

  int count = 0;    // Число точек

  if ((data && center) == 0)
    return 0;

  points = (DRAWPOINT*)(data->Points);
  dpoints = &dpoint[0];


  if (radius < 2)
  {
    points->X = center->X;
    points->Y = center->Y;
    return (*(data->PolyCounts) = 1);
  }

  // Заполняем восьмую часть
  if (radius <= 10)
  {
    double x = count;
    double y = radius;
    long radius2 = radius * radius;
    while (y > x)
    {
      long temp;
      dpoints->X = x;
      dpoints->Y = y;
      count++;
      dpoints++;

      temp = radius2 - count * count;
      if (temp <= 0)
        break;

      x = count;
      y = sqrt((double)temp);
    }
  }
  else
  {
    int OCTPOINT;
    double multi;
    if (radius <= 50) OCTPOINT = (radius + 2) / 4;
    else
      OCTPOINT = 15;

    multi = M_PI / (OCTPOINT * 4.0);
    for (i = 0; i <= OCTPOINT; i++)
    {
      dpoints->X = (double)radius * sin(multi * (double)i);
      dpoints->Y = (double)radius * cos(multi * (double)i);
      dpoints++;
    }
    count = OCTPOINT + 1;
  }

  ii = count;
  if (ii >= 1)
  {
    // симметрично размножим четверть
    dpointex = &dpoint[ii - 1];
    while (ii-- > 0)
    {
      dpoints->X = dpointex->Y;
      dpoints->Y = dpointex->X;
      dpoints++;
      dpointex--;
      count++;
    }
  }

  // половина
  ii = count;
  dpointex = &dpoint[ii - 1];
  while (ii-- > 0)
  {
    dpoints->X = dpointex->X;
    dpoints->Y = -dpointex->Y;
    dpoints++;
    dpointex--;
    count++;
  }

  // вся
  ii = count;
  dpointex = &dpoint[ii - 1];
  while (ii-- > 0)
  {
    dpoints->X = -dpointex->X;
    dpoints->Y = dpointex->Y;
    dpoints++;
    dpointex--;
    count++;
  }

  scale = (double)radiusv / (double)radius;

  // Переписываем метрику с учетом центра окружности
  dpoints = &dpoint[0];
  for (i = 0; i < count; i++)
  {
    points->X = (int)(dpoints->X + 0.4) + center->X;
    points->Y = (int)(dpoints->Y * scale + 0.4) + center->Y;
    points++;
    dpoints++;
  }

  // Записать число точек
  return (*(data->PolyCounts) = count);
}

// ------------------------------------------------------------
// Построить метрику части окружности заданного радиуса
// Заполняем метрику по адресу data (резервируется 120 точек)
// first   - координаты первой точки дуги
// center  - центр дуги в координатах устройства вывода
// last    - координаты второй точки дуги
// radius  - радиус дуги в точках устройства
// При ошибке возвращает ноль, иначе - число точек в построении
// ------------------------------------------------------------
int _fastcall imlBuildArc(DRAWPOINT* first, DRAWPOINT* center, DRAWPOINT* last, long int radius, POLYDATAEX* data)
{
  if ((data && center) == 0)
    return 0;

  int count;
  DRAWPOINT* pointout;            // Выходные координаты

  // Нашли точку входа в окружность
  long xin, yin, xout, yout;

  long   dx;
  long   dy;
  double delta;
  double temp;
  double min;
  double currentmin;
  int k;
  DRAWPOINT* currentpoint;
  DRAWPOINT points[CIRCLECOUNT];

  POLYDATAEX circdata;
  int pointcount = 0;

  int arccount = 1;

  // Нашли номер первой точки на окружности
  int indexin;
  int indexout;

  DRAWPOINT* point = &points[0];

  // Подготовить структуру данных
  circdata.Count = 1;
  circdata.PolyCounts = &pointcount;
  circdata.Points = &points[0];

  pointout = (DRAWPOINT*)(data->Points);

  if (radius < 2)
  {
    pointout->X = center->X;
    pointout->Y = center->Y;
    return (*(data->PolyCounts) = 1);
  }

  // Нет входной линии
  if ((first->X == center->X) && (first->Y == center->Y))
  {
    if ((first->X == last->X) && (first->Y == last->Y))
      return (*(data->PolyCounts) = 0); // Все точки совпадают

    if ((center->X == last->X) && (center->Y == last->Y))
      return (*(data->PolyCounts) = 0); // Нет выходной линии

    first->X = 2 * center->X - last->X;
    first->Y = 2 * center->Y - last->Y;
  }
  else
  {
    // Нет выходной линии
    if ((last->X == center->X) && (last->Y == center->Y))
    {
      last->X = 2 * center->X - first->X;
      last->Y = 2 * center->Y - first->Y;
    }
  }

  // Построили окружность данного радиуса
  count = imlBuildCircle(center, radius, &circdata);
  indexout = count - 1;

  if (count > 140)
    count = 140;

  dx = first->X - center->X;
  dy = first->Y - center->Y;
  delta = ((double)radius) / sqrt(((double)dx) * dx + ((double)dy) * dy + DELTANULL);

  temp = center->X + (double)dx * delta;
  if (temp >= 0)
    xin = (long)(temp + 0.5);
  else
    xin = (long)(temp - 0.5);

  temp = center->Y + (double)dy * delta;
  if (temp >= 0)
    yin = (long)(temp + 0.5);
  else
    yin = (long)(temp - 0.5);

  pointout->X = xin;
  pointout->Y = yin;
  pointout++;

  min = 10.0 * ((double)radius) * radius;  // квадрат максимального расстояния между точками окружности
  k = 0;
  for (indexin = 0; indexin < indexout; indexin++, point++)
  {
    currentmin = ((double)(xin - point->X)) * (xin - point->X) + ((double)(yin - point->Y)) * (yin - point->Y);
    if (currentmin < min)
    {
      min = currentmin;
      k = indexin;
    }
  }

  // проверим, что точка лежит "внутри" требуемой дуги
  if (k == 0)
  {
    indexin = count - 1;
    point = &points[0];
    currentpoint = &points[indexin];
  }
  else
  {
    indexin = k - 1;
    currentpoint = &points[indexin];
    point = &points[indexin + 1];
  }

  if ((((double)xin - currentpoint->X) * (xin - currentpoint->X) + ((double)yin - currentpoint->Y) * (yin - currentpoint->Y)) >
    (((double)point->X - currentpoint->X) * (point->X - currentpoint->X) + ((double)point->Y - currentpoint->Y) * (point->Y - currentpoint->Y)))
    indexin++;

  if (indexin == count)
    indexin = 0;

  // Нашли точку выхода из окружности
  dx = last->X - center->X;
  dy = last->Y - center->Y;
  delta = (double)radius / sqrt((double)dx * dx + (double)dy * dy + DELTANULL);
  temp = (double)center->X + (double)dx * delta;
  if (temp >= 0)
    xout = (long)(temp + 0.5);
  else
    xout = (long)(temp - 0.5);

  temp = (double)center->Y + (double)dy * delta;
  if (temp >= 0)
    yout = (long)(temp + 0.5);
  else
    yout = (long)(temp - 0.5);

  // Нашли номер последней точки на окружности
  point = &points[count - 1];
  min = 10.0 * ((double)radius) * radius;  // квадрат максимального расстояния между точками окружности
  k = 0;
  for (indexout = count - 1; indexout > 0; indexout--, point--)
  {
    currentmin = (((double)xout - point->X)) * (xout - point->X) + (((double)yout - point->Y)) * (yout - point->Y);
    if (currentmin <= min)
    {
      min = currentmin;
      k = indexout;
    }
  }

  if (k == count - 1)
  {
    indexout = 0;
    point = &points[count - 1];
    currentpoint = &points[0];
  }
  else
  {
    indexout = k + 1;
    currentpoint = &points[indexout];
    point = &points[indexout - 1];
  }

  // проверим, что точка лежит "внутри" требуемой дуги
  if (((((double)xout - currentpoint->X)) * (xout - currentpoint->X) + (((double)yout - currentpoint->Y)) * (yout - currentpoint->Y)) >
    ((((double)point->X - currentpoint->X)) * (point->X - currentpoint->X) + (((double)point->Y - currentpoint->Y)) * (point->Y - currentpoint->Y)))
  {
    if (indexout > 0)
      indexout--;
    else
      indexout = count - 1;
  }

  // Записали на место - первая точка входа  - метрика -
  // последняя точка выхода
  point = &points[indexin];
  if (indexout <= indexin)
  {
    arccount += (indexin - indexout + 1);

    // Выбрать точки подряд
    while (indexin >= indexout)
    {
      pointout->X = point->X;
      pointout->Y = point->Y;
      indexin--;
      point--;
      pointout++;
    }
  }
  else
  {
    arccount += (indexin + 1 + count - indexout);
    int in = indexin;

    point = &points[indexin];
    while (indexin-- >= 0)
    {
      pointout->X = point->X;
      pointout->Y = point->Y;
      point--;
      pointout++;
    }

    point = &points[count - 1];
    if (in >= 0)
    {
      // Пропустить повторяющуюся точку
      DRAWPOINT* lastpoint = pointout - 1;
      if ((lastpoint->X == point->X) && (lastpoint->Y == point->Y))
      {
        point--;
        count--;
        arccount--;
      }
    }

    while (count-- > indexout)
    {
      pointout->X = point->X;
      pointout->Y = point->Y;
      point--;
      pointout++;
    }
  }

  if (arccount > 0)
  {
    // Корректировать близкую точку
    DRAWPOINT* lastpoint = pointout - 1;

    if ((lastpoint->X == xout) || (lastpoint->Y == yout))
    {
      pointout--;
      arccount--;
    }
  }

  pointout->X = xout;
  pointout->Y = yout;
  arccount++;

  // Записать число точек
  return (*(data->PolyCounts) = arccount);
}

//----------------------------------------------------------------
// Положение (сторона) точки относительно прямой
// Возвращает imlLEFT, imlRIGHT
// При ошибке возвращает 0
//----------------------------------------------------------------
int _fastcall imlPointSideNearLine(DOUBLEPOINT *point, DOUBLEPOINT *xy1, DOUBLEPOINT *xy2)
{
  double ax,ay,bx,by;

  if (xy1 == 0 || xy2 == 0 || point == 0)
    return 0;

  // **************************************************************
  // Относительное положение точки и прямой линии
  // (Майкл Ласло. Вычислительная геометрия и компьютерная графика
  //  на С++, стр.96, программа Point::classify)
  // **************************************************************

  ax = xy2->X - xy1->X;
  ay = xy2->Y - xy1->Y;

  bx = point->X - xy1->X;
  by = point->Y - xy1->Y;

  // Точка слева
  if (ax*by - bx*ay > 0)
    return imlLEFT;

  // Точка справа
  return imlRIGHT;
}

int _fastcall imlPointSideNearLineDraw(DRAWPOINT *point, DRAWPOINT *xy1, DRAWPOINT *xy2)
{
  double ax,ay,bx,by;

  if (xy1 == 0 || xy2 == 0 || point == 0)
    return 0;

  // **************************************************************
  // Относительное положение точки и прямой линии
  // (Майкл Ласло. Вычислительная геометрия и компьютерная графика
  //  на С++, стр.96, программа Point::classify)
  // **************************************************************

  ax = xy2->X - xy1->X;
  ay = xy2->Y - xy1->Y;

  bx = point->X - xy1->X;
  by = point->Y - xy1->Y;

  // Точка слева
  if (ax*by - bx*ay > 0)
    return imlLEFT;

  // Точка справа
  return imlRIGHT;
}

// --------------------------------------------------------------
// Определение центра окружности по трем точкам
//  pc - точка центра
// Возвращает радиус окружности (0.0 - три точки совпадают,
//                              -1.0 - точки на одной прямой)
// --------------------------------------------------------------
double _fastcall imlCircleCenter(DOUBLEPOINT* p1, DOUBLEPOINT* p2, DOUBLEPOINT* p3, DOUBLEPOINT* pc)
{

  // **************************************************************
  // Уравнение прямой по двум точкам (x1,y1),(x2,y2)
  //
  //   {a*x1 + b = y1   { a*x1 + b =  y1    a * (x1-x2) = (y1-y2)
  //   {a*x2 + b = y2   {-a*x2 - b = -y2    a = (y1-y2) / (x1-x2)
  //
  // Уравнение прямой, перпендикулярной заданной и проходящей через
  // середину отрезка заданного двумя точками (x1,y1),(x2,y2)
  //
  //   A*Xm + B = Ym    A = -1/a = (x2-x1)/(y1-y2)
  //                    B = Ym - A*Xm    Xm = (x1+x2)/2
  //                                     Ym = (y1+y2)/2
  //                    B = ((y1+y2)-(x1+x2)*A)/2;
  //
  // Центр окружности - точка пересечения прямых, перпендикулярных
  // к отрезкам (x1,y1),(x2,y2) и (x2,y2),(x3,y3) и проходящих через
  // середины соответствующих отрезков
  //
  //   {A12*Xc + B12 = Yc   A12*Xc + B12 = A23*Xc + B23
  //   {A23*Xc + B23 = Yc   Xc * (A12-A23) = (B23-B12)
  //
  //                        Xc = (B23-B12)/(A12-A23)
  //                        Yc = B23 - A23*Xc
  // **************************************************************

  double x1, y1, x2, y2, x3, y3;
  double A12, B12;
  double A23, B23;
  int ex12, ex23, ex31, ey12, ey23, ey31;
  int equal12, equal23, equal31;

  if (p1 == 0 || p2 == 0 || p3 == 0 || pc == 0)
    return 0.0;

  x1 = p1->X;
  y1 = p1->Y;
  x2 = p2->X;
  y2 = p2->Y;
  x3 = p3->X;
  y3 = p3->Y;

  // Равенство координат
  ex12 = (fabs(x1 - x2) < DELTANULL);
  ex23 = (fabs(x2 - x3) < DELTANULL);
  ex31 = (fabs(x3 - x1) < DELTANULL);
  ey12 = (fabs(y1 - y2) < DELTANULL);
  ey23 = (fabs(y2 - y3) < DELTANULL);
  ey31 = (fabs(y3 - y1) < DELTANULL);

  // Равенство точек
  equal12 = (ex12 != 0) && (ey12 != 0);
  equal23 = (ex23 != 0) && (ey23 != 0);
  equal31 = (ex31 != 0) && (ey31 != 0);

  if (equal12)
  {
    // Все точки совпадают
    if (equal23)
    {
      *pc = *p1;
      return 0.0;
    }

    // (p1,p3) - диаметр окружности
    pc->X = (x1 + x3) / 2;
    pc->Y = (y1 + y3) / 2;
    return imlDistancePoint(pc, p1);
  }
  else
    if (equal23)
    {
      // (p1,p3) - диаметр окружности
      pc->X = (x1 + x3) / 2;
      pc->Y = (y1 + y3) / 2;
      return imlDistancePoint(pc, p1);
    }
    else
      if (equal31)
      {
        // (p1,p2) - диаметр окружности
        pc->X = (x1 + x2) / 2;
        pc->Y = (y1 + y2) / 2;
        return imlDistancePoint(pc, p1);
      }

  // (p1,p2) - горизонтальная линия
  if (ey12)
  {
    // Три точки на одной прямой
    if (ey23)
      return -1.0;

    if (ex23)       // (p2,p3) - вертикальная линия
    {
      // (p1,p3) - диаметр окружности
      pc->X = (x1 + x3) / 2;
      pc->Y = (y1 + y3) / 2;
      return imlDistancePoint(pc, p1);
    }
    else
      if (ex31)  // (p3,p1) - вертикальная линия
      {
        // (p2,p3) - диаметр окружности
        pc->X = (x2 + x3) / 2;
        pc->Y = (y2 + y3) / 2;
        return imlDistancePoint(pc, p1);
      }

    A23 = (x3 - x2) / (y2 - y3);
    B23 = ((y2 + y3) - (x2 + x3) * A23) / 2;

    // Перпендикуляр - вертикальная линия между точками p1,p2
    pc->X = (x1 + x2) / 2;
    pc->Y = B23 + A23 * pc->X;
    return imlDistancePoint(pc, p1);
  }

  // (p2,p3) - горизонтальная линия
  if (ey23)
  {
    if (ex12)       // (p1,p2) - вертикальная линия
    {
      // (p1,p3) - диаметр окружности
      pc->X = (x1 + x3) / 2;
      pc->Y = (y1 + y3) / 2;
      return imlDistancePoint(pc, p1);
    }
    else
      if (ex31)  // (p3,p1) - вертикальная линия
      {
        // (p1,p2) - диаметр окружности
        pc->X = (x1 + x2) / 2;
        pc->Y = (y1 + y2) / 2;
        return imlDistancePoint(pc, p1);
      }

    A12 = (x2 - x1) / (y1 - y2);
    B12 = ((y1 + y2) - (x1 + x2) * A12) / 2;

    // Перпендикуляр - вертикальная линия между точками p2,p3
    pc->X = (x2 + x3) / 2;
    pc->Y = B12 + A12 * pc->X;
    return imlDistancePoint(pc, p1);
  }

  // (p1,p2) - вертикальная линия
  if (ex12)
  {
    // Три точки на одной прямой
    if (ex23)
      return -1.0;

    if (ey23)       // (p2,p3) - горизонтальная линия
    {
      // (p1,p3) - диаметр окружности
      pc->X = (x1 + x3) / 2;
      pc->Y = (y1 + y3) / 2;
      return imlDistancePoint(pc, p1);
    }
    else
      if (ey31)  // (p3,p1) - горизонтальная линия
      {
        // (p2,p3) - диаметр окружности
        pc->X = (x2 + x3) / 2;
        pc->Y = (y2 + y3) / 2;
        return imlDistancePoint(pc, p1);
      }

    A23 = (x3 - x2) / (y2 - y3);
    B23 = ((y2 + y3) - (x2 + x3) * A23) / 2;

    // Перпендикуляр - горизонтальная линия между точками p1,p2
    pc->Y = (y1 + y2) / 2;
    pc->X = (pc->Y - B23) / A23;
    return imlDistancePoint(pc, p1);
  }

  // (p2,p3) - вертикальная линия
  if (ex23)
  {
    if (ey12)       // (p1,p2) - горизонтальная линия
    {
      // (p1,p3) - диаметр окружности
      pc->X = (x1 + x3) / 2;
      pc->Y = (y1 + y3) / 2;
      return imlDistancePoint(pc, p1);
    }
    else
      if (ey31)  // (p3,p1) - горизонтальная линия
      {
        // (p1,p2) - диаметр окружности
        pc->X = (x1 + x2) / 2;
        pc->Y = (y1 + y2) / 2;
        return imlDistancePoint(pc, p1);
      }

    A12 = (x2 - x1) / (y1 - y2);
    B12 = ((y1 + y2) - (x1 + x2) * A12) / 2;

    // Перпендикуляр - горизонтальная линия между точками p2,p3
    pc->Y = (y2 + y3) / 2;
    pc->X = (pc->Y - B12) / A12;
    return imlDistancePoint(pc, p1);
  }

  A12 = (x2 - x1) / (y1 - y2);
  B12 = ((y1 + y2) - (x1 + x2) * A12) / 2;

  A23 = (x3 - x2) / (y2 - y3);
  B23 = ((y2 + y3) - (x2 + x3) * A23) / 2;

  // Три точки на одной прямой
  if (fabs(A12 - A23) < DELTANULL)
    return -1.0;

  pc->X = (B23 - B12) / (A12 - A23);
  pc->Y = B23 + A23 * pc->X;
  return imlDistancePoint(pc, p1);
}

//----------------------------------------------------------------
// Вычисление расстояния между двумя точками
//----------------------------------------------------------------
double _fastcall imlDistancePoint(DOUBLEPOINT *xy1, DOUBLEPOINT *xy2)
{
  double dx,dy;

  if (xy1 == 0 || xy2 == 0)
    return 0;

  if ((dx = xy1->X - xy2->X) < 0)
    dx = -dx;
  if ((dy = xy1->Y - xy2->Y) < 0)
    dy = -dy;

  if (dx < DOUBLENULL)
    return dy;
  if (dy < DOUBLENULL)
    return dx;

  return sqrt(dx*dx + dy*dy);
}

double _fastcall imlDistancePointDraw(DRAWPOINT *xy1, DRAWPOINT *xy2)
{
  double dx,dy;

  if (xy1 == 0 || xy2 == 0)
    return 0;

  if ((dx = xy1->X - xy2->X) < 0) dx = -dx;
  if ((dy = xy1->Y - xy2->Y) < 0) dy = -dy;

  if (dx < DOUBLENULL)
    return dy;
  if (dy < DOUBLENULL)
    return dx;

  return sqrt(dx*dx + dy*dy);
}

//----------------------------------------------------------------
// Определить положение точки на отрезке, делящей отрезок
// в отношении m1/m2
// При ошибке возвращает 0
//----------------------------------------------------------------
int _fastcall imlPointPositionOnCut(DOUBLEPOINT *point, DOUBLEPOINT *p1, DOUBLEPOINT *p2, double m1, double m2)
{
  double m = m1+m2;
  if (p1 == 0 || p2 == 0 || point == 0)
    return 0;

  // **************************************************************
  // (Дьяконов В.П. Справочник по расчетам на микрокалькуляторах,
  //  стр.155, Программа 3.52.)
  //   x = (m2*x1 + m1*x2) / (m1+m2)
  //   y = (m2*y1 + m1*y2) / (m1+m2)
  // **************************************************************

  if (m < DELTANULL)
  {
    *point = *p1;
    return 0;
  }

  point->X = (m2*p1->X + m1*p2->X) / m;
  point->Y = (m2*p1->Y + m1*p2->Y) / m;
  return 1;
}

// ---------------------------------------------------------
// Запись вещественного числа в символьном виде с фиксированной точкой
// со вставкой разделяющих пробелов (разделение на тройки символов
// от десятичной точки к началу строки)
// ---------------------------------------------------------
void SpaceInsert(char* string, int size, const char* format, double number)
{
  if ((string && format) == 0)
    return;

  Snprintf(string, size, format, number);
  int length = strlen(string);

  int i, j, k;
  char b[256];

  // Формирование временной строки от конца заданной до десятичной точки
  for (i = 0, k = length - 1; string[k] != '.' && string[k] != ',' && k >= 0; i++, k--)
  {
    b[i] = string[k];
  }

  b[i] = ',';
  i++;
  k--;

  // Формирование временной строки от десятичной точки
  // со вставкой разделяющих пробелов
  for (j = 0; k >= 0; j++, i++)
  {
    if ((j + 1) % 4)
    {
      b[i] = string[k];
      k--;
    }
    else
      b[i] = ' ';
  }

  // Формирование строки с конца временной (переворот)
  for (k = 0; k < i; k++)
    string[k] = b[i - k - 1];

  string[k] = 0;
}

// ---------------------------------------------------------
// Формирование строки со вставкой разделяющих пробелов
// ---------------------------------------------------------
void InsertSeparatingBlanks(char* string)
{
  int  i, k;
  char b[256];

  if (string == 0)
    return;

  // Формирование временной строки от конца заданной со вставкой разделяющих пробелов
  int length = strlen(string);

  for (i = 0, k = length - 1; k >= 0; i++)
    if ((i + 1) % 4)
    {
      b[i] = string[k];
      k--;
    }
    else b[i] = ' ';

  // Формирование строки с конца временной (переворот)
  for (k = 0; k < i; k++)
    string[k] = b[i - k - 1];

  string[k] = 0;
}

// ---------------------------------------------------------
// Открыть данные с автоматическим определением их типа
// (векторные,растровые,матричные...)
// name - имя открываемого файла (MAP, SIT, MTW, RSW, MPT) в кодировке UNICODE
// mode - режим чтения/записи (GENERIC_READ, GENERIC_WRITE или 0)
// GENERIC_READ - все данные только на чтение, при этом не открываются
// файлы \Log\name.log и \Log\name.tac - протокол работы и журнал транзакций
// При ошибке возвращает ноль
// ---------------------------------------------------------
HMAP _fastcall imlOpenDataUn(const WCHAR * name, int mode)
{
  if (OpenDataUn)
    return OpenDataUn(name, mode);
  return 0;
}

HMAP _fastcall imlOpenDataRef(HMAP hmap, HSITE hsite, const WCHAR * name, int mode, long int * error)
{
  if (OpenDataRef)
    return OpenDataRef(hmap, hsite, name, mode, error);
  return 0;
}

// ---------------------------------------------------------
// Закрыть все данные электронной карты
// hmap -  идентификатор открытых данных
// Идентификатор HMAP становится недействительным !
// ---------------------------------------------------------
void _fastcall imlCloseData(HMAP hmap)
{
  if (CloseData)
    CloseData(hmap);
}

// ---------------------------------------------------------
// Установить масштаб отображения (знаменатель масштаба)
// hmap - идентификатор открытых данных
// x, y - координаты предполагаемого "центра изображения"
//       (любой точки привязки) в окне в текущем масштабе
// scale - реальный масштаб отображения, который желают получить
// Возвращает:   0 - масштаб не изменился,  1 - масштаб изменился
// ---------------------------------------------------------
int _fastcall imlSetViewScalePro(HMAP hmap, long int *x, long int *y, float scale, HPAINT hPaint)
{
  if (SetViewScalePro)
    return SetViewScalePro(hmap, x, y, scale, hPaint);

  return 0;
}

// ---------------------------------------------------------
// Установить масштаб отображения (знаменатель масштаба)
// hmap - идентификатор открытых данных
// scale - реальный масштаб отображения, который желают получить
// horpix, verpix - разрешение устройства вывода
// Возвращает:   0 - масштаб не изменился,  1 - масштаб изменился
// ---------------------------------------------------------
int _fastcall imlSetViewScalePrint(HMAP hmap, float scale, HPAINT hPaint, double * horpix, double * verpix)
{
  if (SetViewScalePrint)
    return SetViewScalePrint(hmap, scale, hPaint, horpix, verpix);

  return 0;
}

// ---------------------------------------------------------
// Запросить базовый масштаб карты
// hmap - идентификатор открытых данных
// При ошибке возвращает ноль
// ---------------------------------------------------------
int _fastcall imlGetMapScale(HMAP hmap)
{
  if (GetMapScale)
    return GetMapScale(hmap);
  return 0;
}

// ---------------------------------------------------------
// Запросить точный масштаб отображения карты
// hmap - идентификатор открытых данных
// Возвращает значение знаменателя масштаба
// ---------------------------------------------------------
double _fastcall imlGetRealShowScale(HMAP hmap)
{
  if (GetRealShowScale)
    return GetRealShowScale(hmap);
  return 0;
}

// ---------------------------------------------------------
// Запросить размеры общего изображения карты в пикселах
// для текущего масштаба
// hmap - идентификатор открытых данных
// В переменную width заносится ширина изображения (dx),
// в переменную height - высота (dy)
// ---------------------------------------------------------
void _fastcall imlGetPictureSize(long int *width, long int *height, HPAINT hpaint)
{
  if (GetPictureSize)
    GetPictureSize(width, height, hpaint);
}

// ---------------------------------------------------------
// Преобразование из геодезических координат в радианах
// (общеземной эллипсоид WGS84)
// в метры на местности в проекции карты
// (поддерживается не для всех карт !)
// hmap - идентификатор открытых данных
// Bx,Ly,H  - преобразуемые координаты
// на входе радианы, на выходе - метры
// При ошибке возвращает ноль
// ---------------------------------------------------------
int _fastcall imlGeoWGS84ToPlane3D(HMAP hmap, double *Bx, double *Ly, double *H)
{
  if (GeoWGS84ToPlane3D)
    return GeoWGS84ToPlane3D(hmap, Bx, Ly, H);
  return 0;
}

// ---------------------------------------------------------
// Преобразование из метров на местности в пикселы на
// изображении
// hmap - идентификатор открытых данных
// x,y  - преобразуемые координаты
// ---------------------------------------------------------
void _fastcall imlPlaneToPicturePro(double *x, double *y, HPAINT hPaint)
{
  if (PlaneToPicturePro)
    PlaneToPicturePro(x, y, hPaint);
}

// ---------------------------------------------------------
// Отобразить фрагмент карты на заданном устройстве
// в текущих масштабе и составе объектов
// hmap  - идентификатор открытых данных
// hdc   - идентификатор контекста устройства вывода,
// erase - признак стирания фона перед выводом,
//        (0 - фон не стирать, !=0 - очистить фрагмент цветом фона,
//        для экранного способа вывода (VT_SCREEN) всегда стирает
//        цветом фона, кроме значения -2 (минус 2))
// rect - координаты фрагмента карты (Draw) в изображении (Picture)
// alpha - флаг использования альфа канала 0 - не использовать 1 - использовать
// При ошибке возвращает ноль
// ---------------------------------------------------------
int _fastcall imlPaint95Pro(HMAP hmap, HDC hdc, int erase, RECT *rect, int alpha, HPAINT hPaint)
{
  if (Paint95Pro)
    return Paint95Pro(hmap, hdc, erase, rect, alpha, hPaint);
  return 0;
}

// ---------------------------------------------------------
// Вывести изображение карты в XImage (массив)
// Данная функция реализована для XWindow !
// Описание структуры XIMAGEDESC в maptype.h
// hmap - идентификатор открытых данных
// x,y - координаты левого верхнего угла внутри
// битовой области XImage для размещения изображения
// rect - выводимый фрагмент карты
// При ошибке в параметрах возвращает ноль
// ---------------------------------------------------------
int _fastcall imlPaintToXImagePro(HMAP hmap, XIMAGEDESC *imagedesc, int errase,
                                  long int x, long int y, RECT *rect, HPAINT hPaint)
{
  if (PaintToXImagePro)
    return PaintToXImagePro(hmap, imagedesc, errase, x, y, rect, hPaint);
  return 0;
}

// ---------------------------------------------------------
// Запросить/Установить способ отображения карты
// hmap - идентификатор открытых данных
// При установке нового способа отображения возвращается
// предыдущее значение
// (см. Maptype.h : VT_SCREEN(1), VT_PRINT(3), VT_PRINTRST(6),...)
// При ошибке возвращает ноль
// ---------------------------------------------------------
long int _fastcall imlSetViewTypePro(HMAP hmap, long int type, HPAINT hPaint)
{
  if (SetViewTypePro)
    return SetViewTypePro(hmap, type, hPaint);
  return 0;
}

// ---------------------------------------------------------
// Установить степень прозрачности карты (от 0 до 100)
// hMap  - идентификатор открытой карты
// hSite - идентификатор открытой пользовательской карты
// transparent - степень прозрачности карты от 0 (карта не видна)
// до 100 (карта не прозрачная)
// Возвращает новое значение флага
// ---------------------------------------------------------
int _fastcall imlSetSiteTransparent(HMAP hmap, HSITE hsite, int transparent)
{
  if (SetSiteTransparent)
    return SetSiteTransparent(hmap, hsite, transparent);
  return 0;
}

// ---------------------------------------------------------
// Установить цвет фона отображаемой карты
// hmap - идентификатор открытых данных
// color - цвет фона
// ---------------------------------------------------------
COLORREF _fastcall imlSetBackColor(HMAP hmap, COLORREF color)
{
  if (SetBackColor)
    return SetBackColor(hmap, color);
  return 0;
}

// ---------------------------------------------------------
// Запросить открыта ли карта на сервере или локально
// hmap -  идентификатор открытых данных
// hsite - идентификатор открытой пользовательской карты
// (для фоновой (основной) карты hsite = hmap)
// Если карта открыта на сервере возвращает ненулевое значение
// ---------------------------------------------------------
int _fastcall imlIsSiteFromServer(HMAP hmap, HSITE hsite)
{
  if (IsSiteFromServer)
    return IsSiteFromServer(hmap, hsite);
  return 0;
}

// ---------------------------------------------------------
// Запросить идентификатор открытых данных, для которых создан контекст отображения
// При ошибке возвращает ноль
// ---------------------------------------------------------
HMAP _fastcall imlGetMapHandle(HPAINT hpaint)
{
  if (hpaint == 0)
    return 0;
  if (GetMapHandle == 0)
    return (HMAP)hpaint;

  return GetMapHandle(hpaint);
}

// ---------------------------------------------------------
// Записать семантику объекта в символьном виде
// если такая семантика есть и она не повторяемая - заменяет значение семантики
// При ошибке возвращает ноль
// info - идентификатор объекта карты в памяти
// code - код семантики
// value - буфер со значением семантики
// size - размер буфера
// При ошибке возвращает ноль
// ---------------------------------------------------------
int _fastcall imlSetSemantic(HOBJ info, int code, const WCHAR *value, long int size)
{
  if (info == 0)
    return 0;
  if (SetSemantic == 0)
    return 0;

  return SetSemantic(info, code,value,size);
}

// -----------------------------------------------------------------
// Запросить название семантики по коду в кодировке UTF-16
// hRsc - идентификатор классификатора карты
// code - код семантики
// name - адрес строки для размещения результата
// size - максимальный размер выходной строки в байтах (может быть до 2048 байт)
// При ошибке возвращает ноль
// -----------------------------------------------------------------
int _fastcall imlGetRscSemanticNameUn(HRSC hrsc, int code, WCHAR* name, int size)
{
  if (GetRscSemanticNameUn == 0)
    return 0;

  return GetRscSemanticNameUn(hrsc, code, name, size);
}

// -----------------------------------------------------------------
// Проверить совпадение кода семантики с кодами семантик в параметрах
// При ошибке возвращает ноль
// -----------------------------------------------------------------
int _fastcall imlCheckImageSemantic(int number1, int number2, const IMGDRAW* draw, long int code)
{
  if (draw == 0)
    return 0;

  if (number1 && (imlDrawType(draw, number1) == IMG_SEMANTIC))
  {
    IMGLONG* parm = (IMGLONG*)imlDrawParm(draw, number1);
    if (parm)
    {
      if (code == parm->Value)
        return 1;
    }
  }

  if (number2 && (imlDrawType(draw, number2) == IMG_SEMANTIC))
  {
    IMGLONG* parm = (IMGLONG*)imlDrawParm(draw, number2);
    if (parm)
    {
      if (code == parm->Value)
        return 1;
    }
  }

  return 0;
}

// -----------------------------------------------------------------
// Записать семантику в список
// При ошибке возвращает ноль
// -----------------------------------------------------------------
int imlGetImageSemantic(int number1, int number2, const IMGDRAW * draw, long int * list, long int * count)
{
   if (draw == 0)
     return 0;

   int semcode  = 0;
   int semcode2 = 0;

   int semcount = 0;

   if (imlDrawType(draw, number1) == IMG_SEMANTIC)
   {
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, number1);
     if (parm)
       semcode = parm->Value;
   }

   if (semcode != 0)
     semcount++;
   if (number2 && (imlDrawType(draw, 3) == IMG_SEMANTIC))
   {
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, number2);
     if (parm)
       semcode2 = parm->Value;
   }

   if (semcode2 != 0)
     semcount++;

   if ((list == 0) || (count == 0) || (*count <= semcount))
     return -semcount;

   if (semcode != 0)
     *list++ = semcode;
   if (semcode2 != 0)
     *list++ = semcode2;
   return semcount;
}

HPAINT imlCreatePaintControl(HMAP hmap, HPAINT hpaint)
{
  if (CreatePaintControlEx)
    return CreatePaintControlEx(hmap, hpaint);

  if (CreatePaintControl)
    return CreatePaintControl(hmap);

  if (IsDiagnostics && imlIsDiagnostics() && WriteToDiagnosticsLog)
    imlWriteToDiagnosticsLog("imlCreatePaintControl: function does not load - ", "CreatePaintControlEx/CreatePaintControl");

  return 0;
}

void imlFreePaintControl(HPAINT hPaint)
{
  if (FreePaintControl)
  {
    FreePaintControl(hPaint);
    return;
  } 

  if(IsDiagnostics && imlIsDiagnostics() && WriteToDiagnosticsLog)
    imlWriteToDiagnosticsLog("imlFreePaintControl: function does not load - ", "FreePaintControl");
}

void imlWriteToDiagnosticsLog(const char * message, const char * messageex, long int type)
{
  WCHAR w_message[1024];
  WCHAR w_messageex[1024];
  StringToUnicode(message, w_message, sizeof(w_message));
  StringToUnicode(messageex, w_messageex, sizeof(w_messageex));
  imlWriteToDiagnosticsLog(w_message, w_messageex, type);
}

void imlWriteToDiagnosticsLog(const char * message, const WCHAR * messageex, long int type)
{
  WCHAR w_message[1024];
  StringToUnicode(message, w_message, sizeof(w_message));
  imlWriteToDiagnosticsLog(w_message, messageex, type);
}

void imlWriteToDiagnosticsLog(const WCHAR * message, const WCHAR * messageex, long int type)
{
  if (WriteToDiagnosticsLog)
    WriteToDiagnosticsLog(message, messageex, type);
}

long int imlIsDiagnostics()
{
  if (IsDiagnostics)
    return IsDiagnostics();

  return 0;
}

const char * imlGetDocumentFromSitz(HPAINT hpaint, const WCHAR * name, long int * size, long int * error)
{
  if (GetDocumentFromSitz)
    return GetDocumentFromSitz(hpaint, name, size, error);

  return 0;
}

void imlFreeDocumentFromSitz(const char * memory)
{
  if (FreeDocumentFromSitz)
    FreeDocumentFromSitz(memory);
}

