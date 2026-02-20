/******  UTILS.H **************  Korjilov A.N. ******* 11/10/17 ****
*******  UTILS.H **************  Zheleznyakov V.A. *** 27/02/19 ****
*******  UTILS.H **************  Savelov A.S.  ******* 04/03/19 ****
*                                                                  *
*              Copyright (c) PANORAMA Group 1991-2019              *
*                      All Rights Reserved                         *
*                                                                  *
********************************************************************
*                                                                  *
*               Описание специализированных функций                *
*                                                                  *
*******************************************************************/
#ifndef UTILS_H
#define UTILS_H

#include <stdio.h>
#include <stdint.h>

#ifndef HWND
  #define HWND unsigned long int
#endif

const int CONV_UTF16 = 0;
const int CONV_KOI8_R = 1;
const int CONV_CP1251 = 126;

char* convertFromUtf8(const char* text, char* buffer, size_t len_buffer, int fromEncoding);
char* convertToUtf8(const char* text, char* buffer, size_t len_buffer, int fromEncoding);


    // Преобразовать строку символов в целое число
    // (является эквивалентом функции atoi для широких символов)
    // s - входная строка
    // sElementByteCount - сколько байт занимает один символ во входной строке
    // При ошибке возвращает ноль
    int _Wtoi(const char *s, int sElementByteCount);

    // Преобразовать строку символов в число типа double
    // (является эквивалентом функции atof для широких символов)
    // s - входная строка
    // sElementByteCount - сколько байт занимает один символ во входной строке
    // При ошибке возвращает ноль
    double _Wtof(const char *s, int sElementByteCount);

    // Преобразовать целое число в строку символов
    // (является эквивалентом функции itoa для широких символов)
    // value - входное число
    // dest - выходная строка
    // destlength - длина выходной строки
    // radix - основание системы счисления
    // destElementByteCount - сколько байт занимает один символ в dest
    // При ошибке возвращает ноль
    int _Itow(int value, char *dest, int destlength, int radix, int destElementByteCount);

    // Преобразовать целое число (int64) в строку символов
    // value - входное число
    // dest - выходная строка
    // destlength - длина выходной строки
    // radix - основание системы счисления
    // destElementByteCount - сколько байт занимает один символ в dest
    // При ошибке возвращает ноль
    int _I64tow(int64_t value, char *dest, int destlength, int radix, int destElementByteCount);

    // Сравнить две строки символов
    // (является эквивалентом функции strcmp для широких символов)
    // s1 - первая строка
    // s2 - вторая строка
    // s1ElementByteCount - сколько байт занимает один символ в s1
    // s2ElementByteCount - сколько байт занимает один символ в s2
    // Возвращает ноль, если строки s1 и s2 одинаковы
    int _Wcscmp(const char *s1, const char *s2, int s1_size, int s2_size);

    // Сравнить две строки символов (без учета регистра)
    // (является эквивалентом функции stricmp для широких символов)
    // s1 - первая строка
    // s2 - вторая строка
    // s1ElementByteCount - сколько байт занимает один символ в s1
    // s2ElementByteCount - сколько байт занимает один символ в s2
    // Возвращает ноль, если строки s1 и s2 одинаковы
    int _Wcsicmp(const char *s1, const char *s2, int s1ElementByteCount, int s2ElementByteCount);

    // Сравнить первые n символов двух строк
    // (является эквивалентом функции strncmp для широких символов)
    // s1 - первая строка
    // s2 - вторая строка
    // n - число символов, которые необходимо сравнить
    // s1ElementByteCount - сколько байт занимает один символ в s1
    // s2ElementByteCount - сколько байт занимает один символ в s2
    // Возвращает ноль, если первые n символов строк одинаковы
    int _Wcsncmp(const char *s1, const char *s2, int n, int s1ElementByteCount, int s2ElementByteCount);

    // Сравнить первые n символов двух строк (без учета регистра)
    // (является эквивалентом функции strnicmp для широких символов)
    // s1 - первая строка
    // s2 - вторая строка
    // n - число символов, которые необходимо сравнить
    // s1ElementByteCount - сколько байт занимает один символ в s1
    // s2ElementByteCount - сколько байт занимает один символ в s2
    // Возвращает ноль, если первые n символов строк одинаковы
    int _Wcsnicmp(const char *s1, const char *s2, int n, int s1ElementByteCount, int s2ElementByteCount);

    // Найти первое вхождение строки s2 в строке s1
    // (является эквивалентом функции strstr для широких символов)
    // s1 - строка, в которой необходимо искать
    // s2 - строка, которую необходимо найти
    // s1ElementByteCount - сколько байт занимает один символ в s1
    // s2ElementByteCount - сколько байт занимает один символ в s2
    // Возвращает указатель на первое появление строки s2 в строке s1.
    // Если строка не найдена или в случае ошибки, возвращает ноль
    const char* _Wcsstr(const char *s1, const char *s2, int s1ElementByteCount, int s2ElementByteCount);

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
    char* _Wcsncpy(char *dest, const char *src, int n, int destElementByteCount, int srcElementByteCount);

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
    char* _Wcsncat(char *dest, const char *src, int n, int destElementByteCount, int srcElementByteCount);

    // Определить длину строки
    // (является эквивалентом функции strlen для широких символов)
    // src - строка
    // srcElementByteCount - сколько байт занимает один символ в строке
    // Возвращает число символов в строке
    // При ошибке возвращает ноль
    int _Wcslen(const char* src, int srcElementByteCount);

    // Преобразовать строку символов к нижнему регистру
    // (является эквивалентом функции strlwr для широких символов)
    // str - строка
    // strElementByteCount - сколько байт занимает один символ в строке
    // При ошибке возвращает ноль
    int _Wcslwr(char *str, int strElementByteCount);

    // Преобразовать строку символов к верхнему регистру
    // (является эквивалентом функции strupr для широких символов)
    // str - строка
    // strElementByteCount - сколько байт занимает один символ в строке
    // При ошибке возвращает ноль
    int _Wcsupr(char *str, int strElementByteCount);

    //-----------------------------------------------------------------
    // Найти первое вхождение символа s2 в строке s1
    // (является эквивалентом функции strchr для широких символов)
    // s1 - строка, в которой необходимо искать
    // s2 - символ, который необходимо найти
    // s1ElementByteCount - сколько байт занимает один символ в s1
    // Возвращает указатель на первое появление символа s2 в строке s1.
    // Если символ не найден или в случае ошибки, возвращает ноль
    //-----------------------------------------------------------------
    char *_Wcschr(const char *s1, char *s2, int s1ElementByteCount);

    //-----------------------------------------------------------------
    // Найти последенее вхождение символа s2 в строке s1
    // (является эквивалентом функции strrchr для широких символов)
    // s1 - строка, в которой необходимо искать
    // s2 - строка, который необходимо найти
    // s1ElementByteCount - сколько байт занимает один символ в s1
    // Возвращает указатель на последенее появление символа s2 в строке s1.
    // Если символ не найден или в случае ошибки, возвращает ноль
    //-----------------------------------------------------------------
    char *_Wcsrchr(const char *s1, char *s2, int s1ElementByteCount);

    // Открыть файл
    // (является эквивалентом функции fopen для широких символов)
    // path - путь к файлу
    // mode - режим открытия файла
    // pathElementByteCount - сколько байт занимает один символ в строке path
    // modeElementByteCount - сколько байт занимает один символ в строке mode
    FILE* _Wfopen(const char *path, const char *mode, int pathElementByteCount, int modeElementByteCount);

    // Вывести сообщение в отдельном окне
    // (является эквивалентом функции MessageBox для широких символов)
    // hwnd - идентификатор окна
    // message - сообщение, которое необходимо вывести
    // title - заголовок окна
    // flag - параметр, определяющий вид окна (например, MB_OKCANCEL - см. mapsyst.h)
    // messageElementByteCount - сколько байт занимает один символ в строке message
    // titleElementByteCount - сколько байт занимает один символ в строке title
    int _MessageBoxW(HWND hwnd, const char *message, const char *title, int flag, int m_size, int t_size);

#endif // UTILS_H
