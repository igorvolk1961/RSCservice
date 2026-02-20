/****** SHAPESQ.CPP ************ Belenkov O.V.   ****** 02/06/05 ***
******* SHAPESQ.CPP ************ Dishlenko S.G.  ****** 26/03/12 ***
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

//#define  MAPTRACER TRUE   // Для исключения трассировки - убрать строчку

#ifndef IMLSERV_H
  #include "imlserv.h"
#endif

#ifndef SHAPE_H
  #include "shape.h"
#endif

#include <math.h>

// --------------------------------------------------------
// Горизонтальная штриховка полигона
// --------------------------------------------------------
int _fastcall HorizontalShade(const POLYDATAEX* pdata, const IMGDRAW* draw,
                              const IMGCONTEXT * context, int test,
                              FRAME * border)
{
 int          i;
 int          thick;
 int          level;
 const char * parm;
 int          step = 2;

 IMGLINE    defline;
 IMGSQUARE  square;

 POLYDATAEX  data;
 int   number;                 // 26/03/12
 int         count;
 int         subjcount;
 int         ret    = 0;
 double      multi;
 int         type;
 int         begin;
 int         limit;

 if ((pdata && draw && context) == 0)
   return 0;

 if ((test == PREPARE_TEST) || (test == PREPARE_BORDER))
   {
     // Запрос попадания курсора в площадь
     return imgPrepareImage(pdata, IMG_SQUARE, (const char *)&square,
                            context, test, border);
   }

 // Параметры штриховки
 if (imlDrawType(draw, 1) == IMG_LINE)
   {
     // Параметры фоновой линии
     defline = *((IMGLINE *)imlDrawParm(draw, 1));
   }
 else
   {
     defline.Thick = 200;          // 0.2 мм
     defline.Color = 0;            // Черный цвет
   }

 multi = context->Scale * context->PixelX;

 // Ширина штриховки в точках
 thick = (multi * defline.Thick + 0.5);
 if (thick < 1)
   thick = 1;

 // Ширина одной точки
 defline.Thick = 1.0/multi;
 if (defline.Thick < 1)
   defline.Thick = 1;

 if (imlDrawType(draw, 2) == IMG_DOUBLE)
   {
     if ((parm = imlDrawParm(draw, 2)) != 0)
       {
         // Интервал в точках (из мм)
#ifdef RISCCPU                                            // 25/01/05
         DOUBLEREGISTER regvalue;
         DoubleToRegister(regvalue, parm);
         double  value = regvalue.Double;
         step = (context->Scale * context->PixelX *
                               value * 1000.0 + 0.5);
#else
         step = (context->Scale * context->PixelX *
                               ((IMGDOUBLE *)parm)->Value * 1000.0 + 0.5);
#endif	 
         if (step < 1)
           step = 2;
       }
   }

 data.Count      = 1;
 data.PolyCounts = &number;
 number          = 2;

 begin = (pdata->Border.UP);// - (step - 1))/ step * step;
 limit = context->Size.cy;

 // Последовательные сечения объекта с заданным шагом
 for (level = begin; level <= pdata->Border.DN; level += step)
 {
  for (i = 0; i < thick; i++)
   {
    int scanlevel = level + i;
    if (scanlevel < 0)
      continue;

    if (scanlevel > limit)
      {
        // Ниже уже не видно
        level = pdata->Border.DN;
        break;
      }

    data.Points     = Buffer;

    // Построить заполняющие отрезки
    if ((count = BuildHorizontalCross(pdata->Points, *(pdata->PolyCounts),
                                      scanlevel, 0)/ 2) > 0)
     {
       int j             = pdata->Count;
       int * polycounts = pdata->PolyCounts;       // 26/03/12
       DRAWPOINT * point = pdata->Points;

       // Переберем все подобъекты
       while (j-- > 1)
         {
           LEVELCROSSPOINT * subjpoint = ObjectCross;

           // Вырежем подобъекты
           point += *polycounts;
           polycounts++;

           subjcount = BuildHorizontalCross(point, *polycounts,
                                            scanlevel, 1)/ 2;

           // Переберем все пересечения с подобъектом
           while (subjcount-- > 0)
             {
               DRAWPOINT * objpoint  = Buffer;
               int objcount = count;
               int x1, x2;

               x1 = subjpoint->Z;
               subjpoint++;
               x2 = subjpoint->Z;
               subjpoint++;

               // Рассмотрим все пересечения объекта
               while(objcount-- > 0)
               {
                 if ((x1 < objpoint[1].x) && (x2 > objpoint[0].x))
                  {
                   // Нужно вырезать отрезок
                   if (x1 <= objpoint[0].x)
                     {
                       if (x2 >= objpoint[1].x)
                         {
                           // Удалить отрезок полностью
                           memmove((void*)objpoint, (void*)(objpoint+2),
                                   sizeof(DRAWPOINT)*objcount*2);
                           count--;
                           objpoint -= 2;
                         }
                       else
                         {
                           // Заменить первую точку
                           objpoint[0].x = x2;
                         }
                     }
                   else
                     {
                       if (x2 >= objpoint[1].x)
                         {
                           // Заменить вторую точку
                           objpoint[1].x = x1;
                         }
                       else
                         {
                           // Разделить на два отрезка
                           memmove((void*)(objpoint+3),
                                   (void*)(objpoint+1),
                                   sizeof(DRAWPOINT)*(2*objcount+1));

                           // Вставим вторую точку первого отрезка
                           objpoint[1].x = x1;

                           // Вставим первую точку второго отрезка
                           objpoint[2].x = x2;
                           objpoint[2].y = objpoint[1].y;

                           count++;
                           break;
                         }
                     }
                  }
                 else
                  if (x2 < objpoint[0].x)
                    {
                      // Дальше искать нет смысла
                      break;
                    }

                 objpoint +=2;
               }
             }
         }

       // Нарисовать отрезки
       while(count-- > 0)
       {
         ret |= imgPrepareImage(&data, IMG_LINE, (const char *)&defline,
                                 context, test, border);

         if ((ret != 0) && (test == PREPARE_TEST))
           {
             return 1;
           }

         // Обойти две точки
         data.Points += 2;
       }
     }
   }
 }

 if ((type = imlDrawType(draw, 3)) != 0)
   {
     if ((parm = imlDrawParm(draw, 3)) != 0)
       {
         // Контурная линия
         return imgPrepareImage(pdata, type, parm,
                                context, test, border);
       }
   }

 return ret;
}