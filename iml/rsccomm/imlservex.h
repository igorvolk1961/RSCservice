#ifndef IMLSERVEX_H
#define IMLSERVEX_H

#define GIS_OPERATOR 1

//#include <map>
//#include <cstring>
#include "rsc.h"
#include "rscheader.h"

#ifndef MAPGDIEX_H
#include "mapgdiex.h"
#endif

#ifndef LINUXAPI  
__declspec(dllexport)
#endif
void _IMLAPI imlOpenGisLibraryEx(HINSTANCE hMapLibrary);

#ifndef LINUXAPI  
__declspec(dllexport)
#endif
void _IMLAPI imlCloseGisLibraryEx();

int imgPrepareImage_buildSvg(const POLYDATAEX * data,
	int type, const char * parm,
	const IMGCONTEXT * context);

// Открыть классификатор
// name - имя  файла классификатора в кодировке UNICODE
// size    - размер строки
// При ошибке возвращает ноль
//HRSC _fastcall mapOpenRscUn(const WCHAR * name);
HRSC _fastcall mapOpenRsc(const char * name);

// Закрыть классификатор
void _fastcall mapCloseRsc(HRSC hrsc);

//Разрешить/Запретить выдачу сообщений на экран (серверный режим работы).
//\param enable Флаг выдачи сообщений:
//              \arg 0 - запрет выдачи сообщений;
//\returns
//Возвращает предыдущее значение флага.
long int _fastcall mapMessageEnable(long int enable);

//Запросить цвет из палитры.
//param hRsc Идентификатор классификатора карты;
//param index Порядковый номер цвета в палитре (с 1);
//param number Порядковый номер палитры (с 1);
//returns
//Возвращает цвет в COLORREF (mapsyst.h).
COLORREF _fastcall mapGetRscColor(HRSC hRsc,long int index,
                                     long int number = 1);

// Запросить внутренний код (порядковый номер) объекта
// по внешнему коду , локализации  и порядковому номеру среди аналогичных
// объектов(с 1)
// hRsc - идентификатор классификатора карты
// При ошибке возвращает ноль
long int _fastcall imlGetRscObjectCodeByNumber(HRSC hRsc,
	long int excode,
	long int local,
	long int number = 1);

// Запросить внутренний код (порядковый номер) объекта
// по ключу
// hRsc - идентификатор классификатора карты
// key  - ключ объекта
// При ошибке возвращает ноль
long int _fastcall imlGetRscObjectCodeByKey(HRSC hRsc, const char * key);

// Заполнить структуру описания объекта
// RSCOBJECT -  структура входных данных  (см. maptype.h)
// hRsc - идентификатор классификатора карты
// incode - порядковый номер объекта (с 1)
// При ошибке возвращает ноль , иначе порядковый номер объекта

long int  _fastcall imlGetRscObject(HRSC hRsc, long int incode,
    RSCOBJECT * object);

// Заполнить структуру описания сегмента
// RSCSEGMENT -  структура входных данных  (см. maptype.h)
// hRsc - идентификатор классификатора карты
// incode - порядковый номер сегмента (с 1)
// При ошибке возвращает ноль , иначе порядковый номер сегмента

long int  _fastcall imlGetRscSegment(HRSC hRsc,
    RSCSEGMENT * segment, long int incode);

// Запрос количества слоев
// hRsc - идентификатор классификатора карты
// При ошибке возвращает ноль

long int _fastcall imlGetRscSegmentCount(HRSC hRsc);

// Запрос количества объектов в слое с заданной локализацией
// hRsc - идентификатор классификатора карты
// incode - номер слоя
// local - локализация
// При ошибке возвращает ноль
long int  _fastcall imlGetRscSegmentLocalCount(HRSC hRsc, int* count, long int incode, long int local);

// Запрос перечня объектов в слое с заданной локализацией
// hRsc - идентификатор классификатора карты
// incode - номер слоя
// local - локализация
// При ошибке возвращает ноль
long int  _fastcall imlGetRscSegmentLocalIncodes(HRSC hRsc, int** incodes, long int incode, long int local);

// Запросить число объектов описанных в классификаторе
// hRsc - идентификатор классификатора карты
// При ошибке возвращает ноль

long int _fastcall imlGetRscObjectCount(HRSC hRsc);

// Запрос имени слоя по порядковому номеру слоя (с 0)
// hRsc - идентификатор классификатора карты
// incode - номер слоя
// При ошибке возвращает ""

const char * _fastcall imlGetRscSegmentName(HRSC hRsc,long int incode);

// Запрос короткого имени слоя по порядковому номеру слоя (с 0)
// hRsc - идентификатор классификатора карты
// incode - номер слоя
// При ошибке возвращает ""

const char * _fastcall imlGetRscSegmentShortName(HRSC hRsc,long int incode);

// Запросить ключ объекта по внутреннему  коду (порядковому номеру)
// объекта (с 1)
// hRsc - идентификатор классификатора карты
// incode - внутренний код объекта (номер по порядку)
// При ошибке возвращает пустую строку

const char * _fastcall imlGetRscObjectKey(HRSC hRsc, long int incode);

//  Запросить название значения характеристики из
//  классификатора семантики по коду семантики и
//  последовательному номеру в классификаторе
//  hRsc - идентификатор классификатора карты
//  code - код семантики
//  number - последовательный номер в классификаторе(1,2,3...)
//  При ошибке возвращает пустую строку

const char * _fastcall imlGetRscSemanticClassificatorName(HRSC hRsc,long int code,
                                                          long int number);

#endif // IMLSERVEX_H
