/****** IMLSQUARE.CPP ********** Belenkov O.V.   ****** 14/02/02 ***
******* IMLSQUARE.CPP ********** Dishlenko S.G.  ****** 22/03/05 ***
******* IMLMAIN.CPP ************ Panteleeva N.A. ****** 28/12/06 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2005              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*     ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ ДЛЯ ОТОБРАЖЕНИЯ ПОЛЬЗОВАТЕЛЬСКИХ     *
*                  ОБ'ЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ                      *
*                                                                  *
*******************************************************************/

#include "imlserv.h"

#include <math.h>

#ifndef OC2000        // 22/03/05
// Список точек пересечения с объектом
LEVELCROSSPOINT ObjectCross[MAXOBJECTCROSS];

// ------------------------------------------------------------------
// Поиск пересечения горизонтального уровня с отрезком
// Если пересечений нет - возвращает ноль
// ------------------------------------------------------------------
int _fastcall SeekHorizontalCross(DRAWPOINT * line, int level,
                                  LEVELCROSSPOINT * cross)
{
 int direct;
 int dx;

 if ((line[0].Y < level) && (line[1].Y < level))
   return 0;                                        // Нет пересечения
 if ((line[0].Y > level) && (line[1].Y > level))
   return 0;                                        // Нет пересечения

 direct = line[1].Y - line[0].Y;

 if (line[0].Y == level)
   {
     cross->Node = 1;
     if (direct == 0)
       return 0;                      // Горизонтальные линии пропускаем
     else
       cross->Z = line[0].X;
   }
 else
  if (line[1].Y == level)
    {
      cross->Node = 1;
      cross->Z    = line[1].X;
    }
  else
    {
      cross->Node = 0;

      dx = line[1].X - line[0].X;

      if (dx == 0)
        {
          cross->Z = line[0].X;
        }
      else
        {
          // xc = (level - y1 + kx1)/k;  k = dy/dx;
          cross->Z = (level - line[0].Y) * dx / direct + line[0].X;
        }  
    }

 if (direct > 0)
   cross->Direct = 1;
 else
   cross->Direct = 0;

 return 1;
}

// ------------------------------------------------------------------
// Добавить пересечение в список пересечений
// ------------------------------------------------------------------
int AppendCrossPoint(LEVELCROSSPOINT * cross, int crosscount)
{
  int               i;
  LEVELCROSSPOINT * list;
  int               limit;

  list  = ObjectCross;
  limit = MAXOBJECTCROSS;

  for (i = 0; i < crosscount; i++)
  {
    if (list->Z > cross->Z)
      {
         // Это место для новой записи
         break;
      }
    else
     if (list->Z == cross->Z)
       {
         // Совпадение интересов
         if (list->Node != 0)
           {
             if (cross->Node == 0)
               {
                 // "Направление" пересечения замещает направление узла
                 list->Direct = cross->Direct;
                 return crosscount;
               }
             else
               {
                 // Повторный узел записывать впереди другого узла
                 break;
               }
           }
         else
             if (cross->Node != 0)
               {
                 // "Направление" пересечения замещает направление узла
                 cross->Direct = list->Direct;

                 // Замещаем запись
                 *list = *cross;

                 return crosscount;
               }
       }

    list++;
  }

  if (crosscount >= limit)
    return limit;

  if (i < crosscount)
    {
      // Раздвинуть список
      memmove((void *)(list + 1), (void *)list,
                     (sizeof(LEVELCROSSPOINT) * (crosscount - i)));
    }

  // Добавляем запись в список
  *list = *cross;

  crosscount++;

  return crosscount;
}

// ------------------------------------------------------------------
// Построить отрезки и удалить промежуточные узлы
// ------------------------------------------------------------------
int BuildConsecutivePiece(int crosscount)
{
  int               i;
  LEVELCROSSPOINT * list;
  LEVELCROSSPOINT * next;

  list  = ObjectCross;

  for (i = 1; i < crosscount; i++)
  {
    next = list + 1;
    if (list->Node && next->Node)
      {
        if (list->Direct == next->Direct)
          {
            // Внутренняя точка - выбрасываем второй узел
            crosscount--;

            if (i < crosscount)
              memmove((void *)next, (void *)(next+1),
                      (sizeof(LEVELCROSSPOINT) * (crosscount - i)));
          }
        else
          {
             // "Плоский" или "острый" экстремум - выбрасываем оба узла
             crosscount -= 2;

             if (i <= crosscount)
               memmove((void *)list, (void *)(next+1),
                       (sizeof(LEVELCROSSPOINT) * (crosscount - i + 1)));

             list--;       // Оставить прежнюю позицию в списке
             i--;
          }
      }

    list++;
  }

  return crosscount;
}

// ------------------------------------------------------------------
// Поиск пересечений горизонтали с набором отрезков
// Если пересечений нет - возвращает ноль
// ------------------------------------------------------------------
int _fastcall BuildHorizontalCross(DRAWPOINT * line, int count, int level,
                                   int subject)
{
  int i;
  LEVELCROSSPOINT cross;
  DRAWPOINT *     point = line;
  int        crosscount = 0;          // Число пересечений

  if (point == 0) return 0;

  for (i = 1; i < count; i++)
  {
     if (SeekHorizontalCross(point, level, &cross))
       {
         if (cross.Node != 0)
           cross.Node = i;           // Номер пересекаемого узла

         crosscount = AppendCrossPoint(&cross, crosscount);
       }

     point++;
  }

  if (crosscount < 1)
    return 0;

  // Построить отрезки и удалить промежуточные узлы
  crosscount = BuildConsecutivePiece(crosscount);

  if (crosscount < 1)
    return 0;

  if (subject == 0)
    {
      // Переписать отрезки в буфер метрики
      int piececount           = crosscount;
      LEVELCROSSPOINT * source = ObjectCross;

      point                    = Buffer;         // 22/03/05
      while(piececount-- > 0)
      {
        point->X = source->Z;
        point->Y = level;
        point++;
        source++;
      }
    }

  return crosscount;
}

// ------------------------------------------------------------------------
// Вычисление центра тяжести объекта (без подобъектов)         // 28/12/06
// При ошибке - возвращает 0
// ------------------------------------------------------------------------
int _fastcall CentreGravity(const POLYDATAEX* pdata,DRAWPOINT * gravpoint)
{
  int i;
  if ((pdata == 0)||(gravpoint == 0)) return 0;
  double area = 0.0;
  double x = 0.0;
  double y = 0.0;
  double moment;
  int    count = *(pdata->PolyCounts); // Число точек объекта

  //if (count <= 1)
  DRAWPOINT* point = pdata->Points;
  DRAWPOINT* pointnext = point++;

  for (i = 0; i < count - 1; i++)
    {
      moment = (double)point->X * (double)pointnext->Y -
               (double)pointnext->X * (double)point->Y;
      area += moment;
      x +=  moment*(point->X + pointnext->X);
      y +=  moment*(point->Y + pointnext->Y);
      point++;
      pointnext++;
    }

  area = area / 2.0;

  if ((area < 0.0001)&&(area > -0.0001))
     {
      gravpoint = pdata->Points;
      return 0;
     }

  x = x/(6.*area);
  y = y/(6.*area);
  gravpoint->X = x;
  gravpoint->Y = y;
  return 1;
}

#else
// Список точек пересечения с объектом
extern LEVELCROSSPOINT ObjectCross[MAXOBJECTCROSS];

#endif // OC2000

