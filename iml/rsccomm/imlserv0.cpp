/****** IMLSERV.CPP ************ Belenkov O.V.   ****** 13/08/15 ***
******* IMLSERV.CPP ************ Panteleeva N.A. ****** 04/04/16 ***
******* IMLSERV.CPP ************ Dishlenko S.G.  ****** 30/03/15 ***
******* IMLSERV.CPP ************ Shabakov D.A.   ****** 29/04/15 ***
******* IMLSERV.CPP ************ Korjilov A.     ****** 17/11/14 ***
******* IMLSERV.CPP ************ Kruzhkov A.E.   ****** 23/07/15 ***
******* IMLSERV.CPP ************ Savelov A.S.    ****** 18/08/15 ***
******* IMLSERV.CPP ************ Konon V.N.      ****** 02/06/16 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2016              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*     ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ОТОБРАЖЕНИЯ ПОЛЬЗОВАТЕЛЬСКИХ     *
*                  ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ                      *
*                                                                  *
*******************************************************************/

#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "imlservex.h"
#include "imlserv0.h"

#ifndef MAPGDI_H
  #include "mapgdi.h"
#endif

#include <math.h>
#include <stdio.h>
#include <limits.h>

#ifdef WIN32API
 #include <tchar.h>
#endif

#include "rsc.h"
#include "svgbuilder.h"

#define RGB24BGR24(color) (((color&0x000000FF)<<16) |  \
                           ((color&0x0000FF00)    ) |  \
                           ((color&0x00FF0000)>>16))



 // *********************************************************
 // *                                                        *
 // *        ОБРАБОТКА ГРАФИЧЕСКИХ ПАРАМЕТРОВ ОБ' ЕКТА       *
 // *                                                        *
 // **********************************************************

#define DRAWIDENT 0x7FFF7FFE



// ---------------------------------------------------------
// Запросить экранные параметры объекта по incode
// При ошибке возвращает ноль                                 // 15/04/12
// ---------------------------------------------------------
char * _fastcall imlGetRscObjectParameters(HRSC hRsc,long int incode)
{
    TRsc* rsc = (TRsc*)(void*)hRsc;
    PARAMHEADER* paramHeader = rsc->getParams(incode);
    if (!paramHeader) {
       return 0;
    }
    char* parameters = (char*)paramHeader + sizeof(PARAMHEADER);

    return parameters;
}

// ---------------------------------------------------------
// Запросить экранную функцию объекта по incode
// При ошибке возвращает ноль                                 // 15/04/12
// ---------------------------------------------------------
 int _fastcall imlGetRscObjectFunction(HRSC hRsc,long int incode)
{
     TRsc* rsc = (TRsc*)(void*)hRsc;
     PARAMHEADER* paramHeader = rsc->getParams(incode);
     if (!paramHeader) {
         char* message = new char[255];
         sprintf(message, "imlserv0::imlGetRscObjectParameters: Parameters incode=%u not found", incode);
         throw MyException(message);
     }
     return paramHeader->Function;
}

 // Заполнить структуру описания семантической характеристики
 // по коду семантики
 // code - внешний код семантики
 // При ошибке возвращает ноль

 int _fastcall imlGetRscSemanticExByCode(HRSC hRsc, RSCSEMANTICEX * semtype,
                                         int code)
 {
     TRsc* rsc = (TRsc*)(void*)hRsc;
     SEMANTICHEADER* semHeader = rsc->getSemantic(code);
     if (!semHeader) {
         char* message = new char[255];
         sprintf(message, "imlserv0::imlGetRscSemanticExByCode: Semcntic code=%u not found", code);
         return 0;
//         throw MyException(message);
     }
     DEFAULTVALUE* defaultValue = rsc->getSemanticDefaultValue(code);

     semtype->Code = semHeader->Code;         // внешний код
     semtype->Type = semHeader->Type;         // тип значения
     semtype->Reply = semHeader->Multiple;        // разрешается повторение (1)
     semtype->Enable = 1;       // допустимая(1),обязательная(2)
                            // (при создании семантики поле не учитывается)
     semtype->Service = semHeader->Service;      // служебная,допустима для всех объектов (1)
     semtype->Reserve = 0;       // резерв (должен быть равен 0)

     memset(semtype->Name, '0', 32);
     strncpy(semtype->Name, semHeader->Name, 32);     // название характеристики
     memset(semtype->Unit, '0', 32);
     strncpy(semtype->Unit, semHeader->MesureUnit, 32);      // единица измерения

     if (defaultValue) {
       semtype->Minimum = defaultValue->MinValue;      // минимальное значение
       semtype->Default = defaultValue->DefaultValue;      // умалчиваемое значение
       semtype->Maximum = defaultValue->MaxValue;      // максимальное значение
     } else {
       semtype->Minimum = 0;
       semtype->Default = 0;
       semtype->Maximum = 0;
     }
     semtype->Size = semHeader->Size;         // общий размер поля значения семантики
     semtype->Decimal = semHeader->Precision;      // точность поля значения семантики

     memset(semtype->ShortName, '0', 16);
     strncpy(semtype->ShortName, semHeader->ShortName, 16);// короткое имя семантики (связь с полем БД)

     return 1;
 }

 // ---------------------------------------------------------
 // Найти внутренний код объекта по коду,локализации и номеру в аналогичных
 // code - внешний код семантики
 // При ошибке возвращает ноль   06/02/07
 // ---------------------------------------------------------
 int _fastcall imlGetRscObjectIncode(HRSC hRsc,long int excode,
                                        long int local, long int number)
 {
     TRsc* rsc = (TRsc*)(void*)hRsc;
     OBJECTHEADER* objHeader = rsc->getObjectByExcode(excode, local);
     if (!objHeader) {
         if (excode) {
           char message[255];
           sprintf(message, "imlserv0::imlGetRscObjectIncode: Object excode=%u local=%u not found", excode, local);
           printf ("%s\n", message);
         }
         return 0;
     }
     return objHeader->Incode;
 }



 // ---------------------------------------------------------
 // Запросить размеры векторного объекта
 // semantic - указатель на семантику объекта, если подпись зависит от семантики
 // При ошибке возвращает ноль                                 // 30/03/09
 // ---------------------------------------------------------
 int _fastcall imlGetRscVectorSize(IMGVECTOREX * vector,IMAGESIZE* imagesize,    // 30/03/09
                                   char * semantic, HRSC rsc)
 {

     double ve_v_factor_x;
     if (vector->HSize==0 || (vector->HLine2 - vector->HLine1)==0){
         ve_v_factor_x = 1.;
     } else {
         ve_v_factor_x = 1. * vector->HSize / (vector->HLine2 - vector->HLine1) ;
     }

     POLYDATAEX* data = new POLYDATAEX();
     DRAWPOINT point;
     point.x = 0;
     point.y = 0;
     int* polycounts = new int[1];
     polycounts[0] = 1;
     data->Points = &point;  // Адрес массива объектов типа POINT
     data->PolyCounts = polycounts;    // Адрес массива количества точек
     data->Count = 1;    // Число ломаных (многоугольников, текстов)
     data->Text = 0;   // Указатель на массив адресов текстов
     data->AlignCode = 0;   // Код выравнивания для отображения текста
     data->Flags = 0;   // Флаги обработки метрики - равно 0
     data->Semantic = semantic;   // Указатель на семантику объекта (SEMANTIC *)
 //	BOX        Border;   // Прямоугольник габаритов объекта в пикселах относительно области рисования
     data->ShowScale = 100000;   // Текущий масштаб отображения электронной карты
     data->MapRsc = rsc;   // Классификатор карты, содержащей отображаемый объект
     data->Height = 0.;   // Высота первой точки объекта (если 3D-координаты) или 0
     data->DX = 0.;       // Приращение от первой точки до второй на карте
     data->DY = 0.;       // в единицах карты - для ориентирования векторных объектов

     int type = IMG_VECTOREX;
     char * parm = (char*)vector;

     QDomDocument doc;
     QDomElement svg_node = doc.appendChild(doc.createElement("svg")).toElement();
     QDomElement g_node = svg_node.appendChild(doc.createElement("g")).toElement();
     QDomElement desc_node = svg_node.appendChild(doc.createElement("desc")).toElement();
     IMGCONTEXT context1;
     ImgBorder imgBorder;
     context1.imgBorder = &imgBorder;
     SvgDesc svgDesc(&svg_node, &g_node, &desc_node);
     context1.svg_desc = &svgDesc;
     context1.hrsc = rsc;
     TRsc* _rsc = (TRsc*)(void*)rsc;
     context1.baseLineLength_px = _rsc->getBaseLineLength_px();

     SVGBuilder::buildImage(parm, type, data, &context1, NULL);

     if ((imgBorder.minX > imgBorder.maxX) ||
         (imgBorder.minY > imgBorder.maxY)) {
       imagesize->HorizontalSize = 0;
       imagesize->VerticalSize = 0;
       imagesize->DeltaH = 0;
       imagesize->DeltaV = 0;
     } else {
       imagesize->HorizontalSize = (int)(imgBorder.maxX - imgBorder.minX);
       imagesize->VerticalSize = (int)(imgBorder.maxY - imgBorder.minY);
       imagesize->DeltaH = (int)imgBorder.minX;
       imagesize->DeltaV = (int)imgBorder.minY;
     }

     delete[] polycounts;
     delete data;
    return 1;
 }

 // ------------------------------------------------------------------
 //  Определить значение цвета в формате RGB по значению цвета
 //  в параметрах отображения
 // ------------------------------------------------------------------
 long int imgGetTrueColorByColor(const IMGCONTEXT * context,
                                 long int color)
 {
   // Это индекс цвета из палитры карты ?
   if ((color && IMGC_INDEX) == 0)
     {
       // Это чистый цвет
       return color;
     }

   int index = color && IMGC_CLEARCOLOR;
   TRsc* rsc = (TRsc*)(void*)(context->hrsc);
   int palette_index = 1;
   PALETTE* palette = rsc->getPalette(palette_index);
   if (!palette) {
       char* message = new char[255];
       sprintf(message, "imlserv0::imgGetTrueColorByColor: palette index=%u not found", palette_index);
       throw MyException(message);
   }
   int palette_color = palette->Colors[index];

   return RGB24BGR24(palette_color);
 }

 // ---------------------------------------------------------
 // Запросить язык сообщений                                // 19/01/09
 // 1 - английский, 2 - русский, ... (см. Maptype.h)
 // ---------------------------------------------------------
 int _fastcall imlGetMapAccessLanguage()
 {
    return 1;
 }

 // ------------------------------------------------------------------
 // Преобразование из пикселов в изображении в координаты
 // на местности в метрах
 // Применение :
 // xmet = xdis; ymet = ydis;
 // mapPictureToPlane(xmet,ymet);
 // hmap - идентификатор открытых данных
 // x,y  - преобразуемые координаты
 // ------------------------------------------------------------------
 void _fastcall imgPictureToPlane(HMAP hmap, double *x, double *y)
 {
   IMGCONTEXT* context = (IMGCONTEXT*)(void*)hmap;
//   context->Scale;
   double scale = 200000;
   double scale_coeff = 1./1000000 * scale;
   *x *= scale_coeff/context->PixelX;
   *y *= scale_coeff/context->PixelY;
 }

 // ------------------------------------------------------------------
 // Вычисление расстояния между двумя точками на местности
 // hmap   - идентификатор открытых данных
 // point1, point2 - координаты точек в метрах на местности
 // Для вычисления расстояния координаты пересчитываюся
 // в проекцию топографической карты с установкой осевого
 // меридиана в центре отрезка
 // При ошибке возвращает 0
 // ------------------------------------------------------------------
 double _fastcall imgRealDistance(HMAP hmap, DOUBLEPOINT *point1,
                                  DOUBLEPOINT *point2)
 {
     double dx = point1->x - point2->x;
     double dy = point1->y - point2->y;
     return sqrt(dx * dx + dy * dy);
 }

