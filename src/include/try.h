#ifndef _TRY_H
#define	_TRY_H

#include <setjmp.h>
#include <csignal>

extern "C" void setup_signals(void);
extern "C" bool getSigsetjmp_flag(void);
extern "C" void setSigsetjmp_flag(bool value);
  

#ifndef IN_TRY_CPP
   extern
#endif
    sigjmp_buf env;

#define ___try
#define ___except if (false)
#define ___end_try


/*
#define ___try \
   int sigsetjmp_res = 0; \
   bool oldSigsetjmp_flag = getSigsetjmp_flag(); \
   if (!oldSigsetjmp_flag) { \
     setSigsetjmp_flag(true); \
     setup_signals(); \
     sigsetjmp_res = sigsetjmp(env, 1); \
   } \
   if (sigsetjmp_res == 0)

#define ___except else

#define ___end_try setSigsetjmp_flag(oldSigsetjmp_flag);
 */
#define ___RETURN {setSigsetjmp_flag(oldSigsetjmp_flag); return;}
#define ___RETURN_NULL {setSigsetjmp_flag(oldSigsetjmp_flag); return NULL;}
#define ___RETURN_0 {setSigsetjmp_flag(oldSigsetjmp_flag); return 0;}
#define ___RETURN_1 {setSigsetjmp_flag(oldSigsetjmp_flag); return 1;}
#define ___RETURN_FALSE {setSigsetjmp_flag(oldSigsetjmp_flag); return false;}
#define ___RETURN_TRUE {setSigsetjmp_flag(oldSigsetjmp_flag); return true;}

#endif	/* _TRY_H */

