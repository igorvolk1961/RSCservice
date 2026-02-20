//***** dfcmain.cpp  ********************Пантелеева N.A *******  20/03/14  ***
//***** dfcmain.cpp  ********************Dishlenko S.G. *******  01/04/15  ***
//***** dfcmain.cpp  ********************Derevyagina Zh.A.*****  16/11/15  ***
//*                                                                          *
//*                                                                          *
//*          Библиотека условных знаков для классфикаторов АНИ (на С++)      *
//*			    МОДУЛЬ СЕРВИСНЫХ ФУНКЦИЙ                         *
//*                                                                          *
//****************************************************************************
// Автор          : Кота Ф.Ф.                                                *
// Перевод на C++ : Кота Ф.Ф.                                                *
//****************************************************************************
#ifdef WIN32API                             // 01/04/15
#include <windows.h>
#endif

#ifndef IMLSERV_H
  #include "imlserv.h"
#endif

#ifndef DFC_H
  #include "dfc.h"
#endif

#ifndef MATH_H
  #include "math.h"
#endif

#ifndef MATH_H
  #include "math.h"
#endif


#include <imlsemn.cpp>


//****************************************************************************
//  Рисует знак "двухсторонний маршрут"
//  элемент "Параметры маршрута"  - линейный объект в который динамически врезаются 3 знака
//          корректное отображение знака возможно при условии что объект линейный,  
//          имеет 5 точек метрики, и назначены виды трёх векторных знаков (указатель, прямой и обратный МПУ)
//  линейный объект отрисовывается между 1-й и 5-й точкой вне зависимости от формы метрики (спрямление метрики)
//  элемент  - "Вид указателя (по умолчанию)" - врезается в позицию 3-й точки метрики
//  элемент  - "Вид знака (прямой МПУ)" - врезается в позицию 2-й точки метрики
//  элемент  - "Вид знака (обратный МПУ)" - врезается в позицию 4-й точки метрики
//  Алгоритм врезки, заключается в проверке длины линии и сопоставлении её с размерами веторных объектов
//  в текущем масштабе отображения. Знаки отображаются по приоритету - указатель, подписи МПУ
//  Это означает, если вмещается какой то один знак - будет отображён указатель, а потом подписи МПУ
//  При рисовании учитываются семантики:
//        числовые Обрезка-прямой курс (мм.) - обрезает видимую часть линии у 1-й точки
//        числовые Обрезка-обратный курс (мм.) - обрезает видимую часть линии у 5-й точки
//        перечислимая числовая Семантика-код ориентации знака - выбирает один из 7-ми знаков указателей
//  в зависимости от значения. Более подробно расписана в описании классификатора
//        Семантика-статус маршрута - числовая. В зависимости от значения, автоматически поднимает линию
//  истиной метрики в один из цветов 1-жёлтый 2-зелёный 3- красный
//  Семантика используется для установки цвета по определённому статусу объекта
//****************************************************************************
 // готовим параметры для "ДВУХСТОРОННИЙ МАРШРУТ"

IMLPARM DfcRouteTwo =
{
  IMLIDENT, 15, 1, 0, 0, 0,
  {{IMG_LINE,     "Line parameters", "Параметры маршрута",0,0},
   {IMG_VECTOREX, "Arrow object", "Вид указателя (по умолчанию)",0,0},
   {IMG_VECTOREX, "True course object", "Вид знака (прямой МПУ)",0,0},
   {IMG_VECTOREX, "Revers course object", "Вид знака (обратный МПУ)",0,0},
   {IMG_SEMANTIC, "Offset true course (mm.)","Обрезка-прямой курс (мм.)",0,0},
   {IMG_SEMANTIC, "Offset reverse course (mm.)", "Обрезка-обратный курс (мм.)",0,0},
   {IMG_SEMANTIC, "Orientation semantic", "Семантика-код ориентации знака",0,0},
   {IMG_VECTOREX, "1:Arrow (H-Name-Marker)",   "1:Указатель (высота-название-знак)",0,0},
   {IMG_VECTOREX, "2:Arrow (H-Marker-Name)",  "2:Указатель (высота-знак-название)",0,0},
   {IMG_VECTOREX, "3:Arrow (Name-Marker-H/H)", "3:Указатель (название-знак-высоты)",0,0},
   {IMG_VECTOREX, "4:Arrow (H/H-Marker-Name)", "4:Указатель (высоты-знак-название)",0,0},
   {IMG_VECTOREX, "5:Arrow (H/H-Name-Marker)", "5:Указатель (высоты-название-знак)",0,0},
   {IMG_VECTOREX, "6:Arrow (Marker-Name-H)",  "6:Указатель (знак-название-высота)",0,0},
   {IMG_VECTOREX, "7:Arrow (Marker-Name-H/H)", "7:Указатель (знак-название-высоты)",0,0},
   {IMG_SEMANTIC, "Object status", "Семантика-статус маршрута",0,0}
  }
};

//****************************************************************************
//  Рисует знак "односторонний маршрут"
//  элемент "Параметры маршрута"  - линейный объект в который динамически врезаются 2 знака
//          корректное отображение знака возможно при условии что объект линейный,
//          имеет 4 точки метрики, и назначены виды трёх векторных знаков (указатель и прямой МПУ)
//  линейный объект отрисовывается между 1-й и 4-й точкой вне зависимости от формы метрики (спрямление метрики)
//  элемент  - "Вид указателя (по умолчанию)" - врезается в позицию 3-й точки метрики
//  элемент  - "Вид знака (прямой МПУ)" - врезается в позицию 2-й точки метрики
//  Алгоритм врезки, заключается в проверке длины линии и сопоставлении её с размерами векторных объектов
//  в текущем масштабе отображения. Знаки отображаются по приоритету - указатель, подпись МПУ
//  Это означает, если вмещается какой то один знак - будет отображён указатель, а потом подписи МПУ
//  При рисовании учитываются семантики:
//        числовые Обрезка-прямой курс (мм.) - обрезает видимую часть линии у 1-й точки
//        числовые Обрезка-обратный курс (мм.) - обрезает видимую часть линии у 4-й точки
//        перечислимая числовая Семантика-код ориентации знака - выбирает один из 7-ми знаков указателей
//  в зависимости от значения. Более подробно расписана в описании классификатора
//        Семантика-статус маршрута - числовая. В зависимости от значения, автоматически поднимает линию
//  истиной метрики в один из цветов 1-жёлтый 2-зелёный 3- красный
//  Семантика используется для установки цвета по определённому статусу объекта
//****************************************************************************
// готовим параметры для "ОДНОСТОРОННИЙ МАРШРУТ"
IMLPARM DfcRouteOne =
{
  IMLIDENT, 14, 1, 0, 0, 0,
  {{IMG_LINE,     "Line parameters", "Параметры маршрута",0,0},
   {IMG_VECTOREX, "Arrow object", "Вид указателя (по умолчанию)",0,0},
   {IMG_VECTOREX, "True course object", "Вид знака (прямой МПУ)",0,0},
   {IMG_SEMANTIC, "Offset true course (mm.)","Обрезка-прямой курс (мм.)",0,0},
   {IMG_SEMANTIC, "Offset reverse course (mm.)", "Обрезка-обратный курс (мм.)",0,0},
   {IMG_SEMANTIC, "Orientation semantic", "Семантика-код ориентации знака",0,0},
   {IMG_VECTOREX, "1:Arrow (H-Name-Marker)",   "1:Указатель (высота-название-знак)",0,0},
   {IMG_VECTOREX, "2:Arrow (H-Marker-Name)",  "2:Указатель (высота-знак-название)",0,0},
   {IMG_VECTOREX, "3:Arrow (Name-Marker-H/H)", "3:Указатель (название-знак-высоты)",0,0},
   {IMG_VECTOREX, "4:Arrow (H/H-Marker-Name)", "4:Указатель (высоты-знак-название)",0,0},
   {IMG_VECTOREX, "5:Arrow (H/H-Name-Marker)", "5:Указатель (высоты-название-знак)",0,0},
   {IMG_VECTOREX, "6:Arrow (Marker-Name-H)",  "6:Указатель (знак-название-высота)",0,0},
   {IMG_VECTOREX, "7:Arrow (Marker-Name-H/H)", "7:Указатель (знак-название-высоты)",0,0},
   {IMG_SEMANTIC, "Object status", "Семантика-статус маршрута",0,0}
  }
};



//****************************************************************************
//  Рисует знак "двухсторонний маршрут CDR"
//  алгоритм полностью соответствует алгоритму отрисовки знака "двухсторонний маршрут"
// Отличие - вместо линии, главным объектом устанавливается объект пользователя IMG_DRAW
// в РНК, настраивается пунктирная линия
//****************************************************************************
// готовим параметры для "ДВУХСТОРОННИЙ МАРШРУТ" CDR
IMLPARM DfcRouteTwoCDR =
{
  IMLIDENT, 15, 1, 0, 0, 0,
  {{IMG_DRAW,     "Line parameters", "Параметры маршрута",0,0},
   {IMG_VECTOREX, "Arrow object", "Вид указателя (по умолчанию)",0,0},
   {IMG_VECTOREX, "True course object", "Вид знака (прямой МПУ)",0,0},
   {IMG_VECTOREX, "Revers course object", "Вид знака (обратный МПУ)",0,0},
   {IMG_SEMANTIC, "Offset true course (mm.)","Обрезка-прямой курс (мм.)",0,0},
   {IMG_SEMANTIC, "Offset reverse course (mm.)", "Обрезка-обратный курс (мм.)",0,0},
   {IMG_SEMANTIC, "Orientation semantic", "Семантика-код ориентации знака",0,0},
   {IMG_VECTOREX, "1:Arrow (H-Name-Marker)",   "1:Указатель (высота-название-знак)",0,0},
   {IMG_VECTOREX, "2:Arrow (H-Marker-Name)",  "2:Указатель (высота-знак-название)",0,0},
   {IMG_VECTOREX, "3:Arrow (Name-Marker-H/H)", "3:Указатель (название-знак-высоты)",0,0},
   {IMG_VECTOREX, "4:Arrow (H/H-Marker-Name)", "4:Указатель (высоты-знак-название)",0,0},
   {IMG_VECTOREX, "5:Arrow (H/H-Name-Marker)", "5:Указатель (высоты-название-знак)",0,0},
   {IMG_VECTOREX, "6:Arrow (Marker-Name-H)",  "6:Указатель (знак-название-высота)",0,0},
   {IMG_VECTOREX, "7:Arrow (Marker-Name-H/H)", "7:Указатель (знак-название-высоты)",0,0},
   {IMG_SEMANTIC, "Object status", "Семантика-статус маршрута",0,0}
  }
};

//****************************************************************************
//  Рисует знак "ОДНОСТОРОННИЙ маршрут CDR"
//  алгоритм полностью соответствует алгоритму отрисовки знака "двухсторонний маршрут"
// Отличие - вместо линии, главным объектом устанавливается объект пользователя IMG_DRAW
// в РНК, настраивается пунктирная линия
//****************************************************************************
// готовим параметры для "ОДНОСТОРОННИЙ МАРШРУТ"  CDR

IMLPARM DfcRouteOneCDR =
{
  IMLIDENT, 14, 1, 0, 0, 0,
  {{IMG_DRAW,     "Line parameters", "Параметры маршрута",0,0},
   {IMG_VECTOREX, "Arrow object", "Вид указателя (по умолчанию)",0,0},
   {IMG_VECTOREX, "True course object", "Вид знака (прямой МПУ)",0,0},
   {IMG_SEMANTIC, "Offset true course (mm.)","Обрезка-прямой курс (мм.)",0,0},
   {IMG_SEMANTIC, "Offset reverse course (mm.)", "Обрезка-обратный курс (мм.)",0,0},
   {IMG_SEMANTIC, "Orientation semantic", "Семантика-код ориентации знака",0,0},
   {IMG_VECTOREX, "1:Arrow (H-Name-Marker)",   "1:Указатель (высота-название-знак)",0,0},
   {IMG_VECTOREX, "2:Arrow (H-Marker-Name)",  "2:Указатель (высота-знак-название)",0,0},
   {IMG_VECTOREX, "3:Arrow (Name-Marker-H/H)", "3:Указатель (название-знак-высоты)",0,0},
   {IMG_VECTOREX, "4:Arrow (H/H-Marker-Name)", "4:Указатель (высоты-знак-название)",0,0},
   {IMG_VECTOREX, "5:Arrow (H/H-Name-Marker)", "5:Указатель (высоты-название-знак)",0,0},
   {IMG_VECTOREX, "6:Arrow (Marker-Name-H)",  "6:Указатель (знак-название-высота)",0,0},
   {IMG_VECTOREX, "7:Arrow (Marker-Name-H/H)", "7:Указатель (знак-название-высоты)",0,0},
   {IMG_SEMANTIC, "Object status", "Семантика-статус маршрута",0,0}
  }
};


//****************************************************************************
//  Рисует знак "ВЫНОСКА С ТЕКСТОМ"
//  элемент "Параметры линии"  - линейный объект между 1-й 2-й и 3-й точкой метрики
//      если число точек метрики 2 третья создаётся динамически
//  элемент  - "Вид указателя" - стрелка в третьей точке, ориентирована от 2-й точки
//  элемент  - "Параметры знака (подписи)" - векторный знак подпись харакетристик
//  Алгортим : наносится объект "Параметры знака", от него выносится горизонтальная линия
//   и от конца горизонтальной линии проводится линия со стрелкой, указывающей на объект карты
//****************************************************************************
// готовим параметры для "ВЫНОСКА С ТЕКСТОМ"
IMLPARM DfcTextArrow =
{
  IMLIDENT, 3, 1, 0, 0, 0,
  {{IMG_LINE,     "Line parameters", "Параметры линии",0,0},
   {IMG_VECTOREX, "Arrow view", "Вид указателя",0,0},
   {IMG_VECTOREX, "Mark parameters", "Параметры знака (подписи)",0,0},
  }
};

//****************************************************************************
//  Рисует знак "ВЕКТОР С НАКЛОНОМ"
//  элемент "Вид векторного знака"  - базовый элемент, который будет повёрнут
//       на угол, значение которого указывается в семантике "Семантика (азимут)"
// объект должен быть точечным, знак векторным
// используется для нанесения курсов посадки на аэропорт
//****************************************************************************
// готовим параметры для "ВЕКТОР С НАКЛОНОМ"
IMLPARM DfcSideVector =
{
  IMLIDENT, 2, 1, 0, 0, 0,
  {{IMG_VECTOREX, "Vector mark view", "Вид векторного знака",0,0},
   {IMG_SEMANTIC, "Semantic true azimuth", "Семантика (азимут)",0,0},
  }
};

//****************************************************************************
//  Рисует знак "ВЫНОСКА С МАСШТАБИРОВАНИЕМ"
//  элемент "Вид векторного знака"  - базовый элемент, который будет растянут по горизонтали
//      на  значение в процентах, указанное в семантике "Семантика (масштаб в %)"
// используется для подписей с различной шириной текста
//****************************************************************************
// готовим параметры для "ВЫНОСКА С МАСШТАБИРОВАНИЕМ"
IMLPARM DfcScaleVector =
{
  IMLIDENT, 2, 1, 0, 0, 0,
  {{IMG_VECTOREX, "Vector mark view", "Вид векторного знака",0,0},
   {IMG_SEMANTIC, "Semantic (scale %)", "Семантика (масштаб в %)",0,0},
  }
};

//****************************************************************************
//  Рисует "ЗНАК ПО АЗИМУТУ ДАЛЬНОСТИ"
//  элемент "Параметры линии"  - линейный объект который будет отображён длиной в метрах
//     указанных в семантике "Семантика (длина в м.)" и повёрнут на азимут
//     указанный в семантике "Семантика (азимут)"
//   элемент "Вид знака (конец)" - можно использовать, как векторный знак подпись
//   или указатель
//  семантика "Семантика (цвет линии)"- окрашивает указатель в нужный цвет
//  знак используется при расчётах, и в составе классификатора РНК не используется
//****************************************************************************
// готовим параметры для "ЗНАК ПО АЗИМУТУ ДАЛЬНОСТИ"
IMLPARM DfcAzimDist =
{
  IMLIDENT, 5, 1, 0, 0, 0,
  {{IMG_LINE,     "Line parameters", "Параметры линии",0,0},
   {IMG_VECTOREX, "Vector mark view", "Вид знака (конец)",0,0},
   {IMG_SEMANTIC, "Semantic true azimuth", "Семантика (азимут)",0,0},
   {IMG_SEMANTIC, "Semantic distance (м.)", "Семантика (длина в м.)",0,0},
   {IMG_SEMANTIC, "Semantic line color", "Семантика (цвет линии)",0,0}
  }
};

//****************************************************************************
//  Рисует знак "аэродромный маршрут"
//****************************************************************************
// готовим параметры для "АЭРОДРОМНЫЙ МАРШРУТ"
IMLPARM DfcAirportRoute =
{
  IMLIDENT, 7, 1, 0, 0, 0,
  {{IMG_LINE,     "Line parameters", "Параметры линии",0,0},
   {IMG_VECTOREX, "Parameters object", "Вид знака с параметрами",0,0},
   {IMG_VECTOREX, "True course object", "Вид знака МПУ",0,0},
   {IMG_VECTOREX, "Arrow object", "Вид стрелки-направления",0,0},
   {IMG_SEMANTIC, "Offset course (mm.)", "Обрезка курсов (мм.)",0,0},
   {IMG_SEMANTIC, "Offset reverse course (mm.)", "Обрезка-обратный курс (мм.)",0,0},
   {IMG_SEMANTIC, "Parameters segment insert", "Сегмент врезки параметров",0,0}
  }
};

//****************************************************************************
//  Рисует знак "Линия переменной ширины"
//****************************************************************************
// готовим параметры для "Линия переменной ширины"
IMLPARM DfcRunway =
{
  IMLIDENT, 2, 1, 0, 0, 0,
  {{IMG_LINE,     "Line parameters", "Параметры линии",0,0},
   {IMG_SEMANTIC, "Segment width (m)", "Толщины сегмента (м)",0,0}
  }
};

//****************************************************************************
//  Рисует знак "Группа векторных знаков"
//****************************************************************************
// готовим параметры для "Группа векторных знаков"
IMLPARM DfcObjectGroup =
{
  IMLIDENT, 6, 1, 0, 0, 0,
  {{IMG_VECTOREX, "Object", "Объект",0,0},
   {IMG_SEMANTIC, "Row count", "Рядов вдоль метрики",0,0},
   {IMG_SEMANTIC, "Object in row", "Oбъектов в ряду (вправо)",0,0},
   {IMG_SEMANTIC, "Distance between rows (m)", "Расстояние между рядами (м)",0,0},
   {IMG_SEMANTIC, "Distance between object (m)", "Расстояние между объектами (м)",0,0},
   {IMG_SEMANTIC, "Symmetry", "Симметрия (1-симметрично)",0,0},
  }
};

//****************************************************************************
//  Рисует знак "Сектора минимально безопасных высот"   (МБВ)
//****************************************************************************
// готовим параметры для "МБВ"
IMLPARM DfcSectorMSA =
{
  IMLIDENT, 4, 1, 0, 0, 0,
  {{IMG_VECTOREX, "Course marker", "Указатель курса",0,0},
   {IMG_VECTOREX, "Height marker", "Указатель высоты",0,0},
   {IMG_VECTOREX, "Center", "Центральный знак",0,0},
   {IMG_SEMANTIC, "Semantic course:height", "Семантика курсы:высоты",0,0}
  }
};


IMLLIST ImlList =
{
  IMLIDENT, 12,          // <--- Число видов знаков !!!
  {{DFC_ENROUTE_TWO, &DfcRouteTwo, "Two direction enroute", "Двухсторонний маршрут", 0, 0},
   {DFC_ENROUTE_ONE, &DfcRouteOne, "One direction enroute", "Односторонний маршрут", 0, 0},
   {DFC_ENROUTE_TWOCDR, &DfcRouteTwoCDR, "Two direction enroute CDR", "Двухсторонний маршрут CDR", 0, 0},
   {DFC_ENROUTE_ONECDR, &DfcRouteOneCDR, "One direction enroute CDR", "Односторонний маршрут CDR", 0, 0},
   {DFC_VECTOR_ARROW, &DfcTextArrow, "Arrow and vector object", "Бокс с линией выноски", 0, 0},
   {DFC_ROTATED_MARK, &DfcSideVector, "Rotated mark by semantic", "Знак с поворотом по семантике", 0, 0},
   {DFC_HSCALED_MARK, &DfcScaleVector, "Scaled mark by semantic", "Масштабируемый знак (по семантике)", 0, 0},
   {DFC_AZIMUTH_DIST, &DfcAzimDist, "Azimuth distance mark", "Комбинированный знак (азимут/длина)", 0, 0},
   {DFC_SID_STAR, &DfcAirportRoute, "Airport route SID/STAR/IAP", "Аэродромный маршрут SID/STAR/IAP", 0, 0},
   {DFC_VAR_WIDTH_LINE, &DfcRunway, "Variabled width line", "Линия переменной ширины", 0, 0},
   {DFC_VECTOR_GROUP, &DfcObjectGroup, "Vector mark group", "Группа векторных знаков", 0, 0},
   {DFC_SECTOR_MSA, &DfcSectorMSA, "Vector MSA sector", "Знак МБВ", 0, 0},
   { 0, 0, 0, 0, 0, 0},
  }
};



typedef struct PREPARESEMMSA
{
  SEMANTIC    Head;
  SEMBLOCK    Sem9;
} PREPARESEMMSA;



extern "C"
{

 // -----------------------------------------------------------------
 // Запросить список видов графических объектов,
 // поддерживаемых библиотекой
 // -----------------------------------------------------------------
_IMLIMP const IMLLIST * _IMLAPI imlGetImageList(void)                     // 16/11/15
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

   if (border == 0) return 0;

   switch (type)
   {
     case DFC_ENROUTE_TWO   :
         return DrawEnroute(data, parm, context, PREPARE_BORDER, border, 1);
     case DFC_ENROUTE_ONE   :
         return DrawEnroute(data, parm, context, PREPARE_BORDER, border, 0);
     case DFC_ENROUTE_TWOCDR:
         return DrawEnroute(data, parm, context, PREPARE_BORDER, border, 1);
     case DFC_ENROUTE_ONECDR:
         return DrawEnroute(data, parm, context, PREPARE_BORDER, border, 0);
     case DFC_VECTOR_ARROW  :
         return DrawVecArrow(data, parm, context,PREPARE_BORDER, border);
     case DFC_ROTATED_MARK  :
         return DrawRotatedVector(data, parm, context,PREPARE_BORDER, border);
     case DFC_HSCALED_MARK  :
         return DrawScaledVector(data, parm, context,PREPARE_BORDER, border);
     case DFC_AZIMUTH_DIST  :
         return DrawAzimuthDistanceVector(data, parm, context,PREPARE_BORDER, border);
     case DFC_SID_STAR      :
         return DrawAirportRoute(data, parm, context,PREPARE_BORDER, border);
     case DFC_VAR_WIDTH_LINE:
         return DrawVarWidthLine(data, parm, context,PREPARE_BORDER, border);
     case DFC_VECTOR_GROUP  :
         return DrawObjectGroup(data, parm, context,PREPARE_BORDER, border);
     case DFC_SECTOR_MSA  :                                                    // 03/03/2014 Кота
         return DrawSectorMSA(data, parm, context,PREPARE_BORDER, border);
   };
   return 0;
}; //imlGetImageBorder

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

   if (data == 0)
     return 0;

   switch (type)
   {
     case DFC_ENROUTE_TWO   :
           return DrawEnroute(data, parm, context, 0, 0, 1);
     case DFC_ENROUTE_ONE   :
           return DrawEnroute(data, parm, context, 0, 0, 0);
     case DFC_ENROUTE_TWOCDR:
           return DrawEnroute(data, parm, context, 0, 0, 1);
     case DFC_ENROUTE_ONECDR:
           return DrawEnroute(data, parm, context, 0, 0, 0);
     case DFC_VECTOR_ARROW  :
           return DrawVecArrow(data, parm, context,0, 0);
     case DFC_ROTATED_MARK  :
          return DrawRotatedVector(data, parm, context,0, 0);
     case DFC_HSCALED_MARK  :
          return DrawScaledVector(data, parm, context,0, 0);
     case DFC_AZIMUTH_DIST  :
          return DrawAzimuthDistanceVector(data, parm, context,0, 0);
     case DFC_SID_STAR      :
          return DrawAirportRoute(data, parm, context,0, 0);
     case DFC_VAR_WIDTH_LINE:
          return DrawVarWidthLine(data, parm, context,0, 0);
     case DFC_VECTOR_GROUP  :
          return DrawObjectGroup(data, parm, context,0, 0);
     case DFC_SECTOR_MSA  :                                                    // 03/03/2014 Кота
         return DrawSectorMSA(data, parm, context, 0, 0);
    // КОД НЕ НАЙДЕН
    default :
      imgPrepareImage(data, IMG_DRAW, (const char *)parm,  context, 0, 0);
   }

   return 0;
 };


int _fastcall PrepareEnroute(const IMGDRAW * parm, const IMGCONTEXT * context)
{
   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;

  const BOX box = {0,0,context->Size.CX,context->Size.CY};
  const char * textparm;
  int     textfunc;
  int    _Counts;
  POLYDATAEX data;
  DRAWPOINT M[2];
  _Counts    = 2;

  data.Count	    = 1;
  data.PolyCounts   = &_Counts;
  data.Points	    = &M[0];
  data.Text	    = 0;
  data.Semantic     = 0;
  data.Border       = box;

  float dX = context->Size.CX/20;
  float dY = context->Size.CY/20;

  M[0].X = 2*dX; M[0].Y = 18*dY;
  M[1].X = 8*dX; M[1].Y = 10*dY;

  // Тип и Параметры функции
  textfunc = imlDrawType(parm, 1);
  textparm = imlDrawParm(parm, 1);
  imgPrepareImage(&data, textfunc, textparm, context, 0, 0);

  M[0].X = 10*dX; M[0].Y = 8*dY;
  M[1].X = 18*dX; M[1].Y = 2*dY;
  imgPrepareImage(&data, textfunc, textparm, context, 0, 0);

  M[0].X = 8*dX;  M[0].Y = 10*dY;
  M[1].X = 10*dX; M[1].Y = 8*dY;
  // Тип и Параметры функции
  textfunc = imlDrawType(parm, 2);
  textparm = imlDrawParm(parm, 2);
  return imgPrepareImage(&data, textfunc, textparm, context, 0, 0);
};


int _fastcall PrepareAirportRoute(const IMGDRAW * parm,
                   const IMGCONTEXT * context)
{
   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;

  const BOX box = {0,0,context->Size.CX,context->Size.CY};
  const char * textparm;
  int     textfunc;
  int    _Counts;
  POLYDATAEX data;
  DRAWPOINT M[2];
  _Counts    = 2;

  data.Count	    = 1;
  data.PolyCounts   = &_Counts;
  data.Points	    = &M[0];
  data.Text	    = 0;
  data.Semantic     = 0;
  data.Border       = box;

  float dX = context->Size.CX/20;
  float dY = context->Size.CY/20;

  M[0].X = 2*dX;  M[0].Y = 18*dY;
  M[1].X = 16*dX; M[1].Y = 3*dY;

  // Тип и Параметры функции
  textfunc = imlDrawType(parm, 1);
  textparm = imlDrawParm(parm, 1);
  imgPrepareImage(&data, textfunc, textparm, context, 0, 0);

  M[0].X = 16*dX;  M[0].Y = 3*dY;
  M[1].X = 17*dX;  M[1].Y = 2*dY;
  // Тип и Параметры функции
  textfunc = imlDrawType(parm, 4);
  textparm = imlDrawParm(parm, 4);
  return imgPrepareImage(&data, textfunc, textparm, context, 0, 0);
};


int _fastcall PrepareVecArrow(const IMGDRAW * parm,
                   const IMGCONTEXT * context)
{
   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;
  const BOX box = {0,0,context->Size.CX,context->Size.CY};
  int    _Counts;
  POLYDATAEX data;
  DRAWPOINT M[2];
  _Counts    = 2;
  data.Count	    = 1;
  data.PolyCounts   = &_Counts;
  data.Points	    = &M[0];
  data.Text	    = 0;
  data.Semantic     = 0;
  data.Border       = box;
  float dX = context->Size.CX/20;
  float dY = context->Size.CY/20;
  M[0].X = 2*dX;  M[0].Y = 18*dY;
  M[1].X = 12*dX; M[1].Y = 8*dY;
  return DrawVecArrow(&data, parm, context, 0, 0);
};


int _fastcall PrepareSizeScaleVector(const IMGDRAW * parm,
                   const IMGCONTEXT * context, int scaled)
{
   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;
  const BOX box = {0,0,context->Size.CX,context->Size.CY};
  int    _Counts;
  POLYDATAEX data;
  DRAWPOINT M[2];
  _Counts    = 2;
  data.Count	    = 1;
  data.PolyCounts   = &_Counts;
  data.Points	    = &M[0];
  data.Text	    = 0;
  data.Semantic     = 0;
  data.Border       = box;
  float dX = context->Size.CX;
  float dY = context->Size.CY;

  if (scaled)
    {
       M[0].X = dX/2;  M[0].Y = 0.2*dY;
       M[1].X = dX/2;  M[1].Y = 0.8*dY;
       return DrawRotatedVector(&data, parm, context, 0, 0);
    }
  else
    {
     M[0].X = 0.2*dX;  M[0].Y = dY/2;
     M[1].X = 0.8*dX;  M[1].Y = dY/2;
     return DrawRotatedVector(&data, parm, context, 0, 0);
    }
};


int _fastcall PrepareAzimuthDistanceVector(const IMGDRAW * parm,
                   const IMGCONTEXT * context)
{
   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;
  const BOX box = {0,0,context->Size.CX,context->Size.CY};
  int    _Counts;
  POLYDATAEX data;
  DRAWPOINT M[2];
  _Counts    = 2;
  data.Count	    = 1;
  data.PolyCounts   = &_Counts;
  data.Points	    = &M[0];
  data.Text	    = 0;
  data.Semantic     = 0;
  data.Border       = box;
  float dX = context->Size.CX;
  float dY = context->Size.CY;

  M[0].X = dX/2;  M[0].Y = 0.2*dY;
  M[1].X = dX/2;  M[1].Y = 0.8*dY;
  return DrawAzimuthDistanceVector(&data, parm, context, 0, 0);
};

int _fastcall PrepareVarWidthLine(const IMGDRAW * parm,
                   const IMGCONTEXT * context)
{
   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;
  const BOX box = {0,0,context->Size.CX,context->Size.CY};
  IMGLINE line;
  int    _Counts;
  POLYDATAEX data;
  DRAWPOINT VX[2];

  line.Color = 0;  line.Thick = 1000;
  _Counts    = 2;
  data.Count	    = 1;
  data.PolyCounts   = &_Counts;
  data.Points	    = &VX[0];
  data.Text	    = 0;
  data.Semantic     = 0;
  data.Border       = box;
  float dX = context->Size.CX/20;
  float dY = context->Size.CY/20;
  VX[0].X = 2*dX;  VX[0].Y = 18*dY;
  VX[1].X = 10*dX; VX[1].Y = 10*dY;
  imgPrepareImage(&data, IMG_LINE, (const char *)&line, context, 0, 0);

  line.Thick = 2000;
  VX[0].X = 10*dX; VX[0].Y = 10*dY;
  VX[1].X = 18*dX; VX[1].Y = 2*dY;
  return imgPrepareImage(&data, IMG_LINE, (const char *)&line, context, 0, 0);
};



int _fastcall PrepareObjectGroup(const IMGDRAW * parm,
                   const IMGCONTEXT * context)
{
   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;
  const BOX box = {0,0,context->Size.CX,context->Size.CY};
  IMGCIRCLE CRC;
  int    _Counts;
  POLYDATAEX data;
  DRAWPOINT VX;
  int cnt;
  CRC.Color = 0x00000FF;
  CRC.Thick = 0;
  _Counts    = 1;
  data.Count	    = 1;
  data.PolyCounts   = &_Counts;
  data.Points	    = &VX;
  data.Text	    = 0;
  data.Semantic     = 0;
  data.Border       = box;
  float dX = context->Size.CX;
  float dY = context->Size.CY;
  cnt = 1;
  CRC.Radius = context->Size.CX * 2;
  if (CRC.Radius==0)  CRC.Radius = 50;


  for (int i = 1; i <= 5; i++) {
    for (int j = -1; j <= 1; j++) {
     VX.X = (i+2)*dX/10; VX.Y = (5+j)*dY/10;
     imgPrepareImage(&data, IMG_CIRCLE, (const char *)&CRC, context, 0, 0);
    }
  };
  return 1;
};


int _fastcall PrepareSectorMSA(const IMGDRAW * parm,
                   const IMGCONTEXT * context)
{
   if ((context == 0) || (context->Length < (int)sizeof(IMGCONTEXT)))
     return 0;
  const BOX box = {0,0,context->Size.CX,context->Size.CY};
  POLYDATAEX data;
  int    _Counts =2;
  DRAWPOINT M[2];
                   // 18/03/14
  char SEM[] = {255, 127,  44, 0,
               9, 0, 126, 35, '0','0','0',':','3','0','0',';','0','9','0',':','(','1','5','0',')',
               ';','1','8','0',':','5','5','0',';','2','7','0',':','(','2','0','0',')', 0};

  data.Count	    = 1;
  data.PolyCounts   = &_Counts;
  data.Points	    = &M[0];
  data.Text	    = 0;
  data.Semantic     = 0;
  data.Border       = box;
  data.Semantic	    = (char*)SEM;

  float dX = context->Size.CX;
  float dY = context->Size.CY;
  float rad = dY;
  if (dY > dX) rad = dX;

  M[0].X = dX/2;  M[0].Y = dY/2;
  M[1].X = dX/2;  M[1].Y = 0.9*rad; // считаем север вверх     // 20/03/14
  return DrawSectorMSA(&data, parm, context, 0, 0);
};







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
   switch (type)
   {
     case DFC_ENROUTE_TWO   :
     case DFC_ENROUTE_ONE   :
     case DFC_ENROUTE_TWOCDR:
     case DFC_ENROUTE_ONECDR:
           return PrepareEnroute(parm, context);
     case DFC_VECTOR_ARROW  :
           return PrepareVecArrow(parm, context);
     case DFC_ROTATED_MARK  :
           return PrepareSizeScaleVector(parm, context, 0);
     case DFC_HSCALED_MARK  :
           return PrepareSizeScaleVector(parm, context, 1);
     case DFC_AZIMUTH_DIST  :
           return PrepareAzimuthDistanceVector(parm, context);
     case DFC_SID_STAR      :
           return PrepareAirportRoute(parm, context);
     case DFC_VAR_WIDTH_LINE:
           return PrepareVarWidthLine(parm, context);
     case DFC_VECTOR_GROUP  :
           return PrepareObjectGroup(parm, context);
     case DFC_SECTOR_MSA  :
           return PrepareSectorMSA(parm, context);          // 18/03/14
     };

   return 0;  
 };




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
     case DFC_ENROUTE_TWO   :
          return DrawEnroute(data, parm, context, PREPARE_TEST, 0, 1);
     case DFC_ENROUTE_ONE   :
          return DrawEnroute(data, parm, context, PREPARE_TEST, 0, 0);
     case DFC_ENROUTE_TWOCDR:
          return DrawEnroute(data, parm, context, PREPARE_TEST, 0, 1);
     case DFC_ENROUTE_ONECDR:
          return DrawEnroute(data, parm, context, PREPARE_TEST, 0, 0);
     case DFC_VECTOR_ARROW  :
          return DrawVecArrow(data, parm, context,PREPARE_TEST, 0);
     case DFC_ROTATED_MARK  :
          return DrawRotatedVector(data, parm, context,PREPARE_TEST, 0);
     case DFC_HSCALED_MARK  :
          return DrawScaledVector(data, parm, context,PREPARE_TEST, 0);
     case DFC_AZIMUTH_DIST  :
          return DrawAzimuthDistanceVector(data, parm, context,PREPARE_TEST, 0);
     case DFC_SID_STAR      :
          return DrawAirportRoute(data, parm, context,PREPARE_TEST, 0);
     case DFC_VAR_WIDTH_LINE:
          return DrawVarWidthLine(data, parm, context,PREPARE_TEST, 0);
     case DFC_VECTOR_GROUP  :
         return DrawObjectGroup(data, parm, context,PREPARE_TEST, 0);
     case DFC_SECTOR_MSA  :
         return DrawSectorMSA(data, parm, context,PREPARE_TEST, 0);
                                                        // 18/03/14
   default :
      break;
   };

  return imgPrepareImage(data, IMG_LINE, (const char *)&line, context,  PREPARE_TEST, 0);
}; //  imlTestVisible
// extern "C"

#pragma argsused

int WINAPI DllEntryPoint(HINSTANCE hinst, unsigned long reason, void* lpReserved)
{
 return 1;
}


}



