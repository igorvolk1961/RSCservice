#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "semblockinfo.h"
#include <cstring>
#include <cstdlib>
#include "semdef.h"

typedef struct SEMBLOCK // СОСТАВ СЕМАНТИЧЕСКОЙ ХАРАКТЕРИСТИКИ
{
#ifndef RISCCPU
  unsigned short int SemCode;     // КОД СЕМАНТИКИ
#else
  unsigned char      SemCode[2];
#endif
  SEMBASE  Data;               // ЗНАЧЕНИЕ СЕМАНТИКИ - ЧИСЛО ТИПА СЕМАНТИКА
}
  SEMBLOCK;


SemBlockInfo* SemBlockInfo::buildSemBlock(int semType, int semCode, const char* value){
    size_t semBlockSize = sizeof(short int) + 2 * sizeof(char);
    SEMBLOCK* sb;
    char* semBlock;
    switch (semType){
    case SEMBASESTRDOS: // CТРОКА СИМВОЛОВ (ASCIIZ),ОГРАНИЧЕННАЯ НУЛЕМ
    case SEMBASESTRING: // CТРОКА СИМВОЛОВ (ANSI),ОГРАНИЧЕННАЯ НУЛЕМ
    case SEMBASESTRUNI:{ // CТРОКА СИМВОЛОВ (UNICODE),ОГРАНИЧЕННАЯ НУЛЕМ
        size_t valueSize = strlen(value);
        semBlockSize += valueSize + 1;
        semBlock = new char[semBlockSize];
        sb = (SEMBLOCK*) semBlock;
        sb->Data.String.Scale = valueSize;
        strcpy(sb->Data.String.Value, value);
        break;
    }
    case SEMBASECHAR:{ // 1 БАЙТ   SIGNED CHAR
        semBlockSize += 1;
        semBlock = new char[semBlockSize];
        sb = (SEMBLOCK*) semBlock;
        sb->Data.Char.Value = atoi(value);
        break;
    }
    case SEMBASESHORT:{ // 2 БАЙТA  SIGNED SHORT
        semBlockSize += 2;
        semBlock = new char[semBlockSize];
        sb = (SEMBLOCK*) semBlock;
        sb->Data.String.Scale = 0;
        sb->Data.Short.Value = atoi(value);
        break;
    }
    case SEMBASELONG:{ // 4 БАЙТA  SIGNED INT
        semBlockSize += 4;
        semBlock = new char[semBlockSize];
        sb = (SEMBLOCK*) semBlock;
        sb->Data.String.Scale = 0;
        int32_t i = atoi(value);
#ifndef RISCCPU
        sb->Data.Long.Value = i;
#else
        REGISTER reg;
        LongToRegister(reg, &i);
        RegisterToLong(reg, sb->Data.Long.Value);
#endif
        break;
    }
    case SEMBASEDOUBLE:{ // 8 БАЙТ   DOUBLE
        semBlockSize += 8;
        semBlock = new char[semBlockSize];
        sb = (SEMBLOCK*) semBlock;
        sb->Data.String.Scale = 0;
        double f = atof(value);
#ifndef RISCCPU
        sb->Data.Double.Value = f;
#else
        DOUBLEREGISTER reg;
        DoubleToRegister(reg, &f);
        RegisterToDouble(reg, sb->Data.Double.Value);
#endif
        break;
    }
    }
    sb->Data.String.Type = semType;
#ifndef RISCCPU
        sb->SemCode = (unsigned short int)semCode;
#else
        REGISTER reg;
        ShortToRegister(reg, &semCode);
        RegisterToShort(reg, sb->SemCode);
#endif
    SemBlockInfo* semBlockInfo = new SemBlockInfo(semBlock, semBlockSize);
    return semBlockInfo;
}

