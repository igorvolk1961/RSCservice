#ifndef SEMANTIC_H
#define SEMANTIC_H

#ifdef LP64MODEL
//Типы, используемые для работы с данными

  typedef unsigned int UINT; //4 байта
  typedef int LONG; //4 байта
  typedef unsigned int ULONG; //4 байта
#else
  typedef unsigned int UINT;
  typedef long int LONG;
  typedef unsigned long int ULONG;
#endif

// --------------------------------------------------------
//  Определение числа типа "семантика"
// --------------------------------------------------------
#pragma pack(1)
typedef struct SEMCHAR
{
  unsigned char Type;        // Тип характеристики
  signed char  Scale;        // Масштабный коэффициент
  signed char  Value;        // Значение характеристики
}
  SEMCHAR;

typedef struct SEMSHORT
{
  unsigned char Type;        // Тип характеристики
  signed char  Scale;        // Масштабный коэффициент
  signed short Value;        // Значение характеристики
}
  SEMSHORT;

typedef struct SEMLONG
{
  unsigned char Type;        // Тип характеристики
  signed char  Scale;        // Масштабный коэффициент
#ifndef RISCCPU
  ::LONG     Value;        // Значение характеристики
#else
  char      Value[4];        // Значение характеристики
#endif
}
  SEMLONG;

typedef struct SEMDOUBLE
{
  unsigned char Type;        // Тип характеристики
  signed char  Scale;        // Масштабный коэффициент
#ifndef RISCCPU
  double       Value;        // Значение характеристики
#else
  char      Value[8];        // Значение характеристики
#endif
}
  SEMDOUBLE;

typedef struct SEMSTRING
{
  unsigned char Type;        // Тип характеристики
  unsigned char  Scale;      // Масштабный коэффициент
     char   Value[256];      // Значение характеристики
}
  SEMSTRING;

typedef struct SEMWSTRING
{
  unsigned char Type;        // Тип характеристики
  signed char  Scale;        // Масштабный коэффициент
#ifndef RISCCPU
  wchar_t Value[128];        // Значение характеристики в UNICODE
#else
  char    Value[128 * 2];    // Значение характеристики в UNICODE
#endif
}
  SEMWSTRING;
#pragma pack()

typedef union AnySemantic    // ЧИСЛО ТИПА СЕМАНТИКА
{
  SEMWSTRING WString;
  SEMSTRING  String;
  SEMCHAR    Char;
  SEMSHORT   Short;
  SEMLONG    Long;
  SEMDOUBLE  Double;
}
  SEMBASE;

// Типы характеристик
typedef enum sembasetype
{
  SEMBASESTRDOS = 0,   // CТРОКА СИМВОЛОВ (ASCIIZ),ОГРАНИЧЕННАЯ НУЛЕМ
  SEMBASESTRING = 126, // CТРОКА СИМВОЛОВ (ANSI),ОГРАНИЧЕННАЯ НУЛЕМ
  SEMBASESTRUNI = 127, // CТРОКА СИМВОЛОВ (UNICODE),ОГРАНИЧЕННАЯ НУЛЕМ
  SEMBASECHAR   = 1,   // 1 БАЙТ   SIGNED CHAR
  SEMBASESHORT  = 2,   // 2 БАЙТA  SIGNED SHORT
  SEMBASELONG   = 4,   // 4 БАЙТA  SIGNED INT
  SEMBASEDOUBLE = 8    // 8 БАЙТ   DOUBLE IEEE
}
  SEMBASETYPE;

typedef struct SEMANTIC       // СЕМАНТИКА ОБЪЕКТА
{
  unsigned short int Ident;   // 0x7FFF
  unsigned short int Length;  // Длина записи
}
  SEMANTIC;

typedef enum SEMVTYPE // Типы характеристик
{
  SEMVSTRDOS = 0,     // CТРОКА СИМВОЛОВ (ASCIIZ),ОГРАНИЧЕННАЯ НУЛЕМ
  SEMVSTRING = 126,   // CТРОКА СИМВОЛОВ (ANSI),ОГРАНИЧЕННАЯ НУЛЕМ
  SEMVSTRUNI = 127,   // CТРОКА СИМВОЛОВ (UNICODE),ОГРАНИЧЕННАЯ НУЛЕМ
  SEMVCHAR   = 1,     // 1 БАЙТ   SIGNED CHAR
  SEMVSHORT  = 2,     // 2 БАЙТA  SIGNED SHORT
  SEMVLONG   = 4,     // 4 БАЙТA  SIGNED INT
  SEMVDOUBLE = 8      // 8 БАЙТ   DOUBLE IEEE
}
  SEMVTYPE;


typedef struct SEMBASEVAL
{
  SEMBASE Sembase;
}
  SEMBASEVAL;

#pragma pack(1)
//typedef struct _DLLCLASS SEMBLOCK // СОСТАВ СЕМАНТИЧЕСКОЙ ХАРАКТЕРИСТИКИ
typedef struct SEMBLOCK // СОСТАВ СЕМАНТИЧЕСКОЙ ХАРАКТЕРИСТИКИ
{
#ifndef RISCCPU
  unsigned short int SemCode;     // КОД СЕМАНТИКИ
#else
  unsigned char      SemCode[2];
#endif
//#ifdef __cplusplus
//  TSemBase    Data;               // ЗНАЧЕНИЕ СЕМАНТИКИ - ЧИСЛО ТИПА СЕМАНТИКА
//#else
  SEMBASEVAL  Data;               // ЗНАЧЕНИЕ СЕМАНТИКИ - ЧИСЛО ТИПА СЕМАНТИКА
//#endif
}
  SEMBLOCK;
#pragma pack()

#endif // SEMANTIC_H
