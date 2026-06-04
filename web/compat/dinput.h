#ifndef MONOPOLY_WEB_COMPAT_DINPUT_H
#define MONOPOLY_WEB_COMPAT_DINPUT_H

#include "windows.h"

typedef void *LPDIRECTINPUTA;
typedef void *LPDIRECTINPUTDEVICEA;
typedef void *LPDIRECTINPUT;
typedef void *LPDIRECTINPUTDEVICE;

#define DIRECTINPUT_VERSION 0x0500

static inline HRESULT DirectInputCreateA(HINSTANCE, DWORD, LPDIRECTINPUTA *, void *)
{
    return E_FAIL;
}

#define DirectInputCreate DirectInputCreateA

#endif
