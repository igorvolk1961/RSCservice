/**** longstring.h ************  Belenkov O.V.     ********** 20/10/21 ***
***** longstring.h ************  Zheleznyakov V.A. ********** 15/04/22 ***
***** longstring.h ************  Vitsko D.A.       ********** 21/01/22 ***
***** longstring.h ************  Kruzhkova E.A.    ********** 10/05/18 ***
***** longstring.h ************  Kruzhkov A.E.     ********** 14/05/21 ***
*                                                                        *
*                   Copyright (c) PANORAMA Group 1991-2022               *
*                            All Rights Reserved                         *
*                                                                        *
**************************************************************************
*                                                                        *
*    Класс для накопления данных в виде строки произвольной длины        *
*                                                                        *
*************************************************************************/

#ifndef LONGSTRING_H
#define LONGSTRING_H

#ifndef CLASSMACRO_H
  #include "classmacro.h"
#endif

#ifndef MAPTYPE_H
  #include "maptype.h"
#endif

#ifndef SIMLIST_H
  #include "simlist.h"
#endif

#define MINSTRINGSIZE  (4*1024)        // 4 Кб


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Функции работы со строками
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Преобразовать строку, убрав спец символы, запрещенные в имени дирректории,
// заменив их на _
// posStart - позиция начала данных или 0
void ReplaceForbiddenFolderSymbol(WCHAR* rebuildName, int posStart = 0);

// Функция поиска части строки между двумя подстроками
// inputString - входная строка
// beginString - подстрока начала поиска
// endString - подстрока конца поиска
// outputString - найденная строка
// size - размер outputString
// Если строка не содержит информацию между подстроками возвращает 0
int SearchSubString(const WCHAR* inputString, const WCHAR* beginString,
  const WCHAR* endString, WCHAR* outputString, int size);

// Заменить все заданные символы строки на указанный
// instr - входная строка
// size - размер входной строки
// oldsymbol - заменяемый символ
// newsymbol - символ, на который заменяют
// При ошибке возвращает 0
int ReplaceSymbol(WCHAR* instr, int size, WCHAR oldsymbol, WCHAR newsymbol);
int ReplaceCharSymbol(char* instr, int size, char oldsymbol, char newsymbol);

// Заменить символы входной строки '/', '\\' в зависимости от ОС
// instr - входная строка
// size - размер входной строки
// При ошибке возвращает 0
int ReplaceSlashSymbol(WCHAR* instr, int size);

// Проверить, что в строке только числа
int IsStringDigit(const char* str);
int IsStringDigit(const WCHAR *wstr);

// Убрать символ из строки
// instr - входная строка
// symbol - убираемый символ
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
int RemoveSymbol(WCHAR* instr, WCHAR symbol);
int RemoveCharSymbol(char* instr, char symbol);

// Убрать указанные символы в конце строки, заданные фильтром
// instr  - входная строка
// filter - строка из убираемых символов
// length - исходная длина строки, если известна заранее
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
int RemoveLastSymbols(WCHAR *instr, const WCHAR *filter, int length = 0);
int RemoveLastCharSymbols(char *instr,  const char *filter,  int length = 0);

// Заменить указанные символы из строки заданным символом
// instr   - входная строка
// symbols - строка из убираемых символов (с завершающим нулем в конце)
// При ошибке возвращает 0, иначе - количество найденных символов
int ReplaceEachSymbol(WCHAR *instr, const WCHAR *symbols, WCHAR newvalue);
int ReplaceEachCharSymbol(char *instr, const char *symbols, char newvalue);

// Убрать указанные символы из строки из строки
// instr   - входная строка
// symbols - строка из убираемых символов (с завершающим нулем в конце)
// Возвращает итоговую длину строки в символах
// При ошибке возвращает 0
int RemoveEachSymbol(WCHAR *instr, const WCHAR *symbols);
int RemoveEachCharSymbol(char *instr, const char *symbols);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения строк в кодировке ANSI или UTF-8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongString
{
 public:

   TLongString(unsigned int minsize = MINSTRINGSIZE, unsigned int stepsize = MINSTRINGSIZE);
   TLongString(const WCHAR* wstr);
   TLongString(const char* astr);
   TLongString(const TLongString &source);
   ~TLongString() { freeMemory(); }

   // Добавить данные
   // size - размер строки в байтах
   // Возвращает адрес записанной строки
   // При ошибке возвращает ноль
   const char* _fastcall addValue(const char* str, int size = 0);
   const char* _fastcall Append(const char* str, int size = 0) { return addValue(str, size); }
   const char *_fastcall Append(TLongString &source) { return addValue(source.getString(), source.getSize()); }

   // Добавить числовые данные, как строку
   // number - число знаков после точки
   // При ошибке возвращает ноль
   int _fastcall addValue(double value, int number = 3);
   int _fastcall addValue(unsigned int value);
   int _fastcall addValue(int value);
   int _fastcall addValue(long int value) { return addValue((int)value); }
   int _fastcall Append(int value) { return addValue(value); }

   // Добавить строку в кодировке Unicode16 c преобразованием в Unicode8
   // size - размер строки в байтах
   // Возвращает адрес записанной строки
   // При ошибке возвращает ноль
   const char* _fastcall addValue(const WCHAR* wstr);
   const char* _fastcall Append(const WCHAR* wstr) { return addValue(wstr); }

   // Добавить текст с заменой всех вхождений заданного текста на указанный
   const char * _fastcall AppendReplacedText(const char *source, const char *seek, const char *replace);

   // Заменить текст в строке
   // oldvalue - указатель на образец подстроки, которые будут искаться для замены
   // newvalue - указатель на новое значение подстроки, которе будет заменять найденные подстроки
   // При ошибке возвращает ноль
   const char * _fastcall ReplaceText(const char *oldvalue, const char *newvalue);

   // Добавить нулевой символ
   // Используется для массива строк, разделеных нулем
   // Последующее добавление с помощью addValue запишет значение после нуля,
   // то есть если после этого читать буфер как обычную строку, будет прочитан
   // только первый фрагмент
   void _fastcall AppendZeroSymbol();

   // Очистить строку
   int _fastcall clearString(unsigned int size = 0) 
   {
     if (size && size > BufferSize)
     {
       if (allocateMemory(size) == 0)
         return 0;
     }
     else
       FreeSize = BufferSize; 
     if (Buffer)
       *Buffer = 0; 
     return 1;
   }
   int _fastcall Clear(unsigned int size = 0) { return clearString(size); }

   // Добавить в конец пути слэш
   int _fastcall CompleteThePath();

   // Вырезать часть строки от заданной позиции
   // При ошибке возвращает ноль
   int _fastcall cutString(const char * begin, int size) { return deleteString(begin, begin + size - 1); }
   int _fastcall Cut(const char * begin, int size) { return cutString(begin, size); }

   // Обрезать строку до заданной позиции
   // (в указанную позицию будет записан завершающий ноль)
   // При ошибке возвращает ноль
   int _fastcall cutString(const char * end);
   int _fastcall Cut(const char * end) { return cutString(end); }

   // Обрезать текст там, где встретится заданный фрагмент
   int _fastcall CropText(const char *seek);

   // Удалить фрагмент строки
   // begin - указатель на первый байт удаляемогно фрагмента
   // end   - указатель на последний байт удаляемогно фрагмента
   // При ошибке возвращает ноль
   int _fastcall deleteString(const char * begin, const char * end);
   int _fastcall Delete(const char * begin, const char * end) { return deleteString(begin, end); }

   // Найти подстроку
   const char* findString(const char* value)
   {
     if (Buffer && value && (*value != 0))
       return strstr(Buffer, value);
     return 0;
   }
   const char* Find(const char* value) { return findString(value); }

   // Проверить, что в строке только числа
   int _fastcall IsStringDigit();

   // Получить длину буффера
   unsigned int getBufferSize() { return BufferSize; }

   // Запросить текущую позицию
   char * _fastcall getCurrentPoint();

   // Получить длину данных (без 0!)
   unsigned int getSize() { return (BufferSize - FreeSize); }

   // Получить длину строки в символах (для char - тоже самое, что и getSize())
   unsigned int getLength() { return getSize(); }

   // Запросить адрес строки
   // При ошибке возвращает ноль
   char * _fastcall getString() { return Buffer; }
   char * _fastcall Address() { return getString(); }

   // Разместить место в записи
   // Возвращает указатель на начало буфера и продвигает текущую позицию за буфер
   // При ошибке возвращает ноль
   char * _fastcall getPlace(int size);

   // Отсечь часть записи в конце
   // При ошибке возвращает ноль
   int _fastcall freePlace(int size);

   // Записать часть строки по заданной позиции
   // Если буфер меньше заданной позиции, то он будет автоматически расширен,
   // промежуточная память будет содержать случайные значения
   // Копируется строка без замыкающего нуля, но не более count символов
   // При ошибке возвращает ноль
   int _fastcall putString(const char * str, int start, int count = 0);

   // Убрать символ из строки
   // symbol - убираемый символ
   // Возвращает итоговую длину строки в символах
   // При ошибке возвращает 0
   int _fastcall RemoveSymbol(char symbol);

   // Заменить все заданные символы строки на указанный
   // oldsymbol - заменяемый символ
   // newsymbol - символ, на который заменяют
   // При ошибке возвращает 0
   int _fastcall ReplaceSymbol(char oldsymbol, char newsymbol);

   // Убрать указанные символы из строки
   // symbols - строка из убираемых символов (с завершающим нулем в конце)
   // Возвращает итоговую длину строки в символах
   // При ошибке возвращает 0
   int _fastcall RemoveEachSymbol(const char *symbols);

   // Заменить указанные символы из строки заданным символом
   // symbols - строка из убираемых символов (с завершающим нулем в конце)
   // При ошибке возвращает 0, иначе - количество найденных символов
   int _fastcall ReplaceEachSymbol(const char *symbols, char newvalue);

   // Убрать указанные символы в конце строки, заданные фильтром
   // filter - строка из убираемых символов
   // Возвращает итоговую длину строки в символах
   // При ошибке возвращает 0
   int _fastcall RemoveLastSymbols(const char *filter);

   // Заменить символы входной строки '/', '\\' в зависимости от ОС
   // instr - входная строка
   // size - размер входной строки
   // При ошибке возвращает 0
   int ReplaceSlashSymbol()
   {
#ifdef WIN32API
     return ReplaceSymbol('/', '\\');
#else
     return ReplaceSymbol('\\', '/');
#endif
   }

   // Убрать кавычки из строки
   // При ошибке возвращает 0
   int RemoveQuote();

   // Обновить часть строки по заданной позиции
   // Копируется строка без замыкающего нуля, но не более count символов
   // При ошибке возвращает ноль
   int _fastcall updateString(const char * str, int start, int count = 0);

   // Обновить часть строки c заданной позиции и длины на новую строку и длину
   // При ошибке возвращает ноль
   int _fastcall updateString(char * oldstr, int oldsize, const char * newstr, int newsize);
   int _fastcall Update(char * oldstr, int oldsize, const char * newstr, int newsize)
   { return updateString(oldstr, oldsize, newstr, newsize); }

 public:

   // Выделить дополнительную память к текущему буферу
   int _fastcall allocateMemory(unsigned int size = 0);

   // Освободить ресурсы
   void freeMemory();
   void Free() { freeMemory(); }

 public:

   const char * operator = (TLongString& source)
   {
     Clear();
     const char * point = source.getString();
     if (point)
       addValue(point);
     return getString();
   }

   const char* operator = (const char * point)
   {
     Clear();
     if (point)
       addValue(point);
     return getString();
   }

   int operator != (TLongString& source)
   {
     const char* point = source.getString();
     if ((point == 0) || (Buffer == 0))
       return 0;
     return (stricmp(Buffer, point) != 0);
   }

   int operator != (const char* point)
   {
     if ((point == 0) || (Buffer == 0))
       return 0;
     return (stricmp(Buffer, point) != 0);
   }

   int operator == (TLongString& source)
   {
     const char* point = source.getString();
     if ((point == 0) || (Buffer == 0))
       return 0;
     return (stricmp(Buffer, point) == 0);
   }

   int operator == (const char* point)
   {
     if ((point == 0) || (Buffer == 0))
       return 0;
     return (stricmp(Buffer, point) == 0);
   }

   const char* operator + (TLongString& source)
   {
     const char* point = source.getString();
     if (point)
       addValue(point);
     return getString();
   }

   const char* operator + (const char* point)
   {
     if (point)
       addValue(point);
     return getString();
   }

   const char *operator += (TLongString &source)
   {
     const char *point = source.getString();
     if (point)
       addValue(point);
     return getString();
   }

   const char *operator += (const char *point)
   {
     if (point)
       addValue(point);
     return getString();
   }

   const char *operator += (int value)
   {
     char point[32];
     IntToStr(value, point, sizeof(point));
     addValue(point);
     return getString();
   }

   const char* operator = (int value)
   {
     Clear();
     char point[32];
     IntToStr(value, point, sizeof(point));
     addValue(point);
     return getString();
   }

 protected:

   char *       Buffer;
#if !defined(_M_X64) && !defined(BUILD_DLL64)
   char *       BZero;      // Выравнивание на 8
#endif

   unsigned int BufferSize;
   unsigned int FreeSize;
   unsigned int MinSize;
   unsigned int StepSize;
}
   TLongString;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения строк в кодировке UTF-16
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringUn
{
 public:

   TLongStringUn();
   TLongStringUn(const WCHAR* wstr);
   TLongStringUn(const TLongStringUn &source);
   ~TLongStringUn() { freeMemory(); }

   // Добавить данные
   // size - размер строки в байтах
   // Возвращает адрес записанной строки
   // При ошибке возвращает ноль
   const WCHAR * _fastcall addValue(const WCHAR* str, int size = 0);
   const WCHAR * _fastcall Append(const WCHAR * str, int size = 0) { return addValue(str, size); }
   const WCHAR *_fastcall  Append(TLongStringUn &source) { return addValue(source.getString(), source.getSize()); }

   // Добавить строку в Unicode8 с преобразованием в Unicode16
   const WCHAR * _fastcall addValue8(const char* str, int size = 0);
   const WCHAR * _fastcall Append8(const char * str, int size = 0) { return addValue8(str, size); }

   // Добавить строку в ANSI/KOI-8 с преобразованием в Unicode16
//Volkov I.A. BEGIN
//   const WCHAR* _fastcall addString(const char* str, int size = 0);
//   const WCHAR* _fastcall AppendString(const char* str, int size = 0) { return addString(str, size); }
//Volkov I.A. END

   // Добавить нулевой символ
   // Используется для массива строк, разделеных нулем
   // Последующее добавление с помощью addValue запишет значение после нуля,
   // то есть если после этого читать буфер как обычную строку, будет прочитан
   // только первый фрагмент
   void _fastcall AppendZeroSymbol();

   // Добавить текст с заменой всех вхождений заданного текста на указанный
   const WCHAR * _fastcall AppendReplacedText(const WCHAR *source, const WCHAR *seek, const WCHAR *replace);

   // Очистить строку
   void _fastcall clearString() { FreeSize = BufferSize; if (Buffer) *Buffer = 0; }
   void _fastcall Clear() { clearString(); }

   // Добавить в конец пути слэш
   int _fastcall CompleteThePath();

   // Вырезать часть строки от заданной позиции
   // size - длина подстроки в байтах
   // При ошибке возвращает ноль
   int _fastcall cutString(const WCHAR* begin, int size) { return deleteString(begin, (WCHAR*)(((char*)begin) + size) - 1); }
   int _fastcall Cut(const WCHAR* begin, int size) { return cutString(begin, size); }

   // Обрезать строку до заданной позиции
   // (в указанную позицию будет записан завершающий ноль)
   // При ошибке возвращает ноль
   int _fastcall cutString(const WCHAR* end);
   int _fastcall Cut(const WCHAR* end) { return cutString(end); }

   // Обрезать текст там, где встретится заданный фрагмент
   int _fastcall CropText(const WCHAR *seek);

   // Удалить фрагмент строки
   // begin - указатель на первый байт удаляемогно фрагмента
   // end   - указатель на последний байт удаляемогно фрагмента
   // При ошибке возвращает ноль
   int _fastcall deleteString(const WCHAR* begin, const WCHAR* end);
   int _fastcall Delete(const WCHAR* begin, const WCHAR* end) { return deleteString(begin, end); }

   // Найти подстроку
   const WCHAR* findString(const WCHAR* value)
   {
     if (Buffer && value && (*value != 0))
       return Wcsstr(Buffer, value);
     return 0;
   }
   const WCHAR* Find(const WCHAR* value) { return findString(value); }

   // Освободить ресурсы
   void freeMemory();
   void Free() { freeMemory(); }

   // Проверить, что в строке только числа
   int _fastcall IsStringDigit();

   // Запросить текущую позицию
   WCHAR * _fastcall getCurrentPoint();

   // Получить длину данных (без 0!)
   unsigned int getSize() { return (BufferSize - FreeSize); }

   // Получить длину строки в символах
   unsigned int getLength() { return getSize()/sizeof(WCHAR); }

   // Запросить адрес строки
   // При ошибке возвращает ноль
   WCHAR * _fastcall getString() { return Buffer; }

   // Разместить место в записи
   // Возвращает указатель на начало буфера и продвигает текущую позицию за буфер
   // При ошибке возвращает ноль
   WCHAR * _fastcall getPlace(int size);

   // Записать часть строки по заданной позиции
   // Если буфер меньше заданной позиции, то он будет автоматически расширен,
   // промежуточная память будет содержать случайные значения
   // Копируется строка без замыкающего нуля, но не более size байт
   // При ошибке возвращает ноль
   int _fastcall putString(const WCHAR * str, int start, int size = 0);

   // Убрать символ из строки
   // symbol - убираемый символ
   // Возвращает итоговую длину строки в символах
   // При ошибке возвращает 0
   int _fastcall RemoveSymbol(WCHAR symbol);

   // Убрать кавычки из строки
   // При ошибке возвращает 0
   int RemoveQuote();

   // Заменить все заданные символы строки на указанный
   // oldsymbol - заменяемый символ
   // newsymbol - символ, на который заменяют
   // При ошибке возвращает 0
   int _fastcall ReplaceSymbol(WCHAR oldsymbol, WCHAR newsymbol);

   // Убрать указанные символы из строки из строки
   // symbols - строка из убираемых символов (с завершающим нулем в конце)
   // Возвращает итоговую длину строки в символах
   // При ошибке возвращает 0
   int _fastcall RemoveEachSymbol(const WCHAR *symbols);

   // Заменить указанные символы из строки из строки заданным символом
   // symbols - строка из убираемых символов (с завершающим нулем в конце)
   // При ошибке возвращает 0, иначе - количество найденных символов
   int _fastcall ReplaceEachSymbol(const WCHAR *symbols, WCHAR newvalue);

   // Убрать указанные символы в конце строки, заданные фильтром
   // filter - строка из убираемых символов
   // Возвращает итоговую длину строки в символах
   // При ошибке возвращает 0
   int _fastcall RemoveLastSymbols(const WCHAR *filter);

   // Заменить символы входной строки '/', '\\' в зависимости от ОС
   // instr - входная строка
   // size - размер входной строки
   // При ошибке возвращает 0
   int ReplaceSlashSymbol()
   {
#ifdef WIN32API
     return ReplaceSymbol('/', '\\');
#else
     return ReplaceSymbol('\\', '/');
#endif
   }

   // Обновить часть строки по заданной позиции
   // Копируется строка без замыкающего нуля, но не более count символов
   // При ошибке возвращает ноль
   int _fastcall updateString(const WCHAR *str, int start, int count);

   // Обновить часть строки c заданной позиции и длины на новую строку и длину
   // (длина в байтах!)
   // При ошибке возвращает ноль
   int _fastcall updateString(WCHAR *oldstr, int oldsize, const WCHAR *newstr, int newsize);

   // Перевести строку в верхний регистр
   void UpperCase();

   const WCHAR * operator = (TLongStringUn& source)
   {
     Clear();
     const WCHAR * point = source.getString();
     if (point)
       addValue(point);
     return getString();
   }

   const WCHAR * operator = (const WCHAR * point)
   {
     Clear();
     if (point)
       addValue(point);
     return getString();
   }

// Volkov I.A.  BEGIN
//   int operator != (const WCHAR* point)
//   {
//     if ((point == 0) || (Buffer == 0))
//       return 0;
//     return (Wcsicmp(Buffer, point) != 0);
//   }

//   int operator == (TLongStringUn& source)
//   {
//     const WCHAR * point = source.getString();
//     if ((point == 0) || (Buffer == 0))
//       return 0;
//     return (Wcsicmp(Buffer, point) == 0);
//   }

//   int operator == (const WCHAR* point)
//   {
//     if ((point == 0) || (Buffer == 0))
//       return 0;
//     return (Wcsicmp(Buffer, point) == 0);
//   }
// Volkov I.A.  END

   const WCHAR * operator + (TLongStringUn& source)
   {
     const WCHAR* point = source.getString();
     if (point)
       addValue(point);
     return getString();
   }

   const WCHAR * operator + (const WCHAR* point)
   {
     if (point)
       addValue(point);
     return getString();
   }

   const WCHAR * operator += (TLongStringUn &source)
   {
     const WCHAR *point = source.getString();
     if (point)
       addValue(point);
     return getString();
   }

   const WCHAR * operator += (const WCHAR *point)
   {
     if (point)
       addValue(point);
     return getString();
   }

   const WCHAR * operator += (int value)
   {
     WCHAR point[32];
     IntToStr(value, point, sizeof(point));
     addValue(point);
     return getString();
   }

   const WCHAR * operator = (int value)
   {
     Clear();
     WCHAR point[32];
     IntToStr(value, point, sizeof(point));
     addValue(point);
     return getString();
   }

 protected:

   // Выделить память под строку
   int _fastcall allocateMemory(unsigned int size = 0);

 protected:

   WCHAR *      Buffer;
#if !defined(_M_X64) && !defined(BUILD_DLL64)
   char *       BZero;      // Выравнивание на 8
#endif
   unsigned int BufferSize;
   unsigned int FreeSize;
}
  TLongStringUn;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива строк в кодировке ANSI или UTF8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringArray : protected TLongString
{
 public:

  TLongStringArray() : TLongString(), StrList() {}
  ~TLongStringArray() {}

  // Добавить строку, массив строк
  // value     - адрес строки
  // limit     - длина данных или 0
  // separator - разделитель или 0
  // При ошибке возвращает ноль
  int _fastcall Add(const char* value, const char* separator = 0, int limit = 0);
  int _fastcall Append(const char* value) { return Add(value); }

  // Добавить строку, если такого значения еще нет в массиве строк
  // Если такая строка уже есть в списке - возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall AddUnique(const char* value, const char* separator = 0, int * existnumber = 0);
  int _fastcall AddUnique(const WCHAR* value, const char* separator = 0, int * existnumber = 0);

  // Очистить массив
  void _fastcall ClearArray();
  void _fastcall Clear() { ClearArray(); }

  // Запросить число строк
  // При ошибке возвращает ноль
  int Count() { return StrList.Count();  }

  // Удалить элемент
  // number - номер элемента начиная с 1 до Count()
  void _fastcall Delete(int number) { return StrList.Delete(number); }

  // Вставить запись в указанную позицию
  // value  - адрес строки
  // number - номер вставляемой записи
  // При ошибке возвращает ноль
  int _fastcall Insert(const char* value, int size, unsigned int number);

  // Проверить, что заданного значения нет в массиве строк
  // Возвращает номер строки или ноль
  int _fastcall Seek(const char * value, int size = 0);
  int _fastcall Find(const char * value) { return Seek(value); }

  // Запросить значение по номеру строки
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  const char* _fastcall GetValue(int number);
  int _fastcall GetValueUn(int number, WCHAR* value, int valsize);
  const char* _fastcall Item(int number) { return GetValue(number); }

  // Обновить значение
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  int _fastcall Update(int number, const WCHAR* newvalue);
  int _fastcall Update(int number, const char* newvalue);

  // Обновить значение указателя
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  int _fastcall UpdatePoint(int number, const char* value);

 protected:

  // Добавить строку, size - длина строки
  int AddPoint(const char* value, int size = 0);

 protected:

  THandleList     StrList; // массив смещений
}
  TLongStringArray;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива отсортированных строк в кодировке ANSI или UTF8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringList : protected TLongStringArray
{
 public:

  TLongStringList() : TLongStringArray() {}
  ~TLongStringList() {}

  // Добавить строку, массив строк
  // value     - адрес строки
  // size      - длина данных или 0
  // separator - разделитель или 0
  // При ошибке возвращает ноль
  int _fastcall Add(const char* value, const char* separator = 0, int size = 0);
  int _fastcall Add(const WCHAR* value, const char* separator = 0, int size = 0);

  // Добавить строку, если такого значения еще нет в массиве строк
  // Если такая строка уже есть в списке - возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall AddUnique(const char* value, const char* separator = 0, int * existnumber = 0);
  int _fastcall AddUnique(const WCHAR* value, const char* separator = 0, int * existnumber = 0);

  // Удалить элемент
  // number - номер элемента начиная с 1 до Count()
  void _fastcall Delete(int number) { return TLongStringArray::Delete(number); }

  // Найти заданную строку в массиве строк
  // value     - адрес строки
  // size      - длина данных или 0
  // Возвращает номер строки или ноль
  int _fastcall Seek(const char * value, int size = 0);
  int _fastcall Find(const char * value) { return Seek(value); }
  int _fastcall Find(const WCHAR * value);

  // Запросить значение по номеру строки
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  const char* _fastcall Item(int number) { return TLongStringArray::Item(number); }

  // Обновить значение
  // number - номер элемента от 1 до Count()
  // value     - адрес строки
  // После обновления позиция элемента может измениться в соответствии с сортировкой
  // При ошибке возвращает ноль
  int _fastcall Update(int number, const WCHAR* value);
  int _fastcall Update(int number, const char* value);

  // Запросить число строк
  // При ошибке возвращает ноль
  int Count() { return StrList.Count(); }

  // Очистить список строк
  void _fastcall ClearArray() { TLongStringArray::ClearArray(); }
  void _fastcall Clear() { ClearArray(); }

 protected:

  // Добавить строку, size - длина строки
  int _fastcall AddPoint(const char* value, int size = 0);

  // Занять переменные класса
  void Lock() { StrList.Lock(); }

  // Освободить переменные класса
  void UnLock() { StrList.UnLock(); }
}
  TLongStringList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива отсортированных строк и id в кодировке ANSI или UTF8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringAndIdList : protected TLongStringList
{
 public:

  TLongStringAndIdList() : TLongStringList(), IdList() {}
  ~TLongStringAndIdList() {}

  // Добавить строку, если такого значения еще нет в массиве строк
  // Если такая строка уже есть в списке - возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall AddUnique(const char* value, int id, int * exisnumber, int * existid);

  // Добавить строку, если такого значения еще нет в массиве строк
  // Если такая строка уже есть в списке - возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall AddUnique(const WCHAR* value, int id, int * exisnumber, int * existid);

  // Очистить список строк
  void _fastcall Clear() { TLongStringList::Clear(); IdList.Clear(); }

  // Запросить число строк
  // При ошибке возвращает ноль
  int Count() { return TLongStringList::Count(); }

  // Удалить элемент
  // number - номер элемента начиная с 1 до Count()
  void _fastcall Delete(int number) { return TLongStringList::Delete(number); }

  // Найти заданную строку в массиве строк
  // value     - адрес строки
  // Возвращает номер строки или ноль
  int _fastcall Find(const char * value) { return TLongStringList::Find(value); }
  int _fastcall Find(const WCHAR * value) { return TLongStringList::Find(value); }

  // Запросить значение строки по номеру строки
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  const char* _fastcall Item(int number) { return TLongStringList::Item(number); }

  // Запросить значение id строки по номеру строки
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  int ItemId(int number) { return IdList.Item(number); }

 protected:

  TNumberList IdList;
}
  TLongStringAndIdList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения большого массива отсортированных строк в кодировке ANSI или UTF8
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringBigList : protected THandleList
{
 public:

  TLongStringBigList() : THandleList(), SizeList() {}
  ~TLongStringBigList() { Clear(); }

  // Добавить строку, если такого значения еще нет в массиве строк
  // Если такая строка уже есть в списке - возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall AddUnique(const char* value, int id, int * exisnumber, int * existid);

  // Добавить строку, если такого значения еще нет в массиве строк
  // Если такая строка уже есть в списке - возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall AddUnique(const WCHAR* value, int id, int * exisnumber, int * existid)
  {
    char cvalue[4096];
    UnicodeToUnicode8(value, cvalue, sizeof(cvalue));
    return AddUnique(cvalue, id, exisnumber, existid);
  }

  // Очистить списки строк
  void _fastcall Clear();

  // Запросить список строк заданной длины
  // При ошибке возвращает ноль
  TLongStringAndIdList * _fastcall GetListForSize(int size);

 protected:

  TFastNumberListOrder SizeList;
}
  TLongStringBigList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива строк в кодировке UTF16
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringArrayUn : protected TLongStringUn
{
 public:

  TLongStringArrayUn() : TLongStringUn(), StrList(), Array() {}
  ~TLongStringArrayUn() {}

  // Добавить строку, массив строк
  // При ошибке возвращает ноль
  int _fastcall Add(const WCHAR* value, const WCHAR* separator = 0);
  int _fastcall Add8(const char* value, const WCHAR* separator = 0);
  int _fastcall Append(const WCHAR* value) { return Add(value); }

  // Добавить массив строк с разделителем delimiter и кавычками quote
  int _fastcall Add(const WCHAR* value, WCHAR delimiter, WCHAR quote);

  // Добавить строку, size - длина строки
  // При ошибке возвращает ноль
  int AddPoint(const WCHAR* value, int size = 0);

  // Добавить строку, если такого значения еще нет в массиве строк
  // Если такая строка уже есть в списке - возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall AddUnique(const WCHAR* value, const WCHAR* separator = 0, int * exisnumber = 0);

  // Добавить подстроку к крайней строке
  // При ошибке возвращает ноль
  int _fastcall Catenate(const WCHAR* value);

  // Удалить элемент
  // number - номер элемента от 1 до Count()
  void _fastcall Delete(int number) { StrList.Delete(number); }

  // Вставить запись в указанную позицию
  // value  - адрес строки
  // number - номер вставляемой записи
  // При ошибке возвращает ноль
  int _fastcall Insert(const WCHAR* value, int size, unsigned int number);

  // Проверить, что заданного значения нет в массиве строк
  // Возвращает номер строки
  // При ошибке возвращает ноль
  int _fastcall Seek(const WCHAR * value);
  int _fastcall Find(const WCHAR * value) { return Seek(value); }

  // Запросить значение по номеру
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  const WCHAR* _fastcall GetValueUn(int number);
  int _fastcall GetValue(int number, char* value, int valsize);
  int _fastcall GetValueUn(int number, WCHAR* value, int valsize);
  const WCHAR* _fastcall Item(int number) { return GetValueUn(number); }

  // Скопировать список строк из другого списка
  // При ошибке возвращает ноль
  int _fastcall Load(TLongStringArrayUn& array);

  // Обновить значение
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  int _fastcall Update(int number, const WCHAR* newvalue);

  // Обновить значение указателя
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  int _fastcall UpdatePoint(int number, const WCHAR* value);

  // Запросить число строк
  // При ошибке возвращает ноль
  int Count() { return StrList.Count(); }

  // Запросить список указателей на строки
  const WCHAR ** List();

  // Очистить массив
  void _fastcall ClearArray();
  void _fastcall Clear() { ClearArray(); }

 protected:

  THandleList StrList;     // массив смещений
  TFastHandleList Array;   // массив указателей
}
  TLongStringArrayUn;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива строк в кодировке UTF16 и их типов
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringArrayType : protected TLongStringArrayUn
{
 public:

  TLongStringArrayType() : TLongStringArrayUn(), TypeList() {}

  // Добавить строку
  // При ошибке возвращает ноль
  int Append(const WCHAR* value, int size = 0, int type = 0)
  {
    if (AddPoint(value, size))
      return TypeList.Append(type);
    return 0;
  }

  // Очистить массив
  void Clear() { TLongStringArrayUn::Clear(); TypeList.Clear(); }

  // Запросить число строк
  // При ошибке возвращает ноль
  int Count() { return TLongStringArrayUn::Count(); }

  // Удалить элемент
  // number - номер элемента от 1 до Count()
  void _fastcall Delete(int number) { TLongStringArrayUn::Delete(number); TypeList.Delete(number); }

  // Запросить значение строки по номеру
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  const WCHAR* Item(int number) { return TLongStringArrayUn::Item(number); }

  // Запросить значение типа по номеру
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  int Type(int number) { return TypeList.Item(number); }

 protected:

  TNumberList TypeList;
}
  TLongStringArrayType;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения списка массивов строк в кодировке UTF16
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringArrayUnList : protected TFastHandleList
{
 public:

  TLongStringArrayUnList() : TFastHandleList() {}
  ~TLongStringArrayUnList() { Clear(); }

  // Добавить строку, массив строк
  // separators - список разделителей, например: WTEXT("\n\t,*")
  // При ошибке возвращает ноль
  int _fastcall AddWords(const WCHAR* value, const WCHAR* separators);

  // Очистить массив
  void Clear();

  // Запросить число элементов списка
  // При ошибке возвращает ноль
  int Count() { return TFastHandleList::Count(); }

  // Запросить элемент списка по номеру
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  TLongStringArrayType * Item(int number) { return (TLongStringArrayType *)TFastHandleList::Item(number); }

 protected:

  // Сравнить текущий символ со списком разделителей
  // При совпадении возвращает ненулевое значение
  int _fastcall Check(WCHAR value, const WCHAR* separators);

  // Определить тип строки - число или текст
  // При совпадении возвращает ненулевое значение
  int _fastcall GetType(const WCHAR * value, int size);

}
  TLongStringArrayUnList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива отсортированных строк в кодировке UTF16
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringListUn : protected TLongStringArrayUn
{
 public:

  TLongStringListUn() : TLongStringArrayUn() {}
  ~TLongStringListUn() {}

  // Добавить строку, массив строк
  // value     - адрес строки
  // size      - длина данных или 0
  // separator - разделитель или 0
  // При ошибке возвращает ноль
  int _fastcall Add(const WCHAR* value, const WCHAR* separator = 0, int size = 0);

  // Добавить строку, если такого значения еще нет в массиве строк
  // Если такая строка уже есть в списке - возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall AddUnique(const WCHAR* value, const WCHAR* separator = 0, int * existnumber = 0);

  // Удалить элемент
  // number - номер элемента начиная с 1 до Count()
  void _fastcall Delete(int number) { return TLongStringArrayUn::Delete(number); }

  // Найти заданную строку в массиве строк
  // value     - адрес строки
  // size      - длина данных или 0
  // Возвращает номер строки или ноль
  int _fastcall Seek(const WCHAR * value, int size = 0);
  int _fastcall Find(const WCHAR * value) { return Seek(value); }

  // Запросить значение по номеру строки
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  const WCHAR* _fastcall Item(int number) { return TLongStringArrayUn::Item(number); }

  // Обновить значение
  // number - номер элемента от 1 до Count()
  // value     - адрес строки
  // После обновления позиция элемента может измениться в соответствии с сортировкой
  // При ошибке возвращает ноль
  int _fastcall Update(int number, const WCHAR* value);

  // Запросить число строк
  // При ошибке возвращает ноль
  int Count() { return StrList.Count(); }

  // Очистить список строк
  void _fastcall ClearArray() { TLongStringArrayUn::ClearArray(); }
  void _fastcall Clear() { ClearArray(); }

 protected:

  // Добавить строку, size - длина строки
  int _fastcall AddPoint(const WCHAR* value, int size = 0);

  // Занять переменные класса
  void Lock() { StrList.Lock(); }

  // Освободить переменные класса
  void UnLock() { StrList.UnLock(); }
}
  TLongStringListUn;


// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива отсортированных строк и id в кодировке UTF16
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringUnAndIdList : protected TLongStringListUn
{
 public:

  TLongStringUnAndIdList() : TLongStringListUn(), IdList() {}
  ~TLongStringUnAndIdList() {}

  // Добавить строку, если такого значения еще нет в массиве строк
  // При ошибке возвращает ноль
  int _fastcall Add(const WCHAR* value, int id)
  {
    TLongStringListUn::Lock();
    int number = TLongStringListUn::Add(value);
    if (number)
      IdList.Insert(number, id);
    TLongStringListUn::UnLock();
    return number;
  }

  // Добавить строку, если такого значения еще нет в массиве строк
  // Если такая строка уже есть в списке - возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall AddUnique(const WCHAR* value, int id, int * exisnumber = 0, int * existid = 0)
  {
    TLongStringListUn::Lock();
    int oldnumber;
    int number = TLongStringListUn::AddUnique(value, 0, &oldnumber);
    if (number)
      IdList.Insert(number, id);
    else
    {
      if (exisnumber)
        *exisnumber = oldnumber;
      if (existid)
        *existid = ItemId(oldnumber);
    }
    TLongStringListUn::UnLock();
    return number;
  }

  // Очистить список строк
  void _fastcall Clear() { TLongStringListUn::Clear(); IdList.Clear(); }

  // Запросить число строк
  // При ошибке возвращает ноль
  int Count() { return TLongStringListUn::Count(); }

  // Удалить элемент
  // number - номер элемента начиная с 1 до Count()
  void _fastcall Delete(int number) { return TLongStringListUn::Delete(number); }

  // Найти заданную строку в массиве строк
  // value     - адрес строки
  // Возвращает номер строки или ноль
  int _fastcall Find(const WCHAR * value) { return TLongStringListUn::Find(value); }

  // Запросить значение строки по номеру строки
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  const WCHAR* _fastcall Item(int number) { return TLongStringListUn::Item(number); }

  // Запросить значение id строки по номеру строки
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  int ItemId(int number) { return IdList.Item(number); }

 protected:

  TNumberList IdList;
}
  TLongStringUnAndIdList;

typedef class TLongStringUnAndId64List : protected TLongStringListUn
{
 public:

  TLongStringUnAndId64List() : TLongStringListUn(), IdList() {}
  ~TLongStringUnAndId64List() {}

  // Добавить строку, если такого значения еще нет в массиве строк
  // При ошибке возвращает ноль
  int _fastcall Add(const WCHAR* value, __int64 id)
  {
    TLongStringListUn::Lock();
    int number = TLongStringListUn::Add(value);
    if (number)
      IdList.Insert(number, id);
    TLongStringListUn::UnLock();
    return number;
  }

  // Добавить строку, если такого значения еще нет в массиве строк
  // Если такая строка уже есть в списке - возвращает ноль
  // При ошибке возвращает ноль
  int _fastcall AddUnique(const WCHAR* value, __int64 id, int * exisnumber = 0, int * existid = 0)
  {
    TLongStringListUn::Lock();
    int oldnumber;
    int number = TLongStringListUn::AddUnique(value, 0, &oldnumber);
    if (number)
      IdList.Insert(number, id);
    else
    {
      if (exisnumber)
        *exisnumber = oldnumber;
      if (existid)
        *existid = (int)ItemId(oldnumber);
    }
    TLongStringListUn::UnLock();
    return number;
  }

  // Очистить список строк
  void _fastcall Clear() { TLongStringListUn::Clear(); IdList.Clear(); }

  // Запросить число строк
  // При ошибке возвращает ноль
  int Count() { return TLongStringListUn::Count(); }

  // Удалить элемент
  // number - номер элемента начиная с 1 до Count()
  void _fastcall Delete(int number) { return TLongStringListUn::Delete(number); }

  // Найти заданную строку в массиве строк
  // value     - адрес строки
  // Возвращает номер строки или ноль
  int _fastcall Find(const WCHAR * value) { return TLongStringListUn::Find(value); }

  // Запросить значение строки по номеру строки
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  const WCHAR* _fastcall Item(int number) { return TLongStringListUn::Item(number); }

  // Запросить значение id строки по номеру строки
  // number - номер элемента от 1 до Count()
  // При ошибке возвращает ноль
  __int64 ItemId(int number) { return IdList.Item(number); }

 protected:

  TNumber64List IdList;
}
  TLongStringUnAndId64List;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива строк в кодировке UTF16,
//  упорядоченных по весу
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringArrayUnOrder : protected TInt32ListOrder
{
 public:

  TLongStringArrayUnOrder() : StringArray(), TInt32ListOrder(256) {}

  // Добавить строку, массив строк
  int _fastcall Append(const WCHAR* value, unsigned int data)
  {
    Lock();
    unsigned int number = StringArray.Add(value);
    int ret;
    if (number == 0)
      ret = 0;
    else
      ret = TInt32ListOrder::Append(number, data);
    UnLock();
    return ret;
  }

  // Запросить число строк
  unsigned int Count() { return StringArray.Count(); }

  // Очистить массив
  void _fastcall Clear() { StringArray.Clear(); TInt32ListOrder::Clear(); }

  // Запросить значение по номеру, начиная с 1
  const WCHAR* _fastcall Item(unsigned int number)
  {
    unsigned int order = TInt32ListOrder::Item(number);
    if (order == 0) return 0;
    return StringArray.Item(order);
  }

  // Запросить вес элемента
  unsigned int ItemData(unsigned int number) { return TInt32ListOrder::ItemData(number); }

  // Найти значение по весу строки
  const WCHAR* _fastcall Find(unsigned int data)
  {
    unsigned int order = TInt32ListOrder::FindItemByData(data);
    if (order == 0) return 0;
    return StringArray.Item(order);
  }

  // Занять переменные класса
  void Lock() { TInt32ListOrder::Lock(); }

  // Освободить переменные класса
  void UnLock() { TInt32ListOrder::UnLock(); }

 protected:

  TLongStringArrayUn StringArray;
}
  TLongStringArrayUnOrder;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива строк в кодировке UTF16,
//  упорядоченных по весу
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringArrayUnAndNumber64 : protected TNumber64List
{
 public:

  TLongStringArrayUnAndNumber64() : StringArray(), TNumber64List() {}

  // Добавить строку, массив строк
  int _fastcall Append(const WCHAR* value, __int64 data)
  {
    Lock();
    int number = StringArray.Add(value);
    int ret;
    if (number == 0)
      ret = 0;
    else
      ret = TNumber64List::Append(data);
    UnLock();
    return ret;
  }

  // Запросить число строк
  int Count() { return StringArray.Count(); }

  // Очистить массив
  void _fastcall Clear() { StringArray.Clear(); TNumber64List::Clear(); }

  // Запросить значение по номеру, начиная с 1
  const WCHAR* _fastcall Item(int number)
  {
    return StringArray.Item(number);
  }

  // Запросить вес элемента
  __int64 ItemData(int number) { return TNumber64List::Item(number); }

  // Найти значение по тексту строки
  int _fastcall Find(const WCHAR * value)
  {
    return StringArray.Find(value);
  }

  // Занять переменные класса
  void Lock() { TNumber64List::Lock(); }

  // Освободить переменные класса
  void UnLock() { TNumber64List::UnLock(); }

 protected:

  TLongStringArrayUn StringArray;
}
  TLongStringArrayUnAndNumber64;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
//  Класс для хранения массива строк в кодировке UTF-8 или ANSI,
//  упорядоченных по весу
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongStringArrayOrder : protected TInt32ListOrder
{
 public:

  TLongStringArrayOrder() : StringArray(), TInt32ListOrder(256) {}

  // Добавить строку, массив строк
  int _fastcall Append(const char* value, unsigned int data)
  {
    Lock();
    unsigned int number = StringArray.Add(value);
    int ret;
    if (number == 0)
      ret = 0;
    else
      ret = TInt32ListOrder::Append(number, data);
    UnLock();
    return ret;
  }

  // Запросить число строк
  unsigned int Count() { return StringArray.Count(); }

  // Очистить массив
  void _fastcall Clear() { StringArray.Clear(); TInt32ListOrder::Clear(); }

  // Удалить элемент
  // number - номер элемента начиная с 1
  void _fastcall Delete(unsigned int number)
  {
    TInt32ListOrder::Delete(number);
  }

  // Запросить значение по номеру, начиная с 1
  const char* _fastcall Item(unsigned int number)
  {
    unsigned int order = TInt32ListOrder::Item(number);
    if (order == 0) return 0;
    return StringArray.Item(order);
  }

  // Запросить вес элемента
  unsigned int ItemData(unsigned int number) { return TInt32ListOrder::ItemData(number); }

  // Найти значение по весу строки
  const char* _fastcall Find(unsigned int data)
  {
    unsigned int order = TInt32ListOrder::FindItemByData(data);
    if (order == 0) return 0;
    return StringArray.Item(order);
  }

  // Найти запись по значению поля для сортировки (веса элемента)
  unsigned int _fastcall FindItemByData(unsigned int data, unsigned int * index = 0)
  {
    return TInt32ListOrder::FindItemByData(data);
  }

  // Занять переменные класса
  void Lock() { TInt32ListOrder::Lock(); }

  // Освободить переменные класса
  void UnLock() { TInt32ListOrder::UnLock(); }

 protected:

  TLongStringArray StringArray;
}
  TLongStringArrayOrder;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++   Список названий и ключей    ++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

// Смещения на поля Ключ и Имя
typedef struct KEYNAME
{
  unsigned int Key;
  unsigned int Name;
}
  KEYNAME;

typedef class TKeyNames : public TSimpleList
{
 public:

  // Конструктор
  TKeyNames() : TSimpleList(sizeof(KEYNAME)), LongString() {}

  // Деструктор
  ~TKeyNames() {}

  // Добавить запись
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(const WCHAR * key, const WCHAR * name);

  // Очистить списки
  void Clear();

  // Запросить элемент по порядковому номеру в списке
  // number - номер элемента, начиная с 1
  KEYNAME * _fastcall Item(unsigned int number)
  {
    return (KEYNAME *)TSimpleList::Item(number);
  }

  // Найти запись по ключу
  KEYNAME * _fastcall Find(const WCHAR * key);

  // Найти запись по ключу
  unsigned int _fastcall FindNumber(const WCHAR * key);

  // Запросить указатель на имя слоя по ключу
  const WCHAR * _fastcall FindName(const WCHAR * key);

  // Запросить ключ по порядковому номеру в списке
  // number - номер элемента, начиная с 1
  const WCHAR * _fastcall GetKey(unsigned int number);

  // Запросить имя по порядковому номеру в списке
  // number - номер элемента, начиная с 1
  const WCHAR * _fastcall GetName(unsigned int number);

  // Скопировать содержимое списка
  int _fastcall Load(TKeyNames * source);

 protected:

  TLongStringUn  LongString;
}
  TKeyNames;

typedef class TKeyNamesA : public TSimpleList
{
 public:

  // Конструктор
  TKeyNamesA() : TSimpleList(sizeof(KEYNAME)), LongString() {}

  // Деструктор
  ~TKeyNamesA() {}

  // Добавить запись
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(const char * key, const char * name);

  // Очистить списки
  void Clear();

  // Оcвободить память
  void Close();

  // Запросить элемент по порядковому номеру в списке
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  KEYNAME * _fastcall Item(unsigned int number)
  {
    return (KEYNAME *)TSimpleList::Item(number);
  }

  // Найти запись по ключу
  // При ошибке возвращает ноль
  KEYNAME * _fastcall Find(const char * key);

  // Найти запись по ключу
  // При ошибке возвращает ноль
  unsigned int _fastcall FindNumber(const char * key);

  // Запросить указатель на имя элемента по ключу
  // При ошибке возвращает ноль
  const char * _fastcall FindName(const char * key);

  // Запросить ключ по порядковому номеру в списке
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  const char * _fastcall GetKey(unsigned int number);

  // Запросить имя по порядковому номеру в списке
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  const char * _fastcall GetName(unsigned int number);

  // Обновить имя по порядковому номеру в списке
  // number - номер элемента, начиная с 1
  // name   - новое значение имени
  // При ошибке возвращает ноль
  int _fastcall UpdateName(unsigned int number, const char * name);

  // Скопировать содержимое списка
  int _fastcall Load(TKeyNamesA * source);

 protected:

  TLongString  LongString;
}
  TKeyNamesA;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++   Список файлов  +++++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TFileList : public TLongStringArrayUn
{
 public:

  TFileList() : TLongStringArrayUn() {}

  // Проверить, что имени файла с заданным расширением нет в массиве строк
  // ext - расширение имени файла с точкой (".sxf" и т.п.)
  // Возвращает номер строки или ноль
  int _fastcall FindExt(const WCHAR * ext, int first = 1);

  // Проверить, что имени файла с разными путями нет в массиве строк
  // name - имя файла
  // Возвращает номер строки или ноль
  int _fastcall FindName(const WCHAR * name);
}
  TFileList;

typedef class TFileList8 : public TLongStringArray
{
 public:

  TFileList8() : TLongStringArray() {}

  // Проверить, что имени файла с заданным расширением нет в массиве строк
  // ext - расширение имени файла с точкой (".sxf" и т.п.)
  // Возвращает номер строки или ноль
  int _fastcall FindExt(const char* ext, int first = 1);

  // Проверить, что имени файла с разными путями нет в массиве строк
  // name - имя файла
  // Возвращает номер строки или ноль
  int _fastcall FindName(const char* name);
}
  TFileList8;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++   Копия файла в памяти ++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TFileBuffer : public TLongString
{
 public:

   TFileBuffer() : TLongString() { MinSize = 0; }

  // Прочитать файл в память (не более 2 Гбайт)
  int ReadFile(const WCHAR* name, int* error = 0);
}
  TFileBuffer;

#endif

