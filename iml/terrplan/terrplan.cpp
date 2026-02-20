/***** TERRPLAN.CPP ************ Panteleeva N.A. ****** 14/06/16 ***
****** TERRPLAN.CPP ************ Belenkov O.V.   ****** 23/09/15 ***
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

#ifndef IMLSERV_H
  #include "imlserv.h"
#endif

#ifndef TERRPLAN_H
  #include "terrplan.h"
#endif

#include <math.h>


//-----------------------------------------------------------------
// Знак, меняющий размер и цвет
//-----------------------------------------------------------------
int _fastcall SimpleObject(const POLYDATAEX * data, const IMGDRAW * draw,
                                   const IMGCONTEXT * context, int test,
                                   FRAME * border)
{
 const char * vectorparm;
 long         value;
 unsigned int color = 0xFFFFFFFF;

 IMGVECTORMIX  vectormix;

 memset(&vectormix,0,sizeof(IMGVECTORMIX));
 vectormix.Mixing.Exclusion = 0xFFFFFFFF;
 vectormix.Mixing.Transparency = 100;
 vectormix.Mixing.Scale = 100;

 // Запросим значение семантики 10012  НОВИЗНА
 if (imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10012, 1, &value) != 0)
   {
     if (value == 2)  color = 0x0FF;      // Планируемый - красный
     if (value == 3)  color = 0x0287CD0;  // Планируемый к реконстр - коричн
     if (value == 4)  color = 0x480F0F0;  // Планируемый к кап. ремонту - желтый
   }

 // Если заполнены значения семантик 10005 и 10816 можно перебить цвет (приоритет)
 long value5 = -1;
 long value16 = -1;

 imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10005, 1, &value5);
 imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10816, 1, &value16);
 if (value5 == 2) // меняем цвет только у планируемых
  {
     if (value16 == -1 ) color = 0x0FF;      // Планируемый - красный
     if ((value16 == 2)|| (value16 == 3))   color = 0x0287CD0;  // Планируемый к реконстр - коричн
     if (value16 > 5)  color = 0x480F0F0;  // Планируемый к кап. ремонту - желтый - пока такого значения нет
  }

 vectormix.Mixing.Color = color;

 // Запросим значение семантики 10803 Значение ОКС
 if (imlGetSemanticLong((SEMANTIC *)(data->Semantic) , 10803, 1, &value) != 0)
  {
     if (value == 1) vectormix.Mixing.Scale = 140;
     if (value == 2) vectormix.Mixing.Scale = 120;
  }

 // Параметры функции основного знака
 vectorparm = imlDrawParm(draw, 1);

 vectormix.Vector = (IMGVECTOREX *)vectorparm;

 // Отобразить объект с учетом цвета, масштаба....
 return imgPrepareImage(data, IMG_VECTORMIX, (char*)&vectormix,
                        context, test, border);
}

//-----------------------------------------------------------------
// Знак, меняющий вид и цвет
//-----------------------------------------------------------------
int _fastcall ComplexObject(const POLYDATAEX * data, const IMGDRAW * draw,
                                   const IMGCONTEXT * context, int test,
                                   FRAME * border)
{
 const char * vectorparm1;
 const char * vectorparm2;
 long         value;
 unsigned int color = 0xFFFFFFFF;

 IMGVECTORMIX  vectormix;

 memset(&vectormix,0,sizeof(IMGVECTORMIX));
 vectormix.Mixing.Exclusion = 0xFFFFFFFF;
 vectormix.Mixing.Transparency = 100;
 vectormix.Mixing.Scale = 100;
 vectormix.Mixing.Color = color;

 // Запросим значение семантики 10012  НОВИЗНА
 value = 0;
 if (imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10012, 1, &value) != 0)
   {
     if (value == 3)  color = 0x0287CD0;  // Планируемый к реконстр - коричн
     if (value == 4)  color = 0x480F0F0;  // Планируемый к кап. ремонту - желтый
   }

 // Если заполнены значения семантик 10005 и 10816 можно перебить цвет (приоритет)
 long value5 = -1;
 long value16 = -1;

 imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10005, 1, &value5);
 imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10816, 1, &value16);
 if (value5 == 2) // меняем цвет только у планируемых
  {
     value = 2;
     if ((value16 == 2)|| (value16 == 3))
       {
          color = 0x0287CD0;  // Планируемый к реконстр - коричн
          value = 3;
       }
     if (value16 > 5)
       {
         color = 0x480F0F0;  // Планируемый к кап. ремонту - желтый - пока такого значения нет
         value = 4;
       }
  }


 // Параметры функции основного знака
 vectorparm1 = imlDrawParm(draw, 1);

 // Если есть параметры планируемого знака,
 if (value > 1)
   { // Запросим параметры функции планируемого знака
     vectorparm2 = imlDrawParm(draw, 2);
     // знак есть - будем рисовать его нет - основной знак
     if (vectorparm2 != 0)
       vectorparm1 = vectorparm2;
   }

 vectormix.Vector = (IMGVECTOREX *)vectorparm1;

 // Отобразить объект с учетом цвета, масштаба....
 if ((imgPrepareImage(data, IMG_VECTORMIX, (char*)&vectormix,
                      context, test, border) != 0) &&
     (test == PREPARE_TEST))
   {
     return 1;
   }

 if (value < 3) return 0; // больше не рисуем

 // Если есть параметры части, меняющей цвет
 vectorparm2 = imlDrawParm(draw, 3);
 if (vectorparm2 == 0) return 0; // больше не рисуем
 vectormix.Vector = (IMGVECTOREX *)vectorparm2;
 vectormix.Mixing.Color = color;

 // Отобразить объект с учетом цвета, масштаба....
 return imgPrepareImage(data, IMG_VECTORMIX, (char*)&vectormix,
                        context, test, border);
}

//-----------------------------------------------------------------
// Линия, меняющяя вид и цвет
//-----------------------------------------------------------------
int _fastcall LineObject(const POLYDATAEX * data, const IMGDRAW * draw,
                                   const IMGCONTEXT * context, int test,
                                   FRAME * border)
{
 const char * drawparm1;
 const char * drawparm2;
 long         value = 0;
 unsigned int color = 0xFFFFFFFF;

 IMGDRAWMIX  drawmix;
 memset(&drawmix,0,sizeof(IMGDRAWMIX));
 drawmix.Mixing.Exclusion = 0xFFFFFFFF;
 drawmix.Mixing.Transparency = 100;
 drawmix.Mixing.Scale = 100;

 // Запросим значение семантики 10012  НОВИЗНА
 if (imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10012, 1, &value) != 0)
   {
     if (value == 2)  color = 0x0FF;      // Планируемый - красный
     if (value == 3)  color = 0x0287CD0;  // Планируемый к реконстр - коричн
     if (value == 4)  color = 0x480F0F0;  // Планируемый к кап. ремонту - желтый
   }

 // Если заполнены значения семантик 10005 и 10816 можно перебить цвет (приоритет)
 long value5 = -1;
 long value16 = -1;

 imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10005, 1, &value5);
 imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10816, 1, &value16);
 if (value5 == 2) // меняем цвет только у планируемых
  {
     value = 2;
     if (value16 == -1 ) color = 0x0FF;      // Планируемый - красный
     if ((value16 == 2)|| (value16 == 3))
       {
          value = 3;
          color = 0x0287CD0;  // Планируемый к реконстр - коричн
       }
     if (value16 > 5)
       {
         value = 4;
         color = 0x480F0F0;  // Планируемый к кап. ремонту - желтый - пока такого значения нет
       }
  }

  // Параметры функции основной линии
  drawparm1 = imlDrawParm(draw, 1);

  // Если есть параметры планируемой линии,
  if (value > 1)
   { // Запросим параметры функции планируемой линии
     drawparm2 = imlDrawParm(draw, 2);
     // линия есть - будем рисовать ее нет - основную
     if (drawparm2 != 0)
       drawparm1 = drawparm2;
   }

 // Отобразить объект основной или планируемый
 if ((imgPrepareImage(data, IMG_DRAW,drawparm1,
                      context, test, border) != 0) &&
     (test == PREPARE_TEST))
   {
     return 1;
   }

 if (value <= 1) return 0; // больше не рисуем

 if (drawparm2 != 0) // нет параметров планируемой
   {
     if (value < 3) return 0; // больше не рисуем
   }

 // Если есть параметры части, меняющей цвет
 drawparm2 = imlDrawParm(draw, 3);
 if (drawparm2 == 0) return 0; // больше не рисуем
 drawmix.Draw = (IMGDRAW *)drawparm2;
 drawmix.Mixing.Color = color;

 // Отобразить объект с учетом цвета, масштаба....
 return imgPrepareImage(data, IMG_DRAWMIX, (char*)&drawmix,
                                                       context, test, border);
}

//-----------------------------------------------------------------
// Линия, меняющяя вид и цвет
//-----------------------------------------------------------------
int _fastcall ContourPT(const POLYDATAEX * data, const IMGDRAW * draw,
                        const IMGCONTEXT * context, int test,
                         FRAME * border)
{
 const char * drawparm1;
 const char * drawparm2;
 long         value = 0;
 long         impt  = 0;
 unsigned int color = 0xFFFFFFFF;
 long ret;

 IMGDOT dot;
 IMGDOT dot1;
 IMGLINE line;
 IMGLINE line1;

 // Запросим значение семантики 10012  НОВИЗНА
 if (imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10012, 1, &value) != 0)
   {
     if (value >= 2)  color = 0x0FF;      // Планируемый - красный
   }

 // Если заполнены значения семантик 10005 и 10816 можно перебить цвет (приоритет)
 long value5 = -1;

 imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10005, 1, &value5);
 if (value5 == 2) // меняем цвет только у планируемых
  {
     value = 2;
     color = 0x0FF;      // Планируемый - красный
  }


 // Запросим значение семантики 11307  Значение
 imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  11307, 1, &impt);

  // Параметры функции основной линии
  drawparm1 = imlDrawParm(draw, 1);
  if (drawparm1 == 0) return 0;
  if (imlDrawType(draw, 1) != IMG_LINE) return 0;

  // Параметры функции линии, меняющей цвет
  drawparm2 = imlDrawParm(draw, 2);
  if (drawparm2 == 0) return 0;
  if (imlDrawType(draw, 2) != IMG_LINE) return 0;

  // если нет дополнительных условий рисуем по введенным параметрам
  if ((value <= 1)&& ((impt > 2)||(impt == 0)))
   {
      if ((imgPrepareImage(data, IMG_LINE,drawparm1,
                      context, test, border) != 0) &&
             (test == PREPARE_TEST))
          {
            return 1;
          }
     return imgPrepareImage(data, IMG_LINE,drawparm2,context, test, border);
   }

  if ( value <= 1)     // Будем рисовать линию
    {
      line.Color = ((IMGLINE*)drawparm1)->Color;
      line.Thick = ((IMGLINE*)drawparm1)->Thick;
      line1.Color = ((IMGLINE*)drawparm2)->Color;
      line1.Thick = ((IMGLINE*)drawparm2)->Thick;

      if ( impt == 2)   // Региональные
        {
          line1.Color = 0x0FF;
        }
      if ( impt == 1) // федеральные
        {
           line1.Color = 0x0FF;
           line.Thick = 2 * ((IMGLINE*)drawparm1)->Thick;
           line1.Thick = 2 * ((IMGLINE*)drawparm1)->Thick;
        }
      if ((imgPrepareImage(data, IMG_LINE,(const char *)&line,
                      context, test, border) != 0) &&
             (test == PREPARE_TEST))
          {
            return 1;
          }
      return imgPrepareImage(data, IMG_LINE,(const char *)&line1,context, test, border);
   }

  // Будем рисовать пунктир
  dot.Color = ((IMGLINE*)drawparm1)->Color;
  dot.Thick = ((IMGLINE*)drawparm1)->Thick;
  dot1.Color = ((IMGLINE*)drawparm2)->Color;
  dot1.Thick = ((IMGLINE*)drawparm2)->Thick;
  dot.Dash   = 2000;          // 2 мм
  dot.Blank  = 2000;          // 2 мм
  dot1.Dash   = 2000;          // 2 мм
  dot1.Blank  = 2000;          // 2 мм
  if (impt == 2)
    {
       dot1.Color = 0x0FF;
    }
  else
  if (impt == 1)
    {
      dot1.Color = 0x0FF;
      dot.Thick = 2 * ((IMGLINE*)drawparm1)->Thick;
      dot1.Thick = 2 * ((IMGLINE*)drawparm1)->Thick;
    }

  if ((imgPrepareImage(data, IMG_DOT,(const char *)&dot,
                      context, test, border) != 0) &&
      (test == PREPARE_TEST))
    {
      return 1;
    }

  return imgPrepareImage(data, IMG_DOT,(const char *)&dot1,context, test, border);
}

//-----------------------------------------------------------------
// Знак, меняет размер (по заданной семантике) и цвет
//-----------------------------------------------------------------
 int _fastcall SemSizeObject(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border)
{
  const char * vectorparm;
 long         value;
 unsigned int color = 0xFFFFFFFF;
 const  long * semparm;
 int    semfunc;
 IMGVECTORMIX  vectormix;

 memset(&vectormix,0,sizeof(IMGVECTORMIX));
 vectormix.Mixing.Exclusion = 0xFFFFFFFF;
 vectormix.Mixing.Transparency = 100;
 vectormix.Mixing.Scale = 100;


 // Запросим значение семантики 10012  НОВИЗНА
 if (imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10012, 1, &value) != 0)
   {
     if (value == 2)  color = 0x0FF;      // Планируемый - красный
     if (value == 3)  color = 0x0287CD0;  // Планируемый к реконстр - коричн
     if (value == 4)  color = 0x480F0F0;  // Планируемый к кап. ремонту - желтый
   }
 // Если заполнены значения семантик 10005 и 10816 можно перебить цвет (приоритет)
 long value5 = -1;
 long value16 = -1;

 imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10005, 1, &value5);
 imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10816, 1, &value16);
 if (value5 == 2) // меняем цвет только у планируемых
  {
     if (value16 == -1 ) color = 0x0FF;      // Планируемый - красный
     if ((value16 == 2)|| (value16 == 3))   color = 0x0287CD0;  // Планируемый к реконстр - коричн
     if (value16 > 5)  color = 0x480F0F0;  // Планируемый к кап. ремонту - желтый - пока такого значения нет
  }

 vectormix.Mixing.Color = color;

 // По какой семантике изменять размер знака
 semfunc = imlDrawType(draw, 2);
 if (semfunc == IMG_SEMANTIC)
   {
     semparm =(long *) imlDrawParm(draw, 2);
     // Запросим значение семантики
     if (imlGetSemanticLong((SEMANTIC *)(data->Semantic),*semparm, 1, &value)!= 0)
       {
          if (value == 1) vectormix.Mixing.Scale = 140;
          if (value == 2) vectormix.Mixing.Scale = 120;
       }
  }

 // Параметры функции основного знака
 vectorparm = imlDrawParm(draw, 1);

 vectormix.Vector = (IMGVECTOREX *)vectorparm;

 // Отобразить объект с учетом цвета, масштаба....
 return imgPrepareImage(data, IMG_VECTORMIX, (char*)&vectormix,
                        context, test, border);
}

//-----------------------------------------------------------------
// Окантовка участков недр
//-----------------------------------------------------------------
 int _fastcall BowelsContour(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border)
{
  const char * drawparm1;
  long         value = 0;
  unsigned int color = 0xFFFFFFFF;

  IMGDRAWMIX  drawmix;
  memset(&drawmix,0,sizeof(IMGDRAWMIX));
  drawmix.Mixing.Exclusion = 0xFFFFFFFF;
  drawmix.Mixing.Transparency = 100;
  drawmix.Mixing.Scale = 100;

  // Запросим значение семантики 10006  Значение участка недр
  if (imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  10006, 1, &value) != 0)
   {
     if (value == 1)  color = 0x0FF;      // федеральный - красный
     if (value == 2)  color = 0x0FF55FF;  // региональный - розовый
   }

  if (color == 0xFFFFFFFF)  // недра не имеют значения фед или рег - не рисуем
   {
     return 1;
   }

  value = 2;

  // Запросим значение семантики 12005 Использование участка недр
  imlGetSemanticLong((SEMANTIC *)(data->Semantic) ,  12005, 1, &value);

  // Если сематики не было или value != 1 резерв

  // Параметры функции основной линии
  if (value == 1)
     drawparm1 = imlDrawParm(draw, 1);
  else
      // Запросим параметры функции резервной линии
     drawparm1 = imlDrawParm(draw, 2);

  if (drawparm1 == 0) return 0; // Не рисуем

  drawmix.Draw = (IMGDRAW *)drawparm1;
  drawmix.Mixing.Color = color;

 // Отобразить объект
 return imgPrepareImage(data, IMG_DRAWMIX, (char*)&drawmix,context, test, border);

}

//-----------------------------------------------------------------
// Модифицированный TrueType объект
// изменение размера(c указанием семантики и ее значений)
// изменение цвета (с указанием значения и соотв цветов)
//-----------------------------------------------------------------
 int _fastcall ModyTrueType(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border)
{
 const char * vectorparm;
 IMGTRUETYPE trtype;
 long         value;
 long         value120;
 long         value140;
 long         valueColor;
 unsigned int color = 0xFFFFFFFF;
 const  long * semparm;
 int    semfunc;
 int    vsfunc;
 double scale = 1.0;

 semfunc = imlDrawType(draw, 1);
 if (semfunc != IMG_TRUETYPE) return 0;
 // Запросим код семантики для размера
 semfunc = imlDrawType(draw, 2);
 if (semfunc == IMG_SEMANTIC)
   {
     semparm =(long *) imlDrawParm(draw, 2);
     vsfunc = imlDrawType(draw, 3);
     if ( vsfunc == IMG_LONG)
       {
         value140 = *(long *)imlDrawParm(draw, 3);
         // Запросим значение семантики
         if (imlGetSemanticLong((SEMANTIC *)(data->Semantic),*semparm, 1, &value)!= 0)
           {
             if (value == value140) scale = 1.4;
           }
        }

     vsfunc = imlDrawType(draw, 4);
     if ( vsfunc == IMG_LONG)
       {
         value120 = *(long *)imlDrawParm(draw, 4);
         // Запросим значение семантики
         if (imlGetSemanticLong((SEMANTIC *)(data->Semantic),*semparm, 1, &value)!= 0)
           {
             if (value == value120) scale = 1.2;
           }
        }
  }

  color = -1;
 // Запросим код семантики для цвета
 semfunc = imlDrawType(draw, 5);
 if (semfunc == IMG_SEMANTIC)
   {
     semparm =(long *) imlDrawParm(draw, 5);
     if (semparm != 0)
      {
          if (imlGetSemanticLong((SEMANTIC *)(data->Semantic),*semparm, 1, &value) != 0)
             {
               vsfunc = imlDrawType(draw, 6);
               if ( vsfunc == IMG_LONG)        // первый цвет
                 {
                    valueColor = *(long *)imlDrawParm(draw, 6);
                    if (valueColor == value)   // будем рисовать этим цветом
                      {
                        vsfunc = imlDrawType(draw, 7);
                        if ( vsfunc == IMG_COLOR)
                          {
                            color = *(long *)imlDrawParm(draw, 7);
                          }
                     }
                 }
               if (color == -1)                // 2 цвет
                 {
                   vsfunc = imlDrawType(draw, 8);
                   if ( vsfunc == IMG_LONG)
                     {
                        valueColor = *(long *)imlDrawParm(draw, 8);
                        if (valueColor == value)   // будем рисовать этим цветом
                           {
                              vsfunc = imlDrawType(draw, 9);
                              if ( vsfunc == IMG_COLOR)
                                {
                                  color = *(long *)imlDrawParm(draw, 9);
                                }
                            }
                      }
                 }
                if (color == -1)                // 3 цвет
                 {
                   vsfunc = imlDrawType(draw, 10);
                   if ( vsfunc == IMG_LONG)
                     {
                        valueColor = *(long *)imlDrawParm(draw, 10);
                        if (valueColor == value)   // будем рисовать этим цветом
                           {
                              vsfunc = imlDrawType(draw, 11);
                              if ( vsfunc == IMG_COLOR)
                                {
                                  color = *(long *)imlDrawParm(draw, 11);
                                }
                            }
                      }
                 }
                if (color == -1)                // 4 цвет
                 {
                   vsfunc = imlDrawType(draw, 12);
                   if ( vsfunc == IMG_LONG)
                     {
                        valueColor = *(long *)imlDrawParm(draw, 12);
                        if (valueColor == value)   // будем рисовать этим цветом
                           {
                              vsfunc = imlDrawType(draw, 13);
                              if ( vsfunc == IMG_COLOR)
                                {
                                  color = *(long *)imlDrawParm(draw, 13);
                                }
                            }
                      }
                 }
                if (color == -1)                // 2 цвет
                 {
                   vsfunc = imlDrawType(draw, 14);
                   if ( vsfunc == IMG_LONG)
                     {
                        valueColor = *(long *)imlDrawParm(draw, 14);
                        if (valueColor == value)   // будем рисовать этим цветом
                           {
                              vsfunc = imlDrawType(draw, 15);
                              if ( vsfunc == IMG_COLOR)
                                {
                                  color = *(long *)imlDrawParm(draw, 15);
                                }
                            }
                      }
                 }
             }
       }

   }

 // Параметры функции основного знака
 vectorparm = imlDrawParm(draw, 1);
 memmove((char *)&trtype,vectorparm,sizeof(trtype));
 if (color!= -1)
    trtype.Text.Color = color;
 if (scale != 1.0)
    trtype.Text.Height = (unsigned int)((double)(trtype.Text.Height)*scale);

 // Отобразить объект с учетом цвета, масштаба....
 return imgPrepareImage(data, IMG_TRUETYPE, (char*)&trtype, context,
                        test, border);
}


 // Графический знак, меняющий фон и окантовку
 int _fastcall ObjectMark(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border)

{
  if ((data == 0) || (draw == 0) || (context == 0)) return 0;
  const char * parm;

  int func;
  int ret;
  long value = 1;
  // Запросим семантику 501 - статус состояния объекта - для рисования фона и черточки
  if (data->Semantic == 0) value = 1;
  else
    {
      imlGetSemanticLong((SEMANTIC *)(data->Semantic),501, 1, &value);
    }

  long frame = 3;

  // Запросим семантику 500 - статус значимости объекта для рисования рамок
  if (data->Semantic == 0) frame = 3;
  else
    {
      imlGetSemanticLong((SEMANTIC *)(data->Semantic),500, 1, &frame);
    }

  int incode = 0;
  int excode = 401;         // это маленькие
  int frameexcode = 410;
  int dopexcode = 0;
  if (value == 3) excode = 403;  // подложка красного цвета
  if (value == 2) excode = 402;  // подложка желтого цвета
  if (frame == 2)                // Рег. значение
    {
      excode += 3;
      frameexcode = 411;
    }
  if (frame == 1)
    {
      excode += 6;
      frameexcode = 412;
    }

  if (value == 4)
    {
      dopexcode = 414;
    }

  incode = imlGetRscObjectIncode(data->MapRsc,excode,2,1);
  // Взяли параметры знака фона
  if (imlGetRscObjectFunction(data->MapRsc,incode)== IMG_PICTURE)
    {
       parm = imlGetRscObjectParameters(data->MapRsc,incode);
       if ((imgPrepareImage(data, IMG_PICTURE, parm, context,test, border)!= 0)&&
           (test == PREPARE_TEST))
           {
             return 1;
           }
    }

  // Рисование самого знака
  func = imlDrawType(draw, 1);

  if (func != IMG_PICTURE) return 0;
  parm = imlDrawParm(draw, 1);

  // Отобразить объект с учетом цвета, масштаба....
  if ((imgPrepareImage(data, IMG_PICTURE, parm, context,test, border)!= 0)&&
      (test == PREPARE_TEST))
        {
          return 1;
        }

  // Рисование рамки
  incode = imlGetRscObjectIncode(data->MapRsc,frameexcode,2,1);
  // Взяли параметры знака фона
  if (imlGetRscObjectFunction(data->MapRsc,incode)== IMG_PICTURE)
    {
       parm = imlGetRscObjectParameters(data->MapRsc,incode);
       ret = imgPrepareImage(data, IMG_PICTURE, parm, context,test, border);
       if ((ret!= 0)&&(test == PREPARE_TEST))
           {
             return 1;
           }
    }

  // Рисование перечеркивания
  incode = imlGetRscObjectIncode(data->MapRsc,dopexcode,2,1);
  if (incode == 0) return ret;
  // Взяли параметры знака фона
  if (imlGetRscObjectFunction(data->MapRsc,incode)== IMG_PICTURE)
    {
       parm = imlGetRscObjectParameters(data->MapRsc,incode);
       ret = imgPrepareImage(data, IMG_PICTURE, parm, context,test, border);
       if ((ret!= 0)&&(test == PREPARE_TEST))
           {
             return 1;
           }
    }
  return ret;


}

 // Линия, меняющая вид по статусу
 int _fastcall ObjectLine(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border)
 {
  if ((data == 0) || (draw == 0) || (context == 0)) return 0;
  const char * parm;

  int func;
  int ret;
  long value = 1;
   // Запросим семантику 501 - статус состояния объекта - для рисования фона и черточки
  if (data->Semantic == 0) value = 1;
  else
    {
      imlGetSemanticLong((SEMANTIC *)(data->Semantic),501, 1, &value);
    }

 int part = value;
 if (value > 4) part = 1;
 if (value == 2) part = 3;
 if (value == 3) part = 2;
 // Рисование самого знака
 func = imlDrawType(draw, part);
 if (func != IMG_DRAW) return 0;
 parm = imlDrawParm(draw, part);
 if (parm == 0) return 0;
 if (((IMGDRAW *)parm)->Count == 0)
   {
     if (test == PREPARE_TEST) return 1;
     return 0;
   }
 // Отобразить объект с учетом цвета, масштаба....
 ret = imgPrepareImage(data, IMG_DRAW, parm, context,test, border);

 if ((ret!= 0)&&(test == PREPARE_TEST))
    {
       return 1;
    }

 return ret;

}

 // Площадь, меняющая вид по статусу
 int _fastcall ObjectSquare(const POLYDATAEX * data, const IMGDRAW * draw,
                             const IMGCONTEXT * context, int test,
                             FRAME * border)
 {
  if ((data == 0) || (draw == 0) || (context == 0)) return 0;
  const char * parm;

  int func;
  int ret;
  long value = 1;
   // Запросим семантику 501 - статус состояния объекта
  if (data->Semantic == 0) value = 1;
  else
    {
      imlGetSemanticLong((SEMANTIC *)(data->Semantic),501, 1, &value);
    }

 int part = value;
 if (value > 4) part = 1;
 if (value == 2) part = 3;
 if (value == 3) part = 2;
 // Рисование самого знака
 func = imlDrawType(draw, part);
 if (func != IMG_DRAW) return 0;
 parm = imlDrawParm(draw, part);
 if (parm == 0) return 0;
 if (((IMGDRAW *)parm)->Count == 0)
   {
     if (test == PREPARE_TEST) return 1;
     return 0;
   }
 // Отобразить объект с учетом цвета, масштаба....
 ret = imgPrepareImage(data, IMG_DRAW, parm, context,test, border);

 if ((ret!= 0)&&(test == PREPARE_TEST))
    {
       return 1;
    }

 return ret;




 }

