#ifndef MONOPOLY_WEB_COMPAT_DIRECT_H
#define MONOPOLY_WEB_COMPAT_DIRECT_H

#include "windows.h"

#define _getcwd getcwd
#define _chdir chdir
static inline int _mkdir(const char *path) { return mkdir(path, 0777); }

#endif
