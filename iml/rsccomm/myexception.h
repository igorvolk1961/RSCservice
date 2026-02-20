#ifndef _TRY_H
#define	_TRY_H

class MyException {
public:
       MyException(const char* text);
       MyException(char* text);
       MyException(const MyException &myException);
       char d[255];
};

//extern MyException MYEXCEPTION;

#endif	/* _TRY_H */

