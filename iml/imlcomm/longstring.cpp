/*** longstring.cpp ************  Belenkov O.V.   ************ 16/12/21 ***
**** longstring.cpp ************  Zheleznyakov V.A.  ********* 15/04/22 ***
**** longstring.cpp ************  Letov V.I.      ************ 09/03/16 ***
**** longstring.cpp ************  Kruzhkova E.A.  ************ 06/06/18 ***
**** longstring.cpp ************  Savelov A.S.    ************ 22/01/20 ***
**** longstring.cpp ************  Derevyagina Zh.A.*********** 28/02/20 ***
**** longstring.cpp ************  Kruzhkov A.E.    *********** 10/08/21 ***
**** longstring.cpp ************  Vitsko D.A.      *********** 21/01/22 ***
**** longstring.cpp ************  Sotnikov P.I.    *********** 15/11/21 ***
**** longstring.cpp ************  Portnyagin E.V.  *********** 22/04/22 ***
*                                                                         *
*                   Copyright (c) PANORAMA Group 1991-2022                *
*                            All Rights Reserved                          *
*                                                                         *
***************************************************************************
*                                                                         *
*      Класс для накопления данных в виде строки произвольной длины       *
*                                                                         *
**************************************************************************/

#ifdef _AFXDLL
  #include "stdafx.h"
#endif

#include "longstring.h"

#ifndef MAPAPI_H
//  #include "mapapi.h"
#endif

#include "string.h"
#include "stdio.h"

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Функции работы со строками
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

//------------------------------------------------------------------------------
// Преобразовать строку, убрав спец символы, запрещенные в имени дирректории,
// заменив их на _
// posStart - позиция начала данных или 0
//------------------------------------------------------------------------------
void ReplaceForbiddenFolderSymbol(WCHAR* rebuildName, int posStart)
{
  if ((rebuildName == 0) || (posStart < 0)) return;

  int len = (int)Wcslen(rebuildName);
  WCHAR* symbol = rebuildName + posStart;
  while(len > 0)
  {
    if ((*symbol == ':') || (*symbol == '/') || (*symbol == '\\') ||
        (*symbol == '*') || (*symbol == '?') || (*symbol == '\"') ||
        (*symbol == '>') || (*symbol == '<') || (*symbol == '|') )
    {
      *symbol = '_';
    }
    len--;
    symbol++;
  }
}

//---------------------------------------------------------------------------
// Функция поиска части строки между двумя подстроками
// inputString - входная строка
// beginString - подстрока начала поиска
// endString - подстрока конца поиска
// outputString - найденная строка
// size - размер outputString
// Если строка не содержит информацию между подстроками возвращает 0
//---------------------------------------------------------------------------
int SearchSubString(const WCHAR* inputString, const WCHAR* beginString, 
  const WCHAR* endString, WCHAR* outputString, int size)
{
  if (outputString == 0) 
    return 0;                      
  *outputString = 0;

  if ((inputString == 0) || (beginString == 0) || (size <= 0))
    return 0;

  // Поиск первой строки
  const WCHAR* pos = Wcsstr(inputString, beginString);
  // Первая строка не найдена
  if (pos == 0)
    return 0;

  int posstart = (int)((pos - inputString) + Wcslen(beginString));

  // Начало искомой подстроки
  const WCHAR* begin = inputString + posstart;

  int posend;
  if ((endString == 0) || (*endString == 0))
  {
    // Пишем до конца строки
    posend = (int)Wcslen(inputString);
  }
  else
  {
    // Поиск второй строки
    pos = Wcsstr(begin, endString);
    if (pos == 0)
    {
      // Вторая строка не найдена
      return 0;
    }

    posend = (int)(pos - inputString);
    if (posend == posstart)
    {
      // Пустая подстрока
      return 0;
    }
  }

  // Длина подстроки в байтах с дополнительным нулевым символом для WcsCopy
  int delta = (posend - posstart + 1) * 2;
  if (delta > size)
    delta = size;

  // Копировать подстроку с записью нуля в последний символ
  WcsCopy(outputString, begin, delta);
  return 1;
}


//-----------------------------------------------------------------
// Заменить все заданные символы строки на указанный
// instr - входная строка
// size - размер входной строки
// oldsymbol - заменяемый символ
// newsymbol - символ, на который заменяют
// При ошибке возвращает 0
//-----------------------------------------------------------------
int ReplaceSymbol(WCHAR* instr, int size, WCHAR oldsymbol, WCHAR newsymbol)
{
 if (instr == 0)
   return 0;

 size /= sizeof(WCHAR);

 while((*instr != 0) && (size-- > 0))
 {
   if (*instr == oldsymbol)
     *instr = newsymbol;
    instr++;
 }

 return 1;
}

int ReplaceCharSymbol(char* instr, int size, char oldsymbol, char newsymbol)
{
 if (instr == 0)
   return 0;

 while((*instr != 0) && (size-- > 0))
 {
   if (*instr == oldsymbol)
     *instr = newsymbol;
    instr++;
 }

 return 1;
}

//-----------------------------------------------------------------
// Проверить, что в строке число
//-----------------------------------------------------------------
int IsStringDigit(const char* str)
{
  if (str == 0)
    return 0;
  int sep = 0;
  while(*str != 0)
  {
    if ((*str >= 0x30) && (*str <= 0x39))
      str++;
    else
      if ((*str == 0x2C) || (*str == 0x2E))
      {
        str++;
        sep++;
      }
      else
        return 0;
  }
  // если разделитель встречен более 1 раза, то это не число
  return sep < 2 ? 1 : 0;
}
int IsStringDigit(const WCHAR *wstr)
{
  if (wstr == 0)
    return 0;
  int sep = 0;
  while (*wstr != 0)
  {
    if ((*wstr >= (WCHAR)0x30) && (*wstr <= (WCHAR)0x39))
      wstr++;
    else
      if ((*wstr == (WCHAR)0x2C) || (*wstr == (WCHAR)0x2E))
      {
        wstr++;
        sep++;
      }
      else
        return 0;
  }
  // если разделитель встречен более 1 раза, то это не число
  return sep < 2 ? 1 : 0;
}

//-----------------------------------------------------------------
// Заменить символы входной строки '/', '\\' в зависимости от ОС
// instr - входная строка
// size - размер входной строки
// При ошибке возвращает 0
//-----------------------------------------------------------------
int ReplaceSlashSymbol(WCHAR* instr, int size)
{
#ifdef WIN32API
  return ReplaceSymbol(instr, size, '/', '\\');
#else
  return ReplaceSymbol(instr, size, '\\', '/');
#endif
}


//-----------------------------------------------------------------
// Убрать указанные символы из строки из строки
// instr   - входная строка
// symbols - строка из убираемых символов (с завершающим нулем в конце)
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
//-----------------------------------------------------------------
int RemoveEachSymbol(WCHAR *instr, const WCHAR *symbols)
{
  if (instr == 0)
    return 0;

  int wlen = (int)Wcslen(instr);
  int reallen = wlen;
  int removecount = 0;
  WCHAR *inpoint = instr;
  while (*inpoint != 0)
  {
    if (Wcschr(symbols, *inpoint))
    {
      if (wlen != 1)
      {
        memmove(inpoint, inpoint + 1, (wlen - 1)*sizeof(WCHAR));
        wlen--;
        removecount++;
        continue;
      }
      else
      {
        *inpoint = 0;
        removecount++;
        break;
      }
    }

    inpoint++;
    wlen--;
  }

  reallen -= removecount;
  if (reallen >= 0)
  {
    instr[reallen] = 0;
    return reallen;
  }

  return 0;
}

//-----------------------------------------------------------------
// Убрать символ из строки
// instr - входная строка
// symbol - убираемый символ
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
//-----------------------------------------------------------------
int RemoveSymbol(WCHAR* instr, WCHAR symbol)
{
  if (instr && *instr)
  {
    const WCHAR seek[2] ={symbol, 0};
    return RemoveEachSymbol(instr, seek);
  }
  return 0;
}


//-----------------------------------------------------------------
// Заменить указанные символы из строки из строки заданным символом
// instr   - входная строка
// symbols - строка из убираемых символов (с завершающим нулем в конце)
// При ошибке возвращает 0, иначе - количество найденных символов, длина строки не меняется
//-----------------------------------------------------------------
int ReplaceEachSymbol(WCHAR *instr, const WCHAR *symbols, WCHAR newvalue)
{
  int res = 0;
  if (instr && *instr && symbols && *symbols)
  {
    WCHAR *inpoint = instr;
    while (*inpoint != 0)
    {
      if (Wcschr(symbols, *inpoint))
      {
        *inpoint = newvalue;
        res++;
      }
      inpoint++;
    }
  }
  return res;
}

//-----------------------------------------------------------------
// Убрать указанные символы из строки из строки
// instr   - входная строка
// symbols - строка из убираемых символов (с завершающим нулем в конце)
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
//-----------------------------------------------------------------
int RemoveEachCharSymbol(char *instr, const char *symbols)
{
  if (instr == 0)
    return 0;

  int wlen = (int)strlen(instr);
  int reallen = wlen;
  int removecount = 0;
  char *inpoint = instr;
  while (*inpoint != 0)
  {
    if (strchr(symbols, *inpoint))
    {
      if (wlen != 1)
      {
        memmove(inpoint, inpoint + 1, (wlen - 1));
        wlen--;
        removecount++;
        continue;
      }
      else
      {
        *inpoint = 0;
        removecount++;
        break;
      }
    }

    inpoint++;
    wlen--;
  }

  reallen -= removecount;
  if (reallen >= 0)
  {
    instr[reallen] = 0;
    return reallen;
  }

  return 0;
}

//-----------------------------------------------------------------
// Убрать символ из строки
// instr - входная строка
// symbol - убираемый символ
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
//-----------------------------------------------------------------
int RemoveCharSymbol(char* instr, char symbol)
{
  if (instr && *instr)
  {
    const char seek[2] ={symbol, 0};
    return RemoveEachCharSymbol(instr, seek);
  }
  return 0;
}


//-----------------------------------------------------------------
// Заменить указанные символы из строки из строки заданным символом
// instr   - входная строка
// symbols - строка из убираемых символов (с завершающим нулем в конце)
// При ошибке возвращает 0, иначе - количество найденных символов (длина строки не меняется)
//-----------------------------------------------------------------
int ReplaceEachCharSymbol(char *instr, const char *symbols, char newvalue)
{
  int res = 0;
  if (instr && *instr && symbols && *symbols)
  {
    char *inpoint = instr;
    while (*inpoint != 0)
    {
      if (strchr(symbols, *inpoint))
      {
        *inpoint = newvalue;
        res++;
      }
      inpoint++;
    }
  }
  return res;
}

//-----------------------------------------------------------------
// Убрать указанные символы в конце строки, заданные фильтром
// instr  - входная строка
// filter - строка из убираемых символов
// length - исходная длина строки, если известна заранее
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
//-----------------------------------------------------------------
int RemoveLastSymbols(WCHAR *instr, const WCHAR *filter, int length)
{
  if (instr && *instr && filter && *filter)
  {
    if (length <= 0)
      length = Wcslen(instr);
    int res = 0;

    WCHAR *point = instr + length - 1;
    while ((point > instr) && Wcschr(filter, *point))
    {
      *point = 0;
      point--;
      res++;
    }
    length -= res;
    if (length >= 0)
    {
      instr[length] = 0;
      return length;
    }
  }
  return 0;
}


//-----------------------------------------------------------------
// Убрать указанные символы в конце строки, заданные фильтром
// instr  - входная строка
// filter - строка из убираемых символов
// length - исходная длина строки, если известна заранее
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
//-----------------------------------------------------------------
int RemoveLastCharSymbols(char *instr, const char *filter, int length)
{
  if (instr && *instr && filter && *filter)
  {
    if (length <= 0)
      length = (int)strlen(instr);
    int res = 0;

    char *point = instr + length - 1;
    while ((point > instr) && strchr(filter, *point))
    {
      *point = 0;
      point--;
      res++;
    }
    
    length -= res;
    if (length >= 0)
    {
      instr[length] = 0;
      return length;
    }

  }
  return 0;
}


//-----------------------------------------------------------------
// Убрать кавычки из строки
// instr - входная строка
// symbol - убираемый символ
// Возвращает число символов готовой строки
// При ошибке возвращает 0
//-----------------------------------------------------------------
int RemoveQuote(WCHAR * instr, WCHAR symbol)
{
  // Пример: "Озеро ""Глубокое""" -> Озеро "Глубокое"
  WCHAR * srcstr = instr;
  WCHAR * outstr = instr;

  while (*instr != 0)
  {
    if (*instr == symbol)  // Если найдены кавычки
    {
      instr++;             // Пропустить первые кавычки
      if (*instr == 0)
        break;
    }

    *outstr = *instr;      // Сохранить вторые кавычки или иной символ
    outstr++;
    instr++;
  }

  *outstr = 0;
  return (int)(outstr - srcstr);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения строк в кодировке UTF-8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Конструктор
// -------------------------------------------------------------------
TLongString::TLongString(unsigned int minsize, unsigned int stepsize)
{
  Buffer     = 0;
  BufferSize = 0;
  FreeSize   = 0;

  if (minsize < MINSTRINGSIZE)
    minsize = MINSTRINGSIZE;
  if (stepsize < MINSTRINGSIZE)
    stepsize = MINSTRINGSIZE;
  MinSize    = minsize;
  StepSize   = stepsize;
}

TLongString::TLongString(const WCHAR* wstr)
{
  Buffer = 0;
  BufferSize = 0;
  FreeSize = 0;
  MinSize = 0;
  StepSize = MINSTRINGSIZE;

  if (wstr && *wstr)
    addValue(wstr);
}

TLongString::TLongString(const char* astr)
{
  Buffer = 0;
  BufferSize = 0;
  FreeSize = 0;
  MinSize = 0;
  StepSize = MINSTRINGSIZE;

  if (astr && *astr)
    addValue(astr);
}


TLongString::TLongString(const TLongString &source)
{
  Buffer = 0;
  BufferSize = 0;
  FreeSize = 0;
  MinSize = 0;
  StepSize = MINSTRINGSIZE;
  const char *point = source.Buffer;
  if (point)
    addValue(point);
}

// -------------------------------------------------------------------
// Добавить данные
// При ошибке возвращает ноль
// -------------------------------------------------------------------
const char* _fastcall TLongString::addValue(const char* str, int size)
{
  if (str == 0)
    return 0;
  if (size <= 0)
    size = (int)strlen(str);
  if (size == 0)
    return 0;

  if ((Buffer == 0) || (FreeSize <= (unsigned int)(size+2)))
  {
    // Init Buffer
    if (allocateMemory(size+2) == 0)
      return 0;
  }

  char * point = Buffer + (BufferSize - FreeSize);
  memcpy(point, str, size);
  *(point+size) = 0;
  FreeSize -= size;

  // Адрес записанной строки
  return point;
}

// -------------------------------------------------------------------
// Добавить строку в кодировке Unicode16 c преобразованием в Unicode8
// size - размер строки в байтах
// При ошибке возвращает ноль
// -------------------------------------------------------------------
const char* _fastcall TLongString::addValue(const WCHAR* wstr)
{
  if ((wstr == 0) || (*wstr == 0))
    return 0;

  // Для китайского языка 1 символ может занимать 4 байта (русского - 2) в UTF8
  unsigned int size = (unsigned)Wcslen(wstr) * 4 + 16;

  if ((Buffer == 0) || (FreeSize < size))
  {
    // Init Buffer
    if (allocateMemory(size) == 0)
      return 0;
  }

  char * point = Buffer + (BufferSize - FreeSize);
  UnicodeToUnicode8(wstr, point, size);

  // Определим сколько реально записали
  size = (int)strlen(point);
  FreeSize -= size;

  // Адрес записанной строки
  return point;
}

// -------------------------------------------------------------------
// Добавить числовые данные, как строку
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongString::addValue(double value, int number)
{
  if (number <= 0)
  {
    if (value >= 0)
      return addValue((int) (value + 0.5));
    else
      return addValue((int) (value - 0.5));
  }

  char format[32];
  if (number > 16)
    number = 16;

  // %.3f
  Snprintf(format, sizeof(format), "%%.%uf", (unsigned)number);

  char temp[128];
  Snprintf(temp, sizeof(temp), format, value);

  return (addValue(temp) != 0);
}

int _fastcall TLongString::addValue(int value)
{
  char temp[64]; temp[0] = 0;
  IntToStr(value, temp, sizeof(temp));
  return (addValue(temp) != 0);
}

int _fastcall TLongString::addValue(unsigned int value)
{
  char temp[64]; temp[0] = 0;
  Snprintf(temp, sizeof(temp), "%u", value);
  return (addValue(temp) != 0);
}


// -------------------------------------------------------------------
// Добавить нулевой символ
// -------------------------------------------------------------------
void _fastcall TLongString::AppendZeroSymbol()
{
  if ((Buffer == 0) || (FreeSize <= 4))
  {
    if (allocateMemory(256) == 0)
      return;
  }

  char *point = ((char *)Buffer) + (BufferSize - FreeSize);
  *(point) = 0;
  *(point + 1) = 0;
  *(point + 2) = 0;
  *(point + 3) = 0;
  FreeSize -= 2;
}


// -------------------------------------------------------------------
// Добавить в конец пути слэш
// -------------------------------------------------------------------
int _fastcall TLongString::CompleteThePath()
{
  if (Buffer && *Buffer)
  {
    unsigned int length = getLength();
#ifdef WIN32API
    if (Buffer[length-1] != '\\')
      return (addValue("\\") != 0);
#else
    if (Buffer[length-1] != '/')
      return (addValue("/") != 0);
#endif
  }
  return 0;
}

// -------------------------------------------------------------------
// Обрезать строку до заданной позиции
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongString::cutString(const char * end)
{
  unsigned int size = getSize();

  if ((Buffer == 0) || (end < Buffer) || (end > (Buffer+size)))
    return 0;

  FreeSize = BufferSize - (int)(end - Buffer);

  char* point = Buffer + (BufferSize - FreeSize);
  *point = 0;

  return 1;
}


//---------------------------------------------------------------------------
// Обрезать текст там, где встретится заданный фрагмент
//---------------------------------------------------------------------------
int _fastcall TLongString::CropText(const char *seek)
{
  if (Buffer && *Buffer && seek && *seek)
  {
    char *fpos = strstr(Buffer, seek);
    if (fpos)
      return cutString(fpos);
  }
  return 0;
}


// -------------------------------------------------------------------
// Удалить фрагмент строки
// begin - указатель на первый байт удаляемогно фрагмента
// end   - указатель на последний байт удаляемогно фрагмента
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongString::deleteString(const char * begin, const char * end)
{
  if ((Buffer == 0) || (end <= begin))
    return 0;
  char * limit = Buffer + getSize();

  if ((begin < Buffer) || (begin >= limit) || (end <= Buffer))
    return 0;

  if (end >= limit)
    return cutString(begin);

  int copy = (int)(limit - end);
  if (copy > 0)
    memmove((char*)begin, end+1, copy);

  int delta = (int)(end - begin + 1);
  FreeSize += delta;
  if (FreeSize > BufferSize)
    FreeSize = BufferSize;

  char * point = Buffer + (BufferSize - FreeSize);
  *point = 0;

  return 1;
}

// -------------------------------------------------------------------
// Выделить память под строку
// size - размер данных, которые нужно добавить к строке
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongString::allocateMemory(unsigned int size)
{
  // Выделить память с запасом, чтобы не так часто выделять память
  if (Buffer != 0)
  {
    size += BufferSize;
    unsigned int delta;
    if (BufferSize > 4 * 1024 * 1024)
      delta = BufferSize >> 2;  // 1/4
    else
      delta = BufferSize >> 1;  // 1/2
    if (delta < StepSize)
      delta = StepSize;
    size += delta;

    // Размер, кратный 4 Кб, 16 байт в резерв
    size = ((size + 0x0FFF) & 0x0FFFFF000) - 16;
  }
  else
  {
    if (size < MinSize)
      size = MinSize;
    else
    if (size == 0)
      size = StepSize;
  }

  char* point;
  try { point = new char[size + 16]; }
  catch (...) { point = 0; }
  if (point == 0)
    return 0;

  if (Buffer != 0)
  {
    // Сохранить прежнее значение строки
    memcpy(point, Buffer, BufferSize);
    delete[] Buffer;
  }

  Buffer = point;

  FreeSize += (size - BufferSize);
  BufferSize = size;

  point = Buffer + (BufferSize - FreeSize);
  *point = 0;

  return 1;
}

// -------------------------------------------------------------------
// Освободить ресурсы
// -------------------------------------------------------------------
void TLongString::freeMemory()
{
  if (Buffer != 0)
  {
    char * temp = Buffer;
    Buffer     = 0;
    BufferSize = 0;
    FreeSize   = 0;
    delete [] temp;
  }
}

// -------------------------------------------------------------------
// Запросить текущую позицию
// -------------------------------------------------------------------
char * _fastcall TLongString::getCurrentPoint()
{
  if (Buffer == 0)
    return 0;

  return (Buffer + (BufferSize - FreeSize));
}

// -------------------------------------------------------------------
// Зарезервировать место в записи
// -------------------------------------------------------------------
char *_fastcall TLongString::getPlace(int size)
{
  char *point;

  if ((Buffer != 0) && (FreeSize >= (unsigned)size))
  {
    point = getCurrentPoint();
  }
  else
  {
    int delta = size - FreeSize;
    if (delta < 0)
      delta = 64;

    if (allocateMemory(delta) == 0)
      return 0;
    point = getCurrentPoint();
  }

  FreeSize -= size;
  return point;
}

// -------------------------------------------------------------------
// Отсечь часть записи в конце
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongString::freePlace(int size)
{
  if (Buffer == 0)
    return 0;

  if (size >= (int)(BufferSize - FreeSize))
    clearString();
  else
  {
    FreeSize -= size;
    char *point = getCurrentPoint();
    *point = 0;
  }

  return FreeSize;
}

// -------------------------------------------------------------------
// Проверить, что в строке только числа
// -------------------------------------------------------------------
int _fastcall TLongString::IsStringDigit()
{
  if (Buffer == 0)
    return 0;

  return ::IsStringDigit(Buffer);
}

// -------------------------------------------------------------------
// Обновить часть строки по заданной позиции
// Копируется строка без замыкающего нуля, но не более count символов
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongString::putString(const char* str, int start, int count)
{
  if (str == 0)
    return 0;
  if (count <= 0)
    count = (int)strlen(str);

  if (count == 0)
    return 0;

  if ((Buffer == 0) || (BufferSize < (unsigned)(start + count)))
  {
    if (allocateMemory(start + count - BufferSize) == 0)
      return 0;
  }

  unsigned int size = BufferSize - FreeSize;

  if (size < (unsigned)(start + count))
    FreeSize = BufferSize - (start + count);

  memcpy(Buffer + start, str, count);

  return count;
}

// -------------------------------------------------------------------
// Убрать символ из строки
// symbol - убираемый символ
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
// -------------------------------------------------------------------
int _fastcall TLongString::RemoveSymbol(char symbol)
{
  int size;
  if ((size = ::RemoveCharSymbol(Buffer, symbol)) > 0)
    FreeSize = BufferSize - size;
  return size;
}

//-----------------------------------------------------------------
// Убрать указанные символы из строки из строки
// symbols - строка из убираемых символов (с завершающим нулем в конце)
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
//-----------------------------------------------------------------
int _fastcall TLongString::RemoveEachSymbol(const char *symbols)
{
  int size;
  if ((size = ::RemoveEachCharSymbol(Buffer, symbols)) > 0)
    FreeSize = BufferSize - size;
  return size;
}

// -------------------------------------------------------------------
// Заменить указанные символы из строки из строки заданным символом
// instr   - входная строка
// symbols - строка из убираемых символов (с завершающим нулем в конце)
// При ошибке возвращает 0, иначе - количество найденных символов
// -------------------------------------------------------------------
int _fastcall TLongString::ReplaceEachSymbol(const char *symbols, char newvalue)
{
  return ::ReplaceEachCharSymbol(Buffer, symbols, newvalue);
}

// -------------------------------------------------------------------
// Убрать указанные символы в конце строки, заданные фильтром
// filter - строка из убираемых символов
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
// -------------------------------------------------------------------
int _fastcall TLongString::RemoveLastSymbols(const char *filter)
{
  int size;
  if ((size = ::RemoveLastCharSymbols(Buffer, filter, getSize())) > 0)
    FreeSize = BufferSize - size;
  return size;
}

// -------------------------------------------------------------------
// Убрать кавычки из строки
// При ошибке возвращает 0
// -------------------------------------------------------------------
int TLongString::RemoveQuote()
{
  if (Buffer == 0)
    return 0;

  // Пример: "Озеро ""Глубокое""" -> Озеро "Глубокое"
  char *instr = Buffer;
  char *outstr = Buffer;

  while (*instr != 0)
  {
    if (*instr == '\"')  // Если найдены кавычки
    {
      instr++;             // Пропустить первые кавычки
      if (*instr == 0)
        break;
    }

    *outstr = *instr;      // Сохранить вторые кавычки или иной символ
    outstr++;
    instr++;
  }

  *outstr = 0;
  int size = (int)(outstr - Buffer);
  FreeSize = BufferSize - size;
  return 1;
}

// -------------------------------------------------------------------
// Заменить все заданные символы строки на указанный
// oldsymbol - заменяемый символ
// newsymbol - символ, на который заменяют
// При ошибке возвращает 0
// -------------------------------------------------------------------
int _fastcall TLongString::ReplaceSymbol(char oldsymbol, char newsymbol)
{
  return ::ReplaceCharSymbol(Buffer, getSize(), oldsymbol, newsymbol);
}

// -------------------------------------------------------------------
// Обновить часть строки по заданной позиции
// Копируется строка без замыкающего нуля, но не более count символов
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongString::updateString(const char * str, int start, int count)
{
  // Нельзя проверять на ноль первый байт, так как работа ведется не только
  // со строчными, но и с двоичными данными
  if ((str == 0) || (start < 0))
    return 0;

  if (count <= 0)
    count = (int)strlen(str);

  if ((Buffer == 0) || ((BufferSize-FreeSize) < (unsigned)(start+count)))
    return 0;

  memcpy(Buffer + start, str, count);

  return count;
}

// -------------------------------------------------------------------
// Обновить часть строки c заданной позиции и длины на новую строку и длину
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongString::updateString(char * oldstr, int oldsize, const char * newstr, int newsize)
{
  if ((Buffer == 0) || (newstr == 0) || (newsize <= 0))
    return 0;

  char * limit = Buffer + getSize();

  if ((Buffer > oldstr) || (limit < (oldstr + oldsize)) || (oldsize < 0))
    return 0;

  int delta = oldsize - newsize;
  if (delta > 0)
  {
    // Сжать строку
    memmove(oldstr, oldstr+delta, (int)(limit - (oldstr+delta)));
    FreeSize += delta;
  }
  else
  if (delta < 0)
  {
    delta = -delta;

    if (FreeSize < (unsigned int)delta)
    {
      int oldpos = (int)(oldstr - Buffer);

      if (allocateMemory(delta) == 0)
        return 0;

      limit  = Buffer + getSize();
      oldstr = Buffer + oldpos;
    }

    // Раздвинуть строку
    memmove(oldstr+delta, oldstr, (int)(limit - oldstr));
    FreeSize -= delta;
  }

  memcpy(oldstr, newstr, newsize);

  // Установить конечный ноль
  char * point = Buffer + (BufferSize - FreeSize);
  *point = 0;

  return 1;
}

//---------------------------------------------------------------------------
// Добавить текст с заменой всех вхождений заданного текста на указанный
//---------------------------------------------------------------------------
const char *_fastcall TLongString::AppendReplacedText(const char *source, const char *seek, const char *replace)
{
  if (source && *source)
  {
    if (seek && *seek && replace && *replace)
    {
      const size_t len_source  = strlen(source);
      const size_t len_seek    = strlen(seek);
      const size_t len_replace = strlen(replace);

      const char *begin = source;
      const char *end   = source + len_source;
      int delta = 0;
      while ((end = strstr(begin, seek)) != NULL)
      {
        delta = (int)(end-begin);
        // если искомый фрагмент в начале блока, то писать нечего
        if (delta)
          addValue(begin, (int)((delta)));
        // запись подменного текста
        addValue(replace, (int)(len_replace));
        begin += delta + len_seek;
      }
      end = source + len_source;
      // дописать окончание
      addValue(begin, (int)(end-begin));
    }
    else
      addValue(source);
    return Buffer;
  }
  return NULL;
}

//---------------------------------------------------------------------------
// Заменить текст в строке
//---------------------------------------------------------------------------
const char * _fastcall TLongString::ReplaceText(const char *oldvalue, const char *newvalue)
{
  if (Buffer && *Buffer && oldvalue && *oldvalue)
  {
    const int oldlen = (int)strlen(oldvalue);
    const int newlen = (int)(newvalue ? strlen(newvalue) : 0);
    char *finded = Buffer;
    while ((finded = strstr(finded, oldvalue)) != NULL)
    {
      if (newlen)
      {
        Update(finded, oldlen, newvalue, newlen);
        finded += newlen;
      }
      else
        Delete(finded, finded+oldlen);
    }
  }
  return Buffer;
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения строк в кодировке UTF-16
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Конструктор
// -------------------------------------------------------------------
TLongStringUn::TLongStringUn()
{
  Buffer     = 0;
  BufferSize = 0;
  FreeSize   = 0;
}

TLongStringUn::TLongStringUn(const WCHAR* wstr)
{
  Buffer = 0;
  BufferSize = 0;
  FreeSize = 0;

  if (wstr && *wstr)
    addValue(wstr);
}

TLongStringUn::TLongStringUn(const TLongStringUn &source)
{
  Buffer = 0;
  BufferSize = 0;
  FreeSize = 0;
  const WCHAR *pointer = source.Buffer;
  addValue(pointer);
}

// -------------------------------------------------------------------
// Освободить ресурсы
// -------------------------------------------------------------------
void TLongStringUn::freeMemory()
{
  if (Buffer != 0)
  {
    char * temp = (char *)Buffer;
    Buffer      = 0;
    BufferSize  = 0;
    FreeSize    = 0;
    delete [] temp;
  }
}

// -------------------------------------------------------------------
// Добавить нулевой символ
// Используется для массива строк, разделеных нулем
// Последующее добавление с помощью addValue запишет значение после нуля,
// то есть если после этого читать буфер как обычную строку, будет прочитан
// только первый фрагмент
// -------------------------------------------------------------------
void _fastcall TLongStringUn::AppendZeroSymbol()
{
  if ((Buffer == 0) || (FreeSize <= 4))
  {
    if (allocateMemory(256) == 0)
      return;
  }

  char * point = ((char*)Buffer) + (BufferSize - FreeSize);
  *(point)   = 0;
  *(point+1) = 0;
  *(point+2) = 0;
  *(point+3) = 0;
  FreeSize -= 2;
}

// -------------------------------------------------------------------
// Добавить данные
// При ошибке возвращает ноль
// -------------------------------------------------------------------
const WCHAR* _fastcall TLongStringUn::addValue(const WCHAR* str, int size)
{
  if (str == 0)
    return 0;
  if (size <= 0)
    size = (int)Wcslen(str) * sizeof(WCHAR);
  if (size == 0)
    return 0;

  if ((Buffer == 0) || (FreeSize <= (unsigned)(size+sizeof(WCHAR))))
  {
    if (allocateMemory(size+sizeof(WCHAR)) == 0)
      return 0;
  }

  char * point = ((char*)Buffer) + (BufferSize - FreeSize);
  memcpy(point, str, size);
  WCHAR * wpoint = (WCHAR *)(point + size);
  *wpoint = 0;
  FreeSize -= size;

  // Адрес записанной строки
  return (WCHAR*)point;
}

// -------------------------------------------------------------------
// Добавить строку в Unicode8 c преобразованием в Unicode16
// При ошибке возвращает ноль
// -------------------------------------------------------------------
const WCHAR* _fastcall TLongStringUn::addValue8(const char* str, int size)
{
  if (str == 0)
    return 0;
  if (size <= 0)
    size = (int)strlen(str);
  if (size == 0)
    return 0;

  // Вычисляем требуемый размер WCHAR-строки в байтах с нулевым символом
  unsigned int wstrSize = (size + 1) * sizeof(WCHAR);

  if ((Buffer == 0) || (FreeSize <= wstrSize))
    if (allocateMemory(wstrSize) == 0)
        return 0;

  WCHAR* point = (WCHAR*)(((char*)Buffer) + (BufferSize - FreeSize));

  // Преобразовываем строку в Unicode16 и запоминаем число записанных символов 
  int wcharCount = Unicode8ToUnicode(str, size, point, FreeSize);
  if (wcharCount > 0)
    FreeSize -= wcharCount * sizeof(WCHAR);

  // Адрес записанной строки
  return point;
}

// -------------------------------------------------------------------
// Добавить строку в ANSI/KOI-8 с преобразованием в Unicode16
// -------------------------------------------------------------------
//const WCHAR* _fastcall TLongStringUn::addString(const char* str, int size)
//{
//  if (str == 0)
//    return 0;
//  if (size <= 0)
//    size = (int)strlen(str);
//  if (size == 0)
//    return 0;

//  // Вычисляем требуемый размер WCHAR-строки в байтах с нулевым символом
//  unsigned int wstrSize = (size + 1) * sizeof(WCHAR);

//  if ((Buffer == 0) || (FreeSize <= wstrSize))
//    if (allocateMemory(wstrSize) == 0)
//      return 0;

//  WCHAR* point = (WCHAR*)(((char*)Buffer) + (BufferSize - FreeSize));

//  // Преобразовываем строку в Unicode16 и запоминаем число записанных символов
//  int wcharCount = StringToUnicode(str, size, point, FreeSize);
//  if (wcharCount > 0)
//    FreeSize -= wcharCount * sizeof(WCHAR);

//  // Адрес записанной строки
//  return point;
//}

// -------------------------------------------------------------------
// Выделить память под строку
// size - размер данных, которые нужно добавить к строке
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringUn::allocateMemory(unsigned int size)
{
  // Выделить память с запасом, чтобы не так часто выделять память
  if (Buffer != 0)
  {
    size += BufferSize;
    int delta = BufferSize >> 2;  // 1/4
    if (delta < MINSTRINGSIZE)
      delta = MINSTRINGSIZE;
    size += delta;

    // Размер, кратный 4 Кб, 16 байт в резерв
    size = ((size + 0x0FFF) & 0x0FFFFF000) - 16;
  }
  else
  {
    // Размер, кратный 4 Кб, 16 байт в резерв
    size = ((size + MINSTRINGSIZE + 0x0FFF) & 0x0FFFFF000) - 16;
  }

  char* point;
  try { point = new char[size + 16]; }
  catch (...) { point = 0; }
  if (point == 0)
    return 0;

  if (Buffer != 0)
  {
    // Сохранить прежнее значение строки
    memcpy(point, Buffer, BufferSize + sizeof(WCHAR));
    delete[] Buffer;
  }

  Buffer = (WCHAR*)point;

  FreeSize += (size - BufferSize);
  BufferSize = size;

  // Установить конечный ноль
  WCHAR * zeropoint = (WCHAR*)(((char*)Buffer) + (BufferSize - FreeSize));
  *zeropoint = 0;

  return 1;
}

// -------------------------------------------------------------------
// Добавить в конец пути слэш
// -------------------------------------------------------------------
int _fastcall TLongStringUn::CompleteThePath()
{
  if (Buffer && *Buffer)
  {
    unsigned int length = getLength();
#ifdef WIN32API
    if (Buffer[length-1] != '\\')
      return (addValue(WTEXT("\\")) != 0);
#else
    if (Buffer[length-1] != '/')
      return (addValue(WS("/")) != 0);
#endif
  }
  return 0;
}

// -------------------------------------------------------------------
// Обрезать строку до заданной позиции
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringUn::cutString(const WCHAR* end)
{
  unsigned int size = getSize();

  if ((Buffer == 0) || (end < Buffer) || (end > (Buffer + size)))
    return 0;

  FreeSize = BufferSize - (int)(((char*)end) - ((char*)Buffer));

  // Установить конечный ноль
  WCHAR* point = (WCHAR*)(((char*)Buffer) + (BufferSize - FreeSize));
  *point = 0;

  return 1;
}


//---------------------------------------------------------------------------
// Обрезать текст там, где встретится заданный фрагмент
//---------------------------------------------------------------------------
int _fastcall TLongStringUn::CropText(const WCHAR *seek)
{
  if (Buffer && *Buffer && seek && *seek)
  {
    WCHAR *fpos = Wcsstr(Buffer, seek);
    if (fpos)
      return cutString(fpos);
  }
  return 0;
}

// -------------------------------------------------------------------
// Удалить фрагмент строки
// begin - указатель на первый байт удаляемогно фрагмента
// end   - указатель на последний байт удаляемогно фрагмента
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringUn::deleteString(const WCHAR* begin, const WCHAR* end)
{
  if ((Buffer == 0) || (end <= begin))
    return 0;
  char* limit = ((char*)Buffer) + getSize();

  if ((begin < Buffer) || (((char*)begin) >= limit) || (end <= Buffer))
    return 0;

  if (((char*)end) >= limit)
    return cutString(begin);

  int copy = (int)(limit - ((char*)end));
  if (copy > 0)
    memmove((char*)begin, end + 1, copy);

  int delta = (int)(((char*)end) - ((char*)begin) + 1);
  FreeSize += delta;
  if (FreeSize > BufferSize)
    FreeSize = BufferSize;

  // Установить конечный ноль
  WCHAR* point = (WCHAR*)(((char*)Buffer) + (BufferSize - FreeSize));
  *point = 0;

  return 1;
}

// -------------------------------------------------------------------
// Обновить часть строки по заданной позиции
// Копируется строка без замыкающего нуля, но не более count символов
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringUn::updateString(const WCHAR *str, int start, int count)
{
  // Нельзя проверять на ноль первый байт, так как работа ведется не только
  // со строчными, но и с двоичными данными
  if ((str == 0) || (start < 0))
    return 0;

  if (count <= 0)
    count = (int)Wcslen(str);

  if ((Buffer == 0) || ((BufferSize-FreeSize) < (unsigned int)((start+count)*sizeof(WCHAR))))
    return 0;

  memcpy(&Buffer[start], (char*)str, count*sizeof(WCHAR));

  return count;
}

// -------------------------------------------------------------------
// Обновить часть строки c заданной позиции и длины на новую строку и длину
// (длина в байтах!)
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringUn::updateString(WCHAR *oldstr, int oldsize, const WCHAR *newstr, int newsize)
{
  if ((Buffer == 0) || (newstr == 0) || (newsize <= 0))
    return 0;

  char *limit = (char*)Buffer + getSize();

  if ((Buffer > oldstr) || (limit < ((char*)oldstr + oldsize)) || (oldsize < 0))
    return 0;

  int delta = oldsize - newsize;
  if (delta > 0)
  {
    // Сжать строку
    memmove((char*)oldstr, (char *)oldstr+delta, (int)(limit - ((char *)oldstr+delta)));
    FreeSize += delta;
  }
  else
    if (delta < 0)
    {
      delta = -delta;

      if (FreeSize < (unsigned int)delta)
      {
        int oldpos = (int)(oldstr - Buffer);

        if (allocateMemory(delta) == 0)
          return 0;

        limit  = (char *)Buffer + getSize();
        oldstr = Buffer + oldpos;
      }

      // Раздвинуть строку
      memmove((char *)oldstr+delta, (char *)oldstr, (int)(limit - (char *)oldstr));
      FreeSize -= delta;
    }

  memcpy((char *)oldstr, (char *)newstr, newsize);

  // Установить конечный ноль
  WCHAR* point = (WCHAR*)(((char*)Buffer) + (BufferSize - FreeSize));
  *point = 0;

  return 1;
}


// -------------------------------------------------------------------
// Запросить текущую позицию
// -------------------------------------------------------------------
WCHAR * _fastcall TLongStringUn::getCurrentPoint()
{
  if (Buffer == 0)
    return 0;

  char * point = ((char*)Buffer) + (BufferSize - FreeSize);

  return (WCHAR *)point;
}

// -------------------------------------------------------------------
// Зарезервировать место в записи
// -------------------------------------------------------------------
WCHAR* _fastcall TLongStringUn::getPlace(int size)
{
  WCHAR* point;

  if ((Buffer != 0) && (FreeSize >= (unsigned)size))
  {
    point = getCurrentPoint();
  }
  else
  {
    int delta = size - FreeSize;
    if (delta < 0)
      delta = 64;

    if (allocateMemory(delta) == 0)
      return 0;
    point = getCurrentPoint();
  }

  FreeSize -= size;
  return point;
}

// -------------------------------------------------------------------
// Проверить, что в строке только числа
// -------------------------------------------------------------------
int _fastcall TLongStringUn::IsStringDigit()
{
  if (Buffer == 0)
    return 0;

  return ::IsStringDigit(Buffer);
}

// -------------------------------------------------------------------
// Записать часть строки по заданной позиции
// Если буфер меньше заданной позиции, то он будет автоматически расширен,
// промежуточная память будет содержать случайные значения
// Копируется строка без замыкающего нуля, но не более strsize байт
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringUn::putString(const WCHAR* str, int start, int strsize)
{
  if (str == 0)
    return 0;
  if (strsize <= 0)
    strsize = (int)(Wcslen(str) * sizeof(WCHAR));

  if (strsize == 0)
    return 0;

  if ((Buffer == 0) || (BufferSize < (unsigned)(start + strsize)))
  {
    if (allocateMemory(start + strsize - BufferSize) == 0)
      return 0;
  }

  unsigned int size = BufferSize - FreeSize;

  if (size < (unsigned)(start + strsize))
    FreeSize = BufferSize - (start + strsize);

  memcpy(((char*)Buffer) + start, str, strsize);
  return strsize;
}

// -------------------------------------------------------------------
// Убрать символ из строки
// symbol - убираемый символ
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
// -------------------------------------------------------------------
int _fastcall TLongStringUn::RemoveSymbol(WCHAR symbol)
{
  int size;
  if ((size = ::RemoveSymbol(Buffer, symbol)) > 0)
    FreeSize = BufferSize - size*sizeof(WCHAR);
  return size;
}

// -------------------------------------------------------------------
// Убрать кавычки из строки
// При ошибке возвращает 0
// -------------------------------------------------------------------
int TLongStringUn::RemoveQuote()
{
  if (Buffer == 0)
    return 0;

  // Пример: "Озеро ""Глубокое""" -> Озеро "Глубокое"
  WCHAR *instr = Buffer;
  WCHAR *outstr = Buffer;

  while (*instr != 0)
  {
    if (*instr == '\"')  // Если найдены кавычки
    {
      instr++;             // Пропустить первые кавычки
      if (*instr == 0)
        break;
    }

    *outstr = *instr;      // Сохранить вторые кавычки или иной символ
    outstr++;
    instr++;
  }

  *outstr = 0;
  int size = (int)(((char *)outstr) - ((char *)Buffer));
  FreeSize = BufferSize - size;
  return 1;
}

//-----------------------------------------------------------------
// Убрать указанные символы из строки из строки
// symbols - строка из убираемых символов (с завершающим нулем в конце)
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0, иначе - количество найденных символов
//-----------------------------------------------------------------
int _fastcall TLongStringUn::RemoveEachSymbol(const WCHAR *symbols)
{
  int size;
  if ((size = ::RemoveEachSymbol(Buffer, symbols)) > 0)
    FreeSize = BufferSize - size*sizeof(WCHAR);
  return size;
}

// -------------------------------------------------------------------
// Заменить указанные символы из строки из строки заданным символом
// instr   - входная строка
// symbols - строка из убираемых символов (с завершающим нулем в конце)
// При ошибке возвращает 0, иначе - количество найденных символов
// -------------------------------------------------------------------
int _fastcall TLongStringUn::ReplaceEachSymbol(const WCHAR *symbols, WCHAR newvalue)
{
  return ::ReplaceEachSymbol(Buffer, symbols, newvalue);
}

//-----------------------------------------------------------------
// Убрать указанные символы в конце строки, заданные фильтром
// filter - строка из убираемых символов
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0, иначе количество убранных символов
//-----------------------------------------------------------------
int _fastcall TLongStringUn::RemoveLastSymbols(const WCHAR *filter)
{
  int size;
  if ((size = ::RemoveLastSymbols(Buffer, filter, getSize()/sizeof(WCHAR))) > 0)
    FreeSize = BufferSize - size*sizeof(WCHAR);
  return size;

}

// -------------------------------------------------------------------
// Заменить все заданные символы строки на указанный
// oldsymbol - заменяемый символ
// newsymbol - символ, на который заменяют
// При ошибке возвращает 0
// -------------------------------------------------------------------
int _fastcall TLongStringUn::ReplaceSymbol(WCHAR oldsymbol, WCHAR newsymbol)
{
  return ::ReplaceSymbol(Buffer, getSize(), oldsymbol, newsymbol);
}

// -------------------------------------------------------------------
// Перевести строку в верхний регистр
// -------------------------------------------------------------------
void TLongStringUn::UpperCase()
{
  int count;
  if ((Buffer == 0) || ((count = BufferSize - FreeSize) == 0))
    return;

  WCHAR * point = Buffer;
  while(count-- > 0)
  {
    *point = ToUpperCase(*point);
    point++;
  }
}

//---------------------------------------------------------------------------
// Добавить текст с заменой всех вхождений заданного текста на указанный
//---------------------------------------------------------------------------
const WCHAR* _fastcall TLongStringUn::AppendReplacedText(const WCHAR *source, const WCHAR *seek, const WCHAR *replace)
{
  if (source && *source)
  {
    if (seek && *seek && replace && *replace)
    {
      const size_t len_source  = Wcslen(source);
      const size_t len_seek    = Wcslen(seek);
      const size_t len_replace = Wcslen(replace);

      const WCHAR *begin = source;
      const WCHAR *end   = source + len_source;
      int delta = 0;
      while ((end = Wcsstr(begin, seek)) != NULL)
      {
        delta = (int)(end-begin);
        // если искомый фрагмент в начале блока, то писать нечего
        if (delta)
          addValue(begin, (int)((delta)*sizeof(WCHAR)));
        // запись подменного текста
        addValue(replace, (int)(len_replace*sizeof(WCHAR)));
        begin += delta + len_seek;
      }
      end = source + len_source;
      // дописать окончание
      addValue(begin, (int)((end-begin)*sizeof(WCHAR)));
    }
    else
      addValue(source);
    return Buffer;
  }
  return NULL;
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива строк в кодировке ANSI или UTF8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Запросить значение по номеру, начиная с 1
// -------------------------------------------------------------------
const char* _fastcall TLongStringArray::GetValue(int number)
{
  if ((number <= 0) || number > StrList.Count())
    return 0;

  BIGINT point = (BIGINT)StrList.Item((unsigned int)number);
  if (point == 0)
    return 0;
  return (point + Buffer);
}

// -------------------------------------------------------------------
// Запросить значение по номеру, начиная с 1
// -------------------------------------------------------------------
int _fastcall TLongStringArray::GetValueUn(int number, WCHAR* value, int valsize)
{
  if ((number <= 0) || (number > StrList.Count()))
    return 0;

  const char * point = GetValue(number);
  if (point == 0)
    return 0;

  Unicode8ToUnicode(point, value, valsize);
  return 1;
}

// -------------------------------------------------------------------
// Обновить значение по номеру, начиная с 1
// -------------------------------------------------------------------
int _fastcall TLongStringArray::Update(int number, const WCHAR* newvalue)
{
  if ((number <= 0) || number > StrList.Count())
    return 0;

  char value[2048];
  UnicodeToUnicode8(newvalue, value, sizeof(value));
  return UpdatePoint(number, value);
}

// -------------------------------------------------------------------
// Обновить значение по номеру, начиная с 1
// -------------------------------------------------------------------
int _fastcall TLongStringArray::Update(int number, const char* newvalue)
{
  if ((number <= 0) || (number > StrList.Count()))
    return 0;
  return UpdatePoint(number, newvalue);
}

// -------------------------------------------------------------------
// Обновить значение по номеру, начиная с 1
// -------------------------------------------------------------------
int _fastcall TLongStringArray::UpdatePoint(int number, const char* value)
{
  const char* point = TLongString::Append(value);
  if (point)
  {
    StrList.Update(number, (HLISTITEM)(point - Buffer));
    TLongString::Append("\0", 1);
  }
  else
    StrList.Update(number, (HLISTITEM)0);

  return 1;
}

// -------------------------------------------------------------------
// Добавить строку, size - длина строки
// -------------------------------------------------------------------
int TLongStringArray::AddPoint(const char* value, int size)
{
  int ret = 0;

  // Чтобы смещение на первый элемент было ненулевым резервируем 8 байт
  if ((Buffer == 0) || (BufferSize == FreeSize))
    TLongString::Append("        ");
  const char* point = TLongString::Append(value, size);
  if (point)
  {
    ret = StrList.Append((HLISTITEM)(point - Buffer));
    TLongString::Append("\0", 1);
  }
  else
  {
    point = TLongString::Append("\0", 1);
    if (point)
      ret = StrList.Append((HLISTITEM)(point - Buffer));
  }

  return ret;
}

// -------------------------------------------------------------------
// Добавить строку
// -------------------------------------------------------------------
int _fastcall TLongStringArray::Add(const char* value, const char* separator, int valuelen)
{
  if ((separator == 0) || (value == 0) || (*value == 0))
  {
    return AddPoint(value, valuelen);
  }
  // массив строк например ввв,ааа
  else
  {
    int seplen = (int)strlen(separator);
    int stringlen;
    if (valuelen != 0)
      stringlen = valuelen;
    else
      stringlen = (int)strlen(value);

    const char* end = value + stringlen;
    const char* curval = value;
    while((end - curval) > 0)
    {
      const char* fpos = strstr(curval, separator);
      if (fpos != 0)
      {
        int size = (int)(fpos - curval);
        AddPoint(curval, size);
        curval = fpos + seplen;
      }
      else
      {
        AddPoint(curval);
        break;
      }
    }
  }

  return 1;
}

// -------------------------------------------------------------------
// Вставить запись в указанную позицию
// value  - адрес строки
// number - номер вставляемой записи
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringArray::Insert(const char* value, int size, unsigned int number)
{
  if (number > (unsigned int)StrList.Count())
    return AddPoint(value, size);

  // Чтобы смещение на первый элемент было ненулевым резервируем 8 байт
  if ((Buffer == 0) || (BufferSize == FreeSize))
    TLongString::Append("        ");

  HLISTITEM item = 0;
  const char* point = TLongString::Append(value, size);
  if (point)
  {
    item = (HLISTITEM)(point - Buffer);
    TLongString::Append("\0", 1);
  }

  return StrList.Insert(item, number);
}

// -------------------------------------------------------------------
// Очистить массив
// -------------------------------------------------------------------
void _fastcall TLongStringArray::ClearArray()
{
  clearString();
  StrList.Clear();
}

// -------------------------------------------------------------------
// Очистить массив
// -------------------------------------------------------------------
void _fastcall TLongStringArrayUn::ClearArray()
{
  clearString();
  StrList.Clear();
  Array.Clear();
}

// -------------------------------------------------------------------
// Запросить список указателей на строки
// -------------------------------------------------------------------
const WCHAR ** TLongStringArrayUn::List()
{
  Array.Clear();
  int count = Count();
  for(int i = 1; i <= count; i++)
  {
    const char * item = (const char *)Item(i);
    if (item == 0)
      return 0;
    Array.Append(item);
  }

  return (const WCHAR**)Array.List();
}

// -------------------------------------------------------------------
// Запросить значение по номеру, начиная с 1
// -------------------------------------------------------------------
const WCHAR* _fastcall TLongStringArrayUn::GetValueUn(int number)
{
  if ((number <= 0) || (number > StrList.Count()))
    return 0;

  BIGINT point = (BIGINT)StrList.Item(number);
  if (point == 0)
    return 0;
  return (const WCHAR*)(point + ((char*)Buffer));
}

int _fastcall TLongStringArrayUn::GetValueUn(int number, WCHAR* value, int valsize)
{
  if ((number <= 0) || (number > StrList.Count()))
    return 0;

  BIGINT point = (BIGINT)StrList.Item(number);
  if (point == 0)
    return 0;

  return (WcsCopy(value, (const WCHAR*)(point + ((char*)Buffer)), valsize) != 0);
}

// -------------------------------------------------------------------
// Запросить значение по номеру, начиная с 1
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUn::GetValue(int number, char* value, int valsize)
{
  if ((number <= 0) || (number > StrList.Count()))
    return 0;

  const WCHAR * point = GetValueUn(number);
  if (point == 0)
    return 0;

  UnicodeToUnicode8(point, value, valsize);
  return 1;
}

// -------------------------------------------------------------------
// Обновить значение по номеру, начиная с 1
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUn::Update(int number, const WCHAR* newvalue)
{
  if ((number <= 0) || (number > StrList.Count()))
    return 0;
  return UpdatePoint(number, newvalue);
}

// -------------------------------------------------------------------
// Обновить значение по номеру, начиная с 1
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUn::UpdatePoint(int number, const WCHAR* value)
{
  const WCHAR* point = TLongStringUn::Append(value);
  if (point)
  {
    StrList.Update(number, (HLISTITEM)(((char*)point)-((char*)Buffer)));
    AppendZeroSymbol();
  }
  else
    StrList.Update(number, (HLISTITEM)0);

  return 1;
}

// -------------------------------------------------------------------
// Добавить строку, size - длина строки
// -------------------------------------------------------------------
int TLongStringArrayUn::AddPoint(const WCHAR* value, int size)
{
  // Чтобы смещение на первый элемент было ненулевым резервируем 8 байт
  if ((Buffer == 0) || (BufferSize == FreeSize))
  {
    const WCHAR temp[4] = {0};
    TLongStringUn::Append(temp, sizeof(temp));
  }

  int ret = 0;
  const WCHAR* point = TLongStringUn::Append(value, size);
  if (point)
  {
    ret = StrList.Append((HLISTITEM)(((char*)point)-((char*)Buffer)));
    AppendZeroSymbol();
  }
  else
  {
    const WCHAR temp[1] = {0};
    point = TLongStringUn::Append(temp, sizeof(temp));
    if (point)
      ret = StrList.Append((HLISTITEM)(((char*)point)-((char*)Buffer)));
  }

  return ret;
}

// -------------------------------------------------------------------
// Добавить подстроку к крайней строке
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUn::Catenate(const WCHAR* value)
{
  if ((value == 0) || (*value == 0) || (StrList.Count() == 0))
    return 0;

  // Встать на нулевой символ
  char * point = ((char*)Buffer) + (BufferSize - FreeSize);
  if (point > ((char*)Buffer))
    point -= sizeof(WCHAR);

  // Записать строку без нуля
  int ret = putString(value, (int)(point - ((char*)Buffer)));

  // Записать нулевой символ
  AppendZeroSymbol();

  return ret;
}

// -------------------------------------------------------------------
// Добавить строку
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUn::Add8(const char* value, const WCHAR* separator)
{
  TLongStringUn value16;
  value16.addValue8(value);
  return Add(value16.getString(), separator);
}

int _fastcall TLongStringArrayUn::Add(const WCHAR* value, const WCHAR* separator)
{
  if ((separator == 0) || (value == 0) || (*value == 0))
  {
    return AddPoint(value);
  }
  // массив строк например ввв,ааа
  else
  {
    int seplen = (int)Wcslen(separator);
    const WCHAR* end = value + Wcslen(value);
    const WCHAR* curval = value;
    while((end - curval) > 0)
    {
      const WCHAR* fpos = Wcsstr(curval, separator);
      if (fpos)
      {
        int size = (int)(fpos - curval)*sizeof(WCHAR);
        AddPoint(curval, size);
        curval = fpos + seplen;
      }
      else
      {
        AddPoint(curval);
        break;
      }
    }
  }

  return 1;
}

// -------------------------------------------------------------------
// Добавить массив строк с разделителем delimiter и кавычками quote
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUn::Add(const WCHAR* value, WCHAR delimiter, WCHAR quote)
{
  if ((delimiter == 0) || (value == 0) || (*value == 0))
  {
    return AddPoint(value);
  }

  // массив строк например ввв,"а,а,а"
  const WCHAR* end = value + Wcslen(value);
  const WCHAR* curval;
  WCHAR str[1024];

  while (value < end)
  {
    // Если конец строки
    if (*value == '\n')
      return 1;

    // Если найден разделитель
    if (*value == delimiter)
    {
      AddPoint(WS("\0"), sizeof(WCHAR));
      value++;
      continue;  // Пропустить поле
    }

    // Если найдены начальные кавычки: Ориентир,"5, озеро ""Новое"""
    // (в CSV-файле кавычки внутри поля обозначаются двойными кавычками = "")
    if (*value == quote)
    {
      curval = Wcschr(value+1, quote);     // Найти КОНЕЧНЫЕ кавычки

      // Пока НЕ конец строки - пропустить все парные кавычки
      while (curval != 0)
      {
        curval++;                          // Пропустить КОНЕЧНЫЕ кавычки
        if (*curval != quote)              // Если не кавычки
          break;

        // Найдены начальные кавычки
        curval = Wcschr(curval+1, quote);  // Найти КОНЕЧНЫЕ кавычки
      }

      // Если НЕ конец строки
      if (curval != 0)
        curval = Wcschr(curval, delimiter);  // Найти разделитель
    }
    else
    {
      curval = Wcschr(value, delimiter);     // Найти разделитель
    }

    int len;
    if (curval)
    {
      len = (int)(curval - value + 1);
    }
    else
    {
      len = (int)Wcslen(value);
      if (value[len-1] != '\n')
        len++;  // Если конец строки
    }

    // Убрать кавычки из строки
    WcsCopy(str, value, len * sizeof(WCHAR));
    len = ::RemoveQuote(str, quote) + 1;
    AddPoint(str, len * sizeof(WCHAR));

    if (curval == 0)       // Если конец строки
      return 1;

    // Пропустить разделитель
    value = curval + 1;
  }

  return 1;
}

// -------------------------------------------------------------------
// Добавить строку, если такого значения еще нет в массиве строк
// -------------------------------------------------------------------
int _fastcall TLongStringArray::AddUnique(const WCHAR* value, const char* separator, int * exisnumber)
{
  char cvalue[4096];
  UnicodeToUnicode8(value, cvalue, sizeof(cvalue));
  return AddUnique(cvalue, separator, exisnumber);
}

// -------------------------------------------------------------------
// Добавить строку, если такого значения еще нет в массиве строк
// -------------------------------------------------------------------
int _fastcall TLongStringArray::AddUnique(const char* value, const char* separator, int * exisnumber)
{
  if ((separator == 0) || (value == 0) || (*value == 0))
  {
    int ret;
    if ((ret = Seek(value)) == 0)
      return AddPoint(value);
    if (exisnumber)
      *exisnumber = ret;
    return 0;
  }

  int ret = 0;

  // массив строк например ввв,ааа
  int seplen = (int)strlen(separator);
  const char* end = value + strlen(value);
  const char* curval = value;
  while((end - curval) > 0)
  {
    const char* fpos = strstr(curval, separator);
    if (fpos)
    {
      int size = (int)(fpos - curval);
      if (Seek(curval, size) == 0)
        ret |= AddPoint(curval, size);
      curval = fpos + seplen;
    }
    else
    {
      if (Seek(curval) == 0)
        ret |= AddPoint(curval);
      break;
    }
  }

  return ret;
}

// -------------------------------------------------------------------
// Добавить строку, если такого значения еще нет в массиве строк
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUn::AddUnique(const WCHAR* value, const WCHAR* separator, int* exisnumber)
{
  if ((separator == 0) || (value == 0) || (*value == 0))
  {
    int ret = Seek(value);
    if (ret == 0)
      return AddPoint(value);
    if (exisnumber)
      *exisnumber = ret;
    return 0;
  }

  int ret = 0;

  // массив строк например ввв,ааа
  int seplen = (int)Wcslen(separator);
  const WCHAR* end = value + Wcslen(value);
  const WCHAR* curval = value;
  while ((end - curval) > 0)
  {
    const WCHAR* fpos = Wcsstr(curval, separator);
    if (fpos)
    {
      int size = (int)(fpos - curval) * sizeof(WCHAR);
      if (Seek(curval) == 0)
        ret |= AddPoint(curval, size);
      curval = fpos + seplen;
    }
    else
    {
      if (Seek(curval) == 0)
        ret |= AddPoint(curval);
      break;
    }
  }

  return ret;
}

// -------------------------------------------------------------------
// Вставить запись в указанную позицию
// value  - адрес строки
// number - номер вставляемой записи
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUn::Insert(const WCHAR* value, int size, unsigned int number)
{
  if (number > (unsigned int)StrList.Count())
    return AddPoint(value, size);

  // Чтобы смещение на первый элемент было ненулевым резервируем 8 байт
  if ((Buffer == 0) || (BufferSize == FreeSize))
  {
    const WCHAR temp[4] = { 0 };
    TLongStringUn::Append(temp, sizeof(temp));
  }

  HLISTITEM item = 0;
  const WCHAR* point = TLongStringUn::Append(value, size);
  if (point)
  {
    item = (HLISTITEM)(((char*)point) - ((char*)Buffer));
    AppendZeroSymbol();
  }

  return StrList.Insert(item, number);
}

// -------------------------------------------------------------------
// Проверить, что заданного значения нет в массиве строк
// Возвращает номер строки или ноль
// -------------------------------------------------------------------
int _fastcall TLongStringArray::Seek(const char* value, int size)
{
  if ((value == 0) || (*value == 0))
    return 0;

  if (size == 0) size = (int)strlen(value);

  int count = Count();
  for (int i = 1; i <= count; i++)
  {
    const char* item = GetValue(i);
    if (item == 0)
      return 0;

    if (_strnicmp(item, value, size) == 0)
      return i;
  }

  return 0;
}

// -------------------------------------------------------------------
// Проверить, что заданного значения нет в массиве строк
// Возвращает номер строки или ноль
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUn::Seek(const WCHAR* value)
{
  if ((value == 0) || (*value == 0))
    return 0;

  int count = Count();
  for (int i = 1; i <= count; i++)
  {
    const WCHAR* item = GetValueUn(i);
    if (item == 0)
      return 0;

    if (WcsiCmp(item, value) == 0)
      return i;
  }

  return 0;
}

// -------------------------------------------------------------------
// Скопировать список строк из другого списка
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUn::Load(TLongStringArrayUn& array)
{
  Clear();

  int count = array.Count();
  for(int i = 1; i <= count; i++)
  {
    const WCHAR * value = array.GetValueUn(i);
    if (value == 0)
      return 0;

    if (Append(value) == 0)
      return 0;
  }

  return 1;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения списка массивов строк в кодировке UTF16
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Очистить массив
// -------------------------------------------------------------------
void TLongStringArrayUnList::Clear()
{
  int count = Count();
  for (int j = count; j > 0; j--)
  {
    TLongStringArrayType * item = Item(j);
    if (item)
      delete item;
  }

  TFastHandleList::Clear();
}

// -------------------------------------------------------------------
// Добавить строку, массив строк
// separators - список разделителей, например: WTEXT("\n\t,")
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUnList::AddWords(const WCHAR* value, const WCHAR* separators)
{
  if ((value == 0) || (*value == 0))
    return 0;

  TLongStringArrayType * item = 0;

  try { item = new TLongStringArrayType(); } catch(...) { item = 0; }
  if (item == 0)
    return 0;

  if ((separators != 0) && (*separators != 0))
  {
    const WCHAR * begin = value;
    while(*value != 0)
    {
      if (Check(*value, separators))
      {
        int size = (int)((const char*)value - (const char*)begin);
        if (size > 0)
        {
          int type = GetType(begin, size);

          if (item->Append(begin, size, type) == 0)
          {
            delete item;
            return 0;
          }
        }

        value++;
        begin = value;
        continue;
      }

      value++;
    }

    int size = (int)((const char*)value - (const char*)begin);
    if (size > 0)
    {
      int type = GetType(begin, size);

      if (item->Append(begin, size, type) == 0)
      {
        delete item;
        return 0;
      }
    }
  }
  else
  {
    if (item->Append(value) == 0)
    {
      delete item;
      return 0;
    }
  }

  return TFastHandleList::Append((HLISTITEM)item);
}

// -------------------------------------------------------------------
// Сравнить текущий символ со списком разделителей
// При совпадении возвращает ненулевое значение
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUnList::Check(WCHAR value, const WCHAR* separators)
{
  if (separators == 0)
    return 0;

  while(*separators != 0)
  {
    if (value == *separators)
      return 1;
    separators++;
  }

  return 0;
}

// -------------------------------------------------------------------
// Определить тип строки - число или текст
// При совпадении возвращает ненулевое значение
// -------------------------------------------------------------------
int _fastcall TLongStringArrayUnList::GetType(const WCHAR * value, int size)
{
  if ((value == 0) || (*value == 0))
    return 0;

  size /= sizeof(WCHAR);

  int delta = size;

  while((*value != 0) && (delta-- > 0))
  {
    if ((*value < '0') || (*value > '9'))
      return 0;
    value++;
  }

  return size;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива отсортированных строк в кодировке ANSI или UTF8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Добавить строку, массив строк
// value     - адрес строки
// limit     - длина данных или 0
// separator - разделитель или 0
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringList::AddPoint(const char* value, int size)
{
  if ((value == 0) || (*value == 0))
    return TLongStringArray::Insert(value, size, 1);

  int count = Count();
  if (count == 0)
    return TLongStringArray::AddPoint(value, size);

  // Найти элемент с меньшим значением
  unsigned int number = count + 1;

  // Проверить крайние номера в списке (1 и count)
  for (int i = count; i > 0; i -= (count - 1))
  {
    const char* point = TLongStringArray::Item(i);
    if (point)
    {
      int result = _stricmp(point, value);
      if (result < 0)
      {
        if (i == count)
          return TLongStringArray::AddPoint(value, size);
      }
      else
        if (result > 0)
        {
          // Добавляемое значение меньше одной из границ
          if (i == 1)
            // Вставить в начало
            return TLongStringArray::Insert(value, size, 1);
          else
            if (count == 2)
            {
              // В списке всего 2 значения - нужно вставить значение тут
              // Проверить, что первый элемент меньше добавляемого
              point = TLongStringArray::Item(1);
              result = _stricmp(point, value);
              if (result < 0)
                // Больше 1-го значения, вставить перед последним значением
                return TLongStringArray::Insert(value, size, 2);

              // Меньше или равно 1-го значения, вставить в начало
              return TLongStringArray::Insert(value, size, 1);
            }
        }
        else
        {
          // Совпадение значений
          return TLongStringArray::Insert(value, size, i);
        }
    }
  }

  // Устанавливаем первоначальные границы диапазона
  unsigned int first = 2;
  unsigned int last = count - 1;
  count = last - first + 1;

  // Поиск методом половинного деления
  while (count > 0)
  {
    if (count < 9)
    {
      number = last + 1;
      for (unsigned int i = first; i <= last; i++)
      {
        const char* point = TLongStringArray::Item(i);
        int result = _stricmp(point, value);
        if (result > 0)
        {
          number = i;
          break;
        }

        if (result == 0)
          // Совпадение значений
          return TLongStringArray::Insert(value, size, i);
      }

      break;
    }

    unsigned int temp = (last + first) >> 1;
    const char* point = TLongStringArray::Item(temp);
    int result = _stricmp(point, value);

    if (result == 0)
      // Совпадение значений
      return TLongStringArray::Insert(value, size, temp);

    if (result > 0)
    {
      point = TLongStringArray::Item(temp - 1);
      result = _stricmp(point, value);
      if (result == 0)
      {
        number = temp - 1;
        break;
      }
      if (result < 0)
      {
        number = temp;
        break;
      }
    }

    if (result > 0)
      last = temp - 1;
    else
      first = temp + 1;

    count = last - first + 1;
  }

  return TLongStringArray::Insert(value, size, number);
}

// -------------------------------------------------------------------
// Добавить строку
// -------------------------------------------------------------------
int _fastcall TLongStringList::Add(const char* value, const char* separator, int valuelen)
{
  if (separator == 0)
  {
    if (valuelen != 0)
      return AddPoint(value, valuelen);
    else
      return AddPoint(value);
  }
  // массив строк например ввв,ааа
  else
  {
    int seplen = (int)strlen(separator);
    int stringlen;
    if (valuelen != 0)
      stringlen = valuelen;
    else
      stringlen = (int)strlen(value);

    const char* end = value + stringlen;
    const char* curval = value;
    while((end - curval) > 0)
    {
      const char* fpos = strstr(curval, separator);
      if (fpos != 0)
      {
        int size = (int)(fpos - curval);
        AddPoint(curval, size);
        curval = fpos + seplen;
      }
      else
      {
        AddPoint(curval);
        break;
      }
    }
  }

  return 1;
}

// -------------------------------------------------------------------
// Добавить строку в массив строк
// -------------------------------------------------------------------
int _fastcall TLongStringList::Add(const WCHAR* value, const char* separator, int size)
{
  char cvalue[4096]; cvalue[0] = 0;
  UnicodeToUnicode8(value, cvalue, sizeof(cvalue));
  return Add(cvalue, separator, size);
}

// -------------------------------------------------------------------
// Добавить строку, если такого значения еще нет в массиве строк
// -------------------------------------------------------------------
int _fastcall TLongStringList::AddUnique(const WCHAR* value, const char* separator, int * existnumber)
{
  char cvalue[4096]; cvalue[0] = 0;
  UnicodeToUnicode8(value, cvalue, sizeof(cvalue));
  return AddUnique(cvalue, separator, existnumber);
}

// -------------------------------------------------------------------
// Добавить строку, если такого значения еще нет в массиве строк
// -------------------------------------------------------------------
int _fastcall TLongStringList::AddUnique(const char* value, const char* separator, int * existnumber)
{
  if ((separator == 0) || (value == 0) || (*value == 0))
  {
    int oldnumber;
    if ((oldnumber = Seek(value)) == 0)
      return AddPoint(value);
    if (existnumber)
      *existnumber = oldnumber;
    return 0;
  }

  int ret = 0;

  // массив строк например ввв,ааа
  int seplen = (int)strlen(separator);
  const char* end = value + strlen(value);
  const char* curval = value;
  while((end - curval) > 0)
  {
    const char* fpos = strstr(curval, separator);
    if (fpos)
    {
      int size = (int)(fpos - curval);
      if (Seek(curval, size) == 0)
        ret |= AddPoint(curval, size);
      curval = fpos + seplen;
    }
    else
    {
      if (Seek(curval) == 0)
        ret |= AddPoint(curval);
      break;
    }
  }

  return ret;
}

// -------------------------------------------------------------------
// Найти заданную строку в массиве строк
// value     - адрес строки
// size      - длина данных или 0
// Возвращает номер строки или ноль
// -------------------------------------------------------------------
int _fastcall TLongStringList::Seek(const char* value, int size)
{
  char temp[256];
  if (size > 0)
  {
    size++;
    if (size >= sizeof(temp))
      size = sizeof(temp);
    StrCopy(temp, value, size);
    value = temp;
  }

  int count = Count();

  // Проверить крайние номера в списке (1 и count)
  for (int i = count; i > 0; i -= (count - 1))
  {
    const char* point = Item(i);
    if (point)
    {
      int result = _stricmp(point, value);
      if (result < 0)
      {
        if (i == count)
          return 0;
      }
      else
        if (result > 0)
        {
          if (i == 1)
            return 0;
        }
        else
          return i;
    }
  }

  // Устанавливаем первоначальные границы диапазона
  unsigned int first = 2;
  unsigned int last = count - 1;
  count = last - first + 1;

  // Поиск методом половинного деления
  while (count > 0)
  {
    if (count < 9)
    {
      for (unsigned int i = first; i <= last; i++)
      {
        const char* point = Item(i);
        int result = _stricmp(point, value);
        if (result == 0)
          return i;

        if (result > 0)
          break;
      }

      break;
    }

    unsigned int temp = (last + first) >> 1;
    const char* point = Item(temp);
    int result = _stricmp(point, value);
    if (result == 0)
      return temp;

    if (result > 0)
      last = temp - 1;
    else
      first = temp + 1;

    count = last - first + 1;
  }

  return 0;
}

// -------------------------------------------------------------------
// Найти заданную строку в массиве строк
// value     - адрес строки
// Возвращает номер строки или ноль
// -------------------------------------------------------------------
int _fastcall TLongStringList::Find(const WCHAR * value)
{
  char cvalue[4096];
  UnicodeToUnicode8(value, cvalue, sizeof(cvalue));
  return Seek(cvalue);
}

// -------------------------------------------------------------------
// Обновить значение
// number - номер элемента от 1 до Count()
// После обновления позиция элемента может измениться в соответствии с сортировкой
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringList::Update(int number, const char* newvalue)
{
  if ((number <= 0) || number > (StrList.Count()))
    return 0;

  // Удалить старое значение
  Delete(number);

  // Добавить новое значение с учетом сортировки
  return AddPoint(newvalue);
}

// -------------------------------------------------------------------
// Обновить значение по номеру, начиная с 1
// -------------------------------------------------------------------
int _fastcall TLongStringList::Update(int number, const WCHAR* newvalue)
{
  if ((number <= 0) || number > (StrList.Count()))
    return 0;

  char value[MAX_PATH_LONG];
  UnicodeToUnicode8(newvalue, value, sizeof(value));
  return Update(number, value);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива отсортированных строк в кодировке UTF16
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Добавить строку, массив строк
// value     - адрес строки
// limit     - длина данных или 0
// separator - разделитель или 0
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringListUn::AddPoint(const WCHAR* value, int size)
{
  if ((value == 0) || (*value == 0))
    return TLongStringArrayUn::Insert(value, size, 1);

  int count = Count();
  if (count == 0)
    return TLongStringArrayUn::AddPoint(value, size);

  // Найти элемент с меньшим значением
  unsigned int number = count + 1;

  // Проверить крайние номера в списке (1 и count)
  for (int i = count; i > 0; i -= (count - 1))
  {
    const WCHAR* point = TLongStringArrayUn::Item(i);
    if (point)
    {
      int result = Wcsicmp(point, value);
      if (result < 0)
      {
        if (i == count)
          return TLongStringArrayUn::AddPoint(value, size);
      }
      else
        if (result > 0)
        {
          // Добавляемое значение меньше одной из границ
          if (i == 1)
            // Вставить в начало
            return TLongStringArrayUn::Insert(value, size, 1);
          else
            if (count == 2)
            {
              // В списке всего 2 значения - нужно вставить значение тут
              // Проверить, что первый элемент меньше добавляемого
              point = TLongStringArrayUn::Item(1);
              result = Wcsicmp(point, value);
              if (result < 0)
                // Больше 1-го значения, вставить перед последним значением
                return TLongStringArrayUn::Insert(value, size, 2);

              // Меньше или равно 1-го значения, вставить в начало
              return TLongStringArrayUn::Insert(value, size, 1);
            }
        }
        else
        {
          // Совпадение значений
          return TLongStringArrayUn::Insert(value, size, i);
        }
    }
  }

  // Устанавливаем первоначальные границы диапазона
  unsigned int first = 2;
  unsigned int last = count - 1;
  count = last - first + 1;

  // Поиск методом половинного деления
  while (count > 0)
  {
    if (count < 9)
    {
      number = last + 1;
      for (unsigned int i = first; i <= last; i++)
      {
        const WCHAR* point = TLongStringArrayUn::Item(i);
        int result = Wcsicmp(point, value);
        if (result > 0)
        {
          number = i;
          break;
        }

        if (result == 0)
          // Совпадение значений
          return TLongStringArrayUn::Insert(value, size, i);
      }

      break;
    }

    unsigned int temp = (last + first) >> 1;
    const WCHAR* point = TLongStringArrayUn::Item(temp);
    int result = Wcsicmp(point, value);

    if (result == 0)
      // Совпадение значений
      return TLongStringArrayUn::Insert(value, size, temp);

    if (result > 0)
    {
      point = TLongStringArrayUn::Item(temp - 1);
      result = Wcsicmp(point, value);
      if (result == 0)
      {
        number = temp - 1;
        break;
      }
      if (result < 0)
      {
        number = temp;
        break;
      }
    }

    if (result > 0)
      last = temp - 1;
    else
      first = temp + 1;

    count = last - first + 1;
  }

  return TLongStringArrayUn::Insert(value, size, number);
}

// -------------------------------------------------------------------
// Добавить строку
// -------------------------------------------------------------------
int _fastcall TLongStringListUn::Add(const WCHAR* value, const WCHAR* separator, int valuelen)
{
  if (separator == 0)
  {
    if (valuelen != 0)
      return AddPoint(value, valuelen);
    else
      return AddPoint(value);
  }
  // массив строк например ввв,ааа
  else
  {
    int seplen = (int)Wcslen(separator);
    int stringlen;
    if (valuelen != 0)
      stringlen = valuelen;
    else
      stringlen = (int)Wcslen(value);

    const WCHAR* end = value + stringlen;
    const WCHAR* curval = value;
    while((end - curval) > 0)
    {
      const WCHAR* fpos = Wcsstr(curval, separator);
      if (fpos != 0)
      {
        int size = (int)(fpos - curval);
        AddPoint(curval, size);
        curval = fpos + seplen;
      }
      else
      {
        AddPoint(curval);
        break;
      }
    }
  }

  return 1;
}

// -------------------------------------------------------------------
// Добавить строку, если такого значения еще нет в массиве строк
// -------------------------------------------------------------------
int _fastcall TLongStringListUn::AddUnique(const WCHAR* value, const WCHAR* separator, int* existnumber)
{
  if ((separator == 0) || (value == 0) || (*value == 0))
  {
    int oldnumber;
    if ((oldnumber = Seek(value)) == 0)
      return AddPoint(value);
    if (existnumber)
      *existnumber = oldnumber;
    return 0;
  }

  int ret = 0;

  // массив строк например ввв,ааа
  int seplen = (int)Wcslen(separator);
  const WCHAR* end = value + Wcslen(value);
  const WCHAR* curval = value;
  while ((end - curval) > 0)
  {
    const WCHAR* fpos = Wcsstr(curval, separator);
    if (fpos)
    {
      int size = (int)(fpos - curval);
      if (Seek(curval, size) == 0)
        ret |= AddPoint(curval, size);
      curval = fpos + seplen;
    }
    else
    {
      if (Seek(curval) == 0)
        ret |= AddPoint(curval);
      break;
    }
  }

  return ret;
}

// -------------------------------------------------------------------
// Найти заданную строку в массиве строк
// value     - адрес строки
// size      - длина данных или 0
// Возвращает номер строки или ноль
// -------------------------------------------------------------------
int _fastcall TLongStringListUn::Seek(const WCHAR* value, int size)
{
  WCHAR temp[1024];
  if (size > 0)
  {
    size++;
    if (size >= sizeof(temp))
      size = sizeof(temp);
    WcsCopy(temp, value, size);
    value = temp;
  }

  int count = Count();

  // Проверить крайние номера в списке (1 и count)
  for (int i = count; i > 0; i -= (count - 1))
  {
    const WCHAR* point = Item(i);
    if (point)
    {
      int result = Wcsicmp(point, value);
      if (result < 0)
      {
        if (i == count)
          return 0;
      }
      else
        if (result > 0)
        {
          if (i == 1)
            return 0;
        }
        else
          return i;
    }
  }

  // Устанавливаем первоначальные границы диапазона
  unsigned int first = 2;
  unsigned int last = count - 1;
  count = last - first + 1;

  // Поиск методом половинного деления
  while (count > 0)
  {
    if (count < 9)
    {
      for (unsigned int i = first; i <= last; i++)
      {
        const WCHAR* point = Item(i);
        int result = Wcsicmp(point, value);
        if (result == 0)
          return i;

        if (result > 0)
          break;
      }

      break;
    }

    unsigned int temp = (last + first) >> 1;
    const WCHAR* point = Item(temp);
    int result = Wcsicmp(point, value);
    if (result == 0)
      return temp;

    if (result > 0)
      last = temp - 1;
    else
      first = temp + 1;

    count = last - first + 1;
  }

  return 0;
}

// -------------------------------------------------------------------
// Обновить значение
// number - номер элемента от 1 до Count()
// После обновления позиция элемента может измениться в соответствии с сортировкой
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringListUn::Update(int number, const WCHAR* newvalue)
{
  if ((number <= 0) || number > (StrList.Count()))
    return 0;

  // Удалить старое значение
  Delete(number);

  // Добавить новое значение с учетом сортировки
  return AddPoint(newvalue);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива отсортированных строк и id в кодировке ANSI или UTF8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Добавить строку, если такого значения еще нет в массиве строк
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringAndIdList::AddUnique(const char* value, int id, int* exisnumber, int* existid)
{
  TLongStringList::Lock();
  int oldnumber;
  int number = TLongStringList::AddUnique(value, 0, &oldnumber);
  if (number)
    IdList.Insert(number, id);
  else
  {
    if (exisnumber)
      *exisnumber = oldnumber;
    if (existid)
      *existid = ItemId(oldnumber);
  }
  TLongStringList::UnLock();
  return number;
}

// -------------------------------------------------------------------
// Добавить строку, если такого значения еще нет в массиве строк
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringAndIdList::AddUnique(const WCHAR* value, int id, int * exisnumber, int * existid)
{
  char cvalue[4096];
  UnicodeToUnicode8(value, cvalue, sizeof(cvalue));
  return AddUnique(cvalue, id, exisnumber, existid);
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения большого массива отсортированных строк в кодировке ANSI или UTF8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Добавить строку, если такого значения еще нет в массиве строк
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TLongStringBigList::AddUnique(const char* value, int id, int * exisnumber, int * existid)
{
  if ((value == 0) || (*value == 0))
    return 0;

  int size = (int)strlen(value);

  TLongStringAndIdList * list = GetListForSize(size);
  if (list == 0)
    return 0;

  return list->AddUnique(value, id, exisnumber, existid);
}

// -------------------------------------------------------------------
// Запросить список строк заданной длины
// При ошибке возвращает ноль
// -------------------------------------------------------------------
TLongStringAndIdList * _fastcall TLongStringBigList::GetListForSize(int size)
{
  Lock();

  int number = SizeList.Find(size);
  if (number == 0)
  {
    TLongStringAndIdList * item;
    try { item = new TLongStringAndIdList(); } catch(...) { item = 0; }
    number = SizeList.Append(size);
    if (number)
    {
      if (THandleList::Insert((HLISTITEM)item, number) == 0)
      {
        SizeList.Delete(number);
        UnLock();
        return 0;
      }

      UnLock();
      return item;
    }

    UnLock();
    return 0;
  }

  TLongStringAndIdList * item = (TLongStringAndIdList *)THandleList::Item(number);
  UnLock();
  return item;
}

// -------------------------------------------------------------------
// Очистить списки строк
// -------------------------------------------------------------------
void _fastcall TLongStringBigList::Clear()
{
  Lock();

  SizeList.Clear();

  int count = THandleList::Count();
  for (int j = count; j > 0; j--)
  {
    TLongStringAndIdList * list = (TLongStringAndIdList *)THandleList::Item(j);
    if (list)
      delete list;
  }

  THandleList::Clear();

  UnLock();
}


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++   Список названий и ключей    ++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Добавить запись
// При ошибке возвращает ноль
// -------------------------------------------------------------------
unsigned int _fastcall TKeyNames::Append(const WCHAR * key, const WCHAR * name)
{
  if ((key == 0) || (*key == 0))
    return 0;

  const char * point = (const char *)LongString.Append(key);
  if (point == 0)
    return 0;

  // Начальный адрес может меняться при добавлении строк
  const char * begin = (const char *)LongString.getString();

  KEYNAME item;
  item.Key = (unsigned int)(point - begin);
  LongString.AppendZeroSymbol();

  if ((name == 0) || (*name == 0))
    point = 0;
  else
    point = (const char *) LongString.Append(name);
  if (point == 0)
    item.Name = 0;
  else
  {
    begin     = (const char *)LongString.getString();
    item.Name = (unsigned int)(point - begin);
    LongString.AppendZeroSymbol();
  }

  return TSimpleList::Append((char*)&item, sizeof(KEYNAME));
}

// -------------------------------------------------------------------
// Найти запись по ключу
// -------------------------------------------------------------------
KEYNAME* _fastcall TKeyNames::Find(const WCHAR* key)
{
  if ((key == 0) || (*key == 0))
    return 0;

  // Начальный адрес может меняться при добавлении строк
  const char* begin = (const char*)LongString.getString();
  if (begin == 0)
    return 0;

  int count = Count();
  for (int i = 1; i <= count; i++)
  {
    KEYNAME* item = Item(i);
    if (item == 0)
      break;

    const WCHAR* point = (const WCHAR*)(begin + item->Key);
    if (WcsiCmp(point, key) == 0)
      return item;
  }

  return 0;
}

// -------------------------------------------------------------------
// Очистить списки
// -------------------------------------------------------------------
void TKeyNames::Clear()
{
  TSimpleList::Clear();
  LongString.Clear();
}

// -------------------------------------------------------------------
// Запросить указатель на имя слоя по ключу
// -------------------------------------------------------------------
const WCHAR* _fastcall TKeyNames::FindName(const WCHAR* key)
{
  KEYNAME* item = Find(key);
  if (item == 0)
    return 0;

  // Начальный адрес может меняться при добавлении строк
  const char* begin = (const char*)LongString.getString();
  if (begin == 0)
    return 0;

  return (const WCHAR*)(begin + item->Name);
}

// -------------------------------------------------------------------
// Найти запись по ключу
// -------------------------------------------------------------------
unsigned int _fastcall TKeyNames::FindNumber(const WCHAR* key)
{
  if ((key == 0) || (*key == 0))
    return 0;

  // Начальный адрес может меняться при добавлении строк
  const char* begin = (const char*)LongString.getString();
  if (begin == 0)
    return 0;

  int count = Count();
  for (int i = 1; i <= count; i++)
  {
    KEYNAME* item = Item(i);
    if (item == 0)
      break;

    const WCHAR* point = (const WCHAR*)(begin + item->Key);
    if (WcsiCmp(point, key) == 0)
      return i;
  }

  return 0;
}

// -------------------------------------------------------------------
// Запросить ключ по порядковому номеру в списке
// number - номер элемента, начиная с 1
// -------------------------------------------------------------------
const WCHAR * _fastcall TKeyNames::GetKey(unsigned int number)
{
  KEYNAME * item = Item(number);
  if (item == 0)
    return 0;

  const char * begin = (const char *)LongString.getString();
  if (begin == 0)
    return 0;

  return (const WCHAR *)(begin + item->Key);
}

// -------------------------------------------------------------------
// Запросить имя по порядковому номеру в списке
// number - номер элемента, начиная с 1
// -------------------------------------------------------------------
const WCHAR * _fastcall TKeyNames::GetName(unsigned int number)
{
  KEYNAME * item = Item(number);
  if ((item == 0) || (item->Name == 0))
    return 0;

  const char * begin = (const char *)LongString.getString();
  if (begin == 0)
    return 0;

  return (const WCHAR *)(begin + item->Name);
}

// -------------------------------------------------------------------
// Скопировать содержимое списка
// -------------------------------------------------------------------
int _fastcall TKeyNames::Load(TKeyNames * source)
{
  Clear();

  if (source == 0)
    return 0;

  int count = source->Count();
  for(int i = 1; i <= count; i++)
  {
    // Запросить ключ по порядковому номеру в списке
    const WCHAR * key = source->GetKey(i);

    // Запросить имя по порядковому номеру в списке
    const WCHAR * name = source->GetName(i);

    Append(key, name);
  }

  return Count();
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++   Список названий и ключей    ++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Добавить запись
// При ошибке возвращает ноль
// -------------------------------------------------------------------
unsigned int _fastcall TKeyNamesA::Append(const char * key, const char * name)
{
  if (key == 0)
    return 0;

  const char * point = (const char *)LongString.Append(key);
  if (point == 0)
    return 0;

  // Начальный адрес может меняться при добавлении строк
  const char * begin = (const char *)LongString.getString();

  KEYNAME item;
  item.Key = (unsigned int)(point - begin);
  LongString.AppendZeroSymbol();

  if ((name == 0) || (*name == 0))
    point = 0;
  else
    point = (const char *) LongString.Append(name);
  if (point == 0)
    item.Name = 0;
  else
  {
    begin     = (const char *)LongString.getString();
    item.Name = (unsigned int)(point - begin);
    LongString.AppendZeroSymbol();
  }

  return TSimpleList::Append((char*)&item, sizeof(KEYNAME));
}

// -------------------------------------------------------------------
// Найти запись по ключу
// -------------------------------------------------------------------
KEYNAME* _fastcall TKeyNamesA::Find(const char* key)
{
  if ((key == 0) || (*key == 0))
    return 0;

  // Начальный адрес может меняться при добавлении строк
  const char* begin = (const char*)LongString.getString();
  if (begin == 0)
    return 0;

  int count = Count();
  for (int i = 1; i <= count; i++)
  {
    KEYNAME* item = Item(i);
    if (item == 0)
      break;

    const char* point = (const char*)(begin + item->Key);
    if (_stricmp(point, key) == 0)
      return item;
  }

  return 0;
}

// -------------------------------------------------------------------
// Очистить списки
// -------------------------------------------------------------------
void TKeyNamesA::Clear()
{
  TSimpleList::Clear();
  LongString.Clear();
}

// -------------------------------------------------------------------
// Оcвободить память
// -------------------------------------------------------------------
void TKeyNamesA::Close()
{
  TSimpleList::Close();
  LongString.freeMemory();
}

// -------------------------------------------------------------------
// Запросить указатель на имя слоя по ключу
// -------------------------------------------------------------------
const char * _fastcall TKeyNamesA::FindName(const char * key)
{
  KEYNAME * item = Find(key);
  if (item == 0)
    return 0;

  // Начальный адрес может меняться при добавлении строк
  const char * begin = (const char *)LongString.getString();
  if (begin == 0)
    return 0;

  return (const char *)(begin + item->Name);
}

// -------------------------------------------------------------------
// Найти запись по ключу
// -------------------------------------------------------------------
unsigned int _fastcall TKeyNamesA::FindNumber(const char * key)
{
  if ((key == 0) || (*key == 0))
    return 0;

  // Начальный адрес может меняться при добавлении строк
  const char * begin = (const char *)LongString.getString();
  if (begin == 0)
    return 0;

  int count = Count();
  for(int i = 1; i <= count; i++)
  {
    KEYNAME * item = Item(i);
    if (item == 0)
       break;

    const char * point = (const char *)(begin + item->Key);
    if (_stricmp(point, key) == 0)
      return i;
  }

  return 0;
}

// -------------------------------------------------------------------
// Запросить ключ по порядковому номеру в списке
// number - номер элемента, начиная с 1
// -------------------------------------------------------------------
const char * _fastcall TKeyNamesA::GetKey(unsigned int number)
{
  KEYNAME * item = Item(number);
  if (item == 0)
    return 0;

  const char * begin = (const char *)LongString.getString();
  if (begin == 0)
    return 0;

  return (const char *)(begin + item->Key);
}

// -------------------------------------------------------------------
// Запросить имя по порядковому номеру в списке
// number - номер элемента, начиная с 1
// -------------------------------------------------------------------
const char * _fastcall TKeyNamesA::GetName(unsigned int number)
{
  KEYNAME * item = Item(number);
  if ((item == 0) || (item->Name == 0))
    return 0;

  const char * begin = (const char *)LongString.getString();
  if (begin == 0)
    return 0;

  return (const char *)(begin + item->Name);
}

// -------------------------------------------------------------------
// Обновить имя по порядковому номеру в списке
// number - номер элемента, начиная с 1
// name   - новое значение имени
// При ошибке возвращает ноль
// -------------------------------------------------------------------
int _fastcall TKeyNamesA::UpdateName(unsigned int number, const char * name)
{
  if (name == 0)
    return 0;

  KEYNAME * item = Item(number);
  if (item == 0)
    return 0;

  char * begin = (char *)LongString.getString();
  if (begin == 0)
    return 0;

  if (item->Name > 0)
  {
    char * oldname = (begin + item->Name);
    int size = (int)strlen(oldname);
    if (size >= (int)strlen(name))
      return (StrCopy(oldname, name, size+1) != 0);
  }

  const char * point = (const char *) LongString.Append(name);
  if (point == 0)
    item->Name = 0;
  else
  {
    begin      = (char *)LongString.getString();
    item->Name = (unsigned int)(point - begin);
    LongString.AppendZeroSymbol();
  }

  return 1;
}

// -------------------------------------------------------------------
// Скопировать содержимое списка
// -------------------------------------------------------------------
int _fastcall TKeyNamesA::Load(TKeyNamesA * source)
{
  Clear();

  if (source == 0)
    return 0;

  int count = source->Count();
  for(int i = 1; i <= count; i++)
  {
    // Запросить ключ по порядковому номеру в списке
    const char * key = source->GetKey(i);

    // Запросить имя по порядковому номеру в списке
    const char * name = source->GetName(i);

    Append(key, name);
  }

  return Count();
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++   Список файлов  +++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Проверить, что имени файла с заданным расширением нет в массиве строк
// ext - расширение имени файла с точкой (".sxf" и т.п.)
// Возвращает номер строки или ноль
// -------------------------------------------------------------------
int _fastcall TFileList::FindExt(const WCHAR * ext, int first)
{
  int count = Count();

  // Запросить значение по номеру, начиная с 1
  for(int i = first; i <= count; i++)
  {
    const WCHAR * value = GetValueUn(i);
    if (value == 0)
      break;

    WCHAR valueext[MAX_PATH]; valueext[0] = 0;
    SplitThePath(value, 0, 0, 0, valueext);
    if (WcsiCmp(value, ext) == 0)
      return i;
  }

  return 0;
}

// -------------------------------------------------------------------
// Проверить, что имени файла с разными путями нет в массиве строк
// name - имя файла
// Возвращает номер строки или ноль
// -------------------------------------------------------------------
int _fastcall TFileList::FindName(const WCHAR * name)
{
  WCHAR test[MAX_PATH];
  WCHAR ext[MAX_PATH];
  SplitThePath(name, 0, 0, test, ext);
  WcsCat(test, ext, sizeof(test));

  // Запросить значение по номеру, начиная с 1
  int count = Count();
  for(int i = 1; i <= count; i++)
  {
    const WCHAR * value = GetValueUn(i);
    if (value == 0)
      break;

    WCHAR current[MAX_PATH];
    SplitThePath(value, 0, 0, current, ext);
    WcsCat(current, ext, sizeof(current));
    if (WcsiCmp(current, test) == 0)
      return i;
  }

  return 0;
}

// -------------------------------------------------------------------
// Проверить, что имени файла с заданным расширением нет в массиве строк
// ext - расширение имени файла с точкой (".sxf" и т.п.)
// Возвращает номер строки или ноль
// -------------------------------------------------------------------
int _fastcall TFileList8::FindExt(const char* ext, int first)
{
  int count = Count();

  // Запросить значение по номеру, начиная с 1
  for (int i = first; i <= count; i++)
  {
    const char* value = GetValue(i);
    if (value == 0)
      break;

    char valueext[MAX_PATH]; valueext[0] = 0;
    SplitThePath(value, 0, 0, 0, valueext);
    if (_stricmp(value, ext) == 0)
      return i;
  }

  return 0;
}

// -------------------------------------------------------------------
// Проверить, что имени файла с разными путями нет в массиве строк
// name - имя файла
// Возвращает номер строки или ноль
// -------------------------------------------------------------------
int _fastcall TFileList8::FindName(const char* name)
{
  char test[MAX_PATH];
  char ext[MAX_PATH];
  SplitThePath(name, 0, 0, test, ext);
  StrCat(test, ext, sizeof(test));

  // Запросить значение по номеру, начиная с 1
  int count = Count();
  for (int i = 1; i <= count; i++)
  {
    const char* value = GetValue(i);
    if (value == 0)
      break;

    char current[MAX_PATH];
    SplitThePath(value, 0, 0, current, ext);
    StrCat(current, ext, sizeof(current));
    if (_stricmp(current, test) == 0)
      return i;
  }

  return 0;
}

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++   Копия файла в памяти ++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// -------------------------------------------------------------------
// Прочитать файл в память
// -------------------------------------------------------------------
int TFileBuffer::ReadFile(const WCHAR* name, int* error)
{
  Clear();

  if ((name == 0) || (*name == 0))
  {
    if (error) *error = IDS_PARM;
    return 0;
  }

  TEMPHFILE hfile = OpenTheFileForRead(name);
  if (!hfile)
  {
    if (error) *error = IDS_OPEN;
    return 0;
  }

  BIGINT size = LengthTheFile(hfile);
  if (size == 0)
  {
    if (error) *error = IDS_STRUCT;
    return 0;
  }

  BIGINT limit = 2 * 1024 * 1024 * 1024UL;  // 2Гб
  if (size > limit)
  {
    if (error) *error = IDS_MEMORY;
    return 0;
  }

  char* point = getPlace((int)size + 4);
  if (point == 0)
  {
    if (error) *error = IDS_MEMORY;
    return 0;
  }

  OffsetTheFile(hfile, 0, FILE_BEGIN);

  if (ReadTheFile(hfile, point, (int)size) == 0)
  {
    if (error) *error = IDS_READ;
    return 0;
  }

  point[size] = 0;
  point[size+1] = 0;

  hfile = 0;
  return (int)size;
}
