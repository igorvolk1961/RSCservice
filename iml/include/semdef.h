/*****  semdef.h ***********  Belenkov O.V. ********** 23/03/22 ****
******  semdef.h ***********  Derevyagina Zh.A.******* 26/01/16 ****
******  semdef.h ***********  Kruzhkov A.E. ********** 21/03/16 ****
******  semdef.h ***********  Zheleznyakov V.A.******* 11/01/21 ****
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2022              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*         РЕДАКТИРОВАНИЕ СЕМАНТИКИ ОБ'ЕКТА ВЕКТОРНОЙ КАРТЫ         *
*                                                                  *
*******************************************************************/

#ifndef SEMDEF_H
#define SEMDEF_H

#ifndef MAPTYPE_H
  #include "maptype.h"
#endif

#if defined(BUILD_IML)
  #define HIDEBMX
#endif

#ifndef HIDEBMX
#ifndef LONGSTRING_H
  #include "longstring.h"
#endif

class TSearchBMX;
#endif

class TBitSetVariable;

typedef SEMANTIC* PSEMANTIC;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++  СЕМАНТИКА ОБ'ЕКТА ВЕКТОРНОЙ КАРТЫ  +++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef class TObjectSemn
{
public:

  TObjectSemn(SEMANTIC* addr) { Base = addr; }

  TObjectSemn(char* addr) { Base = (SEMANTIC*)addr; }

  TObjectSemn() { Base = 0L; }

  // Запросить адрес записи
  char* Address() { return (char*)Base; }
  SEMANTIC* Semantic() { return Base; }

  // Проинициализировать запись семантки
  // Установить идентификатор записи и длину
  // При ошибке возвращает 0, при удаче 1
  int Clear()
  {
    if (Base == 0)
      return 0;
    Base->Clear();
    return 1;
  }

  // Запросить длину данных
  int DataLength()
  {
    if (Base == 0)
      return 0;
    return Base->GetDataLength();
  }

  // Запросить длину записи
  int Length()
  {
    if (Base == 0)
      return 0;
    return Base->GetLength();
  }

  // Проверить корректность начала записи
  int IsCorrect()
  {
    if (Base == 0)
      return 0;
    return Base->IsCorrect();
  }

  SEMANTIC* operator = (SEMANTIC* addr)
  {
    return (Base = addr);
  }

  int operator == (const char* value)
  {
    return (int)(Base == (SEMANTIC*)value);
  }

  int operator != (const char* value)
  {
    return (int)(Base != (SEMANTIC*)value);
  }

  operator PSEMANTIC()
  {
    return Base;
  }

public:

  SEMANTIC* Base;  // УКАЗАТЕЛЬ НА СЕМАНТИКУ (ПЕРЕМЕННАЯ ДЛИНА)
}
  TObjectSemn;

#pragma pack(push,1)

// --------------------------------------------------------
//  Определение числа типа "семантика"
// --------------------------------------------------------
typedef struct SEMCHAR
{
  unsigned char Type;        // Тип формата записи характеристики (= 1)
  signed char  Scale;        // Масштабный коэффициент
  signed char  Value;        // Значение характеристики
}
  SEMCHAR;

typedef struct SEMSHORT
{
  unsigned char Type;        // Тип формата записи характеристики (= 2)
  signed char  Scale;        // Масштабный коэффициент
  signed short Value;        // Значение характеристики
}
  SEMSHORT;

typedef struct SEMLONG
{
  unsigned char Type;        // Тип формата записи характеристики (= 4)
  signed char  Scale;        // Масштабный коэффициент
#ifndef RISCCPU
  int          Value;        // Значение характеристики
#else
  char      Value[4];        // Значение характеристики
#endif
}
  SEMLONG;

typedef struct SEMDOUBLE
{
  unsigned char Type;        // Тип формата записи характеристики (= 8)
  signed char  Scale;        // Масштабный коэффициент
#ifndef RISCCPU
  double       Value;        // Значение характеристики
#else
  char      Value[8];        // Значение характеристики
#endif
}
  SEMDOUBLE;

typedef struct SEMLONGQUAD   // Набор чисел общей длиной 16 байт
{
  unsigned char Type;        // Тип формата записи характеристики (= 16)
  signed char  Scale;        // Масштабный коэффициент

  union
  {
    INTQUAD    Quad;
    INT64TWO   Two;
  };
}
  SEMLONGQUAD;

typedef struct SEMSTRING
{
  unsigned char   Type;      // Тип формата записи характеристики
  unsigned char  Scale;      // Масштабный коэффициент
     char   Value[256];      // Значение характеристики
}
  SEMSTRING;

typedef struct SEMWSTRING
{
  unsigned char   Type;      // Тип формата записи характеристики
  unsigned char  Scale;      // Масштабный коэффициент
  WCHAR2    Value[128];      // Значение характеристики в UNICODE
}
  SEMWSTRING;

typedef struct BIGSEMWSTRING // Строка UNICODE, содержащая более 127 символов
{
  unsigned char   Type;      // Тип формата записи характеристики - SEMBASEBIGUNI = 128 (0x80)
  unsigned char   Scale;     // Масштабный коэффициент 0xFF
#ifndef RISCCPU
  unsigned int    Size;      // Длина строки в байтах с замыкающими нулями (0x0000)
#else
  char            Size[4];   // Значение характеристики
#endif
  WCHAR2          Value[126]; // Значение характеристики в UNICODE длиной Size
}
  BIGSEMWSTRING;

typedef union AnySemantic    // ЧИСЛО ТИПА СЕМАНТИКА  (264 байт)
{
  char          ReserveSize[264];
  BIGSEMWSTRING BigString;
  SEMWSTRING    WString;
  SEMSTRING     String;
  SEMCHAR       Char;
  SEMSHORT      Short;
  SEMLONG       Long;
  SEMDOUBLE     Double;
  SEMLONGQUAD   Long4;
}
  SEMBASE;

typedef union MiniSemantic    // ЧИСЛО ТИПА СЕМАНТИКА (258 байт)
{
  BIGSEMWSTRING BigString;
  SEMWSTRING    WString;
  SEMSTRING     String;
  SEMCHAR       Char;
  SEMSHORT      Short;
  SEMLONG       Long;
  SEMDOUBLE     Double;
  SEMLONGQUAD   Long4;
}
  MINISEMBASE;

// Типы форматов записи характеристик
typedef enum sembasetype
{
  SEMBASESTRDOS = 0,   // CТРОКА СИМВОЛОВ (CP866), ОГРАНИЧЕННАЯ НУЛЕМ
  SEMBASECHAR   = 1,   // 1 БАЙТ   SIGNED CHAR
  SEMBASESHORT  = 2,   // 2 БАЙТA  SIGNED SHORT
  SEMBASELONG   = 4,   // 4 БАЙТA  SIGNED INT
  SEMBASEDOUBLE = 8,   // 8 БАЙТ   DOUBLE IEEE
  SEMBASELONG4  = 16,  // 16 БАЙТ  НАБОР ИЗ 4 INT
  SEMBASESTRING = 126, // CТРОКА СИМВОЛОВ (ANSI), ОГРАНИЧЕННАЯ НУЛЕМ
  SEMBASESTRUNI = 127, // CТРОКА СИМВОЛОВ (UNICODE), ОГРАНИЧЕННАЯ НУЛЕМ
  SEMBASEBIGUNI = 128, // CТРОКА СИМВОЛОВ (UNICODE) БОЛЬШОЙ ДЛИНЫ, ОГРАНИЧЕННАЯ НУЛЕМ
  SEMBASELAST   = 128, // ПОСЛЕДНЕЕ ЗНАЧЕНИЕ В СПИСКЕ
}
  SEMBASETYPE;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++      МЕТОДЫ ДОСТУПА К ЧИСЛАМ ТИПА СЕМАНТИКА     ++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifdef BUILD_DLL
typedef struct _DLLCLASS TSemBase
#else
typedef struct TSemBase
#endif
{
 public :

  // Заполнить класс числом типа семантика, либо подготовить место
  TSemBase()
  {
    memset((char *)&Sembase,0, sizeof(SEMBASE));
  }

  TSemBase(int type, const char * string)
  {
    LoadAnyString(type, string, 0);
  }

  TSemBase(signed char charval)
  {
    Scale(0);
    Value(charval);
  }

  TSemBase(short shortval)
  {
    Scale(0);
    Value(shortval);
  }

  TSemBase(int longval)
  {
    Scale(0);
    Value(longval);
  }

  TSemBase(double doubleval)
  {
    Value(doubleval);
  }

  // Проверить корректность числа с плавающей точкой
  int CheckDoubleValue();

  // Очистить блок данных
  void Clear() { memset(Sembase.String.Value, 0, sizeof(Sembase.String.Value)); Scale(0); Type(SEMBASESTRING); }

  // Cравнить текущую характеристику с заданной
  // isstr = -1 - ищем подстроку
  // isstr =  1 - ищем по сравнению строк
  // isstr =  2 - ищем по спец.символам
  // Для сравнения полученного результата с требуемым
  // использовать операцию "&"
  // if (one->Compare(two) & CMNOTEQ) { ... }
//Volkov I.A. BEGIN
//#ifndef HIDEBMX
//  HOW _fastcall Compare(TSemBase * base, int isstr = 0, TSearchBMX* bmxSearch = 0);
//#else
//  HOW _fastcall Compare(TSemBase * base, int isstr = 0);
//#endif
//Volkov I.A. END

  // Cравнить текущую характеристику со строкой
  HOW _fastcall Compare(const WCHAR* value);

  // Сформировать GUID в виде строки шестнадцатеричных цифр
  int CreateGUID();

  // Запросить значение характеристики в заданном виде
  double DoubleValue();

  // Запросить длину значения характеристики в байтах
  unsigned int DataLength();

  // Запросить длину значения характеристики со служебными полями
  unsigned int Length();

  // Преобразовать строку,содержащую цифровое значение,
  // к числу типа "семантика"
  // При ошибке возвращает ноль
  int _fastcall LoadStringNumber(const char * string);
  int _fastcall LoadAnyString(int type, const WCHAR * value);  

  // Преобразовать строку к числу типа "семантика"
  // в соответствии с типом характеристики (TYPE)
  // При ошибке возвращает ноль
  int _fastcall LoadAnyString(int type, const char * string, int maxsize);

  // Запросить значение характеристики в виде целого числа
  int LongValue();

  // Запросить значение характеристики в виде даты ("DD/MM/YYYY")
  int _fastcall DateValue(char * place, int maxsize);

  // Запросить масштабный коэффициент
  int Scale() { return(Sembase.Char.Scale); }
  int StringSize() { return Scale(); }

  // Запросить тип характеристики
  int Type() { return Sembase.String.Type; }

  // Установить тип характеристики
  int Type(int type) { return (Sembase.String.Type = (unsigned char)type); }

  // Запросить значение характеристики в заданном виде
  int _fastcall StringUnicode(WCHAR * place, int maxsize);
  int _fastcall StringUnicode8(char * place, int maxsize);

  // Запросить значение характеристики в заданном виде
  int _fastcall StringValue(char * place,int maxsize);

  // Перекодировать строку символов из ASCII (DOS) в ANSI
  // Выполняется автоматически при обращении к строке
  // Если значение характеристики не символьное - возвращает ноль
  char * StringInAnsi();

  // Запросить указатель на строку в кодировке UTF-16
  // Если значение характеристики другого вида - возвращает ноль
  const WCHAR * StringInUTF16();

  // Запросить значение семантики типа "набор чисел"
  // Если тип числа не SEMBASELONG4 и не строка - возвращает ноль
  int _fastcall QuidValue(INT64TWO *quid);

  // Установить значение характеристики
  signed char Value(signed char value)
  {
    Type(SEMBASESHORT);
    return (signed char)(Sembase.Short.Value = value);
  }

  signed short Value(signed short value)
  {
    Type(SEMBASESHORT);
    return(Sembase.Short.Value = value);
  }

  int Value(long int value)
  {
    return Value((int)value);
  }

  int Value(int value)
  {
    Type(SEMBASELONG);
#ifndef RISCCPU
    return (Sembase.Long.Value = value);
#else
    REGISTER reg;
    reg.Long = value;
    return reg.PutLong(Sembase.Long.Value);
#endif
  }

  double Value(double value)
  {
    Type(SEMBASEDOUBLE);
    Scale(0);
#ifndef RISCCPU
    return(Sembase.Double.Value = value);
#else
    DOUBLEREGISTER reg;
    reg.Double = value;
    return reg.PutDouble(Sembase.Double.Value);
#endif
  }

  // Строка должна быть в кодах ANSI !
  // За перекодировку отвечает тот, кто вызывает эту функцию
  char * _fastcall Value(const char * value, int maxsize);

  // Строка должна быть в UNICODE
  // За перекодировку отвечает тот, кто вызывает эту функцию
  int _fastcall Value(const WCHAR * value);

  // Установить значение строки WCHAR в заранее выделенную область памяти
  // size - длина строки в байтах вместе с замыкающим нулевым символом
  // При ошибке возвращает ноль
  int _fastcall BigValue(const WCHAR * value, int size, int addzero);

 protected :

  // Cравнить две строки
  // При совпадении возвращает CMEQUAL
  HOW CompareStringValueUn(const WCHAR * valu1, const WCHAR * valu2, int isstr);
  HOW CompareStringValue(const char * valu1, const char * valu2, int isstr);

  //  Установить масштабный коэффициент
  int Scale(int scale)
  {
    return (int)(Sembase.Char.Scale = (signed char)scale);
  }

  int StringSize(int scale) { return Scale(scale); }

 public :

  SEMBASE Sembase; // Число типа "семантика"
}
  TSemBase;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++ МЕТОДЫ ДОСТУПА К СЕМАНТИЧЕСКИМ ХАРАКТЕРИСТИКАМ  ++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef class _DLLCLASS TSemBlock
{
  public:

   // Инициализация или очистка класса
   TSemBlock()
   {
     Code(0);
     Value(0);
   }

   TSemBlock(int code, TSemBase * sbase)
   {
     Code(code);
     memmove((char *)&Data,(char *)sbase,sbase->Length());
   }

   TSemBlock(int code, int type, const char * value, int maxsize = 0)
   {
     Code(code);
     Value(type,value,maxsize);
   }

   // Проверить корректность числа с плавающей точкой
   int CheckDoubleValue() { return Data.CheckDoubleValue(); }

   // Очистить блок данных
   void Clear() { Data.Clear(); }

   // Запросить код семантики
   int Code()
   {
#ifndef RISCCPU
     return SemCode;
#else
     REGISTER reg;
     ShortToRegister(reg, SemCode);
     return reg.Long;
#endif
   }

   // Установить код семантики
   int Code(int code)
   {
#ifndef RISCCPU
     return (SemCode = (unsigned short int)code);
#else
     REGISTER reg;
     ShortToRegister(reg, &code);
     RegisterToShort(reg, SemCode);
     return reg.Long;
#endif
   }

   // Сравнить код семантики + значение
//Volkov I.A. BEGIN
//   HOW Compare(TSemBlock * semdata)
//   {
//     if ((semdata == 0) || (Code() != semdata->Code()))
//       return CMBAD;

//     return CompareData(semdata->GetData());
//   }
//Volkov I.A. END

   // Сравнить код характеристики
   HOW CompareCode(unsigned int code)
   {
     unsigned int semcode = Code();

     if (code < semcode)
       return CMLESS;
     if (code > semcode)
       return CMMORE;
     return CMEQUAL;
   }

   // Сравнить значение характеристики
   // isstr = -1 - ищем подстроку
   // isstr =  1 - ищем по сравнению строк
   // isstr =  2 - ищем по спец.символам
   // При совпадении возвращает CMEQUAL
//Volkov I.A. BEGIN
//#ifndef HIDEBMX
//   HOW CompareData(TSemBase * sembase, int isstr = 0, TSearchBMX* bmxSearch = 0) { return Data.Compare(sembase, isstr, bmxSearch); }
//   HOW CompareData(TSemBlock& semdata, int isstr = 0, TSearchBMX* bmxSearch = 0) { return Data.Compare(semdata.GetData(), isstr, bmxSearch); }
//#else
//   HOW CompareData(TSemBase * sembase, int isstr = 0) { return Data.Compare(sembase, isstr); }
//   HOW CompareData(TSemBlock& semdata, int isstr = 0) { return Data.Compare(semdata.GetData(), isstr); }
//#endif
//Volkov I.A. END

   // Cравнить текущую характеристику со строкой
//Volkov I.A. BEGIN
//   HOW _fastcall CompareData(const WCHAR* value) { return Data.Compare(value); }
//Volkov I.A. END

   // Сформировать GUID в виде строки шестнадцатеричных цифр
   int CreateGUID() { return Data.CreateGUID(); }

   // Запросить значение характеристики в заданном виде
   double DoubleValue() { if (Code() == SEMDATEFROM) return (double)LongValue(); return Data.DoubleValue(); }

   // Запросить данные блока
   TSemBase * GetData() { return &Data; }

   // Запросить длину значения семантики и длину кода
   int Length() { return (sizeof(SemCode) + Data.Length()); }

   // Запросить длину значения семантики
   int DataLength() { return Data.DataLength(); }

   // Запросить значение характеристики в заданном виде
   int LongValue();

   // Запросить значение характеристики в виде даты ("DD/MM/YYYY")
   int _fastcall DateValue(char * place, int maxsize) { return Data.DateValue(place, maxsize); }

   // Перекодировать строку символов из ASCII (DOS) в ANSI
   // Если значение характеристики не символьное - возвращает ноль
   char * StringInAnsi() { return Data.StringInAnsi(); }

   // Запросить указатель на строку в кодировке UTF-16
   // Если значение характеристики другого вида - возвращает ноль
   const WCHAR * StringInUTF16() { return Data.StringInUTF16(); }

   // Запросить значение характеристики в заданном виде
   // place - адрес области, куда помещается строка
   // maxsize - предельный размер выходной строки
   int StringValue(char *place, int maxsize)
   {
     return Data.StringValue(place, maxsize);
   }

   int StringUnicode(WCHAR *place, int maxsize)
   {
     return Data.StringUnicode(place, maxsize);
   }

   int StringUnicode8(char *place, int maxsize)
   {
     return Data.StringUnicode8(place, maxsize);
   }

   // Запросить значение семантики типа "набор чисел"
   int QuidValue(INT64TWO *quid) { return Data.QuidValue(quid); }

   // Запросить тип значения семантики
   int Type() { return (int)Data.Type(); }

   // Запросить длину поля данных для строки
   int StringSize() { return Data.Scale(); }

   // Содержит ли блок данные в UNICODE
   int IsUnicode() { return ((Type() == SEMBASESTRUNI) || (Type() == SEMBASEBIGUNI)); }

   // Установить значение характеристики
   signed char Value(signed char value) { return Data.Value(value); }

   signed short Value(signed short value) { return Data.Value(value); }

   int Value(int value) { return Data.Value(value); }

   double Value(double value) { return Data.Value(value); }

   // Строка должна быть в кодах ANSI/KOI8
   char * Value(const char * value, int maxsize = 0)
   {
     return Data.Value(value,maxsize);
   }

   // Строка должна быть в кодах ANSI/KOI8
   // При ошибке возвращает ноль
   int Value(int type,const char * value,int maxsize = 0)
   {
     return Data.LoadAnyString(type,value,maxsize);
   }

   // Строка должна быть в UNICODE
   // При ошибке возвращает ноль
   int Value(const WCHAR * value) { return Data.Value(value); }

   // Установить значение строки WCHAR в заранее выделенную область памяти
   // size - длина строки в байтах вместе с замыкающим нулевым символом
   // При ошибке возвращает ноль
   int _fastcall BigValue(const WCHAR * value, int size, int addzero)
   {
     return Data.BigValue(value, size, addzero);
   }

   // Преобразовать строку к числу типа "семантика"
   // в соответствии с типом характеристики (TYPE)
   // При ошибке возвращает ноль
   int _fastcall LoadAnyString(int type, const char * string, int maxsize)
   {
     return Data.LoadAnyString(type, string, maxsize);
   }

   // Преобразовать строку,содержащую цифровое значение,
   // к числу типа "семантика"
   // При ошибке возвращает ноль
   int _fastcall LoadAnyString(int type, const WCHAR * value)
   {
     return Data.LoadAnyString(type, value);
   }

  protected:

#ifndef RISCCPU
   unsigned short int SemCode;     // КОД СЕМАНТИКИ
#else
   unsigned char      SemCode[2];
#endif
   TSemBase           Data;        // ЗНАЧЕНИЕ СЕМАНТИКИ - ЧИСЛО ТИПА СЕМАНТИКА
   char               Reserve[6];
}
  TSemBlock;

#pragma pack(pop)

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++ МЕТОДЫ ОБРАБОТКИ НАБОРА СЕМАНТИЧЕСКИХ ХАРАКТЕРИСТИК ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class _DLLCLASS TWorkSemantic
{
 public:

   // Конструкторы класса
   TWorkSemantic(char * begin,           // начало семантики
                 int lengthsemantic,     // общая длина семантики
                 int lengthbuffer = 0)   // длина буфера
   {
     Access(begin,lengthsemantic,lengthbuffer);
   }

   TWorkSemantic()
   {
     Block     = 0;
     AppLen    = 0;     // нельзя добавлять
     IsCorrect = 0;     // длина семантики не проверялась
     SemLength = 0;
     BufLength = 0;
     EndSem    = 0;
     BegSem    = 0;
     Number    = 0;

     IsDirty(0);
   }

   // Деструктор класса
   ~TWorkSemantic(){}

   // Установить доступ к семантике объекта
   int _fastcall Access(SEMANTIC * semn);

   // Установить доступ к семантике объекта
   void _fastcall Access(char * begin,          // начало семантики
                         int lengthsemantic,    // общая длина семантики
                         int lengthbuffer = 0); // общая длина буфера

   // Добавить характеристику (в конец семантики)
   int Append(TSemBlock& block)
   {
     GoEnd();                   // перешли на конец
     return(Insert(block));     // добавили
   }

   int Append(int code, const WCHAR * value, int size)
   {
     GoEnd();                                // перешли на конец
     return (Insert(code, value, size));     // добавили
   }

   // Запросить длину буфера
   int BufferLength() { return BufLength; }

   // Проверка и коррекция длины семантики
   void CheckSemLength();

   // Проверка и коррекция длины семантики
   // Если данные корректировались - возвращает -1
   int CheckSemStruct(int * group = 0, int * textgroup = 0);

   // Удалить текущую характеристику
   // При ошибке возвращает ноль
   int Delete();

   // Вставить характеристику (перед текущим блоком )
   // При ошибке возвращает ноль
   int _fastcall Insert(TSemBlock& block);

   int _fastcall Insert(int code, const WCHAR * value, int size);

   // Запросить/Установить признак редактирования данных
   int IsDirty() { return Isdirty; }
   int IsDirty(int dirty) { if (dirty == 0) IsRealdirty = 0; return (Isdirty = (dirty != 0)); }

   // Запросить/Установить признак обновления семантики для неслужебных кодов
   int _fastcall IsRealDirty(int code);
   int _fastcall IsRealDirty() { return IsRealdirty; } 

   // Найти характеристику по коду семантики и значению
   // При ошибке возвращает ноль
   TSemBlock * _fastcall FindCodeAndValue(int code, const char * value);
   TSemBlock * _fastcall FindCodeAndValue(int code, int value);
   TSemBlock * _fastcall FindCodeAndValue(int code, double& value);

   // Найти характеристику по коду семантики и сравнить значение
   // Если семантика с кодом code не найдена - возвращает CMBAD !!!
   HOW _fastcall CompareCodeAndValue(int code, const char * value);
   HOW _fastcall CompareCodeAndValue(int code, long value);
   HOW _fastcall CompareCodeAndValue(int code, double& value);

   // Найти характеристику по коду семантики
   // При ошибке возвращает ноль
   TSemBlock * _fastcall FindCode(int code);

   // Продолжить поиск характеристики по коду семантики(после текущего)
   // При ошибке возвращает ноль
   TSemBlock * _fastcall FindCodeNext(int code);

   // Найти характеристику по коду семантики
   // При ошибке возвращает ноль
   TSemBlock * _fastcall FindCode(int code, int number);

   // Найти значение в виде целого числа по коду семантики
   // При ошибке возвращает ноль
   int _fastcall FindCodeLongValue(int code, int number);

   // Найти значение в виде числа с плавающей точкой по коду семантики
   // При ошибке возвращает ноль
   int _fastcall FindCodeDoubleValue(int code, double& value, int number);

   // Найти минимальное значение в виде числа с плавающей точкой по коду семантики
   // При ошибке возвращает ноль
   int _fastcall GetCodeMinDoubleValue(int code, double& value);

   // Найти максимальное значение в виде числа с плавающей точкой по коду семантики
   // При ошибке возвращает ноль
   int _fastcall GetCodeMaxDoubleValue(int code, double& value);

   // Найти среднее значение в виде числа с плавающей точкой по коду семантики
   // При ошибке возвращает ноль
   int _fastcall GetCodeMeanDoubleValue(int code, double& value);

   // Найти характеристику по коду семантики и значению
   // При ошибке возвращает ноль
   TSemBlock * _fastcall FindBlock(TSemBlock& block);

   // Продолжить поиск характеристики по коду семантики и значению (после текущего)
   // При ошибке возвращает ноль
   TSemBlock * FindBlockNext(TSemBlock * block);

   // Найти номер группы объекта в семантике для кодов
   // SEMGROUPLEADER, SEMGROUPSLAVE, SEMGROUPPARTNER
   // code - поле для записи найденного кода семантики
   // При ошибке возвращает ноль
   int _fastcall FindGroup(int * code);

   // Найти семантику заданного типа
   // index - порядковый номер семантики, с которой начинается поиск
   // Возвращает номер семантики заданного типа
   // При ошибке возвращает ноль
   int _fastcall FindType(int index, TBitSetVariable& bits);

   // Запросить код характеристики
   // number - последовательный номер характеристики
   // При ошибке возвращает ноль
   int GetCode(int number)
   {
     if (GoNumber(number) == 0) return 0;
     return Block->Code();
   }

   //  Перейти на конец записи семантики
   // (за последней характеристикой)
   TSemBlock * GoEnd();

   // Перейти на первую характеристику
   TSemBlock * GoFirst()
   {
     Number = 1;
     return (Block = (TSemBlock *)BegSem);
   }

   // Перейти на следующую характеристику из уже существующих
   // При ошибке возвращает ноль
   TSemBlock * GoNext();

   // Запросить адрес текущей характеристики
   TSemBlock * GoNumber() { return Block; }
   TSemBlock * GoCurrent() { return GoNumber(); }

   // Перейти на характеристику с порядковым номером number
   // При ошибке возвращает ноль
   TSemBlock * _fastcall GoNumber(int number);

   // Запросить длину семантики
   int Length()
   {
     if (IsCorrect == 0) { CheckSemLength(); IsCorrect = 1; }
     return SemLength;
   }

   // Запросить последовательный номер текущей характеристики
   int CurrentNumber() { return Number; }

   // Заменить код характеристики у текущего блока
   // При ошибке возвращает ноль
   TSemBlock * ReplaceCode(int code)
   {
      if ((Block == (TSemBlock *)EndSem) || (Block == 0))
        return 0;  // если такого блока нет

      Block->Code(code);
      IsDirty(1);
      return(Block);
   }

   //  Заменить значение характеристики у текущего блока
   int _fastcall ReplaceValue(int value);

   // Заменить значение характеристики у текущего блока
   int _fastcall ReplaceValue(const char * value, int maxsize);

   // Заменить значение характеристики у текущего блока
   int _fastcall ReplaceValue(const WCHAR * value, int maxsize);

   // Определить количество семантик
   int Count() { return SemanticsAmount(); }
   int SemanticsAmount();

   // Запросить значение характеристики в символьном виде (ANSI)
   // number - последовательный номер характеристики,
   // place - адрес размещения строки,
   // maxsize - максимальная длина строки
   // При ошибке возвращает ноль,
   // при успешном выполнении - адрес строки (равен place)
   int StringValue(int number,char *place,int maxsize)
   {
     if (GoNumber(number) == 0) return 0;
     return Block->StringValue(place,maxsize);
   }

   int StringValueUn(int number,WCHAR *place,int maxsize)
   {
     if (GoNumber(number) == 0) return 0;
     return Block->StringUnicode(place,maxsize);
   }

   // Запросить значение семантической характеристики объекта
   // в виде числа с плавающей точкой двойной точности
   // number  - последовательный номер характеристики
   // Если значение семантики не может быть преобразовано
   // к числовому виду или - возвращает 0
   double DoubleValue(int number)
   {
     if (GoNumber(number) == 0) return 0;
     return Block->DoubleValue();
   }

   // Заменить текущую характеристику
   // При ошибке возвращает ноль
   int _fastcall Update(TSemBlock& block);   

   // Установить длину семантики
   // Возвращает новую длину семантики или 0 при ошибке
   unsigned int _fastcall Length(unsigned int value);

   // Загрузить набор семантических характеристик
   // из заданной области
   // При ошибке возвращает ноль
   // При успешном выполнении - размер созданной записи
   int Load(TWorkSemantic * worksemn);

 public:

  TSemBlock *  Block;      // Указатель на текущий блок семантики
  char *       EndSem;     // Конец семантики
  char *       BegSem;     // Начало семантики
#if !defined(_M_X64) && !defined(BUILD_DLL64)
  char *       WZero;      // Выравнивание на 8
#endif

  int          AppLen;     // 1 - разрешает увеличивать общую длину семантики
  int          IsCorrect;  // 1 - длина семантики корректна
  int          SemLength;  // Длина записи семантики
  int          BufLength;  // Длина буфера семантики

  int          Number;     // Номер текущей характеристики
  int          Isdirty;    // Признак редактирования семантики
  int          IsRealdirty;
  int          IZero;
};

#ifndef _GISSERVER
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++ ЗАПОЛНЕНИЕ ЗАПИСИ СЕМАНТИКИ ++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
typedef class TSemanticRecord : public TObjectSemn
{
public:

  TSemanticRecord() : TObjectSemn(), Work() { BufferLength = 0; SemZero = 0; }

  // Запросить число семантик
  int Count()
  {
    if (Base == 0)
      return 0;
    return Work.Count();
  }

  // Добавить характеристику (в конец семантики)
  // type    - предполагаемый тип значения (TYPE)
  // code    - код семантики
  // place   - адрес строки,содержащей значение семантики
  // maxsize - длина строки
  // При ошибке возвращает ноль
  int Append(int code, const char* place, int maxsize, int type = SEMBASESTRING)
  {
    TSemBlock newblock;
    newblock.Code(code);
    if (newblock.Value(type, place, maxsize) == 0)
      return 0;
    return (Append(newblock) != 0);
  }

  // Добавить строку UNICODE (в конец семантики)
  // При ошибке возвращает ноль
  int _fastcall Append(int code, const WCHAR* place, int maxsize = 0);

  // Добавить характеристику (в конец семантики)
  // При ошибке возвращает ноль
  int _fastcall Append(int code, int value);
  int _fastcall Append(int code, double value);

  // Добавить характеристику (в конец семантики)
  // При ошибке возвращает ноль
  int _fastcall Append(TSemBlock& block);

  // Проверить реальную длину записи
  // При ошибке возвращает ноль
  int _fastcall CheckLength();

protected:

  // Выделить память под максимальную запись семантики
  // Проинициализировать структуру данных
  // При ошибке возвращает ноль
  int _fastcall Allocate(int length);

  // Освободить память
  void FreeMemory();

protected:

  TWorkSemantic Work;           // Описание буфера семантики

  int           BufferLength;   // Размер буфера памяти
  int           SemZero;        // Выравнивание
}
  TSemanticRecord;
#endif


#ifndef HIDEBMX
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++ Поиск Бойера-Мура-Хорспула +++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class TSearchBMX
{
 public:

  TSearchBMX(const WCHAR *substr = 0);
  ~TSearchBMX();

  // Найти подстроку substr в строке value
  // При ошибке возвращает ноль
  const WCHAR* SearchSubstring(const WCHAR* value, const WCHAR *substr);
  const WCHAR* SearchSubstring(const WCHAR* value);

 protected:

  // Заполнить значения смещения для каждого символа подстроки
  // substr - подстрока поиска
  // length - длина подстроки
  void FillShiftsArrayForSubstring(const WCHAR *substr, int length);

  // Запросить значение смещения из массива
  short int GetShiftArray(short int symbol)
  {
    if (((symbol -= MinValue) < 0) || (symbol > NormalMaxValue))
      return Step;
    return ShiftArray[symbol];
  }

  // Установить подстроку substr
  // При ошибке возвращает ноль
  int SetSubstring(const WCHAR * substr);

 protected:

  short int * ShiftArray;            // Массив смещений для каждого символа
#if !defined(_M_X64) && !defined(BUILD_DLL64)
  char *      XZero;                 // Выравнивание на 8
#endif

  TLongStringUn SubStr;              // Подстрока поиска

  short int MinValue;                // Минимальное значение символа подстроки
  short int NormalMaxValue;          // Максимальное значение символа подстроки
  short int Step;
  short int Zero;
};
#endif  // !HIDEBMX

#endif  // SEMDEF_H
