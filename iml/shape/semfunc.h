/****** SEMFUNC.H  ************* Belenkov O.V.   ****** 11/12/14 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2014              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*   ОПИСАНИЕ ФУНКЦИЙ ВИЗУАЛИЗАЦИИ ОБЪЕКТОВ ОПЕРАТИВНОЙ ОБСТАНОВКИ  *
*                                                                  *
*******************************************************************/

#ifndef SEMFUNC_H
#define SEMFUNC_H

#define SEM_VOLUME             1  // ОБЪЕМ ОБЪЕКТА

// Вычислить объем объекта на основании площади объекта и значения семантики Относительная высота
int ObjectVolume(HOBJ hobj, int semcode, WCHAR * buffer, int size);


#endif  // SEMFUNC_H



