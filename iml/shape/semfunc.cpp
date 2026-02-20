/****** SEMFUNC.CPP ************ Belenkov O.V.   ****** 11/12/14 ***
 ****** SEMFUNC.CPP ************ Savelov A.S.    ****** 23/05/16 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2016              *
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

#ifndef SEMLIB_H
  #include "semfunc.h"
#endif

// ------------------------------------------------------------------
// Вычислить объем объекта на основании площади объекта и значения семантики Относительная высота
// При ошибке возвращает ноль, иначе - 8 (значение DOUBLE)
// ------------------------------------------------------------------
int ObjectVolume(HOBJ hobj, int semcode, WCHAR * buffer, int size)
{
  const char * semantic = imlGetSemanticRecord(hobj);

  double value = 1;
  
  imlGetSemanticDouble((SEMANTIC * )semantic, 1, 1, &value);

  double square = imlGetObjectSquare(hobj);

  double volume = square * value;

#ifndef LINUXAPI                        // 23/05/16 Савелов
  WCHAR total[128];
  swprintf(total, L"%.3f", volume);
  WcsCopy(buffer, total, size);
#else
  char total[128]; total[0] = 0;
  sprintf(total, "%.3f", volume);
  StringToUnicode(total, buffer, size);
#endif
  
  return 8;
}












