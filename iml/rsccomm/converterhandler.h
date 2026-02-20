#ifndef CONVERTERHANDLER_H
#define CONVERTERHANDLER_H
#include "mapsyst.h"

#ifdef LINUXAPI
#include  <iconv.h>
#else
#define iconv_t int
#endif


class ConverterHandler{
public:
    iconv_t iconv_cd_1251;
    iconv_t iconv_cd_dos;
    iconv_t iconv_cd_wchar_t;
    iconv_t iconv_cd_koi8r;

public:
    ConverterHandler();
    void closeAll();
    iconv_t get(int semanticType);
    char* convertToUTF8(char* text_out, int semanticType);
    char* convertWCharToUTF8(char* text_out);
};

#endif // CONVERTERHANDLER_H
