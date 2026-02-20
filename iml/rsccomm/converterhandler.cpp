#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "converterhandler.h"
#include "myexception.h"
#include <wchar.h>

ConverterHandler::ConverterHandler(){
    iconv_cd_1251 = 0;
    iconv_cd_dos = 0;
    iconv_cd_wchar_t = 0;
    iconv_cd_koi8r = 0;
}

iconv_t ConverterHandler::get(int semanticType){
#ifdef LINUXAPI
    switch (semanticType) {
    case 0: {
        if (iconv_cd_dos == 0){
            iconv_cd_dos = iconv_open("UTF-8", "866");
        }
        return iconv_cd_dos;
    }
    case 1: {
        if (iconv_cd_dos == 0){
            iconv_cd_dos = iconv_open("UTF-8", "KOI8-R");
        }
        return iconv_cd_dos;
    }
    case 126: {
        if (iconv_cd_1251 == 0){
            iconv_cd_1251 = iconv_open("UTF-8", "WINDOWS-1251");
        }
        return iconv_cd_1251;
    }
    case 127: {
        if (iconv_cd_wchar_t == 0){
            iconv_cd_wchar_t = iconv_open("UTF-8", "WCHAR_T");
        }
        return iconv_cd_wchar_t;
    }
    default: return 0;
    }
#else
  return 0;
#endif
}

void ConverterHandler::closeAll(){
#ifdef LINUXAPI
    if (iconv_cd_1251 != 0) iconv_close(iconv_cd_1251);
    if (iconv_cd_dos != 0) iconv_close(iconv_cd_dos);
    if (iconv_cd_wchar_t != 0) iconv_close(iconv_cd_wchar_t);
    if (iconv_cd_koi8r != 0) iconv_close(iconv_cd_koi8r);
#endif
}

char* ConverterHandler::convertToUTF8(char* text, int semanticType){

    size_t len_src = strlen(text);
    size_t len_dst = 2*len_src;
    char* text_dst = new char[len_dst + 1];
    memset(text_dst, 0, len_dst);
#ifdef LINUXAPI
    iconv_t iconv_cd = get(semanticType);
    if (iconv_cd == 0) {
        strcpy(text_dst, text);
        return text_dst;
    }

    char* value_src = text;
    char* value_dst = text_dst;
    if (iconv(iconv_cd, &value_src, &len_src,
                  &value_dst, &len_dst) == -1){
        const char* message;
        switch (errno){
        case E2BIG:
          message = "Error in text convertion to utf8: errno=E2BIG";
          break;
        case EILSEQ:
          message = "Error in text convertion to utf8: errno=EILSEQ";
            break;
        case EINVAL:
          message = "Error in text convertion to utf8: errno=EINVAL";
            break;
        default:
          message = "Error in text convertion to utf8: errno=%2";
        }
        throw MyException(message);
    }
    return text_dst;
#else
    strcpy(text_dst, text_out);
    return text_dst;
#endif
}

char* ConverterHandler::convertWCharToUTF8(char* text){

    size_t len_src = wcslen((wchar_t*)text)  * sizeof(wchar_t);
    size_t len_dst = 2*len_src;
    char* text_dst = new char[len_dst + 1];
    memset(text_dst, 0, len_dst);
#ifdef LINUXAPI
    iconv_t iconv_cd = get(127);
    if (iconv_cd == 0) {
        strcpy(text_dst, text);
        return text_dst;
    }

    char* value_src = text;
    char* value_dst = text_dst;
    if (iconv(iconv_cd, &value_src, &len_src,
                  &value_dst, &len_dst) == -1){
        const char* message;
        switch (errno){
        case E2BIG:
          message = "Error in text convertion to utf8: errno=E2BIG";
          break;
        case EILSEQ:
          message = "Error in text convertion to utf8: errno=EILSEQ";
            break;
        case EINVAL:
          message = "Error in text convertion to utf8: errno=EINVAL";
            break;
        default:
          message = "Error in text convertion to utf8: errno=%2";
        }
        throw MyException(message);
    }
    return text_dst;
#else
    strcpy(text_dst, text_out);
    return text_dst;
#endif
}
