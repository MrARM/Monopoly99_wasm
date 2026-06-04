#ifndef MONOPOLY_WEB_COMPAT_VFW_H
#define MONOPOLY_WEB_COMPAT_VFW_H

#include "windows.h"
#include "mmsystem.h"

typedef void *HIC;
typedef DWORD FOURCC;
typedef void *PAVIFILE;
typedef void *PAVISTREAM;
typedef void *PGETFRAME;
typedef struct AVISTREAMINFO {
    DWORD fccType;
    DWORD fccHandler;
    DWORD dwFlags;
    DWORD dwCaps;
    WORD wPriority;
    WORD wLanguage;
    DWORD dwScale;
    DWORD dwRate;
    DWORD dwStart;
    DWORD dwLength;
    DWORD dwInitialFrames;
    DWORD dwSuggestedBufferSize;
    DWORD dwQuality;
    DWORD dwSampleSize;
    RECT rcFrame;
} AVISTREAMINFO, AVIStreamHeader, AVIFILEINFO;

#define ICM_DECOMPRESS 0x00004000
#define ICM_DECOMPRESS_BEGIN 0x00004003
#define ICM_DECOMPRESS_END 0x00004004
#define ICM_DECOMPRESS_QUERY 0x00004005
#define ICM_DECOMPRESS_GET_FORMAT 0x0000400A
#define ICM_DECOMPRESS_GET_PALETTE 0x0000400D
#define OF_READ 0x00000000
#define OF_SHARE_DENY_WRITE 0x00000020
#define streamtypeVIDEO mmioFOURCC('v','i','d','s')
#define streamtypeAUDIO mmioFOURCC('a','u','d','s')
#define listtypeAVIHEADER mmioFOURCC('h','d','r','l')
#define ckidSTREAMHEADER mmioFOURCC('s','t','r','h')
#define AVIGETFRAMEF_BESTDISPLAYFMT ((LONG)-1)

static inline LRESULT ICSendMessage(HIC, UINT, DWORD_PTR, DWORD_PTR) { return 0; }
static inline void AVIFileInit(void) {}
static inline void AVIFileExit(void) {}
static inline HRESULT AVIFileOpen(PAVIFILE *out, LPCSTR, UINT, void *)
{
    if (out) *out = NULL;
    return E_FAIL;
}
static inline HRESULT AVIFileInfo(PAVIFILE, AVIFILEINFO *info, LONG)
{
    if (info) ZeroMemory(info, sizeof(*info));
    return E_FAIL;
}
static inline HRESULT AVIFileGetStream(PAVIFILE, PAVISTREAM *out, DWORD, LONG)
{
    if (out) *out = NULL;
    return E_FAIL;
}
static inline ULONG AVIFileRelease(PAVIFILE) { return 0; }
static inline HRESULT AVIStreamInfo(PAVISTREAM, AVISTREAMINFO *info, LONG)
{
    if (info) ZeroMemory(info, sizeof(*info));
    return E_FAIL;
}
static inline LONG AVIStreamLength(PAVISTREAM) { return 0; }
static inline HRESULT AVIStreamReadFormat(PAVISTREAM, LONG, LPVOID buffer, LONG *size)
{
    if (size && buffer == NULL) *size = sizeof(WAVEFORMATEX);
    if (buffer && size && *size >= (LONG)sizeof(WAVEFORMATEX)) {
        WAVEFORMATEX *format = (WAVEFORMATEX *)buffer;
        ZeroMemory(format, sizeof(*format));
        format->wFormatTag = WAVE_FORMAT_PCM;
    }
    return E_FAIL;
}
static inline HRESULT AVIStreamBeginStreaming(PAVISTREAM, LONG, LONG, LONG) { return E_FAIL; }
static inline HRESULT AVIStreamEndStreaming(PAVISTREAM) { return 0; }
static inline PGETFRAME AVIStreamGetFrameOpen(PAVISTREAM, LPBITMAPINFOHEADER) { return NULL; }
static inline LPVOID AVIStreamGetFrame(PGETFRAME, LONG) { return NULL; }
static inline HRESULT AVIStreamGetFrameClose(PGETFRAME) { return 0; }
static inline ULONG AVIStreamRelease(PAVISTREAM) { return 0; }
static inline HRESULT AVIStreamRead(PAVISTREAM, LONG, LONG, LPVOID, LONG, LONG *bytesRead, LONG *samplesRead)
{
    if (bytesRead) *bytesRead = 0;
    if (samplesRead) *samplesRead = 0;
    return E_FAIL;
}

#endif
