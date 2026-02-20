//***** dfdraw.cpp  ******************** Кота Ф.Ф.     *******  31/03/2014 ***
//***** dfdraw.cpp  ********************Пантелеева N.A *******  27/03/14  ***
//***** dfdraw.cpp  ********************Dishlenko S.G. *******  30/03/15  ***
//*            Библиотека условных знаков для классфикаторов АНИ  (на С++)
//*			Функции отрисовки знаков
//****************************************************************************
// Автор          : Кота Ф.Ф.
// Перевод на C++ : Кота Ф.Ф.
//****************************************************************************

//#define MAPTRACER 1   // Если трассировка не нужна - FALSE

#ifndef IMLSERV_H
  #include "imlserv.h"
#endif

#ifndef DFC_H
  #include "dfc.h"
#endif

#include <math.h>

#include "maptrace.h"       // Протокол - c:maptace.log
#include <stdio.h>
#include <math.h>

const double PI = 3.14159026533;

typedef struct WIDTHSEGMENT
{
  DRAWPOINT first, last;
  double    pxWidth;
  int       valid;
}
  WIDTHSEGMENT;



typedef struct ROUTESHEMA
{
   ROUTESHEMA() { memset(this, 0, sizeof(*this)); }
   DRAWPOINT    MET[8];
   double       sAngle;   // синус угол участка
   double       cAngle;   // косинус угол участка
   int          Dist;     // длина участка
   int          Da;       // длина от начала до центра указателя
   int          D1;       // длина от начала до указателя прямого курса
   int          D2;       // длина от начала до указателя обратного курса
   int          cut1;     // обрезка у прямого курса
   int          cut2;     // обрезка у обратного курса
   int          vTrue ;   // видимость указателя прямого курса  0, 1
   int          vRev ;    // видимость указателя обратного курса  0, 1
   int          sArr;     // ширина указателя
   int          sV1;      // ширина знака прямого курса
   int          sV2;      // ширина знака обратного курса
   int          Res;      // Выровняли
}
  ROUTESHEMA;


typedef struct DATAPARM
{
  int          bfunc;     // по параметрам
  int          func;      // что пришло
  const char * parm;      // запрошенные параметры
}
 DATAPARM;

typedef struct MSAITEM
{
  char         Height[8];    // высота
  int          Azimuth;      // азимут
  double       Vector;       // геометрическое нарпавление на карте
}
 MSAITEM;

const  long  statusCOLOR[3] = {0x00A6A6FF, 0x0080FF00, 0x0080FFFF};


int _fastcall IntDistance(DRAWPOINT P1, DRAWPOINT P2)
{
  float dx,dy;
  dx = P2.x-P1.x;
  dy = P2.y-P1.y;
  if (dx*dx+dy*dy >= 0)
      return (int)ROUND(sqrt(dx*dx+dy*dy));
  return 0;
}


int _fastcall OrthoPoint(DRAWPOINT Base, DRAWPOINT P1, DRAWPOINT P2, DRAWPOINT * R)
{
  R->x    = P1.x;
  R->y    = P1.y;
  int ret = 0;
  float A = IntDistance(P1, P2);
  if (A >= DELTANULL)
    {
     A = ((Base.X-P1.X)*(P2.X-P1.X)+(Base.Y-P1.Y)*(P2.Y-P1.Y))/(A*A);
     float B = (Base.X-P2.X)*(P2.X-P1.X)+(Base.Y-P2.Y)*(P2.Y-P1.Y);
     R->x = P1.x + ROUND((P2.x-P1.x)*A);
     R->y = P1.y + ROUND((P2.y-P1.y)*A);
     ret  = ((A>=0 ? 1 : -1)==(B>=0 ? 1 : -1)) ? -1 : 1;
   }
  return ret;
}

// Кота 31.03.2014 убрал проблему atan2(0,0)

// Отбили неопределенность в atan2
double _fastcall arctan2(double dY, double dX)
{
  if (   (dY > -1e-3 ) && (dY < 1e-3 ) // очень маленький отрезок
      && (dX > -1e-3 ) && (dX < 1e-3) )
         return 0; //atan2(1,1) - примерно 0.78 зачем ?
    return atan2(dY, dX);
}

double _fastcall imlDistanceToPixel(const IMGCONTEXT * context, double showscale, DRAWPOINT * point, double value)
{
  return (1E6/showscale*context->PixelX);
}

//----------------------------------------------------------------------
// Маршрут
//----------------------------------------------------------------------
int _fastcall DrawEnroute(const POLYDATAEX* pdata, const IMGDRAW* draw,
                            const IMGCONTEXT * context, int test,
                            FRAME * border, int isDual)
{
  int * RealCnt = pdata->PolyCounts;
  if (*RealCnt < 2)  return 0;    /* если количество точек менее 2 - выход */
  POLYDATAEX CpyMet   = *pdata;
  DRAWPOINT * inPoint = (DRAWPOINT *)(pdata->Points); // входящая метрика
  DATAPARM  DMP[16];
  ROUTESHEMA SH;
  int OneRoute = 1;                // односторонний маршрут
  if (isDual == 1) OneRoute = 0;   // двусторонний маршрут
  int  i;
  int  cnt = 1;
  long N = 1; // количество правильных параметров
  int number = 0;
  int ret = 0;
  DMP[0].bfunc = 0;
  DMP[0].func = 0;
  DMP[0].parm = 0;

  // заполним параметры, по структуре
  DMP[1].bfunc = IMG_LINE;
  for ( i = 2; i <= 4; i++) DMP[i].bfunc = IMG_VECTOREX;
  for ( i = 5; i <= 7; i++) DMP[i].bfunc = IMG_SEMANTIC;
  for ( i = 8; i <= 14; i++) DMP[i].bfunc = IMG_VECTOREX;
  DMP[15].bfunc = IMG_SEMANTIC;

  if (OneRoute == 1) // односторонний
    {
      DMP[4].bfunc = 0;
      DMP[4].func = 0;
      DMP[4].parm = 0;
    }

  // запросим параметры объекта
  for (i = 1; i <= 3; i++)
   {
     DMP[i].func  = imlDrawType(draw, i);
     DMP[i].parm  = imlDrawParm(draw, i);
   }

  for (i = 4; i <= 15-OneRoute; i++)
   {
     DMP[i+OneRoute].func = imlDrawType(draw, i);
     DMP[i+OneRoute].parm = imlDrawParm(draw, i);
   }

  // посмотрим, то ли считали
  for (i = 1; i <= 15; i++)
    if (DMP[i].bfunc != 0)
       {if (DMP[i].bfunc == DMP[i].func)
         N++;
       }

  CpyMet.PolyCounts  = RealCnt;
  CpyMet.Points      = inPoint;
  // если подсунули левый знак, есть подобъекты - рисуем линию по метрике
  ret = 0;
  if ((N >= 14) && (CpyMet.Count == 1)) ret = 1;
  if (ret == 0)
   {
      if ((imgPrepareImage(pdata,  DMP[1].func, DMP[1].parm,
                                        context, test, border) != 0) &&
          (test == PREPARE_TEST))
        {
          return 1;
        }
     return 0;
   }

  ret = 0;
  // ------------- Запрашиваем семантики объекта  -----------------

  SH.cut1 = -1;
  SH.cut2 = -1;
  long status = 255;
  double semvalue = 0.0;
  IMGLONG * parm = 0;

  // семантика Обрезка - прямой курс (мм.)
  parm = (IMGLONG *)DMP[5].parm;
  if (parm &&  (parm->Value>0))
  {
    imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &semvalue);
    SH.cut1 =  abs((int)(1000*semvalue));
  }

  semvalue = 0.0;
  parm = (IMGLONG *)DMP[6].parm;
  if (parm &&  (parm->Value>0))           // семантика Обрезка-прямой курс (мм.)
  {
    imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &semvalue);
    SH.cut2 =  abs((int)(1000*semvalue));
  }

  N = 0;
  parm = (IMGLONG *)DMP[7].parm; // Ориентация знака маршрута ОВД(1-7)
  if (parm &&  (parm->Value>0))
    imlGetSemanticLong((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &N);

  parm = (IMGLONG *)DMP[15].parm;
  if (parm &&  (parm->Value>0))          // Семантика-статус маршрута
    imlGetSemanticLong((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &status);

  // статус рисуется до ограничения
  if ((status>0)&&(status<4))
    {
      IMGLINE stline;
      stline.Color = statusCOLOR[status-1];
      stline.Thick = 2048;
     if ((imgPrepareImage(&CpyMet, IMG_LINE, (char *)&stline,
                 context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }
  };

  // Определим количество точек метрики для знака (не более 5)
  cnt = *RealCnt ;
  if (cnt>5-OneRoute) cnt = 5-OneRoute; // 09.03.2013   Кота

  /*------------- ПОДГОТОВКА МЕТРИКИ ---------------*/

  if ((N<1) || (N>7))
       N = 2;
  else N = N+7;

  IMGVECTOREX * V = (IMGVECTOREX * )DMP[N].parm;
  if (V != 0)
    SH.sArr = abs((int)(context->Scale*context->PixelX*V->HSize));
  // адекватные обрезки от 0 до 2 см, остальные считаем ошибками
  SH.cut1 = ((SH.cut1>0) && (SH.cut1<=20000)) ? (int)(context->Scale*context->PixelX*SH.cut1) : 0;
  SH.cut2 = ((SH.cut2>0) && (SH.cut2<=20000)) ? (int)(context->Scale*context->PixelX*SH.cut2) : 0;

  DRAWPOINT tmet  [5];   // временный буфер
  DRAWPOINT R     [8];   // расчётная метрика

  R[0] = *inPoint;  R[1]= *(inPoint+cnt-1);
  SH.Dist = IntDistance(R[0],R[1])- SH.cut1 - SH.cut2;
SH.sAngle = arctan2(R[1].y-R[0].y, R[1].x-R[0].x);   // 31.03.2014
  SH.cAngle = cos(SH.sAngle);
  SH.sAngle = sin(SH.sAngle);
  V = (IMGVECTOREX * )DMP[3].parm;
  SH.sV1= abs(ROUND(context->Scale*context->PixelX*V->HSize));
  if (OneRoute==0)
   {
     V = (IMGVECTOREX * )DMP[4].parm;
     SH.sV2 = abs(ROUND(context->Scale*context->PixelX*V->HSize));
   }

   R[0] = *inPoint;  R[1]= *(inPoint+cnt-1);

  tmet[0].x= ROUND(R[0].x + SH.cut1*SH.cAngle);
  tmet[0].y= ROUND(R[0].y + SH.cut1*SH.sAngle);

  tmet[cnt-1].x= ROUND(R[1].x - SH.cut2*SH.cAngle);
  tmet[cnt-1].y= ROUND(R[1].y - SH.cut2*SH.sAngle);

  if (SH.Dist-SH.sArr>0)
    {  // 1. знак
      // втискиваем с запасом по пикселю
      if (SH.Dist-SH.sArr-SH.sV1 > 2)
         SH.vTrue = 1;  // 2. прямой курс
      if (SH.Dist-SH.sArr-SH.sV1-SH.sV2 > 2)
         SH.vRev = 1; // 3. обратный курс
    }
  else // не втиснули ничего рисуем линию - выходим
    {
     ret = imgPrepareImage(&CpyMet, DMP[1].func, DMP[1].parm, context, test, border);
     return ret;
    }

  // корректируем точки метрики по линии
  for (i=1; i<cnt-1; i++)
   {
     ret = OrthoPoint(*(inPoint+i),tmet[0],tmet[cnt-1], &tmet[i]);
     if (ret!=1)
      {
        ret = imgPrepareImage(&CpyMet, DMP[1].func, DMP[1].parm, context, test, border);
        if ((ret!= 0) && (test == PREPARE_TEST))
           return 1;
        else
           return 0;
      }
   }


  // tmet содержит откорректированные точки метрики
  // проверяем новую метрику
  // знак в 1-й или 2-й точке метрики
  SH.Da = (int)(IntDistance(tmet[0], tmet[BYTE(cnt>3)+1]));           // 30/03/15
  SH.sArr  = SH.sArr / 2;
  if ((SH.Da < SH.sArr) || (SH.Dist - SH.Da < SH.sArr))
    {  // в позицию точки, знак не вклинивается
      ret = imgPrepareImage(&CpyMet, DMP[1].func, DMP[1].parm, context, test, border);
      ret = ((ret != 0) && (test == PREPARE_TEST)) ? 1 : 0;
      return ret;
    }
  SH.sV1   = SH.sV1 / 2;
  SH.sV2   = SH.sV2 / 2;
  switch (cnt)
    {
      // у нас 1 точка посредине. курсы не отображаются - нет базовой точки
      case 3:
           SH.vTrue = 0;
           SH.vRev  = 0;
           break;
      // у нас 2 точки посредине. 1-я считается прямым курсом
      case 4:
           SH.D1 = (int)(IntDistance(tmet[0], tmet[1]));
           if (SH.vTrue)
              {
                 if ((SH.D1-SH.sV1>1) && ((SH.Da-SH.sArr-SH.D1-SH.sV1)>1))
                   SH.vTrue = 1;
                 else
                   SH.vTrue = 0;
              }
           SH.vRev  = 0;
           break;
      // у нас 3 точки посредине. 1-я прямой курс 3-я обратный курс
      case 5:
        if (OneRoute == 0)
          {
             SH.D1 = (int)(IntDistance(tmet[0], tmet[1]));
             SH.D2 = (int)(IntDistance(tmet[0], tmet[3]));
             // проверяем видимость курсов
             if (SH.vTrue)
               {
                 if ((SH.D1-SH.sV1>1) && ((SH.Da-SH.sArr-SH.D1-SH.sV1)>1))
                   SH.vTrue = 1;
                 else
                   SH.vTrue = 0;
               }
             if (SH.vRev)
                if ((SH.Dist-SH.D2-SH.sV2>1) && ((SH.D2-SH.sV2-SH.Da-SH.sArr)>1))
                  SH.vRev  = 1;
                else
                  SH.vRev  = 0;
          }
        break;
   }

   for (i=0; i<=2; i++)
      R[i] = tmet[0];
   // врезаем указатель
   R[3].x = ROUND(R[0].X+(SH.Da-SH.sArr)*SH.cAngle);
   R[3].y = ROUND(R[0].Y+(SH.Da-SH.sArr)*SH.sAngle);
   R[4].x = ROUND(R[0].X+(SH.Da+SH.sArr)*SH.cAngle);
   R[4].y = ROUND(R[0].Y+(SH.Da+SH.sArr)*SH.sAngle);

   for (i=5;i<=7;i++)
      R[i] = tmet[cnt-1];

   if (SH.vTrue)
   {
     R[1].x = ROUND(R[0].X+(SH.D1-SH.sV1)*SH.cAngle);
     R[1].y = ROUND(R[0].Y+(SH.D1-SH.sV1)*SH.sAngle);
     R[2].x = ROUND(R[0].X+(SH.D1+SH.sV1)*SH.cAngle);
     R[2].y = ROUND(R[0].Y+(SH.D1+SH.sV1)*SH.sAngle);
   }

   if ((OneRoute==0) && SH.vRev)
    {
     R[5].x = ROUND(R[0].X+(SH.D2-SH.sV2)*SH.cAngle);
     R[5].y = ROUND(R[0].Y+(SH.D2-SH.sV2)*SH.sAngle);
     R[6].x = ROUND(R[0].X+(SH.D2+SH.sV2)*SH.cAngle);
     R[6].y = ROUND(R[0].Y+(SH.D2+SH.sV2)*SH.sAngle);
   }

  number = 2;
  CpyMet.PolyCounts = &number;
  CpyMet.Points = &R[0];

  if ((imgPrepareImage(&CpyMet, DMP[1].func, DMP[1].parm,
                      context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }

  if (SH.vTrue)
   {
     CpyMet.Points = &R[1];
     if ((imgPrepareImage(&CpyMet, DMP[3].func, DMP[3].parm,
                      context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }
   }

  CpyMet.Points = &R[2];
  if ((imgPrepareImage(&CpyMet, DMP[1].func, DMP[1].parm,
                      context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }

  CpyMet.Points = &R[3];
  if ((imgPrepareImage(&CpyMet, DMP[N].func, DMP[N].parm,
                      context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }

  CpyMet.Points = &R[4];
  if ((imgPrepareImage(&CpyMet, DMP[1].func, DMP[1].parm,
                      context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }

  if (SH.vRev)
   {
     CpyMet.Points = &R[5];
     if ((imgPrepareImage(&CpyMet, DMP[4].func, DMP[4].parm,
                      context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }
  }

  CpyMet.Points = &R[6];
  if ((imgPrepareImage(&CpyMet, DMP[1].func, DMP[1].parm,
                      context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }

  // дорисовываем остаток
  if (*RealCnt > cnt)     // 09.03.2013   Кота
    {
      number = *RealCnt-cnt+1;
      CpyMet.PolyCounts = &number;
      CpyMet.Points = (inPoint+cnt-1);
      return imgPrepareImage(&CpyMet, DMP[1].func, DMP[1].parm, context, test, border);
    }

  return 0;

};

//----------------------------------------------------------------------
// Знак "аэродромный маршрут"
//----------------------------------------------------------------------

int _fastcall DrawAirportRoute(const POLYDATAEX* pdata, const IMGDRAW* draw,
                       const IMGCONTEXT * context, int test,
                       FRAME * border)
{
  int * RealCnt = pdata->PolyCounts;
  if (*RealCnt < 2)  return 0;    /* если количество точек менее 2 - выход */
  POLYDATAEX CpyMet   = *pdata;
  DRAWPOINT * inPoint = (DRAWPOINT *)(pdata->Points); // входящая метрика
  DATAPARM  DATA[8];
  memset((char *)&DATA[0],0,sizeof(DATA));
  ROUTESHEMA SH;
  int  i;
  int N =0;
  int number = 0;
  int ret = 0;
  // анализируем структуру "подсунутого" объекта
  for (i = 1; i<=7; i++)
   {
     DATA[i].func = imlDrawType(draw, i);
     DATA[i].parm = imlDrawParm(draw, i);
     if (i == 1)
       {
         if (DATA[i].func == IMG_LINE) N++;
         continue;
       }
     if (( i > 1)&&(i < 5))
       {
         if (DATA[i].func == IMG_VECTOREX) N++;
         continue;
       }
     if (DATA[i].func == IMG_SEMANTIC) N++;
  }

  CpyMet.PolyCounts  = RealCnt;
  CpyMet.Points      = inPoint;

  // если подсунули левый знак, есть подобъекты - рисуем линию
  if ((N < 7) || (CpyMet.Count !=1 ))
   {
     if ((DATA[1].func == 0)||DATA[1].parm == 0)
       {
         IMGLINE stline;
         stline.Color = 0;
         stline.Thick = 2048;
         if ((imgPrepareImage(&CpyMet, IMG_LINE, (char *)&stline,
                      context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }
       }
     else
       {
         if ((imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm,
                      context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }
       }
    return 0;
  }

  // ------------- Считываем нужные нам семантики  -----------------
  SH.cut1 = -1; SH.cut2 = -1;
  double sem = 0.0;
  long InsIndex = -1;

  IMGLONG * parm = 0;
  // семантика Обрезка-прямой курс (мм.)
  parm = (IMGLONG *)DATA[5].parm;
  if (parm &&  (parm->Value>0))
  {
    imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &sem);
    SH.cut1 =  abs((int)(1000*sem));
  }

  parm = (IMGLONG *)DATA[6].parm;
  sem = 0.0;
  if (parm &&  (parm->Value>0))           // семантика Обрезка-прямой курс (мм.)
  {
    imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &sem);
    SH.cut2 =  abs((int)(1000*sem));
  }

  parm = (IMGLONG *)DATA[7].parm; // семантика Обрезка-прямой курс (мм.)
  if (parm &&  (parm->Value>0))
    imlGetSemanticLong((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &InsIndex);

  // sOffset - величина обрезки в микронах
  // индекс точки врезки знаков InsIndex = 0;
  if ((*RealCnt - InsIndex <3) || (InsIndex < 1)) InsIndex = -1;
  if (InsIndex == -1) // не указан сегмент врезки,
                      // или номер сегмента отсутствует в метрике
     {
       if ((imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm,
                     context, test, border) != 0) &&  (test == PREPARE_TEST))
         {
           return 1;
         }

       return 0;
     };

  InsIndex = InsIndex-1; // номер превращаем в индекс в массиве

  // тут ясно что нужно врезаться в  сегмент с номером InsIndex
  // InsIndex+1 точка вставки курса
  // InsIndex+2 точка вставки параметров
  // InsIndex+3 точка окончания маршрута
  // CpyMet.PolyCounts^ - указатель (ставится от врезки кончиком внутрь знака)

  int HighPnt = *RealCnt-1;

  /*------------- ПОДГОТОВКА МЕТРИКИ --------------- */

  DRAWPOINT tmet[4], R[2];
  IMGVECTOREX * V = (IMGVECTOREX * )DATA[2].parm;
  if (V!= 0)
      SH.sArr = (int)ROUND(context->Scale*context->PixelX*V->HSize);
  // адекватные обрезки от 0 до 2 см, остальные считаем ошибками
  SH.cut1 = ((SH.cut1>0) && (SH.cut1<=20000)) ?
                (int)ROUND(context->Scale*context->PixelX*SH.cut1) : 0;
  SH.cut2 = ((SH.cut2>0) && (SH.cut2<=20000)) ?
                (int)ROUND(context->Scale*context->PixelX*SH.cut2) : 0;
  V = (IMGVECTOREX * )DATA[3].parm;
  if (V != 0)
     SH.sV1= abs((int)ROUND(context->Scale*context->PixelX*V->HSize));
  V = (IMGVECTOREX * )DATA[4].parm;
  if (V != 0)
    SH.sV2 = abs((int)ROUND(context->Scale*context->PixelX*V->HSize));

  R[0] = *inPoint;  R[1]= *(inPoint+HighPnt);
  SH.Dist = IntDistance(R[0],R[1]);
  if (InsIndex == 0) SH.Dist = SH.Dist-SH.cut1;
  if (InsIndex == *RealCnt-3) SH.Dist = SH.Dist-SH.cut2;

  R[0] = *(inPoint+InsIndex);
  R[1] = *(inPoint+InsIndex+3);
SH.sAngle = arctan2(R[1].y-R[0].y, R[1].x-R[0].x);  // 31.03.2014
  SH.cAngle = cos(SH.sAngle);
  SH.sAngle = sin(SH.sAngle);

  // обрезка длинных линейных
  tmet[0] = R[0];
  tmet[3] = R[1];
  if (InsIndex == 0)
    {
      tmet[0].x = (R[0].X + ROUND(SH.cut1*SH.cAngle));
      tmet[0].y = (R[0].Y + ROUND(SH.cut1*SH.sAngle));
    }

  if (InsIndex == HighPnt-3)
   {
    tmet[3].x = (R[1].X - ROUND(SH.cut2*SH.cAngle));
    tmet[3].y = (R[1].Y - ROUND(SH.cut2*SH.sAngle));
  }

  if (SH.Dist - SH.sArr > 0)
   {  // 1. знак
     // втискиваем с запасом по пикселю
      if( SH.Dist - SH.sArr - SH.sV1 > 2)
         SH.vTrue = 1;
      else
         SH.vTrue = 0;
   }
  else   // 2. курс
     // не втиснули ничего рисуем линию - выходим
   {
    ret = imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm, context, test, border);
    ret = ((ret != 0) && (test == PREPARE_TEST)) ? 1 : 0;
    return ret;
   };

  // корректируем точки метрики по линии
  for (i = 1; i <= 2; i++)
    {
       ret = OrthoPoint(*(inPoint+InsIndex+i),tmet[0],tmet[3], &tmet[i]);
       if (ret!=1)
          {
             ret = imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm,
                                           context, test, border);
             ret = ((ret != 0) && (test == PREPARE_TEST)) ? 1 : 0;
             return ret;
          }
    };


  // return imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm, context, test, border);


  SH.Da = ROUND(IntDistance(tmet[0], tmet[2]));
  SH.sArr  = SH.sArr / 2;
  if ((SH.Da < SH.sArr) || (SH.Dist-SH.Da < SH.sArr)) // в позицию точки, знак не вклинивается
   {
    ret = imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm, context, test, border);
    ret = ((ret != 0) && (test == PREPARE_TEST)) ? 1 : 0;
    return ret;
   }

  SH.sV1   = SH.sV1 / 2;
  SH.D1 = IntDistance(tmet[0], tmet[1]);
  if (SH.vTrue)
    {
     if ((SH.D1 - SH.sV1 > 1) && (SH.Da - SH.sArr - SH.D1 - SH.sV1 >1 ))
         SH.vTrue = 1;
     else
         SH.vTrue = 0;
    }

   for (i = 0; i <= 2; i++)
      SH.MET[i] = tmet[0];

   // врезаем указатель
   SH.MET[3].x = ROUND(tmet[2].X - SH.sArr*SH.cAngle);
   SH.MET[3].y = ROUND(tmet[2].Y - SH.sArr*SH.sAngle);
   SH.MET[4].x = ROUND(tmet[2].X + SH.sArr*SH.cAngle);
   SH.MET[4].y = ROUND(tmet[2].Y + SH.sArr*SH.sAngle);

   for (i = 5; i <= 7; i++)
      SH.MET[i] = tmet[3];

   SH.vRev  = 1;
   SH.MET[5].x = ROUND(tmet[3].X - SH.sV2*SH.cAngle);
   SH.MET[5].y = ROUND(tmet[3].Y - SH.sV2*SH.sAngle);

   // врезаем курс
   if (SH.vTrue)
     {
       SH.MET[1].x = ROUND(tmet[1].X - SH.sV1*SH.cAngle);
       SH.MET[1].y = ROUND(tmet[1].Y - SH.sV1*SH.sAngle);
       SH.MET[2].x = ROUND(tmet[1].X + SH.sV1*SH.cAngle);
       SH.MET[2].y = ROUND(tmet[1].Y + SH.sV1*SH.sAngle);
     }

   int j = 0;
   int D = 0;
   ret = 0;
   if (InsIndex > 0)
    {
      number = InsIndex+1;
      CpyMet.PolyCounts = &number;
      // обрезка частей дуги спереди
      if (!SH.cut1)
         {
            j  = InsIndex+1;
            D = 0;
            for (i = 0; i <= InsIndex; i++)
            {
               D = D + IntDistance(*(inPoint+i), *(inPoint+i+1));
               if (SH.cut1 >= D)
                  {
                    CpyMet.Points = (inPoint+i+1);
                    j--;
                  }
            }
            number = j;
         }
         if ((imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm,
                      context, test, border) != 0) && (test == PREPARE_TEST))
             {
               return 1;
             }
  }


  number = 2;
  CpyMet.PolyCounts = &number;
  CpyMet.Points = &SH.MET[0];
  if ((imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm,
                       context, test, border) != 0) &&  (test == PREPARE_TEST))
        {
          return 1;
        }
  if (SH.vTrue)
   {
     CpyMet.Points = &SH.MET[1];
     if ((imgPrepareImage(&CpyMet, DATA[3].func, DATA[3].parm,
                       context, test, border) != 0) &&  (test == PREPARE_TEST))
        {
          return 1;
        }
   }

  CpyMet.Points = &SH.MET[2];
  if ((imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm,
                       context, test, border) != 0) &&  (test == PREPARE_TEST))
        {
          return 1;
        }

  CpyMet.Points = &SH.MET[3];
  if ((imgPrepareImage(&CpyMet, DATA[2].func, DATA[2].parm,
                       context, test, border) != 0) &&  (test == PREPARE_TEST))
        {
          return 1;
        }

  CpyMet.Points = &SH.MET[4];
  if ((imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm,
                       context, test, border) != 0) &&  (test == PREPARE_TEST))
        {
          return 1;
        }

  if (SH.vRev)
    {
      CpyMet.Points = &SH.MET[5];
      if ((imgPrepareImage(&CpyMet, DATA[4].func, DATA[4].parm,
                       context, test, border) != 0) &&  (test == PREPARE_TEST))
        {
          return 1;
        }
  }

  // дорисовываем остаток
  if (InsIndex < HighPnt-3)
   {
     number = HighPnt - InsIndex-2;
     CpyMet.Points = inPoint+InsIndex+3;
    if (!SH.cut2)
      {
       // обрезка частей дуги сзади
       j = HighPnt - InsIndex - 2;
       D = 0;
       for (i = HighPnt; i >= InsIndex+3; i--)
         {
           D = D + IntDistance(*(inPoint+i-1), *(inPoint+i));
           if (SH.cut2 >= D) j--;
         }
       CpyMet.PolyCounts = &j;
      }

     if ((imgPrepareImage(&CpyMet, DATA[1].func, DATA[1].parm,
                       context, test, border) != 0) &&  (test == PREPARE_TEST))
        {
          return 1;
        }
   }

  return 0;
}

//----------------------------------------------------------------------
// Бокс с линией выноски
//----------------------------------------------------------------------
int _fastcall DrawVecArrow(const POLYDATAEX* pdata, const IMGDRAW* draw,
     const IMGCONTEXT * context, int test, FRAME * border)
{
  int          ret;
  int          markfunc;
  const char * markparm;
  int          linefunc;
  const char * lineparm;
  int          arrowfunc;
  const char * arrowparm;
  DRAWPOINT dP, first, last, cross;
  DRAWPOINT CR[4];

  POLYDATAEX data    = *pdata;
  int spcnt = *(pdata->PolyCounts);
  if (spcnt < 2)  return 0;
  int pntcnt = spcnt;

  data.PolyCounts = &pntcnt;
  data.Points = (DRAWPOINT *)(pdata->Points);

  linefunc  = imlDrawType(draw, 1);
  lineparm  = imlDrawParm(draw, 1);   // линия
  arrowfunc = imlDrawType(draw, 2);
  arrowparm = imlDrawParm(draw, 2);   // стрелка
  markfunc  = imlDrawType(draw, 3);
  markparm  = imlDrawParm(draw, 3);   // знак

  IMGVECTOREX V  = *((IMGVECTOREX *)markparm);

  // делаем установку флага горизонтально, знак всегда должен быть горизонтален
  // генерируем точку вставки по размерам векторного знака

  V.Static =1;

  // дельта между геометрическим центром и точкой привязки
  dP.x = (V.HLine1+V.HLine2)/2-(V.PosH);
  dP.y = (V.VLine1+V.VLine2)/2-(V.PosV);
  double delta;

  // переводим в пиксели
  // если один из размеров 0 - пропорции одинаковые, вычислим по другому
  if (V.HSize == 0) // сжатие по высоте
    {
      if ( V.VSize != 0) // если один из размеров 0 - пропорции одинаковы
        {
          delta = (double)(V.VLine2 - V.VLine1);
          if ( delta < 1) delta = 1.0;
          V.HSize = ((double)(V.HLine2 - V.HLine1) * V.VSize)/ delta;
                     (double)(V.VLine2 - V.VLine1);
        }
    }
   else
    {
       if (V.VSize == 0) // сжатие по ширине
         {
          delta = (double)(V.HLine2 - V.HLine1);
          if ( delta < 1) delta = 1.0;

           V.HSize = ((double)(V.VLine2 - V.VLine1) * V.HSize)/ delta;
         }
    }

  dP.x  = ROUND(context->Scale*(V.HSize/2+dP.x)*context->PixelX);
  dP.y  = ROUND(context->Scale*(V.VSize/2+dP.y)*context->PixelY);

  first = *(data.Points);
  last  = *(data.Points+spcnt-1);

  // вычисляем возможные точки начала метрики знака

  CR[0].x = last.x + dP.x;   CR[0].y = last.y + dP.y;
  CR[1].x = last.x + dP.x;   CR[1].y = last.y - dP.y;
  CR[2].x = last.x - dP.x;   CR[2].y = last.y - dP.y;
  CR[3].x = last.x - dP.x;   CR[3].y = last.y + dP.y;
  cross.x = 0; cross.y = 0;

  // ищем максимальное расстояние от крайней точки pE до "углов знака"

  int MaxLen  = -100000000;
  int CurrLen = 0;
  int crossindex = 0;
  for (int i = 0; i <= 3; i++)
  {
    CurrLen = IntDistance(first, CR[i]);
    if (CurrLen > MaxLen)
      {
        cross = CR[i];
        crossindex = i;
        MaxLen = CurrLen;
      }
  }

  // используем массив  CR
  // первая точка - пересечение, вторая должна быть крайняя (конец)

   CR[0] = first;   CR[1] = last;
   pntcnt = 2;
  data.Points = &CR[0];

  // рисуем линию выноски от  1 точки до ближайшего угла знака
  ret = imgPrepareImage(&data, linefunc, lineparm, context, test, border);
  if ((ret != 0) && (test == PREPARE_TEST))  return 1;

  // теперь знак. первая точка метрики в третьем элементе, вторая в четвёртом
 /* CR[0]   = first;
  CR[1].x = first.X + 50; CR[1].y = first.y;  */
  CR[0]   =  cross;
  if (crossindex < 2)
     CR[1].x = cross.X - 50;
  else
     CR[1].x = cross.X + 50;
  CR[1].y = cross.y;
  data.Points = &CR[0];


  // рисуем знак
  ret = imgPrepareImage(&data, markfunc, markparm, context, test, border);
  if ((ret != 0) && (test == PREPARE_TEST))  return 1;

  int dist = 0;
  if (arrowparm)
    {
      V = *((IMGVECTOREX *)arrowparm);
      // вычисление точек указателя
      dist  = ROUND(context->Scale * context->PixelX *  V.Border);


     // если длина стрелки менее длины между 1-й и 2-й точками метрики, она не рисуется вместе с линией
     if (IntDistance(last,first) > dist)
       {
         // если 2 точки то вторая вычисленная, поэтому стрелку рисуем углом от неё
         float sAngle = arctan2(last.y - first.y, last.x - first.x);   // 31.03.2014
         CR[1].x = ROUND(first.X-dist*cos(sAngle));
         CR[1].y = ROUND(first.Y-dist*sin(sAngle));
       }
     else
       dist = 0;
   }
  if (dist > 0)
   { // рисуем стрелку, если нужно
    CR[0] = first;
    data.Points = &CR[0];
    return imgPrepareImage(&data, arrowfunc, arrowparm, context, test, border);
  }
  return 0;
}

//----------------------------------------------------------------------
//  Знак с поворотом по семантике
//----------------------------------------------------------------------
int _fastcall DrawRotatedVector(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  int          arrowfunc;
  const char * arrowparm;
  POLYDATAEX data = *pdata;
  data.Points = (DRAWPOINT *)(pdata->Points);
  DRAWPOINT B[2];
  B[0] =   *(DRAWPOINT *)(pdata->Points);

  arrowfunc = imlDrawType(draw, 1);
  arrowparm = imlDrawParm(draw, 1);  // знак

  if (imlDrawType(draw, 2) == IMG_SEMANTIC)
   {
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 2);
     double angle = 0;
     if (parm &&  (parm->Value>0))
       {
         imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &angle);
         angle = PI*angle/180.0;   // angle угол в радианах
         // знак должен ориентироваться от центра, в сторону обратную азимуту
         B[1].x = B[0].x + ROUND(500*sin(angle));
         B[1].y = B[0].y - ROUND(500*cos(angle));
         int pntcnt = 2;
         data.PolyCounts = &pntcnt;
         data.Points = &B[0];
         return imgPrepareImage(&data, arrowfunc, arrowparm, context, test, border);
      }
   }
  return imgPrepareImage(&data, arrowfunc, arrowparm, context, test, border);

}

//----------------------------------------------------------------------
// Масштабируемый знак (по ширине)
//----------------------------------------------------------------------
int _fastcall DrawScaledVector(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border)
{

  int          vecfunc;
  const char * vecparm;
  POLYDATAEX data = *pdata;
  char param[2048];
  data.Points = (DRAWPOINT *)(pdata->Points);
  vecfunc = imlDrawType(draw, 1);
  vecparm = imlDrawParm(draw, 1);  // знак

  if (imlDrawType(draw, 2) == IMG_SEMANTIC)
   {
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 2);
     double fScale = 100.0;
     if (parm &&  (parm->Value>0) &&
           (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic),
                      parm->Value, 1, &fScale) != 0))
      {
        if (fScale <= 1)
           fScale = 100.0;
        fScale = 0.01 * fScale;
        if (((IMGVECTOREX *)vecparm)->Length < sizeof(param))
          { // сделаем копию параметров, если нужно
            memmove(param, vecparm,((IMGVECTOREX *)vecparm)->Length);
            int h = ((IMGVECTOREX *)param)->HSize;
           ((IMGVECTOREX *)param)->HSize  = (int)((double)h*fScale);
            h = ((IMGVECTOREX *)param)->Border;
           ((IMGVECTOREX *)param)->Border = (int)((double)h*fScale);
           int ret = imgPrepareImage(&data, vecfunc, param, context, test, border);
           return ret;
         }
      }
  }
  return imgPrepareImage(&data, vecfunc,vecparm, context, test, border);

};

//----------------------------------------------------------------------
// ЗНАК ПО АЗИМУТУ ДАЛЬНОСТИ
//----------------------------------------------------------------------
int _fastcall DrawAzimuthDistanceVector(const POLYDATAEX* pdata, const IMGDRAW* draw,
                       const IMGCONTEXT * context, int test,
                       FRAME * border)
{
  int  linefunc;
  const char * linedata;
  int  vecfunc;
  const char * vecdata;
  POLYDATAEX data = *pdata;
  data.Points = (DRAWPOINT *)(pdata->Points);
  DRAWPOINT one =  *(data.Points);
  double azim = 0.0;
  double dist = 0.0;
  IMGLINE line;
  long   color = 0xFFFFFFFF;
  linefunc = imlDrawType(draw, 1);
  linedata = imlDrawParm(draw, 1); // линия
  if ( linefunc != IMG_LINE)
    {
      line.Color = 0;
      line.Thick = 512;
    }
  else
    {
      line.Color = ((IMGLINE *)linedata)->Color;
      line.Thick = ((IMGLINE *)linedata)->Thick;
    }
  vecfunc  = imlDrawType(draw, 2);
  vecdata = imlDrawParm(draw, 2);  // знак

  IMGLONG * parm = 0;

  if (imlDrawType(draw, 3) == IMG_SEMANTIC)
    {  // семантика азимут
      parm = (IMGLONG *)imlDrawParm(draw, 3);
      if (parm &&  (parm->Value>0))
        imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &azim);
   }

  if (imlDrawType(draw, 4) == IMG_SEMANTIC)
   {  // семантика дистанция
     parm = (IMGLONG *)imlDrawParm(draw, 4);
     if (parm &&  (parm->Value>0))
        imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &dist);
   }

  if (imlDrawType(draw, 5) == IMG_SEMANTIC)
    {  // семантика цвет линии
      parm = (IMGLONG *)imlDrawParm(draw, 5);
      if (parm &&  (parm->Value>0))
        imlGetSemanticLong((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &color);
    }

  DOUBLEPOINT AD;

  AD.Y = PI * azim / 180;
  AD.X = 1000 * context->Scale * context->PixelX * dist;

  DRAWPOINT P[2];
  int pntcnt = 2;
  data.PolyCounts = &pntcnt;
  data.Points = &P[0];
  P[0]   = one;
  P[1].x = (int)(one.X+AD.X*sin(AD.Y));
  P[1].y = (int)(one.Y-AD.X*cos(AD.Y));

  if (color != 0xFFFFFFFF)
    {
      line.Color = color;
    }
  int ret = imgPrepareImage(&data, IMG_LINE, (char *)&line, context, test, border);
  if ((ret != 0) && (test == PREPARE_TEST))  return 1;

  P[0]   = P[1];
  P[1].x = (int)(P[0].X  + 50 * sin(AD.Y));
  P[1].y = (int)(P[0].Y  - 50 * cos(AD.Y));

  return imgPrepareImage(&data, vecfunc, vecdata, context, test, border);

}

//----------------------------------------------------------------------
// Линия с изменяющейся шириной из семантики
//----------------------------------------------------------------------
int _fastcall DrawVarWidthLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
         const IMGCONTEXT * context, int test, FRAME * border)
{
  POLYDATAEX data = *pdata;
  const char * param;
  data.Points = (DRAWPOINT *)(pdata->Points);
  int spcnt = *(pdata->PolyCounts);
  if (spcnt < 2)  return 0;

  IMGLINE line;
  param = (imlDrawParm(draw, 1));
  if (param != 0)
    {
      line = *(IMGLINE *)(param);
    }
  else
    {
      line.Color = 0;
      line.Thick = 512;
    }

  char value[255];
  memset(value,0,sizeof(value));

  IMGLONG * parm = 0;
  if (imlDrawType(draw, 2) == IMG_SEMANTIC)
     {  // семантика азимут
       parm = (IMGLONG *)imlDrawParm(draw,2);
       if (parm &&  (parm->Value>0))
         imlGetSemanticString((SEMANTIC *)(pdata->Semantic),parm->Value, 1,
                                value, sizeof(value));
     }

  double m2px = 1.0;

  if (data.ShowScale != 0)
     m2px = imlDistanceToPixel(context, data.ShowScale, data.Points, 1);

  WIDTHSEGMENT segm[32];
  memset(&segm[0],0,sizeof(segm));

  int cnt = 32;
  if (spcnt < cnt) cnt = spcnt;
  int bvar = 0;
  int i,j;
  int valuecount = strlen(value);
  for (i = 0; i < cnt-1; i++)
  {
    segm[i].first =  *(data.Points+i);
    segm[i].last  =  *(data.Points+i+1);
    segm[i].valid   = 1;

    for (j = bvar; j < valuecount; j++)
        {
          if ( value[j] == ';')
            {
              value[j] = 0;
              break;
            }
        }
    if (bvar <= valuecount)
        {
          segm[i].pxWidth = atoi(value + bvar) * m2px;
        }
    else
        {
          if (i == 0)
             segm[0].pxWidth = line.Thick;
          else
             segm[i].pxWidth = segm[i-1].pxWidth;
        }
    bvar = j+1;
  }

  DRAWPOINT buff[2];
  int pntcnt = 2;
  data.PolyCounts = &pntcnt;
  data.Points = &buff[0];
  int ret = 0;
  for (i = 0; i < 31; i++)
   {
    if (segm[i].valid == 1)
     {
       buff[0]  = segm[i].first;
       buff[1]  = segm[i].last;
       line.Thick = (int)(264.583333 * segm[i].pxWidth);
       ret = imgPrepareImage(&data, IMG_LINE, (const char *)&line, context, test, border);
       if ((ret != 0) && (test == PREPARE_TEST))
         {
           return 1;
         }
     }
   }

 return 0;
}

//----------------------------------------------------------------------
// Утолщающаяся линия в конце
//----------------------------------------------------------------------
int _fastcall DrawObjectGroup(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border)

{
  int spcnt = *(pdata->PolyCounts);
  //  нарисовать можно только в какую то сторону поэтому число точек не менее 2-х
  if (spcnt< 2)  return 0;
  int count = 2;

  DRAWPOINT * old  = (DRAWPOINT *)(pdata->Points); // старая метрика
  POLYDATAEX data = *pdata;
  data.PolyCounts = &count;
  data.Count  = 1;
  // берём объект
  int  objfunc;
  const char * objdata;
  objfunc = imlDrawType(draw, 1);
  objdata = imlDrawParm(draw, 1); // объект
  // берём метрику пикселя
  double m2px = 1;
  if (data.ShowScale != 0)
    m2px = imlDistanceToPixel(context, data.ShowScale, data.Points, 1);

  // берём семантики, поочерёдно выуживая нужные значения,
  //  и подставляя дефолтные туда, где их нет
  IMGLONG * parm = 0;
  long rows = 0;
  long cols = 0;
  double rowdist = 0.0;
  double coldist = 0.0;
  int  symm = 0;

  if (imlDrawType(draw, 2) == IMG_SEMANTIC)
   {  // семантика Рядов вдоль метрики
      parm = (IMGLONG *)imlDrawParm(draw, 2);
      if (parm &&  (parm->Value>0))
         imlGetSemanticLong((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &rows);
   }
  if (rows < 1) rows = 1;

  if (imlDrawType(draw, 3) == IMG_SEMANTIC)
   {  // семантика Объектов в ряду (вправо)
      parm = (IMGLONG *)imlDrawParm(draw, 3);
      if (parm &&  (parm->Value>0))
         imlGetSemanticLong((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &cols);
   }
  if (cols<1) cols = 1;

  if (imlDrawType(draw, 4) == IMG_SEMANTIC)
   {  // семантика  Расстояние между рядами (м)
     parm = (IMGLONG *)imlDrawParm(draw, 4);
     if (parm &&  (parm->Value>0))
        imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &rowdist);
   }

  if (imlDrawType(draw, 5) == IMG_SEMANTIC)
   {  // семантика  Расстояние между объектами (м)
    parm = (IMGLONG *)imlDrawParm(draw, 5);
    if (parm &&  (parm->Value>0))
      imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &coldist);
  }

  if (imlDrawType(draw, 6) == IMG_SEMANTIC)
   {  // семантика Объектов в ряду (вправо)
     parm = (IMGLONG *)imlDrawParm(draw, 6);
     long buff = 0;
     if (parm &&  (parm->Value>0))
        imlGetSemanticLong((SEMANTIC *)(pdata->Semantic),parm->Value, 1, &buff);
     if (buff != 0) symm = 1;
     else           symm = 0;
  }

  double OFSX = 0.0;
  if (symm)
     OFSX = 0.5 * coldist;
  // рисуем вдоль метрики объекта rows рядов. В каждом ряду cols  объектов
  // от точки в правую сторону растояние между рядяами rowdist
  // метров между объектоами в ряду coldist метров
  // Флаг симметрия:
  //  - не установлен: первый объeкт ряда начинается на линии второй и более остаются
  //  - установлен: объекты смещаются на половину coldist и размножаются по обе стороны
  DRAWPOINT P[2], R[2];
  DOUBLEPOINT AD,delta;
  data.Points = &R[0];
  int  ret = 0;
  int  i = 0;
  double D = 0.0;

  for (int k = 0; k < spcnt-1; k++)
  {
    P[0] = *(old+k);
    P[1] = *(old+k+1);
    delta.x = P[1].X - P[0].X;
    delta.y = P[1].y - P[0].y;
    AD.x = sqrt(delta.x*delta.x + delta.y*delta.y); // в пикcелах
 AD.y = arctan2(delta.y, delta.x);   // 31.03.2014
    P[1] = P[0]; i = 0;
    // есть осевая и есть угол, рисуем
    while ((i < rows) && (AD.x >= 0))
    {
      for (int j = -symm * (cols); j < cols; j++)
      {
        D    = (j * coldist + OFSX) * m2px; // в пискелах
        R[0].x = P[1].x + (int)(D * cos(AD.y + PI/2));
        R[0].y = P[1].y + (int)(D * sin(AD.y + PI/2));
        R[1].x = R[0].x + (int)(100.0 * cos(AD.y));
        R[1].y = R[0].y + (int)(100.0 * sin(AD.y));
        ret = imgPrepareImage(&data,  objfunc, objdata, context, test, border);
        if ((ret != 0) && (test == PREPARE_TEST))
           return 1;
      }

      D  = (i+1) * rowdist * m2px;
      P[1].x = P[0].X + (int)(D * cos(AD.Y));
      P[1].y = P[0].Y + (int)(D * sin(AD.Y));
      AD.X = AD.X - rowdist * m2px;      // отнимаем пиксели
      i++;
    }
    rows = rows - i;
  }
  return 0;
}

// *****************************************************************************
// ***                               КРУГ МБВ                               ****
// *****************************************************************************
// Объект по знаку, рекомендуется делать векторным с двумя точками.
// Знак состоит из 3 векторных примитивов и семантики:
// 1-й знак - вид указателя курса, 2-й вид подписи высоты 3-й вид центра
// семантика содержит  перечень пар курс:высота через точку с запятой
// Например 10:400;170:650;320:500 три сектора 10-170(Н=400), 170-320(Н=650), 320-10(Н=500)
// Ориентация знака на север осуществляется по метрике объекта. Отсчёты угла
// осуществляется от направления между 1-й и 2-й точкой метрики, по часовой стрелке


// ------------------------------------------------------------------------------
// Замена значения текста
// Если замена была - возвращает 1
// ------------------------------------------------------------------------------
int _fastcall _SetVectorText(IMGVECTOREX * vector, char * text)
{
    IMGPOLYDESC * desc;
    IMGVECTORTEXT * vtext;
    long flagleng = 0;
    desc = &(vector->Desc);
    for(int i = 0; i < vector->Count; i++)
       {
          if (desc->Image == IMG_VECTORTEXT)  // нашли текст
            {
              vtext = (IMGVECTORTEXT *)&(desc->Parm);
              strncpy(vtext->Title, text, 32);
              flagleng = 1;
            }
           long delta = desc->Length - 4;
           // Размер фрагмента
           int size = sizeof(IMGPOLYDESC) +
              (*(long *)((char *)(&(desc->Count))+delta)- 1) *
                      sizeof(IMGVECTPOINT) + delta;
           desc = (IMGPOLYDESC *)((char *)desc + size);
       }

    return flagleng;
}

int _fastcall DrawSectorMSA(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border)
{

  int RealCnt = *pdata->PolyCounts;
  if (RealCnt < 2)  return 0;    /* если количество точек менее 2 - выход */

  POLYDATAEX cpyMET = *pdata;
  DRAWPOINT * inPoint = (DRAWPOINT *)(pdata->Points); // входящая метрика

  MSAITEM  IM[16];
  memset((char *)&IM[0],0,sizeof(IM));

  char buffCourse[1024];
  memset((char *)&buffCourse[0],0,sizeof(buffCourse));
  char buffHeight[1024];
  memset((char *)&buffHeight[0],0,sizeof(buffHeight));

  DRAWPOINT    met[38];
  memset((char *)&met[0],0,sizeof(met));

  // рисуем окружность
  met[36] = *(inPoint+1);
  met[37] = *(inPoint);

  met[36].x = met[36].x-met[37].x;
  met[36].y = met[36].y-met[37].y;
  double Dist = sqrt(met[36].x*met[36].x+ met[36].y*met[36].y);

  for (int index = 0; index <= 36; index++)
  {
    met[index].x = met[37].x+int(Dist*cos(PI*index/18));
    met[index].y = met[37].y+int(Dist*sin(PI*index/18));
  }

  int pntcnt = 37;
  cpyMET.PolyCounts = &pntcnt;
  cpyMET.Points     = &met[0];
  const IMGLINE  vLine = {0, 250};
  int ret = imgPrepareImage(&cpyMET, IMG_LINE, (char *)&vLine, context, test, border);

  const char * vCourse  = imlDrawParm(draw, 1);  // знак курса
  int tCourse           = imlDrawType(draw, 1);
  const char * vHeight  = imlDrawParm(draw, 2);  // высота
  int tHeight           = imlDrawType(draw, 2);
  const char * vCenter  = imlDrawParm(draw, 3);  // центр

  // рисуем центр, безусловно
  pntcnt = 2;
  met[0] = *(inPoint);
  met[1] = met[0];
  met[1].x = met[1].x+10;

  if ((imgPrepareImage(&cpyMET, IMG_VECTOREX, vCenter,
                   context, test, border) != 0) &&  (test == PREPARE_TEST))
        {
          return 1;
        }

  // если есть несогласования в параметрах или их нет
  // эти знаки вообще предопределенные
  if ((tCourse != IMG_VECTOREX)|| (tHeight != IMG_VECTOREX) ||
      (vCourse == 0)||(vHeight == 0))
    return 0;
   // запишем их параметры в буфер, что бы менять текст
   if ( ((IMGVECTOREX *)vCourse)->Length < sizeof(buffCourse))
     memmove(buffCourse,vCourse,((IMGVECTOREX *)vCourse)->Length);
   else return 0;
   if ( ((IMGVECTOREX *)vHeight)->Length < sizeof(buffHeight))
     memmove(buffHeight,vHeight,((IMGVECTOREX *)vHeight)->Length);
   else return 0;

  IMGLONG * parm = 0;
  char buff[512];
  memset((char *)&buff[0],0,sizeof(buff));
  //  разбираем значение семантики  курс+высота
  if (imlDrawType(draw, 4) == IMG_SEMANTIC)
     {
       parm = (IMGLONG *)imlDrawParm(draw,4);
       if (parm &&  (parm>0))
          imlGetSemanticString((SEMANTIC *)(pdata->Semantic),parm->Value, 1,
                                buff, sizeof(buff));
     }

  int i,j;
  int L, N = 0;
  int count = 0;
  int indexaz = 0;
  int indexh= 0;
  int strsize = strlen(buff);

  for (i = 0; i < strsize; i++)
   {
      if (buff[i] == ':')  // разделитель между направлением и высотой
        {
           buff[i] = 0;
           IM[N].Azimuth = atoi(buff+indexaz);
           indexh = i+1;
           for (j = i+1; j < strsize; j++)
             {
               if (buff[j] == ';')  // разделитель между парами
                 {
                   buff[j] = 0;
                   indexaz = j+1;
                   break;
                 }
             }
           i = j;
           StrCopy(IM[N].Height,buff+indexh, sizeof(IM[N].Height));
           IM[N].Vector  = PI*(IM[N].Azimuth+90)/180;
           N = N + ((IM[N].Azimuth>-1) ? 1 : 0);
           IM[N].Azimuth = IM[0].Azimuth;
           IM[N].Vector  = IM[0].Vector;
           if (count == 15) break;
           count++;
        }

   }


  if ((N>-1) && (IM[N].Vector<IM[N-1].Vector))
   {
      IM[N].Vector = IM[N].Vector+2*PI;
   }


  // N- число секторов  Dist-длина до круга
  // нанесём пока сектора без модификации
  met[2] = *(inPoint);
  met[3] = *(inPoint+1);
  met[3].X = met[3].X-met[2].X;
  met[3].Y = met[3].Y-met[2].Y;
  if ((met[3].X == 0) && (met[3].Y == 0)) return 0;

 double A0 = PI+arctan2(met[3].Y, met[3].X);   // 31.03.2014

  if (N == 1)
   {
    met[0].X = met[2].x - 10;
    met[0].Y = met[2].y + ROUND(Dist*0.5);
    met[1].X = met[2].x + 20;
    met[1].Y = met[2].y;
    _SetVectorText((IMGVECTOREX *)buffHeight , IM[0].Height);
    if ((imgPrepareImage(&cpyMET, IMG_VECTOREX, buffHeight,
                   context, test, border) != 0) &&  (test == PREPARE_TEST))
        {
          return 1;
        }

   }
  else
   {
     for (int index = 0; index < N; index++)
      {
        double Azim = IM[index].Vector + A0;
        met[0].X = met[2].x + ROUND(Dist*0.9*sin(Azim));
        met[0].Y = met[2].y - ROUND(Dist*0.9*cos(Azim));
        met[1].X = met[2].x + ROUND(Dist*0.2*sin(Azim));
        met[1].Y = met[2].y - ROUND(Dist*0.2*cos(Azim));
        char mess[32];
        memset(mess,0,sizeof(mess));
        sprintf(mess,"%03d╟",IM[index].Azimuth);
       _SetVectorText((IMGVECTOREX *)buffCourse , mess);
        if ((imgPrepareImage(&cpyMET, IMG_VECTOREX, buffCourse,
                   context, test, border) != 0) &&  (test == PREPARE_TEST))
          {
            return 1;
          }

    Azim = 0.5*(IM[index].Vector+IM[index+1].Vector)+A0;
    met[0].X = met[2].x + ROUND(Dist*0.5*sin(Azim))-10;
    met[0].Y = met[2].y - ROUND(Dist*0.5*cos(Azim));            // 19/03/14 Федор
    met[1].X = met[0].X + 20;
    met[1].Y = met[0].Y;
    _SetVectorText((IMGVECTOREX *)buffHeight , IM[index].Height);
    if ((imgPrepareImage(&cpyMET, IMG_VECTOREX, buffHeight,
                   context, test, border) != 0) &&  (test == PREPARE_TEST))
        {
          return 1;
        }
    }

  }

 return 0;

};


