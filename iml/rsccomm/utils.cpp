#include "utils.h"
#include "myexception.h"
#include "stdlib.h"
#include "string.h"
#include "mapsyst.h"
#include "maptype.h"
#include "semdef.h"
#include <wchar.h>
#include "iconv.h"

iconv_t iconv_cd_utf8_koi8_r;
iconv_t iconv_cd_utf8_cp1251;
iconv_t iconv_cd_utf8_utf16;
iconv_t iconv_cd_koi8_r_utf8;
iconv_t iconv_cd_cp1251_utf8;

char* convertFromUtf8(const char* text, char* buffer, size_t len_buffer, int toEncoding){
    iconv_t iconv_cd;
    switch (toEncoding) {
    case CONV_KOI8_R: {
        if (iconv_cd_koi8_r_utf8 == 0){
            iconv_cd_koi8_r_utf8 = iconv_open("KOI8-R", "UTF-8");
        }
        iconv_cd = iconv_cd_koi8_r_utf8;
        break;
    }
    case CONV_CP1251: {
        if (iconv_cd_cp1251_utf8 == 0){
            iconv_cd_cp1251_utf8 = iconv_open("cp1251", "UTF-8");
        }
        iconv_cd = iconv_cd_cp1251_utf8;
        break;
    }
    default: iconv_cd = 0;
    }

    size_t len_src = strlen(text);
    size_t len_dst = len_buffer;
    memset(buffer, 0, len_buffer);
    if (iconv_cd == 0) {
        strcpy(buffer, text);
        return buffer;
    }

    char* in = const_cast<char*>(text);
    char* out = buffer;
    long int res = iconv(iconv_cd, &in, &len_src,
                 &out, &len_dst);
    if (res == -1){
        const char* message;
        switch (errno){
        case E2BIG:
          message = "Error in semantic convertion to koi8: errno=E2BIG";
          break;
        case EILSEQ:
          message = "Error in semantic convertion to koi8: errno=EILSEQ";
            break;
        case EINVAL:
          message = "Error in semantic convertion to koi8: errno=EINVAL";
            break;
        default:
          message = "Error in semantic convertion to koi8: errno=%2";
        }
        throw MyException(message);
    }
    return buffer;
}

char* convertToUtf8(const char* text, char* buffer, size_t len_buffer, int fromEncoding){
    iconv_t iconv_cd;
    switch (fromEncoding) {
    case CONV_UTF16:{
        if (iconv_cd_utf8_utf16 == 0){
            iconv_cd_utf8_utf16 = iconv_open("UTF-8", "UTF-16");
        }
        iconv_cd = iconv_cd_utf8_utf16;
        break;
    }
    case CONV_KOI8_R: {
        if (iconv_cd_utf8_koi8_r == 0){
            iconv_cd_utf8_koi8_r = iconv_open("UTF-8", "KOI8-R");
        }
        iconv_cd = iconv_cd_utf8_koi8_r;
        break;
    }
    case CONV_CP1251: {
        if (iconv_cd_utf8_cp1251 == 0){
            iconv_cd_utf8_cp1251 = iconv_open("UTF-8", "cp1251");
        }
        iconv_cd = iconv_cd_utf8_cp1251;
        break;
    }
    default: iconv_cd = 0;
    }

    size_t len_src = strlen(text);
    size_t len_dst = len_buffer;
    memset(buffer, 0, len_buffer);
    if (iconv_cd == 0) {
        strcpy(buffer, text);
        return buffer;
    }

    char* in = const_cast<char*>(text);
    char* out = buffer;
    long int res = iconv(iconv_cd, &in, &len_src,
                 &out, &len_dst);
    if (res == -1){
        const char* message;
        switch (errno){
        case E2BIG:
          message = "Error in semantic convertion to koi8: errno=E2BIG";
          break;
        case EILSEQ:
          message = "Error in semantic convertion to koi8: errno=EILSEQ";
            break;
        case EINVAL:
          message = "Error in semantic convertion to koi8: errno=EINVAL";
            break;
        default:
          message = "Error in semantic convertion to koi8: errno=%2";
        }
        throw MyException(message);
    }
    return buffer;
}

size_t u16_len(const char* s){
    size_t result = 0;
    const char* c = s;
    while (*c) {
        c++;
        c++;
        result++;
    }
    return result;
}

void u16tou8(char* u, const char* s){
    size_t j = 0;
    const char* c = s;
    while (*c) {
        *u = *c;
        u++;
        c++;
        c++;
    }
    *u = 0;
}


// Преобразовать строку символов в целое число
// (является эквивалентом функции atoi для широких символов)
// s - входная строка
// sElementByteCount - сколько байт занимает один символ во входной строке
// При ошибке возвращает ноль
int _Wtoi(const char *s, int sElementByteCount){
    if (sElementByteCount == 1) {
      return atoi(s);
    } else {
      wchar_t* w = (wchar_t*)s;
      wchar_t* end;
      return wcstol(w, &end, 10);
    }
}


// Преобразовать строку символов в число типа double
// (является эквивалентом функции atof для широких символов)
// s - входная строка
// sElementByteCount - сколько байт занимает один символ во входной строке
// При ошибке возвращает ноль
double _Wtof(const char *s, int sElementByteCount){
    if (sElementByteCount == 1) {
      return atof(s);
    } else {
      wchar_t* end;
      return wcstof((wchar_t*)s, &end);
    }
}

// Сравнить первые n символов двух строк
// (является эквивалентом функции strncmp для широких символов)
// s1 - первая строка
// s2 - вторая строка
// n - число символов, которые необходимо сравнить
// s1ElementByteCount - сколько байт занимает один символ в s1
// s2ElementByteCount - сколько байт занимает один символ в s2
// Возвращает ноль, если первые n символов строк одинаковы
int _Wcsncmp(const char *s1, const char *s2, int n, int s1ElementByteCount, int s2ElementByteCount){
    if (s1ElementByteCount == 1 && s2ElementByteCount == 1) {
        return strncmp(s1, s2, n);
    } else {
        // для кириллицы этот код не будет работать, нужно использовать wcsncmp
      size_t l1 = u16_len(s1);
      char u1[l1 + 1];
      u16tou8(u1, s1);

      size_t l2 = u16_len(s2);
      char u2[l2 + 1];
      u16tou8(u2, s2);
      return strncmp(u1, u2, n);
    }
}

// Скопировать первые n байт строки src в строку dest
// (является эквивалентом функции strncpy для широких символов)
// Если нулевой байт отсутствует в первых n байтах src,
// то результирующая строка не будет завершена нулем
// dest - выходная строка
// src  - входная строка
// n - число символов, которые необходимо скопировать
// destElementByteCount - сколько байт занимает один символ в dest
// srcElementByteCount  - сколько байт занимает один символ в src
// Всегда возвращает указатель на целевую строку dest
char* _Wcsncpy(char *dest, const char *src, int n, int destElementByteCount, int srcElementByteCount){
    if (destElementByteCount == 1 && srcElementByteCount == 1) {
        return strncpy(dest, src, n);
    } else
    if (destElementByteCount == 1) {
        wchar_t w1[255];
        mbstowcs(w1, dest, 255);
        return (char*) wcsncpy(w1, (wchar_t*)src, n);
    } else
    if (srcElementByteCount == 1) {
        wchar_t w2[255];
        mbstowcs(w2, src, 255);
        return (char*) wcsncpy((wchar_t*)dest, w2, n);
    } else {
      return (char*) wcsncpy((wchar_t*)dest, (wchar_t*)src, n);
    }
}

// Определить длину строки
// (является эквивалентом функции strlen для широких символов)
// src - строка
// srcElementByteCount - сколько байт занимает один символ в строке
// Возвращает число символов в строке
// При ошибке возвращает ноль
int _Wcslen(const char* src, int srcElementByteCount){
    if (srcElementByteCount == 1) {
        return strlen(src);
    } else {
        return wcslen((wchar_t*)src);
    }
}


// Найти первое вхождение строки s2 в строке s1
// (является эквивалентом функции strstr для широких символов)
// s1 - строка, в которой необходимо искать
// s2 - строка, которую необходимо найти
// s1ElementByteCount - сколько байт занимает один символ в s1
// s2ElementByteCount - сколько байт занимает один символ в s2
// Возвращает указатель на первое появление строки s2 в строке s1.
// Если строка не найдена или в случае ошибки, возвращает ноль
const char* _Wcsstr(const char *s1, const char *s2, int s1ElementByteCount, int s2ElementByteCount){
    if (s1ElementByteCount == 1 && s2ElementByteCount == 1) {
        return strstr(s1, s2);
    } else
    if (s1ElementByteCount == 1) {
        wchar_t w1[255];
        mbstowcs(w1, s1, 255);
        return (const char*)wcsstr(w1, (wchar_t*)s2);
    } else
    if (s2ElementByteCount == 1) {
        wchar_t w2[255];
        mbstowcs(w2, s2, 255);
        return (const char*)wcsstr((wchar_t*)s1, w2);
    } else {
      return (const char*)wcsstr((wchar_t*)s1, (wchar_t*)s2);
    }
}


// Добавить первые n байт строки src в строку dest
// (является эквивалентом функции strncat для широких символов)
// Если нулевой байт отсутствует в первых n байтах src,
// то результирующая строка не будет завершена нулем
// В строке dest должно быть место, как минимум, для Wcslen(dest)+n+1 широких символов.
// dest - выходная строка
// src  - входная строка
// n - число символов, которые необходимо добавить
// destElementByteCount - сколько байт занимает один символ в dest
// srcElementByteCount  - сколько байт занимает один символ в src
// Всегда возвращает указатель на целевую строку dest
char* _Wcsncat(char *dest, const char *src, int n, int destElementByteCount, int srcElementByteCount){
    if (destElementByteCount == 1 && srcElementByteCount == 1) {
        return strncat(dest, src, n);
    } else
    if (destElementByteCount == 1) {
        wchar_t w1[255];
        mbstowcs(w1, dest, 255);
        return (char* )wcsncat(w1, (wchar_t*)src, n);
    } else
    if (srcElementByteCount == 1) {
        wchar_t w2[255];
        mbstowcs(w2, src, 255);
        return (char* )wcsncat((wchar_t*)dest, w2, n);
    } else {
      return (char* )wcsncat((wchar_t*)dest, (wchar_t*)src, n);
    }
}

void GetSystemTime(SYSTEMTIME * systemtime){
}

char * imgWin2Koi8(char * text, long int length){
    return 0;
}

char * imgText2Koi8(char * text, long int length){
    return 0;
}

char * imgDos2Koi8(char * text, long int length){
    return 0;
}

int strlenun(const char * text, int limit){
    return wcslen((wchar_t *)text);
}
