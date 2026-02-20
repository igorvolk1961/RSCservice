/****** RSCNAVY.H  ************* Belenkov O.V.   ****** 02/06/05 ***
/****** RSCNAVY.H  ************* Panteleeva N.A. ****** 27/02/09 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2009              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*   ОПИСАНИЕ ФУНКЦИЙ ВИЗУАЛИЗАЦИИ ОБЪЕКТОВ МОРСКОЙ КАРТЫ           *
*                                                                  *
*******************************************************************/

#ifndef RSCNAVY_H
#define RSCNAVY_H

#ifndef MAPGDIEX_H
  #include "mapgdiex.h"
#endif

#ifndef MAPAPI_H
  #include "mapapi.h"
#endif

#include "mapfiles.h"

#define IMG_ORIENTOBJECT   1  // ОРИЕНТИРОВАННЫЙ ВЕКТОРНЫЙ ЗНАК
#define IMG_LIGHTS         2  // ОГНИ
#define IMG_CENTREOBJECT   3  // ОРИЕНТИРОВАННЫЙ ВЕКТОРНЫЙ ЗНАК В ЦЕНТРЕ ПЛОЩАДИ
#define IMG_RESTRN         4  // ЗНАК ПО ОГРАНИЧЕНИЯМ
#define IMG_SERIA          5  // ЗНАК ПО ЗНАЧЕНИЯМ СЕМАНТИКИ
#define IMG_DEPARE         6  // ПЛОЩАДНЫЕ ЗОНЫ ГЛУБИНЫ
#define IMG_OBSTRN         7  // ПРЕПЯТСТВИЯ
#define IMG_VECTNATIONAL   8  // ВЕКТОРНЫЙ ЗНАК С ПОДПИСЯМИ С УЧЕТОМ ЯЗЫКА
#define IMG_DEPCONTOUR     9  // КОНТУРЫ ПЛОЩАДНЫХ ЗОН ГЛУБИНЫ      // 27/02/09
#define IMG_DEPCURV        10 // ИЗОБАТЫ                            // 27/02/09
#define IMG_SLCONS         11  // БЕРЕГОВЫЕ КОНСТРУКЦИИ             // 12/02/09
#define IMG_WRECKS         12  // ОСТАТКИ КОРАБЛЕКРУШЕНИЯ           // 12/02/09
#define IMG_CBLOHD         13  // ПОДПИСИ К НАВЕСНОМУ КАБЕЛЮ        // 13/02/09
#define IMG_QUALN          14  // ЛИНЕЙНЫЕ ОБЪЕКТЫ НИЗКОЙ ТОЧНОСТИ  // 13/02/09
#define IMG_SOUNDS         15  // ОТМЕТКИ ГЛУБИН                    // 17/02/09

// Ориентированный векторный знак, направление берется из
// семантической характеристики 117 (ORIENT)
int _fastcall OrientObject(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Огни
int _fastcall Lights(const POLYDATAEX* pdata, const IMGDRAW* draw,
                     const IMGCONTEXT * context, int test, FRAME * border);

// Ориентированный векторный знак в центре площади, направление берется из       // 28/12/06
// заданной семантической характеристики
int _fastcall CentreObject(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Вычисление ближайшей по горизонтали к центру тяжести точки, // 28/12/06
// входящей в объект
int _fastcall NearCentreGravityPoint(const POLYDATAEX* pdata,DRAWPOINT * gravpoint);

// Системный знак выбираемый по значению семантической характеристики RESTRN(131)
int _fastcall RestrnObject(const POLYDATAEX* pdata, const IMGDRAW* draw,   // 06/02/07
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Знак выбираемый по значению характеристики семантики
int _fastcall SeriaObject(const POLYDATAEX* pdata, const IMGDRAW* draw,   // 06/02/07
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Построение площадных зон глубины
int _fastcall DepareObject(const POLYDATAEX* pdata, const IMGDRAW* draw,   // 05/10/07
                           const IMGCONTEXT * context, int test,
                           FRAME * border);
// Построение точечных, линейных и площадных препятствий
int _fastcall ObstrnObject(const POLYDATAEX* pdata, const IMGDRAW* draw,   // 31/10/07
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Построение векторных с учетом национального языка
int _fastcall NationalVectObject(const POLYDATAEX* pdata, const IMGDRAW* draw,   // 19/01/09
                           const IMGCONTEXT * context, int test,
                           FRAME * border);
// Границы глубин
int _fastcall DepContourObject(const POLYDATAEX* pdata, const IMGDRAW* draw,  // 27/02/09
                           const IMGCONTEXT * context, int test,
                           FRAME * border);
// Изобаты
int _fastcall DepCurvObject(const POLYDATAEX* pdata, const IMGDRAW* draw,   // 27/02/09
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Изображение береговых конструкций
int _fastcall ShorelineConstractions(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Изображение остатков кораблекрушения
int _fastcall Wrecks(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Подписи к навесным трубопроводам (середина 1 отрезка)
int _fastcall CbLohdText(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Линейные объекты низкой точности
int _fastcall QualinLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Отметки глубин
int _fastcall Sounds(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);


// Таблицы служебных объектов для поиска внутренних кодов
typedef struct SERVOBJECTS     // Описание точки объекта
{
  unsigned long Code;         // Код объекта
  unsigned long Incode;       // Внутренний код
}
  SERVOBJECTS;
#define MAXSERVOBJECT  35

// Список точек пересечения с объектом
extern SERVOBJECTS ListServObject[MAXSERVOBJECT];
// Константы времени выполнения

extern double SHALLOW_CONTOUR;     // Контур отмели
extern double DEEP_CONTOUR;        // Контур глубины   // ум. 30
extern double SAFETY_CONTOUR;      // Контур безопасности
extern long   TWO_SHADES;     // 1- градация из 2 цветов, 0 - из 4 цветов
extern long   SHALLOW_PATTERNS;     // Заполнение мели (0 - нет, 1- да)
extern long   SHOW_ISDANGER_SHALLOW; // Показывать изолированные опасности в мелкой воде
                                  // (0 - нет, 1- да)
extern long   IML_LANGUAGE;

// Временные значения индексов таблицы цветов (Colour Table)
// ---------------------------------------------------------
//     НАЗВАНИЕ           СЕЙЧАС       ОПИСАНИЕ
//----------------------------------------------------------
#define CHBLK  (IMGC_INDEX | 2)        //  grey
#define CHMGD  (IMGC_INDEX | 8)        //  magenta
#define CHMGF  (IMGC_INDEX | 9)        //  magenta       
#define CHBRN  (IMGC_INDEX | 10)       //  red
#define LITRD  (IMGC_INDEX | 14)       //  red
#define LITGN  (IMGC_INDEX | 15)       //  green
#define LITYW  (IMGC_INDEX | 16)       //  yellow

#define CSTLN  (IMGC_INDEX | 23)       //  grey
#define SNDG1  (IMGC_INDEX | 24)       //  gray
#define SNDG2  (IMGC_INDEX | 25)       //  black
#define DEPSC  (IMGC_INDEX | 26)       //  grey
#define DEPCN  (IMGC_INDEX | 27)       //  grey
#define DEPDW  (IMGC_INDEX | 28)       //  white
#define DEPMD  (IMGC_INDEX | 29)       //  pale_blue
#define DEPMS  (IMGC_INDEX | 30)       //  light_blue
#define DEPVS  (IMGC_INDEX | 31)       //  medium_blue
#define DEPIT  (IMGC_INDEX | 32)       //  yellow-green





         //(индексы с 1)

// Цвета огней
// ---------------------
#define LIGHTS01   LITRD
#define LIGHTS02   LITGN
#define LIGHTS03   LITYW
#define LIGHTDEF   CHMGD

#endif

