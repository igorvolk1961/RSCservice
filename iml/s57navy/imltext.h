/****** IMLTEXT.H ************** Belenkov O.V.  *****  18/04/16 ****
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2016              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*                ОБЩАЯ ФУНКЦИЯ ЗАПРОСА СТРОКИ                      *
*                                                                  *
*******************************************************************/

#ifndef VCONST

const WCHAR * imltxt(int number); 
int imltxt(int number, char * name, int size);
int imltxt(int number, WCHAR * name, int size);

#else

//------------------------------------------------------------------
// Функция запроса строки в WCHAR
//------------------------------------------------------------------
const WCHAR * VCONST(imltxt)(int number)
{
  switch(number)
  {
    case 1:        return VCONST(imlmain1);
    case 2:        return VCONST(imlmain2);
    case 3:        return VCONST(imlmain3);
    case 4:        return VCONST(imlmain4);
    case 5:        return VCONST(imlmain5);
    case 6:        return VCONST(imlmain6);
    case 7:        return VCONST(imlmain7);
    case 8:        return VCONST(imlmain8);
    case 9:        return VCONST(imlmain9);
    case 10:       return VCONST(imlmain10);
    case 11:       return VCONST(imlmain11);
    case 12:       return VCONST(imlmain12);
    case 13:       return VCONST(imlmain13);
    case 14:       return VCONST(imlmain14);
    case 15:       return VCONST(imlmain15);
    case 16:       return VCONST(imlmain16);
    case 17:       return VCONST(imlmain17);
    case 18:       return VCONST(imlmain18);
    case 19:       return VCONST(imlmain19);
    case 20:       return VCONST(imlmain20);
    case 21:       return VCONST(imlmain21);
    case 22:       return VCONST(imlmain22);
    case 23:       return VCONST(imlmain23);
    case 24:       return VCONST(imlmain24);
    case 25:       return VCONST(imlmain25);
    case 26:       return VCONST(imlmain26);
    case 27:       return VCONST(imlmain27);
    case 28:       return VCONST(imlmain28);
    case 29:       return VCONST(imlmain29);
    case 30:       return VCONST(imlmain30);
    case 31:       return VCONST(imlmain31);
    case 32:       return VCONST(imlmain32);
    case 33:       return VCONST(imlmain33);
    case 34:       return VCONST(imlmain34);
    case 35:       return VCONST(imlmain35);
    case 36:       return VCONST(imlmain36);
    case 37:       return VCONST(imlmain37);
    case 38:       return VCONST(imlmain38);
    case 39:       return VCONST(imlmain39);
    case 40:       return VCONST(imlmain40);
  }

  // Строки с таким номером нет
  return WTEXT(" ??? ");
}

#undef VCONST
#endif
