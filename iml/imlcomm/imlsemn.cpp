/****** imlsemn.cpp ************ Belenkov O.V.   ****** 21/10/19 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2019              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*           ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ЗАПРОСА СЕМАНТИКИ              *
*        ДЛЯ ОТОБРАЖЕНИЯ ПОЛЬЗОВАТЕЛЬСКИХ ОБЪЕКТОВ КАРТЫ           *
*                                                                  *
*******************************************************************/

#ifndef IMLSERV_H
 #include "imlserv.h"
#endif

#ifndef SEMDEF_H
 #include "semdef.h"
#endif

//-------------------------------------------------------------------
// Запросить значение семантической характеристики в виде целого числа
// semn   - указатель на семантику (из POLYDATAEX)
// code   - код запрашиваемой характеристики
// number - порядковый номер (с 1) характеристики с данным кодом
//          (с одним кодом может быть несколько значений)
// При ошибке (если значение не найдено) возвращает ноль
//-------------------------------------------------------------------
int _fastcall imlGetSemanticLong(SEMANTIC * semn, int code, int number, long * value)
{
  if (value == 0)
    return 0;

  TWorkSemantic work;
  if (work.Access(semn) == 0)
    return 0;

  TSemBlock * block = work.FindCode(code, number);
  if (block == 0)
    return 0;

  *value = block->LongValue();
  return 1;
}

//-------------------------------------------------------------------
// Запросить значение характеристики в виде числа с плавающей точкой
// semn   - указатель на семантику (из POLYDATAEX)
// code   - код запрашиваемой характеристики
// number - порядковый номер (с 1) характеристики с данным кодом
//          (с одним кодом может быть несколько значений)
// При ошибке (если значение не найдено) возвращает ноль
//-------------------------------------------------------------------
int _fastcall imlGetSemanticDouble(SEMANTIC * semn, int code, int number, double * value)
{
  if (value == 0)
    return 0;

  TWorkSemantic work;
  if (work.Access(semn) == 0)
    return 0;

  TSemBlock * block = work.FindCode(code, number);
  if (block == 0)
    return 0;

  *value = block->DoubleValue();
  return 1;
}

//-------------------------------------------------------------------
// Запросить значение семантической характеристики в виде строки
// semn   - указатель на семантику (из POLYDATAEX)
// code   - код запрашиваемой характеристики
// number - порядковый номер (с 1) характеристики с данным кодом
//          (с одним кодом может быть несколько значений)
// При ошибке (если значение не найдено) возвращает ноль
//-------------------------------------------------------------------
int _fastcall imlGetSemanticString(SEMANTIC * semn, int code, int number, char * value, int size)
{
  if ((value == 0) || (size <= 0))
    return 0;

  TWorkSemantic work;
  if (work.Access(semn) == 0)
    return 0;

  TSemBlock * block = work.FindCode(code, number);
  if (block == 0)
    return 0;

  return block->StringValue(value, size);
}

//-------------------------------------------------------------------
// Запросить значение семантической характеристики в виде строки
// в кодировке UNICODE 
// semn    - указатель на семантику (из POLYDATAEX)
// code    - код запрашиваемой характеристики
// number  - порядковый номер (с 1) характеристики с данным кодом
//           (с одним кодом может быть несколько значений)
// value   - адрес буфера для размещения результата
// maxsize - длина буфера в байтах!
// При ошибке (если значение не найдено) возвращает ноль
//-------------------------------------------------------------------
int _fastcall imlGetSemanticStringUn(SEMANTIC * semn, int code,
                                     int number, WCHAR * value, int size)
{
  if ((value == 0) || (size <= 0))
    return 0;

  TWorkSemantic work;
  if (work.Access(semn) == 0)
    return 0;

  TSemBlock * block = work.FindCode(code, number);
  if (block == 0)
    return 0;

  return block->StringUnicode(value, size);
}

//-------------------------------------------------------------------
// Запросить значение семантической характеристики - ссылки на документ
// в виде строки в кодировке UNICODE
// semn    - указатель на семантику (из POLYDATAEX)
// number  - порядковый номер (с 1) характеристики с данным типом
//           (с одним кодом может быть несколько значений)
// value   - адрес буфера для размещения результата
// maxsize - длина буфера в байтах!
// При ошибке (если значение не найдено) возвращает ноль
//-------------------------------------------------------------------
int _fastcall imlGetSemanticDocReferenceStringUn(HRSC hrsc, SEMANTIC * semn, int number, WCHAR * value, int size, int& type)
{
  if ((value == 0) || (size <= 0))
    return 0;

  TWorkSemantic work;
  if ((work.Access(semn) == 0) || (work.GoFirst() == 0))
    return 0;

  do
  {
    TSemBlock * block = work.GoCurrent();
    if (block == 0) return 0;

    int semtype = block->Type();
    if ((semtype == SEMBASESTRDOS) || ((semtype >= SEMBASESTRING) && (semtype <= SEMBASEBIGUNI)))
    {
      // Это символьная строка, проверим тип кода
      int code = block->Code();

      RSCSEMANTICEX semex;
      if (imlGetRscSemanticExByCode(hrsc, &semex, code) != 0)                     // 21/10/19
      {
        if ((semex.Type >= TDOCREFMIN) && (semex.Type <= TDOCREFMAX) && (semex.Type != TREFER))
        {
          if (--number <= 0)
          {
            type = semex.Type;
            return block->StringUnicode(value, size);
          }
        }
      }
    }
  }
    while(work.GoNext());

  return 0;
}

//-------------------------------------------------------------------
// Запросить значение семантической характеристики в виде строки
// c учетом формата семантики в классификаторе
// data   - указатель на POLYDATAEX
// code   - код запрашиваемой характеристики
// number - порядковый номер (с 1) характеристики с данным кодом
//          (с одним кодом может быть несколько значений)
// При ошибке (если значение не найдено) возвращает ноль
//-------------------------------------------------------------------
int _fastcall imlGetSemanticStringRsc(const POLYDATAEX * data, int code,
                                      int number, char * value, int size)
{
  HRSC hrsc;

  if ((data == 0) || (data->Semantic == 0))
    return 0;

  SEMANTIC * semn = (SEMANTIC *)data->Semantic;

  if ((hrsc = (HRSC) data->MapRsc) == 0)
    return imlGetSemanticString(semn, code, number, value, size);

  TWorkSemantic work;
  if (work.Access(semn) == 0)
    return 0;

  TSemBlock * block = work.FindCode(code, number);
  if (block == 0)
    return 0;

  int type = block->Type();
  if ((type == SEMBASEDOUBLE) || (type == SEMBASELONG) || (type == SEMBASESHORT))
    {
      RSCSEMANTICEX semex;
      if (imlGetRscSemanticExByCode(hrsc, &semex, code) != 0)
        {
          double dvalue = 0;
          char   buffer[260];
          char   format[8];
          int    decimal = semex.Decimal;

          if (decimal > 16)
            decimal = 8;

          dvalue = block->DoubleValue();

       	  sprintf(format, "%s.%d%s", "%", decimal, "lf"); // %.4lf
          sprintf(buffer, format, dvalue);
          strncpy(value, buffer, size);
          return 1;
        }
    }

  return block->StringValue(value, size);
}

int _fastcall imlGetSemanticStringRscUn(const POLYDATAEX * data, int code,
                                        int number, WCHAR * value, int size)
{
  HRSC hrsc;

  if ((data == 0) || (data->Semantic == 0))
    return 0;

  SEMANTIC * semn = (SEMANTIC *)data->Semantic;

  if ((hrsc = (HRSC) data->MapRsc) == 0)
    return imlGetSemanticStringUn(semn, code, number, value, size);

  TWorkSemantic work;
  if (work.Access(semn) == 0)
    return 0;

  TSemBlock * block = work.FindCode(code, number);
  if (block == 0)
    return 0;

  int type = block->Type();
  if ((type == SEMBASEDOUBLE) || (type == SEMBASELONG) || (type == SEMBASESHORT))
    {
      RSCSEMANTICEX semex;
      if (imlGetRscSemanticExByCode(hrsc, &semex, code) != 0)
        {
          double dvalue = 0;
          char   buffer[260];
          char   format[8];
          int    decimal = semex.Decimal;

          if (decimal > 16)
            decimal = 8;

          dvalue = block->DoubleValue();

       	  sprintf(format, "%s.%d%s", "%", decimal, "lf"); // %.4lf
          sprintf(buffer, format, dvalue);
          StringToUnicode(buffer, value, size);
          return 1;
        }
    }

  return block->StringUnicode(value, size);
}

