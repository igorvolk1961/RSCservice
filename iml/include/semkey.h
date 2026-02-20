/****** SEMKEY.Н ************* Panteleeva N.A. ****** 15/03/16 ***
******* SEMKEY.H ************* Belenkov O.V.   ****** 24/12/15 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2016              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*            ОПИСАНИЕ ФУНКЦИЙ ДЛЯ РАСЧЕТА ЗНАЧЕНИЙ СЕМАНТИК        *
*                                                                  *
*******************************************************************/
#ifndef SEMKEY_H
#define SEMKEY_H


// Таблицы ключей для образования кодов КЭУС
typedef struct COMMONCODE
{
  char OutSideCode[16];       // Внешний код
  char Key[32];               // Ключ объекта
  int  SemanticCode[10];      // Коды семантик, дополняющие внешний код
}
  COMMONCODE;

// Таблица для поиска кодов КЭУС, требующих дополнительной обработки 
typedef struct COMMONCODEEX
{
  char OutSideCode[16];       // Внешний код
  int  FunctionNumber;        // Номер функции обработки
}
  COMMONCODEEX;

#define CONSTPART 12

extern COMMONCODE  CommonCode[];

extern COMMONCODEEX  CommonCodeEx[];


#endif












