/****** SHAPE.H  *************** Belenkov O.V.   ****** 06/03/15 ***
******* SHAPE.CPP ************** Panteleeva N.A. ****** 14/03/16 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2016              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*  ОПИСАНИЕ ВСПОМОГАТЕЛЬНЫХ ФУНКЦИЙ ВИЗУАЛИЗАЦИИ ОБ'ЕКТОВ КАРТЫ    *
*                                                                  *
*******************************************************************/

#ifndef SHAPE_H
#define SHAPE_H

#ifndef MAPGDIEX_H
  #include "mapgdiex.h"
#endif

#define IMG_MARKALONGLINE        1  // ЗНАК ВДОЛЬ ЛИНИИ
#define IMG_HORIZONTALSHADE      2  // ГОРИЗОНТАЛЬНАЯ ШТРИХОВКА
#define IMG_MIRRORMARKALONGLINE  3  // ЗЕРКАЛЬНЫЙ ЗНАК ВДОЛЬ ЛИНИИ
#define IMG_MARKINTOLINE         4  // ЗНАК ВНУТРИ ЗЕРКАЛЬНОЙ ЛИНИИ
#define IMG_TURNMARK             5  // ЗНАК, С ПОВОРОТОМ - УГОЛ В СЕМАНТИКЕ
#define IMG_UNIVERSALTEXT        6  // ТЕКСТ С ПАРАМЕТРАМИ ИЗ СЕМАНТИКИ
#define IMG_SCALEVECTOR          7  // МАСШТАБИРУЕМЫЙ ВЕКТОРНЫЙ
#define IMG_THICKENLINEEX        8  // УТОЛЩЕННЫЙ ЛИНЕЙНЫЙ
#define IMG_MIDDLETHICKENLINEEX  9  // УТОЛЩЕННЫЙ ЛИНЕЙНЫЙ В СЕРЕДИНЕ
#define IMG_MARKANDNOTE         10  // Знак с выноской
#define IMG_SHADOWOBJECT        11  // Объект с тенью
#define IMG_COLORSHADOWOBJECT   12  // Объект с цветной тенью
#define IMG_DOUBLETHICKENLINE   14  // УТОЛЩЕННЫЙ ЛИНЕЙНЫЙ ПО ВСЕЙ ЛИНИИ
#define IMG_POLYARC             15  // ДУГА ПО ЛОМАННОЙ
#define IMG_THICKENDOTLINE      16  // УТОЛЩЕННЫЙ ПУНКТИР
#define IMG_TURNANDLENGTHMARK   17  // ЗНАК С ПОВОРОТОМ И ДЛИНОЙ ПО СЕМАНТИКЕ
#define IMG_LENGTHANDANGLELINE  18  // ЛИНИЯ С ПОДПИСЬЮ ДЛИНЫ
#define IMG_GRADIENTCIRCLE      19  // ОКРУЖНОСТЬ ПО ДВУМ ТОЧКАМ С ГРАДИЕНТНОЙ ЗАЛИВКОЙ 
#define IMG_SCALELIMITEDIMAGE   20  // ЗНАК, РИСУЕМЫЙ ПРИ ЗАДАННЫХ МАСШТАБАХ 
#define IMG_QUARTER             21  // ПРИПОДНЯТЫЙ МНОГОУГОЛЬНИК 
#define IMG_IMAGEVIEW           22  // ОТОБРАЖЕНИЕ ГРАФИЧЕСКИХ ФАЙЛОВ
#define IMG_CONTOURINGLINE      23  // ЛИНИЯ С ПОСТОЯННОЙ ШИРИНОЙ КОНТУРА  14/03/16

// Условный знак вдоль линии на заданном интервале
int _fastcall MarkAlongLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                            const IMGCONTEXT * context, int test,
                            FRAME * border);

// Горизонтальная штриховка полигона
int _fastcall HorizontalShade(const POLYDATAEX* pdata, const IMGDRAW* draw,
                              const IMGCONTEXT * context, int test,
                              FRAME * border);

// Зеркальный знак вдоль линии на заданном интервале
int _fastcall MirrorMarkAlongLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                  const IMGCONTEXT * context, int test,
                                  FRAME * border);

// Условный знак внутри зеркальной линии на заданном интервале
int _fastcall MarkIntoLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Условный знак с поворотом по семантике
int _fastcall TurnMark(const POLYDATAEX* pdata, const IMGDRAW* draw,
                       const IMGCONTEXT * context, int test,
                       FRAME * border);

// Условный знак с поворотом и длиной по семантике
int _fastcall TurnAndLengthMark(const POLYDATAEX* pdata, const IMGDRAW* draw,
                       const IMGCONTEXT * context, int test,
                       FRAME * border);

// Текст с параметрами из семантики
int _fastcall UniversalText(const POLYDATAEX* pdata, const IMGDRAW* draw,
                            const IMGCONTEXT * context, int test,
                            FRAME * border);

// Масштабируемый векторный знак   
int _fastcall ScaleVector(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border);

// Утолщающаяся линия в конце
int _fastcall ThickenLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border);

// Утолщающаяся линия в середине
int _fastcall MiddleThickenLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                const IMGCONTEXT * context, int test,
                                FRAME * border);

// Знак с выноской
int _fastcall MarkAndNote(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border);

// Объект с тенью
int _fastcall ShadowObject(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border);

// Утолщающаяся линия в конце
int _fastcall DoubleThickenLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                const IMGCONTEXT * context, int test,
                                FRAME * border);

// Дуга по ломанной линии (параллели, меридианы и пр.)
int _fastcall PolyArc(const POLYDATAEX* pdata, const IMGDRAW* draw,
                      const IMGCONTEXT * context, int test,
                      FRAME * border);

// Подпись длины и угла векторного знака
int _fastcall LengthAndAngleLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                 const IMGCONTEXT * context, int test,
                                 FRAME * border);

// Окружность по двум точкам с градиентной заливкой с понижением
// от центра
int _fastcall GradientCircle(const POLYDATAEX* pdata, const IMGDRAW* draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border);

// Условный знак, видимый в заданном диапазоне масштабов отображения
int _fastcall ScaleLimitedImage(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                const IMGCONTEXT * context, int test,
                                FRAME * border);

// Приподнятый многоугольник
int _fastcall QuarterImage(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);

// Отображение графических файлов
int _fastcall ImageView(const POLYDATAEX* pdata, const IMGDRAW* draw,
                        const IMGCONTEXT * context, int test,
                        FRAME * border);

// Линия с постоянным контуром       //14/03/16 
int _fastcall ContouringLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                        const IMGCONTEXT * context, int test,
                        FRAME * border);

#endif

