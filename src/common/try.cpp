#ifndef IN_TRY_CPP
#define IN_TRY_CPP
#include "try.h"

#include <stdexcept>
#include <string.h>

bool sigsetjmp_flag = 0;

void unset_signals()
{
    struct sigaction new_action;
    struct sigaction d_old_action;
    memset (&new_action, 0, sizeof (new_action));
    new_action.sa_handler = SIG_DFL;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction (SIGFPE,  &new_action, &d_old_action);
    sigaction (SIGSEGV, &new_action, &d_old_action);
}

void signals_handler(int sig_no)
{
    unset_signals();
    siglongjmp(env, sig_no);
}

void setup_signals(void)
{
    struct sigaction new_action;
    struct sigaction d_old_action;
    memset (&new_action, 0, sizeof (new_action));
    new_action.sa_handler = signals_handler;
    sigemptyset (&new_action.sa_mask);
    new_action.sa_flags = 0;
    sigaction (SIGFPE,  &new_action, &d_old_action);
    sigaction (SIGSEGV, &new_action, &d_old_action);
}

bool getSigsetjmp_flag(void)
{
    return sigsetjmp_flag;
}

void setSigsetjmp_flag(bool value)
{
    sigsetjmp_flag = value;
}

#endif
