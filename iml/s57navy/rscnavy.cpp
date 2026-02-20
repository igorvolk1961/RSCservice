/****** RSCNAVY.CPP ************ Belenkov O.V.   ****** 29/01/13 ***
******* RSCNAVY.CPP ************ Kirpitchev A.V. ****** 11/10/01 ***
******* RSCNAVY.CPP ************ Dishlenko S.G.  ****** 29/04/13 ***
******* RSCNAVY.CPP ************ Panteleeva N.A. ****** 19/01/09 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2013              *
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

#ifndef RSCNAVY_H                                               
  #include "rscnavy.h"
#endif

#include <stdio.h>
#include <math.h>
                                    

#define INFARE51  10148
#define RSRDEF51  10226
#define ENTRES51  10118
#define ENTRES61  10119
#define ENTRES71  10120
#define ACHRES51  10004
#define ACHRES61  10005
#define ACHRES71  10006
#define FSHRES51  10139
#define FSHRES61  10140
#define FSHRES71  10141
#define CTYARE51  10092
#define CTYARE71  10093
#define ISODGR01  10150
#define DANGER01  10098
#define DANGER02  10099
#define DANGER03  10525
#define UWTROC03  10424                           
#define UWTROC04  10425
#define OBSTRN01  10177
#define OBSTRN03  10526
#define OBSTRN11  10179
#define LOWACC21  10494
#define LOWACCO1  10165
#define LOWACC31  10495
#define LOWACC41  10496
#define WRECKS05  10443
#define WRECKS01  10441
#define WRECKS04  10442
#define CBLONGT1  20001
#define CBLONGT2  20002
#define SOUNDSA1  10359
#define SOUNDGB1  10296
#define SOUNDSB1  10360
#define SOUNDSC2  10362

// Список служебных объектов                         // 01/11/07
SERVOBJECTS ListServObject[MAXSERVOBJECT] =
  {
    {INFARE51,0},{RSRDEF51,0},{ENTRES51,0},{ENTRES61,0},{ENTRES71,0},
    {ACHRES51,0},{ACHRES61,0},{ACHRES71,0},{FSHRES51,0},{FSHRES61,0},
    {FSHRES71,0},{CTYARE51,0},{CTYARE71,0},{ISODGR01,0},{DANGER01,0},
    {DANGER02,0},{DANGER03,0},{UWTROC03,0},{UWTROC04,0},{OBSTRN01,0},
    {OBSTRN03,0},{OBSTRN11,0},{LOWACC21,0},{LOWACCO1,0},{WRECKS05,0},
    {WRECKS01,0},{WRECKS04,0},{CBLONGT1,0},{CBLONGT2,0},{LOWACC31,0},
    {LOWACC41,0},{SOUNDSA1,0},{SOUNDGB1,0},{SOUNDSB1,0},{SOUNDSC2,0}
  };
/*
// Константы времени выполнения

double SHALLOW_CONTOUR = 10.0;   // Контур отмели
double DEEP_CONTOUR    = 10.0;  // Контур глубины   // ум. 30
double SAFETY_CONTOUR  = 5.0;  // Контур безопасности
long   TWO_SHADES      = 0;     // 1- градация из 2 цветов, 0 - из 4 цветов
long   SHALLOW_PATTERNS= 0;     // Заполнение мели (0 - нет, 1- да)
long   SHOW_ISDANGER_SHALLOW = 1; // Показывать изолированные опасности в мелкой воде
                                  // (0 - нет, 1- да)
*/

// Подписи к огням
long LitText(SEMANTIC * semantic, char * text,long length);

// Установить цвет и возможную заливку площадей
long ColorSeabed(double drval1, double drval2,long * defcolor,long * shallow);

// Уточнение значений глубины по семантике  DEPVAL02
int Depval(double* least,double * seabed, long * leastflag,
                     long * seabedflag,SEMANTIC * sem );

// Запрос кода отображаемого символа и флага отображения  UDWHAZ04
long Udwhaz(double depthValue, long * udwhazCode,long watlev,SEMANTIC * sem);

// Запрос кода отображаемого символа и флага отображения низкой точности QUAPNT02
long Quapnt(long * quapntCode,SEMANTIC * sem);

// Запрос числовой семантики с проверкой на отсутствие
long CheckDoubleSemasntic(SEMANTIC * sem, long semanticnumber,long number,
             double * val);

// Поиск координат середины линии
// координаты возвращает в point.
// При ошибке возвращает 0
long CheckMiddleLinePoint(DRAWPOINT * point,DRAWPOINT *points,long number);

// Замена кода семантики с 116 на 301 в параметрах текста векторного объекта
// Если замена была - возвращает 1
long ChangeText(IMGVECTOREX * vector);

// Заполним массив внутренних кодов служебных объектов
long FillServObject(HRSC hRsc);

// Найти внутренний код служебного объекта по внешнему
long SeekServObjectIncode(long excode);

// Подписи горизонталей и отметок глубин
int _fastcall DepthText(long color, long foncolor, double depth, DRAWPOINT * point,
                        const IMGCONTEXT * context, int test,FRAME * border);

// Знаки и цвет подписи отметок глубин
long SoundFrm(double depth,DRAWPOINT * point,SEMANTIC * sem,HRSC hRsc,
               const IMGCONTEXT * context, int test,FRAME * border);

// --------------------------------------------------------
// Ориентированный векторный знак, направление берется из
// семантической характеристики 117 (ORIENT)
// --------------------------------------------------------
int _fastcall OrientObject(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
 const char * vecparm;
 int    vecfunc;

 POLYDATAEX data;
 DRAWPOINT Points[2];
 int Number;                          // 26/03/12
 double angle;

 if (pdata == 0) return 0;

 // Запросить значение характеристики с кодом 117 (ORIENT,градусы)
 // (первое значение, если их несколько)
 if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) , 117, 1, &angle) != 0)
   {
     // Длина отрезка - 1 см на карте (условно)
     int Length = context->Scale * context->PixelX * 10000;

     data = *pdata;

     // Настраиваем адреса метрики
     data.Points     = Points;
     data.PolyCounts = &Number;
     Number          = 2;
     data.Count      = 1;

     Points[0].x     = pdata->Points[0].x;
     Points[0].y     = pdata->Points[0].y;

     // Перевести в радианы
     // Исходный угол от юга (?) по часовой стрелке
     // заменяем на угол от востока
     angle = (angle - 270.0) * M_PI / 180.0;

     // Учитываем поворот карты 11/10/01
     angle += context->Angle;

     Points[1].x = Points[0].x + Length * cos(angle);
     Points[1].y = Points[0].y + Length * sin(angle);

     pdata = &data;
   }

 // Тип функции
 vecfunc = imlDrawType(draw, 1);

 // Параметры функции
 vecparm = imlDrawParm(draw, 1);

 char addr[2*1024];
 long flagleng = 0;
 char semstr[256];

 if (IML_LANGUAGE == 1)   // нужна информация на нац.языке
   {
     if (vecfunc == IMG_VECTOREX)
       { // если есть семантика с национальным языком
         if (imlGetSemanticString((SEMANTIC *)(pdata->Semantic), 301, 1,
                                        semstr,sizeof(semstr)-1)!= 0)
            {
             IMGVECTOREX * vector = (IMGVECTOREX *)vecparm;
             if ((vector->Length >= sizeof(IMGVECTOREX))
                   &&(vector->Length < sizeof(addr)))
               {
                 memmove(addr,vecparm,vector->Length);
                 flagleng = ChangeText((IMGVECTOREX *)addr);
              }
            }
        }

   }
 // Параметры функции
 if (flagleng != 0)
   {
     vecparm = addr;
   }

 // Отобразить объект
 return imgPrepareImage(pdata, vecfunc, vecparm, context, test, border);
}

// --------------------------------------------------------
// Огни
// --------------------------------------------------------
int _fastcall Lights(const POLYDATAEX* pdata, const IMGDRAW* draw,
                     const IMGCONTEXT * context, int test,
                     FRAME * border)
{
 POLYDATAEX data;
 DRAWPOINT points[200];
 int  number;               // 26/03/12
 long int  type;
 long int      lightcolor;
 int       base;
 double    angle;
 double    sector1;
 double    sector2;
 int       dx,dy;
 DRAWPOINT p1, p2, p3, p0;
 IMGSQUARE imgsquare;
 float     scale;
 int       ret = 0;
 int       flare45 = 0;
 if (pdata == 0) return 0;

 // Настраиваем адреса метрики
 data.Points     = points;
 data.PolyCounts = &number;
 number          = 0;
 data.Count      = 1;

 p1.x            = pdata->Points[0].x;
 p1.y            = pdata->Points[0].y;

 // Запросить значение характеристики с кодом 37 (CATLIT, вид огня)
 // (первое значение, если их несколько)
 if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 37, 1, &type) == 0)
   {
     type = 0;
   }

 // Запросить значение характеристики с кодом 75 (COLOUR, цвет огня)
 // (первое значение, если их несколько)
 if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 75, 1,
                        &lightcolor) == 0)
   {
     // Умалчиваемый цвет
     lightcolor = LIGHTDEF;
   }
 else
   {
     switch (lightcolor)
     {
       case 1:
               // Запросить значение характеристики с кодом 75
               // (COLOUR, цвет огня) (второе значение)
               if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 37, 2,
                                      &lightcolor) == 0)
                 {
                   lightcolor = LIGHTS03;
                   flare45 = 1;
                   break;
                 }
               else
                 if (lightcolor == 4)
                   {
                     lightcolor = LIGHTS02;
                     break;
                   }
                 else
                   if (lightcolor != 3)
                     {
                       lightcolor = LIGHTS03;
                       break;
                     }

       case 3:
                lightcolor = LIGHTS01;
                break;

       case 4:
                lightcolor = LIGHTS02;
                break;

       case 6:
       case 11:
                lightcolor = LIGHTS03;
                flare45 = 1;
                break;

       default:
                lightcolor = LIGHTDEF;
                break;
     }
   }


 // Запросить значение характеристики с кодом 117 (ORIENT, градусы)
 // (первое значение, если их несколько)
 if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) , 117, 1, &angle) == 0)
   {
     if ((type == 8) || (type == 9))
       {
         // Умалчиваемый угол (восток)
         angle = 0;
       }
     else
       {
         // Умалчиваемый угол (юго-восток)
         angle = M_PI / 4;
       }
   }
 else
   {
     // Перевести в радианы
     // Исходный угол от юга (?) по часовой стрелке
     // заменяем на угол от востока
     angle = (angle - 270.0) * M_PI / 180.0;
   }

 // Пересчитать угол с учетом поворота карты      // 11/10/01
 angle = angle + context->Angle;

 // Определить габариты знака
 if (imlDrawType(draw, 1) == IMG_LONG)
   {
#ifdef RISCCPU                         // 25/01/05
     REGISTER regvalue;
     LongToRegister(regvalue, imlDrawParm(draw, 1));
     base = regvalue.Long;
#else
     base = ((IMGLONG *)imlDrawParm(draw, 1))->Value;
#endif
     if (base < 1000)
       base = 7000;
   }
 else
   {
     base = 7000;  // 7 мм
   }

 // Относительный масштаб отображения
 scale = context->Scale;
 if (scale < 0.5) scale = 0.5;

 // Габариты знака в пикселах
 base = scale * context->PixelX * base;

 p2.x = p1.x + base * cos(angle);
 p2.y = p1.y + base * sin(angle);

 if (type == 8)
   {
     // Рисуем LIGHTS82, габариты 3x7: O=
     // ----------------------------------------

     int i;
     IMGLINE imgline;

     imlBuildCircle(&p1, base*3/14, &data);

     // Рисуем знак заданным цветом
     imgline.Color = lightcolor;
     imgline.Thick = 260;             // 1 пиксел

     ret |= imgPrepareImage(&data, IMG_LINE,(const char *)&imgline,
                                      context, test, border);

     if ((ret != 0) && (test == PREPARE_TEST))
       {
         return 1;
       }

     dx  = p2.x - p1.x;
     dy  = p2.y - p1.y;

     p1.x = p1.x + dx * 2 / 7;
     p1.y = p1.y + dy * 2 / 7;

     points[0].x = p1.x;
     points[0].y = p1.y;
     points[1].x = p2.x;
     points[1].y = p2.y;
     number      = 2;

     ret |= imgPrepareImage(&data, IMG_LINE,(const char *)&imgline,
                                      context, test, border);

     if ((ret != 0) && (test == PREPARE_TEST))
       {
         return 1;
       }

     dx = dx * 3 / 14;
     dy = dy * 3 / 14;

     for (i = 0; i <= 1; i++)
     {
       points[0].x = p1.x + dy / 2;
       points[0].y = p1.y - dx / 2;
       points[1].x = p2.x + dy;
       points[1].y = p2.y - dx;
       number      = 2;

       ret |= imgPrepareImage(&data, IMG_LINE, (const char *)&imgline,
                                     context, test, border);

       if ((ret != 0) && (test == PREPARE_TEST))
         {
            return 1;
         }

       dx = -dx;
       dy = -dy;
     }

     return ret;
   }

 if (type == 9)
   {
     // Рисуем LIGHTS81, габариты 3x7: /\/\/\/\
     // ----------------------------------------

     IMGLINE imgline;
     int i,j;
     int pdx, pdy;

     pdx  = (p2.x - p1.x)/2;
     pdy  = (p2.y - p1.y)/2;

     p2.x = p1.x;
     p2.y = p1.y;
     p1.x = p2.x + pdy;
     p1.y = p2.y - pdx;
     p2.x = p2.x - pdy;
     p2.y = p2.y + pdx;

     pdx = p2.x - p1.x;
     dx  = pdx * 3 / 7;
     pdx = pdx/8;
     pdy = p2.y - p1.y;
     dy  = pdy * 3 / 7;
     pdy = pdy/8;

     j = 0;

     for (i = 0; i <= 8; i++)
     {
       points[i].x = p1.x + dy * j;
       points[i].y = p1.y - dx * j;

       p1.x = p1.x + pdx;
       p1.y = p1.y + pdy;

       if (j == 0) j = 1;
         else j = 0;
     }

     // Число точек
     number = 9;

     // Рисуем знак заданным цветом
     imgline.Color = lightcolor;
     imgline.Thick = 260;             // 1 пиксел

     ret |= imgPrepareImage(&data, IMG_LINE, (const char *)&imgline,
                                       context, test, border);

     if ((ret != 0) && (test == PREPARE_TEST))
       {
         return 1;
       }

     return ret;
   }

 // Запросить значение характеристики с кодом 136 (SECTR1, градусы)
 // (первое значение, если их несколько)
 if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) , 136, 1,
                                                         &sector1) != 0)
   {
     // Запросить значение характеристики с кодом 137 (SECTR2, градусы)
     // (первое значение, если их несколько)
     if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) , 137, 1,
                                                         &sector2) != 0)
       {
         double temp = sector2 - sector1;

         if (sector2 < sector1)
           temp += 360;

         if ((temp > 1) && (temp < 360))
           {
             type = -1;
           }

         // Перевести в радианы
         // Исходный угол от юга (?) по часовой стрелке
         // заменяем на угол от востока
         sector1 = (sector1 - 270.0) * M_PI / 180.0;
         sector2 = (sector2 - 270.0) * M_PI / 180.0;

         // Учитываем поворот карты  // 11/10/01
         sector1 += context->Angle;
         sector2 += context->Angle;
       }
   }

 if (type == -1)
   {
     // Рисуем сектор
     // --------------

     IMGDOT imgdot;
     int i;
     double sectorlength = 0;

     // Рисуем знак заданным цветом
     imgdot.Color = CHBLK;
     imgdot.Thick = 1 * 260;         // 1 пиксел
     imgdot.Dash  = 8 * 260;         // 8 -
     imgdot.Blank = 3 * 260;         // 3 -

     //********  19/09/01 **** Kirpitchev A.V. ******************
     // Учет области дальности видимости огня в милях VALNMR (178)
     // Показывть реальную зону видимости не получается  -
     // сильно большие вектора,
     // пришлось использовать суррогат  20mm + 2mm на каждую милю
     //**********************************************************
     long flaglength = 1;
     if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) , 178, 1,
                                                         &sectorlength)== 0)
        {
          sectorlength = 0.0;
          flaglength = 0;
        }

    if (flaglength != 0)
      {
        sectorlength = sectorlength * 185200.0; // сантиметры
        double maplength = sectorlength/pdata->ShowScale; // длина в см на карте
        base = context->Scale * context->PixelX * maplength *10000;

        number = 2;
        points[0].x = p1.x;
        points[0].y = p1.y;

        angle = sector1;

        for (i = 0; i <= 1; i++)
        {
          points[1].x = p1.x + base * cos(angle);
          points[1].y = p1.y + base * sin(angle);

          ret |= imgPrepareImage(&data, IMG_DOT, (const char *)&imgdot,
                                            context, test, border);

          if ((ret != 0) && (test == PREPARE_TEST))
            {
              return 1;
            }

          angle = sector2;
        }
      }

     //********  19/09/01 **** Kirpitchev A.V. ******************
     // Построить дугу радиусом 15mm + 1mm на каждую милю
     // в fddendium  или 20 или25 мм на экране
     //**********************************************************
     base = context->Scale * context->PixelX * 20000; //(15000 + sectorlength * 1000);

     p2.x = p1.x + base * cos(sector1);
     p2.y = p1.y + base * sin(sector1);

     p3.x = p1.x + base * cos(sector2);
     p3.y = p1.y + base * sin(sector2);

     imlBuildArc(&p2, &p1, &p3, base, &data);

     imgdot.Color = lightcolor;
     imgdot.Thick = 3 * 260;         // 3 пиксела (ширина линии)

     ret |= imgPrepareImage(&data, IMG_LINE, (const char *)&imgdot,
                                      context, test, border);

     if ((ret != 0) && (test == PREPARE_TEST))
       {
         return 1;
       }

     return ret;
   }

 // Рисуем LIGHTDEF выбранным цветом
 // --------------------------------

 dx = (p2.x - p1.x)/6;
 dy = (p2.y - p1.y)/6;

 // Центр полукруга (5/6 габаритов)
 p0.x = p1.x + dx * 5;
 p0.y = p1.y + dy * 5;

 // Первая точка дуги
 p2.x = p0.x + dy;
 p2.y = p0.y - dx;

 // Последняя точка дуги
 p3.x = p0.x - dy;
 p3.y = p0.y + dx;

 // Строим дугу (результат в points)
 data.Points = &points[1];
 imlBuildArc(&p2, &p0, &p3, base/6, &data);
 data.Points = &points[0];

 // Число точек дуги
 if (number < 1)
   number = 0;
 else
  if (number > sizeof(points)/sizeof(DRAWPOINT)-2)
    number = sizeof(points)/sizeof(DRAWPOINT)-2;

 // Первая точка
 points[0].x = p1.x;
 points[0].y = p1.y;

 // Последняя точка
 points[number+1].x = p1.x;
 points[number+1].y = p1.y;

 // Число точек = дуга + 2 (замкнуть)
 number = number + 2;

 // Рисуем знак заданным цветом
 imgsquare.Color = lightcolor;

 // Расчитать габариты знака
 data.Border.LT = p1.x;
 data.Border.UP = p1.y;
 data.Border.RT = p1.x;
 data.Border.DN = p1.y;

 if (data.Border.LT > p2.x)
   data.Border.LT = p2.x;
 else
  if (data.Border.RT < p2.x)
    data.Border.RT = p2.x;
 if (data.Border.UP > p2.y)
   data.Border.UP = p2.y;
 else
  if (data.Border.DN < p2.y)
    data.Border.DN = p2.y;

 if (data.Border.LT > p3.x)
   data.Border.LT = p3.x;
 else
  if (data.Border.RT < p3.x)
    data.Border.RT = p3.x;
 if (data.Border.UP > p3.y)
   data.Border.UP = p3.y;
 else
  if (data.Border.DN < p3.y)
    data.Border.DN = p3.y;

 base = base/4;
 data.Border.UP -= base;
 data.Border.LT -= base;
 data.Border.DN += base;
 data.Border.RT += base;


 ret |= imgPrepareImage(&data, IMG_SQUARE, (const char *)&imgsquare,
                                     context, test, border);

 char txt[64];
 char * ptxt[1];
 IMGTRUETEXT trtext;
 memset((char *)&trtext,0,sizeof(IMGTRUETEXT));

 if (LitText((SEMANTIC *)(pdata->Semantic), txt, sizeof(txt)-1)!= 0)
  {
    // выводим текст на экран
    number = 1;
    points[0].x = pdata->Points[0].x;
    points[0].y = pdata->Points[0].y;
    // Сдвиг по X в мм 10.5
    points[0].x += scale * context->PixelX * 7500;
    flare45 = 3;
    if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 32698, 1, &type) != 0)
      {
        if (type == 1)
             flare45 = 0;
        else
          if (type == 2)
              flare45 = 1;
      }

    if (flare45 == 1)
      {
        points[0].y -= scale * context->PixelX * 3500;
      }

    // points[1].x = p2.x;
    // points[1].y = p2.y;
    // number      = 2;

    strcpy(trtext.FontName,"Microsoft Sans Serif");
    trtext.Text.BkgndColor = IMGC_TRANSPARENT;
    trtext.Text.ShadowColor = IMGC_TRANSPARENT;
    trtext.Text.Height = 3500;
    trtext.Text.Weight = FW_THIN;
    trtext.Text.Wide   = UNIW_NORMAL;
    trtext.Text.Align  = FA_LEFT|FA_MIDDLE;
    if (flare45 == 1)
     {
       trtext.Text.Align  = FA_LEFT|FA_BOTTOM;
     }

    trtext.Text.CharSet = RUSSIAN_CHARSET;
    data.Text = ptxt;
    ptxt[0] = txt;
    if (flare45 < 3)
      ret |= imgPrepareImage(&data, IMG_TRUETEXT, (const char *)&trtext,
                                       context, test, border);
   // ::MessageBox(0,ptxt[0],"",MB_OK);

  }

 if ((ret != 0) && (test == PREPARE_TEST))
   {
     return 1;
   }

 return ret;
}


// --------------------------------------------------------
// Подписи к огням
// --------------------------------------------------------
long LitText(SEMANTIC * semantic, char * text,long length)
{
 long lvalue[5];
 int  i,find;
 int j=0,k=0;
 int  len;
 int  numgrp;
 char cgrp[2][10];
 char siggrpstr[10];
 long  chrvalue;
 char chrstr[10];
 char * ptr;
 double height;
 char vstext[12];
 memset(text,0,length);
 memset(chrstr,0,sizeof(chrstr));
 memset(siggrpstr,0,sizeof(siggrpstr));
 // Установили категорию огня
 // запрос семантики  CATLIT(может быть до 5 семантик)
 find = 0;
 for (i = 0; i < 5; i++)
  {
    if (imlGetSemanticLong(semantic, 37, i+1, &lvalue[i]) == 0)
      {
        lvalue[i] = 0;
        break;
      }

   switch(lvalue[i])
     {
       case 1: strcpy(text,"Dir");
               find = 1;
       break;
       case 6: strcpy(text,"Aero");
               find = 1;
       break;
     }
    // если нашли одну из семантик - подписали
    if (find == 1) break;
   }

  // Запрос описания характеристики огня
  // запрос семантики LITCHR (107)
  if ( imlGetSemanticLong(semantic, 107, 1, &chrvalue) != 0)
    {  // Сформировали строку по номеру характеристики
      switch (chrvalue)
       {
         case(1):	 strcpy(chrstr,"F");        break;
         case(2):	 strcpy(chrstr,"Fl");       break;
         case(3):	 strcpy(chrstr,"LFl");      break;
         case(4):	 strcpy(chrstr,"Q");        break;
         case(5):	 strcpy(chrstr,"VQ");       break;
         case(6):	 strcpy(chrstr,"UQ");       break;
         case(7):	 strcpy(chrstr,"Iso");      break;
         case(8):	 strcpy(chrstr,"Oc");       break;
         case(9):	 strcpy(chrstr,"IQ");       break;
         case(10): strcpy(chrstr,"IVQ");      break;
         case(11): strcpy(chrstr,"IUQ");      break;
         case(12): strcpy(chrstr,"Mo");       break;
         case(13): strcpy(chrstr,"FFl");      break;
         case(14): strcpy(chrstr,"Fl+LFl");   break;
         case(15): strcpy(chrstr,"AlOc Fl");  break;
         case(16): strcpy(chrstr,"FLFl");     break;
         case(17): strcpy(chrstr,"AlOc");     break;
         case(18): strcpy(chrstr,"AlLFl");    break;
         case(19): strcpy(chrstr,"AlFl");     break;
         case(20): strcpy(chrstr,"Al");       break;
         case(25): strcpy(chrstr,"Q+LFl\0");  break;
         case(26): strcpy(chrstr,"VQ+LFl");   break;
         case(27): strcpy(chrstr,"UQ+LFl");   break;
         case(28): strcpy(chrstr,"Al");       break;
         case(29): strcpy(chrstr,"AlF Fl");   break;
       }
   }

  // запрос описаний группы
  // запрос семантики SIGGRP (141)
  if (imlGetSemanticString(semantic, 141, 1, siggrpstr,sizeof(siggrpstr)-1)== 0)
   {// нет семантики - обнулим
     memset(siggrpstr,0,sizeof(siggrpstr));
   }
  numgrp = 0;
  len = strlen ( siggrpstr);
  // разбиваем на  2 группы по наличию )
  while ( j < len )
    {if ( siggrpstr[j] != ')' )
       cgrp[numgrp][k++] = siggrpstr[j++];
     else
      {	cgrp[numgrp][k++] = siggrpstr[j++];
	cgrp[numgrp++][k] = '\0';
        k=0;
      }
    }
   if (numgrp == 0)
     {
       sprintf(vstext,"%s",chrstr);
       strcat(text,vstext);
     }
   else
     { // несколько групп сигналов
       // сигнал составной
       ptr = strchr(chrstr,'+');
       if (ptr != NULL)
          {
           ptr = strtok(chrstr,"+");
           sprintf(vstext,"%s",ptr);
           strcat(text,vstext);
           if (!strncmp(cgrp[0],"()",2));
           else
              {
                sprintf(vstext,"%s",cgrp[0]);
                strcat(text,vstext);
              }
           ptr = strtok(NULL,"+");
           sprintf(vstext,"+%s%s",ptr,cgrp[1]);
           strcat(text,vstext);
          }
       else
          {
           sprintf(vstext,"%s",chrstr);
           strcat(text,vstext);
           while(i < numgrp)
              {
                if (!strncmp(cgrp[i],"()",2));
                else
                  {
                   sprintf(vstext,"%s",cgrp[i]);
                   strcat(text,vstext);
                  }
                i++;
              }
          }
     }


   // Разбор цветов      COLOUR - 75
   i = 1;
   find = imlGetSemanticLong(semantic,75,i,&chrvalue);
   while (find != 0)
     {
       switch(chrvalue)
	     	{
			  case(1): sprintf(vstext,"%s","W");
					break;
			  case(3):	sprintf(vstext,"%s","R");
					break;
			  case(4):	sprintf(vstext,"%s","G");
					break;
			  case(6):	sprintf(vstext,"%s","Y");
					break;
		   }
       i++;
      strcat(text,vstext);
      find = imlGetSemanticLong(semantic,75,i,&chrvalue);
     }

	strcat(text," ");

   // Запишем сигнальный период  SIGPER-142
   memset(vstext,0,sizeof(vstext));
   find = imlGetSemanticLong(semantic,142,1,&chrvalue);
	if( find > 0)
	    {
	     sprintf(vstext,"%ds",chrvalue);
        strcat(text,vstext);
	    }

   // Запишем высоту HEIGHT  95
   find = imlGetSemanticDouble(semantic,95,1,&height);
   if(find != 0)
     {
	   if(height > 0)
        {
         sprintf(vstext,"%.0fm",height);
         strcat(text,vstext);
        }
     }

   // VALNMR 178 (значение номинальной дальности)
   find = imlGetSemanticDouble(semantic,178,1,&height);
   if(find != 0)
     {
	   if(height > 0)
        {
          sprintf(vstext,"%.0fM",height);
          strcat(text,vstext);
        }
     }

  // Запишем статус STATUS - 149
   i = 1;
   find = imlGetSemanticLong(semantic,149,i,&chrvalue);
   while (find != 0)
     {
       switch(chrvalue)
	     	{
			  case(2): sprintf(vstext," %s","(occas)");
                    strcat(text,vstext);
					break;
			  case(7):	sprintf(vstext," %s","(temp)");
                     strcat(text,vstext);
					break;
			  case(8):	sprintf(vstext," %s","(priv)");
                     strcat(text,vstext);
					break;
           case(11):	sprintf(vstext," %s","(exting)");
                     strcat(text,vstext);
					break;
		   }
        i++;
      find = imlGetSemanticLong(semantic,75,i,&chrvalue);
     }

  return 1;

}



// ------------------------------------------------------------------------
// Ориентированный векторный знак в центре площади, направление берется из
// заданной семантической характеристики              // 28/12/06
// ------------------------------------------------------------------------
int _fastcall CentreObject(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  const char * parm;
  int          func;
  int          ret = 0;
  char semstr[256];
  long         semanticnumber;
  double angle;
  int length;
  int shiftX;
  int shiftY;
  int flagleng;
  double * shift;
  if (pdata == 0) return 0;
  int   count = *(pdata->PolyCounts); // Число точек объекта  // 26/03/12
  DRAWPOINT* point = pdata->Points;
  DRAWPOINT  gravpoint[2];
  DRAWPOINT  shpoint;
  if (imlDrawType(draw, 2) == IMG_SEMANTIC)               // 20/02/02
   {
     IMGLONG * parm = (IMGLONG *)imlDrawParm(draw, 2);
     if (parm)
       semanticnumber = parm->Value;
   }

  if (CentreGravity(pdata,&gravpoint[0]) == 0)
    return 0;

  // Нужно сдвинуться на смещение по параметрам
  // Сдвиг по X
  shiftX = 0;
  shiftY = 0;
  if (imlDrawType(draw, 3) == IMG_DOUBLE)
   {  shift = (double *)imlDrawParm(draw, 3);
      if (shift == 0)
        {
          shiftX = 0;
        }
      else
        {
          if ((*shift > 0.01)||(*shift < -0.01))
           {
             shiftX = context->Scale * context->PixelX * 1000 * (*shift);
           }
          else
            {
             shiftX = 0;
            }
        }
   }
  if (imlDrawType(draw, 4) == IMG_DOUBLE)
   {
     // Сдвиг по Y
     shift = (double *)imlDrawParm(draw, 4);
     if (shift == 0)
       {
         shiftY = 0;
       }
     else
       {
         if ((*shift > 0.01)||(*shift < -0.01))
           {
             shiftY = context->Scale * context->PixelY * 1000 * (*shift);
           }
         else
           {
             shiftY = 0;
           }
       }
   }

  // Получим точку входящую в объект - для центра
  NearCentreGravityPoint(pdata,&gravpoint[0]);
  if (test != EXAMPLE_VIEW)  // Для example - не сдвигать
    {
      if ((shiftX != 0)||(shiftY != 0) )    // смещенная от центра точка
       {
          shpoint.X = gravpoint[0].x+shiftX;
          shpoint.Y = gravpoint[0].y+shiftY;
          NearCentreGravityPoint(pdata,&shpoint);
          if ((gravpoint[0].x+shiftX != shpoint.X)||
             (gravpoint[0].x+shiftX != shpoint.X)   ) // не попало рядом с центром
            {
              // не рисуем
              return 0;
            }
          else
            {
              gravpoint[0].x = shpoint.X;
              gravpoint[0].y = shpoint.Y;
            }
       }
    }
  else
    {
      test = 0;
    }
  // Длина отрезка - 1 см на карте (условно)
  length = context->Scale * context->PixelX * 10000;
  // Запросим значение семантики(градусы)
  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,semanticnumber ,
                             1, &angle) != 0)
   {
     // Перевести в радианы
     angle = angle * M_PI / 180.0;

     // Учитываем поворот карты 11/10/01
     angle += context->Angle;

     gravpoint[1].x = gravpoint[0].x + length * cos(angle);
     gravpoint[1].y = gravpoint[0].y + length * sin(angle);
   }
 else
   {
     gravpoint[1].x = gravpoint[0].x + length;
     gravpoint[1].y = gravpoint[0].y;
   }

  // рисуем по 2 точкам
  count = 2;
  POLYDATAEX data;
  data.PolyCounts = &count;
  data.Points = &gravpoint[0];
  data.Count      = 1;
  data.Semantic = pdata->Semantic;
  data.MapRsc = pdata->MapRsc;

  data.Border.UP = gravpoint[0].Y - 32;
  data.Border.LT = gravpoint[0].X - 32;
  data.Border.DN = gravpoint[0].Y + 32;
  data.Border.RT = gravpoint[0].X + 32;

 // Тип функции
 func = imlDrawType(draw, 1);

 // Параметры функции
 parm = imlDrawParm(draw, 1);
 char addr[2*1024];
 flagleng = 0;

 if (IML_LANGUAGE == 1)   // нужна информация на нац.языке
   {
     if (func == IMG_VECTOREX)
       { // если есть семантика с национальным языком
         if (imlGetSemanticString((SEMANTIC *)(pdata->Semantic), 301, 1,
                                        semstr,sizeof(semstr)-1)!= 0)
            {
             IMGVECTOREX * vector = (IMGVECTOREX *)parm;
             if ((vector->Length >= sizeof(IMGVECTOREX))
                   &&(vector->Length < sizeof(addr)))
               {
                 memmove(addr,parm,vector->Length);
                 flagleng = ChangeText((IMGVECTOREX *)addr);
               }
            }
        }

   }
 // Параметры функции
 if (flagleng != 0)
   {
     parm = addr;
   }

 ret = imgPrepareImage(&data, func, parm, context, test, border);

 return ret;
}

// ------------------------------------------------------------------------
//  Вычисление ближайшей по горизонтали к центру тяжести точки, // 28/12/06
//  входящей в объект
// ------------------------------------------------------------------------
int _fastcall NearCentreGravityPoint(const POLYDATAEX* pdata,
                                                         DRAWPOINT * gravpoint)
{
  int i,j;
  if ((pdata == 0)||(gravpoint == 0)) return 0;

  // Проверка попадания в область
  LEVELCROSSPOINT cross;
  LEVELCROSSPOINT crossr[2];
  LEVELCROSSPOINT crossl[2];
  long rcount = 0;
  long oldZ = 0;
  memset((char *)&crossr,0,sizeof(crossr));
  memset((char *)&crossl,0,sizeof(crossl));
  memset((char *)&cross,0,sizeof(cross));
  int  count = *(pdata->PolyCounts); // Число точек объекта
  DRAWPOINT* point = pdata->Points;
  DRAWPOINT* pointend = pdata->Points+count-1;
  point = pdata->Points;
  for (j = 0; j < count; j++)   // пропустим от начала метрики точки,равные последней)
   { if (gravpoint->Y != point->y) break;

     if ((point->x != pointend->x)||(point->y != pointend->y))
             break;
          point++;

   }

  for( i = j; i < count-1; i++)
    {
       if (SeekHorizontalCross(point, gravpoint->Y,&cross) == 0)
        {
          point++;
          continue;
        }
       if (cross.Z == oldZ)
        {
          point++;
          continue;
        }
       else
        {
         oldZ = cross.Z; // запомнили
        }
       if (cross.Z >= gravpoint->X)  // справа
         {
           rcount++;                // считаем правые пересечения
           // заполняем массив справа
           if (crossr[0].Node == 0) // не было ничего
             {
               crossr[0].Node = i;
               crossr[0].Direct = cross.Direct;
               crossr[0].Z = cross.Z;
             }
           else
             { //  не было второго
               if (crossr[1].Node == 0)
                 {
                  crossr[1].Node = i;
                  crossr[1].Direct = cross.Direct;
                  crossr[1].Z = cross.Z;
                 }
               else
                 {
                    if (( cross.Z > crossr[0].Z)&&( cross.Z > crossr[1].Z))
                      continue;
                    if (crossr[0].Z > crossr[1].Z) crossr[0].Z = cross.Z;
                    else crossr[1].Z = cross.Z;
                 }

             }
         }
       else
         {
           // заполняем массив слева
           if (crossl[0].Node == 0) // не было ничего
             {
               crossl[0].Node = i;
               crossl[0].Direct = cross.Direct;
               crossl[0].Z = cross.Z;
             }
           else
             { //  не было второго
               if (crossr[1].Node == 0)
                 {
                  crossl[1].Node = i;
                  crossl[1].Direct = cross.Direct;
                  crossl[1].Z = cross.Z;
                 }
               else
                 {
                    if (( cross.Z < crossl[0].Z)&&( cross.Z < crossl[1].Z))
                      continue;
                    if (crossl[0].Z < crossl[1].Z) crossl[0].Z = cross.Z;
                    else crossl[1].Z = cross.Z;
                 }
             }
         }
      point++;
    }
  if (rcount != (rcount/2)*2)  // нечетное - внутри
      return 1;

  if (abs(crossl[0].Z - crossl[1].Z)- abs(crossr[0].Z - crossr[1].Z) >= 0)
   {
    gravpoint->X = (crossl[0].Z + crossl[1].Z)/2;
   }
  else
   {
    gravpoint->X = (crossr[0].Z + crossr[1].Z)/2;
   }

  return 1;
}

                                                       // 06/02/07
// ------------------------------------------------------------------------
// Системный знак выбираемый по значению семантической характеристики RESTRN(131)
// ------------------------------------------------------------------------
int _fastcall RestrnObject(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  int          func;
  int          i;
  int          ret = 0;
  int          resp;
  long         incode;
  long         code[32];   //  code[0] - количество семантик у объекта
                           //  максимально у объекта может быть 27
                           // значений данной семантики
  long         objectcode;
  if (pdata == 0) return 0;

  // Запросим значение семантики()  // их может быть много
  memset((char *)&code[0],0,sizeof(code));
  i = 1;
  while (i)
    {if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic), 131,i,&code[i])== 0)
       break;
     if (i ++ > 32) break;
    }
  code[0] = i-1;
  // нет у объекта такой семантики не рисует ничего
  if (code[0] == 0) return 0;

  FillServObject(pdata->MapRsc);

  objectcode = 0;
  // Уточним код знака для рисования
  resp = 0;
  for (i = 1; i <= code[0]; i++)
     {
       if ((code[i] == 7)||(code[i] == 8)||(code[i] == 14))
          {
           resp = 1;   // Продолжение А
           break;
          }
     }
  if (resp == 0)
     {
       for (i = 1; i <= code[0]; i++)
         {
           if ((code[i] == 1)||(code[i] == 2))
             {
               resp = 2;  // Продолжение B
               break;
             }
         }
     }

  if (resp == 0)
     {
       for (i = 1; i <= code[0]; i++)
         {
           if ((code[i] <= 6 )||(code[i] == 24))
             {
               resp = 3;  // Продолжение C
               break;
             }
         }
     }

  if (resp == 0)
     {
       for (i = 1; i <= code[0]; i++)
         {
           if ((code[i] == 13 )||(code[i] == 16 )||(code[i] == 17 )||(code[i] >= 23))
             {
               resp = 4;  // Продолжение D
               break;
             }
         }
     }
  if (resp == 0)
     {
       for (i = 1; i <= code[0]; i++)
         {
           if (code[i] < 27 )
             {
               objectcode = INFARE51;
               break;
             }
         }
       if (objectcode == 0) objectcode = RSRDEF51;
     }
  if (resp == 1) // Продолжение А
     {
       for (i = 1; i <= code[0]; i++)
         {
           if ((code[i] <= 6)||(code[i] == 13)||(code[i] == 16)||
               (code[i] == 17)||(code[i] >= 23))
            {
             objectcode = ENTRES61;
             break;
            }
         }
       if ( objectcode == 0 )
         {
           for (i = 1; i <= code[0]; i++)
             {
               if ((code[i] > 9)&&(code[i]!= 14))
                {
                  objectcode = ENTRES71;
                  break;
                }
             }
         }
        if ( objectcode == 0 ) objectcode = ENTRES51;
     }

  if (resp == 2) // Продолжение B
     {
       for (i = 1; i <= code[0]; i++)
         {
           if (((code[i] >= 3)&&(code[i] <= 6))||(code[i] == 13)||(code[i] == 16)||
               (code[i] == 17)||(code[i] >= 23))
            {
             objectcode = ACHRES61;
             break;
            }
         }
       if ( objectcode == 0 )
         {
           for (i = 1; i <= code[0]; i++)
             {
               if (code[i] > 2)
                {
                  objectcode = ACHRES71;
                  break;
                }
             }
         }
        if ( objectcode == 0 ) objectcode = ACHRES51;
     }

  if (resp == 3) // Продолжение C
     {
       for (i = 1; i <= code[0]; i++)
         {
           if ((code[i] >= 25)||(code[i] == 13)||(code[i] == 16)||
               (code[i] == 17)||(code[i] == 23))
            {
             objectcode = FSHRES61;
             break;
            }
         }
       if ( objectcode == 0 )
         {
           for (i = 1; i <= code[0]; i++)
             {
               if ((code[i] >= 9)&&(code[i]!= 24)&&(code[i]!= 14))
                {
                  objectcode = FSHRES71;
                  break;
                }
             }
         }
        if ( objectcode == 0 ) objectcode = FSHRES51;
     }
  if (resp == 4) // Продолжение В
     {
       for (i = 1; i <= code[0]; i++)
         {
           if ((code[i] >= 9)&&(code[i] <= 12)||(code[i] == 15)||
               (code[i] >= 18)||(code[i] <= 22))
            {
             objectcode = CTYARE71;
             break;
            }
         }
        if ( objectcode == 0 ) objectcode = CTYARE51;
     }

  // Запросим параметры выбранного объекта
  incode = SeekServObjectIncode(objectcode);

  // Тип функции
  func = IMG_OBJECT;
  IMGOBJECT parobj;
  parobj.Color = IMGC_NOMARK;          // Цвет выделения объекта на карте
  parobj.Incode = incode;
  parobj.Excode = objectcode;          // Внешний код объекта
  parobj.Local  = IMGL_SQUARE;         // Локализация объекта

  // Параметры функции
  return imgPrepareImage(pdata, func,(char *)&parobj, context, test, border);
}

                                                  // 06/02/07
// ------------------------------------------------------------------------
// Знак с изображением выбранным по значениям заданной семантической
//  характеристики              // 30/01/07
// ------------------------------------------------------------------------
int _fastcall SeriaObject(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  const char * parm;
  int          func;
  int          semanticnumber = 0;
  int          marknumber = 0;
  double       semvalue = 0;

  char semstr[256];
  char addr[4*1024];

  if (pdata == 0) return 0;
  marknumber = 0;

  if (imlDrawType(draw, 2) == IMG_SEMANTIC)               // 20/02/02
   {
     IMGLONG * lparm = (IMGLONG *)imlDrawParm(draw, 2);
     if (lparm)
       semanticnumber = lparm->Value;
     else marknumber = 1;              // нет семантики, рисуем умалчиваемый
   }
  else
   {
     marknumber = 1;                   // нет семантики, рисуем умалчиваемый
   }

  if (marknumber == 0)
   {
     // Запросим значение семантики (не знаем что)
     if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,semanticnumber ,
                             1, &semvalue) == 0)
      {
        marknumber = 1;     // нет значения семантики, рисуем умалчиваемый
      }
   }

  // Найдем, какому знаку соответствует полученное значение семантики
  if (marknumber == 0)
    for (int i = 0; i < 4; i++)
      {
        IMGDOUBLE * min = (IMGDOUBLE *)imlDrawParm(draw, 3+i*3);
        IMGDOUBLE * max = (IMGDOUBLE *)imlDrawParm(draw, 4+i*3);
        if ((min->Value <= semvalue)&& (max->Value >= semvalue)) // попали в интервал
          {
           marknumber = i+2;
           break;
          }
      }
  if (marknumber <= 1)
     marknumber = 1;
  else
     marknumber = marknumber*3 - 1;

  // Параметры функции
  parm = imlDrawParm(draw, marknumber);

  // Тип функции
  func = IMG_DRAW;

  int flagleng = 0;

 if (IML_LANGUAGE == 1)   // нужна информация на нац.языке
   {
     // если есть семантика с национальным языком
     if (imlGetSemanticString((SEMANTIC *)(pdata->Semantic), 301, 1,
                                        semstr,sizeof(semstr)-1)!= 0)
        {
           if (((IMGDRAW*)parm)->Length < sizeof(addr))
               { // cоздали копию параметров
                 memmove(addr,parm,((IMGDRAW*)parm)->Length);
                 // переберем части
                 for (int i = 1; i <= ((IMGDRAW*)parm)->Count; i++)
                   {
                      func = imlDrawType((IMGDRAW*)addr, i);
                      if (func == IMG_VECTOREX)
                        {
                          IMGVECTOREX * vector =
                                  (IMGVECTOREX *)imlDrawParm((IMGDRAW*)addr, i);
                          if (ChangeText(vector) == 1)
                             flagleng = 1;
                        }
                   }
               }
        }

   }
 // Параметры функции
 if (flagleng != 0)
   {
     parm = addr;
   }

  return imgPrepareImage(pdata, IMG_DRAW, parm, context, test, border);
}
                                                  // 05/10/07
// ------------------------------------------------------------------------
// Построение площадных зон глубины
// ------------------------------------------------------------------------
int _fastcall DepareObject(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  int          code;                        // 29/04/13
  int          func;
  long         semanticnumber;
  double       drval1,drval2;
  long         defcolor = 0;
  long         shallow  = 0;
  long         ret;
  if (pdata == 0) return 0;

  code = 0;
  // лучше не цвет, а код объекта
  if (imlDrawType(draw, 1) == IMG_LONG)               // 20/02/02
   {
     int * cparm = (int *)imlDrawParm(draw, 1);       // 29/04/13
     if (cparm)
       code = *cparm;
   }

  semanticnumber = 87;
  // Запросим значение семантики DRVAL1
  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,semanticnumber ,
                             1, &drval1) == 0)
      {
        drval1 = -1.0;     // нет значения семантики,
      }


  // Запросим значение семантики DRVAL2
  semanticnumber = 88;
  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,semanticnumber ,
                             1, &drval2) == 0)
      {
        drval2 = drval1 + 0.01;     // нет значения семантики,
      }


  // Установить цвет и возможную заливку площадей
  ColorSeabed(drval1,drval2,&defcolor,&shallow);
  IMGSQUARE square;
  square.Color = defcolor;
  // Тип функции
  func = IMG_SQUARE;


  ret = imgPrepareImage(pdata, func, (char *)&square, context, test, border);

    // Если пользователь хочет штриховку - по правилам DIAMOND1 - можно
  // штрихованный площадной (45 и 135)
  if (shallow == 1)
    {
     int  scparm[6];                         // 29/04/13
     scparm[0] = sizeof(scparm);
     scparm[1] = 45;
     scparm[2] = 9000;
     scparm[3] = IMG_LINE;
     scparm[4] = DEPCN;
     scparm[5] = 1;

     // Тип функции
     func = IMG_HATCHSQUARE;

     ret = imgPrepareImage(pdata, func, (char *)&scparm[0], context, test, border);
     scparm[1] = 135;
     ret = imgPrepareImage(pdata, func, (char *)&scparm[0], context, test, border);
   }

 // Если объект DRGARE - углубленный район
 if (code == 46)
  {
    // Нарисовать заливку DRGARE01


    // Нарисовать контур
    // (DASH,1.CHGRF)

    // Выбрать знак по ограничениям
    RestrnObject(pdata, draw,context,test,border);
  }

 // Цикл по компонентам
 // Разобрать компоненты

 // Подписать контура
 // SAFCON

  return ret;
}

// ------------------------------------------------------------------------
// Установить цвет и возможную заливку площадей морского дна
// ------------------------------------------------------------------------
long ColorSeabed(double drval1, double drval2,long * defcolor,long * shallow)
{
  if (TWO_SHADES      == 1)    // градация из 2 цветов
    {
      *defcolor = DEPIT;
      *shallow  = 1;
      if ((drval1 >= 0.0) && (drval2 > 0.0))
        {
           *defcolor = DEPVS;
        }
      if ((drval1 >= SAFETY_CONTOUR)&&(drval2 > SAFETY_CONTOUR))
        {
           *defcolor = DEPDW;
           *shallow  = 0;
        }
    }
  else                         // 4 цвета
    {
       *defcolor = DEPIT;
       *shallow  = 1;
       if ((drval1 >= 0.0) && (drval2 > 0.0))
        {
           *defcolor = DEPVS;
        }
       if ((drval1 >= SHALLOW_CONTOUR)&&(drval2 > SHALLOW_CONTOUR))
        {
           *defcolor = DEPMS;
        }
       if ((drval1 >= SAFETY_CONTOUR)&&(drval2 > SAFETY_CONTOUR))
        {
           *defcolor = DEPMD;
           *shallow  = 0;
        }
      if ((drval1 >= DEEP_CONTOUR)&&(drval2 > DEEP_CONTOUR))       //01/11/07
        {
           *defcolor = DEPDW;
           *shallow  = 0;
        }
    }

 if ((SHALLOW_PATTERNS == 0)||(*shallow  == 0))
    *shallow  = 0;
 else
    *shallow  = 1;
 return 1;
}

                                                  // 31/10/07
// ------------------------------------------------------------------------
// Построение точечных, линейных, площадных препятствий
// ------------------------------------------------------------------------
int _fastcall ObstrnObject(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  long         semanticnumber;
  long         incode;
  int          func;
  int i;
  double valsou;
  double depthValue;
  double leastDepth;
  double seabedDepth;        // только для вызова Depval - не используется
  long   seabedDepthFlag;    // только для вызова Depval - не используется
  long   valsouFlag;
  long    ret;
  long    catobs;
  long    watlev;
  long    udwhazFlag;
  long    udwhazCode;
  long    quapntCode;
  long    quapntFlag;
  long    quapos;
  long    depthValueFlag = 0;    // 1 - значение вычислено, 0 - нет
  long    leastDepthFlag = 0;    // 1 - значение вычислено, 0 - нет
  long    local;
  long    code;
  long    sounding;
  long    pointcode;

  if (pdata == 0) return 0;

  // Заполним массив внутренних кодов служебных семантик
  FillServObject(pdata->MapRsc);

  local = 2;
  // параметр - локализация объекта
  if (imlDrawType(draw, 1) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 1);             // 29/04/13
     if (cparm)
       local = *cparm;
     if ((local > 2)||(local < 0)) local = 2; // При любых неприятностях -
                                              // считаем,объект точечный
   }
  code = 0;
  // параметр - код объекта
  if (imlDrawType(draw, 2) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 2);              // 29/04/13
     if (cparm)
       code = *cparm;
   }

  semanticnumber = 179;
  if (CheckDoubleSemasntic((SEMANTIC *)(pdata->Semantic) ,semanticnumber ,
                                    1, &valsou) == 0)
     {
        // нет значения семантики,
        valsouFlag = 0;
        Depval(&leastDepth,&seabedDepth, &leastDepthFlag,&seabedDepthFlag,
                   (SEMANTIC *)(pdata->Semantic));
        if (leastDepthFlag == 1)
          {
            depthValue = leastDepth;
          }
        else
          {
            // запросим значение семантики CATOBS - 42
            if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 42, 1,
                                   &catobs) == 0) catobs = -1;
            if (catobs == 6) depthValue = 0.01;
            else
              {
                // запросим значение семантики WATLEV - 187
                if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 187, 1,
                                                &watlev) == 0) watlev = -1;

                if (watlev == 5) depthValue = 0.0;
                else
                  {
                    if (watlev == 3) depthValue = 0.01;
                    else             depthValue = 15.0;
                  }
              }
           }
        depthValueFlag = 1; // уточнили значение семантики
        //sprintf(mess,"depthValue = %lf",depthValue);
        //::MessageBox(0,mess,"",MB_OK);
      }
  else
      {
        valsouFlag = 1;
        depthValue  = valsou;
        depthValueFlag = 1; // есть значение семантики
      }

 // Запрос кода отображаемого символа и флага отображения
 udwhazFlag = Udwhaz(depthValue, &udwhazCode,watlev,(SEMANTIC *)(pdata->Semantic));
 if (local == 2)  // точечные объекты
   {
     if ((code != 153)&&(code != 86))
        return 0;
     // Запрос кода отображаемого символа и флага отображения низкой точности QUAPNT02
     quapntFlag = Quapnt(&quapntCode,(SEMANTIC *)(pdata->Semantic));
     if (udwhazFlag)
       {
         // Запросим параметры выбранного объекта
         incode = SeekServObjectIncode(udwhazCode);
         //sprintf(mess,"udwhazCode = %lu,incode = %lu ",udwhazCode,incode);
         //::MessageBox(0,mess,"",MB_OK);
         // Тип функции
         func = IMG_OBJECT;
         IMGOBJECT parobj;
         parobj.Color = IMGC_NOMARK;          // Цвет выделения объекта на карте
         parobj.Incode = incode;
         parobj.Excode = udwhazCode;          // Внешний код объекта
         parobj.Local  = IMGL_POINT;          // Локализация объекта
         // Параметры функции
         ret = imgPrepareImage(pdata, func,(char *)&parobj, context, test, border);
         if (quapntFlag)
            {
              incode = SeekServObjectIncode(quapntCode);
              parobj.Excode = quapntCode;          // Внешний код объекта
              parobj.Incode = incode;
              ret = imgPrepareImage(pdata, func,(char *)&parobj, context, test, border);
            }
         return ret;
       }
     sounding = 0;
     pointcode = 0;
     if (valsouFlag == 1)
      {
        if (valsou <= 20.0)
          {
            if(code == 153)     // UWTROC
              {// выбор подходящего символа и подписи
                if (watlev == 3)
                  {
                    pointcode = DANGER01;
                    sounding = 1;
                  }
                else
                  {
                    if ((watlev == 4)||(watlev == 5))
                      {
                         pointcode = UWTROC04;
                         sounding = 0;
                      }
                    else
                      {
                         pointcode = DANGER01;
                         sounding = 1;
                      }
                  }
              }
            else   // это код объекта OBSTRN - 86
              {
                if (catobs == 6)
                  {
                    pointcode = DANGER01;
                    sounding = 1;
                  }
                else
                  {
                     if ((watlev == 1)||(watlev == 2))
                       {
                          pointcode = OBSTRN11;
                          sounding = 1;
                       }
                     else
                       {
                         if (watlev == 3)
                           {
                             pointcode = DANGER01;
                             sounding = 1;
                            }
                         else
                            {
                              if ((watlev == 4)||(watlev == 5))
                                {
                                  pointcode = DANGER03;
                                  sounding = 1;
                                }
                              else
                                {
                                 pointcode = DANGER01;
                                 sounding = 1;
                                }
                            }
                         }
                    }
              }
          }
       else // VALSOU > 20
          {
             pointcode = DANGER02;
          }

      }
    else  // VALSOU - не задана
      {
         if (code == 153)     // UWTROC
           {
             if (watlev == 3) pointcode = UWTROC03;
             else pointcode = UWTROC04;  // и по умолчанию
           }
         else   // OBSTRN
           {
              if (catobs == 6){ pointcode = OBSTRN01;}
              else
                {
                  if ((watlev == 1)||(watlev == 2)) {pointcode =OBSTRN11;}
                  else
                    {
                      if (watlev == 3) {pointcode = OBSTRN01;}
                      else
                        {
                          if ((watlev == 4)||(watlev == 5))
                            { pointcode = OBSTRN03;}
                          else { pointcode = OBSTRN01;}
                        }
                    }
                }
           }
      }
    // отображение
    // Запросим параметры выбранного объекта
    incode = SeekServObjectIncode(pointcode);
    // Тип функции
    func = IMG_OBJECT;
    IMGOBJECT parobj;
    parobj.Color = IMGC_NOMARK;          // Цвет выделения объекта на карте
    parobj.Incode = incode;
    parobj.Excode = pointcode;          // Внешний код объекта
    parobj.Local  = IMGL_POINT;          // Локализация объекта
    // Показать выбранный символ
    ret = imgPrepareImage(pdata, func,(char *)&parobj, context, test, border);
    // Подписать выбранный символ
    if ((sounding)&&(valsouFlag == 1))
      {
        POLYDATAEX data;
        DRAWPOINT points[1];
        int number;                 // 26/03/12
        double scale;
        data = *pdata;
         // Относительный масштаб отображения
         scale = context->Scale;
         if (scale < 0.5) scale = 0.5;


        // Настраиваем адреса метрики
        data.Points     = points;
        data.PolyCounts = &number;
        number          = 1;
        data.Count      = 1;
        char txt[64];
        char * ptxt[1];
        IMGTRUETEXT trtext;
        sprintf(txt,"%.1f", valsou);
        memset((char *)&trtext,0,sizeof(IMGTRUETEXT));
        points[0].x = pdata->Points[0].x;
        points[0].y = pdata->Points[0].y;
        // Сдвиг по X в мм 3
        points[0].x += scale * context->PixelX * 3000;
        strcpy(trtext.FontName,"Microsoft Sans Serif");
        trtext.Text.BkgndColor = IMGC_TRANSPARENT;
        trtext.Text.ShadowColor = IMGC_TRANSPARENT;
        trtext.Text.Height = 2000;
        trtext.Text.Weight = FW_THIN;
        trtext.Text.Wide   = UNIW_NORMAL;
        trtext.Text.Align  = FA_LEFT|FA_MIDDLE;

         trtext.Text.CharSet = RUSSIAN_CHARSET;
         data.Text = ptxt;
         ptxt[0] = txt;
         ret |= imgPrepareImage(&data, IMG_TRUETEXT, (const char *)&trtext,
                                      context, test, border);
      }
    // Показать знак низкой точности
    if (quapntFlag)
       {
         // рисование знака низкой точности - уточним потом
       }
    return ret;
   }   // обработали точечный объект

 if (local == 0)  // линейные объекты
   {
    // Выбрать тип наносимого объекта
     pointcode = udwhazCode;

    // Выбрать тип линии
    // Отображать - не отображать
    // Запрос кода отображаемого символа и флага отображения низкой точности QUAPNT02
    quapntFlag = Quapnt(&quapntCode,(SEMANTIC *)(pdata->Semantic));
     // Тип функции
    func = IMG_OBJECT;
    IMGOBJECT parobj;
    parobj.Local  = IMGL_LINE;          // Локализация объекта
    if (quapntFlag)
      {
        if (udwhazFlag)
          {  // LOWACC41
            parobj.Incode = SeekServObjectIncode(LOWACC41);
            parobj.Excode = LOWACC41;          // Внешний код объекта

          }
        else
          {   // LOWACC31
            parobj.Incode = SeekServObjectIncode(LOWACC31);
            parobj.Excode = LOWACC31;          // Внешний код объекта
          }
        ret |= imgPrepareImage(pdata, IMG_OBJECT,(char *)&parobj, context,
                            test, border);
      }


    IMGDOT imgdot;
    imgdot.Color = CHBLK;
    imgdot.Thick = 2 * 260;         // 2 пиксела (ширина линии)
    imgdot.Blank = 1200;
    imgdot.Dash =   600;
    if (udwhazFlag)
      {
        ret |= imgPrepareImage(pdata, IMG_DOT, (const char *)&imgdot,
                                      context, test, border);
      }
                                      //depthValue  = valsou;

    if (depthValueFlag == 0)  // нет глубины
      {
        ret |= imgPrepareImage(pdata, IMG_DOT, (const char *)&imgdot,
                                      context, test, border);
      }
    else
      {
        if (depthValue <= 20.0)
          {
            ret |= imgPrepareImage(pdata, IMG_DOT, (const char *)&imgdot,
                                      context, test, border);
          }
        else
          {
             imgdot.Blank = 3600;
             imgdot.Dash =  1800;
             ret |= imgPrepareImage(pdata, IMG_DOT, (const char *)&imgdot,
                                      context, test, border);
          }

      }

  DRAWPOINT textm[2];
  POLYDATAEX data;
  int  number;           // 26/03/12
   // Настраиваем адреса метрики
   data.Points     = &textm[0];
   data.PolyCounts = &number;
   number          = 2;
   data.Count      = 1;


  // Поиск координат середины линии
  // координаты возвращает в point.
  // При ошибке возвращает 0
  CheckMiddleLinePoint(&textm[0],pdata->Points,*(pdata->PolyCounts));
  if (udwhazFlag)
    {
      parobj.Incode = SeekServObjectIncode(udwhazCode);
      parobj.Excode = udwhazCode;          // Внешний код объекта
      parobj.Local  = IMGL_POINT;          // Локализация объекта
      return imgPrepareImage(&data, IMG_OBJECT,(char *)&parobj, context, test, border);


    }
  else
    {
      if (depthValueFlag != 0)
        { if (depthValue <= 20.0)
            {
             SoundFrm(depthValue,textm,(SEMANTIC *)(pdata->Semantic),
                 pdata->MapRsc,
                      context, test, border);
            }
         }
    }

  }

 /*
 // Площадные
  // Запросим значение семантики DRVAL2
  semanticnumber = 88;
  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,semanticnumber ,
                             1, &drval2) == 0)
      {
        drval2 = drval1 + 0.01;     // нет значения семантики,
      }


  // Установить цвет и возможную заливку площадей
  ColorSeabed(drval1,drval2,&defcolor,&shallow);
  IMGSQUARE square;
  square.Color = defcolor;
  // Тип функции
  func = IMG_SQUARE;


  ret = imgPrepareImage(pdata, func, (char *)&square, context, test, border);

    // Если пользователь хочет штриховку - по правилам DIAMOND1 - можно
  // штрихованный площадной (45 и 135)
  if (shallow == 1)
    {
     long scparm[6];
     scparm[0] = sizeof(scparm);
     scparm[1] = 45;
     scparm[2] = 9000;
     scparm[3] = IMG_LINE;
     scparm[4] = DEPCN;
     scparm[5] = 1;

     // Тип функции
     func = IMG_HATCHSQUARE;

     ret = imgPrepareImage(pdata, func, (char *)&scparm[0], context, test, border);
     scparm[1] = 135;
     ret = imgPrepareImage(pdata, func, (char *)&scparm[0], context, test, border);
   }

 // Если объект DRGARE - углубленный район
 if (code == 46)
  {
    // Нарисовать заливку DRGARE01


    // Нарисовать контур
    // (DASH,1.CHGRF)

    // Выбрать знак по ограничениям
    RestrnObject(pdata, draw,context,test,border);
  }

 */
  return ret;
}


                                                  // 31/10/07
// ------------------------------------------------------------------------
// Уточнение значений глубины по семантике  DEPVAL02
// Значение флагов: 1 - есть семантика, 0 - нет
// ------------------------------------------------------------------------
int Depval(double* least,double * seabed, long * leastflag,
                     long * seabedflag,SEMANTIC * sem )
{
  *leastflag = 0;
  *seabedflag  = 0;
  long watlev;
  long expsou;
  if (sem == 0) return 0;

  // Запросим значение вспомогательной семантики LEAST_DEPTH  32697
  if (imlGetSemanticDouble(sem ,32697 ,1, least) == 0)
      { // нет семантики для уточнения
        *leastflag = 0;
        *seabedflag = 0;
        return 0;
      }
  *leastflag = 1;
  // запросим значение семантики WATLEV - 187
  if (imlGetSemanticLong(sem , 187, 1, &watlev) == 0)
   {
     watlev = -1;
   }

  // запросим значение семантики EXPSOU - 93
 if (imlGetSemanticLong(sem , 93, 1, &expsou) == 0)
   {
     expsou = -1;
   }

  if ((watlev == 3)&&((expsou == 1)||( expsou == 3)))
   {
     *seabed = *least;
     *seabedflag = 1;
   }
 else
   {
     *seabed = *least;
     *seabedflag = 1;
     *leastflag = 0;
   }
 return 1;
}

                                                  // 31/10/07
// ------------------------------------------------------------------------
// Запрос кода отображаемого символа и флага отображения UDWHAZ04
// ------------------------------------------------------------------------
long Udwhaz(double depthValue, long * udwhazCode,long watlev,SEMANTIC * sem)
{
  long danger = 0;
  double least;
  if (depthValue <= SAFETY_CONTOUR) return 0;
  // Запросим значение вспомогательной семантики LEAST_DEPTH  32697
  if (imlGetSemanticDouble(sem ,32697 ,1, &least) == 0)
      { // нет семантики для уточнения
        danger = 0;
      }
  else
      {
        if (least >= SAFETY_CONTOUR)
          danger = 1;      // изолированная опасность
      }
  if (danger == 1)
      {
        if ((watlev == 1)||(watlev == 2))
         return 0;
        else
          {
            *udwhazCode = ISODGR01;
            return 1;
          }
      }

 if (SHOW_ISDANGER_SHALLOW == 0)
      return 0;
 // Запросим значение вспомогательной семантики  MIN_DEPVAL1 32696
  if (imlGetSemanticDouble(sem ,32696 ,1, &least) == 0)
      return 0;
  if ((least >= 0)||((least <= SAFETY_CONTOUR)))
    {
      if ((watlev == 1)||(watlev == 2))
         return 0;
        else
          {
            *udwhazCode = ISODGR01;
            return 1;
          }
    }
  else
    {
     return 0;
    }
}


                                                  // 31/10/07
// ------------------------------------------------------------------------------
// Запрос кода отображаемого символа и флага отображения низкой точности QUAPNT02
// ------------------------------------------------------------------------------
long Quapnt(long * quapntCode,SEMANTIC * sem)
{
  long quapos;
   // запросим значение семантики 402 QUAPOS - точность измерений
  if (imlGetSemanticLong(sem , 402, 1, &quapos) == 0)
   {
     quapos = 1;      // нет семантики - точность хорошая
   }
  if (( quapos > 1)&&(quapos<=9)) // плохая точность
   {
      quapos = 0;
   }

  if (quapos == 1)
    {
      *quapntCode = 0;
      return 0;
    }
  *quapntCode = LOWACCO1;
  return 1;
}

// ------------------------------------------------------------------------------
// Линейные объекты низкой точности
// ------------------------------------------------------------------------------
int _fastcall QualinLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  long local;
  long code;
  long flag;
  long quapntCode;

   // Заполним массив внутренних кодов служебных семантик
  FillServObject(pdata->MapRsc);

  // параметр - локализация объекта
  if (imlDrawType(draw, 1) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 1);
     if (cparm)
       local = *cparm;
     if (local != 0) return 0;
   }

  code = 0;
  // параметр - код объекта
  if (imlDrawType(draw, 2) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 2);
     if (cparm)
       code = *cparm;
   }


  flag = Quapnt(&quapntCode,(SEMANTIC *)(pdata->Semantic));

  IMGOBJECT parobj;
  parobj.Color = IMGC_NOMARK;          // Цвет выделения объекта на карте

  if (flag == 1)
    {
      parobj.Incode = SeekServObjectIncode(quapntCode);
      parobj.Excode = quapntCode;          // Внешний код объекта
      parobj.Local  = IMGL_LINE;          // Локализация объекта
      return imgPrepareImage(pdata, IMG_OBJECT,(char *)&parobj, context, test, border);
    }

  IMGLINE line;
  line.Color = CSTLN;
  line.Thick = 1*260;  // толщина 1 piх

  long conrad;
  if (code == 30) //  Береговая линия
    {
       // запросим значение семантики CONRAD - 82 ПРИМЕТНЫЙ,РАДАРОМ
       if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 82, 1,
                                   &conrad) == 0)
         {
            imgPrepareImage(pdata, IMG_LINE, (char *)&line, context, test, border);
         }
       else
         {
            if (conrad == 1)
              {
                line.Color = CHMGF;
                line.Thick = 3*260;  // толщина 3 piх
                imgPrepareImage(pdata, IMG_LINE, (char *)&line, context, test, border);
                line.Color = CSTLN;
                line.Thick = 1*260;  // толщина 1 piх
                imgPrepareImage(pdata, IMG_LINE, (char *)&line, context, test, border);
              }
            else
              {
                imgPrepareImage(pdata, IMG_LINE, (char *)&line, context, test, border);
              }
         }

    }
  if (code != 71) return 0;     // берег
  imgPrepareImage(pdata, IMG_LINE, (char *)&line, context, test, border);

  return 1;
}


// ------------------------------------------------------------------------------
// Запрос числовой семантики с проверкой на отсутствие
// ------------------------------------------------------------------------------
long CheckDoubleSemasntic(SEMANTIC * sem, long semanticnumber,long number,
             double * val)
{
  char strvs[256];
  memset(strvs,0,sizeof(strvs));
  imlGetSemanticString(sem,semanticnumber,number,strvs,sizeof(strvs)-1);
  if (strvs[0]== 0)
    return 0;
  else
    return imlGetSemanticDouble(sem ,semanticnumber ,number,val);

}

// ------------------------------------------------------------------------------
// Поиск координат середины линии
// координаты возвращает в point(1 - точка - середина, вторая по направлению линии)
// При ошибке возвращает 0
// ------------------------------------------------------------------------------
long CheckMiddleLinePoint(DRAWPOINT * point,DRAWPOINT *points,long number)
{

  double part;
  double l = 0;
  double endl = 0;
  long k = 0;
  long i= 0;
  for (int j = 0; j < number; j++)  // поиск центрального отрезка
    {
      if (l <= endl)
        { // прибавляем отрезок от начала метрики
         part = ((points+i+1)->x - (points+i)->x)*
                ((points+i+1)->x - (points+i)->x);
         part+= ((points+i+1)->y - (points+i)->y)*
                ((points+i+1)->y - (points+i)->y);
         if (part > 0) part = sqrt(part);
         l+=part;
         i++;
        }
       else
        {
         part = ((points+number-k-1)->x - (points+number-k-2)->x)*
                ((points+number-k-1)->x - (points+number-k-2)->x);
         part+= ((points+number-k-1)->y - (points+number-k-2)->y)*
                ((points+number-k-1)->y - (points+number-k-2)->y);
         if (part > 0) part = sqrt(part);
         k++;
         endl+=part;
        }
       if (i+k >= number) break; 
    }

  // рисуем в центре отрезка k - k+1
  if (k+1 < number)
    {
      point->x = ((points+k+1)->x + (points+k)->x)/2;
      point->y = ((points+k+1)->y + (points+k)->y)/2;
      (point+1)->x = (points+k+1)->x;
      (point+1)->y = (points+k+1)->y;
    }
  else
    {
      if (k > 0)
        {
         point->x = ((points+k)->x + (points+k-1)->x)/2;
         point->y = ((points+k)->y + (points+k-1)->y)/2;
         (point+1)->x = (points+k)->x;
         (point+1)->y = (points+k)->y;
        }
      else
        {
         point->x = ((points+1)->x + (points)->x)/2;
         point->y = ((points+1)->y + (points)->y)/2;
         (point+1)->x = (points+1)->x;
         (point+1)->y = (points+1)->y;
        }
    }

 long len = (point->x -(point+1)->x)*(point->x -(point+1)->x) +
          (point->y -(point+1)->y)*(point->y -(point+1)->y);
 if (len < 10) // слишком маленькая база
   {
     (point+1)->x = point->x + ((point+1)->x - point->x)*2;
     (point+1)->y = point->y + ((point+1)->y - point->y)*2;
   }
 return 1;
}

// ------------------------------------------------------------------------------
// Границы глубин - дополнение а процедуре DEPARE
// ------------------------------------------------------------------------------
int _fastcall DepContourObject(const POLYDATAEX* pdata, const IMGDRAW* draw,   // 19/01/09
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  long         code;
  int          func;
  long         semanticnumber;
  double       drval1,drval2;
  long         ret;
  long neig1 = 0;
  long neig2 = 0;
  long safe = 0;
  long unsafe = 0;
  long quapos = 0;
  long loc_safety = 0;
  double loc_valdco = 0;
 
  if (pdata == 0) return 0;

  code = 0;
  // лучше не цвет, а код объекта
  if (imlDrawType(draw, 1) == IMG_LONG)               // 20/02/02
   {
     int * cparm = (int *)imlDrawParm(draw, 1);       // 29/04/13
     if (cparm)
       code = *cparm;
   }
  if (code != 10190) // вспомогательный объект
    return 0;
  semanticnumber = 32697;     // максимальная глубина соседа

  // Запросим значение семантики
  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,semanticnumber ,
                             1, &drval1) == 0)
      {
        drval1 = -1.0;     // нет значения семантики,
      }

  semanticnumber = 32696;     // минимальная глубина соседа
  // Запросим значение семантики
  semanticnumber = 88;

  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,semanticnumber ,
                             1, &drval2) == 0)
      {
        drval2 = drval1 + 0.01;     // нет значения семантики,
      }

  // запросим значение семантики 32693 - номер кода соседа
  if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 32693, 1, &neig1) == 0)
   {
     neig1 = 0;
   }
  if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 32693, 1, &neig2) == 0)
   {
     neig2 = 0;
   }

  if (drval1 < SAFETY_CONTOUR)
       unsafe = 1;
  else
      safe = 1;
  // Выберем изобаты

  // запросим значение семантики 174 VALDCO - значение изобаты
  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) , 174, 1, &loc_valdco) == 0)
   {
     loc_valdco = -1.0;
   }

  // оба соседа- глубины?
  if ((neig2 == 42)&&(neig1 == 42))
    {
      if (loc_valdco == -1.0) // не задано
          loc_valdco = 0.0;
    }
   else
    {
       loc_valdco = -1.0; // неизвестно
    }

  if (loc_valdco == SAFETY_CONTOUR)
    {
      loc_safety = 1;
    }
  else  // разные соседи - (глубины и драггированные области
    {
      if (((neig2 == 42)||(neig2 == 48))&&
         ((neig1 == 42)||(neig1 == 48)))
        {
          if (drval2 < 0) // нет значения семантики
            {
              drval2 = -1;
            }

          if (drval2 < SAFETY_CONTOUR)
            {
              unsafe = 1;
            }
          else
            {
              safe = 1;
            }
        }
      else  // остальные
        {
          if ((neig1 == 71)||(neig2 == 71)||(neig1 == 154)||(neig2 == 154))
            {
               unsafe = 1;

            }
        }
    }

  if (loc_safety == 0 )
    {
      if ((unsafe == 1)&&(safe == 1))
        {
          loc_safety = 1;
        }
    }

  // запросим значение семантики 402 QUAPOS - точность измерений
  if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 402, 1, &quapos) == 0)
   {
     quapos = 1;
   }

  func = IMG_DOT;
  IMGDOT dot;
  dot.Color = DEPSC;
  dot.Thick = 2*260;  // толщина 2 piх

 if (loc_safety == 1)      // рисуем
    {
      if (( quapos > 1)&&(quapos<=9)) // плохоя точность
        {  // пунктир
          dot.Blank = 1800;
          dot. Dash = 3600;
          ret = imgPrepareImage(pdata, func, (char *)&dot, context, test, border);
          if ((ret != 0) && (test == PREPARE_TEST))
            {
             return 1;
            }
        }
      else
        {  // линия
          func = IMG_LINE;

          ret = imgPrepareImage(pdata, func, (char *)&dot, context, test, border);
          if ((ret != 0) && (test == PREPARE_TEST))
           {
             return 1;
           }
        }
    }
  else
    {
      func = IMG_EMPTY;
      ret = imgPrepareImage(pdata, func, (char *)&dot, context, test, border);
    }
  return ret;
}


// ------------------------------------------------------------------------------
// Изобаты
// ------------------------------------------------------------------------------
int _fastcall DepCurvObject(const POLYDATAEX* pdata, const IMGDRAW* draw,   // 19/01/09
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  long quapos;
  long func;
  long ret = 0;           // 25/05/11
  IMGDOT dot;
  double loc_valdco;
  DRAWPOINT textm[2];
  dot.Color = DEPCN;
  dot.Thick = 1*260;  // толщина 2 piх
  dot.Blank = 1800;
  dot. Dash = 3600;

  // запросим значение семантики 402 QUAPOS - точность измерений
  if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 402, 1, &quapos) == 0)
   {
     func = IMG_LINE;
   }
  else
   {
     if (( quapos > 1)&&(quapos<=9)) // плохоя точность
      {
        func = IMG_DOT;
      }
     else
      {
        func = IMG_LINE;
      }
   }

  ret = imgPrepareImage(pdata, func, (char *)&dot, context, test, border);
  if ((ret != 0) && (test == PREPARE_TEST))
    {
      return 1;
    }

  // Если нужно подпишем горизонталь

  // запросим значение семантики 174 VALDCO - значение изобаты
  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) , 174, 1, &loc_valdco) == 0)
   {
     loc_valdco = -1.0;
   }

  if ((loc_valdco < 0)||(loc_valdco > 99999.0))
       return 1;  // такие горизонтали не подписываем

  // Поиск координат середины линии
  // координаты возвращает в point.
  // При ошибке возвращает 0

  CheckMiddleLinePoint(&textm[0],pdata->Points,*(pdata->PolyCounts));

  ret |= DepthText(SNDG2,DEPDW,loc_valdco,textm,context, test, border);

  return ret;
}


// ------------------------------------------------------------------------------
// Знаки и цвет подписи отметок глубин
// ------------------------------------------------------------------------------
long SoundFrm(double depth,DRAWPOINT * point,SEMANTIC * sem,HRSC rsc,
               const IMGCONTEXT * context, int test,FRAME * border)
{
  long color;
  long texsou;
  long code;
  long quasou;
  long status;
  long quapos;
  long ret = 0;

   POLYDATAEX data;
   int  number;                            // 26/03/12
   // Настраиваем адреса метрики
   data.Points     = point;
   data.PolyCounts = &number;
   number          = 2;
   data.Count      = 1;
   data.MapRsc     = rsc;

  IMGOBJECT parobj;
  parobj.Color = IMGC_NOMARK;          // Цвет выделения объекта на карте


  if (depth <= SAFETY_CONTOUR)
    {
     color = SNDG2;
     code  = SOUNDSB1;
    }
  else
    {
      color = SNDG1;
      code =  SOUNDGB1;
    }

  // TECSOU 156
  if (imlGetSemanticLong(sem , 156, 1, &texsou) == 0)
    {
      texsou = 0;
    }

  if (texsou == 6)
    {
      parobj.Incode = SeekServObjectIncode(code);
      parobj.Excode = code;          // Внешний код объекта
      parobj.Local  = IMGL_POINT;    // Локализация объекта
      ret = imgPrepareImage(&data, IMG_OBJECT,(char *)&parobj, context, test, border);
    }

  // quasou  125
  if (imlGetSemanticLong(sem , 125, 1, &quasou) == 0)
    {
      quasou = 0;
    }
  if (quasou == 32000)  quasou = 0;

  // status  149
  if (imlGetSemanticLong(sem , 149, 1, &status) == 0)
    {
      status = 0;
    }
  if (status == 32000)  status = 0;

  if ((quasou == 3)||(quasou == 4)||(quasou == 5)||(quasou == 8)||(quasou == 9)||
         (status == 18))
    {
      parobj.Incode = SeekServObjectIncode(SOUNDSC2);
      parobj.Excode = SOUNDSC2;          // Внешний код объекта
      parobj.Local  = IMGL_POINT;        // Локализация объекта
      ret |= imgPrepareImage(&data, IMG_OBJECT,(char *)&parobj, context, test, border);
    }
  else
    {
        // quapos  402
      if (imlGetSemanticLong(sem , 402, 1, &quapos) == 0)
        {
          quapos = 0;
        }
      if (quapos == 32000)  quapos = 0;
      if ((quapos > 1)&&(quapos <= 9))
        {
         parobj.Incode = SeekServObjectIncode(SOUNDSC2);
         parobj.Excode = SOUNDSC2;          // Внешний код объекта
         parobj.Local  = IMGL_POINT;       // Локализация объекта
         ret |= imgPrepareImage(&data, IMG_OBJECT,(char *)&parobj, context, test, border);
       }
    }

  ret |= DepthText(color,IMGC_TRANSPARENT,depth,point,context, test, border);
  return ret;
}

// ------------------------------------------------------------------------------
// Подписи горизонталей и отметок глубин
// foncolor = IMGC_TRANSPARENT, если не нужен контур шрифта
// ------------------------------------------------------------------------------
int _fastcall DepthText(long color,long foncolor,double depth,DRAWPOINT * point,
                        const IMGCONTEXT * context, int test,FRAME * border)
{
 char txt[64];
 char * ptxt[1];
 POLYDATAEX data;
 int  number;               // 26/03/12
 long h1;
 DRAWPOINT pointf[2];    // отрезок симметричный полученному, для рисования целой части
 pointf[1].x = point->x;
 pointf[1].y = point->y;
 pointf[0].x = 2*point->x - (point+1)->x;
 pointf[0].y = 2*point->y - (point+1)->y;

 // Настраиваем адреса метрики
 data.Points     = &pointf[0];
 data.PolyCounts = &number;
 number          = 2;
 data.Count      = 1;

 IMGTRUETEXT trtext;
 memset((char *)&trtext,0,sizeof(IMGTRUETEXT));

 long h = depth;  // оставили только целые

 // Выделим целую часть
 // выводим текст на экран
 sprintf(txt,"%i",h);
 strcpy(trtext.FontName,"Arial");  // 25/05/11 ""Microsoft Sans Serif");
 trtext.Text.Color = SNDG2;
 trtext.Text.BkgndColor = IMGC_TRANSPARENT;
 if (foncolor!= IMGC_TRANSPARENT)
   {
      trtext.Text.ShadowColor = DEPDW;
      trtext.Text.Outline = 1;
   }
 else
   {
     trtext.Text.ShadowColor = IMGC_TRANSPARENT;
     trtext.Text.Outline = 0;
   }

 trtext.Text.Height = 3000;                 // 29/01/13 2300;
 trtext.Text.Weight = FW_THIN;
 trtext.Text.Wide   = UNIW_NORMAL;
 trtext.Text.Align  = FA_RIGHT|FA_MIDDLE;
 trtext.Text.Horizontal = 0;

 trtext.Text.Outline = 1;

#ifdef WIN32API                          // 26/03/12
 trtext.Text.CharSet = DEFAULT_CHARSET;  // RUSSIAN_CHARSET;
#endif
 data.Text = ptxt;
 ptxt[0] = txt;

 int ret = imgPrepareImage(&data, IMG_TRUETEXT, (const char *)&trtext,
                                       context, test, border);
 if ((ret != 0) && (test == PREPARE_TEST))
   {
     return 1;
   }

// data.Points     = point;
 if (depth < 31.0)
   { // проверяем, есть ли дробная часть
     h = depth;

     h1 = (long)(depth*10.0 + 0.0001);

     h1 = h1 - h*10;
     if (h1 == 0)
       {
         return ret;                      // 25/05/11
       }

     if (h1 < 0) h1 = -h1;

     sprintf(txt,"%i",h1);
     int Length = context->Scale * context->PixelX * 10000;

     float scale = context->Scale;
     if (scale > 1) scale = 1;
     int delta = scale * context->PixelX * trtext.Text.Height / 3;
     pointf[0].x = point->x;
     pointf[0].y = point->y - delta;
     pointf[1].x = (point+1)->x;
     pointf[1].y = (point+1)->y - delta;

     // Шрифт дробной части немного меньше 
     trtext.Text.Height = trtext.Text.Height * 3 / 4;         // 29/01/13

     // Немного поднять текст дробной части 
     trtext.Text.Align  = FA_LEFT|FA_TOP;

     ret |= imgPrepareImage(&data, IMG_TRUETEXT, (const char *)&trtext,
                                       context, test, border);
   }

 return ret;
}

// ------------------------------------------------------------------------------
// Построение векторных с учетом национального языка
// ------------------------------------------------------------------------------
int _fastcall NationalVectObject(const POLYDATAEX* pdata, const IMGDRAW* draw,   // 19/01/09
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
 long vecfunc;
 const char * vecparm;
 long ret;

 if (pdata == 0) return 0;

 // Тип функции
 vecfunc = imlDrawType(draw, 1);

 // Параметры функции
 vecparm = imlDrawParm(draw, 1);
 char addr[2*1024];
 long flagleng = 0;
 char semstr[256];

 if (IML_LANGUAGE == 1)   // нужна информация на нац.языке
   {
     if (vecfunc == IMG_VECTOREX)
       { // если есть семантика с национальным языком
         if (imlGetSemanticString((SEMANTIC *)(pdata->Semantic), 301, 1,
                                        semstr,sizeof(semstr)-1)!= 0)
            {
             IMGVECTOREX * vector = (IMGVECTOREX *)vecparm;
             if ((vector->Length >= sizeof(IMGVECTOREX))
                   &&(vector->Length < sizeof(addr)))
               {
                 memmove(addr,vecparm,vector->Length);
                 flagleng = ChangeText((IMGVECTOREX *)addr);

               }
            }
       }

   }

 // Параметры функции
 if (flagleng != 0)
   {
     vecparm = addr;
   }

 ret = imgPrepareImage(pdata, vecfunc, vecparm, context, test, border);

 return ret;

}

// ------------------------------------------------------------------------------
// Замена кода семантики с 116 на 301 в параметрах текста векторного объекта
// Если замена была - возвращает 1
// ------------------------------------------------------------------------------
long ChangeText(IMGVECTOREX * vector)
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
            if (vtext->Code == 116)
              {
                vtext->Code = 301;
                flagleng = 1;
              }
          }
         long delta = desc->Length - 4;
         // Размер фрагмента
         int size = sizeof(IMGPOLYDESC) +
            (*(int *)((char *)(&(desc->Count))+delta)- 1) *     // 29/04/13
                    sizeof(IMGVECTPOINT) + delta;
         desc = (IMGPOLYDESC *)((char *)desc + size);
     }

  return flagleng;
}

// ------------------------------------------------------------------------
// Изображение береговых конструкций
// ------------------------------------------------------------------------
int _fastcall ShorelineConstractions(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  int local;
  int quapntFlag;
  int quapntIncode;
  long quapntCode;
  char * param;
  long watlev;
  long condtn;
  long catslc;
  long ret = 0;
  if (pdata == 0) return 0;

  // Заполним массив внутренних кодов служебных семантик
  FillServObject(pdata->MapRsc);

  local = 2;
  // параметр - локализация объекта
  if (imlDrawType(draw, 1) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 1);
     if (cparm)
       local = *cparm;
     if ((local > 2)||(local < 0)) local = 2; // При любых неприятностях -
                                              // считаем,объект точечный
   }

  quapntFlag = Quapnt(&quapntCode,(SEMANTIC *)(pdata->Semantic));
  if (local == 2)      // точечные
   {
     if (quapntFlag) // найдем
       {
         quapntIncode = SeekServObjectIncode(quapntCode);
         // Тип функции
         long func = IMG_OBJECT;
         IMGOBJECT parobj;
         parobj.Color = IMGC_NOMARK;          // Цвет выделения объекта на карте
         parobj.Incode = quapntIncode;
         parobj.Excode = quapntCode;          // Внешний код объекта
         parobj.Local  = IMGL_POINT;          // Локализация объекта

         // Параметры функции
         return imgPrepareImage(pdata, func,(char *)&parobj, context,
                                           test, border);
       }

       param = (char *)imlDrawParm(draw, 2);
       if (param != 0)
       return imgPrepareImage(pdata, IMG_DRAW, param, context, test, border);
   }

   // запросим значение семантики WATLEV - 187
   if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 187, 1,
                                                &watlev) == 0) watlev = -1;
   // запросим значение семантики CONDTN - 81 - состояние
   if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 81, 1,
                                                &condtn) == 0) condtn = -1;
   // запросим значение семантики CATCLS - 60  категория берегового сооружения
   if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 60, 1,
                                                &catslc) == 0) catslc = -1;

   // Если значения назначены в конверторе по умолчанию
   if (watlev == 32000) (watlev = -1);
   if (condtn == 32000) (condtn = -1);
   if (catslc == 32000) (catslc = -1);


  IMGDOT dot;
  dot.Color = CSTLN;
  dot.Thick = 2*260;  // толщина 2 piх
  dot.Blank = 1800;
  dot. Dash = 3600;

  if (local < 2 )      // линейные и площадные
   {
      if (quapntFlag != 0) // рисуем специальную линию
        {
          // LOWACC21
          return 1;    // пока
        }
      if ((watlev == -1)&&(condtn == -1)&&(catslc == -1))
         return imgPrepareImage(pdata, IMG_LINE, (char *)&dot, context, test, border);

      if ((condtn == 1)||(condtn == 2))
        {
           dot.Thick = 1*260;  // толщина 1 piх
           ret |= imgPrepareImage(pdata, IMG_DOT, (char *)&dot, context, test, border);
        }
      if ((catslc == 6)||(catslc == 15)||(catslc == 16))
        {
           dot.Thick = 4*260;  // толщина 1 piх
           ret |= imgPrepareImage(pdata, IMG_LINE, (char *)&dot, context, test, border);
        }
      if (watlev == 2)
        {
           dot.Thick = 2*260;  // толщина 2 piх
           ret |= imgPrepareImage(pdata, IMG_LINE, (char *)&dot, context, test, border);
        }
      else
        {

          if ((watlev == 3)&&(watlev == 4))
            {
              dot.Thick = 2*260;
              ret |= imgPrepareImage(pdata, IMG_DOT, (char *)&dot, context, test, border);
            }
         }
       return ret;
    }

  return ret;
};


// ------------------------------------------------------------------------
// Изображение остатков кораблекрушения
// ------------------------------------------------------------------------
int _fastcall Wrecks(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  long local;
  long ret;
  long catwrk;
  long quapntIncode;
  long udwhazCode;
  long udwhazFlag;
  long quapntFlag;
  long incode;
  double depthValue;
  long depthValueFlag;
  double seabedDepth;
  long seabedDepthFlag;
  long code;
  long func;

  long quapntCode = 0;
  long watlev = -1;                              // 03/04/09
    // Заполним массив внутренних кодов служебных семантик
  FillServObject(pdata->MapRsc);

  local = 2;
  // параметр - локализация объекта
  if (imlDrawType(draw, 1) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 1);
     if (cparm)
       local = *cparm;
     if ((local > 2)||(local < 0)) local = 2; // При любых неприятностях -
                                              // считаем,объект точечный
   }

  code = 0;
  // параметр - код объекта
  if (imlDrawType(draw, 2) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 2);
     if (cparm)
       code = *cparm;
   }

  if (code != 159) return 0; // только для остатков кораблекрушения

  // Запросим значение семантики VALSOU 179 Значение глубины

  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,179 ,
                             1, &depthValue) == 0)
      {
        depthValue = 12000.0;   // нет значения семантики,
      }

   if ( depthValue == 12000.0)   // нет значения семантики,
      {                          // уточняем глубину
        // Уточнение значений глубины по семантике  DEPVAL02
        ret = Depval(&depthValue,&seabedDepth, &depthValueFlag,
                     &seabedDepthFlag,(SEMANTIC *)(pdata->Semantic));
        if (depthValueFlag == 1)   // взяли семантику
          {
            depthValue = depthValue;
          }
        else
          {
            // запросим значение семантики CATWRK - 71 Категория затонувшего судна
            if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 71, 1,
                                   &catwrk) == 0)
               {
                 // запросим значение семантики WATLEV - 187
                 if (imlGetSemanticLong((SEMANTIC *)(pdata->Semantic) , 187, 1,
                                                 &watlev) == 0) watlev = -1;

                 if ((watlev == 5)||(watlev == 3)) depthValue = 0.0;
                 else
                   {
                      depthValue = -15.0;
                   }
               }
             else
               {
                 if (catwrk == 1) // неопасное препятствие
                   {
                      depthValue = 20.1;
                      if ( seabedDepthFlag == 1) // есть значения семантики
                        {  // по схеме противоречие - осталось непонятно
                           if (seabedDepth > 20.1)
                               depthValue = seabedDepth;

                        }
                   }
                 else
                   {
                     depthValue = -15.0;
                   }
               }
           }
          depthValueFlag = 1; // уточнили значение семантики
      }
   // Запрос кода отображаемого символа и флага отображения UDWHAZ04
   udwhazFlag = Udwhaz(depthValue,&udwhazCode,watlev,(SEMANTIC *)(pdata->Semantic));
   // Запрос кода отображаемого символа и флага отображения низкой точности QUAPNT02
   quapntFlag = Quapnt(&quapntCode,(SEMANTIC *)(pdata->Semantic));
   quapntIncode = 0;
   if (quapntFlag) // найдем
     {
       quapntIncode = SeekServObjectIncode(quapntCode);
     }

   if (local == 2)  // точечные объекты
     {
       // Тип функции
       func = IMG_OBJECT;
       IMGOBJECT parobj;
       parobj.Color = IMGC_NOMARK;          // Цвет выделения объекта на карте
       parobj.Incode = 0;
       parobj.Excode = 0;                   // Внешний код объекта
       parobj.Local  = IMGL_POINT;          // Локализация объекта

       if (udwhazFlag)  // Отображать Изолированную опасность
         {
           // Запросим параметры выбранного объекта
           incode = SeekServObjectIncode(udwhazCode);

           parobj.Incode = incode;
           parobj.Excode = udwhazCode;          // Внешний код объекта

           // Параметры функции
           ret = imgPrepareImage(pdata, func,(char *)&parobj, context,
                                           test, border);
           if (quapntFlag)
            {
              // Запросим параметры выбранного объекта
              parobj.Excode = quapntCode;          // Внешний код объекта
              parobj.Incode = quapntIncode;
              ret |= imgPrepareImage(pdata, func,(char *)&parobj, context, test, border);
            }
           return ret;
         }
       // Далее рисуем выбранные точечные  (продолжение А)
       if ( depthValue == 12000.0)  // нет значения семантики VALSOU
         { // подберем подходящий символ
           // учтено, что семантики могут повторяться - разборка по важности значений
           udwhazCode = WRECKS05;
           if ((catwrk == 1) &&(watlev == 3))
             {
               udwhazCode = WRECKS04;
             }
           else
             {
               if ((catwrk == 2) &&(watlev == 3))
                 {
                   udwhazCode = WRECKS05;
                 }
               else
                 {
                    if (catwrk == 4)
                      {
                        udwhazCode = WRECKS01;
                      }
                    else
                      {
                         if (catwrk == 5)
                           {
                             udwhazCode = WRECKS01;
                           }
                          else
                           {
                             if (watlev == 1)
                               {
                                 udwhazCode = WRECKS01;
                               }
                             else
                               {
                                 if (watlev == 2)
                                   {
                                     udwhazCode = WRECKS01;
                                   }
                                 else
                                   {
                                     if (watlev == 5)
                                       {
                                         udwhazCode = WRECKS01;
                                       }
                                     else
                                       {
                                          if (watlev == 4)
                                            {
                                              udwhazCode = WRECKS01;
                                            }
                                       }
                                   }
                               }
                           }

                      }
                 }
             }
            parobj.Incode = SeekServObjectIncode(udwhazCode);
            parobj.Excode = udwhazCode;
         }
       else
         {
           if (depthValue <= 20.0)
             {
               udwhazCode = DANGER01;
               // вывести текст  depthValue &&&&&&&
             }
           else
             {
               udwhazCode = DANGER02;
             }
           parobj.Incode = SeekServObjectIncode(udwhazCode);
           parobj.Excode = udwhazCode;          // Внешний код объекта
         }
      // Параметры функции
      ret = imgPrepareImage(pdata, func,(char *)&parobj, context,
                                           test, border);
      if (quapntFlag)   // рисуем знак низкой точности
        {
          parobj.Excode = quapntCode;          // Внешний код объекта
          parobj.Incode = quapntIncode;
          ret |= imgPrepareImage(pdata, func,(char *)&parobj, context,
                                    test, border);
        }
       return ret;
     }

   // Обработка площадных
   quapntFlag = Quapnt(&quapntCode,(SEMANTIC *)(pdata->Semantic));

   IMGDOT dot;
   dot.Color = CSTLN;
   dot.Thick = 2*260;  // толщина 2 piх
   dot.Blank = 1800;
   dot.Dash = 3600;
   func = IMG_DOT;

   if (quapntFlag == 1)
     {
        // 1 - рисуем линию LOWACC41
     }

   // Выберем подходящее значение заливки при отсутствии значения глубины
   // перенесено выше, чтобы не закрывать контур
   if (depthValueFlag == 0)
     {
       dot.Color = DEPVS;
       if (watlev == 1)
          {
            dot.Color = CHBRN;
          }
       else
          {
            if (watlev == 2)
              {
                dot.Color = CHBRN;
              }
            else
              {
                if (watlev == 4)
                  {
                    dot.Color = DEPIT;
                  }
                else
                  {
                    if (watlev == 5)
                      {
                        dot.Color = DEPVS;
                      }
                    else
                      {
                        if (watlev == 3)
                          {
                            dot.Color = DEPVS;
                          }
                      }
                  }
              }
          }
       // Нарисуем площадной объект
       ret = imgPrepareImage(pdata, IMG_SQUARE,(char *)&dot, context,
                                                       test, border);
       if ((ret != 0) && (test == PREPARE_TEST))
            {
             return 1;
            }
     }

   if (udwhazFlag == 1)
     {
       dot.Color = CHBLK;
       dot.Blank = 1200;
       dot.Dash =   600;
     }
   else
     {
       if (depthValueFlag == 1) // есть значение глубины
        {
           if (depthValue <= 20)
             {
               dot.Color = CHBLK;
               dot.Blank = 1200;
               dot.Dash =   600;
             }
           else
             {
               dot.Color = CHBLK;
             }
        }
       else
        {
         // выберем подходящий вид границы
         // По умолчанию
          func = IMG_DOT;
          dot.Color = CSTLN;
          dot.Blank = 1200;
          dot.Dash =   600;
          if (watlev == 1)
            {
              func = IMG_LINE;
            }
          else
            {
              if (watlev == 2)
                {
                  func = IMG_LINE;
                }
              else
                {
                  if (watlev == 4)
                    {
                      dot.Blank = 1800;
                      dot.Dash = 3600;
                    }
                  else
                    {
                      if (watlev == 5)
                        {
                          dot.Color = CSTLN;
                        }
                      else
                        {
                           if ((watlev == 3))
                            {
                              dot.Color = CSTLN;
                            }
                        }
                    }
                }

            }
        }
     }

   // рисуем линию выбранного типа
   ret |= imgPrepareImage(pdata, func,(char *)&dot, context, test, border);
   if ((ret != 0) && (test == PREPARE_TEST))
      {
        return 1;
      }

   long textflag;
   if (depthValueFlag == 1) // есть значение глубины
     {
       // выберем центрированный знак
       if (depthValue <= 20)
         {
           textflag = 1;
         }

     }

  // Если нужно, нарисуем центральный знак
  if ((udwhazCode != 0)||(quapntIncode != 0))
   {
      quapntIncode = SeekServObjectIncode(quapntCode);

      // рисование точечного объекта в центре площади
      DRAWPOINT  gravpoint[2];

      CentreGravity(pdata,&gravpoint[0]);
      // Получим точку входящую в объект - для центра
      NearCentreGravityPoint(pdata,&gravpoint[0]);

      // рисуем по 2 точкам
      int  count = 2;                                 // 26/03/12
      POLYDATAEX data;
      data.PolyCounts = &count;
      data.Points = &gravpoint[0];
      data.Count      = 1;
      data.Semantic = pdata->Semantic;
      data.MapRsc = pdata->MapRsc;

      data.Border.UP = gravpoint[0].Y - 32;
      data.Border.LT = gravpoint[0].X - 32;
      data.Border.DN = gravpoint[0].Y + 32;
      data.Border.RT = gravpoint[0].X + 32;

      IMGOBJECT parobj;
      parobj.Color = IMGC_NOMARK;          // Цвет выделения объекта на карте
      parobj.Incode = quapntIncode;
      parobj.Excode = quapntCode;          // Внешний код объекта
      parobj.Local  = IMGL_POINT;          // Локализация объекта

      // Тип функции
      func =IMG_OBJECT;                // 03/04/09
      ret |= imgPrepareImage(&data, func,(char *)&parobj, context, test, border);
      parobj.Incode = SeekServObjectIncode(udwhazCode);
      parobj.Excode = udwhazCode;          // Внешний код объекта
      parobj.Local  = IMGL_POINT;          // Локализация объекта

      // Тип функции
      func = IMG_OBJECT;               // 03/04/09
      ret |= imgPrepareImage(&data, func,(char *)&parobj, context, test, border);
  }
 return ret;
}

// ------------------------------------------------------------------------
// Заполним массив внутренних кодов служебных семантик
// ------------------------------------------------------------------------
long FillServObject(HRSC hRsc)
{
  long i;
  if (ListServObject[0].Incode == 0) // не заполнено - заполним
    {
      for (i = 0; i < MAXSERVOBJECT; i++)
         { // Заполним Incode по кoду объекта
           ListServObject[i].Incode = imlGetRscObjectIncode(hRsc,
                              ListServObject[i].Code,1,1);
           if (ListServObject[i].Incode == 0)   // поищем среди точечных
             {  ListServObject[i].Incode = imlGetRscObjectIncode(hRsc,
                              ListServObject[i].Code,2,1);
             }
         }
   }
  return 1;
}

// ------------------------------------------------------------------------
// Найти внутренний код служебного объекта по внешнему
// ------------------------------------------------------------------------
long SeekServObjectIncode(long excode)
{
  int i;
  for (i = 0; i < MAXSERVOBJECT; i++)
    {
      if (ListServObject[i].Code == excode)
        {
          return ListServObject[i].Incode;
        }
    }
   return 0;
}


// ------------------------------------------------------------------------
// Изображение подписей к навесному кабелю
// ------------------------------------------------------------------------
int _fastcall CbLohdText(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  double value;
  long local;
  long code;

  // Заполним массив внутренних кодов служебных семантик
  FillServObject(pdata->MapRsc);

  // параметр - локализация объекта
  if (imlDrawType(draw, 1) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 1);
     if (cparm)
       local = *cparm;
     if (local != 0) return 0;
   }

  code = 0;
  // параметр - код объекта
  if (imlDrawType(draw, 2) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 2);
     if (cparm)
       code = *cparm;
   }

  if (code != 21) return 0; // только для подписей кабелей

  // Запросим значение семантики VERCSA 184 ВЕРТИКАЛЬНЫЙ ПРОСВЕТ,БЕЗОПАСНЫЙ

  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,184 ,
                             1, &value) == 0)
    {
      value = -200.0;
    }

  IMGOBJECT parobj;
  parobj.Color = IMGC_NOMARK;          // Цвет выделения объекта на карте
  if (value > 0)  //  есть значение - печатаем знак
    {
      parobj.Incode = SeekServObjectIncode(CBLONGT1);
      parobj.Excode = CBLONGT1;          // Внешний код объекта
      parobj.Local  = IMGL_LINE;          // Локализация объекта
      return imgPrepareImage(pdata, IMG_OBJECT,(char *)&parobj, context, test, border);
    }


  // Запросим значение семантики VERCLR 181 ВЕРТИКАЛЬНЫЙ ПРОСВЕТ
  if (imlGetSemanticDouble((SEMANTIC *)(pdata->Semantic) ,181 ,
                             1, &value) == 0)
    {
      value = -200.0;
    }

  if (value > 0)  //  есть значение - печатаем знак
    {
      parobj.Incode = SeekServObjectIncode(CBLONGT2);
      parobj.Excode = CBLONGT2;          // Внешний код объекта
      parobj.Local  = IMGL_LINE;          // Локализация объекта
      return imgPrepareImage(pdata, IMG_OBJECT,(char *)&parobj, context, test, border);
    }

  return 0; // ничего не печатаем

}

// ------------------------------------------------------------------------
// Отметки глубин
// ------------------------------------------------------------------------
int _fastcall Sounds(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border)
{
  double value;
  long local;
  long code;
  DRAWPOINT point[2];

  // Заполним массив внутренних кодов служебных семантик
  FillServObject(pdata->MapRsc);

  // параметр - локализация объекта
  if (imlDrawType(draw, 1) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 1);
     if (cparm)
       local = *cparm;
     if (local != 2) return 0;
   }

  code = 0;
  // параметр - код объекта
  if (imlDrawType(draw, 2) == IMG_LONG)
   {
     int * cparm = (int *)imlDrawParm(draw, 2);
     if (cparm)
       code = *cparm;
   }

  if (code != 129) return 0; // только для отметок глубин

  value = - pdata->Height;

  point[0].x = pdata->Points[0].x;
  point[0].y = pdata->Points[0].y;

  point[1].x = pdata->Points[0].x + context->Scale * context->PixelX * 1500;
  point[1].y = pdata->Points[0].y;

  // Знаки и цвет подписи отметок глубин
  return SoundFrm(value,&point[0],(SEMANTIC *)(pdata->Semantic),pdata->MapRsc,
                  context, test, border);
}




