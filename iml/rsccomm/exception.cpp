#if !defined(__linux__)
#include "stdafx.h"
#endif

#include "exception.h"
#include <cstdarg>

void throwException(JNIEnv* jenv, const char* format, ...){
    va_list ptr;
    va_start(ptr, format);
    vprintf(format, ptr);
    printf("\n");
    va_end(ptr);

    char message[255];
    va_start(ptr, format);
    vsprintf(message, format, ptr);
    va_end(ptr);

    printf("!!!! %s\n", message);
    jclass exception = jenv->FindClass("java/lang/Throwable");
//    jenv->ThrowNew(exception, message);
}
