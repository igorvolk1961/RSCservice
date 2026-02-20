/******* TERRPLAN.H  ************* Пантелеева N.А. ****** 11/04/12 **
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2012              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*      ОПИСАНИЕ ФУНКЦИЙ ЗНАКОВ ТЕРРИТОРИАЛЬНОГО ПЛАНИРОВАНИЯ       *
*                                                                  *
*******************************************************************/

#ifndef TERRPLAN_H
#define TERRPLAN_H

#ifndef MAPGDIEX_H
  #include "mapgdiex.h"
#endif

#define IMG_SIMPLEOBJECT          1  // Знак, меняет размер и цвет
#define IMG_COMPLEXOBJECT         2  // Знак, меняет вид и цвет
#define IMG_LINEOBJECT            3  // Линия, меняет вид и цвет
#define IMG_PTCONTOUR             4  // Контур площадных, меняет вид(линия,
                                     // широкая линия, пунктир) и цвет
#define IMG_SEMSIZEOBJECT         5  // Знак, меняет размер (по заданной семантике)
                                     // и цвет
#define IMG_BOWELSCONTOUR         6  // Окантовка участков недр
#define IMG_MODYTRUETYPE          7  // Знак TRUETYPE, меняет размер (по заданной семантике)
                                     // и цвет (по значениям заданной семантики)

#define IMG_OBJMARK               8  // Знак PNG, меняет цвет фона и окантовку
#define IMG_OBJLINE               9  // Линейный объект, вид по статусу
#define IMG_OBJSQUARE            10  // Площадной объект, вид по статусу

// Знак, меняющий размер и цвет
 int _fastcall SimpleObject(const POLYDATAEX * data, const IMGDRAW * parm,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Знак, меняющий вид и цвет
 int _fastcall ComplexObject(const POLYDATAEX * data, const IMGDRAW * parm,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Линия, меняющая вид и цвет
 int _fastcall LineObject(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border);

 // Контур для природных территорий
 int _fastcall ContourPT(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border);
 // Знак, меняет размер (по заданной семантике) и цвет
 int _fastcall SemSizeObject(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border);

 // Окантовка участков недр
 int _fastcall BowelsContour(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border);

 // Модифицированный TrueType объект
 // изменение размера(c указанием семантики и ее значений)
 // изменение цвета (с указанием значения и соотв цветов)
 int _fastcall ModyTrueType(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border);
 // Графический знак, меняющий фон и окантовку
 int _fastcall ObjectMark(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border);
 // Линия, меняющяя вид по статусу
 int _fastcall ObjectLine(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border);
 // Площадь, меняющяя вид по статусу
 int _fastcall ObjectSquare(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border);
#endif



