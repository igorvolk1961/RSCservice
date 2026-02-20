/****** IMLLINE.CPP ************ Belenkov O.V.   ****** 23/03/05 ***
******* IMLLINE.CPP ************ Dishlenko S.G.  ****** 22/03/05 ***
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

#include "math.h"

DRAWPOINT Buffer[MAXPOINTCOUNT];    // Рабочий буфер для пересчета метрики

// ------------------------------------------------------------------
//  Построить ломаную вдоль заданной линии на расстоянии
//  Выходной буфер превышает входной не менее, чем в два раза
//  При ошибке возвращает ноль иначе - число точек в ломанной
// ------------------------------------------------------------------
int _fastcall ShiftLineByValue(DRAWPOINT * dest, DRAWPOINT * src, int count,
                               int value)
{
  if (((dest && src) == 0) || (count <= 0)) return 0;

  int    ddx,ddy;
  int    dx,dy;
  double sdx;

  DRAWPOINT * point;
  DRAWPOINT   last;
  DRAWPOINT   current;

  if (count == 1)
    {
      dest->X = src->X;
      dest->Y = src->Y + value;
      return 1;
    }

  count--;
  point    = dest;

#ifdef WIN32API                   // 22/03/05
  last.X   = 777777777;
  last.Y   = 777777777;
#else
  last.X   = 65255;
  last.Y   = 65255;
#endif

  // Построить ломанную линию с выбрасыванием соседних точек
  while(count-- > 0)
  {
    current.X = src->X;
    current.Y = src->Y;
    src++;

    dx  = src->X - current.X;
    dy  = src->Y - current.Y;
    sdx = dx*dx + dy*dy;
    if (sdx < 1)                              // 04/04/02
      continue;

    dx  = dx * value;
    if (dx > 0)
      dx = (dx/sqrt(sdx) + 0.5);
    else
      dx = (dx/sqrt(sdx) - 0.5);

    dy  = dy * value;
    if (dy > 0)
      dy = (dy/sqrt(sdx) + 0.5);
    else
      dy = (dy/sqrt(sdx) - 0.5);

    // Первая точка отрезка. Положительное значение - линия справа
    current.X -= dy;
    current.Y += dx;

    ddx = (current.X - last.X);
    ddy = (current.Y - last.Y);

    if ((ddx > 1) || (ddx < -1) || (ddy > 1) || (ddy < -1))
      {
        point->X = current.X;
        point->Y = current.Y;
        last.X   = current.X;
        last.Y   = current.Y;
        point++;
      }

    // Вторая точка отрезка
    current.X = src->X - dy;
    current.Y = src->Y + dx;

    ddx = (current.X - last.X);
    ddy = (current.Y - last.Y);

    if ((ddx > 1) || (ddx < -1) || (ddy > 1) || (ddy < -1))
      {
        point->X = current.X;
        point->Y = current.Y;
        last.X   = current.X;
        last.Y   = current.Y;
        point++;
      }
  }

  return ((int)(((char*)point) - ((char*)dest))/(int)sizeof(DRAWPOINT));
}

// ------------------------------------------------------------------
//  Поиск пересечений двух отрезков
//  Если пересечений нет - возвращает ноль
// ------------------------------------------------------------------
int _fastcall SeekLineCross(DRAWPOINT * line1, DRAWPOINT * line2,
                            DRAWPOINT * cross)
{
 if ((line1 == 0) || (line2 == 0) || (cross == 0))
   return 0;
 
 int a,b,c,det,r;
 double temp;

 int A = line1[1].Y - line1[0].Y;
 int B = line1[0].X - line1[1].X;
 int C = line1[0].Y * line1[1].X - line1[0].X * line1[1].Y;

 int code1 = A * line2[0].X + B * line2[0].Y + C;
 int code2 = A * line2[1].X + B * line2[1].Y + C;

 // Грубая проверка
 if (code1 > 0)
  {
    if (code2 > 0) return 0;
  }
 else
  {
    if (code2 < 0)
      if (code1 < 0)
        return 0;
  }

 // Находим точку пересечения
 a = line2[1].Y - line2[0].Y;
 b = line2[0].X - line2[1].X;
 c = line2[0].Y * line2[1].X - line2[0].X * line2[1].Y;

 det = a*B - A*b;
 if (det == 0)  return 0;

 r = C*b - c*B;

 temp = (double)r/det;
 if (temp > 0)
   cross->X = (temp + 0.5);
 else
   cross->X = (temp - 0.5);

 r = c*A - C*a;

 temp = (double)r/det;
 if (temp > 0)
   cross->Y = (temp + 0.5);
 else
   cross->Y = (temp - 0.5);

 if (line1[0].X < line1[1].X)
  {
    if ((cross->X < line1[0].X) ||
        (cross->X > line1[1].X))
      return 0;
  }
 else
  {
    if ((cross->X > line1[0].X) ||
        (cross->X < line1[1].X))
      return 0;
  }

 if (line2[0].X < line2[1].X)
  {
    if ((cross->X < line2[0].X) ||
        (cross->X > line2[1].X))
      return 0;
  }
 else
  {
    if ((cross->X > line2[0].X) ||
        (cross->X < line2[1].X))
      return 0;
  }

 if (line1[0].Y < line1[1].Y)
  {
    if ((cross->Y < line1[0].Y) ||
        (cross->Y > line1[1].Y))
      return 0;
  }
 else
  {
    if ((cross->Y > line1[0].Y) ||
        (cross->Y < line1[1].Y))
      return 0;
  }

 if (line2[0].Y < line2[1].Y)
  {
    if ((cross->Y < line2[0].Y) ||
        (cross->Y > line2[1].Y))
      return 0;
  }
 else
  {
    if ((cross->Y > line2[0].Y) ||
        (cross->Y < line2[1].Y))
      return 0;
  }

 return 1;
}

// ------------------------------------------------------------------
//  Выбросить в ломаной линии петли
//  src   - указатель на массив точек
//  count - число точек в ломаной
//  При ошибке возвращает ноль иначе - число точек в ломанной
// ------------------------------------------------------------------
int _fastcall DeleteNooseInLine(DRAWPOINT * src, int count)
{
 if ((src == 0) || (count <= 0)) return 0;

 int     i,j;
 DRAWPOINT cross;                          // Точка пересечения
 DRAWPOINT * begin  = src;                 // Первая точка
 DRAWPOINT * end;                          // Предпоследняя точка
 int     number = count - 3;               // Число проверок
 int     delta;

 for (i = 0; i < number; i++)
 {
   end = (src + count - 2);

   for (j = i; j < number; j++)
   {
     if (SeekLineCross(begin, end, &cross) && ((i || j) != 0)) // 11/03/02
       {
         // Заменить 2-ю точку отрезка на точку пересечения
         begin[1] = cross;

         // Удалить точки, расположенные между 2-ой точкой начального
         // отрезка (исключая) и второй точкой замыкающего отрезка (искл.)
         delta = (int)((char *)(src + count) - (char *)&(end[1].X));
         if (delta > 0)
           {
             memmove(&(begin[2].X), &(end[1].X), delta);

             // Скорректировать число проверок и общее число точек
             // Длина удаляемой области
             delta = (int)((char *)&(end[1].X) - (char *)&(begin[2].X));
             delta   = delta/sizeof(DRAWPOINT);
             number -= delta;
             count  -= delta;
             break;
           }
       }
     else
       {
         end--;
       }
   }

   begin++;
 }

 return count;
}

// ------------------------------------------------------------------
//  Поcтроить линию на заданном расстоянии от исходной
//  value - смещение от исходной линии (вправо - больше 0)
//  count - число точек в исходной линии(не более 1024)
//  Возвращает адрес буфера с новой метрикой
// ------------------------------------------------------------------
int _fastcall BuildShiftLine(DRAWPOINT ** dest, DRAWPOINT * src,
                             int count, int value)
{
  int savecount = count;

  if ((value == 0) || (src == Buffer))
    {
      if (dest) *dest = src;
      return count;
    }

  if (count > 1024) count = 1024;

  if ((count = ShiftLineByValue(Buffer, src, count, value)) > 0)
    {
      if ((count = DeleteNooseInLine(Buffer, count)) > 0)
        {
          if (dest) *dest = Buffer;
          return count;
        }
    }

  // Не получилось - вернем исходные координаты
  if (dest) *dest = src;
  return savecount;
}


// ===============================================================
// Рабочие переменные для поиска точки на отрезке или контуре
// ===============================================================

int  FindX2 = 0;       // Следующая точка
int  FindY2 = 0;
int  FinddX = 0;       // Разности координат
int  FinddY = 0;
int  FindsigdX = 0;    // Разность со знаком
int  FindsigdY = 0;    // Разность со знаком

// "Вертикальная" линия внутри области памяти
void _fastcall FindLineVertical(DRAWPOINT& Find, int length);

// "Горизонтальная" линия
void _fastcall FindLineHorizontal(DRAWPOINT& Find, int length);

// ---------------------------------------------------------------
// Определить длину наибольшей стороны отрезка
// ---------------------------------------------------------------
int _fastcall imlLineLength(DRAWLINE * line)
{
  DRAWPOINT Find = {0};

  if (line == 0) return 0;

  // Установим значение координат линии
  Find.X = line->First.X;
  Find.Y = line->First.Y;

  FindX2 = line->Second.X;
  FindY2 = line->Second.Y;

  if ((FinddY = (FindsigdY = FindY2 - Find.Y )) < 0)
    {
      FinddY = -FinddY;
    }

  if ((FinddX = (FindsigdX = FindX2 - Find.X )) < 0)
    {
      FinddX = -FinddX;
    }

  FinddX++;
  FinddY++;

  if (FinddX > FinddY)
    {
      return FinddX;
    }

  return FinddY;
}

// ---------------------------------------------------------------
// Определить длину наибольших сторон контура
// ---------------------------------------------------------------
int _fastcall imlContourLength(DRAWPOINT * points, int count)
{
  if ((points == 0) || (count < 2))
    return 0;

  DRAWLINE line;
  int      length = 0;

  line.Second.X = points->X;
  line.Second.Y = points->Y;
  points++;
  count--;

  while(count-- > 0)
  {
    line.First.X  = line.Second.X;
    line.First.Y  = line.Second.Y;
    line.Second.X = points->X;
    line.Second.Y = points->Y;
    points++;

    length += imlLineLength(&line);
  }

  return length;
}

// ---------------------------------------------------------------
// Поиск точки на заданном расстояниии вдоль контура
// ---------------------------------------------------------------
DRAWPOINT _fastcall imlFindPointOnContour(DRAWPOINT * points, int count, int length)
{
  DRAWPOINT Find = {0};

  DRAWLINE line;
  int      size;

  if ((points == 0) || (count <= 0))
    {
      Find.X = 0;
      Find.Y = 0;
      return Find;
    }

  line.Second.X = points->X;
  line.Second.Y = points->Y;
  points++;
  count--;

  while(count-- > 0)
  {
    line.First.X  = line.Second.X;
    line.First.Y  = line.Second.Y;
    line.Second.X = points->X;
    line.Second.Y = points->Y;
    points++;

    size = imlLineLength(&line);

    if (size > length)
      return imlFindPointOnLine(&line, length);

    if (size == length)
      {
        return line.Second;
      }

    length -= size;
  }

  return line.Second;
}

// ---------------------------------------------------------------
// Поиск точки расположенной на заданном расстояниии вдоль линии
// ---------------------------------------------------------------
DRAWPOINT _fastcall imlFindPointOnLine(DRAWLINE * line, int length)
{
  DRAWPOINT Find = {0};

  if (line == 0) return Find;

  // Установим значение координат линии
  Find.X = line->First.X;
  Find.Y = line->First.Y;

  FindX2 = line->Second.X;
  FindY2 = line->Second.Y;

  // Выберем алгоритм рисования линии
  if ((FinddY = (FindsigdY = FindY2 - Find.Y )) < 0)
    {
      FinddY = -FinddY;
    }

  if ((FinddX = (FindsigdX = FindX2 - Find.X )) < 0)
    {
      FinddX = -FinddX;
    }

  FinddX++;
  FinddY++;

  if (FinddX > FinddY)
    {
      // Горизонтальный алгоритм
      FindLineHorizontal(Find, length);
    }
  else
    {
      // Вертикальный алгоритм
      FindLineVertical(Find, length);
    }

  return Find;
}

// --------------------------------------------------------
//  "Вертикальная" линия внутри области памяти
// --------------------------------------------------------
void _fastcall FindLineVertical(DRAWPOINT& Find, int length)
{
 int Balance = FinddY;                // Флажок шага линии
 int Count   = length;                // Число выводимых точек
 int Sign = FindsigdX;                // Знак приращения
 int Delta;

 if (FindsigdY >= 0) Delta = 1;
   else
     Delta = -1;

 if (Sign == 0)
   {
     if (Count > 0)
       if (FindsigdY >= 0) Find.Y += Count;
         else Find.Y -= Count;
   }
 else
  if (Sign > 0)
   while(Count-- > 0)
    {
      Find.Y += Delta;

      if ((Balance -= FinddX) <= 0)
        {
          Find.X++;            // Слева направо !!!
          Balance += FinddY;
        }
    }
 else
  while(Count-- > 0)
   {
     Find.Y += Delta;

     if ((Balance -= FinddX) <= 0)
      {
        Find.X--;              // Справа налево !!!
        Balance += FinddY;
      }
   }
}

// --------------------------------------------------------
//  "Горизонтальная" линия
// --------------------------------------------------------
void _fastcall FindLineHorizontal(DRAWPOINT& Find, int length)
{
 int Balance = FinddX;             // Флажок шага линии
 int Count   = length;             // Число выводимых точек
 int Sign = FindsigdY;             // Знак приращения
 int Delta;

 if (FindsigdX >= 0) Delta = 1;
   else
     Delta = -1;


 if (Sign == 0)
   {
     if (Count > 0)
      if (FindsigdX >= 0) Find.X += Count;
       else Find.X -= Count;
   }
else
 if (Sign > 0)
  while(Count-- > 0)
   {
     Find.X += Delta;

     if ((Balance -= FinddY) <= 0)
       {
         Find.Y++;            // Шаг на строчку
         Balance += FinddX;
       }
    }
 else
  while(Count-- > 0)
   {
     Find.X += Delta;

     if ((Balance -= FinddY) <= 0)
       {
         Find.Y--;             // Шаг на строчку
         Balance += FinddX;
       }
   }
}


