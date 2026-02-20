#ifndef EXCEPTION_H
#define EXCEPTION_H

#include <jni.h>

void throwException(JNIEnv* jenv, const char* format, ...);

#endif // EXCEPTION_H
