/* 
 * File:   rscheader.h
 * Author:   Volkov Igor
 *
 * Created on 28 Январь 2013 г., 17:20
 */

#ifndef _RSCHEADER_H
#define	_RSCHEADER_H


typedef  struct {
                                // ИДЕНТИФИКАТОР ФАЙЛА
int   RscLabel         ;  // 0x0D524944 (RSC)
int   HeaderLength     ;  // ДЛИНА ЗАГОЛООВКА В БАЙТАХ
//unsigned short int   Edition ;  // 0x0700
//int   Encoding         ;  // Кодировка
int   Edition ;  // 0x0700
unsigned short int   Encoding         ;  // Кодировка
int   State            ;  // Номер состояния файла
int   ModificationState;  // Номер модификации состояния
int   Language         ;  // Используемый язык (1-английский, 2-русский)
int   NextObject       ;  // Идентификатор для нового объекта
char        CreationDate[8]  ;  //[10] ;  // ДАТА СОЗДАНИЯ ФАЙЛА ГГГГММДД//(ДД/ММ/ГГ\0)
char        MapType[32]      ;  // Тип карты
char        RscName[32]      ;  // Условное название классификатора
char        RscCode[8 ]      ;  // Код классификатора
int   Scale            ;  // Масштаб карты
int   ScaleTable       ;  // Масштабный ряд
int   ObjectOffset     ;  // Смещение на таблицу объектов от начала файла
int   ObjectLength     ;  // Длина таблицы объектов в байтах
int   ObjectCount      ;  // Число записей
int   SemanticOffset   ;  // Смещение на таблицу семантики от начала файла
int   SemanticLength   ;  // Длина таблицы семантики
int   SemanticCount    ;  // Число записей
int   SemanticClassOffset   ;  // Смещение на таблицу классификатор семантики от начала файла
int   SemanticClassLength   ;  // Длина таблицы классификатор семантики
int   SemanticClassCount    ;  // Число записей
int   DefaultValueOffset    ;  // Смещение на таблицу умолчаний от начала файла
int   DefaultValueLength    ;  // Длина таблицы умолчаний в байтах
int   DefaultValueCount     ;  // Число записей
int   ObjectSemanticOffset  ;  // Смещение на таблицу возможных семантик от начала файла
int   ObjectSemanticLength  ;  // Длина таблицы возможных семантик
int   ObjectSemanticCount   ;  // Число записей
int   SegmentOffset         ;  // Смещение на таблицу сегментов от начала файла
int   SegmentLength         ;  // Длина таблицы сегментов в байтах
int   SegmentCount          ;  // Число записей
int   ValueBoundaryOffset   ;  // Смещение на таблицу порогов от начала файла
int   ValueBoundaryLength   ;  // Длина таблицы порогов в байтах
int   ValueBoundaryCount    ;  // Число записей
int   ParameterOffset       ;  // Смещение на таблицу параметров от начала файла
int   ParameterLength       ;  // Длина таблицы параметров в байтах
int   ParameterCount        ;  // Число записей
int   PrintParameterOffset  ;  // Смещение на таблицу параметров печати от начала файла
int   PrintParameterLength  ;  // Длина таблицы параметров печати в байтах
int   PrintParameterCount   ;  // Число записей
int   PaletteOffset         ;  // Смещение на таблицу палитр от начала файла
int   PaletteLength         ;  // Длина таблицы палитр в байтах
int   PaletteCount          ;  // Число записей
int   FontOffset            ;  // Смещение на таблицу шрифтов от начала файла
int   FontLength            ;  // Длина таблицы палитр в шрифтов
int   FontCount             ;  // Число записей
int   LibraryOffset         ;  // Смещение на таблицу библиотек от начала файла
int   LibraryLength         ;  // Длина таблицы палитр в библиотек
int   LibraryCount          ;  // Число записей
char        Reserve[32]           ;  // Резерв
int   FontEncoding          ;  // Кодировка шрифтов (125- KOI8, 126 - ANSI, WINDOWS)
int   ColorCount            ;  // Количество цветов в палитрах
} RSCHEADER;

typedef  struct {
int   RecordLength          ;  // Длина записи
char        Name[32]              ;  // Название слоя
char        ShortName[16]         ;  // Короткое название слоя
unsigned char  Number             ;  // Номер слоя (сегмента)
unsigned char  Order              ;  // Порядок отображения объектов слоя
unsigned short SemanticCount      ;  // Количество семантик слоя
int   SemanticCode          ;  //  Коды семантик
} SEGMENTHEADER;

typedef  struct {
int RecordLength;  // Длина записи объекта
int Excode;        // Классификационный код
int Incode;        // Внутренний код объекта (порядковый номер)
int Code;          // Идентификационный код
char     ShortName[32]; // Короткое имя объекта
char     Name[32];      // Название
unsigned char Local;    // Характер локализации
unsigned char Segment;  // Номер сегмента
unsigned char Scalable; // Признак масштабируемости
unsigned char LowerScale; // Нижняя граница видимости
unsigned char UpperScale; // Верхняя граница видимости
unsigned char LocalExt; // Расширение локализации
unsigned char Direct;   // Направление цифрования
unsigned char DrawWithSemantic; // Отображение с учетом семантики
unsigned short Extension; // Номер объекта в серии

int LabelCode;     // Идентификационный код связанной подписи
int LabelSemantic; // Классификационный код семантики
char     Reserv[7];     // Резерв
unsigned short LabelPrecision; //Количество десятичных знаков после запятой
} OBJECTHEADER;

typedef  struct {
int Code;          // Код семантики
unsigned short Type;    // Тип значения семантики
unsigned char  Multiple;// Повторяемость семантики
unsigned char  Service; // Признак служебной семантики
char     Name[32];      // Название
char     ShortName[16]; // Короткое имя семантики
char     MesureUnit[8]; // Единица измерения
unsigned short Size;    // Размер поля семантики
unsigned char  Precision;// Точность семантики
unsigned char  Reserv;   // Резерв
int ClassifierOffset;// Смещение на описание классификатора семантики
int ClassifierCount; // Количество записей в классификаторе данной семантики
int DefaultValueOffset;// Смещение на умалчиваемые значения семантики
int DefaultValueCount; // Количество записей для умалчиваемых значений
} SEMANTICHEADER;

typedef struct {
int RecordLength;
short Incode;
short Function;
} PARAMHEADER;

typedef struct {
int  Colors[256];
char Name[32];
} PALETTE;

typedef struct {
int Value;
char Name[32];
} SEMANTICCLASS;

typedef struct {
int Incode;
int Code;
double MinValue;
double DefaultValue;
double MaxValue;
} DEFAULTVALUE;

typedef struct {
int RecordLength;
int Excode;
unsigned char Local;
unsigned char Reserv[3];
short ObligatorySemanticCount;
short OptionalSemanticCount;
} OBJECTSEMANTIC;


typedef struct {
int RecordLength;
int Excode;
unsigned char Local;
unsigned char Reserv[7];
int Code1;
short LimitCount1;
short DefaultLimit1;
int Code2;
short LimitCount2;
short DefaultLimit2;
} VALUEBOUNDARY;

typedef struct {
char FileName[32];
char Name[32];
int  Incode;
unsigned char ReperSymbolIndex;
unsigned char CodePage;
unsigned char Resrev[2];
} FONT;

typedef struct {
char FileName[80];
char Name[32];
int  Incode;
int  CurrentLibraryIndex;
} LIBRARY;

#endif	/* _RSCHEADER_H */

