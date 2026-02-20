/******* simlist.h *************  Belenkov O.V. ******  29/11/21 ***
******** simlist.h *************  Zheleznyakov V.A. **  13/05/21 ***
******** simlist.h *************  Vitsko D.A.   ******  01/08/16 ***
******** simlist.h *************  Derevyagina Zh.A.***  04/05/17 ***
******** simlist.h ************** Sikhnevich V.A.  ***  11/12/18 ***
******** simlist.h ************** Konon V.N.       ***  15/08/19 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2021              *
*                     All Rights Reserved                          *
*                                                                  *
********************************************************************
*                                                                  *
*               СПИСОК ЗАПИСЕЙ ФИКСИРОВАННОГО РАЗМЕРА              *
*                                                                  *
*******************************************************************/

#ifndef SIMLIST_H
#define SIMLIST_H

#ifndef CLASSMACRO_H
 #include "classmacro.h"
#endif

#ifndef MAPTYPE_H
  #include "maptype.h"
#endif

class TBitSetVariable;

typedef void * HLISTITEM;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++  Список идентификаторов памяти  ++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TFastHandleList
{
 public:

  // Создать список
  // reserve - минимальное число элементов
  TFastHandleList(int reserve = 256);

  ~TFastHandleList();

  // Добавить запись
  // item - адрес начала записи
  // При ошибке возвращает ноль, иначе - номер элемента (с 1)
  int _fastcall Append(HLISTITEM item);
  int _fastcall Append(const char *item) { return Append((HLISTITEM)item); }

  // Вставить запись
  // item   - адрес начала записи
  // number - номер вставляемой записи
  // При ошибке возвращает ноль
  int _fastcall Insert(HLISTITEM item, unsigned int number);

  // Проверить - есть ли такой элемент
  // Возвращает номер элемента в списке начиная с 1
  // При ошибке возвращает ноль
  int _fastcall Check(HLISTITEM item);

  // Число элементов
  int _fastcall Count() { return ItemCount; }

  // Очистить список
  void Clear();

  // Обнулить значение элемента
  // number - номер элемента начиная с 1
  // При ошибке возвращает ноль
  int _fastcall Clear(unsigned int number);

  // Удалить элемент
  // number - номер элемента начиная с 1
  void _fastcall Delete(unsigned int number);

  // Удалить элемент
  // item - адрес начала записи
  // При ошибке возвращает ноль
  int _fastcall DeleteByItem(HLISTITEM item);

  // Изменить порядок элемента в списке
  // oldnumber - номер элемента начиная с 1
  // newnumber - новый номер элемента начиная с 1
  int _fastcall SetOrder(unsigned int oldnumber, unsigned int newnumber);

  // Обновить элемент
  // number - номер элемента начиная с 1
  // При ошибке возвращает ноль
  int _fastcall Update(unsigned int number, HLISTITEM item);

  // Запросить элемент
  // number - номер элемента начиная с 1
  // При ошибке возвращает ноль
  HLISTITEM _fastcall Item(unsigned int number);

  // Установить число резервируемых записей при выделении памяти
  void _fastcall SetReserve(int reserve)
  {
    if (reserve <= 0)
      reserve = 256;
    ItemReserve = reserve;
  }

  // Установить число резервируемых записей при выделении памяти
  unsigned int _fastcall GetReserve() { return ItemReserve; }

  // Установить максимальное кол-во элементов в списке
  // count - максимальное кол-во элементов в списке
  // При ошибке возвращает ноль
  int _fastcall SetMaxListElementCount(int count)
  {
    if (count < 0)
      return 0;
    MaxElementCount = count;
    return 1;
  }

  // Запросить адрес списка элементов
  const char ** List() { return (const char **)ItemBegin; }

 protected:

  // Выделить память под список идентификаторов
  int _fastcall Allocate(unsigned int size);

 protected:

  HLISTITEM * ItemBegin;
  HLISTITEM * OldItemBegin;

  unsigned int ItemCount;
  unsigned int MemorySize;
  unsigned int ItemReserve;
  unsigned int MaxElementCount; // максимальное кол-во элементов в листе
}
  TFastHandleList;

typedef class THandleList : public TFastHandleList
{
 public:

  // Создать список
  // reserve - минимальное число элементов
  THandleList(int reserve = 256) : TFastHandleList()
  {
    InitializeTheCriticalSection(&Section);
    IsActive = 1;
    IsFast   = 0;
  }

  ~THandleList()
  {
    if (IsActive != 0)
      DeleteTheCriticalSection(&Section);
    IsActive = 0;
  }

  // Добавить запись
  // item - адрес начала записи
  // При ошибке возвращает ноль, иначе - номер элемента (с 1)
  int _fastcall Append(HLISTITEM item)
  {
    Lock();
    int ret = TFastHandleList::Append(item);
    UnLock();
    return ret;
  }

  // Очистить список
  void Clear()
  {
    Lock();
    TFastHandleList::Clear();
    UnLock();
  }

  // Обнулить значение элемента
  // number - номер элемента начиная с 1
  int _fastcall Clear(unsigned int number)
  {
    Lock();
    int ret = TFastHandleList::Clear(number);
    UnLock();
    return ret;
  }

  // Проверить - есть ли такой элемент
  // Возвращает номер элемента в списке начиная с 1
  // При ошибке возвращает ноль
  int _fastcall Check(HLISTITEM item)
  {
    Lock();
    int ret = TFastHandleList::Check(item);
    UnLock();
    return ret;
  }

  // Удалить элемент
  // number - номер элемента начиная с 1
  void _fastcall Delete(unsigned int number)
  {
    Lock();
    TFastHandleList::Delete(number);
    UnLock();
  }

  // Удалить элемент
  // item - адрес начала записи
  int _fastcall DeleteByItem(HLISTITEM item)
  {
    Lock();
    int ret = TFastHandleList::DeleteByItem(item);
    UnLock();
    return ret;
  }

  // Вставить запись
  // item   - адрес начала записи
  // number - номер вставляемой записи
  // При ошибке возвращает ноль
  int _fastcall Insert(HLISTITEM item, unsigned int number)
  {
    Lock();
    int ret = TFastHandleList::Insert(item, number);
    UnLock();
    return ret;
  }

  // Установить максимальное кол-во элементов в списке
  // count - максимальное кол-во элементов в списке
  // При ошибке возвращает ноль
  int _fastcall SetMaxListElementCount(int count)
  {
    Lock();
    int ret = TFastHandleList::SetMaxListElementCount(count);
    UnLock();
    return ret;
  }

  // Изменить порядок элемента в списке
  // oldnumber - номер элемента начиная с 1
  // newnumber - новый номер элемента начиная с 1
  int _fastcall SetOrder(unsigned int oldnumber, unsigned int newnumber)
  {
    Lock();
    int ret = TFastHandleList::SetOrder(oldnumber, newnumber);
    UnLock();
    return ret;
  }

  // Обновить элемент
  // number - номер элемента начиная с 1
  int _fastcall Update(unsigned int number, HLISTITEM item)
  {
    Lock();
    int ret = TFastHandleList::Update(number, item);
    UnLock();
    return ret;
  }

  // Занять переменные класса
  void Lock()
  {
    if (IsFast)
      return;

    if (IsActive == 0)
    {
      InitializeTheCriticalSection(&Section);
      IsActive = 1;
    }
    EnterTheCriticalSection(&Section);
  }

  // Освободить переменные класса
  void UnLock()
  {
    if (IsFast)
      return;

    LeaveTheCriticalSection(&Section);
  }

 protected:

  int          IsActive;
  unsigned int IsFast;

  MAPCRITICALSECTION Section;
}
  THandleList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++  Список структур фиксированного размера  ++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TFastSimpleListBase
{
 public:

  // Создать список
  // size    - размер элемента списка
  // reserve - минимальное число элементов
  TFastSimpleListBase(unsigned int size, unsigned int reserve = 256);

  ~TFastSimpleListBase() { Clear(); }

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(const char * item, unsigned int size);

  // Число элементов
  unsigned int Count() { return ItemCount; }

  // Очистить список
  void Clear(unsigned int itemsize = 0);

  // Очистить список
  void Close() { Clear(); }

  // Удалить список
  // number - номер элемента, начиная с 1
  int _fastcall Delete(unsigned int number);

  // Удалить элементы из входного списка
  // first - номер первого удаляемого элемента
  // count - число удалаяемых элементов
  // При ошибке возвращает ноль
  int _fastcall Delete(unsigned int first, unsigned int count);

  // Добавить элемент
  // number - номер элемента, в который будет вставлено новое значение
  // item   - адрес начала записи
  // size   - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Insert(unsigned int number, const char * item, unsigned int size);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  char * _fastcall Item(unsigned int number);

  // Установить число резервируемых записей при выделении памяти
  void _fastcall SetReserve(unsigned int reserve) { if (reserve <= 0) reserve = 256; ItemReserve = reserve; }
  unsigned int _fastcall GetReserve() { return ItemReserve; }

  // Запросить размер элемента
  unsigned int GetItemSize() { return ItemSize; }

  // Запросить адрес первого элемента списка
  char * Address() { return ItemBegin; }

  // Выделить буфер для считывания элементов из файла
  // При ошибке возвращает ноль
  char * _fastcall GetMemoryAddresForWrite(unsigned int size);

  // Запросить размер элемента в памяти
  unsigned int GetMemorySize() { return MemorySize; }

  // Скопировать из одного списка в другой элементы
  // При ошибке возвращает ноль
  int _fastcall Load(TFastSimpleListBase& list);

  // Скопировать данные из памяти
  // memory - адрес данных
  // size - размер данных
  // itemcount - количество элементов
  // При ошибке возвращает ноль
  int _fastcall Load(const char* memory, unsigned int size, unsigned int itemcount);

  // Обновить список
  // number - номер элемента, начиная с 1
  int _fastcall UpdateItem(unsigned int number, const char * item);
  int _fastcall Update(unsigned int number, const char * item)
  { return UpdateItem(number, item); }

 protected:

  // Выделить память
  int _fastcall Allocate(unsigned int size);

 protected:

  char * ItemBegin;
#if !defined(_M_X64) && !defined(BUILD_DLL64)
  char * IZero;          // Выравнивание на 8
#endif

  unsigned int ItemCount;
  unsigned int ItemSize;
  unsigned int ItemReserve;
  unsigned int MemorySize;
}
  TFastSimpleListBase;

typedef class TFastSimpleList : public TFastSimpleListBase
{
 public:

  // Создать список
  // size    - размер элемента списка
  // reserve - минимальное число элементов
  TFastSimpleList(unsigned int size, unsigned int reserve = 256) : TFastSimpleListBase(size, reserve) { IsAssign = 0; SZero = 0; }

  ~TFastSimpleList() { Clear(); }

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(const char * item, unsigned int size)
  {
    if (IsAssign != 0)
      return 0;
    return TFastSimpleListBase::Append(item, size);
  }    

  // Добавить массив записей
  // item - адрес первого элемента
  // itemsize - контрольный размер элемента, кратный 4
  // itemcount - количество добавляемых элементов
  // При ошибке возвращает ноль
  unsigned int _fastcall AppendArray(const char * item, unsigned int itemsize, unsigned int itemcount);

  // Назначить классу внешний буфер для поиска по списку
  // item - адрес первого элемента в буфере
  // itemsize - контрольный размер элемента, кратный 4
  // itemcount - количество элементов в буфере
  // При ошибке возвращает ноль
  unsigned int _fastcall AssignArray(const char * item, unsigned int itemsize, unsigned int itemcount);

  // Очистить список
  void Clear(unsigned int size = 0)
  {
    if (IsAssign)
      ItemBegin = 0;
    TFastSimpleListBase::Clear(size);
    IsAssign = 0;
  }

  // Очистить список
  void Close() { Clear(); }

  // Удалить список
  // number - номер элемента, начиная с 1
  int _fastcall Delete(unsigned int number)
  {
    if (IsAssign != 0)
      return 0;
    return TFastSimpleListBase::Delete(number);
  }

  // Удалить элементы из входного списка
  // first - номер первого удаляемого элемента
  // count - число удалаяемых элементов
  // При ошибке возвращает ноль
  int _fastcall Delete(unsigned int first, unsigned int count)
  {
    if (IsAssign != 0)
      return 0;
    return TFastSimpleListBase::Delete(first, count);
  }

  // Добавить элемент
  // number - номер элемента, в который будет вставлено новое значение
  // item   - адрес начала записи
  // size   - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Insert(unsigned int number, const char * item, unsigned int size)
  {
    if (IsAssign != 0)
      return 0;
    return TFastSimpleListBase::Insert(number, item, size);
  }

  // Обновить список
  // number - номер элемента, начиная с 1
  int _fastcall UpdateItem(unsigned int number, const char * item)
  {
    if (IsAssign != 0)
      return 0;
    return TFastSimpleListBase::UpdateItem(number, item);
  }

  int _fastcall Update(unsigned int number, const char * item)
  { return UpdateItem(number, item); }

 protected:

  // Выделить память
  int _fastcall Allocate(unsigned int size)
  {
    if (IsAssign != 0)
      return 0;
    return TFastSimpleListBase::Allocate(size);
  }

 protected:

  int          IsAssign;
  int          SZero;
}
  TFastSimpleList;

typedef class TSimpleList : public TFastSimpleList
{
 public:

  // Создать список
  // size    - размер элемента списка
  // reserve - минимальное число элементов
  TSimpleList(unsigned int size, unsigned int reserve = 256);
  
  ~TSimpleList();

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(const char * item, unsigned int size)
  {
    Lock();
    unsigned int ret = TFastSimpleList::Append(item, size);
    UnLock();
    return ret;
  }

  // Добавить массив записей
  // item - адрес первого элемента
  // itemsize - контрольный размер элемента, кратный 4
  // itemcount - количество добавляемых элементов
  // При ошибке возвращает ноль
  unsigned int _fastcall AppendArray(const char * item, unsigned int itemsize, unsigned int itemcount)
  {
    Lock();
    unsigned int ret = TFastSimpleList::AppendArray(item, itemsize, itemcount);
    UnLock();
    return ret;
  }
  
  // Назначить классу внешний буфер для поиска по списку
  // item - адрес первого элемента в буфере
  // itemsize - контрольный размер элемента, кратный 4
  // itemcount - количество элементов в буфере
  // При ошибке возвращает ноль
  unsigned int _fastcall AssignArray(const char * item, unsigned int itemsize, unsigned int itemcount)
  {
    Lock();
    unsigned int ret = TFastSimpleList::AssignArray(item, itemsize, itemcount);
    UnLock();
    return ret;
  }

  // Очистить список
  void Clear(unsigned int size = 0)
  {
    Lock();
    TFastSimpleList::Clear(size);
    UnLock();
  }

  // Очистить список
  void Close() { Clear(); }
  
  // Удалить список
  // number - номер элемента, начиная с 1
  int _fastcall Delete(unsigned int number)
  {
    Lock();
    unsigned int ret = TFastSimpleList::Delete(number);
    UnLock();
    return ret;
  }

  // Удалить элементы из входного списка
  // first - номер первого удаляемого элемента
  // count - число удалаяемых элементов
  // При ошибке возвращает ноль
  int _fastcall Delete(unsigned int first, unsigned int count)
  {
    Lock();
    unsigned int ret = TFastSimpleList::Delete(first, count);
    UnLock();
    return ret;
  }

  // Добавить элемент
  // number - номер элемента, в который будет вставлено новое значение
  // item   - адрес начала записи
  // size   - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Insert(unsigned int number, const char * item, unsigned int size)
  {
    Lock();
    unsigned int ret = TFastSimpleList::Insert(number, item, size);
    UnLock();
    return ret;
  }

  // Скопировать из одного списка в другой элементы
  // При ошибке возвращает ноль
  int _fastcall Load(TSimpleList& list)
  {
    Lock();
    unsigned int ret = TFastSimpleList::Load(list);
    UnLock();
    return ret;
  }

  // Скопировать данные из памяти
  // memory - адрес данных
  // size - размер данных
  // itemcount - количество элементов
  // При ошибке возвращает ноль
  int _fastcall Load(const char* memory, unsigned int size, unsigned int itemcount)
  {
    Lock();
    unsigned int ret = TFastSimpleList::Load(memory, size, itemcount);
    UnLock();
    return ret;
  }
  
  // Занять переменные класса
  void Lock()
  {
    if (IsFast)
      return;

    if (IsActive == 0)
      {
        InitializeTheCriticalSection(&Section);
        IsActive = 1;
      }
    EnterTheCriticalSection(&Section);
  }

  // Освободить переменные класса
  void UnLock()
  {
    if (IsFast)
      return;

    LeaveTheCriticalSection(&Section);
  }

  // Обновить список
  // number - номер элемента, начиная с 1
  int _fastcall UpdateItem(unsigned int number, const char * item)
  {
    Lock();
    unsigned int ret = TFastSimpleList::UpdateItem(number, item);
    UnLock();
    return ret;
  }

  int _fastcall Update(unsigned int number, const char * item)
  { return UpdateItem(number, item); }

 protected:
  
  MAPCRITICALSECTION Section;
  
  int          IsActive;
  int          IsFast;
}
  TSimpleList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++  Простой несортированный список номеров  ++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TNumberList : public TSimpleList
{
 public:

  // Создать список
  // size    - размер элемента списка
  // reserve - минимальное число элементов
  TNumberList(int reserve = 256) : TSimpleList(4, reserve) {}

  // Добавить запись
  // item - адрес начала записи
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(int item) { return TSimpleList::Append((char*)&item, 4); }

  // Назначить классу внешний буфер для поиска по списку
  // item - адрес первого элемента в буфере
  // itemcount - количество элементов в буфере
  // При ошибке возвращает ноль
  unsigned int _fastcall AssignArray(const int * item, unsigned int itemcount)
  {
    return TSimpleList::AssignArray((const char *)item, 4, itemcount);
  }

  // Очистить список
  void Clear() { TSimpleList::Clear(4); }

  // Добавить элемент
  // number - номер элемента, в который будет вставлено новое значение
  // item   - адрес начала записи
  // При ошибке возвращает ноль
  unsigned int _fastcall Insert(unsigned int number, int item)
  { return TSimpleList::Insert(number, (char*)&item, 4); }

  // Запросить элемент
  // number - номер элемента, начиная с 1
  int _fastcall Item(unsigned int number)
  {
    int * item = (int *)TSimpleList::Item(number);
    if (item == 0)
      return 0;
    return *item;
  }

  // Найти элемент
  // item - значение элемента
  // Возвращает номер записи с 1
  // При ошибке возвращает ноль
  int _fastcall Find(int item);

  // Обновить список
  // number - номер элемента, начиная с 1
  int _fastcall Update(unsigned int number, int item)
  { return TSimpleList::UpdateItem(number, (char*)&item); }

  // Запросить список значений
  int * List() { return (int *)TSimpleList::Item(1); }

  // Скопировать из одного списка в другой элементы
  // При ошибке возвращает ноль
  int _fastcall Load(TNumberList& list) { return TSimpleList::Load(list); }

  // Скопировать данные из памяти
  // memory - адрес данных
  // size - размер данных
  // itemcount - количество элементов
  // При ошибке возвращает ноль
  int _fastcall Load(const int * memory, unsigned int itemcount)
  {
    return TSimpleList::Load((const char *)memory, itemcount * 4, itemcount);
  }
}
  TNumberList;

typedef class TFastNumberList : public TNumberList
{
 public:

  // Создать список
  // size    - размер элемента списка
  // reserve - минимальное число элементов
  TFastNumberList(int reserve = 256) : TNumberList(reserve) { IsFast = 1; }
}
  TFastNumberList;

typedef class TNumber64List : public TSimpleList
{
 public:

  // Создать список
  // size    - размер элемента списка
  // reserve - минимальное число элементов
  TNumber64List(int reserve = 256) : TSimpleList(8, reserve) {}

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(__int64 item) { return TSimpleList::Append((char*)&item, 8); }

  // Назначить классу внешний буфер для поиска по списку
  // item - адрес первого элемента в буфере
  // itemcount - количество элементов в буфере
  // При ошибке возвращает ноль
  unsigned int _fastcall AssignArray(const int * item, unsigned int itemcount)
  {
    return TSimpleList::AssignArray((const char *)item, 8, itemcount);
  }

  // Добавить элемент
  // number - номер элемента, в который будет вставлено новое значение
  // item   - адрес начала записи
  // size   - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Insert(unsigned int number, __int64 item)
  {
    return TSimpleList::Insert(number, (char*)&item, 8);
  }

  // Запросить элемент
  // number - номер элемента, начиная с 1
  __int64 _fastcall Item(unsigned int number)
  {
    __int64 * item = (__int64 *)TSimpleList::Item(number);
    if (item == 0) return 0;
    return *item;
  }

  // Обновить список
  // number - номер элемента, начиная с 1
  int _fastcall Update(unsigned int number, __int64 item)
  {
    return TSimpleList::Update(number, (char*)&item);
  }

  // Запросить список значений
  __int64 * List() { return (__int64 *)TSimpleList::Item(1); }

}
  TNumber64List;


typedef class TDoubleList : public TSimpleList
{
 public:

  // Создать список
  // size    - размер элемента списка
  // reserve - минимальное число элементов
  TDoubleList(int reserve = 256) : TSimpleList(8, reserve) {}

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(double item) { return TSimpleList::Append((char*)&item, 8); }

  // Добавить элемент
  // number - номер элемента, в который будет вставлено новое значение
  // item   - адрес начала записи
  // size   - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Insert(unsigned int number, double item)
  { return TSimpleList::Insert(number, (char*)&item, 8); }

  // Запросить элемент
  // number - номер элемента, начиная с 1
  double _fastcall Item(unsigned int number)
  { double * item = (double *)TSimpleList::Item(number); if (item == 0) return 0; return *item; }

  // Обновить список
  // number - номер элемента, начиная с 1
  int _fastcall Update(unsigned int number, double item)
  { return TSimpleList::Update(number, (char*)&item); }

  // Запросить список значений
  double * List() { return (double *)TSimpleList::Item(1); }

}
  TDoubleList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++++++++++++ Список отсортированных номеров ++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TNumberListOrder : public TSimpleList
{
 public:

  // Создать список
  // size    - размер элемента списка
  TNumberListOrder(unsigned int reserve = 256) : TSimpleList(4, reserve) {}

  // Добавить номер в список
  // item - значение элемента (записи)
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(int item);

  // Добавить номер в список, если такого номера не было
  // item - значение элемента (записи)
  // При ошибке возвращает ноль
  unsigned int _fastcall AppendUnique(int item)
  {
    if (Find(item))
      return 0;
    return Append(item);
  }

  // Назначить классу внешний буфер для поиска по списку
  // item - адрес первого элемента в буфере
  // itemcount - количество элементов в буфере
  // При ошибке возвращает ноль
  unsigned int _fastcall AssignArray(const int * item, unsigned int itemcount)
  {
    return TSimpleList::AssignArray((const char *)item, 4, itemcount);
  }

  // Найти запись по значению
  // Возвращает номер записи
  // При ошибке возвращает ноль и заполняет номер первого элемента, который больше запрошенного
  unsigned int Check(int item, int& next);

  // Найти запись по значению
  // item - значение элемента
  // Возвращает номер записи с 1
  // При ошибке возвращает ноль
  unsigned int Find(int item)
  {
    int next;
    return Check(item, next);
  }

  // Запросить элемент
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  unsigned int _fastcall Item(unsigned int number)
  {
    unsigned int * item = (unsigned int *)TSimpleList::Item(number);
    if (item == 0)
      return 0;
    return *item;
  }

  // Запросить список значений
  // При ошибке возвращает ноль
  int * List() { return (int *)TSimpleList::Item(1); }

  // Скопировать из одного списка в другой элементы
  // При ошибке возвращает ноль
  int _fastcall Load(TNumberListOrder& list, unsigned int first, unsigned int count);
  int _fastcall Load(TNumberListOrder& list)
  {
    Clear();
    return Load(list, 1, list.Count());
  }

  // Загрузить данные
  // firstelement - указатель на массив отсортированных данных
  // count - количество элементов массива
  // При ошибке возвращает ноль
  int _fastcall Load(const char* firstelement, unsigned int count);

  // Скопировать из одного списка в другой элементы и удалить в исходном списке
  // При ошибке возвращает ноль
  int _fastcall LoadAndRemove(TNumberListOrder& list, unsigned int first, unsigned int count);
}
  TNumberListOrder;

typedef class TFastNumberListOrder : public TNumberListOrder
{
 public:

  // Создать список
  // size    - размер элемента списка
  TFastNumberListOrder(unsigned int reserve = 256) : TNumberListOrder(reserve) { IsFast = 1; }

}
  TFastNumberListOrder;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++ Вспомогательный класс внутренних списков элементов +++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TBigNumberList : protected THandleList
{
 public:

  TBigNumberList(unsigned int reserve = 16, unsigned int limit = 32767);
  ~TBigNumberList() { Clear(); }

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (при несовпадении с size - ошибка)
  // data - значение поля для сортировки по возрастанию значения
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(unsigned int value);

  // Очистить список
  void _fastcall Clear();

  // Запросить число элементов в списке
  unsigned int _fastcall Count();

  // Найти запись по значению
  // Возвращает номер записи
  // При ошибке возвращает ноль и заполняет номер первого элемента, который больше запрошенного
  unsigned int _fastcall Check(unsigned int value, int& next);

  // Удалить запись по значению поля
  unsigned int _fastcall DeleteByValue(unsigned int value);

  // Найти запись по значению
  // item - значение элемента
  // Возвращает номер записи с 1
  // При ошибке возвращает ноль
  unsigned int _fastcall Find(unsigned int item)
  {
    int next;
    return Check(item, next);
  }

  // Запросить элемент
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  unsigned int _fastcall Item(unsigned int number);

  // Скопировать из одного списка в другой элементы
  // При ошибке возвращает ноль
  int _fastcall Load(TBigNumberList& list);

  // Запросить число частей в списке
  int _fastcall GetFrameCount() { return Numbers.Count(); }

  // Запросить начальные значения частей списка
  int _fastcall GetFrameValue(unsigned int index) { return Numbers.Item(index); }

  // Запросить предельный размер части списка
  unsigned int GetLimit() { return Limit; }

 protected:

  TFastNumberList  Numbers;
  unsigned int     Limit;
  unsigned int     Zero;
}
  TBigNumberList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++ Список отсортированных структур фиксированного размера
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TSimpleListOrder : public TSimpleList
{
 public:

  // Конструктор класса
  // Размер структуры должен быть кратен 8
  TSimpleListOrder(unsigned int size, unsigned int reserve = 256, int isrepeat = 1);

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (как в конструкторе)
  // data - значение поля для сортировки по возрастанию значения
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(const char * item, int size, BIGINT data)
  {
    Lock();
    unsigned int ret = AppendInside(item, size, data);
    UnLock();
    return ret;
  }

  // Число элементов
  unsigned int Count() { return TSimpleList::Count(); }

  // Очистить список
  void Clear(unsigned int size = 0) { TSimpleList::Clear(size); }

  // Очистить список
  void Close() { Clear(); }

  // Удалить список
  // number - номер элемента, начиная с 1
  int _fastcall Delete(unsigned int number) { return TSimpleList::Delete(number); }

  // Удалить элементы из входного списка
  // first - номер первого удаляемого элемента
  // count - число удалаяемых элементов
  // При ошибке возвращает ноль
  int _fastcall Delete(unsigned int first, unsigned int count) { return TSimpleList::Delete(first, count); }

  // Найти запись по значению поля для сортировки
  const char * FindItemByData(BIGINT data, unsigned int& number);
  const char * FindItemByData(BIGINT data)
  {
    unsigned int number;
    return FindItemByData(data, number);
  }

  // Добавить элемент
  // number - номер элемента, в который будет вставлено новое значение
  // item   - адрес начала записи
  // size   - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Insert(unsigned int number, const char * item, unsigned int size)
  {
    Lock();
    unsigned int ret = InsertInside(number, item, size);
    UnLock();
    return ret;
  }

  // Запросить вес элемента
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  BIGINT _fastcall ItemData(unsigned int number);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  char * _fastcall Item(unsigned int number, BIGINT& data);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  char * _fastcall Item(unsigned int number) { return TSimpleList::Item(number); }

  // Скопировать из одного списка в другой элементы и удалить в исходном списке
  // При ошибке возвращает ноль
  unsigned int _fastcall LoadAndRemove(TSimpleListOrder& list, unsigned int first, unsigned int count);

  // Установить число резервируемых записей при выделении памяти
  void _fastcall SetReserve(unsigned int reserve) { TSimpleList::SetReserve(reserve); }
  unsigned int _fastcall GetReserve() { return TSimpleList::GetReserve(); }

 private:

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(const char * item, unsigned int size) { return 0; }

  unsigned int _fastcall AppendInside(const char * item, int size, BIGINT data);

  // Добавить элемент
  // number - номер элемента, в который будет вставлено новое значение
  // item   - адрес начала записи
  // size   - контрольный размер (при несовпадении с size - ошибка)
  // При ошибке возвращает ноль
  unsigned int _fastcall InsertInside(unsigned int number, const char * item, unsigned int size);

 protected:

  int IsRepeat;
  int Zero;
}
  TSimpleListOrder;

typedef class TFastSimpleListOrder : public TSimpleListOrder
{
 public:

  // Конструктор класса
  // Размер структуры должен быть кратен 8
  TFastSimpleListOrder(unsigned int size, unsigned int reserve = 256, int isrepeat = 1) :
   TSimpleListOrder(size, reserve = 256, isrepeat = 1) { IsFast = 1; }
}
  TFastSimpleListOrder;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++ Список отсортированных по весам уникальных номеров ++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TUniqueInt64ListOrder : public TSimpleList
{
 public:

  // Конструктор класса
  TUniqueInt64ListOrder(unsigned int reserve = 256);

  // Добавить запись
  // item - адрес начала записи
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(__int64 item);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  __int64 _fastcall Item(unsigned int number)
  { __int64 * item = (__int64 *)TSimpleList::Item(number); if (item == 0) return 0; return *item; }

  // Скопировать из одного списка в другой элементы и удалить в исходном списке
  // При ошибке возвращает ноль
  unsigned int _fastcall LoadAndRemove(TUniqueInt64ListOrder& list, unsigned int first, unsigned int count);
}
  TUniqueInt64ListOrder;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++ Список отсортированных по весам номеров +++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct INT32ITEM
{
  unsigned int item;
  unsigned int data;
}
  INT32ITEM;

typedef class TInt32ListOrder : public TSimpleList
{
 public:

  // Конструктор класса
  // isrepeat - признак допустимости повтора значения весов
  TInt32ListOrder(unsigned int reserve = 256);

  // Добавить запись
  // item - значение элемента
  // data - значение поля (веса элемента) для сортировки по возрастанию значения
  // isfast - добавить в конец списка
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(unsigned int item, unsigned int data, int isfast = 0);

  // Удалить запись
  // item - значение элемента
  // data - значение поля (веса элемента) для сортировки по возрастанию значения
  // При ошибке возвращает ноль
  unsigned int _fastcall DeleteItem(unsigned int item, unsigned int data);

  // Найти номер элемента по его значению
  // item - значение элемента
  // Возвращает номер записи с 1
  // При ошибке возвращает ноль
  unsigned int _fastcall Find(unsigned int item, unsigned int data);

  // Найти запись по значению поля для сортировки (веса элемента)
  // Возвращает значение элемента
  // Если совпадения нет, то возвращает ноль и устанавливает параметр index
  // номером элемента, вес которого больше заданного
  // При ошибке возвращает ноль
  unsigned int _fastcall FindItemByData(unsigned int data, unsigned int * index = 0);

  // Запросить вес элемента
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  unsigned int _fastcall ItemData(unsigned int number);

  // Запросить значение элемента
  // number - номер элемента, начиная с 1
  unsigned int _fastcall Item(unsigned int number, unsigned int& data);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  unsigned int _fastcall Item(unsigned int number)
  {
    unsigned int * item = (unsigned int *)TSimpleList::Item(number);
    if (item == 0)
      return 0;
    return *item;
  }

  // Заполнить список из буфера
  // При ошибке возвращает ноль
  unsigned int _fastcall Load(const char * block, unsigned int size);

  // Скопировать из одного списка в другой элементы и удалить в исходном списке
  // При ошибке возвращает ноль
  unsigned int _fastcall LoadAndRemove(TInt32ListOrder& list, unsigned int first, unsigned int count);
}
  TInt32ListOrder;

typedef class TFastInt32ListOrder : public TInt32ListOrder
{
 public:

  // Конструктор класса
  // isrepeat - признак допустимости повтора значения весов
  TFastInt32ListOrder(unsigned int reserve = 256) : TInt32ListOrder(reserve) { IsFast = 1; }
}
  TFastInt32ListOrder;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++ Список отсортированных по весам номеров и флагов ++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TInt32ListOrderEx : protected TInt32ListOrder
{
 public:

  // Конструктор класса
  TInt32ListOrderEx() : TInt32ListOrder(), Flags(2) {}

  // Добавить запись
  // item - значение элемента
  // data - значение поля для сортировки по возрастанию значения
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(unsigned int item, unsigned char iflag,
                                unsigned int data, unsigned char dflag);

  // Очистить список
  void Clear();

  // Найти запись по значению поля для сортировки
  unsigned int _fastcall FindItemByData(unsigned int data, unsigned char dflag,
                                        unsigned char& iflag);

 protected:

  TFastSimpleList Flags;

}
  TInt32ListOrderEx;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++ Список отсортированных структур фиксированного размера
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TBigSimpleListOrder : protected THandleList
{
 public:

  // Конструктор
  TBigSimpleListOrder(unsigned int size, unsigned int reserve = 64, unsigned int limit = 32767);
  ~TBigSimpleListOrder() { Clear(); }

  // Число элементов
  int _fastcall Count();

  // Очистить список
  void Clear();

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер
  // data - значение поля для сортировки по возрастанию значения
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(const char * item, int size, unsigned int data);

  // Найти запись по значению поля для сортировки
  const char * FindItemByData(unsigned int data);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  const char * _fastcall Item(unsigned int number);

 protected:

  TFastNumberList  Numbers;
  unsigned int     Limit;
  unsigned int     Size;
}
  TBigSimpleListOrder;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++ Список отсортированных по весу номеров +++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TNumberListIndexOrder : protected THandleList
{
 public:

  // Создать список
  TNumberListIndexOrder(unsigned int reserve = 64, unsigned int limit = 32*1024);
  ~TNumberListIndexOrder() { Clear(); }

  // Добавить запись
  // item - адрес начала записи
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(unsigned int item, unsigned int data, int isfast = 0);

  // Добавить блок записей
  // point - указатель на блок
  // size  - размер блока
  // При ошибке возвращает ноль
  unsigned int _fastcall AppendBlock(const char * point, unsigned int size);

  // Число элементов
  int _fastcall Count();

  // Очистить список
  void Clear();

  // Удалить объект в списке
  // При ошибке возвращает ноль
  int _fastcall Delete(unsigned int item, unsigned int data);

  // Найти номер элемента по его значению
  // item - значение элемента
  // Возвращает номер записи с 1
  // При ошибке возвращает ноль
  unsigned int _fastcall Find(unsigned int item, unsigned int data);

  // Найти запись по значению поля для сортировки
  unsigned int _fastcall FindItemByData(unsigned int data);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  unsigned int _fastcall Item(unsigned int number);

  // Запросить элемент в цикле перебора элементов
  // number - номер элемента, начиная с 1
  // block  - номер блока от предыдущего запроса или ноль
  // firstnumber - первый номер блока от предыдущего запроса или ноль
  // При ошибке возвращает ноль
  unsigned int _fastcall FastItem(unsigned int number, unsigned int& block, unsigned int& firstnumber);
  unsigned int _fastcall PrevItem(unsigned int number, unsigned int& block, unsigned int& firstnumber);
  unsigned int _fastcall NextItem(unsigned int number, unsigned int& block, unsigned int& firstnumber)
  {
    if (number == 0)
      return 0;
    return FastItem(number+1, block, firstnumber);
  }

  // Запросить число блоков элементов
  unsigned int GetBlockCount() { return TFastHandleList::Count(); }

  // Запросить максимальный размер одного списка для записи в файл
  // При ошибке возвращает ноль
  unsigned int GetBlockSize() { return (Limit * 8); }

  // Запросить адрес буфера списка по номеру
  // При ошибке возвращает ноль
  const char * GetBlockAddressByNumber(unsigned int number);

  // Запросить размер списка по номеру для записи в файл
  // При ошибке возвращает ноль
  unsigned int GetBlockSizeByNumber(unsigned int number);

  // Запросить размер списка в Кб
  unsigned int GetSize();

 protected:

  TFastNumberList  Numbers;
  unsigned int     Limit;
  unsigned int     Zero;
}
  TNumberListIndexOrder;

typedef class TFastNumberListIndexOrder : public TNumberListIndexOrder
{
 public:

  // Создать список
  TFastNumberListIndexOrder(unsigned int reserve = 64, unsigned int limit = 32*1024) :
      TNumberListIndexOrder(reserve, limit) { IsFast = 1; }
}
  TFastNumberListIndexOrder;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++ Список отсортированных по весу уникальных номеров +++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TUniqueNumberList : protected THandleList
{
 public:

  typedef union ITEM
  {
    __int64 value;
    struct
    {
      unsigned int item;
      unsigned int data;
    }
      record;
  }
   ITEM;

  // Создать список
  // size    - размер элемента списка
  TUniqueNumberList(unsigned int reserve = 64, unsigned int limit = 16*1024);
  ~TUniqueNumberList() { Clear(); }

  // Число элементов
  int _fastcall Count();

  // Очистить список
  void Clear();

  // Добавить запись
  // item - адрес начала записи
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(unsigned int item, unsigned int data);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  unsigned int _fastcall Item(unsigned int number);

 protected:

  TNumber64List  Numbers;
  unsigned int   Limit;
  unsigned int   Zero;
}
  TUniqueNumberList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++ Вспомогательный класс внутренних списков элементов +++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

class TKeyListOrder;

typedef class TKeyTreeList : protected TFastHandleList
{
 public:

  TKeyTreeList(unsigned int reserve = 8);
  ~TKeyTreeList();

  // Добавить запись
  // При ошибке возвращает ноль, иначе - номер элемента (с 1)
  unsigned int _fastcall Append();

  // Очистить списки
  void Clear();

  // Запросить резерв на расширение списка
  int GetReserve() { return TFastHandleList::GetReserve(); }

  // Запросить список номеров
  // number - номер списка, начиная с 1
  TKeyListOrder * _fastcall Item(unsigned int number);

  // Запросить число классов в списке
  // При ошибке возвращает ноль
  int TreeCount() { return TFastHandleList::Count(); }

  // Установить число резервируемых записей при выделении памяти
  void _fastcall SetReserve(int reserve) { TFastHandleList::SetReserve(reserve); }
}
  TKeyTreeList;

typedef class TKeyListOrder : protected TInt32ListOrder
{
 public:

  TKeyListOrder(unsigned int reserve = 256);
  ~TKeyListOrder();

  // Добавить запись
  // item - адрес начала записи
  // size - контрольный размер (при несовпадении с size - ошибка)
  // data - значение поля для сортировки по возрастанию значения
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(unsigned int number, unsigned int key);

  // Очистить списки ключей
  // При ошибке возвращает ноль
  int Clear();

  // Найти запись по значению поля для сортировки
  unsigned int _fastcall FindItemByKey(unsigned int key);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  unsigned int _fastcall Item(unsigned int number);

  // Установить планируемые границы списка
  int _fastcall SetBorder(unsigned int keymin, unsigned int keymax, unsigned int count = 0);

  // Запросить число классов в списке
  // При ошибке возвращает ноль
  int TreeCount() { int count = 0; if (TInt32ListOrder::Count()) count++; if (Step) count += TreeList.TreeCount(); return count; }

  // Обновить номер объекта в листе по ключу
  // number - номер объекта в листе
  // key - идентификатор объекта
  // При ошибке возвращает ноль
  unsigned int _fastcall Update(unsigned int number, unsigned int key);

 public:

  int *        KeyList;               // Массив прямого доступа по ключу
#if !defined(_M_X64) && !defined(BUILD_DLL64)
  char *       KZero;                 // Выравнивание на 8
#endif

  unsigned int Min;
  unsigned int Max;
  unsigned int BorderMin;
  unsigned int BorderMax;
  unsigned int Step;
  unsigned int KeyCount;

  TKeyTreeList TreeList;
}
  TKeyListOrder;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++ Список блоков номеров объектов в листе ++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TInt32List : protected TFastHandleList
{
 public:

  // Создать список
  // size    - размер элемента списка
  TInt32List(unsigned int size = 32 * 1024);

  ~TInt32List();

  // Добавить запись
  // item - значение элемента
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(unsigned int item);

  // Добавить блок записей
  // point - указатель на блок
  // size  - размер блока
  // При ошибке возвращает ноль
  unsigned int _fastcall AppendBlock(unsigned int * point, unsigned int size);

  // Добавить элемент
  // number - номер элемента, в который будет вставлено новое значение
  // item   - адрес начала записи
  // При ошибке возвращает ноль
  unsigned int _fastcall Insert(unsigned int number, unsigned int item);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  unsigned int _fastcall Item(unsigned int number);

  // Запросить элемент в цикле перебора элементов
  // number - номер элемента, начиная с 1
  // block  - номер блока от предыдущего запроса или ноль
  // firstnumber - первый номер блока от предыдущего запроса или ноль
  // При ошибке возвращает ноль
  unsigned int _fastcall FastItem(unsigned int number, unsigned int& block, unsigned int& firstnumber);

  // Очистить список элементов
  void Clear(unsigned int size = 0);

  // Запросить число элементов в списке
  unsigned int Count() { return IntCount; }

  // Найти запись по значению
  // item - значение элемента
  // Возвращает номер записи с 1
  // При ошибке возвращает ноль
  unsigned int _fastcall Find(unsigned int item, int issorted);

  // Запросить число блоков элементов
  unsigned int GetBlockCount() { return TFastHandleList::Count(); }

  // Запросить адрес блока элементов по номеру с 1
  // При ошибке возвращает ноль
  const char * _fastcall GetBlockAddressByNumber(unsigned int number);

  // Запросить размер блока
  unsigned int GetBlockSize() { return BlockSize; }

  // Обновить элемент
  // number - номер элемента, начиная с 1
  // value - новое значение
  // При ошибке возвращает 0
  unsigned int _fastcall Update(unsigned int number, unsigned int item);

  // Запросить размер списка в Кб
  unsigned int GetSize();

  // Заполнить битовую область по списку номеров объектов
  // bits - битовая область, в которой выставляются 1 по номерам объектов
  // firstnumber - номер объекта, соответствующий нулевому биту
  // При ошибке возвращает 0
  int _fastcall SetBitArea(TBitSetVariable& bits, int& firstnumber);

 protected:

  // Добавить новый блок элементов
  // При ошибке возвращает ноль
  unsigned int _fastcall AllocateBlock();

  // Установить размер блока данных
  void SetBlockSize(unsigned int size);

 protected:

  unsigned int BlockSize;
  unsigned int IntCount;
  unsigned int FreeCount;
  unsigned int BlockCount;

  unsigned int LastCount;
  unsigned int IsSorted;
}
  TInt32List;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++ Список габаритов объектов/подобъектов +++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TFrameList : public TSimpleList
{
  public:

  TFrameList(unsigned int reserve = 256) : TSimpleList(sizeof(FRAME), reserve) {}

  // Добавить запись
  // frame - добавляемая запись габаритов
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(FRAME& frame) { return TSimpleList::Append((char*)&frame, sizeof(FRAME)); }
  unsigned int _fastcall Append(DFRAME& dframe)
  {
    FRAME frame;
    DframeToFrame(dframe, frame);
    return Append(frame);
  }

  // Проверить элемент на пересечение с заданными габаритами
  // number - номер проверяемого элемента
  // test - проверяемые габариты
  // При отсутствии пересечения возвращает ноль
  unsigned int _fastcall CheckCross(unsigned int number, FRAME& test)
  {
    FRAME* item = Item(number);
    if (item == 0)
      return 0;

    if (item->X1 > test.X2)
      return 0;
    if (item->X2 < test.X1)
      return 0;
    if (item->Y1 > test.Y2)
      return 0;
    if (item->Y2 < test.Y1)
      return 0;

    return 1;
  }
  unsigned int _fastcall CheckCross(unsigned int number, DFRAME& dtest)
  {
    FRAME test;
    DframeToFrame(dtest, test);
    return CheckCross(number, test);
  }

  // Преобразовать запись
  // dframe - исходная запись габаритов
  // frame - округленная запись габаритов
  // При ошибке возвращает ноль
  void _fastcall DframeToFrame(DFRAME& dframe, FRAME& frame)
  {
    if (dframe.X1 < 0)
    {
      double temp = dframe.X1 - 1;
      frame.X1 = CHECK_NEGINT(temp);
    }
    else
      frame.X1 = CHECK_POSINT(dframe.X1);
    if (dframe.Y1 < 0)
    {
      double temp = dframe.Y1 - 1;
      frame.Y1 = CHECK_NEGINT(temp);
    }
    else
      frame.Y1 = CHECK_POSINT(dframe.Y1);

    double temp = dframe.X2 + 1;
    frame.X2 = CHECK_INT(temp);
    temp = dframe.Y2 + 1;
    frame.Y2 = CHECK_INT(temp);
  }

  // Запросить габариты элемента
  // number - номер элемента, начиная с 1
  FRAME* _fastcall Item(unsigned int number) { return (FRAME*)TSimpleList::Item(number); }

  // Запросить список подобъектов, с которыми пересекается заданная рамка
  // Возвращает число добавленных записей в списке
  unsigned int _fastcall GetItemList(FRAME& test, TFastNumberListOrder& list)
  {
    int findcount = 0;

    // Перебрать все элементы в списке
    int count = Count();
    for (int k = 1; k <= count; k++)
    {
      if (CheckCross(k, test))
      {
        // Подобъекты нумеруются с нуля
        if (list.AppendUnique(k - 1))
          findcount++;
      }
    }

    return findcount;
  }

  unsigned int _fastcall GetItemList(DFRAME& dtest, TFastNumberListOrder& list)
  {
    FRAME test;
    DframeToFrame(dtest, test);
    return GetItemList(test, list);
  }

  // Установить число резервируемых записей при выделении памяти
  void _fastcall SetReserve(int reserve) { TSimpleList::SetReserve(reserve); }
}
 TFrameList;

typedef class TFastFrameList : public TFrameList
{
 public:

   TFastFrameList(unsigned int reserve = 256) : TFrameList(reserve) { IsFast = 1; }
}
 TFastFrameList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++ Список блоков точек объектов в листе ++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TLongPointList : protected TInt32List
{
 public:

  // Создать список
  // size    - размер элемента списка
  TLongPointList(unsigned int size = 128 * 1024) : TInt32List(size) {}

  // Выделить место под список точек
  // count - число точек
  // При ошибке возвращает ноль
  LONGPOINT * _fastcall GetBuffer(unsigned int count);

  // Очистить список элементов
  void Clear() { return TInt32List::Clear(); }

  // Запросить число элементов в списке
  unsigned int Count() { return IntCount; }
}
  TLongPointList;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++   Список нумерованных названий  ++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct IDENTNAME
{
  int   Ident;
  int   Zero;
  WCHAR Name[256];
}
  IDENTNAME;

typedef class TIdentNames : public TSimpleList
{
  public:

  // Конструктор
  TIdentNames() : TSimpleList(sizeof(IDENTNAME)) {}

  // Деструктор
  ~TIdentNames() {}

  // Добавить запись
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(IDENTNAME * item)
  {
    return TSimpleList::Append((char*)item, sizeof(IDENTNAME));
  }

  // Запросить элемент по порядковому номеру в списке
  // number - номер элемента, начиная с 1
  IDENTNAME * _fastcall Item(unsigned int number)
  {
    return (IDENTNAME *)TSimpleList::Item(number);
  }

  // Найти запись по идентификатору элемента
  IDENTNAME * _fastcall Find(unsigned int id);

  // Найти запись по идентификатору элемента
  unsigned int _fastcall FindNumber(unsigned int id);

  // Запросить указатель на имя слоя по идентификатору элемента
  const WCHAR * _fastcall FindName(unsigned int id)
  {
    IDENTNAME * item = Find(id);
    if (item == 0) return 0;
    return item->Name;
  }
}
  TIdentNames;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++++++++   Список нумерованных названий  ++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct IDENTNAMEA
{
  int   Ident;
  int   Zero;
  char  Name[256];
}
  IDENTNAMEA;

typedef class TIdentNamesA : public TSimpleList
{
  public:

  // Конструктор
  TIdentNamesA() : TSimpleList(sizeof(IDENTNAME)) {}

  // Деструктор
  ~TIdentNamesA() {}

  // Добавить запись
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(IDENTNAMEA * item)
  {
    return TSimpleList::Append((char*)item, sizeof(IDENTNAMEA));
  }

  // Запросить элемент по порядковому номеру в списке
  // number - номер элемента, начиная с 1
  IDENTNAMEA * _fastcall Item(unsigned int number)
  {
    return (IDENTNAMEA *)TSimpleList::Item(number);
  }

  // Найти запись по идентификатору элемента
  IDENTNAMEA * _fastcall Find(unsigned int id);

  // Найти запись по идентификатору элемента
  unsigned int _fastcall FindNumber(unsigned int id);

  // Запросить указатель на имя по идентификатору элемента
  const char * _fastcall FindName(unsigned int id)
  {
    IDENTNAMEA * item = Find(id);
    if (item == 0) return 0;
    return item->Name;
  }
}
  TIdentNamesA;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// +++ Список наклонных прямоугольников с габаритами +++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef struct ANYRECT        // 32 байта
{
  FRAME       Frame;          // Габариты элемента в мкм
  LONGPOINT * Points;         // Точки элемента в мкм
  int         Count;          // Число точек
  int         Zero;
}
  ANYRECT;

typedef class TRectListOrder : public TSimpleListOrder
{
  public:

  TRectListOrder(unsigned int reserve = 256, int isrepeat = 0) :
                 TSimpleListOrder(sizeof(ANYRECT), reserve, isrepeat) {}

  // Деструктор
  ~TRectListOrder() { Clear(); }

  // Добавить запись
  // При ошибке возвращает ноль
  unsigned int _fastcall Append(ANYRECT& item, BIGINT data)
  {
    return TSimpleListOrder::Append((char*)&item, sizeof(ANYRECT), data);
  }

  // Найти запись по значению поля для сортировки
  // При ошибке возвращает ноль
  ANYRECT * FindItemByData(BIGINT data)
  {
    return (ANYRECT *)TSimpleListOrder::FindItemByData(data);
  }

  // Запросить элемент
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  ANYRECT * _fastcall Item(unsigned int number, BIGINT& data)
  {
    return (ANYRECT *)TSimpleListOrder::Item(number, data);
  }
}
  TRectListOrder;

class TSheetRectList;

typedef class TRectList
{
  public:

  // Конструктор
  TRectList(FRAME& frame, unsigned int level, unsigned int maxlevel, TSheetRectList& list);
  TRectList(unsigned int level, unsigned int maxlevel);

  // Деструктор
  ~TRectList() { Clear(); }

  // Добавить запись в дерево объектов
  // Если габариты не пересекаются - возвращает ноль
  int _fastcall Append(ANYRECT& item, BIGINT data, TSheetRectList& list);

  // Сформировать список объектов, попадающих в выбранную область по габаритам
  // Если габариты не пересекаются - возвращает ноль
  int _fastcall GetList(TRectListOrder ** list, FRAME& frame);

  // Сформировать список записей, попадающих в выбранную область по габаритам
  // Если габариты не пересекаются - возвращает ноль
//  int _fastcall GetItemList(TRectListOrder ** list, FRAME& frame);

  // Запросить элемент
  // number - номер элемента, начиная с 1
  // При ошибке возвращает ноль
  ANYRECT * _fastcall Item(unsigned int number, BIGINT& data);

  // Очистить список элементов
  void Clear();

  // Запросить число элементов в списке
  unsigned int Count() { return ItemCount; }

  // Установить границы области списка
  void SetFrame(FRAME& frame, TSheetRectList& list);

  protected:

#ifndef HIDEMAPAPI
  // Добавить запись в дерево объектов
  // Если габариты не пересекаются - возвращает ноль
  int _fastcall Append(HOBJ info, int subject,
                       FRAME& frame, TSheetRectList& list);
#endif

  protected:

  DFRAME             DFrame;         // Габариты элемента в м
  FRAME              Frame;

  TRectListOrder     List;

  TRectList        * Item1;
  TRectList        * Item2;
  TRectList        * Item3;
  TRectList        * Item4;

  int                CX;
  int                CY;

  unsigned int       Level;
  unsigned int       MaxLevel;
  unsigned int       ItemCount;
  unsigned int       Zero;
}
  TRectList;

typedef class TSheetRectList : public TRectList
{
  public:

  // Конструктор списка рамок
  TSheetRectList(DFRAME& frame, long int scale, unsigned int maxlevel);
  ~TSheetRectList() { PointList.Clear(); }

  // Добавить запись в дерево объектов
  // Если габариты не пересекаются - возвращает ноль
  int _fastcall Append(const DOUBLEPOINT * dpoint, int count, BIGINT number);

#ifndef HIDEMAPAPI
  // Добавить запись в дерево объектов (для линейных объектов)
  // Если габариты не пересекаются - возвращает ноль
  int _fastcall Append(HOBJ info);

  // Добавить список выделенных объектов в дерево
  // При ошибке возвращает ноль
  int _fastcall AppendObjectList(HMAP hmap, HSITE hsite, int list, HSELECT hselect,
                                 int flag = WO_VISUAL | WO_VISUALSCALE | WO_INMAP);
#endif

  // Пересчитать габариты в метрах на местности к микронам на карте
  void _fastcall PlaneToMkm(DFRAME& dframe, FRAME& frame);

  // Пересчитать точку в метрах на местности к микронам на карте
  void _fastcall PlaneToMkm(const DOUBLEPOINT& dpoint, LONGPOINT& point);

  // Пересчитать точку в микронах на карте к метрам на местности
  void _fastcall MkmToPlane(LONGPOINT& point, DOUBLEPOINT& dpoint);

  // Пересчитать габариты в микронах на карте к метрам на местности
  void _fastcall MkmToPlane(FRAME& frame, DFRAME& dframe);

  // Запросить число необработанныъ объектов
  int ErrorCount() { return Errors; }

  // Заполнить содержимое элемента
  int _fastcall SetItem(ANYRECT& item, const DOUBLEPOINT * dpoint, int count);

  // Добавить в буфер точек отдельную точку
  LONGPOINT * _fastcall AppendPoint(LONGPOINT& point);

  protected:

  // Коэффициенты для перехода к микронам на карте
  // Multi = 1000000/scale;
  // Xm = (X - SheetX) * Multi;
  // Ym = (Y - SheetY) * Multi;
  double Multi;

  int    SheetX;                 // Юго-Западный угол листа кратно 100 метрам
  int    SheetY;
  int    Errors;
  int    Total;

  TLongPointList   PointList;
}
  TSheetRectList;

#endif // SIMLIST_H
