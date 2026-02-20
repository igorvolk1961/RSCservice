/***** mapbits.h ***************  Belenkov O.V  ******* 28/05/21 ***
****** mapbits.h ***************  Shabakov D.A.  ****** 13/12/16 ***
****** mapbits.h ***************  Kruzhkov A.E.  ****** 10/04/17 ***
****** mapbits.h ***************  Zheleznyakov V.A.**** 21/10/21 ***
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2021              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*            ОПИСАНИЕ БАЗОВЫХ КЛАССОВ ЭЛЕКТРОННОЙ КАРТЫ            *
*                                                                  *
*******************************************************************/

#ifndef MAPBITS_H
#define MAPBITS_H

#ifndef MAPMACRO_H
  #include "mapmacro.h"
#endif

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++ КЛАСС ДАННЫХ: НАБОР БИТОВ ++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class _DLLCLASS TBitSet32
{
 public:

 // Коды возврата управляющих функций

 enum RETCODE { SET = 1 , CLEAR = 0 } ;

 // Инициализация данных

 TBitSet32() { Set(); }

 // Обнулить все биты

 void Clear() { Bits[0] = 0; }

 // Установить все биты в 1

 void Set() { Bits[0] = 0x0FFFFFFFFL; }

 // Обнулить заданный бит

 void Clear(int number) 
 {
   Bits[0] &= ~(1 << (number & 0x01F)); 
 }

 // Установить заданный бит

 void Set(int number)
 {
   Bits[0] |= (1 << (number & 0x01F)); 
 }

 // Опросить заданный бит

 int Check(int number) 
 {
   return (Bits[0] & (1 << (number & 0x01F))); 
 }

 // Сложить биты

 void And(unsigned int * bits)
 {
   if (bits == 0) return;
   Bits[0] &= *bits;
 }

 // Сложить биты

 void Or(unsigned int * bits)
 {
   if (bits == 0) return;
   Bits[0] |= *bits;
 }

 int operator == (unsigned int value)
 {
   return (Bits[0] == value);
 }

 int operator != (unsigned int value)
 {
   return (Bits[0] != value);
 }

// private:

 unsigned int Bits[2];          // Выравнивание на 8

}
 TBitSet32;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++ КЛАСС ДАННЫХ: НАБОР БИТОВ ++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TBitSet256
{
  public:

 // Коды возврата управляющих функций

 enum RETCODE { SET = 1 , CLEAR = 0 } ;

 // Инициализация данных

 TBitSet256() { Set(); }

 // Обнулить все биты

 void Clear() { memset(Bits,0,sizeof(Bits)); }

 // Установить все биты в 1

 void Set() { memset(Bits,-1,sizeof(Bits)); }

 // Обнулить заданный бит

 void Clear(int number)
 {
   Bits[(number & 0x0FF) >> 3] &= (unsigned char)~(1 << (number & 0x07));
 }

 // Установить заданный бит

 void Set(int number)
 {
   Bits[(number & 0x0FF) >> 3] |= (unsigned char)(1 << (number & 0x7));
 }

 // Опросить заданный бит

 int Check(int number)
 {
   return (Bits[(number & 0x0FF) >> 3] & (1 << (number & 0x7)));
 }

 // Сложить биты

 void And(unsigned char * bits)
 {
   if (bits == 0) return;
   int count = (32/sizeof(int));
   for (int i = 0; i < count; i++)
    *(((int*)Bits)+i) &= *(((int*)bits)+i);
 }

 // Сложить биты

 void Or(unsigned char * bits)
 {
   if (bits == 0) return;
   int count = (32/sizeof(int));
   for (int i = 0; i < count; i++)
    *(((int*)Bits)+i) |= *(((int*)bits)+i);
 }

// private:

 unsigned char Bits[32];

}
  TBitSet256;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++ КЛАСС ДАННЫХ: НАБОР БИТОВ ++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TBitSet1K
{
 public:

 // Коды возврата управляющих функций

 enum RETCODE { SET = 1 , CLEAR = 0 } ;

 // Инициализация данных

 TBitSet1K() { Set(); }

 // Обнулить все биты

 void Clear() { memset(Bits,0,sizeof(Bits)); }

 // Установить все биты в 1

 void Set() { memset(Bits,-1,sizeof(Bits)); }

 // Обнулить заданный бит

 void Clear(int number)
 {
   Bits[(number & 1023) >> 3] &= (unsigned char)~(1 << (number & 0x7));
 }

 // Установить заданный бит

 void Set(int number)
 {
   Bits[(number & 1023) >> 3] |= (unsigned char)(1 << (number & 0x7));
 }

 // Опросить заданный бит

 int Check(int number)
 {
   return (Bits[(number & 1023) >> 3] & (1 << (number & 0x7)));
 }

 // Сложить биты

 void And(unsigned char * bits)
 {
   int count = (128/sizeof(int));
   if (bits == 0) return;
   for (int i = 0; i < count; i++)
    *(((int*)Bits)+i) &= *(((int*)bits)+i);
 }

 // Сложить биты

 void Or(unsigned char * bits)
 {
   int count = (128/sizeof(int));
   if (bits == 0) return;
   for (int i = 0; i < count; i++)
    *(((int*)Bits)+i) |= *(((int*)bits)+i);
 }

// private:

 unsigned char Bits[128];        // 1024 / 8

}
  TBitSet1K;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++ КЛАСС ДАННЫХ: НАБОР БИТОВ ++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TBitSet4K
{
  public:

 // Коды возврата управляющих функций

 enum RETCODE { SET = 1 , CLEAR = 0 } ;

 // Инициализация данных

 TBitSet4K() { Set(); }

 // Обнулить все биты

 void Clear() { memset(Bits,0,sizeof(Bits)); }

 // Установить все биты в 1

 void Set() { memset(Bits,-1,sizeof(Bits)); }

 // Обнулить заданный бит

 void Clear(int number)
 {
   Bits[(number & 4095) >> 3] &= (unsigned char)~(1 << (number & 0x7));
 }

 // Установить заданный бит

 void Set(int number)
 {
   Bits[(number & 4095) >> 3] |= (unsigned char)(1 << (number & 0x7));
 }

 // Опросить заданный бит

 int Check(int number)
 {
   return (Bits[(number & 4095) >> 3] & (1 << (number & 0x7)));
 }

 // Сложить биты

 void And(unsigned char * bits)
 {
   int count = (512/sizeof(int));
   if (bits == 0) return;
   for (int i = 0; i < count; i++)
    *(((int*)Bits)+i) &= *(((int*)bits)+i);
 }

 // Сложить биты

 void Or(unsigned char * bits)
 {
   int count = (512/sizeof(int));
   if (bits == 0) return;
   for (int i = 0; i < count; i++)
    *(((int*)Bits)+i) |= *(((int*)bits)+i);
 }

//  private:

 unsigned char Bits[512];      //  4096 / 8

}
 TBitSet4K;

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++ КЛАСС ДАННЫХ: НАБОР БИТОВ ++++++++++++++++++++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class TBitSet64K
{
 public:

 // Коды возврата управляющих функций

 enum RETCODE { SET = 1 , CLEAR = 0 } ;

 // Инициализация данных

 TBitSet64K() { Set(); }

 // Обнулить все биты

 void Clear() { memset(Bits,0,sizeof(Bits)); }

 // Установить все биты в 1

 void Set() { memset(Bits,-1,sizeof(Bits)); }

 // Обнулить заданный бит

 void Clear(int number)
 {
   Bits[(number & 0x0FFFF) >> 3] &= (unsigned char)~(1 << (number & 0x7));
 }

 // Установить заданный бит

 void Set(int number)
 {
   Bits[(number & 0x0FFFF) >> 3] |= (unsigned char)(1 << (number & 0x7));
 }

 // Опросить заданный бит

 int Check(int number)
 {
   return (Bits[(number & 0x0FFFF) >> 3] & (1 << (number & 0x7)));
 }

 // Сложить биты

 void And(unsigned char * bits)
 {
   int count = (8192/sizeof(int));
   if (bits == 0) return;
   for (int i = 0; i < count; i++)
    *(((int*)Bits)+i) &= *(((int*)bits)+i);
 }

 // Сложить биты

 void Or(unsigned char * bits)
 {
   int count = (8192/sizeof(int));
   if (bits == 0) return;
   for (int i = 0; i < count; i++)
    *(((int*)Bits)+i) |= *(((int*)bits)+i);
 }

//  private:

 unsigned char Bits[8192];       // 64*1024 / 8

}
 TBitSet64K;

 // Запросить число единиц в битовом массиве
 // в пределах заданного числа бит

 int _fastcall bitCount64K(TBitSet64K * bits, int limit);

// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++
// ++++++++ КЛАСС ДАННЫХ: НАБОР БИТОВ ПЕРЕМЕННОЙ ДЛИНЫ +++++
// +++++++++++++++++++++++++++++++++++++++++++++++++++++++++

typedef class _DLLCLASS TBitSetVariable
{
  public:

  // Коды возврата управляющих функций

  enum RETCODE { SET = 1 , CLEAR = 0 } ;

  // Инициализация данных

  TBitSetVariable(unsigned int count = 0)
  {
    Bits = 0;
    Size = 0;
    IsAllocate = 1;
    if (count)
      ChangeSize(count);
  }

  ~TBitSetVariable() { Free();}

  // Освободить память

  void Free()
  {
    if (Bits && IsAllocate)
      delete [] Bits;
    Bits = 0;
    IsAllocate = 1;
  }

  // Изменить число бит в массиве

  int ChangeSize(unsigned int count)
  {
    if (IsAllocate == 0) return 0;
    Free();

    // Добавить запас для удобства с номерами объектов (начинаются с 1)
    count++;
    if (count < 256) count = 256;
    Size = (count + 7) >> 3;
    try { Bits = new unsigned char[Size]; } catch(...) { Bits = 0; }
    Clear();
    return (Bits != 0);
  }

  // Обнулить все биты

  void Clear() { if (Bits) memset(Bits, 0, Size); }

  // Установить все биты в 1

  void Set() { if (Bits) memset(Bits, -1, Size); }

  // Обнулить заданный бит

  void Clear(unsigned int number)
  {
    unsigned int number8 = number >> 3;
    if (Bits && (number8 < Size))
      Bits[number8] &= (unsigned char)~(1 << (number & 0x7));
  }

  // Установить заданный бит

  void Set(unsigned int number)
  {
    unsigned int number8 = number >> 3;
    if (Bits && (number8 < Size))
      Bits[number8] |= (unsigned char)(1 << (number & 0x7));
  }

  // Опросить заданный бит

  int Check(unsigned int number)
  {
    unsigned int number8 = number >> 3;
    if (Bits && (number8 < Size))
      return (Bits[number8] & (1 << (number & 0x7)));
    return 0;
  }

  // Запросить число бит в массиве

  int Count() { if (Bits == 0) return 0; return (Size << 3); }

  // Установить внешнюю область памяти

  int SetMemory(char * bits, unsigned int size)
  {
    if ((bits == 0) || (size <= 0)) return 0;
    Free();

    Bits = (unsigned char*)bits;
    Size = size;
    IsAllocate = 0;
    return 1;
  }

  // Заполнить область из памяти
  // bits - область памяти
  // size - размер памяти в байтах

  int LoadFromMemory(char* bits, unsigned int size)
  {
    if ((bits == 0) || (size <= 0)) return 0;
    ChangeSize(size * 8);
    Clear();
    if (Bits == 0)
      return 0;
    memcpy(Bits, bits, size);
    return 1;
  }  

  // Запросить область памяти

  const unsigned char* GetBits()
  {
    return Bits;
  }

  // Запросить размер памяти

  unsigned int GetSize()
  {
    return Size;
  }

  // Заполнить область из другого массива
  // bits - область памяти

  int LoadFromBit(TBitSetVariable* bits)
  {
    if (bits == 0) return 0;
    ChangeSize(bits->GetSize() * 8);
    Clear();
    if (Bits == 0)
      return 0;
    if (Size < bits->GetSize())
      return 0;
    memcpy(Bits, bits->GetBits(), bits->GetSize());
    return 1;
  }

  private:

  unsigned char * Bits;
  unsigned int    Size;
  unsigned int    IsAllocate;
}
  TBitSetVariable;

#endif  // MAPBITS_H

