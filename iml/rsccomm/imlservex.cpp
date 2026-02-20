#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imlservex.h"

#ifndef MAPGDI_H
#include "mapgdi.h"
#endif

#include "math.h"
#include <stdio.h>
#include <unordered_map>

#if !defined(__linux__)
#include <tchar.h>
#endif

#include "svgbuilder.h"
#include "rsc.h"

/*
typedef HRSC(WINAPI * OPENRSC)(const char * name);
typedef long int (WINAPI * MESSAGEENABLE)(long int enable);
typedef COLORREF(WINAPI * GETRSCCOLOR)(HRSC hRsc,long int index, long int number);
typedef long int(WINAPI * GETRSCOBJECTCODEBYNUMBER)(HRSC hRsc, long int excode, long int local, long int number);
typedef long int(WINAPI * GETRSCOBJECTCODEBYKEY)(HRSC hRsc, const char* key);
typedef long int(WINAPI * GETRSCOBJECT)(HRSC hRsc, long int incode, RSCOBJECT * object);
typedef long int(WINAPI * GETRSCSEGMENT)(HRSC hRsc, RSCSEGMENT * object, long int incode);
typedef long int(WINAPI * GETRSCOBJECTCOUNT)(HRSC hRsc);
typedef const char* (WINAPI * GETRSCSEGMENTNAME)(HRSC hRsc, long int incode);
typedef const char* (WINAPI * GETRSCSEGMENTSHORTNAME)(HRSC hRsc, long int incode);
typedef const char* (WINAPI * GETRSCOBJECTKEY)(HRSC hRsc, long int incode);


OPENRSC OpenRsc = (OPENRSC)0;
MESSAGEENABLE MessageEnable = (MESSAGEENABLE)0;
GETRSCCOLOR GetRscColor = (GETRSCCOLOR)0;
GETRSCOBJECTCODEBYNUMBER GetRscObjectCodeByNumber = (GETRSCOBJECTCODEBYNUMBER)0;
GETRSCOBJECTCODEBYKEY GetRscObjectCodeByKey = (GETRSCOBJECTCODEBYKEY)0;
GETRSCOBJECT GetRscObject = (GETRSCOBJECT)0;
GETRSCSEGMENT GetRscSegment = (GETRSCSEGMENT)0;
GETRSCOBJECTCOUNT GetRscObjectCount = (GETRSCOBJECTCOUNT)0;
GETRSCSEGMENTNAME GetRscSegmentName = (GETRSCSEGMENTNAME)0;
GETRSCSEGMENTSHORTNAME GetRscSegmentShortName = (GETRSCSEGMENTSHORTNAME)0;
GETRSCOBJECTKEY GetRscObjectKey = (GETRSCOBJECTKEY)0;

#ifndef LINUXAPI
__declspec(dllexport)
#endif
void _IMLAPI imlCloseGisLibraryEx()
{
    OpenRsc = (OPENRSC)0;
    MessageEnable = (MESSAGEENABLE)0;
    GetRscColor = (GETRSCCOLOR)0;
    GetRscObjectCodeByNumber = (GETRSCOBJECTCODEBYNUMBER)0;
    GetRscObjectCodeByKey = (GETRSCOBJECTCODEBYKEY)0;
    GetRscObject = (GETRSCOBJECT)0;
    GetRscSegment = (GETRSCSEGMENT)0;
    GetRscObjectCount = (GETRSCOBJECTCOUNT)0;
    GetRscSegmentName = (GETRSCSEGMENTNAME)0;
    GetRscSegmentShortName = (GETRSCSEGMENTSHORTNAME)0;
    GetRscObjectKey = (GETRSCOBJECTKEY)0;
}

#ifndef LINUXAPI
__declspec(dllexport)
#endif
void _IMLAPI imlOpenGisLibraryEx(HINSTANCE hMapLibrary)
{
#ifndef LINUXAPI
    OpenRsc = (OPENRSC)
        GetProcAddress(hMapLibrary, "mapOpenRsc");
    MessageEnable = (MESSAGEENABLE)
            GetProcAddress(hMapLibrary, "mapMessageEnable");
    GetRscColor = (GETRSCCOLOR)
        GetProcAddress(hMapLibrary, "mapGetRscColor");
    GetRscObjectCodeByNumber = (GETRSCOBJECTCODEBYNUMBER)
        GetProcAddress(hMapLibrary, "mapGetRscObjectCodeByNumber");
    GetRscObjectCodeByKey = (GETRSCOBJECTCODEBYKEY)
        GetProcAddress(hMapLibrary, "mapGetRscObjectCodeByKey");
    GetRscObject = (GETRSCOBJECT)
        GetProcAddress(hMapLibrary, "mapGetRscObject");
    GetRscSegment = (GETRSCSEGMENT)
        GetProcAddress(hMapLibrary, "mapGetRscSegment");
    GetRscObjectCount = (GETRSCOBJECTCOUNT)
        GetProcAddress(hMapLibrary, "mapGetRscObjectCount");
    GetRscSegmentName = (GETRSCSEGMENTNAME)
        GetProcAddress(hMapLibrary, "mapGetRscSegmentName");
    GetRscSegmentShortName = (GETRSCSEGMENTSHORTNAME)
        GetProcAddress(hMapLibrary, "mapGetRscSegmentShortName");
    GetRscObjectKey = (GETRSCOBJECTKEY)
        GetProcAddress(hMapLibrary, "mapGetRscObjectKey");
#else
    OpenRsc = (OPENRSC)
        dlsym((void *)hMapLibrary, "mapOpenRsc");
    MessageEnable = (MESSAGEENABLE)
        dlsym((void *)hMapLibrary, "mapMessageEnable");
    GetRscColor = (GETRSCCOLOR)
        dlsym((void *)hMapLibrary, "mapGetRscColor");
    GetRscObjectCodeByNumber = (GETRSCOBJECTCODEBYNUMBER)
        dlsym((void *)hMapLibrary, "mapGetRscObjectCodeByNumber");
    GetRscObjectCodeByKey = (GETRSCOBJECTCODEBYKEY)
        dlsym((void *)hMapLibrary, "mapGetRscObjectCodeByKey");
    GetRscObject = (GETRSCOBJECT)
        dlsym((void *)hMapLibrary, "mapGetRscObject");
    GetRscSegment = (GETRSCSEGMENT)
        dlsym((void *)hMapLibrary, "mapGetRscSegment");
    GetRscObjectCount = (GETRSCOBJECTCOUNT)
        dlsym((void *)hMapLibrary, "mapGetRscObjectCount");
    GetRscSegmentName = (GETRSCSEGMENTNAME)
        dlsym((void *)hMapLibrary, "mapGetRscSegmentName");
    GetRscSegmentShortName = (GETRSCSEGMENTSHORTNAME)
        dlsym((void *)hMapLibrary, "mapGetRscSegmentShortName");
    GetRscObjectKey = (GETRSCOBJECTKEY)
        dlsym((void *)hMapLibrary, "mapGetRscObjectKey");
#endif   // LINUXAPI
}


HRSC _fastcall mapOpenRsc(const char * name){
    if (OpenRsc) {
        return OpenRsc(name);
    } else {
        printf("JNI mapOpenRsc not found\n");
    }
    return 0;
}

long int _fastcall mapMessageEnable(long int enable){
    if (MessageEnable) {
        return MessageEnable(enable);
    } else {
        printf("JNI mapMessageEnable not found\n");
    }
    return 0;
}

COLORREF _fastcall mapGetRscColor(HRSC hRsc,long int index,
                                  long int number){
    if (GetRscColor) {
        return GetRscColor(hRsc, index, number);
    } else {
        printf("JNI mapGetRscColor not found\n");
    }
    return 0;
}

long int _fastcall imlGetRscObjectCodeByNumber(HRSC hRsc,
    long int excode, long int local, long int number)
{
  if (GetRscObjectCodeByNumber) {
    return GetRscObjectCodeByNumber(hRsc, excode, local, number);
    }
    else {
        printf("JNI mapGetRscObjectCodeByNumber not found\n");
    }
    return 0;
}

long int _fastcall imlGetRscObjectCodeByKey(HRSC hRsc, const char* key)
{
  if (GetRscObjectCodeByKey) {
    return GetRscObjectCodeByKey(hRsc, key);
    }
    else {
        printf("JNI mapGetRscObjectCodeByKey not found\n");
    }
    return 0;
}

long int  _fastcall imlGetRscObject(HRSC hRsc, long int incode, RSCOBJECT * object)
{
    if (GetRscObject) {
        return GetRscObject(hRsc, incode, object);
    } else {
        printf("JNI GetRscObject not found\n");
    }
    return 0;
}

long int  _fastcall imlGetRscSegment(HRSC hRsc, RSCSEGMENT * segment, long int incode)
{
    if (GetRscSegment) {
        return GetRscSegment(hRsc, segment, incode);
    } else {
        printf("JNI GetRscSegment not found\n");
    }
    return 0;
}


long int  _fastcall  imlGetRscObjectCount(HRSC hRsc)
{
    if (GetRscObjectCount) {
        return GetRscObjectCount(hRsc);
    } else {
        printf("JNI GetRscObjectCount not found\n");
    }
    return 0;
}

const char*   _fastcall  imlGetRscSegmentName(HRSC hRsc,long int incode)
{
    if (GetRscSegmentName) {
        return GetRscSegmentName(hRsc, incode);
    } else {
        printf("JNI GetRscSegmentName not found\n");
    }
    return 0;
}

const char*   _fastcall  imlGetRscSegmentShortName(HRSC hRsc,long int incode)
{
    if (GetRscSegmentShortName) {
        return GetRscSegmentShortName(hRsc, incode);
    } else {
        printf("JNI GetRscSegmentShortName not found\n");
    }
    return 0;
}

const char*   _fastcall  imlGetRscObjectKey(HRSC hRsc,long int incode)
{
    if (GetRscObjectKey) {
        return GetRscObjectKey(hRsc, incode);
    } else {
        printf("JNI GetRscObjectKey not found\n");
    }
    return 0;
}
*/

int imgPrepareImage_buildSvg(const POLYDATAEX * data,
    int type, const char * parm,
    const IMGCONTEXT * context){

    SVGBuilder::buildImage(parm, type, data, context, NULL);
    return 1;
}

HRSC _fastcall mapOpenRsc(const char * name){
    TRsc* rsc = new TRsc(name);
    return (HRSC)(void*)rsc;
}

void _fastcall mapCloseRsc(HRSC hrsc){
    TRsc* rsc = (TRsc*)(void*)hrsc;
    delete rsc;
}

long int _fastcall mapMessageEnable(long int enable){
    return 0;
}

COLORREF _fastcall mapGetRscColor(HRSC hRsc,long int index,
                                  long int number){
    TRsc* rsc = (TRsc*)(void*)hRsc;
    PALETTE* palette = rsc->getPalette(number);
    if (!palette) {
        char* message = new char[255];
        sprintf(message, "imlservex::mapGetRscColor: Palette %u not found", number);
        throw MyException(message);
    }
    return palette->Colors[index - 1];
}

long int _fastcall imlGetRscObjectCodeByNumber(HRSC hRsc,
    long int excode, long int local, long int number)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    OBJECTHEADER* objHeader = rsc->getObjectByExcode(excode, local);
    if (!objHeader) {
        char* message = new char[255];
        sprintf(message, "imlservex::imlGetRscObjectCodeByNumber: Object excode=%u local=%u not found", excode, local);
        throw MyException(message);
    }
    return objHeader->Incode;
}

long int _fastcall imlGetRscObjectCodeByKey(HRSC hRsc, const char* key)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    OBJECTHEADER* objHeader = rsc->getObjectByKey(key);
    if (!objHeader) {
        return 0;
//        char* message = new char[255];
//        sprintf(message, "imlservex::imlGetRscObjectCodeByKey: Object key=%s not found", key);
//        throw MyException(message);
    }
    return objHeader->Incode;
}

long int  _fastcall imlGetRscObject(HRSC hRsc, long int incode, RSCOBJECT * object)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    OBJECTHEADER* objHeader = rsc->getObjectByIncode(incode);
    if (!objHeader) {
        char* message = new char[255];
        sprintf(message, "imlservex::imlGetRscObjectL Object incode=%u not found", incode);
        throw MyException(message);
    }
    object->Length = sizeof(RSCOBJECT);
    object->Code = objHeader->Code;
    object->Local = objHeader->Local;
    object->Segment = objHeader->Segment + 1;
    object->Scale = objHeader->Scalable;
    object->Direct = objHeader->Direct;
    object->Top = objHeader->UpperScale;
    object->Bot = objHeader->LowerScale;

    memset(object->Name, '0', 32);
    strncpy((char*)object->Name, objHeader->Name, 32);

    return 0;
}

long int  _fastcall imlGetRscSegment(HRSC hRsc, RSCSEGMENT * segment, long int incode)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    SEGMENTHEADER* segHeader = rsc->getSegment(incode);
    if (!segHeader) {
        char* message = new char[255];
        sprintf(message, "imlservex::imlGetRscSegment: Segment incode=%u not found", incode);
//        throw MyException(message);
        return -1;
    }
    segment->Order = segHeader->Number;
    segment->Order = segHeader->Order;
    memset(segment->Name, '0', 32);
    strncpy((char*)segment->Name, segHeader->Name, 32);

    return 0;
}

long int  _fastcall imlGetRscSegmentLocalCount(HRSC hRsc, int* count, long int incode, long int local)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    *count = rsc->getSegmentLocalObjectCount(incode, local);
    return 0;
}

long int  _fastcall imlGetRscSegmentLocalIncodes(HRSC hRsc, int** incodes, long int incode, long int local)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    *incodes = rsc->getSegmentLocalObjectIncodes(incode, local);
    return 0;
}

long int  _fastcall  imlGetRscSegmentCount(HRSC hRsc)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    RSCHEADER* header = rsc->getHeader();
    return header->SegmentCount;
}

long int  _fastcall  imlGetRscObjectCount(HRSC hRsc)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    RSCHEADER* header = rsc->getHeader();
    return header->ObjectCount;
}

const char*   _fastcall  imlGetRscSegmentName(HRSC hRsc,long int incode)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    SEGMENTHEADER* segHeader = rsc->getSegment(incode);
    if (!segHeader) {
        char* message = new char[255];
        sprintf(message, "imlservex::imlGetRscSegmentName: Segment incode=%u not found", incode);
        throw MyException(message);
    }

    return segHeader->Name;
}

const char* _fastcall  imlGetRscSegmentShortName(HRSC hRsc,long int incode)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    SEGMENTHEADER* segHeader = rsc->getSegment(incode);
    if (!segHeader) {
        char* message = new char[255];
        sprintf(message, "imlservex::imlGetRscSegmentShortName: Segment incode=%u not found", incode);
        throw MyException(message);
    }

    return segHeader->ShortName;
}

const char*   _fastcall  imlGetRscObjectKey(HRSC hRsc,long int incode)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    OBJECTHEADER* objHeader = rsc->getObjectByIncode(incode);
    if (!objHeader) {
        char* message = new char[255];
        sprintf(message, "imlservex::imlGetRscObjectKey: Object incode=%sunot found", incode);
        throw MyException(message);
    }

    return objHeader->ShortName;
}

//  Запросить название значения характеристики из
//  классификатора семантики по коду семантики и
//  последовательному номеру в классификаторе
//  hRsc - идентификатор классификатора карты
//  code - код семантики
//  number - последовательный номер в классификаторе(1,2,3...)
//  При ошибке возвращает пустую строку

const char * _fastcall imlGetRscSemanticClassificatorName(HRSC hRsc,long int code,
                                                          long int number){
    TRsc* rsc = (TRsc*)(void*)hRsc;
    Class_SemClass_Map* map = rsc->getSemanticClassByCode(code);
    return map->at(number)->Name;
}

Class_SemClass_Map* _fastcall imlGetRscSemanticClassificatorMap(HRSC hRsc,  long int code){
    TRsc* rsc = (TRsc*)(void*)hRsc;
    Class_SemClass_Map* map = rsc->getSemanticClassByCode(code);
    return map;
}

// ------------------------------------------------------------------


int _fastcall MessageBox(HWND hwnd, const TCHAR * message,
                         const TCHAR * title, int flag){
  return 1;
}


// ------------------------------------------------------------------
// Обработать графический объект в соответствии с видом операции
// test    - вид операции (0 - рисовать, 1 - определить попадание в область,
//                         2 - определить габариты графического объекта),
// data    - координаты графического объекта,
// type    - тип графического объекта (см. mapgdi.h : IMG_LINE...),
// parm    - параметры рисуемого объекта (см. mapgdi.h : IMGLINE...),
// context - вспомогательные параметры отображения (см. mapgdiex.h),
// border  - рассчитанные габариты с учетом их предыдущего значения !
// При ошибке возвращает ноль                      28/09/01
// ------------------------------------------------------------------
int _fastcall imgPrepareImage(const POLYDATAEX * data,
                              int type, const char * parm,
                              const IMGCONTEXT * context,
                              int test, FRAME * border)
{
//   printf ("imlserv0::imgPrepareImage begin type=%d test=%d\n", type, test)  ;
   if (type < IMG_LINE)
     {
       return 0;
     }

   if (test == 0)
     {
       return imgPrepareImage_buildSvg(data, type, parm, context);
     }
   return 0;
}
