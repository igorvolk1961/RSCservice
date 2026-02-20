/******  semdef.cpp ********* Belenkov O.V.   ****** 20/10/21 ******
*******  semdef.cpp ********* Кружков А.Е.    ****** 26/07/11 ******
*******  semdef.cpp ********* Shishkova L.A   ****** 02/06/21 ******
*******  semdef.cpp ********* Shabakov D.A    ****** 28/02/20 ******
*******  semdef.cpp ********* Derevyagina Zh.A****** 13/01/21 ******
*******  semdef.cpp ********* Savelov A.S.    ****** 03/03/20 ******
*******  semdef.cpp ********* Ganyushin K.A.  ****** 08/08/18 ******
*******  semdef.cpp ********* Zheleznyakov V.A. **** 11/01/21 ******
*******  semdef.cpp ********* Vitsko D.A      ****** 10/03/21 ******
*******  semdef.cpp ********* Tarakanova I.R. ****** 06/04/21 ******
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2021              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*         РЕДАКТИРОВАНИЕ СЕМАНТИКИ ОБ'ЕКТА ВЕКТОРНОЙ КАРТЫ         *
*                                                                  *
*******************************************************************/

#include <math.h>
#include <string.h>

#include "semdef.h"

#ifndef MAPBITS_H
 #include "mapbits.h"
#endif

#ifndef LONGSTRING_H
 #include "longstring.h"
#endif

#ifndef BUILD_IML
#ifndef MAPFILES_H
 #include "mapfiles.h"
#endif

#else

// ------------------------------------------------------------------
// Преобразовать строку (32/36 байт) в GUID (16 байт, без замыкающего нуля)
// При ошибке возвращает ноль
// ------------------------------------------------------------------
int StringToGUID(const char * point, char * guid)
{
  if ((guid && point) == 0)
    return 0;

  if (*point == '{')
    point++;

  // Преобразовать строку из 32 шестнадцатеричных символов в двоичный вид
  int size = (int)strlen(point);
  if ((size > 0) && (*(point + size - 1) == '}'))
    size--;

  if ((size != 32) && (size != 36))
    return 0;

  for (int i = 0; i < 16; i++)
  {
    char value;
    char value2;

    value = *point++;
    if (value == '-')
    {
      // Пропустить символ '-'
      i--;
      continue;
    }

    if (value > 0x40)
      value -= 0x37;
    else
      value -= 0x30;

    value = value << 4;

    value2 = *point++;
    if (value2 > 0x40)
      value2 -= 0x37;
    else
      value2 -= 0x30;

    value |= value2;
    *guid = value;
    guid++;
  }

  return 16;
}

// ------------------------------------------------------------------
// Преобразовать указатель на GUID (16 байт) в строку (37 байт)
// {G0G1G2G3-G4G5-G6G7-G8G9-G10G11G12G13G14G15}, где Gx — значение соответствующего байта
// Возвращает длину созданной строки
// При ошибке возвращает ноль
// ------------------------------------------------------------------
int GUIDToString(const unsigned char * guid, char * point)
{
  if ((guid && point) == 0)
    return 0;

  // Преобразовать в строку шестнадцатеричных цифр
  for (int i = 0; i < 4; i++)
  {
    unsigned char byte = *guid >> 4;
    if (byte < 10)
      *point = 0x30 + byte;
    else
      *point = 0x37 + byte;
    point++;

    byte = *guid & 0x0F;
    if (byte < 10)
      *point = 0x30 + byte;
    else
      *point = 0x37 + byte;
    point++;
    guid++;
  }

  for (int k = 0; k <= 2; k++)
  {
    *point = '-';
    point++;

    for (int i = 0; i < 2; i++)
    {
      unsigned char byte = *guid >> 4;
      if (byte < 10)
        *point = 0x30 + byte;
      else
        *point = 0x37 + byte;
      point++;

      byte = *guid & 0x0F;
      if (byte < 10)
        *point = 0x30 + byte;
      else
        *point = 0x37 + byte;
      point++;
      guid++;
    }
  }

  *point = '-';
  point++;

  for (int i = 0; i < 6; i++)
  {
    unsigned char byte = *guid >> 4;
    if (byte < 10)
      *point = 0x30 + byte;
    else
      *point = 0x37 + byte;
    point++;

    byte = *guid & 0x0F;
    if (byte < 10)
      *point = 0x30 + byte;
    else
      *point = 0x37 + byte;
    point++;
    guid++;
  }

  *point = 0;

  return 37;
}
#endif

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++      ВЫЧИСЛЕНИЕ ЧИСЛА ДНЕЙ ОТ ЗАДАННОЙ ДАТЫ     ++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

unsigned int monthdays[12] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };

// --------------------------------------------------------
// Разделить дату на составляющие
// Возвращает год и устанавливает месяц и день
// --------------------------------------------------------
inline unsigned int PrepareDate(unsigned int date, unsigned int& month, unsigned int& day)
{
  unsigned int year     = date / 10000;
  unsigned int monthday = date - (year * 10000);
  month = monthday / 100;
  if (month > 12)
    month = 12;
  day = monthday % 100;
  if (day > 31)
    day = 31;
  return year;
}

// --------------------------------------------------------
// Определить сколько дней прошло от 1 января
// date - число вида YYYYMMDD
// --------------------------------------------------------
unsigned int _fastcall GetDayCountFromBegin(unsigned int date)
{
  unsigned int month;
  unsigned int count;
  unsigned int year = PrepareDate(date, month, count);

  unsigned int first = 1;

  while(month > first)
  {
    count += monthdays[first-1];
    first++;
  }

  if (month > 2)
  {
    // Високосный год?
    if ((year % 4) == 0)
      count++;
  }

  return count;
}

// --------------------------------------------------------
// Определить сколько дней в году (с 2001 года)
// --------------------------------------------------------
inline unsigned int _fastcall GetDayCountInYear(unsigned int year)
{
  // Високосный год (без учета столетий)?
  if ((year % 4) == 0)
    return 366;
  return 365;
}

// --------------------------------------------------------
// Определить сколько дней в месяце
// --------------------------------------------------------
inline unsigned int _fastcall GetDayCountInMonth(unsigned int month, unsigned int year)
{
  unsigned int count = monthdays[month-1];
  if ((month == 2) && ((year % 4) == 0))
    count++;
  return count;
}

// --------------------------------------------------------
// Определить сколько дней прошло от 1 января заданного года
// date - число вида YYYYMMDD
// --------------------------------------------------------
unsigned int _fastcall GetDayCountFromYear(unsigned int date, unsigned int year)
{
  unsigned int count = 0;
  unsigned int dateyear = date / 10000;

  while(dateyear > year)
  {
    count += GetDayCountInYear(year);
    year++;
  }

  if (dateyear == year)
    count += GetDayCountFromBegin(date);

  return count;
}

// --------------------------------------------------------
// Определить дату по числу дней от текущей даты (+ до, - после)
// --------------------------------------------------------
unsigned int _fastcall GetDateAfterDays(int after)
{
  // Запросить текущую дату
  unsigned int today;
  GetTheSystemDateAndTime(&today, 0);

  if (after == 0)
    return today;

  unsigned int month;
  unsigned int day;
  unsigned int year = PrepareDate(today, month, day);

  if (after < 0)
  {
    after = -after;

    while(after > 365)
    {
      if (month <= 2)
      {
        after -= GetDayCountInYear(year);
        year++;
      }
      else
      {
        year++;
        after -= GetDayCountInYear(year);
      }
    }

    unsigned int tail = GetDayCountInMonth(month, year);
    tail -= day;

    if (tail >= (unsigned)after)
    {
      // Прибавить несколько дней в пределах месяца
      today += after;
      return today;
    }

    after -= tail;
    day    = 1;

    while(after > 0)
    {
      month++;
      if (month > 12)
      {
        month = 1;
        year++;
      }

      unsigned int delta = monthdays[month-1];
      if ((month == 2) && ((year % 4) == 0))
        delta++;
      if (delta >= (unsigned)after)
      {
        // Несколько дней в пределах месяца
        day = after;
        break;
      }

      after -= delta;
    }
  }
  else
  {
    while(after > 365)
    {
      if (month > 2)
      {
        after -= GetDayCountInYear(year);
        year--;
      }
      else
      {
        year--;
        after -= GetDayCountInYear(year);
      }
    }

    while(after > 0)
    {
      if (day > (unsigned)after)
      {
        // Отнять несколько дней в пределах месяца
        day -= after;
        break;
      }

      after -= day;

      month--;
      if (month <= 0)
      {
        month = 12;
        year--;
      }

      day = GetDayCountInMonth(month, year);
    }
  }

  return (year * 10000 + month * 100 + day);
}

#ifdef BUILD_IML
 char * _fastcall imgDos2Win(char * text,  long int size);
 char * _fastcall imgKoi82Win(char * text, long int size);

 char * Dos2Win(char * text, int length)
 {
   return imgDos2Win(text, length);
 }

 char * Koi82Win(char * text, int length)
 {
   return imgKoi82Win(text, length);
 }

 #ifdef LINUXAPI
 char * _fastcall imgWin2Koi8(char * text, long int length);
 char * _fastcall imgText2Koi8(char * text, long int length);
 char * _fastcall imgDos2Koi8(char * text, long int length);

 char * Win2Koi8(char * text,int length)
 {
   return imgWin2Koi8(text, length);
 }

 char * Text2Koi8(char * text,int length)
 {
   return imgText2Koi8(text, length);
 }

 char * Dos2Koi8(char * text,int length)
 {
   return imgDos2Koi8(text, length);
 }
 #endif
#else
#ifndef HIDEMAPAPI
extern "C"
{
// Преобразование из DOS в WINDOWS и обратно (OEM to ANSI)
// text - адрес обрабатываемой строки,заканчивающейся нулем
// length - максимальное число обрабатываемых символов
// Возвращаемое значение равно значению text
_MAPIMP char * _MAPAPI Dos2Win(char * text,int length);
// Преобразование из KOI-8 в WIN
_MAPIMP char * _MAPAPI Koi82Win(char * text,int length);

// Преобразование из ANSI в KOI-8
_MAPIMP char * _MAPAPI Win2Koi8(char * text,int length);
// Преобразование из DOS-OEM в KOI-8 (0x80 - 0xFF)
_MAPIMP char * _MAPAPI Text2Koi8(char * text,int length);
// Преобразование из DOS-OEM в KOI-8
_MAPIMP char * _MAPAPI Dos2Koi8(char * text,int length);
};
#endif
#endif

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++      МЕТОДЫ ДОСТУПА К ЧИСЛАМ ТИПА СЕМАНТИКА     ++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// --------------------------------------------------------
//  Сравнить символьную строку и шаблон поиска
//  При совпадении строк возвращает CMEQUAL
// --------------------------------------------------------

unsigned char SymbolIndex[256] =      // БУКВЫ В ВЕРХНЕМ РЕГИСТРЕ
{
0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A,0x0B,0x0C,0x0D,0x0E,0x0F,
0x10,0x11,0x12,0x13,0x14,0x15,0x16,0x17,0x18,0x19,0x1A,0x1B,0x1C,0x1D,0x1E,0x1F,
0x20,0x21,0x22,0x23,0x24,0x25,0x26,0x27,0x28,0x29,0x2A,0x2B,0x2C,0x2D,0x2E,0x2F,
0x30,0x31,0x32,0x33,0x34,0x35,0x36,0x37,0x38,0x39,0x3A,0x3B,0x3C,0x3D,0x3E,0x3F,
0x40,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x4F,
0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x5B,0x5C,0x5D,0x5E,0x5F,
0x60,0x41,0x42,0x43,0x44,0x45,0x46,0x47,0x48,0x49,0x4A,0x4B,0x4C,0x4D,0x4E,0x6F,
0x50,0x51,0x52,0x53,0x54,0x55,0x56,0x57,0x58,0x59,0x5A,0x7B,0x7C,0x7D,0x7E,0x7F,
0x80,0x81,0x82,0x83,0x84,0x85,0x86,0x87,0x88,0x89,0x8A,0x8B,0x8C,0x8D,0x8E,0x8F,
0x90,0x91,0x92,0x93,0x94,0x95,0x96,0x97,0x98,0x99,0x9A,0x9B,0x9C,0x9D,0x9E,0x9F,
0xA0,0xA1,0xA2,0xA3,0xA4,0xA5,0xA6,0xA7,0xA8,0xA9,0xAA,0xAB,0xAC,0xAD,0xAE,0xAF,
0xB0,0xB1,0xB2,0xB3,0xB4,0xB5,0xB6,0xB7,0xB8,0xB9,0xBA,0xBB,0xBC,0xBD,0xBE,0xBF,
0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF,
0xC0,0xC1,0xC2,0xC3,0xC4,0xC5,0xC6,0xC7,0xC8,0xC9,0xCA,0xCB,0xCC,0xCD,0xCE,0xCF,
0xD0,0xD1,0xD2,0xD3,0xD4,0xD5,0xD6,0xD7,0xD8,0xD9,0xDA,0xDB,0xDC,0xDD,0xDE,0xDF
};

// -----------------------------------------------------------------
//  Сравнить символьную строку и шаблон
//  При совпадении строк возвращает CMEQUAL
//  При несовпадении строк возвращает CMNOTEQ, CMLESS, CMMORE
//  Например в шаблоне "XXXX*XX?*" используются символы :
//  Х - символ для сравнения,
//  ? - произвольный символ на фиксированном месте,
//  * - произвольное число произвольных символов
// -----------------------------------------------------------------

#ifdef BUILD_DLL
extern "C"
{
_MAPIMP long int _MAPAPI mapCompareTemplateString(const unsigned char * value, const unsigned char * temp)
#else
long int mapCompareTemplateString(const unsigned char* value, const unsigned char* temp)
#endif
{
  int vcount;                              // Шаблон c управляющими символами
  int tcount;
  int i = -1, ii, j, jj, ivbegin, itbegin; // счетчики
  int vbegin = 0, tbegin;
  int picture = 0,                         // наличие шаблона '*'
    count = 0,                           // количество совпадений шаблона '*'
    error = 0, errstr = 0;               // несовпадения шаблона '*'

  if ((value && temp) == 0)
    return CMBAD;

  vcount = (int)strlen((const char*)value);
  tcount = (int)strlen((const char*)temp);

  // Уберем хвостовые пробелы
  while (vcount > 0)
  {
    if (value[vcount - 1] != ' ')
      break;
    vcount--;
  }

  while (tcount > 0)
  {
    if (temp[tcount - 1] != ' ')
      break;
    tcount--;
  }

  // Символ '%' обрабатываем как '*'
  if ((temp[0] == '%') || (temp[0] == '*'))
  {
    if (tcount <= 1)
    {
      // Допустимо любое значение
      return CMEQUAL;
    }

    int issubstr = 1;
    char csymbol = temp[0];
    for (int k = 1; k < tcount; k++)
    {
      if (temp[k] == csymbol)
      {
        issubstr = 0;
        break;
      }
    }

    // Ищем вхождение подстроки ?
    if (issubstr != 0)
    {
      // Должно совпадать окончание строки

      // Более короткая строка ?
      if (vcount < (tcount - 1))
        return CMLESS;

      int jt = 1;
      int jv;
      for (jv = vcount - (tcount - 1); jv < vcount; jv++)
      {
        if (SymbolIndex[*(value + jv)] < SymbolIndex[*(temp + jt)])
          return CMLESS;
        if (SymbolIndex[*(value + jv)] > SymbolIndex[*(temp + jt)])
          return CMMORE;
        jt++;
      }

      return CMEQUAL;
    }

    // Подстрока может быть в любом месте

    // Более короткая строка ?
    if (vcount < (tcount - 3))
      return CMLESS;

    int vlimit = vcount - (tcount - 2);
    for (int jv = 0; jv <= vlimit; jv++)
    {
      int jvs = jv;
      int tlimit = tcount - 2;
      int jt;
      for (jt = 1; jt <= tlimit; jt++)
      {
        if (SymbolIndex[*(value + jvs)] != SymbolIndex[*(temp + jt)])
          break;
        jvs++;
      }

      if (jt > tlimit)
        return CMEQUAL;

    }

    return CMNOTEQ;
  }
  else
    // Ищем совпадение подстроки в начале?
    if ((tcount > 2) && (temp[tcount - 1] == '%') && (temp[tcount - 2] == '%'))
    {
      int limit = tcount - 2;

      // Более короткая строка ?
      if (vcount < limit)
        return CMLESS;

      for (int j = 0; j < limit; j++)
      {
        if (SymbolIndex[*(value + j)] < SymbolIndex[*(temp + j)])
          return CMLESS;
        if (SymbolIndex[*(value + j)] > SymbolIndex[*(temp + j)])
          return CMMORE;
      }

      return CMEQUAL;
    }

  while (i++ < tcount)
  {
    if (*(temp + i) == '*')
    {
      picture++;
      continue;
    }
    if (picture) // ищем следующий символ за '*'
    {
      tbegin = i;
      for (ii = tbegin; ii < tcount; ii++)
      {
        // Проскочить '?', если он первый
        if (*(temp + ii) == '?' && ii == tbegin)
        {
          tbegin++;
          continue;
        }
        if (*(temp + ii) == '*')
        {
          if (ii - tbegin > 0)
            break;
          tbegin = ii + 1;
        }
      }

      if (tbegin == tcount)
        return CMEQUAL;

      // Ищем в строке первый символ шаблона '*'
      // С tbegin до ii-1
      for (j = vbegin; j < vcount; j++)
      {
        if (SymbolIndex[*(value + j)] != SymbolIndex[*(temp + tbegin)])
          continue;
        else  // нашли, сравниваем остальные символы
        {
          errstr = 0;
          ivbegin = j;
          for (jj = tbegin; jj < ii && ivbegin < vcount; jj++)
          {
            // необязательные символы проверке не подлежат
            if (*(temp + jj) != '?')
            {
              if (SymbolIndex[*(value + ivbegin)] != SymbolIndex[*(temp + jj)])
              {
                error = 1;
                errstr = 1;
                break;
              }
            }
            ivbegin++;
          }

          // если строка кончилась, а шаблон нет
          if (ivbegin == vcount && jj < ii)
            return CMNOTEQ;

          // не нашли строку, надо искать дальше
          if (errstr)
            continue;

          // если шаблон кончился,
          if (jj == ii)
          {
            // если строка кончилась
            if (ivbegin == vcount)
            {
              for (itbegin = jj; itbegin < tcount; itbegin++)
                if (*(temp + itbegin) != '*') return CMNOTEQ;
              return CMEQUAL;
            }

            // если строка не кончилась  и нет ошибок
            if (ivbegin < vcount)
            {
              for (itbegin = jj; itbegin < tcount; itbegin++)
                if (*(temp + itbegin) != '*' && *(temp + itbegin) != '?')
                  goto newpict;
              return CMEQUAL;
            }
          }

        newpict:
          // нашли строку
          vbegin = ivbegin;
          i = ii - 1;
          count++;
          break;
        }
      }

      if ((error && errstr) || (j == vcount))
        return CMNOTEQ;
    }
    else
    {
      // необязательные символы проверке не подлежат
      if (*(temp + i) != '?')
      {
        if (i > vcount)
          return CMNOTEQ; // шаблон больше строки
        if (SymbolIndex[*(value + i)] < SymbolIndex[*(temp + i)])
          return CMLESS;
        if (SymbolIndex[*(value + i)] > SymbolIndex[*(temp + i)])
          return CMMORE;
      }
    }
  }

  if (count && picture == count)
    return CMEQUAL;
  else
  {
    if (i - 1 == vcount) // шаблон равен строке
    {
      if (picture == 0)
        return CMEQUAL;
    }
  }

  return CMNOTEQ;
}

#ifdef BUILD_DLL
}
#endif

#ifdef RISCCPU
  #define GetAlignedWCHAR(curval) (*((char *)(curval)) + ((*(((char *)(curval))+1)) << 8))
  #define CompareWithSpecialChar(symbol, spchar) (*((char *)(symbol)) == spchar && *(((char *)(symbol))+1) == 0)
#else
  #define GetAlignedWCHAR(curval) (*(curval))
  #define CompareWithSpecialChar(symbol, spchar) (*(symbol) == spchar)
#endif

#ifdef BUILD_DLL

#define CompareStringUn mapCompareStringUn

extern "C"
{
_MAPIMP long int _MAPAPI mapCompareStringUn(const WCHAR * value, const WCHAR * temp)
#else
long int CompareStringUn(const WCHAR * value, const WCHAR * temp)
#endif
{
  int i = -1, ii, j, jj, ivbegin, itbegin; // счетчики
  int vbegin = 0, tbegin;
  int picture = 0,                         // наличие шаблона '*'
      count = 0,                           // количество совпадений шаблона '*'
      error = 0, errstr = 0;               // несовпадения шаблона '*'

  if ((value && temp) == 0)
    return CMBAD;

  int vcount = (int)Wcslen(value);
  int tcount = (int)Wcslen(temp);

  // Уберем хвостовые пробелы
  while(vcount > 0)
  {
    if (!CompareWithSpecialChar(value+vcount-1, 0x20))            // ' '
      break;
    vcount--;
  }

  while(tcount > 0)
  {
    if (!CompareWithSpecialChar(temp+tcount-1, 0x20))             // ' '
      break;
    tcount--;
  }

  while (i++ < tcount)
  {
    if (CompareWithSpecialChar(temp + i, '*'))
    {
      picture++;
      continue;
    }

    if (picture) // ищем следующий символ за '*'
    {
      tbegin = i;
      for (ii = tbegin; ii < tcount; ii++)
      {
        // Проскочить '?', если он первый
        if (CompareWithSpecialChar(temp + ii, '?') && ii == tbegin)
        {
          tbegin++;
          continue;
        }
        if (CompareWithSpecialChar(temp + ii, '*'))
        {
          if (ii - tbegin > 0)
            break;
          tbegin = ii + 1;
        }
      }

      if (tbegin == tcount)
        return CMEQUAL;

      WCHAR symbol1 = 0;
      WCHAR symbol2 = 0;

      // Ищем в строке первый символ шаблона '*'
      // С tbegin до ii-1
      for (j = vbegin; j < vcount; j++)
      {
        symbol1 = GetAlignedWCHAR(value + j);
        symbol2 = GetAlignedWCHAR(temp + tbegin);

        if (ToUpperCase(symbol1) != ToUpperCase(symbol2))
          continue;
        else  // нашли, сравниваем остальные символы
        {
          errstr = 0;
          ivbegin = j;
          for (jj = tbegin; jj < ii && ivbegin < vcount; jj++)
          {
            // необязательные символы проверке не подлежат
            if (!CompareWithSpecialChar(temp + jj, '?'))
            {
              symbol1 = GetAlignedWCHAR(value + ivbegin);
              symbol2 = GetAlignedWCHAR(temp + jj);
              if (ToUpperCase(symbol1) != ToUpperCase(symbol2))
              {
                error = 1;
                errstr = 1;
                break;
              }
            }
            ivbegin++;
          }

          // если строка кончилась, а шаблон нет
          if (ivbegin == vcount && jj < ii)
            return CMNOTEQ;

          // не нашли строку, надо искать дальше
          if (errstr)
            continue;

          // если шаблон кончился,
          if (jj == ii)
          {
            // если строка кончилась
            if (ivbegin == vcount)
            {
              for (itbegin = jj; itbegin < tcount; itbegin++)
                if (!CompareWithSpecialChar(temp + itbegin, '*'))
                  return CMNOTEQ;
              return CMEQUAL;
            }

            // если строка не кончилась  и нет ошибок
            if (ivbegin < vcount)
            {
              for (itbegin = jj; itbegin < tcount; itbegin++)
                if (!CompareWithSpecialChar(temp + itbegin, '*') && !CompareWithSpecialChar(temp + itbegin, '?'))
                  goto newpict;
              return CMEQUAL;
            }
          }

        newpict:
          // нашли строку
          vbegin = ivbegin;
          i = ii - 1;
          count++;
          break;
        }
      }

      if ((error && errstr) || (j == vcount))
        return CMNOTEQ;
    }
    else
    {
      // необязательные символы проверке не подлежат
      if (!CompareWithSpecialChar(temp + i, '?'))
      {
        if (i > vcount)
          return CMNOTEQ; // шаблон больше строки

        WCHAR symbol1 = GetAlignedWCHAR(value + i);
        WCHAR symbol2 = GetAlignedWCHAR(temp + i);

        int ret = ToUpperCase(symbol1) - ToUpperCase(symbol2);
        if (ret < 0)
          return CMLESS;
        if (ret > 0)
          return CMMORE;
      }
    }
  }

  if (count && (picture == count))
    return CMEQUAL;
  else
  {
    if ((i - 1) == vcount) // шаблон равен строке
      if (picture == 0)
        return CMEQUAL;
  }

  return CMNOTEQ;
}

#ifdef BUILD_DLL
}
#endif

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++ МЕТОДЫ ОБРАБОТКИ НАБОРА СЕМАНТИЧЕСКИХ ХАРАКТЕРИСТИК ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

#define sbsminilength 4 // (sizeof(sembase->String.Type) +
                        // sizeof(sembase->String.Scale)+
                        // sizeof(worksemantic->Block->SemCode))

// -----------------------------------------------------------------
// Проверить корректность числа с плавающей точкой
// -----------------------------------------------------------------
int TSemBase::CheckDoubleValue()
{
  if (Type() == SEMBASEDOUBLE)
  {
    char * point = (char *)&Sembase.Double.Value;
#ifndef NUMBERTURN
    char value = (*(point + 7))&(0x7F);
#else
    char value = (*point)&(0x7F);
#endif
    if ((value != 0) && ((value < 0x30) || (value > 0x50)))
      return 0;
  }

  return 1;
}

// -----------------------------------------------------------------
// Запросить длину значения характеристики
// -----------------------------------------------------------------
unsigned int TSemBase::DataLength()
{
  if ((Sembase.String.Type == SEMBASESTRING) || (Sembase.String.Type == SEMBASESTRDOS))
    return (unsigned int)(strlen(Sembase.String.Value) + 1);
  else
    if (Sembase.String.Type == SEMBASESTRUNI)
      return (unsigned int)((Wcslen(Sembase.WString.Value) + 1) * sizeof(WCHAR2));
    else
      if (Sembase.String.Type == SEMBASEBIGUNI)
      {
#ifdef RISCCPU
        REGISTER value;
        LongToRegister(value, Sembase.BigString.Size);
        return value.Long;
#else
        return Sembase.BigString.Size;
#endif
      }
      else
        return Sembase.String.Type;
}

// -----------------------------------------------------------------
// Запросить длину значения характеристики со служебными полями
// -----------------------------------------------------------------
unsigned int TSemBase::Length()
{
  if ((Sembase.String.Type == SEMBASESTRING) || (Sembase.String.Type == SEMBASESTRDOS))
  {
    return (Sembase.String.Scale + sizeof(Sembase.String.Type) + sizeof(Sembase.String.Scale) + 1);
  }
  else
    if (Sembase.String.Type == SEMBASESTRUNI)
      return (Sembase.String.Scale + sizeof(Sembase.String.Type) + sizeof(Sembase.String.Scale) + sizeof(WCHAR2));
    else
      if (Sembase.String.Type == SEMBASEBIGUNI)
      {
#ifdef RISCCPU
        REGISTER value;
        LongToRegister(value, Sembase.BigString.Size);
        return value.Long + 2 + 4;
#else
        return (Sembase.BigString.Size + 2 + 4); // Type,Scale,Size
#endif
      }
      else
        return (Sembase.String.Type + sizeof(Sembase.String.Type) + sizeof(Sembase.String.Scale));
}

// -----------------------------------------------------------------
// Cравнить текущую характеристику с заданной
// isstr = -1 - ищем подстроку
// isstr =  1 - ищем по сравнению строк
// isstr =  2 - ищем по спец.символам
// -----------------------------------------------------------------
//Volkov I.A. BEGIN
//#ifndef HIDEBMX
//HOW _fastcall TSemBase::Compare(TSemBase * base, int isstr, TSearchBMX* bmxSearch)
//#else
//HOW _fastcall TSemBase::Compare(TSemBase* base, int isstr)
//#endif
//{
//  if (base == 0)
//    return CMBAD;

//  // Если оба числа типа семантика символьные сравниваем как строки
//  if ((Sembase.String.Type == SEMBASESTRUNI) || (Sembase.String.Type == SEMBASEBIGUNI))
//  {
//    WCHAR2* wstr;
//    if (Sembase.String.Type == SEMBASESTRUNI)
//      wstr = Sembase.WString.Value;
//    else
//      wstr = Sembase.BigString.Value;

//#ifndef HIDEBMX
//    if (bmxSearch != 0)
//    {
//      if (bmxSearch->SearchSubstring(wstr) != 0)
//        return CMEQUAL;
//      return CMNOTEQ;
//    }
//#endif
//    if (isstr || (base->Sembase.String.Type == SEMBASESTRUNI) || (base->Sembase.String.Type == SEMBASEBIGUNI) ||
//        (base->Sembase.String.Type == SEMBASESTRING))
//    {
//      WCHAR strvalue[260]; strvalue[0] = 0;

//      WCHAR2* wstr2;
//      if (base->Sembase.String.Type == SEMBASESTRUNI)
//        wstr2 = base->Sembase.WString.Value;
//      else
//        if (base->Sembase.String.Type == SEMBASEBIGUNI)
//          wstr2 = base->Sembase.BigString.Value;
//        else
//        {
//          // Семантика типа строка, переведем значения в строки
//          if (base->StringUnicode(strvalue, sizeof(strvalue)) != 0)
//            wstr2 = strvalue;
//          else
//            return CMNOTEQ;
//        }

//      return CompareStringValueUn((WCHAR*)wstr, wstr2, isstr);
//    }
//  }
//  else
//    if ((Sembase.String.Type == SEMBASESTRING) || (Sembase.String.Type == SEMBASESTRDOS))
//    {
//      if ((base->Sembase.String.Type == SEMBASESTRING) || (base->Sembase.String.Type == SEMBASESTRDOS))
//      {
//        return CompareStringValue(StringInAnsi(), base->StringInAnsi(), isstr);
//      }
//      else
//        if ((base->Sembase.String.Type == SEMBASESTRUNI) || (base->Sembase.String.Type == SEMBASEBIGUNI))
//        {
//          WCHAR temp[260]; temp[0] = 0;
//          AnsiStringToUnicode((const char*)StringInAnsi(), temp, sizeof(temp));

//#ifndef HIDEBMX
//          // Ищем подстроку
//          if (bmxSearch != 0)
//          {
//            if (bmxSearch->SearchSubstring(temp) != 0)
//              return CMEQUAL;
//            return CMNOTEQ;
//          }
//#endif
//          WCHAR2* wstr;
//          if (base->Sembase.String.Type == SEMBASESTRUNI)
//            wstr = base->Sembase.WString.Value;
//          else
//            wstr = base->Sembase.BigString.Value;

//          return CompareStringValueUn(temp, wstr, isstr);
//        }

//      // Семантика типа строка, переведем значения в строки
//      if (isstr)
//      {
//        char strvalue[260]; strvalue[0] = 0;

//        if (base->StringValue(strvalue, sizeof(strvalue)) != 0)
//          return CompareStringValue(StringInAnsi(), strvalue, isstr);

//        return CMNOTEQ;
//      }
//    }
//    else
//      if ((base->Sembase.String.Type == SEMBASELONG4) || (Sembase.String.Type == SEMBASELONG4))
//      {
//        INT64TWO quid1;
//        INT64TWO quid2;
//        if (QuidValue(&quid1) == 0)
//          return CMNOTEQ;
//        if (base->QuidValue(&quid2) == 0)
//          return CMNOTEQ;
//        if (quid1 == quid2)
//          return CMEQUAL;
//        return CMNOTEQ;
//      }
//      else
//      {
//        // Текущая характеристика не строковая
//        if ((base->Sembase.String.Type == SEMBASESTRING) || (base->Sembase.String.Type == SEMBASESTRDOS))
//        {
//          const char* temp = base->StringInAnsi();

//          // Семантика типа строка, переведем значения в строки
//          if (isstr)
//          {
//            char strvalue[260]; strvalue[0] = 0;

//            if (StringValue(strvalue, sizeof(strvalue)) != 0)
//              return CompareStringValue(strvalue, temp, isstr);
//            return CMNOTEQ;
//          }
//        }
//        else
//        {
//          if ((base->Sembase.String.Type == SEMBASESTRUNI) || (base->Sembase.String.Type == SEMBASEBIGUNI))
//          {
//            WCHAR2* wstr;
//            if (base->Sembase.String.Type == SEMBASESTRUNI)
//              wstr = base->Sembase.WString.Value;
//            else
//              wstr = base->Sembase.BigString.Value;

//            WCHAR strvalue[260]; strvalue[0] = 0;

//            // Семантика типа строка, переведем значения в строки
//            if (isstr)
//            {
//              if (StringUnicode(strvalue, sizeof(strvalue)) != 0)
//                return CompareStringValueUn(strvalue, (WCHAR*)wstr, isstr);
//              return CMNOTEQ;
//            }
//          }
//        }
//      }

//  // не символьный вид числа типа семантика
//  double result = DoubleValue() - base->DoubleValue();

//  if (result < -0.00000001)
//    return CMLESS;

//  if (result > 0.00000001)
//    return CMMORE;

//  return CMEQUAL;
//}
//Volkov I.A. END

// -----------------------------------------------------------------
// Cравнить текущую характеристику со строкой
// -----------------------------------------------------------------
//Volkov I.A. BEGIN
//HOW _fastcall TSemBase::Compare(const WCHAR* value)
//{
//  WCHAR strvalue[260]; strvalue[0] = 0;

//  WCHAR2* wstr;
//  if (Sembase.String.Type == SEMBASESTRUNI)
//    wstr = Sembase.WString.Value;
//  else
//    if (Sembase.String.Type == SEMBASEBIGUNI)
//      wstr = Sembase.BigString.Value;
//    else
//    {
//      // Семантика типа строка, переведем значения в строки
//      if (StringUnicode(strvalue, sizeof(strvalue)) != 0)
//        wstr = strvalue;
//      else
//        return CMNOTEQ;
//    }

//  return CompareStringValueUn(wstr, value, 1);
//}
//Volkov I.A. END

// -----------------------------------------------------------------
// Cравнить две строки
// isstr = -1 - ищем подстроку
// isstr =  2 - ищем по спец.символам
// isstr =  * - ищем по сравнению строк
// При совпадении возвращает CMEQUAL
// -----------------------------------------------------------------
HOW TSemBase::CompareStringValue(const char * valu1, const char * valu2, int isstr)
{
  if (isstr == -1)
  {
    // Ищем подстроку
    if (strstr(valu1, valu2) != 0)
      return CMEQUAL;
    return CMNOTEQ;
  }

  if (isstr != 2)
  {
    if (_stricmp(valu1, valu2) == 0)
      return CMEQUAL;
    return CMNOTEQ;
  }

  return (HOW)mapCompareTemplateString((const unsigned char *)valu1, (const unsigned char *)valu2);
}

// -----------------------------------------------------------------
// Cравнить две строки
// При совпадении возвращает CMEQUAL
// -----------------------------------------------------------------
// Volkov I.A. BEGIN
//HOW TSemBase::CompareStringValueUn(const WCHAR * valu1, const WCHAR * valu2, int isstr)
//{
//  if (isstr == -1)
//  {
//    // Ищем подстроку
//    if (Wcsstr(valu1, valu2) != 0)
//      return CMEQUAL;
//    return CMNOTEQ;
//  }

//  if (isstr != 2)
//  {
//    if (WcsiCmp(valu1, valu2) == 0)
//      return CMEQUAL;
//    return CMNOTEQ;
//  }

//  return (HOW)CompareStringUn(valu1, valu2);
//}
// Volkov I.A. END

// -----------------------------------------------------------------
// Сформировать GUID в виде строки шестнадцатеричных цифр
// -----------------------------------------------------------------
int TSemBase::CreateGUID()
{
  Clear();

/* Volkov I.A. BEGIN */
//#ifndef BUILD_IML
//  if (::CreateTheGUID(&Sembase.Long4.Quad) == 0)
//    return 0;
//#endif
/* Volkov I.A. BEGIN */

  Type(SEMBASELONG4);
  return 1;
}

// -----------------------------------------------------------------
//  Запрос значения характеристики в символьном виде
// -----------------------------------------------------------------
int _fastcall TSemBase::StringValue(char* place, int maxsize)
{
  if ((place == 0) || (maxsize == 0))
    return 0;

  if (Sembase.String.Type == SEMBASESTRING)
  {
    StrCopy(place, &Sembase.String.Value[0], maxsize);
#ifdef LINUXAPI
    Win2Koi8(place, Sembase.String.Scale);
#endif
    return maxsize;
  }

  if (Sembase.String.Type == SEMBASESTRDOS)
  {
#ifdef WIN32API
    Dos2Win(Sembase.String.Value, Sembase.String.Scale);
    Sembase.Char.Type = SEMBASESTRING;
    StrCopy(place, &Sembase.String.Value[0], maxsize);
#else
    StrCopy(place, &Sembase.String.Value[0], maxsize);
    Text2Koi8(place, Sembase.String.Scale);
#endif
    return maxsize;
  }

  if (Sembase.String.Type == SEMBASESTRUNI)
  {
    place[0] = 0;
    UnicodeToString((WCHAR2*)&Sembase.String.Value[0], place, maxsize);
    return maxsize;
  }

  if (Sembase.String.Type == SEMBASEBIGUNI)
  {
    place[0] = 0;
    UnicodeToString((WCHAR2*)&Sembase.BigString.Value[0], place, maxsize);
    return maxsize;
  }

  if (Sembase.String.Type == SEMBASELONG4)
  {
    // Отобразить как строку из 32 шестнадцатеричных символов и 4 символа '-'
    if (maxsize <= 36) return 0;

    // Преобразовать в строку шестнадцатеричных цифр
    unsigned char* point = (unsigned char*)&Sembase.Long4.Quad;
    return GUIDToString(point, place);
  }

  char str[256];

  if ((Sembase.String.Type == SEMBASEDOUBLE) || (Sembase.Char.Scale != 0))
  {
    DOUBLEREGISTER value;

    // Определим формат представления
    if (Sembase.String.Type == SEMBASEDOUBLE)
    {
#ifndef NUMBERTURN
      char* point = Sembase.String.Value;
      char test = (*(point + 7)) & (0x7F);
      if ((test != 0) && ((test < 0x30) || (test > 0x50)))
        value.Double = 0;
      else
#endif
        value.GetDouble(Sembase.String.Value);
    }
    else
      value.Double = DoubleValue();

    if ((value.Double > -100000000.0) && (value.Double < 4000000000.0))
    {
      Snprintf(str, sizeof(str), "%.9lf", value.Double);

      // Усечение незначащих нулей в строке
      int i = (int)strlen(str);
      int j = i - 1;
      char* zeropos = (char*)(str + j);
      char* point = strstr(str, ".");

      if ((point != 0) && (point < zeropos))
      {
        while ((i-- > 0) && (*zeropos == '0'))
        {
          *zeropos = 0;
          zeropos--;
          if ((i > 0) && (*zeropos == '.'))
          {
            *zeropos = 0;
            break;
          }
        }
      }
    }
    else
    {
      Snprintf(str, sizeof(str), "%lE", value.Double);
    }
  }
  else
    IntToStr((int)LongValue(), str, sizeof(str));

  StrCopy(place, str, maxsize);
  return maxsize;
}

// -----------------------------------------------------------------
//  Запрос значения характеристики в символьном виде
// -----------------------------------------------------------------
int _fastcall TSemBase::StringUnicode(WCHAR* place, int maxsize)
{
  if ((place == 0) || (maxsize == 0))
    return 0;

  if (Sembase.String.Type == SEMBASESTRUNI)
  {
    WcsCopy(place, &Sembase.WString.Value[0], maxsize);
    return 1;
  }

  if (Sembase.String.Type == SEMBASEBIGUNI)
  {
    WcsCopy(place, &Sembase.BigString.Value[0], maxsize);
    return 1;
  }

  if (Sembase.String.Type == SEMBASESTRING)
    return AnsiStringToUnicode(&Sembase.String.Value[0], place, maxsize);

  if (Sembase.String.Type == SEMBASELONG4)
  {
    // Отобразить как строку из 32 шестнадцатеричных символов и 4 символа '-'
    if (maxsize <= 36)
      return 0;

    // Преобразовать в строку шестнадцатеричных цифр
    unsigned char* point = (unsigned char*)&Sembase.Long4.Quad;

    char aplace[48] = { 0 };
    GUIDToString(point, aplace);
    return (StringToUnicode(aplace, place, maxsize) != 0);
  }

  char text[260];
  if (StringValue(text, sizeof(text)) != 0)
    return StringToUnicode(text, place, maxsize);

  return 0;
}

// -----------------------------------------------------------------
//  Запрос значения характеристики в символьном виде
// -----------------------------------------------------------------
int _fastcall TSemBase::StringUnicode8(char* place, int maxsize)
{
  if ((place && maxsize) == 0)
    return 0;

  if (Sembase.String.Type == SEMBASESTRUNI)
  {
    UnicodeToUnicode8(&Sembase.WString.Value[0], place, maxsize);
    return 1;
  }

  if (Sembase.String.Type == SEMBASEBIGUNI)
  {
    const WCHAR2* point = &Sembase.BigString.Value[0];
#ifndef RISCCPU
    int count = Sembase.BigString.Size / sizeof(WCHAR2);
#else
    REGISTER reg;
    LongToRegister(reg, Sembase.BigString.Size);
    int count = reg.Long / sizeof(WCHAR2);
#endif
    int ret = UnicodeToUnicode8(point, count, place, maxsize);
    if (ret && (ret < maxsize)) place[ret] = 0;
    return ret;
  }

  char text[260];
  if (StringValue(text, sizeof(text) - 1) != 0)
  {
    text[sizeof(text) - 1] = 0;
    return StringToUnicode8(text, place, maxsize);
  }

  return 0;
}

// -----------------------------------------------------------------
//  Перекодировать строку символов из ASCII (DOS) в ANSI
//  Выполняется автоматически при обращении к строке
//  Если значение характеристики не символьное - возвращает ноль
// -----------------------------------------------------------------
char* TSemBase::StringInAnsi()
{
  if (Sembase.String.Type == SEMBASESTRDOS)
  {
    Sembase.Char.Type = SEMBASESTRING;
    return Dos2Win(Sembase.String.Value, Sembase.String.Scale);
  }
  else
    if (Sembase.String.Type == SEMBASESTRING)  // Строка уже в ANSI
      return Sembase.String.Value;

  return 0;
}

// -----------------------------------------------------------------
// Запросить указатель на строку в кодировке UTF-16
// Если значение характеристики другого вида - возвращает ноль
// -----------------------------------------------------------------
const WCHAR * TSemBase::StringInUTF16()
{
  if (Sembase.String.Type == SEMBASESTRUNI)
    return Sembase.WString.Value;

  if (Sembase.String.Type == SEMBASEBIGUNI)
    return Sembase.BigString.Value;

  return 0;
}

// -----------------------------------------------------------------
// Запросить значение семантики типа "набор чисел"
// Если тип числа не SEMBASELONG4 и не строка - возвращает ноль
// -----------------------------------------------------------------
int _fastcall TSemBase::QuidValue(INT64TWO *quid)
{
  if (quid == 0)
    return 0;

  if (Sembase.String.Type == SEMBASELONG4)
  {
    memcpy((char*)quid, Sembase.String.Value, sizeof(INTQUAD));
    return 1;
  }

  if (Sembase.String.Type == SEMBASESTRUNI)         // 02/06/21
  {
    char charValue[256];
    UnicodeToString(Sembase.WString.Value, charValue, sizeof(charValue));
    return StringToGUID(charValue, (char *)quid);
  }

  return StringToGUID(Sembase.String.Value, (char *)quid);
}

// -----------------------------------------------------------------
// Запрос значения характеристики в числовом виде
// -----------------------------------------------------------------
int TSemBase::LongValue()
{
  if (Sembase.String.Type == SEMBASECHAR)
  {
    if (Sembase.Char.Scale == 0)
    {
      return(Sembase.Char.Value);
    }
    else
    {
      double dvalue = (pow(10.0, (int)Sembase.Char.Scale) * Sembase.Char.Value);
      return CHECK_INT(dvalue);
    }
  }

  if (Sembase.String.Type == SEMBASESHORT)
  {
    if (Sembase.Char.Scale == 0)
    {
#ifdef RISCCPU
      REGISTER value;
      signed short shortval;

      ShortToRegister(value, &(Sembase.Short.Value));
      shortval = value.Long;
      return ((int)shortval);
#else
      return(Sembase.Short.Value);
#endif
    }
    else
    {
#ifdef RISCCPU
      REGISTER value;
      signed short shortval;

      ShortToRegister(value, &(Sembase.Short.Value));
      shortval = value.Long;
      double dvalue = (pow(10, Sembase.Char.Scale) * shortval);
#else
      double dvalue = (pow(10.0, (int)Sembase.Char.Scale) * Sembase.Short.Value);
#endif
      return CHECK_INT(dvalue);
    }
  }

  if (Sembase.String.Type == SEMBASELONG)
  {
    REGISTER value;
#ifdef RISCCPU
    LongToRegister(value, Sembase.String.Value);
#else
    value.GetLong(Sembase.String.Value);
#endif
    if (Sembase.Char.Scale == 0)
    {
      return ((int)value.Long);
    }
    else
    {
      double dvalue = (pow(10.0, (int)Sembase.Char.Scale) * ((int)value.Long));
      return CHECK_INT(dvalue);
    }
  }

  if (Sembase.String.Type == SEMBASEDOUBLE)
  {
    double dvalue;

#ifndef NUMBERTURN
    char* point = Sembase.String.Value;
    char test = (*(point + 7)) & (0x7F);
    if ((test != 0) && ((test < 0x30) || (test > 0x50)))
      dvalue = 0;
    else
#endif
    {
      DOUBLEREGISTER value;
#ifdef RISCCPU
      DoubleToRegister(value, Sembase.String.Value);
      dvalue = value.Double;
#else
      dvalue = value.GetDouble(Sembase.String.Value);
#endif
    }

    return CHECK_INT(dvalue);
  }

  if ((Sembase.String.Type == SEMBASESTRING) || (Sembase.String.Type == SEMBASESTRDOS))
    return(atoi(&Sembase.String.Value[0]));

  if (Sembase.String.Type == SEMBASESTRUNI)
    return Wtoi(&Sembase.WString.Value[0]);

  if (Sembase.String.Type == SEMBASEBIGUNI)
    return Wtoi(&Sembase.BigString.Value[0]);

  return((int)-111111L);  // Тип не определен
}

// -----------------------------------------------------------------
// Запрос значения характеристики в числовом виде
// -----------------------------------------------------------------
double TSemBase::DoubleValue()
{
  if (Sembase.String.Type == SEMBASECHAR)
  {
    if (Sembase.Char.Scale == 0)
      return(Sembase.Char.Value);
    else
      return(pow(10.0, (int)Sembase.Char.Scale) * Sembase.Char.Value);
  }

  if (Sembase.String.Type == SEMBASESHORT)
  {
    if (Sembase.Char.Scale == 0)
    {
#ifdef RISCCPU
      REGISTER value;
      signed short shortval;

      ShortToRegister(value, &(Sembase.Short.Value));
      shortval = value.Long;
      return ((double)shortval);
#else
      return(Sembase.Short.Value);
#endif
    }
    else
    {
#ifdef RISCCPU
      REGISTER value;
      signed short shortval;

      ShortToRegister(value, &(Sembase.Short.Value));
      shortval = value.Long;
      return(pow(10, Sembase.Char.Scale) * shortval);
#else
      return(pow(10.0, (int)Sembase.Char.Scale) * Sembase.Short.Value);
#endif
    }
  }

  if (Sembase.String.Type == SEMBASELONG)
  {
    REGISTER value;
#ifdef RISCCPU
    LongToRegister(value, Sembase.String.Value);
#else
    value.GetLong(Sembase.String.Value);
#endif
    if (Sembase.Char.Scale == 0)
      return (double)((int)value.Long);
    else
      return (double)(pow(10.0, (int)Sembase.Char.Scale) * ((int)value.Long));
  }

  if (Sembase.String.Type == SEMBASEDOUBLE)
  {
    DOUBLEREGISTER value;
#ifndef NUMBERTURN
    char* point = Sembase.String.Value;
    char test = (*(point + 7)) & (0x7F);
    if ((test != 0) && ((test < 0x30) || (test > 0x50)))
      value.Double = 0;
    else
#endif
#ifdef RISCCPU
      DoubleToRegister(value, Sembase.String.Value);
#else
      value.GetDouble(Sembase.String.Value);
#endif
    return value.Double;
  }

  if ((Sembase.String.Type == SEMBASESTRING) || (Sembase.String.Type == SEMBASESTRDOS))
    return atof(&Sembase.String.Value[0]);

  if (Sembase.String.Type == SEMBASESTRUNI)
    return Wtof(&Sembase.WString.Value[0]);

  if (Sembase.String.Type == SEMBASEBIGUNI)
    return Wtof(&Sembase.BigString.Value[0]);

  return 0;  // Тип не определен
}

// -----------------------------------------------------------------
// Запросить значение характеристики в виде даты ("DD/MM/YYYY")
// -----------------------------------------------------------------
int _fastcall TSemBase::DateValue(char * place, int maxsize)
{
  int value = LongValue();

  int y  = value/10000;
  int ms = value - (y * 10000);
  int m  = ms/100;
  int d  = ms - (m * 100);

  char text[16];
  Snprintf(text, sizeof(text), "%02i/%02i/%04i", d, m, y);

  return (StrCopy(place, text, maxsize) != 0);
}

// -----------------------------------------------------------------
//  Установить значение строки WCHAR
// -----------------------------------------------------------------
int _fastcall TSemBase::Value(const WCHAR * value)
{
  Type(SEMBASESTRUNI);

  WcsCopy(Sembase.WString.Value, value, sizeof(Sembase.WString.Value));

  // Заменить двоичные коды на пробел
  unsigned short int * test = (unsigned short int*)Sembase.WString.Value;
  while(*test != 0)
  {
    if (*test <= 0x09)    // '\t'
      *test = 0x20;       // ' '
    test++;
  }

  // Установить размер
  int length = (int)Wcslen(Sembase.WString.Value) * sizeof(WCHAR2);

  Sembase.WString.Scale = (unsigned char)length;
  return 1;
}

// -----------------------------------------------------------------
// Установить значение строки WCHAR в заранее выделенную область памяти
// size - длина строки в байтах вместе с замыкающим нулевым символом
// При ошибке возвращает ноль
// -----------------------------------------------------------------
int _fastcall TSemBase::BigValue(const WCHAR * value, int size, int addzero)
{
  if ((value == 0) || (size <= 0))
    return 0;

  Type(SEMBASEBIGUNI);
  Scale(0xFF);

  WcsCopy(Sembase.BigString.Value, value, size);

  if (addzero)
  {
    WCHAR temp[2];
    temp[0] = value[size / sizeof(WCHAR2) - 1];
    temp[1] = 0;

    size += sizeof(WCHAR2);
    WcsCat(Sembase.BigString.Value, temp, size);
  }

#ifndef RISCCPU
  Sembase.BigString.Size = size;
#else
  REGISTER reg;
  reg.Long = size;
  reg.PutLong((char*)&Sembase.BigString.Size);
#endif

  // Заменить двоичные коды на пробел
#ifndef NUMBERTURN
  int tsize = size / sizeof(WCHAR2) - 1;
  unsigned short int * test = (unsigned short int*)Sembase.BigString.Value;
  while((tsize-- > 0) && (*test != 0))
  {
    if (*test <= 0x09)                      // '\t'
      *test = 0x20;                         // ' '
    test++;
  }
#endif

  return size;
}

// -----------------------------------------------------------------
//  Строка должна быть в кодах ANSI (для Windows) или KOI8 (для UNIX)
//  За перекодировку отвечает тот, кто вызывает эту функцию
// -----------------------------------------------------------------
char * _fastcall TSemBase::Value(const char * value, int maxsize)
{
   if (value == 0)
   {
     Sembase.Char.Type = SEMBASESTRING;
     Sembase.String.Value[0] = 0;
     Sembase.String.Value[1] = 0;
     Sembase.String.Scale = (unsigned char)1;
     return Sembase.String.Value;
   }
#ifdef LINUXAPI
   char newvalue[260];
   int delta = sizeof(newvalue)-1;
   if ((maxsize > 0) && (delta > maxsize))
     delta = maxsize;
   strncpy(newvalue, value, delta);           // Не менять на StrCopy
   newvalue[delta] = 0;
   Koi82Win(newvalue, sizeof(newvalue));
   value = &newvalue[0];
#endif
   Sembase.Char.Type = SEMBASESTRING;

   int length = sizeof(Sembase.String.Value)-1;
   if ((maxsize > 0) && (length > maxsize))
     length = maxsize;
   strncpy(Sembase.String.Value, value, length);           // Не менять на StrCopy
   Sembase.String.Value[length] = 0;                       // length -> sizeof(Sembase.String.Value)-1

   // Заменить двоичные коды на пробел
   unsigned char * test = (unsigned char*)Sembase.String.Value;
   while(*test != 0)
   {
     if (*test <= 0x09)    // '\t'
       *test = 0x20;       // ' '
     test++;
   }

   // Установить размер не кратно 2 (чтобы с нулем было кратно 2)
   length = (int)strlen(Sembase.String.Value) | 1;

   Sembase.String.Scale = (unsigned char)length;
   Sembase.String.Value[length] = 0;
   return Sembase.String.Value;
}

// -----------------------------------------------------------------
//  Преобразовать строку к числу типа "семантика"
//  в соответствии с типом характеристики (TYPE)
//  При ошибке возвращает ноль
// -----------------------------------------------------------------
int _fastcall TSemBase::LoadAnyString(int type, const char* string, int maxsize)
{
  char newvalue[260];
  int  ret;

  if ((type == SEMBASELONG) || (type == SEMBASESHORT) || (type == SEMBASECHAR) || (type == SEMBASEDOUBLE))
  {
    ret = LoadStringNumber(string);
    if (ret)
      return ret;
  }

  if (string == 0)
    return 0;

  if (type == SEMBASELONG4)
  {
    // Преобразовать строку из 32/36 шестнадцатеричных символов в двоичный вид
    char* ident = (char*)&Sembase.Long4.Quad;
    int ret = StringToGUID(string, ident);
    if (ret == 0)
      return 0;

    Type(SEMBASELONG4);
    return ret;
  }

  if (type == SEMBASESTRDOS)
  {
    StrCopy(newvalue, string, sizeof(newvalue));
#ifdef LINUXAPI
    Dos2Koi8(newvalue, sizeof(newvalue) - 1);
#else
    Dos2Win(newvalue, sizeof(newvalue) - 1);
#endif
    string = &newvalue[0];
  }

  Value(string, maxsize);
  return 1;
}

int _fastcall TSemBase::LoadAnyString(int type, const WCHAR* value)
{
  if (value == 0)
    return 0;

  if ((type == SEMBASELONG) || (type == SEMBASESHORT) || (type == SEMBASECHAR) || (type == SEMBASEDOUBLE))
  {
    char avalue[64];
    UnicodeToString(value, avalue, sizeof(avalue));
    int ret = LoadStringNumber(avalue);
    if (ret)
      return ret;
  }

  Value(value);
  return 1;
}

// -----------------------------------------------------------------
//  Перевод строки,(изображающей число) в число типа "семантика"
//  Возврат - количество правильно преобразованных
//  символов, число м.б. округлено
// -----------------------------------------------------------------
int _fastcall TSemBase::LoadStringNumber(const char* str)
{
  if (str == 0)
    return 0;

  int k;
  int num;
  char sim;

  int placezn = 0; // место знака
  int is_zn = 0; // 1- в числе был знак
  int placedot = 0; // место . или ,
  int is_null = 0; // 1 - был 0
  int val = 0; // количество значащих цифр
  int placeval = 0; // начало значащих цифр
  int scale = 0;
  int is_dot = 0; // была . или ,

  char st[40]; st[0] = 0;
  StrCopy(st, str, sizeof(st));
  int j = (int)strlen(st);
  char* endptr = st;

  int i;
  for (i = 0; i < j; i++)
  {
    sim = *(st + i);
    if ((sim == '-') || (sim == '+'))
    {
      // место знака ,единственность знака
      if (!is_zn)        // если уже были цифры - это ошибка
      {
        if (val)
          break;
        is_zn++;
        placezn = i;
        continue;
      }
      else
        break;
    }

    if ((sim == '.') || (sim == ','))
    {                    // место точки,единственность точки
      if (!is_dot)
      {
        is_dot++;
        placedot = i;
        continue;
      }
      else
        break;
    }

    if ((sim <= '9') && (sim >= '1'))  // место начала цифр
    {
      if (!val)
        placeval = i;   // подсчет значащих
      val++;
      if (is_dot)
        scale--; // учли значащие после ,
      continue;
    }
    if (sim == '0')
    {
      is_null = 1;
      if (val)
        val++;     // подсчет значащих
      if (is_dot)
        scale--;   // учли значащие после ,
      continue;
    }
    if (sim == ' ')
    {
      if ((val) || (is_dot))
        break;
      else
        continue;      // если пробел после начала цифр или .,закончили разбирать строку
    }
/* Volkov I.A. BEGIN */
//    if ((sim == 'e') || (sim == 'E') || (sim == 'е') || (sim == 'Е'))
//    {
//      // float
//      if ((sim == 'Е') || (sim == 'е'))
//        *(st + i) = 'E';
      if ((sim == 'e') || (sim == 'E'))
          // float
      {
/* Volkov I.A. END */

      double dnum = strtod(st, &endptr);
      for (i = 0; i < j; i++)
        if (*(st + i) == *endptr)
          break;

      Value(dnum);
      return(i);
    }

    break;                // получили неположенный символ
  }

  if (!val)       // не было значащих цифр
  {
    if (!is_null) // не было 0 запишем как строку
    {
      Value(str, 0);
      return(int)(((strlen(str) < sizeof(Sembase.String.Value)) ? strlen(str) : sizeof(Sembase.String.Value)));
    }
    else  // были 0 ,запишем 0
    {
      Sembase.Char.Scale = 0;
      Sembase.Short.Value = 0;
      Sembase.Short.Type = SEMBASESHORT;
      return i;
    }
  }

  // убрали точку
  if (is_dot)
  {
    int ssize = (int)strlen((const char*)&st[0]);
    for (j = placedot; j < ssize; j++)
    {
      *(st + j) = *(st + j + 1);
    }

    if (placedot < placeval)
      placeval--;
  }

  k = 0;
  if (is_zn)                  // число начинаем со знака,если он есть
  {
    *st = *(st + placezn);
    k++;
  }

  memmove(st + k, st + placeval, val);   //приформировали к знаку число
  if (scale < 0)
  {
    if (-scale > 15)
    {
      val -= (-scale - 16);
      scale = -16;
    }
  }
  // если число со столькими знаками не входит в строку из 18
  // обрезаю последние значащие в дроби
  else
  {
    if ((scale + val) > 10)
    {
      // это число слишком большое,должно быть записано  тип float
      double dnum = strtod(st, &endptr);
      for (i = 0; i < j; i++)
      {
        if (*(st + i) == *endptr)
          break;
      }

      Value(dnum);
      return(i);
    }
  }

  *(st + k + val) = 0;
  if (val > 10)
  {
    *(st + k + 10) = 0;      // отрезали лишние цифры
    scale += (val - 10);
    val = 10;
  }

  if ((val == 10) && (strcmp(st + k, "2147483647") > 0))
  {
    *(st + k + 9) = '\0';
    scale++;
  }

  num = atol(st);
  if (num < 0)
  {
    is_zn = 0;
    num = -num;
  }
  else
    is_zn = 2;

  if (num < 32767)
  {
    num *= (-1 + is_zn);
    Sembase.Short.Value = (signed short int)num;
    Sembase.Short.Type = SEMBASESHORT;
  }
  else
  {
    num *= (-1 + is_zn);
    Value(num);
  }

  Sembase.Char.Scale = (signed char)scale;
  return i;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++ МЕТОДЫ ДОСТУПА К СЕМАНТИЧЕСКИМ ХАРАКТЕРИСТИКАМ  ++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -----------------------------------------------------------------
// Запросить значение характеристики в заданном виде
// -----------------------------------------------------------------
int TSemBlock::LongValue()
{
  unsigned int value = Data.LongValue();

  if ((Code() == SEMDATEFROM) && (value != 0))
  {
    unsigned int base = value / 10000;
    if ((base > 2000) && (base < 2100))
    {
      // Запросить текущую дату
      unsigned int today;
      GetTheSystemDateAndTime(&today, 0);

      if (value < today)
      {
        int sourceday = GetDayCountFromYear(value, base);
        int currentday = GetDayCountFromYear(today, base);
        return (currentday - sourceday);
      }
      else
      if (value > today)
      {
        // Дата не наступила - возвращается отрицательное значение
        unsigned int todayyear = today / 10000;
        int sourceday = GetDayCountFromYear(value, todayyear);
        int currentday = GetDayCountFromYear(today, todayyear);
        return (currentday - sourceday);
      }
      else
        return 0;
    }
  }

  return value;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++ МЕТОДЫ ОБРАБОТКИ НАБОРА СЕМАНТИЧЕСКИХ ХАРАКТЕРИСТИК ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -----------------------------------------------------------------
//  Установить доступ к семантике объекта
// -----------------------------------------------------------------
void _fastcall TWorkSemantic::Access(char* begin, int lengthsemantic, int lengthbuffer)
{
  AppLen = 1;
  IsCorrect = 0;
  IsDirty(0);

  // Контроль корректности параметров
  if (lengthsemantic <= 0)
  {
    lengthsemantic = 0;
    IsCorrect = 1;
  }

  if ((lengthbuffer > 0) && (lengthbuffer < lengthsemantic))
  {
    lengthsemantic = lengthbuffer;
  }
  else
    if (lengthbuffer == 0)
    {
      lengthbuffer = lengthsemantic;
      AppLen = 0;
    }

  BufLength = lengthbuffer;
  SemLength = lengthsemantic;
  BegSem = (char*)begin;
  EndSem = begin + lengthsemantic;
  if (begin)
  {
    GoFirst();
  }
  else
  {
    Number = 0;
    Block = 0;
  }
}

// -----------------------------------------------------------------
// Установить доступ к семантике объекта
// -----------------------------------------------------------------
int _fastcall TWorkSemantic::Access(SEMANTIC * semn)
{
  if ((semn == 0) || !semn->IsCorrect())
    return 0;

  char * begin  = ((char *)semn) + semn->GetHeadLength();
  int length = semn->GetDataLength();
  if (length <= 0)
    return 0;

  Access(begin, length, 0);
  return 1;
}

// -----------------------------------------------------------------
// Проверка и коррекция длины семантики
// -----------------------------------------------------------------
void TWorkSemantic::CheckSemLength()
{
  TSemBlock* block = Block;
  int number = Number;

  if (GoFirst() == 0)
  {
    Number = 0;
    return;
  }

  while (GoNext() != 0) { ; }

  Block = block;
  Number = number;
}

// -----------------------------------------------------------------
// Проверка и коррекция длины семантики
// Если запись корректировалась - возвращает -1
// -----------------------------------------------------------------
int TWorkSemantic::CheckSemStruct(int* group, int* textgroup)
{
  int ret = 1;

  if (GoFirst() == 0)
  {
    Number = 0;
    return ret;
  }

  TSemBlock* block = Block;
  int number = Number;

  do
  {
    if (Block != 0)
    {
      int code = Block->Code();
      if (group || textgroup)
      {
        if ((code >= SEMGROUPLEADER) && (code <= SEMGROUPPARTNER))
        {
          if (group)
            *group = 1;
        }
        else
          if ((code == SEMOBJECTTOTEXT) || (code == SEMOBJECTFROMTEXT))
          {
            if (textgroup)
              *textgroup = 1;
          }
      }

      if (Block->CheckDoubleValue() == 0)
      {
        // Некорректное значение записи
        Block->Value(0.0);
        IsDirty(1);
        IsRealDirty(code);
        ret = -1;
      }
    }
  } while (GoNext() != 0);

  Block = block;
  Number = number;

  return ret;
}

// -----------------------------------------------------------------
//  Найти блок по коду характеристики
// -----------------------------------------------------------------
TSemBlock * _fastcall TWorkSemantic::FindCode(int code)
{
  if ((Length() <= 4) || (GoFirst() == 0))
    return 0;

  if (Block->Code() == code)
    return Block;

  return FindCodeNext(code);
}

// -----------------------------------------------------------------
//  Продолжить поиск характеристики по коду семантики(после текущего)
// -----------------------------------------------------------------
TSemBlock * _fastcall TWorkSemantic::FindCodeNext(int code)
{
  while (GoNext() != 0)
  {
    if (Block->Code() == code)
      return Block;
  }

  return 0;
}

// --------------------------------------------------------
// Найти характеристику по коду семантики
// При ошибке возвращает ноль
// --------------------------------------------------------
TSemBlock * _fastcall TWorkSemantic::FindCode(int code, int number)
{
  TSemBlock *block = FindCode(code);
  if (block == 0)
    return 0;

  while(--number > 0)
  {
    block = FindCodeNext(code);
    if (block == 0)
      break;
  }

  return block;
}

// -----------------------------------------------------------------
// Найти характеристику по коду семантики и и значению
// -----------------------------------------------------------------
//Volkov I.A. BEGIN
//TSemBlock * _fastcall TWorkSemantic::FindBlock(TSemBlock& block)
//{
//  if ((Length() <= 4) || (GoFirst() == 0))
//    return 0;

//  if (Block->Compare(&block) == CMEQUAL)
//    return Block;
//                         // Проверить следующий
//  while (GoNext() != 0)
//  {
//    if (Block->Compare(&block) == CMEQUAL)
//      return Block;
//  }

//  return 0;              // Не нашли
//}
//Volkov I.A. END

// -----------------------------------------------------------------
// Найти номер группы объекта в семантике для кодов
// SEMGROUPLEADER, SEMGROUPSLAVE, SEMGROUPPARTNER
// code - поле для записи найденного кода семантики
// При ошибке возвращает ноль
// -----------------------------------------------------------------
int _fastcall TWorkSemantic::FindGroup(int* code)
{
  if ((Length() <= 4) || (GoFirst() == 0))
    return 0;

  do
  {
    int bcode = Block->Code();
    if ((bcode == SEMGROUPLEADER) || (bcode == SEMGROUPSLAVE) || (bcode == SEMGROUPPARTNER))
    {
      if (code) *code = bcode;
      return Block->LongValue();
    }
  }
    while (GoNext() != 0);

  return 0;              // Не нашли
}

// -----------------------------------------------------------------
// Найти семантику заданного типа
// index - порядковый номер семантики, с которой начинается поиск
// Возвращает номер семантики заданного типа
// При ошибке возвращает ноль
// -----------------------------------------------------------------
int _fastcall TWorkSemantic::FindType(int index, TBitSetVariable& bits)
{
  if (index <= 0)
    index = 1;

  while(1)
  {
    TSemBlock * block = GoNumber(index);
    if (block == 0)
      break;

    if (bits.Check(block->Code()))
      return index;

    index++;
  }

  return 0;
}

// -----------------------------------------------------------------
//  Вставить характеристику (перед текущим блоком)
// -----------------------------------------------------------------
int _fastcall TWorkSemantic::Insert(TSemBlock& block)
{
  if (((char *)Block > EndSem) || (Block == 0))
    return 0;  // если такого блока нет

  int size = block.Length();

  if ((AppLen == 0) ||                 // если вообще нельзя добавлять
      (size   <= 0) ||                 // если отрицательная или нулевая длина блока
                                       // добавленная информация не входит в буфер
      ((EndSem + size) > (BegSem + BufLength)))
    return 0;

  // не в конце - раздвигаем
  if ((char *)Block < EndSem)
    memmove((char *)Block + size, (char *)Block, EndSem - (char *)Block);

  // изменили конец и длину
  EndSem    += size;
  SemLength += size;

  // запись
  memmove((char *)Block, &block, size);

  IsDirty(1);
  IsRealDirty(Block->Code());

  return size;
}

// -----------------------------------------------------------------
//  Вставить характеристику (перед текущим блоком)
// -----------------------------------------------------------------
int _fastcall TWorkSemantic::Insert(int code, const WCHAR* value, int vsize)
{
  if (value == 0)
    return 0;

  if (((char*)Block > EndSem) || (Block == 0))
    return 0;  // если такого блока нет

  // Ограничить буфер входной строки ее длиной с учетом конца строки
  int len = (int)(Wcslen(value) + 1) * sizeof(WCHAR);
  if (vsize <= 0)
    vsize = len;
  else
    if (vsize > len)
      vsize = len;

  if (vsize <= 254)
  {
    TSemBlock newblock;
    newblock.Code(code);
    if (newblock.Value(value) == 0)
      return 0;

    return (Insert(newblock) != 0);
  }

  BIGSEMWSTRING* point;

  // int size = sizeof(code) + sizeof(BIGSEMWSTRING) - sizeof(point->Value) + vsize;
  // 2 = sizeof(Block->SemCode)
  int size = 2 + sizeof(BIGSEMWSTRING) - sizeof(point->Value) + vsize;

  int zero = 0;
  if (*(value + vsize / sizeof(WCHAR) - 1) != 0)
  {
    // Строка не заканчивается нулем - добавить
    size += 2;
    zero = 2;
  }

  if ((AppLen == 0) ||               // если вообще нельзя добавлять
      (size <= 0) ||                 // если отрицательная или нулевая длина блока
                                     // добавленная информация не входит в буфер
    ((EndSem + size) > (BegSem + BufLength)))
    return 0;

  // не в конце - раздвигаем
  if ((char*)Block < EndSem)
    memmove((char*)Block + size, (char*)Block, EndSem - (char*)Block);

  // изменили конец и длину
  EndSem += size;
  SemLength += size;

  Block->Code(code);
  Block->BigValue(value, vsize, zero);

  IsDirty(1);
  IsRealDirty(code);

  return 1;
}

// -----------------------------------------------------------------
// Установить признак обновления семантики для неслужебных кодов
// -----------------------------------------------------------------
int _fastcall TWorkSemantic::IsRealDirty(int code)
{
  if (IsRealdirty)
    return 1;

  if ((code >= EXTSEMANTIC) && (code <= EXTSEMANTICEND))
    return 0;

  if ((code >= SEMHIDEFIRST) && (code <= SEMSERVICELAST))
    return 0;

  return (IsRealdirty = 1);
}

// -----------------------------------------------------------------
//  Перейти на конец записи семантики
// -----------------------------------------------------------------
TSemBlock * TWorkSemantic::GoEnd()
{
  while(GoNext() != 0);
  return Block;
}

// -----------------------------------------------------------------
//  Перейти на следующую характеристику из уже существующих
// -----------------------------------------------------------------
TSemBlock* TWorkSemantic::GoNext()
{
  if ((char*)Block >= EndSem)  // Выход за границы семантики
    return 0;

  if (Block == 0)
    if (GoFirst() == 0)
      return 0;

  TSemBlock* block = Block;            // для коррекции длины семантики

  unsigned int type = Block->Type();
  if (((type & 1) && (type != 1) && (type != 127)) || (type > SEMBASELAST) || ((type > 16) && (type < 126)))
  {
    // Ошибочный тип семантики - длина будет мусором
    return 0;
  }

  int blocklength = Block->Length();
  if (blocklength < 0)
  {
    if ((SemLength = (int)((char*)block - BegSem)) < 0)
    {
      SemLength = 0;
      EndSem = BegSem;
      GoFirst();
      return 0;
    }

    EndSem = (char*)block;
    Block = block;
  }
  else
  {
    Block = (TSemBlock*)((char*)Block + blocklength);	// сдвинулись

    if ((((char*)Block) + sbsminilength) > EndSem)     // блок не вошел
    {                                  // корректируем длину семантики
      if ((char*)Block > EndSem)
      {
        if ((SemLength = (int)((char*)block - BegSem)) < 0)
        {
          SemLength = 0;
          EndSem = BegSem;
          GoFirst();
          return 0;
        }

        EndSem = (char*)block;
        Block = block;
      }
      else      // Отбросим неполный блок
      {
        EndSem = (char*)Block;
        SemLength = (int)((char*)Block - BegSem);
        Number++;
      }
    }
    else
    {
      Number++;
    }
  }

  if ((char*)Block == EndSem)   // больше семантик нет
  {
    IsCorrect = 1;

    if (SemLength < BufLength)
      AppLen = 1; // добавлять можно

    return 0;
  }

  return Block;
}

// -----------------------------------------------------------------
//  Перейти на характеристику с порядковым номером number
//  number начинается с 1
// -----------------------------------------------------------------
TSemBlock* _fastcall TWorkSemantic::GoNumber(int number)
{
  if (number <= 0)
    return 0;

  if ((Number <= 0) || (Number > number) || (Block == 0) || ((char*)Block >= EndSem))
  {
    if (GoFirst() == 0)
      return 0;
  }

  if (Number == number)
    return Block;

  number -= Number;

  while (number-- > 0)
  {
    if (GoNext() == 0)
      return 0;
  }

  return Block;
}

// -----------------------------------------------------------------
//  Удалить текущую характеристику
// -----------------------------------------------------------------
int TWorkSemantic::Delete()
{
  if (SemLength < 5)
    return 0;

  if (((char*)Block >= EndSem) || (Block == 0))
    return 0;  // если такого блока нет

  int blocklength = Block->Length();

  EndSem -= blocklength;

  if ((SemLength -= blocklength) < 0)
  {
    EndSem = BegSem;
    SemLength = 0;
    GoFirst();
    return 1;
  }

  int length;
  if ((length = (int)(EndSem - (char*)Block)) > 0)
  {
    memmove((char*)Block, (char*)((char*)Block + blocklength), length);
  }
  else
  {
    if ((char*)Block > EndSem)  // выход за границы семантики
      GoFirst();
  }

  Isdirty = 1;
  return 1;
}

// -----------------------------------------------------------------
//  Определить количество семантик
// -----------------------------------------------------------------
int TWorkSemantic::SemanticsAmount()
{
  if (SemLength < 5)
    return 0;

  TSemBlock* block = Block;
  int number = Number;
  int amount = 1;

  if (GoFirst() == 0)
    return 0;
  while (GoNext() != 0)
  {
    amount++;
  }

  Block  = block;
  Number = number;

  return amount;
}

// -----------------------------------------------------------------
//  Заменить значение характеристики у текущего блока
// -----------------------------------------------------------------
int _fastcall TWorkSemantic::ReplaceValue(int value)
{
  if (((char *)Block >= EndSem) || (Block == 0))
    return 0;  // если такого блока нет

  TSemBlock newblock;
  newblock.Code(Block->Code());

  if (newblock.Value(value) == 0)
    return 0;

  return Update(newblock);
}

// -----------------------------------------------------------------
//  Заменить значение характеристики у текущего блока
// -----------------------------------------------------------------
int _fastcall TWorkSemantic::ReplaceValue(const char * value, int maxsize)
{
// Volkov I.A. BEGIN
//  if (((char *)Block >= EndSem) || (Block == 0))
//    return 0;

//  if (maxsize <= 254)
//  {
//    TSemBlock newblock;
//    newblock.Code(Block->Code());

//    int type = Block->Type();
//    if (type == SEMBASESTRDOS)
//      type = SEMBASESTRING;

//    if (newblock.Value(type, value, maxsize) == 0)
//      return 0;

//    return Update(newblock);
//  }

//  TLongStringUn buffer;
//  buffer.addString(value, maxsize);

//  const WCHAR2* point = buffer.getString();

//  return ReplaceValue(point, 0);
    return 0;
// Volkov I.A. END
}

// -----------------------------------------------------------------
//  Заменить значение характеристики у текущего блока
// -----------------------------------------------------------------
int _fastcall TWorkSemantic::ReplaceValue(const WCHAR2* value, int vsize)
{
  if (((char*)Block >= EndSem) || (Block == 0))
    return 0;  // если такого блока нет

  if (value == 0)
    return 0;

  if (vsize <= 0)
    vsize = (Wcslen(value) + 1) * sizeof(WCHAR2);

  if (vsize <= 254)
  {
    TSemBlock newblock;
    newblock.Code(Block->Code());
    if (newblock.Value(value) == 0)
      return 0;

    return Update(newblock);
  }

  int code = Block->Code();

  if ((char*)Block < EndSem)  // находимся не в конце семантики
    Delete();

  return Insert(code, value, vsize);
}

// -----------------------------------------------------------------
//  Установить длину семантики
//  Возвращает новую длину семантики или 0 при ошибке
// -----------------------------------------------------------------
unsigned int _fastcall TWorkSemantic::Length(unsigned int value)
{
  if (value > (unsigned int)BufLength)
    value = BufLength;

  SemLength = value;

  if (BegSem)
  {
    GoFirst();

    EndSem = BegSem + value;
    IsCorrect = 0;

    return Length();
  }

  return 0;  // Адрес буфера не установлен
}

// -----------------------------------------------------------------
//  Загрузить набор семантических характеристик
//  из заданной области
//  При ошибке возвращает ноль
//  При успешном выполнении - размер созданной записи
// -----------------------------------------------------------------
int TWorkSemantic::Load(TWorkSemantic * worksemn)
{
  if (worksemn == 0)
    return 0;
  Length(0);

  int count = worksemn->Count();
  int i = 1;
  while(count-- > 0)
  {
     TSemBlock * block = worksemn->GoNumber(i++);
     if (block == 0)
       break;
     if (Insert(*block) == 0)
       break;
     GoNext();
  }

  Isdirty = 0;
  return Length();
}

// -----------------------------------------------------------------
//  Найти характеристику по коду семантики и значению
//  При ошибке возвращает ноль
// -----------------------------------------------------------------
TSemBlock * _fastcall TWorkSemantic::FindCodeAndValue(int code,const char * value)
{
  TSemBlock * block = FindCode(code);
  if (block == 0)
    return 0;

  int j = (int)strlen(value);

  // Для русских букв все равно различает регистр ...
  int i = strncmpi(block->StringInAnsi(), value, j);

  if (i != 0)
    return 0;

  return block;  // Совпадение
}

TSemBlock * _fastcall TWorkSemantic::FindCodeAndValue(int code, int value)
{
  TSemBlock * block = FindCode(code);
  if (block == 0)
    return 0;
  if (block->LongValue() != value)
    return 0;
  return block;  // Совпадение
}

TSemBlock * _fastcall TWorkSemantic::FindCodeAndValue(int code,double& value)
{
  TSemBlock * block = FindCode(code);
  if (block == 0)
    return 0;
  double i = block->DoubleValue();
  if (i != value)
    return 0;

  return block;  // Совпадение
}

// -----------------------------------------------------------------
//  Найти характеристику по коду семантики и сравнить значение
//  При ошибке возвращает BAD
// -----------------------------------------------------------------
HOW _fastcall TWorkSemantic::CompareCodeAndValue(int code,const char * value)
{
  if (value == 0)
    return CMBAD;

  TSemBlock * block = FindCode(code);
  if (block == 0)
    return CMBAD;

  const unsigned char * point = (const unsigned char *)(block->StringInAnsi());

  return (HOW) mapCompareTemplateString(point,(const unsigned char *)value);
}

HOW _fastcall TWorkSemantic::CompareCodeAndValue(int code,long value)
{
  TSemBlock * block = FindCode(code);
  if (block == 0)
    return CMBAD;

  long i = block->LongValue();

  if (i < value)
    return(CMLESS);
  if (i > value)
    return(CMMORE);

  return CMEQUAL;  // Совпадение
}

HOW _fastcall TWorkSemantic::CompareCodeAndValue(int code, double& value)
{
  TSemBlock * block = FindCode(code);
  if (block == 0)
    return CMBAD;

  double i = block->DoubleValue();

  if (i < value)
    return(CMLESS);
  if (i > value)
    return(CMMORE);

  return CMEQUAL;  // Совпадение
}

// -----------------------------------------------------------------
// Заменить текущую характеристику
// При ошибке возвращает ноль
// -----------------------------------------------------------------
int _fastcall TWorkSemantic::Update(TSemBlock& block)
{
  if (((char*)Block >= EndSem) || (Block == 0))
    return 0;  // если такого блока нет

  if ((Block->Type() == block.Type()) && ((Block->Type() == SEMBASELONG) || (Block->Type() == SEMBASEDOUBLE)))
  {
    // Семантика типа "число" заменить на месте,
    // чтобы не менять нумерацию блоков при обновлении формулы
    memmove((char*)Block, (char*)&block, Block->Length());
    Isdirty = 1;
    return Number;
  }

  int ret = 0;

  if ((char*)Block < EndSem)  // находимся не в конце семантики
    ret = Delete();

  if (ret == 0)
    return Append(block);

  return Insert(block);
}

// --------------------------------------------------------
// Найти значение в виде целого числа по коду семантики
// При ошибке возвращает ноль
// --------------------------------------------------------
int _fastcall TWorkSemantic::FindCodeLongValue(int code, int number)
{
  TSemBlock *block = FindCode(code, number);
  if (block == 0)
    return 0;
  return block->LongValue();
}

// --------------------------------------------------------
// Найти значение в виде числа с плавающей точкой по коду семантики
// При ошибке возвращает ноль
// --------------------------------------------------------
int _fastcall TWorkSemantic::FindCodeDoubleValue(int code, double& value, int number)
{
  TSemBlock *block = FindCode(code, number);
  if (block == 0)
    return 0;
  value = block->DoubleValue();
  return 1;
}

// --------------------------------------------------------
// Найти минимальное значение в виде числа с плавающей точкой по коду семантики
// При ошибке возвращает ноль
// --------------------------------------------------------
int _fastcall TWorkSemantic::GetCodeMinDoubleValue(int code, double& value)
{
  TSemBlock *block = FindCode(code);
  if (block == 0)
    return 0;

  value = block->DoubleValue();

  while((block = FindCodeNext(code)) != 0)
  {
    block = FindCodeNext(code);
    if (block == 0)
      break;

    double temp = block->DoubleValue();
    if (temp < value)
      value = temp;
  }

  return 1;
}

// --------------------------------------------------------
// Найти максимальное значение в виде числа с плавающей точкой по коду семантики
// При ошибке возвращает ноль
// --------------------------------------------------------
int _fastcall TWorkSemantic::GetCodeMaxDoubleValue(int code, double& value)
{
  TSemBlock* block = FindCode(code);
  if (block == 0)
    return 0;

  value = block->DoubleValue();

  while ((block = FindCodeNext(code)) != 0)
  {
    block = FindCodeNext(code);
    if (block == 0)
      break;

    double temp = block->DoubleValue();
    if (temp > value)
      value = temp;
  }

  return 1;
}

// --------------------------------------------------------
// Найти среднее значение в виде числа с плавающей точкой по коду семантики
// При ошибке возвращает ноль
// --------------------------------------------------------
int _fastcall TWorkSemantic::GetCodeMeanDoubleValue(int code, double& value)
{
  TSemBlock* block = FindCode(code);
  if (block == 0)
    return 0;

  value = block->DoubleValue();
  int number = 1;

  while ((block = FindCodeNext(code)) != 0)
  {
    block = FindCodeNext(code);
    if (block == 0)
      break;

    value += block->DoubleValue();
    number++;
  }

  value /= number;

  return number;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++ ЗАПОЛНЕНИЕ ЗАПИСИ СЕМАНТИКИ ++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
#ifndef _GISSERVER

// ----------------------------------------------------------
//  Выделить память под максимальную запись семантики
//  Проинициализировать структуру данных
//  При ошибке возвращает ноль
// ----------------------------------------------------------
int _fastcall TSemanticRecord::Allocate(int length)
{
  if (length < sizeof(SEMANTIC))
    return 0;

  if ((Base != 0) && (BufferLength >= length))
    return 1;

  if (Base == 0)
    length += 2 * 1024;

  char* temp;
  try { temp = new char[length]; }
  catch (...) { temp = 0; }
  if (temp == 0)
    return 0;

  if (Base == 0)
  {
    Base = (SEMANTIC*)temp;
    BufferLength = length;

    Work.Access(((char*)Base) + sizeof(SEMANTIC), 0, BufferLength - sizeof(SEMANTIC));

    Base->Clear();
  }
  else
  {
    int semlength = Base->GetDataLength();
    memcpy(temp, (char*)Base, BufferLength);
    FreeMemory();

    Base = (SEMANTIC*)temp;
    BufferLength = length;

    Work.Access(((char*)Base) + sizeof(SEMANTIC), semlength, BufferLength - sizeof(SEMANTIC));
  }

  return 1;
}

// ----------------------------------------------------------
// Освободить память
// ----------------------------------------------------------
void TSemanticRecord::FreeMemory()
{
  if (Base != 0)
  {
    delete[] (char*)Base;
    Base = 0;
    BufferLength = 0;
  }
}

// ----------------------------------------------------------
//  Добавить характеристику (в конец семантики)
//  При ошибке возвращает ноль
// ----------------------------------------------------------
int _fastcall TSemanticRecord::Append(TSemBlock& block)
{
  int size = block.Length();

  if ((Base == 0) || ((size += Base->GetLength()) > Work.BufLength))
  {
    if (Allocate(size) == 0)
      return 0;
  }

  if (Work.Append(block) == 0)
    return 0;

  return CheckLength();
}

// --------------------------------------------------------
// Добавить строку UNICODE (в конец семантики)
// При ошибке возвращает ноль
// --------------------------------------------------------
int _fastcall TSemanticRecord::Append(int code, const WCHAR* value, int vsize)
{
  if (value == 0)
    return 0;
  if (vsize == 0)
    vsize = (int)(Wcslen(value) + 1) * 2;

  BIGSEMWSTRING* p;
  int size = sizeof(code) + sizeof(BIGSEMWSTRING) - sizeof(p->Value) + vsize;

  if ((Base == 0) || ((size += Base->GetLength()) > Work.BufLength))
  {
    if (Allocate(size) == 0)
      return 0;
  }

  if (Work.Append(code, value, vsize) == 0)
    return 0;

  return CheckLength();
}

// --------------------------------------------------------
// Добавить характеристику (в конец семантики)
// При ошибке возвращает ноль
// --------------------------------------------------------
int _fastcall TSemanticRecord::Append(int code, int value)
{
  TSemBlock newblock;
  newblock.Code(code);
  newblock.Value(value);
  return Append(newblock);
}

// --------------------------------------------------------
// Добавить характеристику (в конец семантики)
// При ошибке возвращает ноль
// --------------------------------------------------------
int _fastcall TSemanticRecord::Append(int code, double value)
{
  TSemBlock newblock;
  newblock.Code(code);
  newblock.Value(value);
  return Append(newblock);
}

// --------------------------------------------------------
// Проверить реальную длину записи
// При ошибке возвращает ноль
// --------------------------------------------------------
int _fastcall TSemanticRecord::CheckLength()
{
  if (Base == 0)
    return 0;

  int length = Work.Length();

  if (Base->IsBigLength() || (length < 65000))
    return Base->SetDataLength(length);

  // Расширить запись
  char* begin = ((char*)Base) + sizeof(BIGSEMANTIC);
  memmove(begin, begin - 4, length);
  Work.Access(begin, length, Work.BufferLength());
  Work.IsDirty(1);

  return (((BIGSEMANTIC*)Base)->SetBigLength(length + sizeof(BIGSEMANTIC)));
}

#endif

#ifndef HIDEBMX
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++ Поиск Бойера-Мура-Хорспула +++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

TSearchBMX::TSearchBMX(const WCHAR *substr) : SubStr()
{
  ShiftArray     = 0;
  MinValue       = 0;
  NormalMaxValue = 0;

  if (substr)
    SetSubstring(substr);
}

TSearchBMX::~TSearchBMX()
{
  DELETE_ARR(ShiftArray);
}

//-------------------------------------------------------------------------------
// Установить подстроку substr
//-------------------------------------------------------------------------------
int TSearchBMX::SetSubstring(const WCHAR *substr)
{
  SubStr = substr;

  if ((substr == 0) || (*substr == 0))
    return 0;

  SubStr.UpperCase();

  FillShiftsArrayForSubstring(SubStr.getString(), SubStr.getSize() / sizeof(WCHAR));
  return 1;
}

//-------------------------------------------------------------------------------
// Найти подстроку substr в строке value
//-------------------------------------------------------------------------------
const WCHAR* TSearchBMX::SearchSubstring(const WCHAR* value, const WCHAR *substr)
{
  if ((substr == 0) || (SetSubstring(substr) == 0))
    return 0;

  return SearchSubstring(value);
}

//-------------------------------------------------------------------------------
// Найти подстроку substr в строке value
// При ошибке возвращает ноль
//-------------------------------------------------------------------------------
const WCHAR* TSearchBMX::SearchSubstring(const WCHAR* value)
{
  if (value == 0)
    return 0;

  int substrLength = SubStr.getSize() / sizeof(WCHAR);
  if (substrLength == 0)
    return 0;

  int valueLength = Wcslen(value);
  if (valueLength < substrLength)
    return 0;

  int substrLengthMinus = substrLength - 1;
  int i = substrLengthMinus;
  valueLength--;

  while(i <= valueLength) // Пока правый конец подстроки не достигнет правого конца строки
  {
    int k = 0; // Количество совпавших символов

    const WCHAR * point  = SubStr.getString() + substrLengthMinus;
    const WCHAR * pvalue = value + i;

    while(k < substrLength)
    {
      WCHAR symbol = ToUpperCase(*pvalue);
      if (*point != symbol)
      {
        i += GetShiftArray(symbol);
        break;
      }

      k++;
      point--;
      pvalue--;
    }

    if (k == substrLength)
      return &value[i - substrLengthMinus];
  } 

  return 0;
}

//-------------------------------------------------------------------------------
// Заполнить значения смещения для каждого символа подстроки
// substr - подстрока поиска
// length - длина подстроки
//-------------------------------------------------------------------------------
void TSearchBMX::FillShiftsArrayForSubstring(const WCHAR *substr, int length)
{
  NormalMaxValue = 0;
  MinValue = 32767; // 0x0FFFF;
  Step     = length;

  short int maxvalue = 0;

  short int * point = (short int *)substr;

  for(int i = 0; i < length; i++, point++)
  {
    short int value = (short int)(*point);
    if (value > maxvalue)
      maxvalue = value;
    if (value < MinValue)
      MinValue = value;
  }

  int count = maxvalue - MinValue + 1;

  NormalMaxValue = maxvalue - MinValue;
  if (ShiftArray == 0)
    NEW_ARR(ShiftArray, short int, count);

  short int * shiftarray = ShiftArray;

  // Инициализация
  for (int ii = 0; ii < count; ii++)
  {
    *shiftarray = Step;
    shiftarray++;
  }

  short int curpos = length - 1;

  for (int ii = 0; ii < length - 1; ii++, curpos--, substr++)
  {
    ShiftArray[((short int)*substr) - MinValue] = curpos;
  }
}
#endif // !BUILD_IML
