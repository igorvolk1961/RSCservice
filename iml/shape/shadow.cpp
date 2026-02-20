/****** SHADOW.CPP ************* Belenkov O.V.   ****** 14/01/06 ***
****** SHADOW.CPP ************** Panteleeva N.A. ****** 06/07/09 ***
****** SHADOW.CPP ************** Dishlenko S.G.  ****** 26/03/12 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2012              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*      ОПИСАНИЕ ПАРАМЕТРОВ ФУНКЦИЙ ВИЗУАЛИЗАЦИИ ПРОИЗВОЛЬНЫХ       *
*             ГРАФИЧЕСКИХ ОБЪЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ               *
*                                                                  *
*******************************************************************/

#ifndef IMLSERV_H
  #include "imlserv.h"
#endif

#ifndef SHAPE_H
  #include "shape.h"
#endif


// --------------------------------------------------------
// Сдвинуть координаты в заданном направлении
// --------------------------------------------------------
void _fastcall MoveMetric(const POLYDATAEX* pdata, int deltaX,
                          int deltaY)
{
  int count        = pdata->Count;
  DRAWPOINT* point = pdata->Points;
  int*  polycount = pdata->PolyCounts;          // 26/03/12

  while(count-- > 0)
  {
    int polycounts = *polycount;
    polycount++;

    while (polycounts-- > 0)
    {
      point->x += deltaX;
      point->y += deltaY;
      point++;
    }
  }
}

// --------------------------------------------------------
// Отобразить объект с тенью
// (проявляется при увеличении масштаба)
// --------------------------------------------------------
int _fastcall ShadowObject(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
 const char * parm;
 int          func;
 int          ret = 0;

 if (pdata == 0) return 0;


 int        deltaX = 3;
 int        deltaY = 3;

 if (imlDrawType(draw, 3) == IMG_DOUBLE)
   {
     if ((parm = imlDrawParm(draw, 3)) != 0)
       {
         // Интервал в мм
#ifdef RISCCPU
         DOUBLEREGISTER regvalue;
         DoubleToRegister(regvalue, parm);
         double  value = regvalue.Double;
#else
         double value = ((IMGDOUBLE *)parm)->Value;
#endif
         if (value < -20) value = -20;      // 06/07/09
         if (value >  20) value =  20;

         // Интервал в точках (из мм)
         deltaX = (context->Scale * context->PixelX * value * 1000.0);
       }
   }
 if (imlDrawType(draw, 4) == IMG_DOUBLE)
   {
     if ((parm = imlDrawParm(draw, 4)) != 0)
       {
         // Интервал в мм
#ifdef RISCCPU
         DOUBLEREGISTER regvalue;
         DoubleToRegister(regvalue, parm);
         double  value = regvalue.Double;
#else
         double value = ((IMGDOUBLE *)parm)->Value;
#endif
         
         if (value < -20) value = -20;      // 06/07/09
         if (value >  20) value =  20;


         // Интервал в точках (из мм)
         deltaY = (context->Scale * context->PixelY * value * 1000.0);
       }
   }

 // Тип функции
 func = imlDrawType(draw, 2);

 // Параметры функции
 parm = imlDrawParm(draw, 2);

/****************************************************************
 IMGLINE    imgline;

 int        count = *(pdata->PolyCounts); // Число точек объекта
 DRAWPOINT* point = pdata->Points;

 // Точек больше трех и объект замкнут?
 if ((count > 3) && (point->x == point[count-1].x) &&
                    (point->y == point[count-1].y))
   {
     // Тень площадного объекта
   }
 else
   {
     // Тень линейного объекта
     func           = IMG_LINE;
     parm           = (char *)&imgline;
     imgline.Color  = 0x0545454;
     imgline.Thick  = 520;
   }
*****************************************************************/

 if ((deltaX != 0) || (deltaY != 0))
   // Построить метрику тени
   MoveMetric(pdata, deltaX, deltaY);

 // Отобразить тень
 ret |= imgPrepareImage(pdata, func, parm, context, test, border);

 if ((deltaX != 0) || (deltaY != 0))
   // Восстановить метрику объекта
   MoveMetric(pdata, -deltaX, -deltaY);

 if ((ret != 0) && (test == PREPARE_TEST))
   {
     return 1;
   }

 // Тип функции
 func = imlDrawType(draw, 1);

 // Параметры функции
 parm = imlDrawParm(draw, 1);

 // Отобразить объект
 ret |= imgPrepareImage(pdata, func, parm, context, test, border);

 return ret;
}

