//****************************       DFC.H     ******************************
//*	        	Заголовочный файл  (на С++)
//****************************************************************************
// Автор          : Кота Ф.Ф.
// Перевод на C++ : Кота Ф.Ф.
//****************************************************************************

#ifndef DFC_H
#define DFC_H

#ifndef MAPGDIEX_H
  #include "mapgdiex.h"
#endif

#define DFC_ENROUTE_TWO         1000  // Двухсторонний маршрут
#define DFC_ENROUTE_ONE         1001  // Односторонний маршрут
#define DFC_ENROUTE_TWOCDR      1002  // Двухсторонний маршрут
#define DFC_ENROUTE_ONECDR      1003  // Односторонний маршрут
#define DFC_VECTOR_ARROW        1004  // Бокс с линией выноски
#define DFC_ROTATED_MARK        1005  // Знак с поворотом по семантике
#define DFC_HSCALED_MARK        1006  // Масштабируемый знак (по ширине)
#define DFC_AZIMUTH_DIST        1007  // Комбинированный знак (азимут/длина)
#define DFC_SID_STAR            1008  // Аэродромный маршрут SID/STAR/IAP
#define DFC_VAR_WIDTH_LINE      1009  // Линия переменной ширины
#define DFC_VECTOR_GROUP        1010  // Группа векторных знаков
#define DFC_SECTOR_MSA          1011  // Секторы МБВ


// Маршрут
int _fastcall DrawEnroute(const POLYDATAEX* pdata, const IMGDRAW* draw,
                            const IMGCONTEXT * context, int test,
                            FRAME * border, int IsDual);

// Бокс с линией выноски
int _fastcall DrawVecArrow(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border);

//  Знак с поворотом по семантике
int _fastcall DrawRotatedVector(const POLYDATAEX* pdata, const IMGDRAW* draw,
                           const IMGCONTEXT * context, int test,
                           FRAME * border);
// Масштабируемый знак (по ширине)
int _fastcall DrawScaledVector(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border);

// Условный знак с поворотом по семантике
int _fastcall DrawAzimuthDistanceVector(const POLYDATAEX* pdata, const IMGDRAW* draw,
                       const IMGCONTEXT * context, int test,
                       FRAME * border);

// Знак "аэродромный маршрут"
int _fastcall DrawAirportRoute(const POLYDATAEX* pdata, const IMGDRAW* draw,
                       const IMGCONTEXT * context, int test,
                       FRAME * border);

// Текст с параметрами из семантики
int _fastcall DrawVarWidthLine(const POLYDATAEX* pdata, const IMGDRAW* draw,
                            const IMGCONTEXT * context, int test,
                            FRAME * border);

// Группа векторных объектов
int _fastcall DrawObjectGroup(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border);

// Секторы МБВ
int _fastcall DrawSectorMSA(const POLYDATAEX* pdata, const IMGDRAW* draw,
                          const IMGCONTEXT * context, int test,
                          FRAME * border);


#endif

