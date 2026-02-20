#ifndef _TRY_H
#define	_TRY_H

#include <QString>

class MyException {
public:
       MyException(const char* text);
       MyException(char* text);
       MyException(QString const &text);
       MyException(const MyException &myException);
       char d[255];
};

//extern MyException MYEXCEPTION;

#endif	/* _TRY_H */

