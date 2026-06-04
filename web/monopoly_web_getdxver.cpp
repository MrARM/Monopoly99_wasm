#include "windows.h"

#ifndef VER_PLATFORM_WIN32_WINDOWS
#define VER_PLATFORM_WIN32_WINDOWS 1
#endif

void GetDXVersion(DWORD& dwDXVersion, DWORD& dwDXPlatform)
{
  dwDXVersion = 0x600;
  dwDXPlatform = VER_PLATFORM_WIN32_WINDOWS;
}
