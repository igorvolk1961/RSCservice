//#include "utf8/mapsyst.h"
#include <stdlib.h>
#include <string.h>
#include <wchar.h>
//int _fastcall strlenun(const char * text, int limit){
int strlenun(const char * text, int limit){
    const size_t cSize = strlen(text) + 1;
    wchar_t wc[cSize];
    mbstowcs(wc, text, cSize);
    return wcslen(wc);
}
