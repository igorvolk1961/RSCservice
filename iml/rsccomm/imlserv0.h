/****** IMLSERV.H ************** Belenkov O.V.   ****** 18/04/16 ***
******* IMLSERV.H ************** Panteleeva N.A. ****** 04/04/16 ***
******* IMLSERV.H ************** Dishlenko S.G.  ****** 30/03/09 ***
******* IMLSERV.H ************** Korjilov A.     ****** 17/11/14 ***
******* IMLSERV.H ************** Savelov A.S.    ****** 17/08/15 ***
******* IMLSERV.H ************** Konon V.N.      ****** 02/06/16 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2016              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*     ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ОТОБРАЖЕНИЯ ПОЛЬЗОВАТЕЛЬСКИХ     *
*                  ОБЪЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ                      *
*                                                                  *
*******************************************************************/

#ifndef IMLSERV_H
#define IMLSERV_H

#ifndef MAPGDIEX_H
  #include "mapgdiex.h"
#endif

typedef struct IMLNAMECODES
{
  int Type;                // Идентификатор примитива
  int Code;                // Номер мультиязычного сообщения с названием примитива
  int Count;               // Число параметров в списке
  int Zero;                // Резерв = 0
  
  int Codes[IMLPARMCOUNT]; // Номера мультиязычных сообщений с названием параметров примитива  
}
  IMLNAMECODES;

extern "C"
{
#ifndef LINUXAPI  // 23/07/15
__declspec( dllexport )
#endif
#if defined(_M_X64) || defined(BUILD_DLL64)       // 16/08/09
long int  imlOpenGisLibrary64(const char * libname);
#else
long int  imlOpenGisLibrary(const char * libname);
#endif
}


  /*********************************************************
  *                                                        *
  *        ОБРАБОТКА ГРАФИЧЕСКИХ ПАРАМЕТРОВ ОБЪЕКТА        *
  *                                                        *
  *********************************************************/

// Запросить адрес описания примитива
// по последовательному номеру (начиная с 1)
// При ошибке возвращает ноль

const IMGDECOR * _fastcall imlDrawAddress(const IMGDRAW * draw, int number);


// Запросить тип функции отображения примитива
// по последовательному номеру (начиная с 1)
// При ошибке возвращает ноль

int _fastcall imlDrawType(const IMGDRAW * draw, int number);


// Запросить адрес параметров примитива
// по последовательному номеру (начиная с 1)
// При ошибке возвращает ноль

const char * _fastcall imlDrawParm(const IMGDRAW * draw, int number);


  /*********************************************************
  *                                                        *
  *     ПОСТРОЕНИЕ ТИПОВЫХ ГЕОМЕТРИЧЕСКИХ ПРИМИТИВОВ       *
  *                                                        *
  *********************************************************/

// Построить метрику окружности заданного радиуса (140)
// Заполняем метрику по адресу data (резервируется 120 точек)
// center - центр окружности в координатах устройства вывода
// radius - радиус окружности в точках устройства
// data   - адрес структуры, описывающей выходные данные
// При ошибке возвращает ноль, иначе - число точек в построении

int _fastcall imlBuildCircle(DRAWPOINT * center, long radius,
                             POLYDATAEX * data);

// Построить эллипс
// Заполняем метрику по адресу data (резервируется 120 точек)
// center  - центр эллипса в координатах устройства вывода
// radius  - радиус эллипса в точках устройства по горизонтали
// radiusv - радиус эллипса в точках устройства по вертикали
// data    - адрес структуры, описывающей выходные данные
// При ошибке возвращает ноль, иначе - число точек в построении

int _fastcall imlBuildEllipse(DRAWPOINT * center, long radius,
                              long radiusv, POLYDATAEX * data);

// Построить метрику части окружности заданного радиуса
// Заполняем метрику по адресу data (резервируется 120 точек)
// first   - координаты первой точки дуги
// center  - центр дуги в координатах устройства вывода
// last    - координаты второй точки дуги
// radius  - радиус дуги в точках устройства
// При ошибке возвращает ноль, иначе - число точек в построении

int _fastcall imlBuildArc(DRAWPOINT * first, DRAWPOINT * center,
                          DRAWPOINT * last, long int radius, POLYDATAEX * data);

#define imlLEFT  1
#define imlRIGHT 2

// Положение (сторона) точки относительно прямой
// Возвращает imlLEFT, imlRIGHT
// При ошибке возвращает 0

int _fastcall imlPointSideNearLine(DOUBLEPOINT *point,
                                   DOUBLEPOINT *xy1, DOUBLEPOINT *xy2);

int _fastcall imlPointSideNearLineDraw(DRAWPOINT *point,
                                       DRAWPOINT *xy1, DRAWPOINT *xy2);

// Определение центра окружности по трем точкам
// pc - точка центра
// Возвращает радиус окружности (0.0 - три точки совпадают,
//                              -1.0 - точки на одной прямой)

double _fastcall imlCircleCenter(DOUBLEPOINT *p1, DOUBLEPOINT *p2,
                                 DOUBLEPOINT *p3, DOUBLEPOINT *pc);

// Вычисление расстояния между двумя точками

double _fastcall imlDistancePoint(DOUBLEPOINT *xy1, DOUBLEPOINT *xy2);
double _fastcall imlDistancePointDraw(DRAWPOINT *xy1, DRAWPOINT *xy2);

// Вычисление расстояния между двумя точками на местности
// hmap   - идентификатор открытых данных
// point1, point2 - координаты точек в метрах на местности
// Для вычисления расстояния координаты пересчитываюся
// в проекцию топографической карты с установкой осевого
// меридиана в центре отрезка
// При ошибке возвращает 0

double _fastcall imgRealDistance(HMAP hmap, DOUBLEPOINT *point1,
                                 DOUBLEPOINT *point2);
                                       
// Преобразование из пикселов в изображении в координаты
// на местности в метрах
// Применение :
// xmet = xdis; ymet = ydis;
// mapPictureToPlane(xmet,ymet);
// hmap - идентификатор открытых данных
// x,y  - преобразуемые координаты

void _fastcall imgPictureToPlane(HMAP hmap, double *x, double *y);


// Определить положение точки на отрезке, делящей отрезок
// в отношении m1/m2
// При ошибке возвращает 0

int _fastcall imlPointPositionOnCut(DOUBLEPOINT *point,
                                    DOUBLEPOINT *p1, DOUBLEPOINT *p2,
                                    double m1, double m2);


// Определить длину наибольшей стороны отрезка

int _fastcall imlLineLength(DRAWLINE * line);

// Определить длину наибольших сторон контура

int _fastcall imlContourLength(DRAWPOINT * points, int count);

// Поиск точки расположенной на заданном расстояниии вдоль линии

DRAWPOINT _fastcall imlFindPointOnLine(DRAWLINE * line, int length);

// Поиск точки на заданном расстояниии вдоль контура

DRAWPOINT _fastcall imlFindPointOnContour(DRAWPOINT * points, int count, int length);


#define MAXPOINTCOUNT 2048   // Предельное число точек в построенной линии

extern DRAWPOINT Buffer[MAXPOINTCOUNT]; // Рабочий буфер для пересчета метрики

typedef struct LEVELCROSSPOINT     // Описание точки пересечения уровня
{
  int Z;                           // Координата пересечения уровня
  int Direct;                      // Направление пересекающей линии 0/1
  int Node;                        // Номер пересекаемого узла или 0
}
  LEVELCROSSPOINT;

#define MAXOBJECTCROSS  512

// Список точек пересечения с объектом
extern LEVELCROSSPOINT ObjectCross[MAXOBJECTCROSS];

// Поcтроить линию на заданном расстоянии от исходной (src)
// shift - смещение от исходной линии (вправо - больше 0)
// count - число точек в исходной линии (не более 1024)
// Возвращает число точек в построенной линии и устанавливает
// адрес буфера с новой метрикой (dest)

int _fastcall BuildShiftLine(DRAWPOINT ** dest, DRAWPOINT * src, int count,
                             int shift);

// Поиск пересечения горизонтального уровня с отрезком
// Если пересечений нет - возвращает ноль

int _fastcall SeekHorizontalCross(DRAWPOINT * line, int level,
                                  LEVELCROSSPOINT * cross);

// Поиск пересечений горизонтали с набором отрезков (полигоном)
// Список отрезков заносится в Buffer
// level   - координата Y расположения горизонтали
// subject - признак объекта/подобъекта (0/1)
// Если пересечений нет - возвращает ноль, иначе - число точек в буфере

int _fastcall BuildHorizontalCross(DRAWPOINT * line, int count, int level,
                                   int subject);

// Вычисление центра тяжести объекта (без подобъектов)         // 28/12/06
// При ошибке - возвращает 0

int _fastcall CentreGravity(const POLYDATAEX* pdata,DRAWPOINT * gravpoint);

  /*********************************************************
  *                                                        *
  *     ЗАПРОС СВОЙСТВ ОБЪЕКТА                             *
  *                                                        *
  *********************************************************/

// Запросить строку с ключом объекта
// При ошибке возвращает ноль

const char * imlGetObjectKey(HOBJ hobj);

// Запросить строку с ключом объекта
// При ошибке возвращает ноль

// Запросить строку с уникальным номером объекта на карте
// При ошибке возвращает ноль                                 // 01/04/16

long int imlMapObjectKey(HOBJ hobj);

double imlGetObjectSquare(HOBJ hobj);

// Запросить точность поля значения семантики
int imlGetSemanticDecimal(HOBJ hobj, int semcode);

  /*********************************************************
  *                                                        *
  *     ОБРАБОТКА СЕМАНТИЧЕСКИХ ХАРАКТЕРИСТИК ОБ'ЪЕКТА     *
  *                                                        *
  *********************************************************/

// Запросить адрес записи семантики объекта
// При ошибке возвращает ноль

const char * imlGetSemanticRecord(HOBJ hobj);

#define SEMIDENT 0x7FFF      // ИДЕНТИФИКАТОР НАЧАЛА ЗАПИСИ

//typedef struct BIGSEMANTIC    // СЕМАНТИКА ОБ'ЕКТА
//{
//  unsigned int GetLength()
//  { if (Ident != SEMIDENT) return 0;
//    if (Length == 0x0FFFF) return BigLength;
//    return Length;
//  }

//  unsigned int HeadLength()
//  { if (Length != 0x0FFFF) return 4; // sizeof(SEMANTIC);
//    return sizeof(BIGSEMANTIC);
//  }

//  unsigned int DataLength()
//  { if (Ident != SEMIDENT) return 0;
//    int size = GetLength() - HeadLength();
//    if (size < 0) size = 0;
//    return size;
//  }

//  unsigned short int Ident;      // 0x7FFF
//  unsigned short int Length;     // Длина записи = 0xFFFF
//  unsigned       int BigLength;  // Длина записи, когда запись больше 65000 байт
//}
//  BIGSEMANTIC;

//typedef struct SEMANTIC       // СЕМАНТИКА ОБ'ЕКТА
//{
//  unsigned int GetLength() const
//  { if (Ident  != SEMIDENT) return 0;
//    if (Length != 0x0FFFF) return Length;
//    return ((BIGSEMANTIC*)this)->BigLength;
//  }

//  unsigned int HeadLength() const
//  { if (Length != 0x0FFFF) return sizeof(SEMANTIC);
//    return sizeof(BIGSEMANTIC);
//  }

//  unsigned int DataLength() const
//  { if (Ident != SEMIDENT) return 0;
//    int size = GetLength() - HeadLength();
//    if (size < 0) size = 0;
//    return size;
//  }

//  unsigned short int Ident;   // 0x7FFF
//  unsigned short int Length;  // Длина записи
//}
//  SEMANTIC;

// Запросить значение семантической характеристики в виде целого числа
// semn   - указатель на семантику (из POLYDATAEX)
// code   - код запрашиваемой характеристики
// number - порядковый номер (с 1) характеристики с данным кодом
//          (с одним кодом может быть несколько значений)
// При ошибке (если значение не найдено) возвращает ноль

int _fastcall imlGetSemanticLong(SEMANTIC * semn, int code,
                                 int number, long * value);


// Запросить значение характеристики в виде числа c плавающей точкой
// semn   - указатель на семантику (из POLYDATAEX)
// code   - код запрашиваемой характеристики
// number - порядковый номер (с 1) характеристики с данным кодом
//          (с одним кодом может быть несколько значений)
// При ошибке (если значение не найдено) возвращает ноль

int _fastcall imlGetSemanticDouble(SEMANTIC * semn, int code,
                                   int number, double * value);

// Запросить значение семантической характеристики в виде строки
// semn   - указатель на семантику (из POLYDATAEX)
// code   - код запрашиваемой характеристики
// number - порядковый номер (с 1) характеристики с данным кодом
//          (с одним кодом может быть несколько значений)
// При ошибке (если значение не найдено) возвращает ноль

int _fastcall imlGetSemanticString(SEMANTIC * semn, int code,
                                   int number, char * value, int size);

// Запросить значение семантической характеристики в виде строки
// в кодировке UNICODE 
// semn    - указатель на семантику (из POLYDATAEX)
// code    - код запрашиваемой характеристики
// number  - порядковый номер (с 1) характеристики с данным кодом
//           (с одним кодом может быть несколько значений)
// value   - адрес буфера для размещения результата
// maxsize - длина буфера в байтах!
// При ошибке (если значение не найдено) возвращает ноль

int _fastcall imlGetSemanticStringUn(SEMANTIC * semn, int code,
                                     int number, WCHAR * value, int size);
                                     
// Запросить значение семантической характеристики - ссылки на документ 
// в виде строки в кодировке UNICODE  
// semn    - указатель на семантику (из POLYDATAEX)
// number  - порядковый номер (с 1) характеристики с данным типом
//           (с одним кодом может быть несколько значений)
// value   - адрес буфера для размещения результата
// maxsize - длина буфера в байтах!
// При ошибке (если значение не найдено) возвращает ноль

int _fastcall imlGetSemanticDocReferenceStringUn(HRSC hrsc, SEMANTIC * semn, int number, WCHAR * value, int size, int& type);
                                   
// Запросить значение семантической характеристики в виде строки
// c учетом формата семантики в классификаторе
// data   - указатель на POLYDATAEX
// code   - код запрашиваемой характеристики
// number - порядковый номер (с 1) характеристики с данным кодом
//          (с одним кодом может быть несколько значений)
// При ошибке (если значение не найдено) возвращает ноль

int _fastcall imlGetSemanticStringRsc(const POLYDATAEX * data, int code,
                                      int number, char * value, int size);

// Заполнить структуру описания семантической характеристики
// по коду семантики
// code - внешний код семантики
// При ошибке возвращает ноль

int _fastcall imlGetRscSemanticExByCode(HRSC hrsc, RSCSEMANTICEX * semtype,
                                        int code);

// Найти внутренний код объекта по коду,локализации и номеру в аналогичных
// code - внешний код семантики
// При ошибке возвращает ноль

int _fastcall imlGetRscObjectIncode(HRSC hrsc,long int excode,           // 06/02/07
                                       long int local, long int number);

// Запросить параметры объекта по incode
// При ошибке возвращает ноль         

char * _fastcall imlGetRscObjectParameters(HRSC hrsc,long int incode);

// Запросить принтерные параметры объекта по incode
// При ошибке возвращает ноль                                 // 15/04/12

char * _fastcall imlGetRscPrintObjectParameters(HRSC hrsc,long int incode);

// Запросить функцию отображения объекта по incode
// При ошибке возвращает ноль

int _fastcall imlGetRscObjectFunction(HRSC hrsc,long int incode);     // 06/03/13


  /*********************************************************
  *                                                        *
  *        ОБРАБОТКА ГРАФИЧЕСКОГО ОБЪЕКТА                  *
  *                                                        *
  *********************************************************/

#define PREPARE_VIEW    0
#define PREPARE_TEST    1
#define PREPARE_BORDER  2
#define EXAMPLE_VIEW    3 // 06/02/07 Флаг для отдельного рисования в example

// Обработать графический объект в соответствии с видом операции
// test    - вид операции (0 - рисовать, 1 - определить попадание в область,
//                         2 - определить габариты графического объекта),
// data    - координаты графического объекта,
// type    - тип графического объекта (см. mapgdi.h : IMG_LINE...),
// parm    - параметры рисуемого объекта (см. mapgdi.h : IMGLINE...),
// context - вспомогательные параметры отображения (см. mapgdiex.h),
// border  - рассчитанные габариты с учетом их предыдущего значения !
// При ошибке возвращает ноль                      

int _fastcall imgPrepareImage(const POLYDATAEX * data, int type, const char * parm,
                              const IMGCONTEXT * context, int test, FRAME * border);

  /*********************************************************
  *                                                        *
  *        ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ                         *
  *                                                        *
  *********************************************************/

// Открыть данные с автоматическим определением их типа
// (векторные,растровые,матричные...)
// name - имя открываемого файла (MAP, SIT, MTW, RSW, MPT) в кодировке UNICODE
// mode - режим чтения/записи (GENERIC_READ, GENERIC_WRITE или 0)
// GENERIC_READ - все данные только на чтение, при этом не открываются
// файлы \Log\name.log и \Log\name.tac - протокол работы и журнал транзакций
// При ошибке возвращает ноль

HMAP _fastcall imlOpenDataUn(const WCHAR * name, int mode = GENERIC_READ);

// Закрыть все данные электронной карты
// hmap -  идентификатор открытых данных
// Идентификатор HMAP становится недействительным !

void _fastcall imlCloseData(HMAP hMap);

// Установить масштаб отображения (знаменатель масштаба)
//  Параметры:
//          hmap - идентификатор открытых данных
//          x, y - координаты предполагаемого "центра изображения"
//                 (любой точки привязки) в окне в текущем масштабе
//         scale - реальный масштаб отображения, который желают получить
//  Возвращает:   0 - масштаб не изменился,  1 - масштаб изменился
//          x, y - координаты предполагаемого "центра изображения"
//                 в окне относительно всей картинки
//                 в новом масштабе отображения

int _fastcall imlSetViewScale(HMAP hmap, long int *x, long int *y, float scale);

// Запросить базовый масштаб карты
// hmap - идентификатор открытых данных
// При ошибке возвращает ноль

int _fastcall imlGetMapScale(HMAP hmap);
              
// Запросить точный масштаб отображения карты
// hmap - идентификатор открытых данных
// Возвращает значение знаменателя масштаба

double _fastcall imlGetRealShowScale(HMAP hmap);

// Запросить размеры общего изображения карты в пикселах
// для текущего масштаба
// hmap - идентификатор открытых данных
// В переменную width заносится ширина изображения (dx),
// в переменную height - высота (dy)

void _fastcall imlGetPictureSize(HMAP hmap, long int *width, long int *height);

// Установить цвет фона отображаемой карты
// hmap - идентификатор открытых данных
// color - цвет фона

COLORREF _fastcall imlSetBackColor(HMAP hmap, COLORREF color);

// Установить степень прозрачности карты (от 0 до 100)
// hMap  - идентификатор открытой карты
// hSite - идентификатор открытой пользовательской карты
// transparent - степень прозрачности карты от 0 (карта не видна)
// до 100 (карта не прозрачная)
// Возвращает новое значение флага

int _fastcall imlSetSiteTransparent(HMAP hMap, HSITE hSite, int transparent);

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

int _fastcall imlPaint95Ex(HMAP hmap, HDC hdc, int erase, RECT *rect, int alpha);

// Вывести изображение карты в XImage (массив)
// Данная функция реализована для XWindow !
// Описание структуры XIMAGEDESC в maptype.h
// hmap - идентификатор открытых данных
// x,y - координаты левого верхнего угла внутри
// битовой области XImage для размещения изображения
// rect - выводимый фрагмент карты
// При ошибке в параметрах возвращает ноль

int _fastcall imlPaintToXImage(HMAP hmap, XIMAGEDESC *imagedesc,        // 17/08/15 Савелов
                               long int x, long int y, RECT *rect);

// Запросить/Установить способ отображения карты
// hmap - идентификатор открытых данных
// При установке нового способа отображения возвращается
// предыдущее значение
// (см. Maptype.h : VT_SCREEN(1), VT_PRINT(3), VT_PRINTRST(6),...)
// При ошибке возвращает ноль

long int _fastcall imlSetViewType(HMAP hmap, long int type);


// Преобразование из геодезических координат в радианах
// (общеземной эллипсоид WGS84)
// в метры на местности в проекции карты
// (поддерживается не для всех карт !)
// hmap - идентификатор открытых данных
// Bx,Ly,H  - преобразуемые координаты
// на входе радианы, на выходе - метры
// При ошибке возвращает ноль

int _fastcall imlGeoWGS84ToPlane3D(HMAP hmap, double *Bx, double *Ly, double *H);

// Преобразование из метров на местности в пикселы на
// изображении
// hmap - идентификатор открытых данных
// x,y  - преобразуемые координаты

void _fastcall imlPlaneToPicture(HMAP hmap, double *x, double *y);

// Запросить рамку подписи                                  // 14/01/13
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

int _fastcall imgGetTitleBorder(const POLYDATAEX * data,
                                const IMGTEXT * parm,
                                const IMGCONTEXT * context);

// Преобразовать текстовую строку из кодировки OEM (DOS) в ANSI (WINDOWS)
// При ошибке возвращает 0

char * _fastcall imgDos2Win(char * string, long int size);

// Путь к приложению

const char * _fastcall imgGetPathShell();

// Запросить язык сообщений                                // 19/01/09
// 1 - английский, 2 - русский, ... (см. Maptype.h)

int _fastcall imlGetMapAccessLanguage();

// Определить значение цвета в формате RGB по значению цвета
// в параметрах отображения 

long int imgGetTrueColorByColor(const IMGCONTEXT * context,
                                long int color);

// Запросить является ли имя идентификатором данных на Сервере
// Если да, то возвращает ненулевое значение (1 - устаревший
// формат без имени сервера, 2 - содержит имя сервера)

int _fastcall imlIsAliasNameUn(const WCHAR *name);

// Запросить идентификатор открытых данных, для которых создан контекст отображения
// В поле IMGCONTEXT::HView теперь записывается HPAINT
// При ошибке возвращает ноль

//HMAP _fastcall imlGetMapHandle(HPAINT hpaint);           // 26/10/15

// Запросить идентификатор текущей отображаемой карты 
// (при запросе в момент отображения из вспомогательной библиотеки -
// считаем, что один HMAP применяется в одном потоке отображения)
// При ошибке возвращает ноль

HSITE _fastcall imlGetCurrentViewSite(HMAP hmap);        // 06/03/15

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

int _fastcall imlReadSiteDocumentUn(HMAP hmap, HSITE hsite, const WCHAR *alias,
                                    WCHAR *name, int size);

// Определить имя файла пользовательской карты
// по ее идентификатору
// hMap  - идентификатор открытой карты
// hSite - идентификатор открытой пользовательской карты
// При ошибке возвращает пустую строку

int _fastcall imlGetSiteFileNameUn(HMAP hmap, HSITE hsite, WCHAR *name, int size);

// Запросить открыта ли карта на сервере или локально
// hmap -  идентификатор открытых данных
// hsite - идентификатор открытой пользовательской карты
// (для фоновой (основной) карты hsite = hmap)
// Если карта открыта на сервере возвращает ненулевое значение

int _fastcall imlIsSiteFromServer(HMAP hmap, HSITE hsite);

// Построить "длинное" имя файла (полный путь к файлу)
// templ - эталонный путь, относительно которого строится полный путь;
// templ - путь к библиотекам приложения (mapGetPathShell)
// name - короткий путь к файлу; например, имя файла
// path - "длинное" имя файла (указатель на массив для размещения результата, MAX_PATH)
// При ошибке возвращает ноль

int _fastcall imlBuildLongNameUn(const WCHAR *templ, const WCHAR *name, WCHAR *path);

// Запись вещественного числа в символьном виде с фиксированной точкой
// со вставкой разделяющих пробелов (разделение на тройки символов
// от десятичной точки к началу строки)

void SpaceInsert(char* string, const char * format, double number);

// Формирование строки со вставкой разделяющих пробелов

void InsertSeparatingBlanks(char* string);

#endif // IMLSERV_H

