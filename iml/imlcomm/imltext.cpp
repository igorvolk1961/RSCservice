/****** IMLTEXT.CPP ************ Belenkov O.V.  *****  25/11/19 ****
******* IMLTEXT.CPP ************ Panteleeva N.А.*****  09/10/17 ****
******* IMLTEXT.CPP ************ Letov V.I.     *****  17/03/20 ****
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2020              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*               МУЛЬТИЯЗЫЧНЫЕ ФУНКЦИИ ЗАПРОСА СТРОК                *
*                                                                  *
*******************************************************************/

#ifndef IMLSERV_H
 #include "imlserv.h"
#endif

#ifdef WIN32API
 #include <tchar.h>
#endif

#include "mapversn.h"

#ifndef HIDEEN
 #include "imltext_en.h"
#endif
#ifndef PANENGL
#ifndef HIDERU
 #include "imltext_ru.h"
#endif
#ifndef HIDEFR
 #include "imltext_fr.h"
#endif
#ifndef HIDEES
 #include "imltext_es.h"
#endif
#ifndef HIDEUA
 #include "imltext_ua.h"
#endif
#ifndef HIDEKZ
 #include "imltext_kz.h"
#endif
#ifndef HIDEVN
 #include "imltext_vn.h"
#endif
#ifndef HIDECN
 #include "imltext_cn.h"
#endif
#endif

//------------------------------------------------------------------
// Функция запроса строки в WCHAR
//------------------------------------------------------------------
const WCHAR * imltxt(int number)
{
#ifndef PANENGL
  int language = imlGetMapAccessLanguage();

#ifndef HIDERU
  if (language == ML_RUSSIAN)   return imltxtRU(number);
#endif
#ifndef HIDEFR
  if (language == ML_FRENCH)    return imltxtFR(number);
#endif
#ifndef HIDEES
  if (language == ML_SPANISH)   return imltxtES(number);
#endif
#ifndef HIDEUA
  if (language == ML_UKRAINIAN) return imltxtUA(number);
#endif
#ifndef HIDEKZ
  if (language == ML_KAZAKH)    return imltxtKZ(number);
#endif
#ifndef HIDEVN
  if (language == ML_VIETNAM)   return imltxtVN(number);
#endif
#ifndef HIDECN
  if (language == ML_CHINESE)   return imltxtCN(number);
#endif
#endif
#ifndef HIDEEN
  return imltxtEN(number);
#else
  return L" ? ";
#endif
}

//------------------------------------------------------------------
// Функция запроса строки в char
//------------------------------------------------------------------
int imltxt(int number, char * name, int size)
{
  const WCHAR * text = imltxt(number);
  return UnicodeToString(text, name, size);
}

//------------------------------------------------------------------
// Функция запроса строки в WCHAR
//------------------------------------------------------------------
int imltxt(int number, WCHAR * name, int size)
{
  const WCHAR * text = imltxt(number);
  return (WcsCopy(name, text, size) != 0);
}
