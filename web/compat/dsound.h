#ifndef MONOPOLY_WEB_COMPAT_DSOUND_H
#define MONOPOLY_WEB_COMPAT_DSOUND_H
#include "windows.h"
#include "mmsystem.h"
typedef void *LPDIRECTSOUND;
typedef void *LPDIRECTSOUNDBUFFER;
typedef void *LPDIRECTSOUND3DBUFFER;
typedef void *LPDIRECTSOUND3DLISTENER;
typedef void *LPDIRECTSOUNDCAPTURE;
typedef void *LPDIRECTSOUNDCAPTUREBUFFER;
typedef void *LPDIRECTSOUNDNOTIFY;
typedef struct _DS3DLISTENER *LPDS3DLISTENER;
typedef struct _DS3DBUFFER *LPDS3DBUFFER;
typedef struct { DWORD dwSize; DWORD dwFlags; } DSCAPS;
typedef struct { DWORD dwSize; DWORD dwFlags; DWORD dwBufferBytes; void *lpwfxFormat; } DSBUFFERDESC;
#define DSSCL_PRIORITY 2
#define DS_OK 0
#define DSCAPS_PRIMARYMONO 1
#define DSCAPS_PRIMARYSTEREO 2
#define DSCAPS_PRIMARY8BIT 4
#define DSCAPS_PRIMARY16BIT 8
static inline HRESULT DirectSoundCreate(void *, LPDIRECTSOUND *, void *) { return E_FAIL; }
#endif
