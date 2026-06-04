#ifndef MONOPOLY_WEB_COMPAT_PROCESS_H
#define MONOPOLY_WEB_COMPAT_PROCESS_H

#include "windows.h"

typedef unsigned (__stdcall *MONOPOLY_WEB_THREAD_PROC)(void *);

static inline uintptr_t _beginthreadex(void *, unsigned, MONOPOLY_WEB_THREAD_PROC, void *, unsigned, unsigned *threadId)
{
    if (threadId) *threadId = 0;
    return 0;
}

static inline void _endthreadex(unsigned) {}

#endif
