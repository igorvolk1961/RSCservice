/******  mapmacro.h  ***********  Belenkov O.V. ******  05/08/21 ***
*******  mapmacro.h  ***********  Shabakov D.A. ******  28/08/13 ***
*******  mapmacro.h  ***********  Борзов А.Ю.   ******  03/02/14 ***
*******  mapmacro.h  ***********  Kruzhkov A.E. ******  04/02/20 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2021              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*                  ОПИСАНИЕ МАКРОРАСШИРЕНИЙ                        *
*                                                                  *
*******************************************************************/

#ifndef MAPMACRO_H
#define MAPMACRO_H

#define HUGER  // Устаревшее определение

#ifndef MAPSYST_H
  #include "mapsyst.h"
#endif

// Идентификаторы сообщений процессов обработки карты
#define IMM_OBJECT 0x585
#define IMM_LIST   0x586
#define IMM_ERROR  0x587

#ifndef M_SQRT2
  #define M_SQRT2     1.41421356237309504880
#endif

#ifndef M_PI
  #define M_PI        3.14159265358979323846
  #define M_PI_2      1.57079632679489661923
  #define M_PI_4      0.785398163397448309616
#endif

#define MPI M_PI

#ifndef WM_TOUCH
  #define WM_TOUCH 0x0240
#endif

#define MAX_PATH_LONG  1024

#ifndef Min_Body
  #define Min_Body

  // The following Min() and Max() are for compatibility
  template <class T> inline T Min(const T& a, const T& b)
    {
      return a < b ? a : b;
    }
#endif

#ifndef Max_Body
  #define Max_Body
  template <class T> inline T Max(const T& a, const T& b)
    {
      return a > b ? a : b;
    }
#endif

#ifdef CHECK_MEMORY
#ifndef MACRONEW
#define MACRONEW

extern "C"
{
  _MAPIMP char * _MAPAPI mapAllocate(unsigned int value);
  _MAPIMP void   _MAPAPI mapFree(char * memory);
}

#ifndef _MSC_VER

inline void * operator new[] (size_t value)
{
  return (void *)::mapAllocate((unsigned int) value);
}

inline void operator delete[] (void * point)
{
  ::mapFree((char *)point);
}

inline void * operator new (size_t value)
{
  return (void *)::mapAllocate((unsigned int) value);
}

inline void operator delete (void * point)
{
  ::mapFree((char *)point);
}

#endif

#endif // MACRONEW
#endif // CHECK_MEMORY

#endif // MAPMACRO_H
