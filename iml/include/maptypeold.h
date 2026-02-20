/****** maptypeold.h *********** Belenkov O.V.   ****** 16/04/22 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2022              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*          ОПИСАНИЕ УСТАРЕВШИХ БАЗОВЫХ ТИПОВ ПЕРЕМЕННЫХ            *
*                                                                  *
*******************************************************************/

#ifndef MAPTYPEOLD_H
#define MAPTYPEOLD_H

#ifndef MAPMACRO_H
 #include "mapmacro.h"
#endif

typedef struct SHORTPOINT    // КООРДИНАТЫ ТОЧКИ
{
  unsigned short int X;
  unsigned short int Y;
}
  SHORTPOINT;

typedef struct FLOATPOINT    // КООРДИНАТЫ ТОЧКИ (ПЛАВАЮЩАЯ МЕТРИКА)
{
  float X;
  float Y;
}
  FLOATPOINT;

#pragma pack(push,1)

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++ Структуры форматов представления метрики объектов     +++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ВИДЫ ФОРМАТОВ ОТДЕЛЬНОЙ ТОЧКИ МЕТРИКИ
typedef struct XYSHORT
{
  unsigned short int X;
  unsigned short int Y;
}
  XYSHORT;

typedef struct XYHSHORT
{
  unsigned short int X;
  unsigned short int Y;
           float     H;
}
  XYHSHORT;

typedef struct XYFLOAT
{
  float X;
  float Y;
}
  XYFLOAT;

typedef struct XYHFLOAT
{
  float X;
  float Y;
  float H;
}
  XYHFLOAT;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++++++ Структуры интерфейса MAPAPI +++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct MTRDESCRIBE // ОПИСАНИЕ ОТКРЫТОЙ МАТРИЦЫ
{
  char   Name[256];      // Имя файла
  double MinValue;       // Минимальное значение элемента в матрице
  double MaxValue;       // Максимальное значение элемента в матрице
  double ElementInPlane; // Размер элемента в метрах на местности
  DFRAME FrameMeters;    // Габариты матрицы (метры)
  int    ReliefType;     // Тип матрицы
                         //   0 - абсолютные высоты,
                         //   1 - суммарные высоты (абсолютные + относительные),
                         //   2 - относительные высоты
  int    UserType;       // Тип пользовательских данных
  int    View;           // Признак отображаемости
}
  MTRDESCRIBE;

#define MTRTILEIDENT      0x7F7F7F7F

typedef struct MTRTILE    // ОПИСАНИЕ ФРАГМЕНТА МАТРИЦЫ НА УЧАСТОК МЕСТНОСТИ
{
  int  Ident;             // Идентификатор записи 0x7F7F7F7F
  int  Length;            // Длина записи в байтах с заголовком (262208 байт)
  int  Width;             // Число столбцов (256)
  int  Height;            // Число строк    (256)
  int  Epsg;              // Код EPSG плановой системы координат (например, 3857)
  int  Level;             // Уровень матрицы в пирамиде матриц (аналогично пирамиде тайлов)
  int  WidthNumber;       // Номер столбца матрицы в заданной системе координат (в пространстве от -180 до +180 градусов)
  int  HeightNumber;      // Номер строки матрицы в заданной системе координат (в пространстве от -90 до +90 градусов)
  int  Unit;              // Единица измерения высоты (0 - метры, 1 - дециметры, 2 - сантиметры, 3 - миллиметры)
  int  UnitPlane;         // Единица измерения в плане (0 - метры, 1 - дециметры, 2 - сантиметры, 3 - миллиметры)
  int  ItemWidth;         // Ширина элемента в заданных единицах измерения в плане (справочно)
  int  ItemHeight;        // Высота элемента в заданных единицах измерения в плане (справочно)
  int  MinValue;          // Минимальное значение в матрице
  int  MaxValue;          // Максимальное значение в матрице
  int  Reserve[2];        // Резерв, содержит нули

  int  Value[256][256];    // Значения высот в заданных единицах измерения высоты
}
  MTRTILE;

typedef struct MTRTILE2   // ОПИСАНИЕ ФРАГМЕНТА МАТРИЦЫ НА УЧАСТОК МЕСТНОСТИ
{
  int  Ident;             // Идентификатор записи 0x7F7F7F7F
  int  Length;            // Длина записи в байтах с заголовком (131136 байт)
  int  Width;             // Число столбцов (256)
  int  Height;            // Число строк    (256)
  int  Epsg;              // Код EPSG плановой системы координат (например, 3857)
  int  Level;             // Уровень матрицы в пирамиде матриц (аналогично пирамиде тайлов)
  int  WidthNumber;       // Номер столбца матрицы в заданной системе координат (в пространстве от -180 до +180 градусов)
  int  HeightNumber;      // Номер строки матрицы в заданной системе координат (в пространстве от -90 до +90 градусов)
  int  Unit;              // Единица измерения высоты (0 - метры, 1 - дециметры, 2 - сантиметры, 3 - миллиметры)
  int  UnitPlane;         // Единица измерения в плане (0 - метры, 1 - дециметры, 2 - сантиметры, 3 - миллиметры)
  int  ItemWidth;         // Ширина элемента в заданных единицах измерения в плане (справочно)
  int  ItemHeight;        // Высота элемента в заданных единицах измерения в плане (справочно)
  int  MinValue;          // Минимальное значение в матрице
  int  MaxValue;          // Максимальное значение в матрице
  int  Reserve[2];        // Резерв, содержит нули

  unsigned short int Value[256][256];  // Значения высот в заданных единицах измерения высоты, относительно MinValue
}
  MTRTILE2;

typedef struct MTR3DVIEW     // ПАРАМЕТРЫ 3D-ОТОБРАЖЕНИЯ МАТРИЦЫ ВЫСОТ
{
  double AreaCenterX;        // Центр отображаемой области X,Y
  double AreaCenterY;        //   (в метрах на местности)
  double ViewPointX;         // Точка наблюдения X,Y,H
  double ViewPointY;         //   (в метрах на местности)
  double ViewPointH;         //
  unsigned int  ShowScale;   // Знаменатель масштаба
  unsigned char Style;       // Cтиль (0 - серый, 1 - цветной)
  unsigned char ShowGrid;    // Наличие сетки (0 - нет, 1 - есть)
  unsigned char GridStep;    // Шаг сетки в пикселах (2 - 255)
  unsigned char ModelHeight; // Высота модели в пикселах (2 - 127)
}
  MTR3DVIEW;

typedef struct METAFILEBUILDPARM  // ПАРАМЕТРЫ СОЗДАНИЯ МЕТАФАЙЛА
{
  long CenterX;     // Координаты центра выводимой области
  long CenterY;     // (в метрах на местности)
  long Width;       // Ширина фрагмента (в миллиметрах)
  long Height;      // Высота фрагмента (в миллиметрах)
  long Scale;       // Масштаб изображения
  long VisualType;  // Тип визуализации (от VT_SCREEN до VT_PRINTEX)
}
  METAFILEBUILDPARM;

typedef struct GRIDPARM      // СТРУКТУРА ПАРАМЕТРОВ ПРЯМОУГОЛЬНОЙ СЕТКИ
{
  GRIDPARM()  { Step = 1000; Type = GRT_LINE; Image.Color = 0x040404; Image.Thick = 250;
                Size = 10*250; Shadow = 0; Under = 0; }

  double    Step;           // Шаг сетки в метрах на местности
  long int  Type;           // Тип элемента (сетка, точки, кресты)
  struct
  {
   long int Color;
   long int Thick;
  }
            Image;          // Цвет и толщина линий (RGB, мкм)
  long int  Size;           // Размер элемента типа "крест" в мкм (1:250)
  long int  Shadow;         // Признак полупрозрачного отображения
  long int  Under;          // Признак отображения под картой
}
  GRIDPARM;

typedef struct GRIDPARMEX   // СТРУКТУРА ПАРАМЕТРОВ ПРЯМОУГОЛЬНОЙ СЕТКИ
{
  GRIDPARMEX()  { Step = 1000; Type = GRT_LINE; Image.Color = 0x040404; Image.Thick = 250;
                  Size = 10*250; Shadow = 0; Under = 0; BeginX = 0; BeginY = 0; }

  double    Step;           // Шаг сетки в метрах на местности
  long int  Type;           // Тип элемента (сетка, точки, кресты)
  struct
  {
   long int Color;
   long int Thick;
  }
            Image;          // Цвет и толщина линий (RGB, мкм)
  long int  Size;           // Размер элемента типа "крест" в мкм (1:250)
  long int  Shadow;         // Признак полупрозрачного отображения
  long int  Under;          // Признак отображения под картой

  double    BeginX;         // Начало координат сетки
  double    BeginY;
}
  GRIDPARMEX;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//++++++++ СТРУКТУРЫ ДЛЯ СОЗДАНИЯ ПЛАНОВ, КАРТ И ++++++++++++++++++
//++++++++ ПОЛЬЗОВАТЕЛЬСКИХ КАРТ                 ++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct APPENDPLANELIST
{
  int  Length;              // Длина записи структуры APPENDLIST

  char Nomenclature[32];    // Номенклатура листа
  char ListName[32];        // Условное название листа
  char FileName[256];       // Имя файла по которому в районе будет
                            // создан лист с данным именем и расширениями
                            // HDR,DAT,SEM ...
  int  Reserve;             // Резерв для выравнивания, должен быть 0

  // Прямоугольные координаты листа в метрах
  // (для пользовательской карты (SITE) должны быть 0)

  double XSouthWest   ;  // X ю-з
  double YSouthWest   ;  // Y ю-з
  double XNorthWest   ;  // X с-з
  double YNorthWest   ;  // Y с-з
  double XNorthEast   ;  // X с-в
  double YNorthEast   ;  // Y с-в
  double XSouthEast   ;  // X ю-в
  double YSouthEast   ;  // Y ю-в
}
  APPENDPLANELIST;

typedef struct CREATEPLANE
{
  int Length;               // Длина записи структуры CREATEPLANE

  char MapName[32];         // Имя района
  int  Scale;               // Знаменатель масштаба карты

  APPENDPLANELIST List;     // Описание листа плана
}
  CREATEPLANE;


typedef struct CREATESITE
{
  int      Length;             // Длина записи структуры CREATESITE (80)
  char     MapName[32];        // Имя района
  int      MapType;            // Обобщенный тип карты
  int      MaterialProjection; // Проекция исх. материала
  int      Scale;              // Знаменатель масштаба карты
                               // В радианах
  double FirstMainParallel;    // Первая главная параллель
  double SecondMainParallel;   // Вторая главная параллель
  double AxisMeridian;         // Осевой меридиан
  double MainPointParallel;    // Параллель главной точки
}
  CREATESITE;

typedef struct CREATESITEEX
{
 int      Length;             // Длина записи структуры CREATESITE (328)
 char     MapName[256];       // Имя района в кодировке ANSI (UNICODE - для функций "Un")
 int      MapType;            // Обобщенный тип карты
 int      MaterialProjection; // Проекция исх. материала
 union
 {
   int      EllipsoidKind;    // Тип эллипсоида (1 - Красовского, 9 - WGS84,...)
   int      EllipsoideKind;
 };
 int      Scale;              // Знаменатель масштаба карты
 int      Reserve;            // Резерв (должен быть 0)

                              // В радианах
 double FirstMainParallel;    // Первая главная параллель
 double SecondMainParallel;   // Вторая главная параллель
 double AxisMeridian;         // Осевой меридиан
 double MainPointParallel;    // Параллель главной точки
 double PoleLatitude;         // Широта полюса проекции
 double PoleLongitude;        // Долгота полюса проекции
}
  CREATESITEEX;

typedef struct CREATESITEUN
{
 int      Length;             // Длина записи структуры CREATESITE
 WCHAR    MapName[128];       // Имя района в кодировке UNICODE (для функций "Un")
 int      MapType;            // Обобщенный тип карты
 int      MaterialProjection; // Проекция исх. материала
 union
 {
   int      EllipsoidKind;    // Тип эллипсоида (1 - Красовского, 9 - WGS84,...)
   int      EllipsoideKind;
 };
 int      Scale;              // Знаменатель масштаба карты
 int      Reserve;            // Резерв (должен быть 0)

                              // В радианах
 double FirstMainParallel;    // Первая главная параллель
 double SecondMainParallel;   // Вторая главная параллель
 double AxisMeridian;         // Осевой меридиан
 double MainPointParallel;    // Параллель главной точки
 double PoleLatitude;         // Широта полюса проекции
 double PoleLongitude;        // Долгота полюса проекции
}
  CREATESITEUN;

// --------------------------------------------------------------
//  Структуры для вызова прикладных задач
// --------------------------------------------------------------

typedef struct TASKPARM        // ПАРАМЕТРЫ ПРИКЛАДНОЙ ЗАДАЧИ
{
  long int    Language;        // Код языка диалогов (1 - ENGLISH,
                               // 2 - RUSSIAN, ...)
#if defined(_M_X64) || defined(BUILD_DLL64)
#ifdef WIN32API
  long int    Zero;            // Выравнивание указателей
#endif
#endif
  HINSTANCE   Resource;        // УСТАРЕВШЕЕ ПОЛЕ
  const char* HelpName;        // УСТАРЕВШЕЕ ПОЛЕ
  const char* IniName;         // УСТАРЕВШЕЕ ПОЛЕ
  const char* PathShell;       // УСТАРЕВШЕЕ ПОЛЕ
  const char* ApplicationName; // УСТАРЕВШЕЕ ПОЛЕ
  HMESSAGE    Handle;          // Идентификатор главного окна приложения (обработчик команд в Linux)
}
  TASKPARM;

// --------------------------------------------------------------
//  Структуры для взаимодействия с ГИС-сервером
// --------------------------------------------------------------

typedef struct TMCMAPLISTITEM
{
  unsigned int Level;      // Уровень вложенности элемента с 1 до MaxLevel
  unsigned int Flags;      // Флаг редактирования, 0 - нет прав редактировать
  unsigned int Type;       // Тип файла MAPFILES или 0 для промежуточного уровня
  unsigned int Size;       // Размер данных Кбайт
  char         Name[256];  // Имя элемента (Алиас)
}
  TMCMAPLISTITEM;

typedef struct TMCMAPLIST  // СПИСОК ДОСТУПНЫХ КАРТ
{
  TMCMAPLIST() { Init(); }

  void Init() {  Ident = 0x7F7F7F7F;
                 Length = sizeof(TMCMAPLIST);
                 Count = 0; MaxLevel = 1; }

  int Ident;               // Идентификатор записи 0x7F7F7F7F
  int Length;              // Общая длина записи
  int Count;               // Число элементов в списке
  int MaxLevel;            // Максимальный уровень вложенности элементов (дерево)
  TMCMAPLISTITEM Item[1];  // Массив элементов списка доступных карт (в количестве Count)
}
  TMCMAPLIST;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++  Описание структур классификатора (RSC)                   +++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// СТРУКТУРА ДЛЯ СОЗДАНИЯ КЛАССИФИКАТОРА
typedef struct RSCCREATE
{
  char Name[32];   // Название классификатора
  char Type[32];   // Тип классификатора
  char Code[8];    // Код классификатора
  int  Scale;      // Знаменатель масштаба
  int  Language;   // Язык (MAPAPILANGUAGE)
}
  RSCCREATE;

// ОПИСАНИЕ ОБ'ЕКТОВ
typedef struct RSCOBJECT
{
  unsigned long Length   ; // РАЗМЕР СТРУКТУРЫ
  unsigned long Code     ; // КЛАССИФИКАЦИОННЫЙ КОД
  unsigned long Local    ; // ХАРАКТЕР ЛОКАЛИЗАЦИИ  (OBJECT_LOCAL)
  unsigned long Segment  ; // Н0МЕР СЛОЯ ( НЕ БОЛЕЕ 255)
  unsigned long Scale    ; // 1 - ОБ'ЕКТ МАСШТАБИРУЕТСЯ
                           // 0 - ОБ'ЕКТ НЕ МАСШТАБИРУЕТСЯ
  unsigned long Direct   ; // НАПРАВЛЕНИЕ ЦИФРОВАНИЯ ОБ'ЕКТА (OBJECT_DIRECT)
  unsigned long Bot      ; // НИЖНЯЯ ГРАНИЦА ВИДИМОСТИ (ЗНАМЕНАТЕЛЬ МАСШТАБА)
  unsigned long Top      ; // ВЕРХНЯЯ ГРАНИЦА ВИДИМОСТИ (ЗНАМЕНАТЕЛЬ МАСШТАБА)
           char Name[32] ; // НАЗВАНИЕ ( НЕ БОЛЕЕ 30)
}
  RSCOBJECT;

//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//+++++++ ВСПОМОГАТЕЛЬНЫЕ СТРУКТУРЫ +++++++++++++++++++++++++++++++
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// ПАРАМЕТРЫ ПОСТРОЕНИЯ ПРОФИЛЯ
typedef struct PROFBUILDPARM
{
  DOUBLEPOINT Point;               // Координаты текущей точки на объекте
  HOBJ        Object;              // Объект для построения профиля
  long int    ProfStepVertical;    // Шаг шкалы по вертикали (м)
  long int    ProfStepHorizontal;  // Шаг шкалы по горизонтали (м)
}
  PROFBUILDPARM;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++ Параметры КАЛЬКУЛЯТОРА ++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct SAVEMAPCOMPPARM
{
 char Titlsave[40];          // Название режима
 double MemoryRegister;      // Регистр
 double MemoryBuffer;        // Буфер памяти
 double Buffer;              // Буфер вычисленный
 double Memory;              // Память
 double Value;               // Передаваемое значение
 long int Flagcomp;          // Признак действия 1-(+),2-(-),3-(*),4-(/),
                             //                  0 - действия не было
 long int Index;             // Индекс для единиц измерения длины
 long int Indexold;          // Предыдущий индекс
 long int IndexSq;           // Индекс для единиц измерения площади
 long int Flagshow;          // Видимость диалога
 long int Regnow;            // Передаваемый режим
 long int Regold;            // Режим предыдущий-лин. или площ.
 long int Flagreg;           // Текущий режим: 0- длина, 1- площадь
 long int StopFlag;          // Флаг закрытия диалога
 long int Precision;         // Точность вычислений

}
 SAVEMAPCOMPPARM;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// Параметры создания TIFF растров
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct CREATETIFPARM
{
  int     BandCount;    // количество каналов
  int     BitInBand;    // бит на канал
  int     PixelType;    // тип пикселя (PT_BYTE и т.д.)
  int     Compress;     // тип компрессии (см. NOTCOMPRESS и т.д.)
                        // JPEG - возможен только для 3 и 1 канальных 8, 16 битных растров
                        //      - для 3 канальных допустим ColorSpace = CS_RGB или CS_YCBCR (лучшее сжатие, но медленнее)
                        //      - при CS_YCBCR в буфер тайла записываются RGB
  int     IsPredictor;  // для LZW и DEFLATE компрессии - признак сжатия расхождений
                        // 0 - сжимаются сами цвета
                        // 1 - сжимаются расхождения между соседними пикселями в строке
  int     JPEGQuality;  // качество JPEG сжатия (1-100) (по умолчанию 75)
  int     ColorSpace;   // интерпретация каналов для 3 канальных растров (CMYK для 4 канальных) см. CS_RGB и т.д.
  int     DeflateLevel; // уровень DEFLATE сжатия (1-9) (по умолчанию 6)
  int     TileWidth;    // ширина тайла в пикселях
  int     TileHeight;   // высота тайла в пикселях
  int     BigTIFF;      // условия создания BIGTIFF (см. BT_NO и т.д.)
  int     IsPlane;      // признак хранения изображения по плоскостям (0 - RGB RGB ..., 1 - RR...BB...GG...)
                        // при JPEG сжатии с цветовой схемой YCBCR должно быть = 0
  int     IsAlpha;      // признак использования альфа канала
  RGBQUAD Palette[256]; // палитра 1,4,8 битных растров
  int     Reserved[64];
}
  CREATETIFPARM;

#pragma pack(pop)

#endif  // MAPTYPE_H

