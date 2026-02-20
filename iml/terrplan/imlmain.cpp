/******* IMLMAIN.CPP ************ Panteleeva N.A. ****** 13/01/16 **
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2016              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*      ОПИСАНИЕ ПАРАМЕТРОВ ФУНКЦИЙ ВИЗУАЛИЗАЦИИ ПРОИЗВОЛЬНЫХ       *
*             ГРАФИЧЕСКИХ ОБЪЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ               *
*                   ДЛЯ ТЕРРИТОРИАЛЬНОГО ПЛАНИРОВАНИЯ              *
*******************************************************************/
#ifndef IMLSERV_H
  #include "imlserv.h"
#endif

#ifndef TERRPLAN_H
  #include "terrplan.h"
#endif

// Простой векторный объект изменение размера и цвета
IMLPARM ImlSimpleObject =
{
  IMLIDENT, 1, 1, 0, 0, 0,
  {{ IMG_VECTOREX, "BaseMark parameters", "Параметры основного знака", 0, 0 },
  }
};

// Сложный векторный - изображение основного объекта либо планируемого, сверху
// часть объекта, изменяющяя цвет
IMLPARM ImlComplexObject =
{
  IMLIDENT, 3, 1, 0, 0, 0,
  {{ IMG_VECTOREX, "BaseMark parameters", "Параметры основного знака", 0, 0 },
   { IMG_VECTOREX, "PlanMark parameters", "Параметры планируемого знака", 0, 0 },
   { IMG_VECTOREX, "СolorPart parameters", "Параметры части, меняющей цвет", 0, 0 }
  }
};


// Линия - изображение основного объекта либо планируемого, сверху
// часть объекта, изменяющяя цвет
IMLPARM ImlLineObject =
{
  IMLIDENT, 3, 1, 0, 0, 0,
  {{ IMG_DRAWLINE, "BaseLine parameters", "Параметры основной линии", 0, 0 },
   { IMG_DRAWLINE, "PlanLine parameters", "Параметры планируемой линии", 0, 0 },
   { IMG_DRAWLINE, "СolorLine parameters", "Параметры линии, меняющей цвет", 0, 0 }
  }
};

// Окантовка особых природных территорий
IMLPARM ImlPTContour =
{
  IMLIDENT, 2, 1, 0, 0, 0,
  {{ IMG_LINE, "BaseLine parameters", "Параметры основной линии", 0, 0 },
   { IMG_LINE, "СolorLine parameters", "Параметры линии, меняющей цвет", 0, 0 }
  }
};

// Простой векторный объект изменение размера( c указанием семантики) и цвета
IMLPARM ImlSemSizeObject =
{
  IMLIDENT, 2, 1, 0, 0, 0,
  {
    { IMG_VECTOREX, "BaseMark parameters", "Параметры основного знака", 0, 0 },
    { IMG_SEMANTIC, "Semantic for changing size",
                    "Семантика для изменения размера", 0, 0 },
  }
};

// Окантовка участков недр
IMLPARM ImlBowelsContour =
{
  IMLIDENT, 2, 1, 0, 0, 0,
  {
    { IMG_DRAWLINE,"Base contour", "Oсновной контур", 0, 0 },
    { IMG_DRAWLINE, "Reserved contour", "Резервный контур", 0, 0 },
  }
};

// Модифицированный TrueType объект
// изменение размера(c указанием семантики и ее значений)
// изменение цвета (с указанием значения и соотв цветов)
IMLPARM ImlModyTrueType =
{
  IMLIDENT, 15, 1, 0, 0, 0,
  {
    { IMG_TRUETYPE, "BaseMark parameters", "Параметры основного знака", 0, 0 },
    { IMG_SEMANTIC, "Semantic for changing size",
                    "Семантика для изменения размера", 0, 0 },
    { IMG_LONG,     "Semantic value for 140%",
                    "Значение семантики для 140% размера", 0, 0 },
    { IMG_LONG,     "Semantic value for 120%",
                    "Значение семантики для 120% размера", 0, 0 },
    { IMG_SEMANTIC, "Semantic for changing color",
                    "Семантика для изменения цвета", 0, 0 },
    { IMG_LONG,     "Semantic value for 1 color",
                    "Значение семантики для 1 цвета", 0, 0 },
    { IMG_COLOR,    "1 color",
                    "1 цвет", 0, 0 },
    { IMG_LONG,     "Semantic value for 2 color",
                    "Значение семантики для 2 цвета", 0, 0 },
    { IMG_COLOR,    "2 color",
                    "2 цвет", 0, 0 },
    { IMG_LONG,     "Semantic value for 3 color",
                    "Значение семантики для 3 цвета", 0, 0 },
    { IMG_COLOR,    "3 color",
                    "3 цвет", 0, 0 },
    { IMG_LONG,     "Semantic value for 4 color",
                    "Значение семантики для 4 цвета", 0, 0 },
    { IMG_COLOR,    "4 color",
                    "4 цвет", 0, 0 },
    { IMG_LONG,     "Semantic value for 5 color",
                    "Значение семантики для 5 цвета", 0, 0 },
    { IMG_COLOR,    "5 color",
                    "5 цвет", 0, 0 },
  }
};

// Графический знак, меняющий фон и окантовку
IMLPARM ImlPicture =
{
  IMLIDENT, 1, 1, 0, 0, 0,
  {
    { IMG_PICTURE, "Base mark", "Oсновной знак", 0, 0 },
  }
};

// Линия с учетом статуса
IMLPARM ImlObjLine =
{
  IMLIDENT, 4, 1, 0, 0, 0,
  {
    { IMG_DRAWLINE, "Base line", "Oсновная линия", 0, 0 },
    { IMG_DRAWLINE, "Plan line", "Планируемая линия", 0, 0 },
    { IMG_DRAWLINE, "Reconstraction line", "Линия для реконструкции", 0, 0 },
    { IMG_DRAWLINE, "Deletion line", "Удаляемая линия", 0, 0 },
  }
};


// Площадь с учетом статуса
IMLPARM ImlObjSquare =
{
  IMLIDENT, 4, 1, 0, 0, 0,
  {
    { IMG_DRAWSQUARE, "Base square", "Oсновная площадь", 0, 0 },
    { IMG_DRAWSQUARE, "Plan square", "Планируемая площадь", 0, 0 },
    { IMG_DRAWSQUARE, "Reconstraction square", "Площадь для реконструкции", 0, 0 },
    { IMG_DRAWSQUARE, "Deletion square", "Удаляемая площадь", 0, 0 },
  }
};

IMLLIST ImlList =
{
  IMLIDENT, 10,
  {{ IMG_SIMPLEOBJECT, &ImlSimpleObject,
                       "Simple Mark", "Простой знак", 0, 0 },
   { IMG_COMPLEXOBJECT,   &ImlComplexObject,
                       "Complex Mark", "Сложный знак", 0, 0 },
   { IMG_LINEOBJECT,   &ImlLineObject,
                        "LineObject", "Линейный объект", 0, 0 },
   { IMG_PTCONTOUR,    &ImlPTContour,
                        "PTContour", "Окантовка ООПТ", 0, 0 },
   { IMG_SEMSIZEOBJECT,  &ImlSemSizeObject,
                        "Mark - size from semantic", "Знак - размер по семантике", 0, 0 },
   { IMG_BOWELSCONTOUR,    &ImlBowelsContour,
                        "BowelsContour", "Окантовка недр", 0, 0 },
   { IMG_MODYTRUETYPE,    &ImlModyTrueType,
                        "ModyTrueType", "Изменяемый TrueType", 0, 0 },
   { IMG_OBJMARK,      &ImlPicture,
                        "Sign with account statuses", "Знак с учетом статусов", 0, 0 },
   { IMG_OBJLINE,      &ImlObjLine,
                        "Line with account statuses", "Линия с учетом статусов", 0, 0 },
   { IMG_OBJSQUARE,    &ImlObjSquare,
                        "Square with account statuses", "Площадь с учетом статусов", 0, 0 },

  }
};

extern "C"
{
 // -----------------------------------------------------------------
 // Запросить список видов графических объектов,
 // поддерживаемых библиотекой
 // -----------------------------------------------------------------
_IMLIMP const IMLLIST * _IMLAPI imlGetImageList(void)
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
     case IMG_SIMPLEOBJECT :

                 return SimpleObject(data, parm, context,
                                           PREPARE_BORDER, border);

    case IMG_COMPLEXOBJECT :

                 return ComplexObject(data, parm, context, PREPARE_BORDER, border);

    case IMG_LINEOBJECT :

                 return LineObject(data, parm, context, PREPARE_BORDER, border);

    case IMG_PTCONTOUR :

                 return ContourPT(data, parm, context, PREPARE_BORDER, border);

    case IMG_SEMSIZEOBJECT:

                 return SemSizeObject(data, parm, context, PREPARE_BORDER, border);

    case IMG_BOWELSCONTOUR:

                 return BowelsContour(data, parm, context, PREPARE_BORDER, border);

    case IMG_MODYTRUETYPE:

                 return ModyTrueType(data, parm, context, PREPARE_BORDER, border);

    case IMG_OBJMARK:
                 return ObjectMark(data, parm, context, PREPARE_BORDER, border);
    case IMG_OBJLINE:
                 return ObjectLine(data, parm, context, PREPARE_BORDER, border);
    case IMG_OBJSQUARE:
                 return ObjectSquare(data, parm, context, PREPARE_BORDER, border);
    }

   return 0;
 }

 // -----------------------------------------------------------------
 // Отобразить объект в экранном виде
 // type - тип объекта (IMLLIST);
 // hmap - идентификатор карты;
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

   if (data == 0)
     return 0;

   switch (type)
   {
     case IMG_SIMPLEOBJECT :

                 return SimpleObject(data, parm, context, 0, 0);

     case IMG_COMPLEXOBJECT :

                 return ComplexObject(data, parm, context, 0, 0);

     case IMG_LINEOBJECT :

                 return LineObject(data, parm, context, 0, 0);

     case IMG_PTCONTOUR :

                 return ContourPT(data, parm, context, 0, 0);

     case IMG_SEMSIZEOBJECT:

                 return SemSizeObject(data, parm, context, 0, 0);

     case IMG_BOWELSCONTOUR:

                  return BowelsContour(data, parm, context, 0, 0);

     case IMG_MODYTRUETYPE:

                 return ModyTrueType(data, parm, context, 0, 0);

     case IMG_OBJMARK:
                 return ObjectMark(data, parm, context, 0, 0);

     case IMG_OBJLINE:
                 return ObjectLine(data, parm, context, 0, 0);

     case IMG_OBJSQUARE:
                 return ObjectSquare(data, parm, context, 0, 0);

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
_IMLIMP long int _IMLAPI imlPaintExample(long int type,
                                  const IMGDRAW * parm,
                                  const IMGCONTEXT * context)
 {
   POLYDATAEX data;
   DRAWPOINT points[5];
   int  count[2];                                    // 26/03/12
   FRAME  vborder;                                   // 30/03/09
   const char * vparm;
   float scale = 1.0;
   float scale1 = 1.0;
   long vfunc;
   char * text = "Abc123";



   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;

   IMGCONTEXT contextd;                            // 19/03/09
   IMAGESIZE  imgsize;
   int hsize;
   int vsize;
   int dh;
   int dv;
   memcpy((char *)&contextd,context,sizeof(IMGCONTEXT));

   memset((void*)&imgsize, 0, sizeof(IMAGESIZE));
   memset((void*)&data, 0, sizeof(data));

   data.Count      = 1;
   data.PolyCounts = &count[0];
   data.Points     = &points[0];

   data.Border.LT  = 0;
   data.Border.UP  = 0;
   data.Border.RT  = context->Size.cx;
   data.Border.DN  = context->Size.cy;

   data.Semantic   = 0;

   switch (type)
   {
     case IMG_SIMPLEOBJECT :
     case IMG_COMPLEXOBJECT :
     case IMG_SEMSIZEOBJECT:
         count[0]    = 2;
         data.ShowScale = 1000;
         scale = 1.0;
         vparm = imlDrawParm(parm, 1);
         imlGetRscVectorSize((IMGVECTOREX *)vparm, &imgsize,0, 0);    // 30/03/09

         hsize = imgsize.HorizontalSize * contextd.PixelX;            // 13/01/16
         vsize = imgsize.VerticalSize * contextd.PixelY;

         if (hsize > (float)(contextd.Size.cx * 0.9))
            scale = (float)(contextd.Size.cx * 0.9)/(hsize + 1);
         if (vsize > (float)(contextd.Size.cy * 0.9))
            scale1 = (float)(contextd.Size.cy * 0.9)/(vsize + 1);
         if (scale1 < scale) scale = scale1;

         contextd.Scale = /*contextd.Scale*/ scale;  

         dh    = imgsize.DeltaH * contextd.PixelX*scale;
         dv    = imgsize.DeltaV * contextd.PixelY*scale;
         points[0].x = (context->Size.cx - hsize*scale)/2 + dh;
         points[1].x = context->Size.cx;
         points[0].y = (context->Size.cy - vsize*scale)/2 + dv;
         points[1].y = points[0].y;
         if (type == IMG_SIMPLEOBJECT)
                 return SimpleObject(&data, parm, &contextd, 0, 0);
         if (type == IMG_COMPLEXOBJECT)
                 return ComplexObject(&data, parm, &contextd, 0, 0);
         if (type == IMG_SEMSIZEOBJECT)
                 return SemSizeObject(&data, parm, &contextd, 0, 0);

    case IMG_LINEOBJECT :
               	 //contextd.Scale = 1;
                 count[0]    = 2;
                 points[0].x = context->Size.cx / 5;
                 points[0].y = context->Size.cy - context->Size.cy / 5;
                 points[1].x = context->Size.cx - context->Size.cx / 5;
                 points[1].y = context->Size.cy / 5;

                 return LineObject(&data, parm, &contextd, 0, 0);

    case IMG_PTCONTOUR :

                 count[0]    = 5;
                 points[0].x = context->Size.cx / 5;
                 points[0].y = context->Size.cy - context->Size.cy / 5;
                 points[1].x = context->Size.cx - context->Size.cx /5;
                 points[1].y = context->Size.cy - context->Size.cy / 5;
                 points[2].x = context->Size.cx - context->Size.cx /5;
                 points[2].y = context->Size.cy / 5;
                 points[3].x = context->Size.cx / 5;
                 points[3].y = context->Size.cy / 5;
                 points[4].x = context->Size.cx / 5;
                 points[4].y = context->Size.cy - context->Size.cy / 5;

                 return ContourPT(&data, parm, context, 0, 0);

     case IMG_BOWELSCONTOUR:
                 count[0]    = 5;
                 points[0].x = context->Size.cx / 5;
                 points[0].y = context->Size.cy - context->Size.cy / 5;
                 points[1].x = context->Size.cx - context->Size.cx /5;
                 points[1].y = context->Size.cy - context->Size.cy / 5;
                 points[2].x = context->Size.cx - context->Size.cx /5;
                 points[2].y = context->Size.cy / 5;
                 points[3].x = context->Size.cx / 5;
                 points[3].y = context->Size.cy / 5;
                 points[4].x = context->Size.cx / 5;
                 points[4].y = context->Size.cy - context->Size.cy / 5;

                 return BowelsContour(&data, parm, context, 0, 0);

      case IMG_MODYTRUETYPE:

                 count[0]    = 2;
                 points[0].x = context->Size.cx / 3;
                 points[0].y = context->Size.cy / 2;
                 points[1].x = context->Size.cx  - context->Size.cx / 3;
                 points[1].y = context->Size.cy/2;

                 return ModyTrueType(&data, parm, context, 0, 0);

         case IMG_OBJMARK:
                 count[0]    = 1;
                 points[0].x = context->Size.cx / 2;
                 points[0].y = context->Size.cy / 2;

                 return ObjectMark(&data, parm, context, 0, 0);

     case IMG_OBJLINE:
                 count[0]    = 2;
                 points[0].x = context->Size.cx / 5;
                 points[0].y = context->Size.cy - context->Size.cy / 5;
                 points[1].x = context->Size.cx - context->Size.cx / 5;
                 points[1].y = context->Size.cy / 5;

                 return ObjectLine(&data, parm, context, 0, 0);

     case IMG_OBJSQUARE:
                 count[0]    = 5;
                 points[0].x = context->Size.cx / 5;
                 points[0].y = context->Size.cy - context->Size.cy / 5;
                 points[1].x = context->Size.cx - context->Size.cx /5;
                 points[1].y = context->Size.cy - context->Size.cy / 5;
                 points[2].x = context->Size.cx - context->Size.cx /5;
                 points[2].y = context->Size.cy / 5;
                 points[3].x = context->Size.cx / 5;
                 points[3].y = context->Size.cy / 5;
                 points[4].x = context->Size.cx / 5;
                 points[4].y = context->Size.cy - context->Size.cy / 5;

                 return ObjectSquare(&data, parm, context, 0, 0);


    default :   // КОД НЕ НАЙДЕН

                 count[0]    = 2;
                 points[0].x = context->Size.cx / 5;
                 points[0].y = context->Size.cy - context->Size.cy / 5;
                 points[1].x = context->Size.cx - context->Size.cx / 5;
                 points[1].y = context->Size.cy / 5;

                 imgPrepareImage(&data, IMG_DRAW,
                                   (const char *)parm, context, 0, 0);
                 break;
   }

   return 0;
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

   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;

   switch (type)
   {
     case IMG_SIMPLEOBJECT :

                return SimpleObject(data, parm, context, PREPARE_TEST, 0);

     case IMG_COMPLEXOBJECT :

                 return ComplexObject(data, parm, context, PREPARE_TEST, 0);

     case IMG_LINEOBJECT :

                 return LineObject(data, parm, context, PREPARE_TEST, 0);

     case IMG_PTCONTOUR :

                 return ContourPT(data, parm, context, PREPARE_TEST, 0);

     case IMG_SEMSIZEOBJECT:

                 return SemSizeObject(data, parm, context, PREPARE_TEST, 0);

     case IMG_BOWELSCONTOUR:

                 return BowelsContour(data, parm, context, PREPARE_TEST, 0);

     case IMG_MODYTRUETYPE:

                 return ModyTrueType(data, parm, context, PREPARE_TEST, 0);

     case IMG_OBJMARK:
                 return ObjectMark(data, parm, context, PREPARE_TEST, 0);

     case IMG_OBJLINE:
                 return ObjectLine(data, parm, context, PREPARE_TEST, 0);

     case IMG_OBJSQUARE:
                 return ObjectSquare(data, parm, context, PREPARE_TEST, 0);

   }

   return imgPrepareImage(data, IMG_LINE,
                          (const char *)&line, context, PREPARE_TEST, 0);
 }

} // extern "C"







