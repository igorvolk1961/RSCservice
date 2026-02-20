/****** IMLMAIN.CPP ************ Belenkov O.V.   ****** 19/04/16 ***
******* IMLMAIN.CPP ************ Panteleeva N.A. ****** 19/04/16 ***
******* IMLMAIN.CPP ************ Derevyagina Zh.A.***** 16/11/15 ***
******* IMLMAIN.CPP ************ Savelov A.S.    ****** 12/07/16 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2016              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*      ОПИСАНИЕ ПАРАМЕТРОВ ФУНКЦИЙ ВИЗУАЛИЗАЦИИ ПРОИЗВОЛЬНЫХ       *
*             ГРАФИЧЕСКИХ ОБ ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ               *
*                                                                  *
*******************************************************************/
#include <stdio.h>
#ifndef IMLSERV_H
  #include "imlserv.h"
#endif

#ifndef RSCNAVY_H
  #include "rscnavy.h"
#endif

#include "imltext.h"            // 19/04/16

// Константы времени выполнения
long   IML_LANGUAGE = 1;
double SHALLOW_CONTOUR = 10.0;  // Контур отмели
double DEEP_CONTOUR    = 10.0;  // Контур глубины   // ум. 30
double SAFETY_CONTOUR  = 10.0;  // Контур безопасности
long   TWO_SHADES      = 0;     // 1- градация из 2 цветов, 0 - из 4 цветов
long   SHALLOW_PATTERNS= 0;     // Заполнение мели (0 - нет, 1- да)
long   SHOW_ISDANGER_SHALLOW = 1; // Показывать изолированные опасности в мелкой воде
                                  // (0 - нет, 1- да)

// ТЕКСТОВАЯ ИНФОРМАЦИЯ НА ПОДДЕРЖИВАЕМЫХ СИСТЕМОЙ ЯЗЫКАХ (до 4)
// ML_ENGLISH (1), ML_RUSSIAN(2), ...

// Параметры ориентированных знаков
IMLPARM ImlOrientObject =
{
  IMLIDENT, 1,
                1, 0, 0, 0,                // Передавать семантику
  {{ IMG_VECTOREX, "Mark parameters", "Параметры знака", 0, 0},
  }
};

// Параметры для построения огней
IMLPARM ImlLights =
{
  IMLIDENT, 1,
                1, 0, 0, 0,                // Передавать семантику
  {{ IMG_LONG, "Mark border", "Габариты знака", 0, 0 },
  }
};

// Параметры ориентированных знаков с поворотом по семантике
// в центре площадного

IMLPARM ImlCentreObject =
{
  IMLIDENT, 4,
               1, 0, 0, 0,     // Передавать семантику
  {{ IMG_VECTOREX, "Mark parameters", "Параметры знака", 0, 0},
   { IMG_SEMANTIC, "Semantic number for angle in degree", "Номер семантики - угол в градусах", 0, 0},
   { IMG_DOUBLE,   "Horizontal shift (mm) ", "Горизонтальное смещение (мм на карте)", 0, 0},
   { IMG_DOUBLE,   "Vertical shift (mm)", "Вертикальное смещение (мм на карте)", 0, 0},
  }
};

// Параметры ориентированных знаков по семантике RESTRN

IMLPARM ImlRestrn =             // 06/02/07
{
  IMLIDENT, 1,
               1, 0, 0, 0,     // Передавать семантику
  {
    { IMG_VECTOREX, "Default mark parameters", "Параметры знака по умолчанию", 0, 0},
  }
};



// Параметры знаков, выбираемых по значению характеристики семантики

IMLPARM ImlSeriaObject =                 // 06/02/07
{
  IMLIDENT, 14,
               1, 0, 0, 0,     // Передавать семантику
  {{ IMG_DRAW, "Default mark", "Параметры умалчиваемого знака", 0, 0},
   { IMG_SEMANTIC, "Semantic number", "Номер семантики", 0, 0},
   { IMG_DOUBLE,   "Minimum", "Минимум", 0, 0},
   { IMG_DOUBLE,   "Maximum", "Максимум", 0, 0},
   { IMG_DRAW, "Mark 1", "Параметры первого знака", 0, 0},
   { IMG_DOUBLE,   "Minimum", "Минимум", 0, 0},
   { IMG_DOUBLE,   "Maximum", "Максимум", 0, 0},
   { IMG_DRAW, "Mark 2", "Параметры второго знака", 0, 0},
   { IMG_DOUBLE,   "Minimum", "Минимум", 0, 0},
   { IMG_DOUBLE,   "Maximum", "Максимум", 0, 0},
   { IMG_DRAW, "Mark 3", "Параметры третьего знака", 0, 0},
   { IMG_DOUBLE,   "Minimum", "Минимум", 0, 0},
   { IMG_DOUBLE,   "Maximum", "Максимум", 0, 0},
   { IMG_DRAW, "Mark 4", "Параметры четвертого знака", 0, 0},
  }
};

// Параметры для построения площадных зон глубины
IMLPARM ImlDepareObject =
{
  IMLIDENT, 1,
                1, 0, 0, 0,                // Передавать семантику
  {{ IMG_LONG, "Object code", "Код объекта", 0, 0 },
  }
};

// Параметры для построения препятствий
IMLPARM ImlObstrnObject =
{
  IMLIDENT, 2,
                1, 0, 0, 0,                // Передавать семантику
  {{ IMG_LONG, "Object local ", "Локализация объекта", 0, 0 },
   { IMG_LONG, "Object Code ", "Код объекта", 0, 0 }
  }
};

// Параметры векторных знаков с подписями с учетом языка
IMLPARM ImlVectorNational =
{
  IMLIDENT, 1, 1, 0, 0, 0,             // Передавать семантику
  {{ IMG_VECTOREX, "Mark parameters", "Параметры знака", 0, 0},
  }
};

// Параметры для построения границ зон глубины
IMLPARM ImlDepContourObject =
{
  IMLIDENT, 1,
                1, 0, 0, 0,                // Передавать семантику
  {{ IMG_LONG, "Object code", "Код объекта", 0, 0 },
  }
};

// Параметры для построения изобат
IMLPARM ImlDepCurvObject =
{
  IMLIDENT, 1,
                1, 0, 0, 0,                // Передавать семантику
  {{ IMG_LONG, "Object code", "Код объекта", 0, 0 },
  }
};

IMLPARM ImlSlCons =       // Вид береговых конструкций
{
  IMLIDENT, 2,
                1, 0, 0, 0,                // Передавать семантику
  {
   { IMG_LONG, "Object local ", "Локализация объекта", 0, 0 },
   { IMG_DRAW, "Default mark", "Параметры умалчиваемого знака", 0, 0},
  }
};

IMLPARM ImlWrecks =       // Вид остатков кораблекрушения
{
  IMLIDENT, 2,
                1, 0, 0, 0,                // Передавать семантику
  {
   { IMG_LONG, "Object local", "Локализация объекта", 0, 0 },
   { IMG_LONG, "Object code", "Код объекта", 0, 0},
  }
};

IMLPARM ImlCblondText =       // Подписи к трубопроводам
{
  IMLIDENT, 2,
                1, 0, 0, 0,                // Передавать семантику
  {
   { IMG_LONG, "Object local", "Локализация объекта", 0, 0 },
   { IMG_LONG, "Object code", "Код объекта", 0, 0},
  }
};

IMLPARM ImlQualin =           // Линейные объекты низкой точности
{
  IMLIDENT, 2,
                1, 0, 0, 0,                // Передавать семантику
  {
   { IMG_LONG, "Object local", "Локализация объекта", 0, 0 },
   { IMG_LONG, "Object code", "Код объекта", 0, 0},
  }
};

IMLPARM ImlSounds =           // Отметки глубин
{
  IMLIDENT, 2,
                1, 0, 0, 0,                // Передавать семантику
  {
   { IMG_LONG, "Object local", "Локализация объекта", 0, 0 },
   { IMG_LONG, "Object code", "Код объекта", 0, 0},
  }
};


IMLLIST ImlList =
{
  IMLIDENT, 15,
  {{ IMG_ORIENTOBJECT, &ImlOrientObject,
                              "Orient mark", "Ориентированный знак", 0, 0},
   { IMG_LIGHTS, &ImlLights, "Lights", "Огни", 0, 0},
   { IMG_CENTREOBJECT, &ImlCentreObject,
                              "Centre mark", "Центрированный  знак", 0, 0},
   { IMG_RESTRN, &ImlRestrn,
                              "Restrictions mark", "Знак по ограничениям", 0, 0},
   { IMG_SERIA,  &ImlSeriaObject,
                              "Mark, changed on semantics", "Знак по семантике", 0, 0},
   { IMG_DEPARE,  &ImlDepareObject,
                              "Depth square", "Изображение площадных глубин", 0, 0},
   { IMG_OBSTRN,  &ImlObstrnObject,
                              "Obstrn", "Изображение препятствий", 0, 0},
   { IMG_VECTNATIONAL, &ImlVectorNational,
                              "Mark,changed language", "Знак,со сменой языка", 0, 0},
   { IMG_DEPCONTOUR , &ImlDepContourObject,
                              "Depth contour", "Контур глубины", 0, 0},
   { IMG_DEPCURV, &ImlDepCurvObject,
                              "Depth curve", "Изобата", 0, 0},
   { IMG_SLCONS, &ImlSlCons, "Shoreline construction","Береговые конструкции",0,0},

   { IMG_WRECKS, &ImlWrecks, "Wrecks","Вид остатков кораблекрушения",0,0},

   { IMG_CBLOHD, &ImlCblondText, "Cable text","Подпись к кабелю",0,0},

   { IMG_QUALN , &ImlQualin, "Line low accuracy","Линия низкой точности",0,0},

   { IMG_SOUNDS, &ImlSounds, "Sounds","Отметки глубин",0,0},
   { 0, 0, 0, 0, 0, 0},
  }

};

// Список кодов мультиязычных названий примитивов и их параметров     // 19/04/16
IMLNAMECODES ImlCodes[] =
{
  // { Код примитива, номер названия, число параметров, 0, { номер названия параметра, ... 0 } }
  { IMG_ORIENTOBJECT, 1, 1, 0, { 2, 0 } },
  { IMG_LIGHTS, 3, 1, 0, { 4, 0 } },
  { IMG_CENTREOBJECT, 5, 4, 0, { 6, 7, 8, 9, 0 } },
  { IMG_RESTRN, 10, 1, 0, { 11, 0 } },
  { IMG_SERIA, 12, 14, 0, { 13, 14, 15, 16, 17, 15, 16, 18, 15, 16, 19, 15, 16, 20, 0 } },
  { IMG_DEPARE, 21, 1, 0, { 22, 0 } },
  { IMG_OBSTRN, 23, 2, 0, { 24, 25, 0 } },
  { IMG_VECTNATIONAL, 26, 1, 0, { 27, 0 } },
  { IMG_DEPCONTOUR, 28, 1, 0, { 29, 0 } },
  { IMG_DEPCURV, 30, 1, 0, { 31, 0 } },
  { IMG_SLCONS, 32, 2, 0, { 33, 34, 0 } },
  { IMG_WRECKS, 35, 2, 0, { 36, 37, 0 } },
  { IMG_CBLOHD, 38, 2, 0, { 36, 37, 0 } },
  { IMG_QUALN, 39, 2, 0, { 36, 37, 0 } },
  { IMG_SOUNDS, 40, 2, 0, { 36, 37, 0 } },
  { 0, 0, 0, 0, { 0 } }                                               // 12/07/16 Савелов
};



/*
IMLPARMLIST ImlParmList =
{


};
*/

extern "C"
{
 // -----------------------------------------------------------------
 // Запросить мультиязычное название примитивов и их параметров     // 19/04/16
 // type - код примитива
 // number - номер параметра, если равен нулю, то запрашивается название примитива
 // Возвращает указатель на название на текущем языке интерфейса программы
 // При ошибке возвращает ноль
 // -----------------------------------------------------------------
_IMLIMP const WCHAR * _IMLAPI imlGetImageName(long int type, long int number)
 {
   // Найти описание примитива
   int count = sizeof(ImlCodes) / sizeof(ImlCodes[0]);
   for (int i = 0; i < count; i ++)
   {
     if (ImlCodes[i].Type == type)
       {
         if (number <= 0) return imltxt(ImlCodes[i].Code);
         if ((number > ImlCodes[i].Count) || (number > IMLPARMCOUNT))
           return 0;
         int code = ImlCodes[i].Codes[number-1];       // 19/04/16
         if (code <= 0) return 0;
         return imltxt(code);
       }
   }

   return 0;
 }

 // -----------------------------------------------------------------
 // Запросить список видов графических объектов,
 // поддерживаемых библиотекой
 // -----------------------------------------------------------------
_IMLIMP const IMLLIST * _IMLAPI imlGetImageList(void)       // 16/11/15
 {
   return &ImlList;
 }

 // -----------------------------------------------------------------
 // Запросить габариты изображения объекта в дискретах в базовом масштабе
 // type - тип объекта (IMLLIST),
 // data - координаты объекта в дискретах (maptype.h);
 // parm - параметры отображения (MAPGDI.H);
 // context - дополнительные параметры отображения,
 // border  - рассчитанные габариты изображения в дискретах.
 // Если функция не поддерживается - возвращает ноль,
 // иначе - ненулевое значение
 // -----------------------------------------------------------------
_IMLIMP long int _IMLAPI imlGetImageBorder(long int type,
                                    const POLYDATAEX* data,
                                    const IMGDRAW * parm,
                                    const IMGCONTEXT * context,
                                    FRAME * border)
 {
   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;
   switch (type)
   {
     case IMG_ORIENTOBJECT :

                 return OrientObject(data, parm, context,
                                         PREPARE_BORDER, border);

     case IMG_LIGHTS :

                 return Lights(data, parm, context,
                                   PREPARE_BORDER, border);
     case IMG_CENTREOBJECT :

                return CentreObject(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_RESTRN :

              return RestrnObject(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_SERIA :

              return SeriaObject(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_DEPARE :

              return DepareObject(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_OBSTRN :

              return ObstrnObject(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_VECTNATIONAL:

              return NationalVectObject(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_DEPCONTOUR:

              return DepContourObject(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_DEPCURV:
              return DepCurvObject(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_SLCONS :

              return ShorelineConstractions(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_WRECKS:

              return Wrecks(data, parm, context,
                                    PREPARE_BORDER, border);
    case IMG_CBLOHD:
              return CbLohdText(data, parm, context,PREPARE_BORDER, border);

    case IMG_QUALN:
               return QualinLine(data, parm, context,PREPARE_BORDER, border);

    case IMG_SOUNDS:
               return Sounds(data, parm, context,PREPARE_BORDER, border);


   }

   return 0;
 }

 // -----------------------------------------------------------------
 // Отобразить объект в экранном виде
 // type - тип объекта (IMLLIST);
 // data - координаты объекта (MAPTYPE.H);
 //   Параметр data содержит координаты отображаемого объекта
 //   в системе устройства вывода (пикселы)
 //   относительно левого верхнего угла отображаемой области.
 //   (Координаты могут быть (!) :
 //     - отрицательными,  когда объект частично
 //       или полностью не виден;
 //     - не соответствовать правилам цифрового описания
 //       для объектов данного вида :
 //       незамкнутый площадной объект и т.п.;
 //     - ... прочие неприятности ).
 // parm - параметры отображения (MAPGDI.H);
 // context - дополнительные параметры отображения.
 // Возвращаемое значение :
 //  0  - объект не виден (слишком мелкий масштаб
 //       или объект не виден по координатам ...),
 //       или код объекта не поддерживается;
 //  +1 - объект отображен.
 // -----------------------------------------------------------------
_IMLIMP long int _IMLAPI imlPaintImage(long int type,
                                const POLYDATAEX* data,
                                const IMGDRAW * parm,
                                const IMGCONTEXT * context)
 {
   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;

   switch (type)
   {
     case IMG_ORIENTOBJECT :

                 return OrientObject(data, parm, context, 0, 0);

     case IMG_LIGHTS :

                 return Lights(data, parm, context, 0, 0);

     case IMG_CENTREOBJECT :
                 return CentreObject(data, parm, context, 0, 0);

     case IMG_RESTRN :

              return RestrnObject(data, parm, context, 0, 0);

     case IMG_SERIA :

              return SeriaObject(data, parm, context,0,0);

     case IMG_DEPARE :

              return DepareObject(data, parm, context,0,0);

     case IMG_OBSTRN :

              return ObstrnObject(data, parm, context,0,0);

     case IMG_VECTNATIONAL:              // 19/01/09

              return NationalVectObject(data, parm, context,0, 0);

     case IMG_DEPCONTOUR:                // 27/02/09

              return DepContourObject(data, parm, context,0, 0);

     case IMG_DEPCURV:                   // 27/02/09

              return DepCurvObject(data, parm, context,0, 0);

     case IMG_SLCONS :

              return ShorelineConstractions(data, parm, context,0,0);

     case IMG_WRECKS:

              return Wrecks(data, parm, context,0, 0);

    case IMG_CBLOHD:

              return CbLohdText(data, parm, context,0, 0);

    case IMG_QUALN:
              return QualinLine(data, parm, context,0,0);

    case IMG_SOUNDS:
              return Sounds(data, parm, context,0, 0);


    default :   // КОД НЕ НАЙДЕН

                 imgPrepareImage(data, IMG_DRAW, (const char *)parm,
                                 context, 0, 0);
                 break;
   }

   return 0;
 }

 // -----------------------------------------------------------------
 // Отобразить образец объекта в экранном виде
 // по упрощенной метрике (1,2-5 точек), формируемой
 // самой функцией.
 // Применяется для выбора вида объекта в режиме
 // "дизайна", в меню пиктограмм и т.п.
 // Метрика подбирается исходя из вкусов программиста,
 // результат можно проверить в диалоге "Выбор объекта" в
 // закладке "Вид"
 // Размер "окошка вывода" в параметре context,в поле size.
 // -----------------------------------------------------------------
_IMLIMP long int _IMLAPI imlPaintExampleEx(long int type,
                                   const IMGDRAW * parm,
                                   const IMGCONTEXT * context,
                                   HRSC hrsc, const char * semantic)  // 29/08/13
 {
   POLYDATAEX  data;
   DRAWPOINT points[7];
   int  count[2];                 // 26/03/12
                       // Ident    Length    Code  Type Scale  Value
   char Semantic[] = { 0xFF, 0x7F,  16, 0,  136, 0,  2,   0,   100, 0,
                                            137, 0,  2,   0,   170, 0};

   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;

   memset((void*)&data, 0, sizeof(data));

   data.Count      = 1;
   data.PolyCounts = &count[0];
   data.Points     = &points[0];

   data.Border.LT  = 0;
   data.Border.UP  = 0;
   data.Border.RT  = context->Size.CX;
   data.Border.DN  = context->Size.CY;

   data.MapRsc     = hrsc;                                       // 29/08/13
   data.Semantic   = (char*)semantic;                            // 29/08/13

   switch (type)
   {
     case IMG_ORIENTOBJECT :

                 count[0]    = 2;
                 points[0].X = context->Size.CX / 5;
                 points[0].Y = context->Size.CY - context->Size.CY / 5;
                 points[1].X = context->Size.CX - context->Size.CX / 5;
                 points[1].Y = context->Size.CY / 5;

                 return OrientObject(&data, parm, context, 0, 0);


    case IMG_LIGHTS :

                 count[0]    = 1;
                 points[0].X = context->Size.CX  - context->Size.CX / 4;
                 points[0].Y = context->Size.CY  - context->Size.CY / 4;

                 if (data.Semantic == 0)                                  // 29/08/13
                   data.Semantic = Semantic;

                 return Lights(&data, parm, context, 0, 0);

    case IMG_CENTREOBJECT :

                count[0]    = 5;
                points[0].X = context->Size.CX / 5;
                points[0].Y = context->Size.CY / 5;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY / 5;
                points[2].X = context->Size.CX - context->Size.CX / 5;
                points[2].Y = context->Size.CY - context->Size.CY / 5;
                points[3].X = context->Size.CX / 5;
                points[3].Y = context->Size.CY - context->Size.CY / 5;
                points[4].X = context->Size.CX / 5;
                points[4].Y = context->Size.CY / 5;
                return CentreObject(&data, parm, context, EXAMPLE_VIEW, 0);

    case IMG_RESTRN :

                count[0]    = 5;
                points[0].X = context->Size.CX / 5;
                points[0].Y = context->Size.CY / 5;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY / 5;
                points[2].X = context->Size.CX - context->Size.CX / 5;
                points[2].Y = context->Size.CY - context->Size.CY / 5;
                points[3].X = context->Size.CX / 5;
                points[3].Y = context->Size.CY - context->Size.CY / 5;
                points[4].X = context->Size.CX / 5;
                points[4].Y = context->Size.CY / 5;
                return RestrnObject(&data, parm, context, EXAMPLE_VIEW, 0);

      case IMG_SERIA :
                count[0]    = 7;
                points[0].X = context->Size.CX / 2;
                points[0].Y = context->Size.CY / 2;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY / 2;
                points[2].X = context->Size.CX - context->Size.CX / 5;
                points[2].Y = context->Size.CY / 5;
                points[3].X = context->Size.CX / 5;
                points[3].Y = context->Size.CY / 5;
                points[4].X = context->Size.CX / 5;
                points[4].Y = context->Size.CY - context->Size.CY / 5;
                points[5].X = context->Size.CX / 2;
                points[5].Y = context->Size.CY - context->Size.CY / 5;
                points[6].X = context->Size.CX / 2;
                points[6].Y = context->Size.CY / 2;
                return SeriaObject(&data, parm, context,0,0);
       case IMG_DEPARE :
                count[0]    = 5;
                points[0].X = context->Size.CX / 5;
                points[0].Y = context->Size.CY / 5;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY / 5;
                points[2].X = context->Size.CX - context->Size.CX / 5;
                points[2].Y = context->Size.CY - context->Size.CY / 5;
                points[3].X = context->Size.CX / 5;
                points[3].Y = context->Size.CY - context->Size.CY / 5;
                points[4].X = context->Size.CX / 5;
                points[4].Y = context->Size.CY / 5;
                return DepareObject(&data, parm, context,0,0);
       case IMG_OBSTRN :
                count[0]    = 5;
                points[0].X = context->Size.CX / 5;
                points[0].Y = context->Size.CY / 5;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY / 5;
                points[2].X = context->Size.CX - context->Size.CX / 5;
                points[2].Y = context->Size.CY - context->Size.CY / 5;
                points[3].X = context->Size.CX / 5;
                points[3].Y = context->Size.CY - context->Size.CY / 5;
                points[4].X = context->Size.CX / 5;
                points[4].Y = context->Size.CY / 5;
                return ObstrnObject(&data, parm, context,0,0);
       case IMG_VECTNATIONAL:                             // 19/01/09
                count[0]    = 2;
                points[0].X = context->Size.CX / 5;
                points[0].Y = context->Size.CY / 2;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY / 2;
                return NationalVectObject(&data, parm, context,0,0);
       case IMG_DEPCONTOUR:                // 27/02/09
                count[0]    = 2;
                points[0].X = context->Size.CX / 5;
                points[0].Y = context->Size.CY / 5;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY - context->Size.CY / 5;
                IMGLINE line;
                line.Color = DEPSC;
                line.Thick = 2*260;  // толщина 2 piх
                return imgPrepareImage(&data, IMG_LINE, (char *)&line, context,0,0);
                //DepContourObject(&data, parm, context,0, 0);
       case IMG_DEPCURV:                   // 27/02/09
                count[0]    = 2;
                points[0].X = context->Size.CX / 5;
                points[0].Y = context->Size.CY / 5;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY - context->Size.CY / 5;
                return DepCurvObject(&data, parm, context,0, 0);
       case IMG_SLCONS :    // метрика на все локализации
                count[0]    = 7;
                points[0].X = context->Size.CX / 2;
                points[0].Y = context->Size.CY / 2;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY / 2;
                points[2].X = context->Size.CX - context->Size.CX / 5;
                points[2].Y = context->Size.CY / 5;
                points[3].X = context->Size.CX / 5;
                points[3].Y = context->Size.CY / 5;
                points[4].X = context->Size.CX / 5;
                points[4].Y = context->Size.CY - context->Size.CY / 5;
                points[5].X = context->Size.CX / 2;
                points[5].Y = context->Size.CY - context->Size.CY / 5;
                points[6].X = context->Size.CX / 2;
                points[6].Y = context->Size.CY / 2;
                return ShorelineConstractions(&data, parm, context,0,0);
       case IMG_WRECKS:
                    // метрика на все локализации
                count[0]    = 7;
                points[0].X = context->Size.CX / 2;
                points[0].Y = context->Size.CY / 2;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY / 2;
                points[2].X = context->Size.CX - context->Size.CX / 5;
                points[2].Y = context->Size.CY / 5;
                points[3].X = context->Size.CX / 5;
                points[3].Y = context->Size.CY / 5;
                points[4].X = context->Size.CX / 5;
                points[4].Y = context->Size.CY - context->Size.CY / 5;
                points[5].X = context->Size.CX / 2;
                points[5].Y = context->Size.CY - context->Size.CY / 5;
                points[6].X = context->Size.CX / 2;
                points[6].Y = context->Size.CY / 2;
                return Wrecks(&data, parm, context,0,0);
       case IMG_CBLOHD:
                // Подписи к навесным трубопроводам (середина 1 отрезка)
                count[0]    = 2;
                points[0].X = context->Size.CX / 5;
                points[0].Y = context->Size.CY / 5;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY - context->Size.CY / 5;
                return CbLohdText(&data, parm, context,0, 0);
       case IMG_QUALN:
                count[0]    = 2;
                points[0].X = context->Size.CX / 5;
                points[0].Y = context->Size.CY / 5;
                points[1].X = context->Size.CX - context->Size.CX / 5;
                points[1].Y = context->Size.CY - context->Size.CY / 5;
                return QualinLine(&data, parm, context,0, 0);
      case IMG_SOUNDS:
                 count[0]    = 1;
                 points[0].X = context->Size.CX  - context->Size.CX / 4;
                 points[0].Y = context->Size.CY  - context->Size.CY / 4;
                 data.Semantic = Semantic;
                 return Sounds(&data, parm, context,0,0);

   }

  // КОД НЕ НАЙДЕН
  count[0]    = 2;
  points[0].X = context->Size.CX / 5;
  points[0].Y = context->Size.CY - context->Size.CY / 5;
  points[1].X = context->Size.CX - context->Size.CX / 5;
  points[1].Y = context->Size.CY / 5;

  return imgPrepareImage(&data, IMG_DRAW,
                              (const char *)parm, context, 0, 0);
 }

 long int _IMLAPI imlPaintExample(long int type,
                                  const IMGDRAW * parm,
                                  const IMGCONTEXT * context)
 {
   // Вызвать новую функцию
   return imlPaintExampleEx(type, parm, context, 0, 0);                   // 29/08/13
 }

 // -----------------------------------------------------------------
 // Проверить попадание графического объекта в отображаемую область
 // type - тип объекта (IMLLIST);
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
 // -----------------------------------------------------------------
_IMLIMP long int _IMLAPI imlTestVisible(long int type,
                                 const POLYDATAEX * data,
                                 const IMGDRAW * parm,
                                 const IMGCONTEXT * context)
 {
   IMGLINE line;
   line.Thick = 260;  // 1 пиксел

   if ((context == 0) || ((unsigned)context->Length < sizeof(IMGCONTEXT)))
     return 0;
   switch (type)
   {
     case IMG_ORIENTOBJECT :

                 return OrientObject(data, parm, context,
                                       PREPARE_TEST, 0);

     case IMG_LIGHTS :

                 return Lights(data, parm, context,
                                PREPARE_TEST, 0);

     case IMG_CENTREOBJECT :

                return CentreObject(data, parm, context,
                                PREPARE_TEST, 0);

     case IMG_RESTRN :

                return RestrnObject(data, parm, context,
                                 PREPARE_TEST, 0);
     case IMG_SERIA :

              return SeriaObject(data, parm, context,
                                 PREPARE_TEST, 0);
     case IMG_DEPARE :

              return DepareObject(data, parm, context,
                                    PREPARE_TEST, 0);
     case IMG_OBSTRN :

              return ObstrnObject(data, parm, context,
                                    PREPARE_TEST, 0);

     case IMG_VECTNATIONAL:

              return NationalVectObject(data, parm, context,
                                    PREPARE_TEST, 0);
     case IMG_DEPCONTOUR:                // 27/02/09
              return imgPrepareImage(data, IMG_LINE,
                          (const char *)&line, context,
                               PREPARE_TEST, 0);

              //return DepContourObject(data, parm, context,
              //                      PREPARE_TEST, 0);

     case IMG_DEPCURV:                   // 27/02/09
              return imgPrepareImage(data, IMG_LINE,
                          (const char *)&line, context,
                               PREPARE_TEST, 0);
              //return DepCurvObject(data, parm, context,
              //                      PREPARE_TEST, 0);
     case IMG_SLCONS :

             return ShorelineConstractions(data, parm, context,
                                           PREPARE_TEST, 0);
    case IMG_WRECKS:

              return Wrecks(data, parm, context,PREPARE_TEST, 0);


    case IMG_CBLOHD:
              return CbLohdText(data, parm, context,PREPARE_TEST, 0);

    case IMG_QUALN:
              return QualinLine(data, parm, context,PREPARE_TEST,0);

    case IMG_SOUNDS:
              return Sounds(data, parm, context,PREPARE_TEST, 0);

   }

   return imgPrepareImage(data, IMG_LINE,
                          (const char *)&line, context,
                               PREPARE_TEST, 0);
 }


// -----------------------------------------------------------------
// Действия при открытии библиотеки
// При ошибке возвращает ноль
// -----------------------------------------------------------------
_IMLIMP long int _IMLAPI imlInit(const char * inifile)  // 18/03/09
{
#if defined(LINUXAPI) || defined(WINCE)

   TMapProfile* File;

  try
  {
      TMapProfile* File;
      File = new TMapProfile("S57NAVY", inifile);

      IML_LANGUAGE        = File->GetInt("LANGUAGE", 1);
      SHALLOW_CONTOUR  = File->GetInt("SHALLOW_CONTOUR", 20);
      SHALLOW_CONTOUR = SHALLOW_CONTOUR/10.0;
      DEEP_CONTOUR     = File->GetInt("DEEP_CONTOUR", 300);
      DEEP_CONTOUR = DEEP_CONTOUR/10.0;
      SAFETY_CONTOUR   = File->GetInt("SAFETY_CONTOUR", 300);
      SAFETY_CONTOUR = SAFETY_CONTOUR/10.0;
      TWO_SHADES       = File->GetInt("TWO_SHADES", 1);
      SHALLOW_PATTERNS = File->GetInt("SHALLOW_PATTERNS", 1);
      SHOW_ISDANGER_SHALLOW = File->GetInt("SHOW_ISDANGER_SHALLOW", 0);

      delete File;
   }
  catch(...)
   {
   }
#else
      /*
      TIniFile *ini = new TIniFile(name);
      if (!ini) return;
      count = ini->ReadInteger("USERS","Count",0);
      for (i = 0; i < count; i++)
        {
          strall = str + i;
          string = ini->ReadString("USERS",strall,"");
          if (string != "")
            CbLogin->Items->Add(string);
        }
      // Считываем последнего пользователя
      string = ini->ReadString("LASTUSER","user","");      // 18/04/08
      if (string != "")   // есть запись с именем последнего пользователя
        {
         // поставим текущим
         for (i = 0; i < CbLogin->Items->Count; i++)
           {
             if (string == CbLogin->Items->Strings[i])
               {
                 CbLogin->ItemIndex = i;
                 break;
               }

           }
          // Переведем фокус ввода
          if (CbLogin->ItemIndex >= 0)
             {
                BtSave->Enabled = true;
               // MaskEdit1->SetFocus();
             }
        }
      delete ini;
        */
#endif
  return 1;
}


#ifdef __cplusplus
} // extern "C"
#endif


