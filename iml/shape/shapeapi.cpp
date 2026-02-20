/****** SHAPE.CPP ************** Belenkov O.V.   ****** 23/05/16 ***
******* SHAPE.CPP ************** Dishlenko S.G.  ****** 26/03/12 ***
******* SHAPE.CPP ************** Shabakov D.A.   ****** 29/04/12 ***
******* SHAPE.CPP ************** Борзов А.Ю.     ****** 17/07/15 ***
******* SHAPE.CPP ************** Savelov A.S.    ****** 03/09/15 ***
******* SHAPE.CPP ************** Kruzhkov A.E.   ****** 12/11/15 ***
******* SHAPE.CPP ************** Panteleeva N.A. ****** 11/04/16 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2016              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*      ОПИСАНИЕ ПАРАМЕТРОВ ФУНКЦИЙ ВИЗУАЛИЗАЦИИ ПРОИЗВОЛЬНЫХ       *
*             ГРАФИЧЕСКИХ ОБЪЕКТОВ ЭЛЕКТРОННОЙ КАРТЫ               *
*                                                                  *
*******************************************************************/

//#define MAPTRACER 1   // Если трассировка не нужна - FALSE

#ifndef IMLSERV_H
  #include "imlserv.h"
#endif

#ifndef SHAPE_H
  #include "shape.h"
#endif

#include <math.h>

#include "maptrace.h"       // Протокол - c:maptace.log
#include <stdio.h>

#ifndef SIMLIST_H
 #include "simlist.h"
#endif

#ifdef WIN32API            // 17/08/15 Савелов
#include <olectnrs.hpp>
#endif

#define CHECKFILESEC 30 // временной интервал проверки изменения файла в секундах

#ifdef WIN32API    // 17/08/15 Савелов
// Невидимая форма для размещения OLE контейнеров
TForm *OleForm = 0;

// Класс для освобождения OLE формы при выгрузке библиотеки
// (создается форма при первой отрисовке OLE объекта)
class TFreeOleFormClass
{
  public:
 ~TFreeOleFormClass();
};
TFreeOleFormClass::~TFreeOleFormClass()
{
  if (OleForm) delete OleForm;
}
TFreeOleFormClass FreeOleFormClass;

// Данные на OLE объект
typedef struct OLEITEM
{
  WCHAR          FileName[MAX_PATH_LONG]; // имя файла
  TOleContainer *OleContainer;            // OLE контейнер
  unsigned int   CheckFileDate;           // дата последней проверки обновления файла
  unsigned int   CheckFileTime;           // время последней проверки обновления файла
  unsigned int   FileDate;                // дата последнего обновления файла
  unsigned int   FileTime;                // время последнего обновления файла
}
  OLEITEM;

// Список OLE объектов
typedef class TOleList : public TSimpleList
{
  public:

  TOleList() : TSimpleList(sizeof(OLEITEM)) {}
 ~TOleList();

  // Добавить запись
  int _fastcall Append(OLEITEM *item) {return TSimpleList::Append((char*) item, sizeof(OLEITEM));}

  // Запросить элемент
  // number - номер элемента, начиная с 1
  OLEITEM* _fastcall Item(int number) {return (OLEITEM*) TSimpleList::Item(number);}

  // Найти запись по имени OLE документа
  OLEITEM* _fastcall Find(const WCHAR *name);
}
  TOleList;

TOleList OleList;

//-----------------------------------------------------------------
// Освобождает ресурсы
//-----------------------------------------------------------------
TOleList::~TOleList()
{
  for (int i = 1; i <= ItemCount; i++)
  {
    OLEITEM *item = Item(i);
    if (item && item->OleContainer) delete item->OleContainer;
  }
}
//-----------------------------------------------------------------
// Ищет запись по имени файла
//-----------------------------------------------------------------
OLEITEM* _fastcall TOleList::Find(const WCHAR *name)
{
  for (int i = 1; i <= ItemCount; i++)
    {
      OLEITEM *item = Item(i);
      if (item && _wcsicmp(item->FileName, name) == 0) return item;
    }
  return 0;
}
//-----------------------------------------------------------------
// Отрисовка OLE объекта
//-----------------------------------------------------------------
bool _fastcall PaintOle(WCHAR *filename, int xmax, int xmin, int ymax, int ymin,
                        const IMGCONTEXT *context, int imgcolor, int transparent,
                        int test, FRAME *border)
{
  int     OleWidth, OleHeight, SourceWidth, SourceHeight;
  OLEITEM NewOleItem;

  bool           ExitCode     = false;
  TOleContainer *OleContainer = 0;

  // Если этот OLE объект ещё не грузился
  OLEITEM *OleItem = OleList.Find(filename);
  if (OleItem == 0)
    {
      // Если форма для размещения OLE контейнера не создавалась, то создаем её
      if (OleForm == 0)
        try {OleForm = new TForm((TComponent*) 0);}
        catch(...) {goto Exit;}

      // Создаем OLE контейнер и добавляем к невидимой форме
      // (если не добавить, то Link упадет)
      try {OleContainer = new TOleContainer(OleForm);}
      catch(...)
        {
          OleContainer = 0;
          goto Exit;
        }
      OleContainer->Parent = OleForm;

      // Линкуем контейнер с файлом
      try {OleContainer->CreateLinkToFile(filename, false);}
      catch (...) {goto Exit;}

      // Добавляем новый OLE объект в список
      WcsCopy(NewOleItem.FileName, filename, sizeof(NewOleItem.FileName));
      NewOleItem.OleContainer  = OleContainer;
      GetTheSystemDateAndTime(&NewOleItem.CheckFileDate, &NewOleItem.CheckFileTime);
      if (! GetTheFileSystemDateAndTime(filename, &NewOleItem.FileDate,
                                                  &NewOleItem.FileTime)) goto Exit;
      OleList.Append(&NewOleItem);
    }
  else
    {
      // Считываем OLE container
      OleContainer = OleItem->OleContainer;

      // Если пора проверять изменение файла
      unsigned int ThisDate, ThisTime;
      GetTheSystemDateAndTime(&ThisDate, &ThisTime);
      if (ThisDate != OleItem->CheckFileDate ||
          ThisTime - OleItem->CheckFileTime > CHECKFILESEC)
        {
          // Если файл изменился
          unsigned int FileDate, FileTime;
          if (GetTheFileSystemDateAndTime(OleItem->FileName, &FileDate, &FileTime) &&
             (FileDate != OleItem->FileDate || FileTime != OleItem->FileTime))
            {
              // Обновляем OLE объект в контейнере из файла
              OleContainer->UpdateObject();

              // Запоминаем новое время обновления файла
              OleItem->FileDate = FileDate;
              OleItem->FileTime = FileTime;
            }

          // Запоминаем время последней проверки обновления файла
          OleItem->CheckFileDate = ThisDate;
          OleItem->CheckFileTime = ThisTime;
        }
    }

  // Вычисляем габариты объекта в пикселях
  OleWidth  = xmax - xmin;
  OleHeight = ymax - ymin;

  // Получаем исходные габариты OLE объета в пикселях (переводим их из Himetric = 0.01 мм в пиксели устройства)
  SourceWidth  = MulDiv(OleContainer->FViewSize.x, GetDeviceCaps(context->Hdc, LOGPIXELSX), 2540);
  SourceHeight = MulDiv(OleContainer->FViewSize.y, GetDeviceCaps(context->Hdc, LOGPIXELSY), 2540);

  // Подгоняем габариты OLE объекта так, чтобы они уместились в габаритах объекта
  if (OleWidth * SourceHeight > OleHeight * SourceWidth)
    {
      SourceWidth  = (double) SourceWidth  * OleHeight / SourceHeight;
      SourceHeight = OleHeight;
    }
  else
    {
      SourceHeight = (double) SourceHeight * OleWidth / SourceWidth;
      SourceWidth  = OleWidth;
    }

  // Вычисляем габариты отрисовки OLE объекта
  RECT PaintRect;
  PaintRect.left   = xmin;
  PaintRect.top    = ymin;
  PaintRect.right  = PaintRect.left + SourceWidth;
  PaintRect.bottom = PaintRect.top  + SourceHeight;

  // Рисуем фон по габаритам OLE объекта
  if (imgcolor != -1)
    {
      IMGPOLYGONGLASS glass;
      glass.Color        = imgcolor;
      glass.Bright       = 0;
      glass.Contrast     = 0;
      glass.Transparency = 100;
      if (transparent >= 0 && transparent <= 100)
        glass.Transparency = transparent;

      int PointCount = 5;
      DRAWPOINT Points[5];
      Points[0].X = PaintRect.left;  Points[0].Y = PaintRect.top;
      Points[1].X = PaintRect.right; Points[1].Y = PaintRect.top;
      Points[2].X = PaintRect.right; Points[2].Y = PaintRect.bottom;
      Points[3].X = PaintRect.left;  Points[3].Y = PaintRect.bottom;
      Points[4].X = PaintRect.left;  Points[4].Y = PaintRect.top;

      POLYDATAEX PolyData;
      PolyData.Points     = Points;
      PolyData.PolyCounts = &PointCount;
      PolyData.Count      = 1;
      imgPrepareImage(&PolyData, IMG_POLYGONGLASS, (char*) &glass,
                      context, test, border);
    }

  // Рисуем OLE Объект
  OleDraw(OleContainer->OleObjectInterface, DVASPECT_CONTENT, context->Hdc, &PaintRect);

  ExitCode = true;
  Exit : if (! ExitCode)
           if (OleContainer) delete OleContainer;
         return ExitCode;
}
#endif  // WIN32API

// --------------------------------------------------------
// Окружность по двум точкам с градиентной заливкой с понижением
// от центра
// --------------------------------------------------------
int _fastcall GradientCircle(const POLYDATAEX* pdata, const IMGDRAW* draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border)
{
  // Не менее 2 точек
  if (*pdata->PolyCounts < 2)
    return imgPrepareImage(pdata, IMG_DRAW, (char *)draw,
                           context, test, border);
  POLYDATAEX data;
  DRAWPOINT points[400];

  // Настраиваем адреса метрики
  int number[2] = { 0, 0};
  data.Points     = points;
  data.PolyCounts = &number[0];
  data.Count      = 1;

  DRAWPOINT p1, p2;
  p1.x            = pdata->Points[0].x;
  p1.y            = pdata->Points[0].y;

  p2.x            = pdata->Points[1].x;
  p2.y            = pdata->Points[1].y;

  double temp = (p1.x - p2.x) * (p1.x - p2.x) + (p1.y - p2.y) * (p1.y - p2.y);
  if (temp <= 1)
    return imgPrepareImage(pdata, IMG_DRAW, (char *)draw,
                           context, test, border);
  int radius = (int)(sqrt(temp) + 0.5);
  if (radius < 1)
    return 0;

  // Внешний контур
  imlBuildCircle(&p1, radius, &data);

  data.Border.DN = p1.y + radius+1;
  data.Border.RT = p1.x + radius+1;
  data.Border.UP = p1.y - radius-1;
  data.Border.LT = p1.x - radius-1;

  IMGPOLYGONGLASS glass;

  // Цвет
  int * parm  = (int *)imlDrawParm(draw, 1);
  glass.Color = *parm;

  parm  = (int *)imlDrawParm(draw, 2);
  int minbright = *parm;

  parm  = (int *)imlDrawParm(draw, 3);
  int maxbright = *parm;
  glass.Bright  = *parm;

  glass.Contrast = 100;

  parm  = (int *)imlDrawParm(draw, 4);
  glass.Transparency = *parm;

  int zone = *((int *)imlDrawParm(draw, 5));

  double delta = 0;
  if (zone > 1)
    delta = ((double)radius) / zone;

  if (delta < 3)
    {
      zone  = radius / 3;
      if (zone < 1) zone = 1;
      delta = ((double)radius) / zone;
    }

  double deltabright = ((double)maxbright - minbright) / (double)zone;
  if (deltabright < 0.01)
    deltabright = 0.01;

  DRAWPOINT points2[200];

  int ret = 0;

  for (int i = 1; i <= zone; i++)
  {
    POLYDATAEX data2;
    int number2      = 0;
    data2.Points     = points2;
    data2.PolyCounts = &number2;
    data2.Count      = 1;

    int radius2      = (int)(radius - delta * i + 0.5);

    if (radius2 >= 3)
      {
        // Внутренний контур
        imlBuildCircle(&p1, radius2, &data2);

        if (number2 > 0)
          {
            // Дописать подобъект
            memcpy((char*)(points+number[0]), (char*)points2, number2*8);
            data.Count = 2;
            number[1]  = number2;
          }
      }

    ret |= imgPrepareImage(&data, IMG_POLYGONGLASS, (char *)&glass,
                           context, test, border);
    if (number2 <= 0)
      break;

    memcpy((char*)points, (char*)points2, number2*8);
    number[0]  = number2;
    number[1]  = 0;
    data.Count = 1;
    glass.Bright = (int)(maxbright - deltabright * i);
  }

  return ret;
}

// --------------------------------------------------------
// Подпись длины и угла векторного знака
// --------------------------------------------------------
int _fastcall LengthAndAngleLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                 const IMGCONTEXT * context, int test,
                                 FRAME * border)
{
  if (pdata->PolyCounts <= 0)
    return 0;

  if ((pdata->DX == 0) && (pdata->DY == 0))
    return 0;

  double scale = pdata->ShowScale; // context->Scale;  // ViewScale;       // 05/04/13
  if (scale < 0.0000001)
   scale = 1; // context->Scale;

  double DX = pdata->DX / context->PixelX * scale / 1000000.;
  double DY = pdata->DY / context->PixelY * scale / 1000000.;

  // Длина отрезка по карте
  double length = (DX * DX + DY * DY);
  if (length < 0.001)
    return 0;

  length = sqrt(length);

  // Если отрезок менее 0.3 см - не подписывать
  if ((pdata->ShowScale > 1) && ((length * 100.0 / pdata->ShowScale) < 0.3))
    return 0;

  int fixed = 0;

  if (imlDrawType(draw, 2) == IMG_LONG)
   {
     long * parm = (long *)imlDrawParm(draw, 2);
     if (parm)
       fixed = *parm;
   }
    
  if (fixed < 0)
    fixed = 0;
  else
   if (fixed > 6)
     fixed = 6;

  TEXTDATA buffer;


  if (fixed > 0)
    {
      char format[16];

      // %.3f
      sprintf(format, "%s%uf", "%.", fixed);
      SpaceInsert(buffer.Text, format, length);
    }
  else
    {
      itoa((int)(length+0.5), buffer.Text, 10);               // 22/07/15

      // Формирование строки со вставкой разделяющих пробелов
      InsertSeparatingBlanks(buffer.Text);
    }

  buffer.Zero = 0;
  buffer.Length = strlen(buffer.Text);

  if (imlDrawType(draw, 1) == IMG_TEXT)
    {
      IMGTEXT * parm = (IMGTEXT *)imlDrawParm(draw, 1);
      if (parm)
        parm->Align = FA_CENTER | FA_BOTTOM;

      POLYDATAEX data = *pdata;
      char * text     = buffer.Text;
      data.Text       = &text;
      data.AlignCode  = 0;

      return imgPrepareImage(&data, IMG_TEXT, (char *)parm,
                             context, test, border);
    }

  return 0; 
}

// --------------------------------------------------------
// Условный знак вдоль линии на заданном интервале
// --------------------------------------------------------
int _fastcall BuildMarkAlongLine(const POLYDATAEX* pdata,
                                 int markfunc, const char * markparm,
                                 int distance,
                                 const IMGCONTEXT * context, int test,
                                 FRAME * border)
{
  POLYDATAEX   data;
  DRAWPOINT    points[2];
  int    number;                      // 26/03/12
  int          length, tail;
  int          current;
  int          dx,dy;
  DRAWPOINT *  begin;
  int          ret = 0;

  data = *pdata;                               // 05/08/05

  // Настраиваем адреса исходной метрики
  DRAWPOINT * pointin = (DRAWPOINT *)(pdata->Points);
  int *  counts  = pdata->PolyCounts;            // 26/03/12
  int count           = pdata->Count;

  if (distance < 1) return 0;                   // 01/03/02

  // Настраиваем адреса выходной метрики
  data.Points     = points;
  data.PolyCounts = &number;
  data.Count      = 1;

  // Отрисовка знака вдоль линии на заданной дистанции
  while(count-- > 0)                                      // 01/03/01
  {
    int polycount = *counts;  // Установим число точек в ломанной

    counts++;                 // Переходим к следуюшей ломаной

    if ((polycount < 1) || (polycount > 200000))
      return ret;      // Ошибка в данных

    points[0].x = pointin->x;
    points[0].y = pointin->y;
    points[1].x = pointin->x;
    points[1].y = pointin->y;

    if (polycount == 1)
      {
         number = 1;
         ret |= imgPrepareImage(&data, markfunc, markparm,
                                      context, test, border);

         if ((ret != 0) && (test == PREPARE_TEST))
           return 1;

        pointin++;
        continue;
      }

    number    = 2;
    current   = distance;
    polycount--;

    while(polycount-- > 0)
    {
      begin  = pointin;

      // Перейти к следующей точке
      pointin++;

      points[0].x = begin->x;
      points[0].y = begin->y;

      // Размер отрезка
      dx     = pointin->x - points[0].x;
      dy     = pointin->y - points[0].y;
      double temp   = dx*dx + dy*dy;
      if (temp < 1)                              // 04/04/02
        continue;
      length = (sqrt(temp) + 0.5);
      tail   = length;

      // Вторую точку вытянем по вектору
      points[1].x = pointin->x + dx;
      points[1].y = pointin->y + dy;

      while(1)
      {
        if (current >= distance)
          {
            current = 0;
            ret |= imgPrepareImage(&data, markfunc, markparm,
                                   context, test, border);

            if ((ret != 0) && (test == PREPARE_TEST))
              {
                return 1;
              }
          }

        if ((current + tail) >= distance)
          {
            // Отщипнем от длины отрезка недостающий кусочек
            tail   -= (distance - current);
            current = distance;

            // Определим координаты первой точки
            // Вторая постоянная для ориентации вдоль отрезка
            points[0].x = begin->x + dx * (length - tail)/length;
            points[0].y = begin->y + dy * (length - tail)/length;
          }
        else
          {
            // Учтем оставшуюся длину текущего отрезка
            current += tail;

            // Идем к следующему отрезку
            break;
          }
      }
     }

   // Перейти к следующей точке                     // 11/02/04
   pointin++;
  }

 return ret;
}

// --------------------------------------------------------
// Смещенный условный знак вдоль линии на заданном интервале
// --------------------------------------------------------
int _fastcall ShiftMarkAlongLine(int shift, const POLYDATAEX* pdata,
                                 const IMGDRAW* draw,
                                 const IMGCONTEXT * context, int test,
                                 FRAME * border, int mirror)
{
 const char * parm;
 const char * markparm;
 int   markfunc;
 const char * lineparm;
 int   linefunc;
 long  distance = 16;         // Расстояние между знаками
 int   ret = 0;

 if (pdata == 0) return 0;

 POLYDATAEX   shiftdata = *pdata;
 int          shiftcount;
 int  *  shiftpolycounts;
 DRAWPOINT *  shiftpoint;
 int     shiftnumber;

 shiftcount      = pdata->Count;
 shiftpolycounts = pdata->PolyCounts;
 shiftpoint      = (DRAWPOINT *)(pdata->Points);

 if (shift)
   {
     shiftdata.Count      = 1;
     shiftdata.PolyCounts = &shiftnumber;
   }

 // Тип функции
 markfunc = imlDrawType(draw, 1);

 // Параметры функции
 markparm = imlDrawParm(draw, 1);

 if (mirror)
   {
     if (markfunc == IMG_VECTOREX)
       markfunc = IMG_VECTOREXTURN;                // 13/03/02
   }

 if (imlDrawType(draw, 2) == IMG_DOUBLE)
   {
     if ((parm = imlDrawParm(draw, 2)) != 0)
       {
         // Интервал в мм
#ifdef RISCCPU                                            // 25/01/05
         DOUBLEREGISTER regvalue;
         DoubleToRegister(regvalue, parm);
         double  value = regvalue.Double;
#else
         double value = ((IMGDOUBLE *)parm)->Value;
#endif
         if ((value < 0.1) || (value > 200))              // 11/03/02
           value = 4;

         // Интервал в точках (из мм)
         distance = (context->Scale * context->PixelX * value * 1000.0);
       }
   }

 while(shiftcount-- > 0)
 {
   if (shift)
     {
       int count   = *shiftpolycounts;
       shiftnumber = BuildShiftLine(&(shiftdata.Points), shiftpoint,
                                    count, shift);
       shiftpoint += count;
       shiftpolycounts++;
     }

   // Нарисовать фоновую линию, если есть параметры
   if ((linefunc = imlDrawType(draw, 4)) != 0)
    {
     // Параметры фоновой линии
     lineparm = imlDrawParm(draw, 4);

     ret |= imgPrepareImage(&shiftdata, linefunc, lineparm,
                                      context, test, border);

     if ((ret != 0) && (test == PREPARE_TEST))
       {
         return 1;
       }
    }

   ret |= BuildMarkAlongLine(&shiftdata, markfunc, markparm, distance,
                             context, test, border);

   if ((ret != 0) && (test == PREPARE_TEST))
     {
       return 1;
     }

   // Нарисовать верхнюю линию, если есть параметры
   if ((linefunc = imlDrawType(draw, 5)) != 0)
   {
     // Параметры фоновой линии
     lineparm = imlDrawParm(draw, 5);

     ret |= imgPrepareImage(&shiftdata, linefunc, lineparm,
                                      context, test, border);

     if ((ret != 0) && (test == PREPARE_TEST))
       {
         return 1;
       }
   }
 }

 return ret;
}

// --------------------------------------------------------
// Условный знак вдоль линии на заданном интервале
// --------------------------------------------------------
int _fastcall MarkAlongLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                            const IMGCONTEXT * context, int test,
                            FRAME * border)
{
 int shift;
 const char * parm;

 if (imlDrawType(draw, 3) == IMG_DOUBLE)
   {
     if ((parm = imlDrawParm(draw, 3)) != 0)
       {
#ifdef RISCCPU                         // 25/01/05
         DOUBLEREGISTER regvalue;
         DoubleToRegister(regvalue, parm);
         double  temp = regvalue.Double;
#else
         double temp = ((IMGDOUBLE *)parm)->Value;
#endif
         if ((temp < -50) || (temp > 50))              // 11/03/02
           temp = 1.0;

         // Интервал в точках (из мм)
         shift = (context->Scale * context->PixelX * temp * 1000.0);
       }
   }

 return ShiftMarkAlongLine(shift, pdata, draw, context, test, border, 1);
}

// --------------------------------------------------------
// Зеркальный знак вдоль линии на заданном интервале
// --------------------------------------------------------
int _fastcall MirrorMarkAlongLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                  const IMGCONTEXT * context, int test,
                                  FRAME * border)
{
 int shift;
 const char * parm;
 int   ret = 0;

 if (imlDrawType(draw, 3) == IMG_DOUBLE)
   {
     if ((parm = imlDrawParm(draw, 3)) != 0)
       {
#ifdef RISCCPU                         // 19/01/05
         DOUBLEREGISTER regvalue;
         DoubleToRegister(regvalue, parm);
         double  temp = regvalue.Double;
#else
         double temp = ((IMGDOUBLE *)parm)->Value;
#endif
         if ((temp < -50) || (temp > 50))              // 11/03/02
           temp = 1.0;

         // Интервал в точках (из мм)
         shift = (context->Scale * context->PixelX * temp * 1000.0);
       }
   }

 ret |= ShiftMarkAlongLine(shift, pdata, draw, context, test, border, 1);

 if ((ret != 0) && (test == PREPARE_TEST))
   {
      return 1;
   }

 shift = -shift;
 return ShiftMarkAlongLine(shift, pdata, draw, context, test, border, 0);
}

// ------------------------------------------------------------
// Условный знак внутри зеркальной линии на заданном интервале
// ------------------------------------------------------------
int _fastcall MarkIntoLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
 const char * parm;
 const char * markparm;
 int          markfunc;
 const char * lineparm;
 int          linefunc;
 long         distance = 16;         // Расстояние между знаками
 int          ret = 0;

 POLYDATAEX   shiftdata;
 int          shiftcount;
 int  *  shiftpolycounts;
 DRAWPOINT *  shiftpoint;
 int     shiftnumber;
 int          shift;
 int          i;

 if (pdata == 0) return 0;

 shiftdata = *pdata;

 // Тип функции
 markfunc = imlDrawType(draw, 1);

 // Параметры функции
 markparm = imlDrawParm(draw, 1);

 if (imlDrawType(draw, 2) == IMG_DOUBLE)
   {
     if ((parm = imlDrawParm(draw, 2)) != 0)
       {
         // Интервал в мм
#ifdef RISCCPU                         // 25/01/05
         DOUBLEREGISTER regvalue;
         DoubleToRegister(regvalue, parm);
         double  value = regvalue.Double;
#else
         double value = ((IMGDOUBLE *)parm)->Value;
#endif
         if ((value < 0.1) || (value > 200))             // 11/03/02
           value = 4;

         // Интервал в точках (из мм)
         distance = (context->Scale * context->PixelX * value * 1000.0);
       }
   }

 if (imlDrawType(draw, 3) == IMG_DOUBLE)
   {
     if ((parm = imlDrawParm(draw, 3)) != 0)
       {
#ifdef RISCCPU                                         // 25/01/05
         DOUBLEREGISTER regvalue;
         DoubleToRegister(regvalue, parm);
         double  temp = regvalue.Double;
#else
         double temp = ((IMGDOUBLE *)parm)->Value;
#endif
         if ((temp < -50) || (temp > 50))              // 11/03/02
           temp = 1.0;

         // Смещение в точках (из мм)
         shift = (context->Scale * context->PixelX * temp * 1000.0);
       }
   }

 if (shift)
   {
     shiftdata.Count      = 1;
     shiftdata.PolyCounts = &shiftnumber;
   }

 // Тип функции линии
 linefunc = imlDrawType(draw, 4);

 // Параметры линии
 lineparm = imlDrawParm(draw, 4);

 for (i = 1; i <= 2; i++)
 {
  shiftcount      = pdata->Count;
  shiftpolycounts = pdata->PolyCounts;
  shiftpoint      = (DRAWPOINT *)(pdata->Points);

  while(shiftcount-- > 0)
  {
   if (shift)
     {
       int count   = *shiftpolycounts;
       shiftnumber = BuildShiftLine(&(shiftdata.Points), shiftpoint,
                                    count, shift);
       shiftpoint += count;
       shiftpolycounts++;
     }

   // Нарисовать линию, если есть параметры
   if (linefunc != 0)
    {
     ret |= imgPrepareImage(&shiftdata, linefunc, lineparm,
                            context, test, border);

     if ((ret != 0) && (test == PREPARE_TEST))
       {
         return 1;
       }
    }
  }

  if (shift == 0)
    break;

  shift = -shift;
 }

 ret |= BuildMarkAlongLine(pdata, markfunc, markparm, distance,
                           context, test, border);
 return ret;
}

// --------------------------------------------------------
// Условный знак с поворотом по семантике
// --------------------------------------------------------
int _fastcall TurnMark(const POLYDATAEX* pdata, const IMGDRAW* draw,
                       const IMGCONTEXT * context, int test,
                       FRAME * border)
{
 const char * vecparm;
 int    vecfunc;
 int    semanticnumber = 333;

 POLYDATAEX data;
 DRAWPOINT Points[2];
 int Number;
 double angle;

 if (pdata == 0) return 0;

 if (imlDrawType(draw, 2) == IMG_SEMANTIC)               // 20/02/02
   {
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 2);
     if (parm)
       semanticnumber = parm->Value;
   }

 if (semanticnumber > 0)
 {
  Tracer1("semanticnumber = %i\n", semanticnumber);

  // Запросить значение характеристики с кодом semanticnumber (градусы)
  // (первое значение, если их несколько)
  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,
                            semanticnumber, 1, &angle) != 0)
   {
     // Длина отрезка - 3 см на карте (условно)       // 09/06/06
     int Length = context->Scale * context->PixelX * 30000;

     Tracer1("source angle = %lf\n", angle);

     data = *pdata;

     // Настраиваем адреса метрики
     data.Points     = Points;
     data.PolyCounts = &Number;
     Number          = 2;
     data.Count      = 1;

     Points[0].x     = pdata->Points[0].x;
     Points[0].y     = pdata->Points[0].y;

     // Перевести в радианы
     angle = angle * M_PI / 180.0;

     // Учитываем поворот карты 11/10/01
     angle += context->Angle;

     Tracer1("target angle = %lf\n", angle);

     Points[1].x = Points[0].x + Length * cos(angle);
     Points[1].y = Points[0].y + Length * sin(angle);

     pdata = &data;
   }
 }

 // Тип функции
 vecfunc = imlDrawType(draw, 1);

 // Параметры функции
 vecparm = imlDrawParm(draw, 1);

 // Отобразить объект
 return imgPrepareImage(pdata, vecfunc, vecparm, context, test, border);
}

// --------------------------------------------------------
// Условный знак с длиной и поворотом по семантике  // 14/10/08
// --------------------------------------------------------
int _fastcall TurnAndLengthMark(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                const IMGCONTEXT * context, int test,
                                FRAME * border)
{
 const char * vecparm;
 int    vecfunc;
 int    semanticnumber = 0;

 POLYDATAEX data;
 DRAWPOINT Points[2];
 int Number;

 double angle = 0;
 double length = 30000;      // 3 см в базовом масштабе карты
 int isupdate = 0;

 if (pdata == 0) return 0;

 if (imlDrawType(draw, 3) == IMG_SEMANTIC)
   {
     int number = 0;
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 3);
     if (parm)
       number = parm->Value;

     if (number > 0)
       {
         // Запросить значение характеристики с кодом number (длина в мм)
         // (первое значение, если их несколько)
         if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,
                                  number, 1, &length) != 0)
           {
             isupdate = 1;

             if (length <= 0)
               length = 30000;
             else
               if (length > 300)
                 {
                   // Не более 30 см
                   length = 300000;
                 }
               else
                 {
                   // Переведем в мкм
                   length = length * 1000;
                 }
           }
       }
   }

 if (imlDrawType(draw, 2) == IMG_SEMANTIC)               // 20/02/02
   {
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 2);
     if (parm)
       semanticnumber = parm->Value;
   }

 if ((semanticnumber > 0) &&
     // Запросить значение характеристики с кодом semanticnumber (градусы)
     // (первое значение, если их несколько)
     (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,
                            semanticnumber, 1, &angle) != 0))
   {
     Tracer1("semanticnumber = %i\n", semanticnumber);

     isupdate = 1;

     angle -= 90.0;

     // Перевести в радианы
     angle = angle * M_PI / 180.0;

     // Учитываем поворот карты 11/10/01
     angle += context->Angle;
   }
 else
   {
     // ...
   }

 if (isupdate)
   {
     // Длина отрезка на карте
     int Length = context->Scale * context->PixelX * length;

     Tracer1("source angle = %lf\n", angle);

     data = *pdata;

     // Настраиваем адреса метрики
     data.Points     = Points;
     data.PolyCounts = &Number;
     Number          = 2;
     data.Count      = 1;

     Points[0].x     = pdata->Points[0].x;
     Points[0].y     = pdata->Points[0].y;

     Tracer1("target angle = %lf\n", angle);

     Points[1].x = Points[0].x + Length * cos(angle);
     Points[1].y = Points[0].y + Length * sin(angle);

     pdata = &data;
   }

 // Тип функции
 vecfunc = imlDrawType(draw, 1);

 // Параметры функции
 vecparm = imlDrawParm(draw, 1);

 // Отобразить объект
 return imgPrepareImage(pdata, vecfunc, vecparm, context, test, border);
}


// --------------------------------------------------------
// Текст с параметрами из семантики
// --------------------------------------------------------
int _fastcall UniversalText(const POLYDATAEX* pdata, const IMGDRAW* draw,
                            const IMGCONTEXT * context, int test,
                            FRAME * border)
{
 const char * textparm;
 int     textfunc;
 int     semanticnumber = 0;
 int    color;
 IMGTRUETEXT parm;

 // Тип функции
 textfunc = imlDrawType(draw, 1);

 // Параметры функции
 textparm = imlDrawParm(draw, 1);

 if (textfunc != IMG_TEXT)
   {
     // Отобразить объект
     return imgPrepareImage(pdata, textfunc, textparm, context, test, border);
   }

 memcpy((void *)&(parm.Text), textparm, sizeof(parm.Text));

 parm.Text.CharSet = RUSSIAN_CHARSET;
 strcpy(parm.FontName, "Arial");


 textparm = (const char *)&parm;
 textfunc = IMG_TRUETEXT;

 if (imlDrawType(draw, 2) == IMG_SEMANTIC)
   {
     IMGLONG * imgparm = (IMGLONG *)imlDrawParm(draw, 2);
     if (imgparm)
       semanticnumber = imgparm->Value;
   }

 if (semanticnumber > 0)
 {
  double hieght = 0;

  // Запросить значение характеристики с кодом semanticnumber (высота в мм)
  // (первое значение, если их несколько)
  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,
                            semanticnumber, 1, &hieght) != 0)
   {
     // От 0 до 50 см
     if ((hieght > 0) && (hieght < 500))
       {
         parm.Text.Height = (int)(hieght * 1000.0);
       }
   }
 }

 if (imlDrawType(draw, 3) == IMG_SEMANTIC)
   {
     char value[32];
     IMGLONG * imgparm = (IMGLONG *)imlDrawParm(draw, 3);
     if (imgparm)
       semanticnumber = imgparm->Value;

     // Запросить значение характеристики с кодом
     // semanticnumber (имя шрифта)
     // (первое значение, если их несколько)
     if (imlGetSemanticString((SEMANTIC *)(pdata->Semantic),
                              semanticnumber, 1, value,
                              sizeof(value)) != 0)
       {
         strncpy(parm.FontName, value, sizeof(parm.FontName));
         parm.FontName[sizeof(parm.FontName)-1] = 0;
         parm.Text.Service = 0;
       }
     else
       {
         textfunc = IMG_TEXT;
       }
   }
 else
   {
     textfunc = IMG_TEXT;
   }

 long int color_l = color;            // 26/03/12

 // Запросить значение цвета, если есть
 if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic),
                        SEMIMAGECOLOR, 1, &color_l) != 0)
   {
     parm.Text.Color = color_l;                  // 18/08/05
   }
   
 // Отобразить объект
 return imgPrepareImage(pdata, textfunc, textparm, context, test, border);
}

// --------------------------------------------------------
// Масштабируемый векторный знак         // 15/08/03
// --------------------------------------------------------
int _fastcall ScaleVector(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border)
{
 const char * vecparm;
 int          vecfunc;
 int          semanticnumber = 0;

 // Тип функции
 vecfunc = imlDrawType(draw, 1);

 // Параметры функции
 vecparm = imlDrawParm(draw, 1);

 if (vecfunc != IMG_VECTOREX)
   {
     // Отобразить объект
     return imgPrepareImage(pdata, vecfunc, vecparm, context, test, border);
   }

 if (imlDrawType(draw, 2) == IMG_SEMANTIC)
   {
     IMGLONG * imgparm = (IMGLONG *)imlDrawParm(draw, 2);
     if (imgparm)
       semanticnumber = imgparm->Value;
   }

 if (semanticnumber > 0)
   {
     long percent;
     float scale;

     // Запросить значение характеристики с кодом semanticnumber
     // (процент масштабирования) (первое значение, если их несколько)
     if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) ,
                            semanticnumber, 1, &percent) != 0)
       {
         int ret;
         int vcopy, hcopy, bcopy;

         if (percent < 10)
           percent = 10;
         else
          if (percent > 1000)
            percent = 1000;

         scale = (float)percent/100.0;

         vcopy = ((IMGVECTOREX *)vecparm)->VSize;
         hcopy = ((IMGVECTOREX *)vecparm)->HSize;
         bcopy = ((IMGVECTOREX *)vecparm)->Border;

         ((IMGVECTOREX *)vecparm)->VSize = ((IMGVECTOREX *)vecparm)->VSize * scale;
         ((IMGVECTOREX *)vecparm)->HSize = ((IMGVECTOREX *)vecparm)->HSize * scale;

         ((IMGVECTOREX *)vecparm)->Border= ((IMGVECTOREX *)vecparm)->Border* scale;

         ret = imgPrepareImage(pdata, vecfunc, vecparm, context, test, border);

         ((IMGVECTOREX *)vecparm)->VSize = vcopy;
         ((IMGVECTOREX *)vecparm)->HSize = hcopy;
         ((IMGVECTOREX *)vecparm)->Border= bcopy;

         return ret;
       }
   }

 // Отобразить объект
 return imgPrepareImage(pdata, vecfunc, vecparm, context, test, border);
}

int _fastcall AllThickenLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border, int type);

// --------------------------------------------------------
// Утолщающаяся линия в конце
// --------------------------------------------------------
int _fastcall ThickenLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border)
{
  int type = IMG_THICKENLINE;
  return AllThickenLine(pdata, draw, context, test, border, type);
}

// --------------------------------------------------------
// Утолщающаяся линия в середине
// --------------------------------------------------------
int _fastcall MiddleThickenLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                const IMGCONTEXT * context, int test,
                                FRAME * border)
{
  int type = IMG_MIDDLETHICKENLINE;
  return AllThickenLine(pdata, draw, context, test, border, type);
}

// --------------------------------------------------------
// Утолщающаяся линия
// --------------------------------------------------------
int _fastcall AllThickenLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border, int type)
{
 IMGTHICKENLINE thickenparm;
 int semanticnumber;
 const char * lineparm;
 int          linefunc;

 thickenparm.Color  = 0;
 thickenparm.Thick  = 255;
 thickenparm.Thick2 = 512;

 // Тип функции
 linefunc = imlDrawType(draw, 1);

 // Параметры функции
 lineparm = imlDrawParm(draw, 1);

 if (linefunc == IMG_LINE)
   {
     thickenparm.Color = ((IMGLINE*)lineparm)->Color;
     thickenparm.Thick = ((IMGLINE*)lineparm)->Thick;
   }

 thickenparm.Thick2 = thickenparm.Thick;

 if (imlDrawType(draw, 2) == IMG_SEMANTIC)
   {
     double thick;

     IMGLONG * imgparm = (IMGLONG *)imlDrawParm(draw, 2);
     if (imgparm)
       semanticnumber = imgparm->Value;

     // Запросить значение характеристики с кодом semanticnumber
     // (процент масштабирования) (первое значение, если их несколько)
     if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,
                               semanticnumber, 1, &thick) != 0)
       {
         // мм в микроны
         thickenparm.Thick2 = thick * 1000;
       }
   }

 // Отобразить объект
 return imgPrepareImage(pdata, type, (char*)&thickenparm, context, test, border);
}

// --------------------------------------------------------
// Утолщающаяся линия от начала до конца      // 29/03/06
// --------------------------------------------------------
int _fastcall DoubleThickenLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                const IMGCONTEXT * context, int test,
                                FRAME * border)
{
 IMGTHICKENLINE thickenparm;
 int semanticnumber;
 const char * lineparm;
 int          linefunc;

 thickenparm.Color  = 0;
 thickenparm.Thick  = 255;
 thickenparm.Thick2 = 512;

 // Тип функции
 linefunc = imlDrawType(draw, 1);

 // Параметры функции
 lineparm = imlDrawParm(draw, 1);

 if (linefunc == IMG_LINE)
   {
     thickenparm.Color = ((IMGLINE*)lineparm)->Color;
     thickenparm.Thick = ((IMGLINE*)lineparm)->Thick;
   }

 if (imlDrawType(draw, 2) == IMG_SEMANTIC)
   {
     double thick;

     IMGLONG * imgparm = (IMGLONG *)imlDrawParm(draw, 2);
     if (imgparm)
       semanticnumber = imgparm->Value;

     // Запросить значение характеристики с кодом semanticnumber
     // (процент масштабирования) (первое значение, если их несколько)
     if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,
                               semanticnumber, 1, &thick) != 0)
       {
         // мм в микроны
         thickenparm.Thick = thick * 1000;
       }
   }

 thickenparm.Thick2 = thickenparm.Thick;

 if (imlDrawType(draw, 3) == IMG_SEMANTIC)
   {
     double thick;

     IMGLONG * imgparm = (IMGLONG *)imlDrawParm(draw, 3);
     if (imgparm)
       semanticnumber = imgparm->Value;

     // Запросить значение характеристики с кодом semanticnumber
     // (процент масштабирования) (первое значение, если их несколько)
     if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,
                               semanticnumber, 1, &thick) != 0)
       {
         // мм в микроны
         thickenparm.Thick2 = thick * 1000;
       }
   }

 // Отобразить объект
 return imgPrepareImage(pdata, IMG_THICKENLINE, (char*)&thickenparm, context, test, border);
}

// --------------------------------------------------------
// Знак с выноской
// --------------------------------------------------------
int _fastcall MarkAndNote(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border)
{
 int          ret;
 int          markfunc;
 const char * markparm;
 int          linefunc;
 const char * lineparm;
 int          textfunc;
 const char * textparm;

 int          semanticnumber;
 int          first;
 int          last;
 int          i, j;
 int          delta;

 DRAWPOINT    points[2];
 struct
 {
//   char         length;
//   char          text[300];
   unsigned char length;
   char          text[256];  // 12/11/15
 }
   datatext;


 POLYDATAEX data = *pdata;

 // Настраиваем адреса выходной метрики
 data.Points     = (DRAWPOINT *)(pdata->Points);
 int number      = 1;
 data.PolyCounts = &number;
 data.Count      = 1;

 // Тип функции (знак)
 markfunc = imlDrawType(draw, 1);

 // Параметры функции (знак)
 markparm = imlDrawParm(draw, 1);

 // Тип функции (линия)
 linefunc = imlDrawType(draw, 2);

 // Параметры функции (линия)
 lineparm = imlDrawParm(draw, 2);

 // Тип функции (текст)
 textfunc = imlDrawType(draw, 3);

 // Параметры функции (текст)
 textparm = imlDrawParm(draw, 3);

 // Число точек
 int count = *(pdata->PolyCounts);

 if (count > 1)
   {
     // Отобразить линию
     ret = imgPrepareImage(pdata, linefunc, lineparm, context, test, border);

     if ((ret != 0) && (test == PREPARE_TEST))
       {
         return 1;
       }
   }

 // Отобразить объект
 ret = imgPrepareImage(&data, markfunc, markparm, context, test, border);

 if ((ret != 0) && (test == PREPARE_TEST))
   {
     return 1;
   }

 // Запросить семантику числителя
 if (imlDrawType(draw, 4) != IMG_SEMANTIC)
   return 0;

 IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 4);
 if (parm)
   semanticnumber = parm->Value;
 else
   semanticnumber = 0;

 // Метрика подписей
 if (count > 2)
   {
     // Предпоследняя точка
     first = count - 2;
     last  = count - 1;

     if (pdata->Points[last].X < pdata->Points[first].X)
       {
         first = count - 1;
         last  = count - 2;
       }

      delta = 1;
   }
 else
   {
     // Последняя точка
     first = 0;
     last  = 0;
     delta = 3;                 // 12/12/05
   }

 // Интервал в точках (из мм)
 delta = (context->Scale * context->PixelX * delta * 1000.0);

 data.Points     = points;
 number          = 1;

 points[0].X     = pdata->Points[first].X + delta * context->Scale;
 points[0].Y     = pdata->Points[first].Y;

 if (last != 0)
   {
     points[1].X     = pdata->Points[last].X;
     points[1].Y     = pdata->Points[last].Y;
     number          = 2;
   }

 char * point     = datatext.text;
 data.Text        = &point;
 datatext.text[0] = 0;
 data.AlignCode   = 1;


 if (semanticnumber > 0)
 {
  // Запросить значение характеристики с кодом semanticnumber
  // (первое значение, если их несколько)
  if (imlGetSemanticStringRsc(pdata, semanticnumber, 1, datatext.text,
//                              sizeof(datatext.text)) != 0)
                              sizeof(datatext.text)-2) != 0)  // 12/11/15 место для кода выравнивания
   {
     datatext.length = strlen(datatext.text);                            // 26/05/13
/*  // 12/11/15
     // Проверить, что длина не слишком большая
     if ((int)datatext.length > ((int)sizeof(datatext.text)-4))
       {
         datatext.length = sizeof(datatext.text)-4;
         datatext.text[datatext.length] = 0;
       }
*/
     // Записать код выравнивания после '\0' и увеличить длину записи
     datatext.length += 2;  // 253 + 2 = 255 (максимум)
     datatext.text[datatext.length-1] = CA_BOTTOM_LEFT;
     datatext.text[datatext.length]   = 0;

     // Отобразить комментарии числителя на выноске
     ret = imgPrepareImage(&data, textfunc, textparm, context, test, border);

     if ((ret != 0) && (test == PREPARE_TEST))
       {
         return 1;
       }
   }
 }

 datatext.text[0] = 0;
 j = 0;

 // Собрать текст знаменателя
 for (i = 0; i < 7; i++)
 {
   IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 5 + i);
   if (parm)
     semanticnumber = parm->Value;
   else
     break;

   // Запросить значение характеристики с кодом semanticnumber
   // (первое значение, если их несколько)
   if (imlGetSemanticStringRsc(pdata, semanticnumber, 1,
                               point, sizeof(datatext.text)-j) != 0)
     {
       strcat(datatext.text, "\n");

       j = strlen(datatext.text);
       if (j > 250)
         break;

       point = datatext.text + j;
     }
 }

 point   = datatext.text;

 if (datatext.text[0] != 0)
   {
     datatext.length = strlen(datatext.text) + 2;
     datatext.text[datatext.length-1] = CA_TOP_LEFT;

     // Отобразить комментарии знаменателя на выноске
     ret = imgPrepareImage(&data, textfunc, textparm, context, test, border);
   }

 return ret;
}

// --------------------------------------------------------
// Дуга по ломанной линии (параллели, меридианы и пр.)  02/04/06
// --------------------------------------------------------
int _fastcall PolyArc(const POLYDATAEX* pdata, const IMGDRAW* draw,
                      const IMGCONTEXT * context, int test,
                      FRAME * border)
{
 const char * lineparm;
 int          linefunc;
 DRAWPOINT    points[200];
 int    number;
 int          ret = 0;

 POLYDATAEX   data;

 if (pdata == 0) return 0;

 // Тип функции
 linefunc = imlDrawType(draw, 1);

 // Параметры функции
 lineparm = imlDrawParm(draw, 1);

 // Настраиваем адреса исходной метрики
 DRAWPOINT * pointin = (DRAWPOINT *)(pdata->Points);
 int *  counts  = pdata->PolyCounts;
 int count           = pdata->Count;

 // Настраиваем адреса выходной метрики
 data.Points     = points;
 data.PolyCounts = &number;
 data.Count      = 1;

 while(count-- > 0)
  {
    int polycount = *counts;  // Установим число точек в ломанной

    counts++;                      // Переходим к следуюшей ломаной

    if ((polycount < 1) || (polycount > 200000))
      return ret;      // Ошибка в данных

    if (polycount < 3)
      {
         // Рисуем отрезок
         data.Points     = pointin;
         number          = polycount;

         ret |= imgPrepareImage(&data, linefunc, lineparm,
                                context, test, border);

         if ((ret != 0) && (test == PREPARE_TEST))
           return 1;

         data.Points     = points;

         pointin += polycount;
         continue;
      }

    while(polycount-- > 2)
    {
      DOUBLEPOINT   p1;
      DOUBLEPOINT   p2;
      DOUBLEPOINT   p3;
      DOUBLEPOINT   pc;
      double        radius;

      // Найти центр окружности по трем точкам
      p1.X = pointin->x;
      p1.Y = pointin->y;
      p2.X = (pointin+1)->x;
      p2.Y = (pointin+1)->y;
      p3.X = (pointin+2)->x;
      p3.Y = (pointin+2)->y;

      radius = imlCircleCenter(&p1, &p2, &p3, &pc);

      if (radius > 0)
        {
          DRAWPOINT * pb = pointin;
          DRAWPOINT * pe = pointin+1;
          DRAWPOINT   pv;
          if (polycount <= 2)
            pe = pointin+2;

          double dx     = pe->x - pb->x;
          double dy     = pe->y - pb->y;
          double length = sqrt(dx * dx + dy * dy) * 15;
          if (length < radius)
            {
              radius = 0;
            }
          else
            {
              pv.x = pc.X;
              pv.y = pc.Y;

              // Определим направление окружности
              if (imlPointSideNearLine(&p2, &p1, &p3) == imlLEFT)
                {
                  // Развернем окружность, она обычно идет по часовой
                  DRAWPOINT * pt = pb;
                  pb = pe;
                  pe = pt;
                }

              // Строим по первым двум точкам дугу
              if (imlBuildArc(pb, &pv, pe, radius, &data) == 0)
                {
                  radius = 0;
                }
            }
        }

      if (radius <= 0)
        {
          points[0].X = pointin->x;
          points[0].Y = pointin->y;
          points[1].X = (pointin+1)->x;
          points[1].Y = (pointin+1)->y;

          // Соединяем точки прямой линией
          if (polycount == 2)
            {
              number = 3;
              points[2].X = (pointin+2)->x;
              points[2].Y = (pointin+2)->y;
            }
          else
              number = 2;
        }

      ret |= imgPrepareImage(&data, linefunc, lineparm,
                             context, test, border);

      if ((ret != 0) && (test == PREPARE_TEST))
        return 1;

      // Перейти к следующей паре точек
      if (polycount <= 2)
        {
          pointin   += 3;
          break;
        }
      else
        {
          pointin++;
        }
    }
  }

 return ret;
}

//----------------------------------------------------------------
// Условный знак, видимый в заданном диапазоне масштабов отображения
//----------------------------------------------------------------
int _fastcall ScaleLimitedImage(const POLYDATAEX* pdata, const IMGDRAW* draw,
                                const IMGCONTEXT * context, int test,
                                FRAME * border)
{
 if (pdata == 0) return 0;

 IMGLINE line;

 int minimum = 0;
 int maximum = 200000000;

 IMGLONG * sparm = (IMGLONG *)imlDrawParm(draw, 1);
 if (sparm)
   minimum = sparm->Value;

 sparm = (IMGLONG *)imlDrawParm(draw, 2);
 if (sparm)
   maximum = sparm->Value;

 if (test != PREPARE_TEST)
   {
     // Определить текущий масштаб отображения
     if ((pdata->ShowScale < minimum) ||
         (pdata->ShowScale > maximum))
       return 0;
   }

 // Тип функции
 int func = imlDrawType(draw, 3);

 // Параметры функции
 const char * parm = imlDrawParm(draw, 3);

 if (parm == 0)
   {
     parm = (const char *)&line;
     line.Color = 127;
     line.Thick = 255;
   }

 return imgPrepareImage(pdata, func, parm,
                        context, test, border);
}

//----------------------------------------------------------------
// Условный знак, видимый в заданном диапазоне масштабов отображения
//----------------------------------------------------------------
int _fastcall QuarterImage(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  if (pdata == 0) return 0;

  // Не менее 4 точек
  if (*pdata->PolyCounts < 4)
    return imgPrepareImage(pdata, IMG_DRAW, (char *)draw,
                           context, test, border);

  if (test == PREPARE_TEST)
    {
      IMGSQUARE parm;
      return imgPrepareImage(pdata, IMG_SQUARE, (char *)&parm,
                             context, test, border);
    }                         
                           
  // Запросить масштаб модели по высоте в базовом масштабе карты
  double scale = 5000.;  // 10 m = 2 mm на изображении

  if (imlDrawType(draw, 1) == IMG_LONG)
   {
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 1);
     if (parm)
       scale = parm->Value;
    
    if (scale < 10)
      scale = 10;
    else
     if (scale > 10000)
       scale = 10000;
   }

  double height = 10.;  // метров

  // Запросить семантику со значением высоты
  if (imlDrawType(draw, 2) == IMG_SEMANTIC)
   {
     int code = 0;
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 2);
     if (parm)
       code = parm->Value;
    
    if (code != 0)
      {
        imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,
                             code, 1, &height);
      }
   }

  // Высота на карте в точках
  int delta  = context->Scale * context->PixelX * height / scale * 1000000;

  int  * srccounts     = pdata->PolyCounts;
  DRAWPOINT * srcpoint = pdata->Points; 
  int srccount = *srccounts++;
  int j;
  
  // Тип функции
  int sidefunc = imlDrawType(draw, 3);

  // Параметры функции
  const char * sideparm = imlDrawParm(draw, 3);

  // Тип функции
  int edgefunc = imlDrawType(draw, 4);   

  // Параметры функции
  const char * edgeparm = imlDrawParm(draw, 4);
  
  // Базовый контур
  if (edgeparm)
    {
     // Опустить на точку
     for (j = 0; j < srccount; j++)
     {
       srcpoint->Y += 1;
       srcpoint++;
     }  

     imgPrepareImage(pdata, edgefunc, edgeparm, context, test, border);

     // Вернуть
     srcpoint  = pdata->Points;
     for (j = 0; j < srccount; j++)
     {
       srcpoint->Y -= 1;
       srcpoint++;
     }  
    } 
  
  // Нарисовать боковые поверхности
  DRAWPOINT    points[5];
  int number   = 5;

  POLYDATAEX   data;
  data.Points     = points;
  data.PolyCounts = &number;
  data.Count      = 1;

  int srcpolycount     = pdata->Count;
  srccounts = pdata->PolyCounts;
  srcpoint  = pdata->Points; 
  number = 5;
  for (int i = 0; i < srcpolycount; i++)
  {
    int srccount = *srccounts++;
    for (int j = 1; j < srccount; j++)
    {
      points[0] = *srcpoint;
      points[1] = points[0];
      points[3] = *(srcpoint + 1);
      points[2] = points[3];
      points[4] = points[0];
      points[1].Y -= delta;
      points[2].Y -= delta;
      srcpoint->Y -= delta;
      srcpoint++;

      data.Border.left   = min(points[0].X, points[3].X);
      data.Border.right  = max(points[0].X, points[3].X);
      data.Border.top    = min(points[0].Y-delta, points[3].Y-delta);
      data.Border.bottom = max(points[0].Y, points[3].Y);
      
      // Нарисовать боковую сторону
      if (sideparm)
        imgPrepareImage(&data, sidefunc, sideparm, context, test, border);
    }

    srcpoint->Y -= delta;
  } 

  // Геометрический центр выпуклого полигона
  int centerX = 0;
  int centerY = 0;
  
  number = 2;
  srccounts = pdata->PolyCounts;
  srcpoint  = pdata->Points; 
  for (int i = 0; i < srcpolycount; i++)
  {
    int srccount = *srccounts++;
    for (int j = 0; j < srccount; j++)
    {
      if (i == 0)
        {
          centerX += srcpoint->X; 
          centerY += srcpoint->Y; 
        }  
    
      points[1] = *srcpoint;
      points[0] = points[1];
      points[0].Y += delta;
      srcpoint++;

      // Нарисовать вертикальное ребро
      if (edgeparm)
        imgPrepareImage(&data, edgefunc, edgeparm, context, test, border);
    }

    if (i == 0)
      {
        centerX = centerX / srccount; 
        centerY = centerY / srccount; 
      }
  }

  // Тип функции
  int topfunc = imlDrawType(draw, 5);

  // Параметры функции
  const char * topparm = imlDrawParm(draw, 5);
  
  if (topparm)
    {
      imgPrepareImage(pdata, topfunc, topparm, context, test, border);
    }  

  // Вернуть координаты обратно
  srccounts = pdata->PolyCounts;
  srcpoint  = pdata->Points; 
  for (int i = 0; i < srcpolycount; i++)
  {
    int srccount = *srccounts++;
    number = 5;
    for (int j = 0; j < srccount; j++)
    {
      srcpoint->Y -= delta;
      srcpoint++;
    }
  } 

  // Тип функции
  int textfunc = imlDrawType(draw, 6);

  // Параметры функции
  const char * textparm = imlDrawParm(draw, 6);

  if (textparm)
    {
       // Запросить текст
       if (imlDrawType(draw, 7) == IMG_SEMANTIC)
         {
           int code = 0;
           IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 7);
           if (parm)
             code = parm->Value;
    
           if (code != 0)
             {
                struct
                {
                  char         length;
                  char         text[300];
                }
                  datatext;

               if (imlGetSemanticString((SEMANTIC *)(pdata->Semantic),
                                         code, 1, datatext.text, sizeof(datatext.text)) != 0)
                 {
                   char * point     = datatext.text;
                   data.Text        = &point;
                   data.AlignCode   = 1;

                   datatext.length = strlen(datatext.text);
                   datatext.length += 2;
                   datatext.text[datatext.length-1] = CA_MIDDLE_CENTER;
                   datatext.text[datatext.length]   = 0;
                   
                   points[0].X = centerX;
                   points[0].Y = centerY;
                   number = 1;
                   
                   imgPrepareImage(&data, textfunc, textparm, context, test, border);
                 }                         
             }
         }
    }
  
  return 1;
}

//----------------------------------------------------------------
// Отображение графических файлов
//----------------------------------------------------------------

#ifdef WIN32API    // 17/08/15 Савелов
#include <GdiPlus.h>
#endif

#define TYPEGDICOUNT    11        // Форматы файлов, которые обрабатывает GDI+
WCHAR * TYPEGDI[] = {L"BMP", L"GIF", L"JPEG", L"JPG", L"PNG", L"TIFF",
                     L"TIF", L"ICO", L"EXIF", L"WMF", L"EMF"};


int _fastcall IsGDIType(WCHAR * ext)
{
  if (ext[0] == 0) return 0;

  int length = wcslen(ext);
  if (length <= 1) return 0;
  
  for (int i = 0; i < TYPEGDICOUNT; i++)
   {
     if (_wcsnicmp(ext, TYPEGDI[i], length) != 0)
        continue;

     return 1;
   }  
  return 0;
}   

#ifdef WIN32API    // 17/08/15 Савелов
extern "C"
{
 int WINAPI GDIInit();
}
#endif

#define TYPEMAPCOUNT    6        // Форматы файлов врезки
WCHAR * TYPEMAP[] = {L"MAP", L"SIT", L"SITX", L"RSW", L"MTW", L"MPT"};

int _fastcall IsMapType(const WCHAR * ext)              // 03/07/15
{
  if (ext[0] == 0) return 0;

  int length = wcslen(ext);
  if (length <= 1) return 0;
  
  for (int i = 0; i < TYPEMAPCOUNT; i++)
   {
     if (wcsncmpi(ext, TYPEMAP[i], length) != 0)
        continue;

     return 1;
   }  
  return 0;
}


typedef struct TMapItem                                  // 03/07/15
{
  HMAP   hMap;                           // Открываемая карта
  WCHAR  Name[MAX_PATH_LONG];            // Путь к данным
  HRSC   Parent;                         // Родительская карта
  int    IsPainting;
  int    Zero;
}
  TMapItem;

typedef class TShapeMapList : TSimpleList
{
  public:

  TShapeMapList() : TSimpleList(sizeof(TMapItem))
  {
    InitializeTheCriticalSection(&Section);
    IsActive = 1;
  }
  ~TShapeMapList();

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  int _fastcall Append(TMapItem * item) 
  { return TSimpleList::Append((char *)item, sizeof(TMapItem)); }

  // Запросить элемент
  // number - номер элемента, начиная с 1
  TMapItem * _fastcall Item(int number) { return (TMapItem *)TSimpleList::Item(number); }
  
  // Найти запись по имени карты
  HMAP _fastcall Find(const WCHAR * name);

  // Установить признак отрисовки карты
  void _fastcall SetPainting(HMAP hmap);

  // Снять признак отрисовки карты
  void _fastcall ClearPainting(HMAP hmap);

  // Запросить признак отрисовки карты
  int _fastcall IsPainting(HMAP hmap);

  // Закрыть карты, которые были открыты указанным родителем
  void _fastcall CloseItemForParent(HMAP parent);
  
  // Занять переменные класса                                 // 13/08/15
  void Lock()
  {
    if (IsActive == 0)
      {
        InitializeTheCriticalSection(&Section);
        IsActive = 1;
      }
    EnterTheCriticalSection(&Section);
  }  

  // Освободить переменные класса                            // 13/08/15
  void UnLock() { LeaveTheCriticalSection(&Section); }
  
  MAPCRITICALSECTION Section;
  int IsActive;
  int Zero;
}
  TShapeMapList;

//-----------------------------------------------------------------
// Освободить ресурсы
//-----------------------------------------------------------------
TShapeMapList::~TShapeMapList()
{
  for (int i = 1; i <= Count(); i++)
  {
    TMapItem * item = Item(i);
    if (item == 0) break;

    imlCloseData(item->hMap);
  }

  if (IsActive != 0)
    DeleteTheCriticalSection(&Section);
}
  
//-----------------------------------------------------------------
// Найти запись по имени карты
//-----------------------------------------------------------------
HMAP _fastcall TShapeMapList::Find(const WCHAR * name)
{
  for (int i = 1; i <= Count(); i++)
    {
      TMapItem * item = Item(i);
      if (item == 0) break;

      if (_wcsicmp(item->Name, name) == 0)
        return item->hMap;
    }

  return 0;
}

//-----------------------------------------------------------------
// Установить признак отрисовки карты
//-----------------------------------------------------------------
void _fastcall TShapeMapList::SetPainting(HMAP hmap)
{
  for (int i = 1; i <= Count(); i++)
    {
      TMapItem * item = Item(i);
      if (item == 0) break;

      if (item->hMap == hmap)
        {
          item->IsPainting = 1;
          return;
        }  
    }
}

//-----------------------------------------------------------------
// Снять признак отрисовки карты
//-----------------------------------------------------------------
void _fastcall TShapeMapList::ClearPainting(HMAP hmap)
{
  for (int i = 1; i <= Count(); i++)
    {
      TMapItem * item = Item(i);
      if (item == 0) break;

      if (item->hMap == hmap)
        {
          item->IsPainting = 0;
          return;
        }  
    }
}

//-----------------------------------------------------------------
// Запросить признак отрисовки карты
//-----------------------------------------------------------------
int _fastcall TShapeMapList::IsPainting(HMAP hmap)
{
  for (int i = 1; i <= Count(); i++)
    {
      TMapItem * item = Item(i);
      if (item == 0) break;

      if (item->hMap == hmap)
        return item->IsPainting;
    }

  return 0;  
}  

//-----------------------------------------------------------------
// Закрыть карты, которые были открыты указанным родителем
//-----------------------------------------------------------------
void _fastcall TShapeMapList::CloseItemForParent(HRSC parent)        // 13/08/15
{
  Lock();

  for (int i = 1; i <= Count(); i++)
    {
      TMapItem * item = Item(i);
      if (item == 0) break;

      if (item->Parent == parent)
        {
          HMAP temp = item->hMap;
          item->Name[0] = 0;
          item->hMap    = 0;
          
          int period = 2000;  // 2 сек
          while(item->IsPainting != 0)
          {
            SleepTheThread(100);
            period -= 100;
            if (period <= 0) break;
          }

          imlCloseData(temp);
          Delete(i);
          i--;
        }  
    }

  UnLock();
}

TShapeMapList InsetList;

void CloseMapList(HRSC hrsc)                           // 13/08/15
{
  InsetList.CloseItemForParent(hrsc);
}

//-----------------------------------------------------------------
// Отобразить линию
//-----------------------------------------------------------------
int _fastcall ViewLine(const POLYDATAEX * data, 
                       const IMGCONTEXT * context, int test,
                       FRAME * border)
{                       
  IMGLINE line;

  line.Color = 127;
  line.Thick = 255;

  return imgPrepareImage(data, IMG_LINE, (char *)&line,
                         context, test, border);
}

//-----------------------------------------------------------------
// Отображение графических файлов
//-----------------------------------------------------------------
int _fastcall ImageView(const POLYDATAEX * data, const IMGDRAW * draw,
                        const IMGCONTEXT * context, int test,
                        FRAME * border)
{
  if ((test == PREPARE_TEST) || (test == PREPARE_BORDER))
    {
      IMGSQUARE parm;
      return imgPrepareImage(data, IMG_SQUARE, (char *)&parm,
                             context, test, border);
    }                         

#ifdef WIN32API    // 17/08/15 Савелов
 GDIInit();
#endif

 int semcode = 0;
   
 if (imlDrawType(draw, 1) == IMG_SEMANTIC)
   {
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 1);
     if (parm)
       semcode = parm->Value;
   }

  WCHAR value[1024];
  HRSC hRsc = data->MapRsc;
  SEMANTIC * semn = (SEMANTIC *)(data->Semantic);
  int type = 0;
   
  if (semcode == 0)
    {
      int number = 1;
    
      // Найти значение семантики типа ссылка на документ
      while(1)
      {
        if (imlGetSemanticDocReferenceStringUn(hRsc, semn, number, value, sizeof(value), type) == 0)
          return ViewLine(data, context, test, border);      

        if (value[0] != 0)
          break;

        number++;    
      }    
    }
  else  
    {
      int number = 1;

      // Найти значение семантики для заданного кода семантики
      while(1)
      {
        if (imlGetSemanticStringUn(semn, semcode, number, value, sizeof(value)) == 0)
          return ViewLine(data, context, test, border);      

        if (value[0] != 0)
          break;

        number++;    
      }    
    }

  HPAINT hpaint = (HPAINT)context->HView;  
  HMAP hmap = imlGetMapHandle(hpaint);           // 26/10/15

  int isalias = 0;

  WCHAR sname[1024];
  
  if (hmap != 0)
    {
      HSITE hsite = imlGetCurrentViewSite(hmap);

      int isserver = imlIsSiteFromServer(hmap, hsite);
      
      // Значение не является алиасом данных на сервере?
      if (_wcsnicmp(value, WHOSTALIAS, HOSTSIZE) == 0)
        {
          if (wcsstr(value, L"DOC#") == 0)                // 03/07/15
            {
              isalias = 1;
            } 
          else
            {
              // Считать документ на сервере
              if (imlReadSiteDocumentUn(hmap, hsite, value, value, sizeof(value)) == 0)
                return ViewLine(data, context, test, border);      
            }    
        }
      else
        if (isserver != 0)
        {
          WCHAR vext[MAX_PATH];
          SplitThePath(value, 0, 0, 0, vext);
          // Не читать карты, растры и матрицы
          if ((_wcsicmp(vext, L".sitx") == 0) ||
              (_wcsicmp(vext, L".rsc") == 0) ||
              (_wcsicmp(vext, L".mtw") == 0) ||
              (_wcsicmp(vext, L".mtq") == 0) ||
              (_wcsicmp(vext, L".rsw") == 0) ||
              (_wcsicmp(vext, L".sit") == 0) ||
              (_wcsicmp(vext, L".sxf") == 0) ||
              (_wcsicmp(vext, L".txf") == 0) ||
              (_wcsicmp(vext, L".map") == 0))
            {
              // Может это ссылка на себя?
              // Запросить путь к карте
              imlGetSiteFileNameUn(hmap, hsite, sname, sizeof(sname));

              int vsize = wcslen(value);
              int ssize = wcslen(sname);
              if ((vsize < ssize) && (_wcsicmp(sname+ssize-vsize, value) == 0))
                {    
                  // Заменим ссылкой на себя
                  WcsCopy(value, sname, sizeof(sname));
                  type = TMAPFILE;
                }
              else
                return ViewLine(data, context, test, border);      
            }  
          else
          // Считать документ на сервере                   // 30/07/15 (ядро 11.12.10)
          if (imlReadSiteDocumentUn(hmap, hsite, value, value, sizeof(value)) == 0)
            return ViewLine(data, context, test, border);      
        }
        else  
        {
          // Запросить путь к карте
          imlGetSiteFileNameUn(hmap, hsite, sname, sizeof(sname));

          WCHAR filename[1024];
 
          if ((imlBuildLongNameUn(sname, value, filename) == 0) || (filename[0] == 0))
            return ViewLine(data, context, test, border);      

          WcsCopy(value, filename, sizeof(value));
        }
       
      if ((isalias == 0) && (type != TMAPFILE) && (ExistTheFile(value) == 0))
        return ViewLine(data, context, test, border);      
    }
    
  // Определить габариты изображения на карте
  // Настраиваем адреса исходной метрики
  DRAWPOINT * pointin = (DRAWPOINT *)(data->Points);
  int count = *data->PolyCounts; 

  int Xmin =  999999;
  int Ymin =  999999;
  int Xmax = -999999;
  int Ymax = -999999;
    
  while(count-- > 0)
  {
    if (Xmin > pointin->x)
      Xmin = pointin->x;
    if (Xmax < pointin->x)
      Xmax = pointin->x;
    if (Ymin > pointin->y)
      Ymin = pointin->y;
    if (Ymax < pointin->y)
      Ymax = pointin->y;
    
    pointin++;
  }

  // Это графический файл?
  WCHAR ext[MAX_PATH];
  SplitThePath(value, NULL, NULL, NULL, ext);

  // Степень прозрачности изображения
  long transparent = -1;
  imlGetSemanticLong(semn, SEMCOLORWEIGHT, 1, &transparent);

  long imgcolor = -1;
  imlGetSemanticLong(semn, SEMIMAGECOLOR, 1, &imgcolor);

#ifdef WIN32API    // 17/08/15 Савелов
  // Рисуем объект через GDI
  if ((isalias == 0) && (ext[0] != 0) && IsGDIType(ext+1))
    {
      // При отображении во внутренний DIB функциями WIN API учитываются резервные строки
      if (context->View < VT_PRINT)
        {
          Ymin += SHIFTUP32;
          Ymax += SHIFTUP32;
        }  

      // Создать объект класса Graphics
      Gdiplus::Graphics gdip(context->Hdc);

      try
      {
        // Загрузить изображение
        Gdiplus::Image image(value);

        if (((transparent >= 0) && (transparent < 100)) || (imgcolor != -1))
          {
            Gdiplus::ImageAttributes attr; 

            Gdiplus::ColorMatrix matrix;
            memset(&matrix, 0, sizeof(matrix));
            matrix.m[0][0] = 1;
            matrix.m[1][1] = 1;
            matrix.m[2][2] = 1;
            matrix.m[3][3] = 1;
            matrix.m[4][4] = 1;

            if ((transparent >= 0) && (transparent < 100))
              matrix.m[3][3] = ((float)transparent)/100.;

            if (imgcolor != -1)
              {
                float base = (imgcolor & 0x0FF);
                float temp = ((float)((imgcolor & 0x0FF00) >> 8));
                if (base < temp) base = temp;
                temp = ((float)((imgcolor & 0x0FF0000) >> 16));
                if (base < temp) base = temp;

                float limit = 0.5 * base / 255.;
              
                // Придать заданный оттенок изображению
                float weight = ((float)(imgcolor & 0x0FF)) / 255.;
                if (weight >= limit)
                 matrix.m[4][0] += weight / 2.;
                else
                 matrix.m[4][0] -= (limit - weight) / 2.;
                 
                weight = ((float)((imgcolor & 0x0FF00) >> 8)) / 255.; 
                if (weight >= limit)
                  matrix.m[4][1] += weight / 2.;
                else
                  matrix.m[4][1] -= (limit - weight) / 2.;
                  
                weight = ((float)((imgcolor & 0x0FF0000) >> 16)) / 255.;  
                if (weight >= limit)
                  matrix.m[4][2] += weight / 2.;
                else  
                  matrix.m[4][2] -= (limit - weight) / 2.;
              }
            
            attr.SetColorMatrix(&matrix, Gdiplus::ColorMatrixFlagsDefault/*0*/, Gdiplus::ColorAdjustTypeDefault/*0*/);   // 13/03/15 // Sh.D.            

            Gdiplus::Rect destRect(Xmin, Ymin, Xmax - Xmin, Ymax - Ymin);
            gdip.DrawImage(&image, destRect, 0, 0, image.GetWidth(), image.GetHeight(),
                           Gdiplus::UnitPixel, &attr, 0, 0);
          }
        else
          {
            gdip.DrawImage(&image, Xmin, Ymin, Xmax - Xmin, Ymax - Ymin);
          }  
      }
      catch(...)
      {
        return ViewLine(data, context, test, border);      
      }  
      return 1;
    }
#endif  // WIN32API

  // Рисуем врезку карты
  if (((ext[0] != 0) && IsMapType(ext+1)) || (isalias != 0))       // 03/07/15
    {
      int sdx = Xmax - Xmin;
      if (sdx < 8) return 0;
      int sdy = Ymax - Ymin;
      if (sdy < 8) return 0;

      // Отображение врезки
      InsetList.Lock();                                              // 13/08/15
      HMAP ihmap = InsetList.Find(value);                            // 03/09/15 Савелов
      if (ihmap == 0)
        {
          ihmap = imlOpenDataUn(value);
          TMapItem item;
          item.hMap = ihmap;
          item.Parent = hRsc;
          WcsCopy(item.Name, value, sizeof(item.Name));
          InsetList.Append(&item);
          InsetList.UnLock();                                       // 13/08/15
        }
      else
        {
          InsetList.UnLock();                                       // 13/08/15
          if (InsetList.IsPainting(ihmap)) return 1;
        }  

      if (ihmap == 0)
        {
          return ViewLine(data, context, test, border);      
        }

      long int tdx, tdy;
        
      RECT viewrect;

      // Определить масштаб отображения врезки
      double scale = 0;
      if (imlGetSemanticDouble(semn, SEMIMAGESCALE, 1, &scale) != 0)
        if (scale > 0)
        {
          // Запросить базовый масштаб карты и пересчитать
          scale = scale / 100. * imlGetMapScale(ihmap);
          // Сжимать пропорционально
          if (context->ViewScale <= 1)
            scale = scale/context->ViewScale;
          else  
            // Увеличивать с отставанием в 2 раза
            scale = scale/(context->ViewScale / 2.);
            
          imlSetViewScale(ihmap, 0, 0, scale);
        }
        else
        if (scale < 0)
        {
          // Вся карта в окне (scale < 0)
          double rscale = imlGetRealShowScale(ihmap);

          // Обновить текущее состояние габаритов для стабильности работы
          imlSetViewScale(ihmap, 0, 0, rscale);

          // Вся карта в окне - подогнать масштаб под размеры карты
          imlGetPictureSize(ihmap, &tdx, &tdy);

          // Определить коэффициент масштабирования
          float change = (float)sdx/(float)tdx;
          float change2 = (float)sdy/(float)tdy;

          if (change2 < change)
            change = change2;

          rscale = imlGetRealShowScale(ihmap);
          rscale = rscale/change;

          imlSetViewScale(ihmap, 0, 0, rscale);

          // Запросить габариты изображения для отрисовки
          imlGetPictureSize(ihmap, &tdx, &tdy);

          viewrect.LT = 0;
          viewrect.UP = 0;
          viewrect.RT = tdx;
          viewrect.DN = tdy;
        }

      if (scale >= 0)
        {
         if (scale == 0)
           {
             // Сжимать пропорционально
             if (context->ViewScale < 1)
               {
                 double rscale = imlGetMapScale(ihmap);
                 rscale = rscale/context->ViewScale;
                 
                 imlSetViewScale(ihmap, 0, 0, rscale);
               }
             else  
               if (context->ViewScale == 1)
                 {
                   double rscale = imlGetMapScale(ihmap);
                   imlSetViewScale(ihmap, 0, 0, rscale);
                 }
               else
                 {
                   // Увеличивать с отставанием в 2 раза
                   double rscale = imlGetMapScale(ihmap);
                   rscale = rscale/(context->ViewScale / 2.);
                   imlSetViewScale(ihmap, 0, 0, rscale);
                 }    
           }

         // Определить координаты верхнего левого угла изображения  
         double geox = 0;
         imlGetSemanticDouble(semn, SEMLATWGS84, 1, &geox);

         double geoy = 0;
         imlGetSemanticDouble(semn, SEMLONWGS84, 1, &geoy);

         if ((geox == 0) || (geoy == 0))
           {
             // Рисуем центр карты в центре окна
             // Запросить габариты изображения для отрисовки
             imlGetPictureSize(ihmap, &tdx, &tdy);

             viewrect.LT = tdx/2 - sdx/2;
             if (viewrect.LT < 0) viewrect.LT = 0;
             viewrect.UP = tdy/2 - sdy/2;
             if (viewrect.UP < 0) viewrect.UP = 0;
             viewrect.RT = viewrect.LT + sdx;
             viewrect.DN = viewrect.UP + sdy;
           }
         else
           {  
             double h = 0;

             // Перевести в радианы
             geox = geox / 180. * M_PI;
             geoy = geoy / 180. * M_PI;

             imlGeoWGS84ToPlane3D(ihmap, &geox, &geoy, &h);
             imlPlaneToPicture(ihmap, &geox, &geoy);

             viewrect.LT = geox - sdx/2;
             if (viewrect.LT < 0) viewrect.LT = 0;
             viewrect.UP = geoy - sdy/2;
             if (viewrect.UP < 0) viewrect.UP = 0;
             viewrect.RT = viewrect.LT + sdx;
             viewrect.DN = viewrect.UP + sdy;
           }
        }    

      if (Xmin < 0)
        {
          viewrect.LT -= Xmin;
          Xmin = 0;
        }
        
      if (Ymin < 0)
        {
          viewrect.UP -= Ymin;
          Ymin = 0;
        }
        
      int dx = viewrect.RT - viewrect.LT;
      if (dx <= 0) return 0;
      if (dx > context->Size.CX) viewrect.RT = viewrect.LT + context->Size.CX;
      int dy = viewrect.DN - viewrect.UP;
      if (dy <= 0) return 0;
      if (dy > context->Size.CY) viewrect.DN = viewrect.UP + context->Size.CY;
        
      if (imgcolor != -1)
        {
          IMGPOLYGONGLASS glass;
          // Цвет
          glass.Color  = imgcolor;
          glass.Bright = 0;
          glass.Contrast = 0;
          glass.Transparency = 100;

          if ((transparent >= 0) && (transparent <= 100))
            glass.Transparency = transparent;

          imgPrepareImage(data, IMG_POLYGONGLASS, (char *)&glass,
                          context, test, border);
        }

      if ((transparent >= 0) && (transparent <= 100))
        imlSetSiteTransparent(ihmap, ihmap, transparent);
      else  
        imlSetSiteTransparent(ihmap, ihmap, 100);

#ifdef WIN32API                                  
      POINT offpoint;
      if (context->View < VT_PRINT)
        ::SetViewportOrgEx(context->Hdc, Xmin, Ymin+SHIFTUP32, &offpoint);
      else  
        ::SetViewportOrgEx(context->Hdc, Xmin, Ymin, &offpoint);
#endif
      InsetList.SetPainting(ihmap);

      imlSetViewType(ihmap, context->View);
        
#ifdef WIN32API    // 17/08/15 Савелов
      // Нарисовать карту в заданном масштабе без очистки фона (-2)
      imlPaint95Ex(ihmap, context->Hdc, -2, &viewrect, 1);
#else

      XIMAGEDESC *XImageDesc = (XIMAGEDESC*)context->Hdc;
      imlPaintToXImage(ihmap, XImageDesc, Xmin, Ymin,  &viewrect);
#endif

      InsetList.ClearPainting(ihmap);
      
#ifdef WIN32API
      ::SetViewportOrgEx(context->Hdc, offpoint.x, offpoint.y, NULL);
#endif

      return 1;
    }

#ifdef WIN32API    // 17/08/15 Савелов
  // Рисуем OLE объект
  if (PaintOle(value, Xmax, Xmin, Ymax, Ymin, context, imgcolor, transparent, test, border)) return 1;
#endif  // WIN32API

  // В остальных случаях рисуем контур
  ViewLine(data, context, test, border);
  return 0;
}

//-----------------------------------------------------------------
// Линия с постоянным контуром                 // 14/03/16
//-----------------------------------------------------------------
int _fastcall ContouringLine(const POLYDATAEX * data, const IMGDRAW * draw,
                        const IMGCONTEXT * context, int test,
                        FRAME * border)
{

 const char * lineparm;
 int   linefunc;
 const char * parm;

 float thickfon;
 float thickline;
 IMGLINE  linefon;
 IMGLINE  linecontour;
 int contourfunc = 0;
 if ((data == 0)||(context == 0)) return 0;


 IMGCONTEXT contextd;      // 23/10/14
 memcpy((char *)&contextd,context,sizeof(IMGCONTEXT));

 // "Ширина фоновой линии в мм"
 if (imlDrawType(draw, 1) == IMG_DOUBLE)
   {
     if ((parm = imlDrawParm(draw, 1)) != 0)
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

         // Интервал мм
         thickfon =  value;

       }
   }

 // "Ширина линии контура в мм"
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

         // Интервал мм)
         thickline = value;

       }
   }

  linefon.Color = 0;
  if (imlDrawType(draw, 2) == IMG_COLOR)
    {
      parm = imlDrawParm(draw,2);
      if (parm)
        linefon.Color = *((int*)parm);
    }

  linecontour.Color = 0;
  if (imlDrawType(draw, 4) == IMG_COLOR)
    {
      parm = imlDrawParm(draw,4);
      if (parm)
        linecontour.Color = *((int*)parm);
    }



  // посмотрим размер thickline в пикс для единичного масштаба столько пикс и нужно держать
  double thickpix = (context->PixelX * thickline * 1000.0);

  // if (thickpix < 1.0) thickpix = 1.0;

  if (thickpix < 0.2) thickpix = 0.2;                // 11/04/08
  if (context->Scale == 0)  contextd.Scale = 1;
  if (context->PixelX == 0) contextd.PixelX = 0.006;
  thickline = thickpix / (contextd.PixelX * contextd.Scale);
  linecontour.Thick = thickfon* 1000.0 + 2*thickline;
  if ((imgPrepareImage(data, IMG_LINE, (char *)&linecontour, &contextd, test, border) != 0) &&
         (test == PREPARE_TEST))
        {
          return 1;
        }

  linefon.Thick = thickfon * 1000.0;
  if ((imgPrepareImage(data, IMG_LINE, (char *)&linefon, &contextd, test, border) != 0) &&
         (test == PREPARE_TEST))
        {
          return 1;
        }

   return 0;
}
