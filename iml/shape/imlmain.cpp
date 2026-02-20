/****** IMLMAIN.CPP ************ Belenkov O.V.   ****** 06/03/15 ***
******* IMLMAIN.CPP ************ Kruzhkov A.E.   ****** 23/07/15 ***
******* IMLMAIN.CPP ************ Dishlenko S.G.  ****** 26/03/12 ***
******* IMLMAIN.CPP ************ Derevyagina Zh.A.***** 16/11/15 ***
******* IMLMAIN.CPP ************ Panteleeva N.A. ****** 14/03/16 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2016              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*      ОПИСАНИЕ ПАРАМЕТРОВ ФУНКЦИЙ ВИЗУАЛИЗАЦИИ ПРОИЗВОЛЬНЫХ       *
*             ГРАФИЧЕСКИХ ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ               *
*                                                                  *
*******************************************************************/

#ifndef IMLSERV_H
  #include "imlserv.h"
#endif

#ifndef SHAPE_H
  #include "shape.h"
#endif

// ТЕКСТОВАЯ ИНФОРМАЦИЯ НА ПОДДЕРЖИВАЕМЫХ СИСТЕМОЙ ЯЗЫКАХ (до 4)
// ML_ENGLISH (1), ML_RUSSIAN(2), ...

// Линия с утолщением в конце
IMLPARM ImlThickenLine =
{
  IMLIDENT, 2, 1, 0, 0, 0,
  {{ IMG_LINE, "Line parameters", "Параметры линии", 0, 0},
   { IMG_SEMANTIC, "Semantic number for last point thick in mm", "Номер семантики - толщина окончания в мм", 0, 0},
  }
};

// Линия с утолщением в середине
IMLPARM ImlMiddleThickenLine =
{
  IMLIDENT, 2, 1, 0, 0, 0,
  {{ IMG_LINE, "Line parameters", "Параметры линии", 0, 0},
   { IMG_SEMANTIC, "Semantic number for middle thick in mm", "Номер семантики - толщина середины в мм", 0, 0},
  }
};

// Условный знак вдоль линии
IMLPARM ImlMarkAlongLine =
{
  IMLIDENT, 5, 0, 0, 0, 0,
  {{ IMG_VECTOREX, "Mark parameters", "Параметры знака", 0, 0},
   { IMG_DOUBLE,   "Mark distance in mm",  "Расстояние между знаками в мм", 0, 0},
   { IMG_DOUBLE,   "Mark shift in mm",  "Смещение от осевой линии в мм", 0, 0},
   { IMG_DRAWLINE, "Down line parameters", "Параметры фоновой линии", 0, 0},
   { IMG_DRAWLINE, "Top line parameters", "Параметры накладываемой линии", 0, 0},
  }
};

// Зеркальный знак вдоль линии
IMLPARM ImlMirrorMarkAlongLine =
{
  IMLIDENT, 5, 0, 0, 0, 0,
  {{ IMG_VECTOREX, "Mark parameters", "Параметры знака", 0, 0},
   { IMG_DOUBLE,   "Mark distance in mm",  "Расстояние между знаками в мм", 0, 0},
   { IMG_DOUBLE,   "Mark shift in mm",  "Смещение от осевой линии в мм", 0, 0},
   { IMG_DRAWLINE, "Down line parameters", "Параметры фоновой линии", 0, 0},
   { IMG_DRAWLINE, "Top line parameters", "Параметры накладываемой линии", 0, 0},
  }
};

// Условный знак внутри двойной линии
IMLPARM ImlMarkIntoLine =
{
  IMLIDENT, 4, 0, 0, 0, 0,
  {{ IMG_VECTOREX, "Mark parameters", "Параметры знака", 0, 0},
   { IMG_DOUBLE,   "Mark distance in mm",  "Расстояние между знаками в мм", 0, 0},
   { IMG_DOUBLE,   "Line shift in mm",  "Смещение линии от оси в мм", 0, 0},
   { IMG_DRAWLINE, "Line parameters", "Параметры линии", 0, 0},
  }
};

// Знак с поворотом по семантике
IMLPARM ImlTurnMark =
{
  IMLIDENT, 2, 1, 0, 0, 0,
  {{ IMG_VECTOREX, "Mark parameters", "Параметры знака", 0, 0},
   { IMG_SEMANTIC, "Semantic number for angle in degree", "Номер семантики - угол в градусах", 0, 0},
  }
};


// Знак с поворотом и длиной линии по семантике
IMLPARM ImlTurnAndLengthMark =
{
  IMLIDENT, 3, 1, 0, 0, 0,
  {{ IMG_DRAWLINE, "Mark parameters", "Параметры знака", 0, 0},
   { IMG_SEMANTIC, "Semantic number for mark angle in degree", "Номер семантики - угол в градусах", 0, 0},
   { IMG_SEMANTIC, "Semantic number for mark length in mm on the map", "Номер семантики - длина в мм на карте", 0, 0},
  }
};

// Горизонтальная штриховка полигона
IMLPARM ImlHorizontalShade =
{
  IMLIDENT, 3, 0, 0, 0, 0,
  {{ IMG_LINE,     "Line parameters", "Параметры штриховки", 0, 0},
   { IMG_DOUBLE,   "Interval in mm",  "Интервал между штрихами в мм", 0, 0},
   { IMG_DRAWLINE, "Border line", "Контурная линия", 0, 0},
  }
};

// Текст с параметрами из семантики
IMLPARM ImlUniversalText =
{
  IMLIDENT, 3, 1, 0, 0, 0,
  {{ IMG_TEXT,     "Text parameters", "Параметры шрифта", 0, 0},
   { IMG_SEMANTIC, "Semantic number for font height (mm)", "Номер семантики - высота шрифта (мм)", 0, 0},
   { IMG_SEMANTIC, "Semantic number for font name", "Номер семантики - название шрифта", 0, 0},
  }
};

// Масштабируемый векторный знак
IMLPARM ImlScaleVector =
{
  IMLIDENT, 2, 1, 0, 0, 0,
  {{ IMG_VECTOREX, "Mark parameters", "Параметры знака", 0, 0},
   { IMG_SEMANTIC, "Semantic number for scale (%)", "Номер семантики - масштаб знака (%)", 0, 0},
  }
};

// Знак с выноской
IMLPARM ImlMarkAndNote =
{
  IMLIDENT, 11, 1, 0, 0, 0,
  {{ IMG_VECTOREX, "Mark parameters", "Параметры знака", 0, 0},
   { IMG_LINE,     "Line parameters", "Параметры линии", 0, 0},
   { IMG_TEXT,     "Text parameters", "Параметры шрифта", 0, 0},
   { IMG_SEMANTIC, "Semantic number for numerator", "Номер семантики - в числитель", 0, 0},
   { IMG_SEMANTIC, "Semantic number for denominator", "Номер семантики - в знаменатель", 0, 0},
   { IMG_SEMANTIC, "Semantic number for denominator", "Номер семантики - в знаменатель", 0, 0},
   { IMG_SEMANTIC, "Semantic number for denominator", "Номер семантики - в знаменатель", 0, 0},
   { IMG_SEMANTIC, "Semantic number for denominator", "Номер семантики - в знаменатель", 0, 0},
   { IMG_SEMANTIC, "Semantic number for denominator", "Номер семантики - в знаменатель", 0, 0},
   { IMG_SEMANTIC, "Semantic number for denominator", "Номер семантики - в знаменатель", 0, 0},
   { IMG_SEMANTIC, "Semantic number for denominator", "Номер семантики - в знаменатель", 0, 0},
  }
};

// Объект с тенью
IMLPARM ImlShadowObject =              // IMG_SQUAREGLASS
{
  IMLIDENT, 4, 0, 0, 0, 0,
  {{ IMG_DRAWSQUARE, "Object parameters", "Параметры знака", 0, 0},
   { IMG_DRAWSQUARE, "Object parameters", "Параметры тени", 0, 0},
   { IMG_DOUBLE, "Shift along X mm", "Сдвиг по X мм", 0, 0},
   { IMG_DOUBLE, "Shift along Y mm", "Сдвиг по Y мм", 0, 0},
  }
};

// Линия с утолщением от начала до конца
IMLPARM ImlDoubleThickenLine =
{
  IMLIDENT, 3, 1, 0, 0, 0,
  {{ IMG_LINE, "Line parameters", "Параметры линии", 0, 0},
   { IMG_SEMANTIC, "Semantic number for first point thick in mm", "Номер семантики - толщина начала в мм", 0, 0},
   { IMG_SEMANTIC, "Semantic number for last point thick in mm", "Номер семантики - толщина окончания в мм", 0, 0},
  }
};

// Дуга по ломаной линии (параллели, меридианы и пр.)  02/04/06
IMLPARM ImlPolyArc =
{
  IMLIDENT, 1, 0, 0, 0, 0,
  {{ IMG_DRAWLINE, "Line parameters", "Параметры линии", 0, 0},    // 30/06/06
  }
};

// Линия с подписью длины
IMLPARM ImlLengthAndAngleLine =
{
  IMLIDENT, 2, 0, 0, 0, 0,
  {{ IMG_TEXT,     "Text parameters", "Параметры шрифта", 0, 0},
   { IMG_LONG,     "Decimal places number", "Число знаков после запятой", 0, 0 }
  }
};

// Окружность по двум точкам с градиентной заливкой
IMLPARM ImlGradientCircle =
{
  IMLIDENT, 5, 0, 0, 0, 0,
  {{ IMG_COLOR, "Brush parameters", "Параметры заливки", 0, 0},
   { IMG_LONG,  "Bright in center, percent", "Процент яркости в центре", 0, 0 },
   { IMG_LONG,  "Bright at border, percent", "Процент яркости на краю", 0, 0 },
   { IMG_LONG,  "Transparency, percent", "Процент прозрачности", 0, 0 },
   { IMG_LONG,  "Zone number", "Число зон яркости", 0, 0 }
  }
};

// Условный знак, видимый в заданном диапазоне масштабов отображения
IMLPARM ImlScaleLimitedImage =
{
  IMLIDENT, 3, 1, 0, 0, 0,
  {{ IMG_LONG,  "Minimal scale limit", "Крайний масштаб при увеличении", 0, 0 },
   { IMG_LONG,  "Maximal scale limit", "Крайний масштаб при уменьшении", 0, 0 },
   { IMG_DRAW,  "Object image", "Условный знак", 0, 0 },
  }
};

// Приподнятый многоугольник                                    // 11/12/14
IMLPARM ImlQuarterImage =
{
  IMLIDENT, 7, 1, 0, 0, 0,
  {{ IMG_LONG,       "Height scale", "Масштаб по высоте", 0, 0 },
   { IMG_SEMANTIC,   "Semantic for Height", "Семантика - высота объекта", 0, 0 },
   { IMG_DRAWSQUARE, "Side parameters", "Параметры боковой стороны", 0, 0},
   { IMG_DRAWLINE,   "Edge parameters", "Параметры линии ребер", 0, 0},
   { IMG_DRAWSQUARE, "Top parameters",  "Параметры поверхности", 0, 0},
   { IMG_TEXT,       "Text parameters", "Параметры шрифта", 0, 0},
   { IMG_SEMANTIC,   "Semantic for label", "Семантика - текст подписи", 0, 0 },
  }
};

// Отображение графических файлов на карте
IMLPARM ImlImageView =
{
  IMLIDENT, 1, 1, 0, 0, 0,
  {{ IMG_SEMANTIC, "Semantic number - reference to file", "Номер семантики - ссылка на файл", 0, 0 },
  }
};

// Линия с постоянным контуром                                  // 10/03/16
IMLPARM ImlContouringLine =
{
  IMLIDENT, 4, 0, 0, 0, 0,
  {{ IMG_DOUBLE,     "Fon thick in mm",  "Ширина фоновой линии в мм", 0, 0},
   { IMG_COLOR,      "Foncolor", "Цвет фоновой линии", 0, 0 },
   { IMG_DOUBLE,     "Contour thick in mm",  "Ширина контура в мм", 0, 0},
   { IMG_COLOR,      "Contour color", "Цвет контура", 0, 0 },
  }
};

IMLLIST ImlList =
{
  IMLIDENT, 20,          // <--- Число видов знаков !!!
  {{ IMG_MARKALONGLINE, &ImlMarkAlongLine,
                        "Mark along line", "Знак вдоль линии", 0, 0},
   { IMG_MIRRORMARKALONGLINE, &ImlMirrorMarkAlongLine,
                        "Mirror mark along line", "Зеркальный знак вдоль линии", 0, 0},
   { IMG_MARKINTOLINE, &ImlMarkIntoLine,
                        "Mark into line", "Знак внутри зеркальной линии", 0, 0},
   { IMG_TURNMARK, &ImlTurnMark,
                        "Turn mark", "Знак с поворотом по семантике", 0, 0},
   { IMG_HORIZONTALSHADE, &ImlHorizontalShade,
                        "Horizontal shade",
                        "Горизонтальная штриховка полигона", 0, 0},
   { IMG_UNIVERSALTEXT, &ImlUniversalText,
                        "Text with parameters from data",
                        "Шрифт с параметрами из семантики", 0, 0},
   { IMG_SCALEVECTOR,   &ImlScaleVector,
                        "Scale vector object",
                        "Масштабируемый векторный знак", 0, 0},
   { IMG_THICKENLINEEX,   &ImlThickenLine,
                        "Thicken line in the end",
                        "Утолщенная в конце линия", 0, 0},
   { IMG_MIDDLETHICKENLINEEX,   &ImlMiddleThickenLine,
                        "Thicken line in middle",
                        "Утолщенная в середине линия", 0, 0},
   { IMG_MARKANDNOTE,   &ImlMarkAndNote,
                        "Mark and Note",
                        "Знак с линией выноски и текстом", 0, 0},
   { IMG_SHADOWOBJECT,  &ImlShadowObject,
                        "Object with shadow", "Объект с тенью", 0, 0},
   { IMG_DOUBLETHICKENLINE,   &ImlDoubleThickenLine,
                        "Thicken line",
                        "Утолщенная линия", 0, 0},
   { IMG_POLYARC,        &ImlPolyArc,
                        "Arc along line",
                        "Дуга по ломаной", 0, 0},
   { IMG_TURNANDLENGTHMARK, &ImlTurnAndLengthMark,
                        "Turn and scaling mark",
                        "Знак с длиной и поворотом по семантике", 0, 0},
    { IMG_LENGTHANDANGLELINE, &ImlLengthAndAngleLine,
                        "Line with its length in caption",
                        "Линия с подписью длины", 0, 0},
    { IMG_GRADIENTCIRCLE, &ImlGradientCircle,
                        "Gradient circle zones",
                        "Окружность с градиентной заливкой", 0, 0},
    { IMG_SCALELIMITEDIMAGE, &ImlScaleLimitedImage,
                        "Image with scale limit",
                        "Знак, видимый в заданных масштабах", 0, 0},
    { IMG_QUARTER, &ImlQuarterImage,
                        "Raised polygon",
                        "Приподнятый многоугольник", 0, 0}, 
    { IMG_IMAGEVIEW, &ImlImageView,
                        "Graphic files",
                        "Графические файлы", 0, 0},
    { IMG_CONTOURINGLINE, &ImlContouringLine,"Contouring line",
                          "Линия с контуром", 0, 0},

    { 0, 0, 0, 0, 0, 0},
  }
};

extern "C"
{

 // -----------------------------------------------------------------
 // Запросить список видов графических объектов,
 // поддерживаемых библиотекой
 // -----------------------------------------------------------------

_IMLIMP const IMLLIST * _IMLAPI imlGetImageList(void)               // 16/11/15
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

   if (border == 0)
     return 0;

   switch (type)
   {
     case IMG_MARKALONGLINE:

                 return MarkAlongLine(data, parm, context,
                                         PREPARE_BORDER, border);

     case IMG_HORIZONTALSHADE:

                 return HorizontalShade(data, parm, context,
                                         PREPARE_BORDER, border);

     case IMG_MIRRORMARKALONGLINE:

                 return MirrorMarkAlongLine(data, parm, context,
                                            PREPARE_BORDER, border);

     case IMG_MARKINTOLINE:

                 return MarkIntoLine(data, parm, context,
                                     PREPARE_BORDER, border);

     case IMG_TURNMARK:

                 return TurnMark(data, parm, context,
                                 PREPARE_BORDER, border);

     case IMG_TURNANDLENGTHMARK:

                 return TurnAndLengthMark(data, parm, context,
                                 PREPARE_BORDER, border);

     case IMG_UNIVERSALTEXT:

                 return UniversalText(data, parm, context,
                                 PREPARE_BORDER, border);

     case IMG_SCALEVECTOR:

                 return ScaleVector(data, parm, context,
                                    PREPARE_BORDER, border);

     case IMG_THICKENLINEEX:

                 return ThickenLine(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_MIDDLETHICKENLINEEX:

                 return MiddleThickenLine(data, parm, context,
                                          PREPARE_BORDER, border);
     case IMG_MARKANDNOTE:

                 return MarkAndNote(data, parm, context,
                                    PREPARE_BORDER, border);
     case IMG_SHADOWOBJECT:
     case IMG_COLORSHADOWOBJECT:

                 return ShadowObject(data, parm, context,
                                     PREPARE_BORDER, border);
     case IMG_DOUBLETHICKENLINE:

                 return DoubleThickenLine(data, parm, context,
                                          PREPARE_BORDER, border);
     case IMG_POLYARC:

                 return PolyArc(data, parm, context,
                                PREPARE_BORDER, border);

     case IMG_LENGTHANDANGLELINE:

                 return LengthAndAngleLine(data, parm, context,
                                           PREPARE_BORDER, border);

     case IMG_GRADIENTCIRCLE:

                 return GradientCircle(data, parm, context,
                                       PREPARE_BORDER, border);

     case IMG_SCALELIMITEDIMAGE:

                 return ScaleLimitedImage(data, parm, context,
                                          PREPARE_BORDER, border);

     case IMG_QUARTER:
                 return QuarterImage(data, parm, context,
                                     PREPARE_BORDER, border);

     case IMG_IMAGEVIEW:
                 return ImageView(data, parm, context,
                                     PREPARE_BORDER, border);
     case IMG_CONTOURINGLINE:
                 return ContouringLine(data, parm, context,
                                     PREPARE_BORDER, border);
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
     case IMG_MARKALONGLINE :

                 return MarkAlongLine(data, parm, context, 0, 0);

     case IMG_HORIZONTALSHADE:

                 return HorizontalShade(data, parm, context, 0, 0);

     case IMG_MIRRORMARKALONGLINE:

                 return MirrorMarkAlongLine(data, parm, context,
                                            0, 0);

     case IMG_MARKINTOLINE:

                 return MarkIntoLine(data, parm, context,
                                     0, 0);
     case IMG_TURNMARK:

                 return TurnMark(data, parm, context,
                                 0, 0);

     case IMG_TURNANDLENGTHMARK:

                 return TurnAndLengthMark(data, parm, context,
                                          0, 0);

     case IMG_UNIVERSALTEXT:

                 return UniversalText(data, parm, context,
                                      0, 0);

     case IMG_SCALEVECTOR:

                 return ScaleVector(data, parm, context,
                                    0, 0);
     case IMG_THICKENLINEEX:

                 return ThickenLine(data, parm, context,
                                    0, 0);

     case IMG_MIDDLETHICKENLINEEX:

                 return MiddleThickenLine(data, parm, context,
                                          0, 0);

     case IMG_MARKANDNOTE:

                 return MarkAndNote(data, parm, context,
                                    0, 0);

     case IMG_SHADOWOBJECT:
     case IMG_COLORSHADOWOBJECT:

                 return ShadowObject(data, parm, context,
                                     0, 0);
     case IMG_DOUBLETHICKENLINE:

                 return DoubleThickenLine(data, parm, context,
                                          0, 0);

     case IMG_POLYARC:

                 return PolyArc(data, parm, context,
                                0, 0);

     case IMG_LENGTHANDANGLELINE:

                 return LengthAndAngleLine(data, parm, context,
                                           0, 0);

     case IMG_GRADIENTCIRCLE:

                 return GradientCircle(data, parm, context,
                                       0, 0);

    case IMG_SCALELIMITEDIMAGE:

                 return ScaleLimitedImage(data, parm, context,
                                          0, 0);

     case IMG_QUARTER:
                 return QuarterImage(data, parm, context,
                                     0, 0);

     case IMG_IMAGEVIEW:
                 return ImageView(data, parm, context,
                                  0, 0);
     case IMG_CONTOURINGLINE:
                 return ContouringLine(data, parm, context,
                                     0, 0);

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
                                   HRSC hrsc, const char * semantic)  // 03/09/13
 {
   POLYDATAEX  data;
   DRAWPOINT points[5];
   int  count[2];                                    // 26/03/12
   TEXTDATA textdata;                                // 25/01/08
   const char * vparm;
   IMGLONG * sparm;   
   float scale  = 1.0;
   float scale1 = 1.0;
   char * text = textdata.Text;

   // Две строки "Abc" и "123"
   char Semantic[] = { 0xFF, 0x7F,  20, 0,
                       3, 0,  0,   3,   'A', 'b', 'c', 0,
                       4, 0,  0,   3,   '1', '2', '3', 0 };

   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;

   IMGCONTEXT contextd;                            // 19/03/09
   IMAGESIZE  imgsize;

   memcpy((char *)&contextd,context,sizeof(IMGCONTEXT));

   memset((void*)&imgsize, 0, sizeof(IMAGESIZE));
   memset((void*)&data, 0, sizeof(data));

   data.Count      = 1;
   data.PolyCounts = &count[0];
   data.Points     = &points[0];
   data.Text       = &text;
   data.DX         = 5;
   data.DY         = 5;
   data.ShowScale  = 100;

   // Если textdata.Length <= длине строки ("Abc123"), то код выравнивания
   // не проверяется (выравнивание текста устанавливается по умолчанию -
   // слева по базовой линии)
   strcpy(textdata.Text, "Abc123");
   textdata.Length = 6;                                 // 25/01/08
   textdata.Text[textdata.Length] = 0;

   // Проверять наличие кода выравнивания за текстом при визуализации
   // (данный флаг отменяет проверку выравнивания по параметрам IMGTEXT и
   //  назначает проверку кода выравнивания типа CA_BASELINE_LEFT за текстом)
   data.AlignCode  = 1;                                 // 25/01/08

   data.Border.LT  = 0;
   data.Border.UP  = 0;
   data.Border.RT  = context->Size.cx;
   data.Border.DN  = context->Size.cy;

   data.MapRsc     = hrsc;                                       // 03/09/13
   data.Semantic   = (char*)semantic;                            // 03/09/13

   count[0]    = 2;
   points[0].x = context->Size.cx / 5;
   points[0].y = context->Size.cy - context->Size.cy / 5;
   points[1].x = context->Size.cx - context->Size.cx / 5;
   points[1].y = context->Size.cy / 5;

   switch (type)
   {
     case IMG_MARKALONGLINE :

          return MarkAlongLine(&data, parm, context, 0, 0);

     case IMG_HORIZONTALSHADE:

                 count[0]    = 4;
                 points[0].x = context->Size.cx / 5;
                 points[0].y = context->Size.cy - context->Size.cy / 5;
                 points[1].x = context->Size.cx / 2;
                 points[1].y = context->Size.cy / 5;
                 points[2].x = context->Size.cx - context->Size.cx / 5;
                 points[2].y = context->Size.cy - context->Size.cy / 5;
                 points[3].x = context->Size.cx / 5;
                 points[3].y = context->Size.cy - context->Size.cy / 5;

                 return HorizontalShade(&data, parm, context, 0, 0);

     case IMG_SHADOWOBJECT:
     case IMG_COLORSHADOWOBJECT:

                 count[0]    = 4;
                 points[0].x = context->Size.cx / 5;
                 points[0].y = context->Size.cy - context->Size.cy / 5;
                 points[1].x = context->Size.cx / 2;
                 points[1].y = context->Size.cy / 5;
                 points[2].x = context->Size.cx - context->Size.cx / 5;
                 points[2].y = context->Size.cy - context->Size.cy / 5;
                 points[3].x = context->Size.cx / 5;
                 points[3].y = context->Size.cy - context->Size.cy / 5;

                 return ShadowObject(&data, parm, context, 0, 0);

     case IMG_MIRRORMARKALONGLINE:

                 return MirrorMarkAlongLine(&data, parm, context,
                                            0, 0);

     case IMG_MARKINTOLINE:

                 return MarkIntoLine(&data, parm, context,
                                     0, 0);

     case IMG_TURNMARK:

                 return TurnMark(&data, parm, context,
                                 0, 0);

     case IMG_TURNANDLENGTHMARK:

                 return TurnAndLengthMark(&data, parm, context,
                                          0, 0);

     case IMG_UNIVERSALTEXT:

                 points[1].y = context->Size.cy / 2;     // 25/01/08

                 return UniversalText(&data, parm, context,
                                      0, 0);

     case IMG_SCALEVECTOR:

                 vparm = imlDrawParm(parm, 1);                   // 30/03/09
              		 contextd.Scale = 1;

                 imlGetRscVectorSize((IMGVECTOREX *)vparm, &imgsize,    // 30/03/09
                                     0, 0);
                 if (imgsize.HorizontalSize * contextd.PixelX > (float)(contextd.Size.cx * 0.9))
                    scale = (float)(contextd.Size.cx * 0.9)/(imgsize.HorizontalSize * contextd.PixelX + 1);
                 if (imgsize.VerticalSize * contextd.PixelY > (float)(contextd.Size.cy * 0.9))
                    scale1 = (float)(contextd.Size.cy * 0.9)/(imgsize.VerticalSize * contextd.PixelY + 1);
                 if (scale1 < scale) scale = scale1;

                 contextd.Scale = contextd.Scale*scale;  // 19/03/09
                 count[0]     = 1;
                 points[0].x = context->Size.cx / 10;
                 points[0].y = context->Size.cy - context->Size.cy / 10;
    
                 return ScaleVector(&data, parm, &contextd,
                                    0, 0);

     case IMG_THICKENLINEEX:

                 return ThickenLine(&data, parm, context,
                                    0, 0);

     case IMG_MIDDLETHICKENLINEEX:

                 return MiddleThickenLine(&data, parm, context,
                                          0, 0);

     case IMG_MARKANDNOTE:

                 count[0]    = 3;
                 points[0].x = context->Size.cx / 4;
                 points[0].y = context->Size.cy - context->Size.cy / 3;
                 points[1].x = context->Size.cx / 3;
                 points[1].y = context->Size.cy / 3;
                 points[2].x = context->Size.cx - context->Size.cx / 4;
                 points[2].y = context->Size.cy / 3;

                 if (data.Semantic == 0)                                  // 03/09/13
                   data.Semantic   = Semantic;

                 // Установим коды семантик из параметров
                 if (imlDrawType(parm, 4) == IMG_SEMANTIC)
                   {
                     int semanticnumber = 0;
                     IMGLONG * imglong = (IMGLONG *)imlDrawParm(parm, 4);
                     if (imglong)
                       semanticnumber = imglong->Value;

                     if (semanticnumber != 0)
                       {
                         Semantic[4] = semanticnumber;
                       }

                     if (imlDrawType(parm, 5) == IMG_SEMANTIC)
                       {
                         semanticnumber = 0;
                         imglong = (IMGLONG *)imlDrawParm(parm, 5);
                         if (imglong)
                           semanticnumber = imglong->Value;

                         if (semanticnumber != 0)
                           {
                            Semantic[12] = semanticnumber;
                           }
                       }

                   }

                 return MarkAndNote(&data, parm, context,
                                    0, 0);

     case IMG_DOUBLETHICKENLINE:

                 return DoubleThickenLine(&data, parm, context,
                                          0, 0);

     case IMG_POLYARC:

                 count[0]    = 3;
                 points[0].x = context->Size.cx / 4;
                 points[0].y = context->Size.cy - context->Size.cy / 3;
                 points[1].x = context->Size.cx / 3;
                 points[1].y = context->Size.cy / 3;
                 points[2].x = context->Size.cx - context->Size.cx / 4;
                 points[2].y = context->Size.cy / 3;

                 return PolyArc(&data, parm, context,
                                0, 0);


     case IMG_LENGTHANDANGLELINE:

                 return LengthAndAngleLine(&data, parm, context,
                                           0, 0);

     case IMG_GRADIENTCIRCLE:

                 points[0].x = (points[0].x + points[1].x)/2;
                 points[0].y = (points[0].y + points[1].y)/2;

                 return GradientCircle(&data, parm, context,
                                       0, 0);

    case IMG_SCALELIMITEDIMAGE:

                 count[0]    = 4;
                 points[0].x = context->Size.cx / 4;
                 points[0].y = context->Size.cy - context->Size.cy / 3;
                 points[1].x = context->Size.cx / 3;
                 points[1].y = context->Size.cy / 3;
                 points[2].x = context->Size.cx - context->Size.cx / 4;
                 points[2].y = context->Size.cy / 3;
                 points[3].x = context->Size.cx / 4;
                 points[3].y = context->Size.cy - context->Size.cy / 3;

                 sparm = (IMGLONG *)imlDrawParm(parm, 1);
                 if (sparm)
                   {
                     if (sparm->Value > 0)
                       data.ShowScale = sparm->Value;
                     else
                       data.ShowScale = 1;
                   }

                 return ScaleLimitedImage(&data, parm, context,
                                          0, 0);
     case IMG_QUARTER:

                 count[0]    = 4;
                 points[0].x = context->Size.cx / 5;
                 points[0].y = context->Size.cy - context->Size.cy / 5;
                 points[1].x = context->Size.cx / 2;
                 points[1].y = context->Size.cy / 5;
                 points[2].x = context->Size.cx - context->Size.cx / 5;
                 points[2].y = context->Size.cy - context->Size.cy / 5;
                 points[3].x = context->Size.cx / 5;
                 points[3].y = context->Size.cy - context->Size.cy / 5;
     
                 return QuarterImage(&data, parm, context,
                                     0, 0);

     case IMG_IMAGEVIEW:
                 count[0]    = 5;
                 points[0].x = context->Size.cx / 5;
                 points[0].y = context->Size.cy - context->Size.cy / 5;
                 points[1].x = context->Size.cx / 2;
                 points[1].y = context->Size.cy / 5;
                 points[2].x = context->Size.cx - context->Size.cx / 5;
                 points[2].y = context->Size.cy - context->Size.cy / 5;
                 points[3].x = context->Size.cx / 5;
                 points[3].y = context->Size.cy - context->Size.cy / 5;
                 points[4].x = context->Size.cx / 5;
                 points[4].y = context->Size.cy - context->Size.cy / 5;

                 return ImageView(&data, parm, context,
                                  0, 0);

     case IMG_CONTOURINGLINE:
                 return ContouringLine(&data, parm, context,
                                        0, 0);
   }

  // КОД НЕ НАЙДЕН
  return imgPrepareImage(&data, IMG_DRAW,
                              (const char *)parm, context, 0, 0);
 }


 long int _IMLAPI imlPaintExample(long int type,
                                  const IMGDRAW * parm,
                                  const IMGCONTEXT * context)
 {
   // Вызвать новую функцию
   return imlPaintExampleEx(type, parm, context, 0, 0);                   // 03/09/13
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
     case IMG_MARKALONGLINE :

         return MarkAlongLine(data, parm, context,
                              PREPARE_TEST, 0);

     case IMG_HORIZONTALSHADE:

                 return HorizontalShade(data, parm, context,
                                         PREPARE_TEST, 0);

     case IMG_MIRRORMARKALONGLINE:

                 return MirrorMarkAlongLine(data, parm, context,
                                            PREPARE_TEST, 0);

     case IMG_MARKINTOLINE:

                 return MarkIntoLine(data, parm, context,
                                     PREPARE_TEST, 0);

     case IMG_TURNMARK:

                 return TurnMark(data, parm, context,
                                 PREPARE_TEST, 0);

     case IMG_TURNANDLENGTHMARK:

                 return TurnAndLengthMark(data, parm, context,
                                          PREPARE_TEST, 0);

     case IMG_UNIVERSALTEXT:

                 return UniversalText(data, parm, context,
                                      PREPARE_TEST, 0);

     case IMG_SCALEVECTOR:

                 return ScaleVector(data, parm, context,
                                    PREPARE_TEST, 0);

     case IMG_THICKENLINEEX:

                 return ThickenLine(data, parm, context,
                                    PREPARE_TEST, 0);

     case IMG_MIDDLETHICKENLINEEX:

                 return MiddleThickenLine(data, parm, context,
                                          PREPARE_TEST, 0);

     case IMG_MARKANDNOTE:

                 return MarkAndNote(data, parm, context,
                                    PREPARE_TEST, 0);

     case IMG_SHADOWOBJECT:
     case IMG_COLORSHADOWOBJECT:

                 return ShadowObject(data, parm, context,
                                     PREPARE_TEST, 0);

     case IMG_DOUBLETHICKENLINE:

                 return DoubleThickenLine(data, parm, context,
                                          PREPARE_TEST, 0);
     case IMG_POLYARC:

                 return PolyArc(data, parm, context,
                                PREPARE_TEST, 0);

     case IMG_LENGTHANDANGLELINE:

                 return LengthAndAngleLine(data, parm, context,
                                           PREPARE_TEST, 0);

     case IMG_GRADIENTCIRCLE:

                 return GradientCircle(data, parm, context,
                                       PREPARE_TEST, 0);

    case IMG_SCALELIMITEDIMAGE:

                 return ScaleLimitedImage(data, parm, context,
                                          PREPARE_TEST, 0);

     case IMG_QUARTER:

                 return QuarterImage(data, parm, context,
                                     PREPARE_TEST, 0);

     case IMG_IMAGEVIEW:
                 return ImageView(data, parm, context,
                                  PREPARE_TEST, 0);
     case IMG_CONTOURINGLINE:
                 return ContouringLine(data, parm, context,
                                        PREPARE_TEST, 0);

   }

   return imgPrepareImage(data, IMG_LINE,
                          (const char *)&line, context,
                               PREPARE_TEST, 0);
 }

} // extern "C"


