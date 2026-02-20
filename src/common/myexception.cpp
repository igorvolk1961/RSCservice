#ifndef IN_TRY_CPP
#define IN_TRY_CPP
#if !defined(__linux__)
//#include "stdafx.h"
#endif

#include "myexception.h"

#include <string.h>

MyException::MyException(const char* text){
  strcpy(d, text);
}

MyException::MyException(char* text){
  strcpy(d, text);
}

MyException::MyException(QString const & text){
  strcpy(d, qPrintable(text));
}

MyException::MyException(const MyException &myException){
  strcpy(d, myException.d);
}

#endif
