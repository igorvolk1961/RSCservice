/****** SEMMAIN.CPP ************ Belenkov O.V.   ****** 11/12/14 ***
******* SEMMAIN.CPP ************ Kruzhkov A.E.   ****** 23/07/15 ***
******* SEMMAIN.CPP ************ Derevyagina Zh.A.***** 15/12/15 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2015              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*            ОПИСАНИЕ ФУНКЦИЙ ДЛЯ РАСЧЕТА ЗНАЧЕНИЙ СЕМАНТИК        *
*                                                                  *
*******************************************************************/

#include <stdio.h>  

#ifndef IMLSERV_H
  #include "imlserv.h"
#endif

#ifndef SEMFUNC_H
  #include "semfunc.h"
#endif


SEMLIBLIST SemList =
{
  SEMLIBIDENT, 1, 
  {{ SEM_VOLUME, 0, L"Object volume", L"Объем объекта", 0, 0 },
   { 0, 0, 0, 0, 0, 0},
  }
};


extern "C"
{
 // -----------------------------------------------------------------
 // Запросить список видов формул для расчета семантики,
 // поддерживаемых библиотекой
 // -----------------------------------------------------------------
_IMLIMP const SEMLIBLIST * _IMLAPI semGetSemLibList(void)               // 16/11/15
 {
   return &SemList;
 }

 // -----------------------------------------------------------------
 // Запросить значение семантической характеристики объекта
 // type - тип объекта (задан в SEMLIBLIST),
 // Если функция не поддерживается - возвращает ноль,
 // иначе - ненулевое значение c типом значения:
 // 1 - строка ANSI или UTF-16 (в буфере возвращается UTF-16, но можно преобразовать к ANSI)
 // 2 - строка UTF-16 (нельзя преобразовывать к ANSI)
 // 4 - число типа LONG (в буфере возвращается UTF-16, но можно преобразовать к LONG)
 // 8 - число типа DOUBLE (в буфере возвращается UTF-16, но можно преобразовать к DOUBLE)
 // -----------------------------------------------------------------
_IMLIMP long int _IMLAPI semBuildSemanticValue(long int type,
                                        HOBJ hobj,
                                        long int semcode,
                                        WCHAR * buffer,
                                        long int size)
 {
   if ((type <= 0) || (type > SemList.Count) || (hobj == 0) || (buffer == 0) || (size <= 0))
     return 0;

   switch (type)
   {
     case SEM_VOLUME:

                 return ObjectVolume(hobj, semcode, buffer, size);
   }

   return 0;
 }

// ------------------------------------------------------------------
//  Подключиться к библиотеке ядра
// (вызывается автоматически при каждом открытии классификатора)
// ------------------------------------------------------------------

#if defined(_M_X64) || defined(BUILD_DLL64)
long int _IMLAPI imlOpenGisLibrary64(const char * libname);

_IMLIMP long int _IMLAPI semOpenGisLibrary64(const char * libname)
{
  return imlOpenGisLibrary64(libname);
}
#else
_IMLIMP long int _IMLAPI imlOpenGisLibrary(const char * libname);

_IMLIMP long int _IMLAPI semOpenGisLibrary(const char * libname)
{
  return imlOpenGisLibrary(libname);
}
#endif

// ------------------------------------------------------------------
//  Отключить библиотеку ядра ГИС
// ------------------------------------------------------------------


#if defined(_M_X64) || defined(BUILD_DLL64)
_IMLIMP void _IMLAPI imlCloseGisLibrary64();            // 15/12/15

_IMLIMP void _IMLAPI semCloseGisLibrary64()
{
  imlCloseGisLibrary64();
}
#else
_IMLIMP void _IMLAPI imlCloseGisLibrary();

_IMLIMP void _IMLAPI semCloseGisLibrary()
{
  imlCloseGisLibrary();
}
#endif

} // extern "C"









