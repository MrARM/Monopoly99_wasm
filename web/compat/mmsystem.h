#ifndef MONOPOLY_WEB_COMPAT_MMSYSTEM_H
#define MONOPOLY_WEB_COMPAT_MMSYSTEM_H
#include "windows.h"
#include <stdlib.h>
#include <string.h>
typedef UINT MMRESULT;
typedef UINT MCIDEVICEID;
typedef DWORD FOURCC;
typedef void *HMMIO;
typedef void (CALLBACK *LPTIMECALLBACK)(UINT, UINT, DWORD, DWORD, DWORD);
#define WAVE_FORMAT_PCM 1
#define MMSYSERR_NOERROR 0
#define TIMERR_NOERROR 0
#define TIME_PERIODIC 0x0001u
#define FOURCC_MEM 0x204d454du
#define MMIO_READ 0x00000000u
#define MMIO_READWRITE 0x00000002u
#define MMIO_CREATE 0x00001000u
#define MMIO_DENYWRITE 0x00000020u
#define MMIO_FINDRIFF 0x00000020u
#define MMIO_FINDCHUNK 0x00000010u
#define MMIO_FINDLIST 0x00000040u
#define MMIO_CREATERIFF 0x00000020u
#define MAKEFOURCC(ch0, ch1, ch2, ch3) ((DWORD)(BYTE)(ch0) | ((DWORD)(BYTE)(ch1) << 8) | ((DWORD)(BYTE)(ch2) << 16) | ((DWORD)(BYTE)(ch3) << 24))
#define mmioFOURCC(ch0, ch1, ch2, ch3) MAKEFOURCC(ch0, ch1, ch2, ch3)
#define formtypeAVI mmioFOURCC('A','V','I',' ')
#define listtypeSTREAMHEADER mmioFOURCC('s','t','r','l')
#define ckidSTREAMFORMAT mmioFOURCC('s','t','r','f')
#define MCI_NOTIFY 0x00000001
#define MCI_WAIT 0x00000002
#define MCI_OPEN 0x0803
#define MCI_PLAY 0x0806
#define MCI_CLOSE 0x0804
#define MCI_STATUS 0x0814
#define MCI_SEEK 0x0807
#define MCI_SEEK_TO_START 0x00000100
#define MCI_STATUS_ITEM 0x00000100
typedef struct { DWORD dwCallback; LPCSTR lpstrDeviceType; LPCSTR lpstrElementName; LPCSTR lpstrAlias; MCIDEVICEID wDeviceID; } MCI_OPEN_PARMS;
typedef struct { DWORD dwCallback; DWORD dwFrom; DWORD dwTo; } MCI_PLAY_PARMS;
typedef struct { DWORD dwCallback; DWORD dwTo; } MCI_SEEK_PARMS;
typedef struct { DWORD dwCallback; DWORD dwReturn; DWORD dwItem; DWORD dwTrack; } MCI_STATUS_PARMS;
typedef struct {
    DWORD dwFlags;
    FOURCC fccIOProc;
    LPSTR pchBuffer;
    LONG cchBuffer;
    LONG adwInfo[3];
} MMIOINFO, *LPMMIOINFO;
typedef struct {
    FOURCC ckid;
    DWORD cksize;
    FOURCC fccType;
    DWORD dwDataOffset;
    DWORD dwFlags;
} MMCKINFO, *LPMMCKINFO;
typedef struct timecaps_tag {
    UINT wPeriodMin;
    UINT wPeriodMax;
} TIMECAPS, *LPTIMECAPS;
typedef struct tWAVEFORMATEX {
    WORD wFormatTag;
    WORD nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD nBlockAlign;
    WORD wBitsPerSample;
    WORD cbSize;
} WAVEFORMATEX, *LPWAVEFORMATEX;
static inline MMRESULT mciSendCommand(MCIDEVICEID, UINT, DWORD, DWORD) { return 0; }

typedef struct {
    BYTE *buffer;
    LONG capacity;
    LONG size;
    LONG position;
    DWORD openFlags;
    struct {
        LONG headerOffset;
        LONG dataOffset;
        FOURCC ckid;
        DWORD flags;
    } chunkStack[32];
    int chunkDepth;
} MONOPOLY_WEB_MMIO;

static inline DWORD monopoly_web_mmio_read32(const BYTE *ptr)
{
    DWORD value;
    memcpy(&value, ptr, sizeof(value));
    return value;
}

static inline void monopoly_web_mmio_write32(BYTE *ptr, DWORD value)
{
    memcpy(ptr, &value, sizeof(value));
}

static inline LONG monopoly_web_mmio_chunk_data_start(LPMMCKINFO chunk)
{
    if (chunk->ckid == mmioFOURCC('R','I','F','F') || chunk->ckid == mmioFOURCC('L','I','S','T'))
        return (LONG) chunk->dwDataOffset + 4;
    return (LONG) chunk->dwDataOffset;
}

static inline LONG monopoly_web_mmio_chunk_end(LPMMCKINFO chunk)
{
    LONG end = (LONG) chunk->dwDataOffset + (LONG) chunk->cksize;
    if (chunk->cksize & 1)
        end++;
    return end;
}

static inline HMMIO mmioOpen(LPSTR, LPMMIOINFO info, DWORD flags)
{
    MONOPOLY_WEB_MMIO *stream;

    if (!info || info->fccIOProc != FOURCC_MEM || !info->pchBuffer || info->cchBuffer <= 0)
        return NULL;

    stream = (MONOPOLY_WEB_MMIO *) calloc(1, sizeof(MONOPOLY_WEB_MMIO));
    if (!stream)
        return NULL;

    stream->buffer = (BYTE *) info->pchBuffer;
    stream->capacity = info->cchBuffer;
    stream->openFlags = flags;
    stream->size = (flags & MMIO_CREATE) ? 0 : info->cchBuffer;
    stream->position = 0;
    return (HMMIO) stream;
}

static inline MMRESULT mmioClose(HMMIO handle, UINT)
{
    if (handle)
        free(handle);
    return MMSYSERR_NOERROR;
}

static inline MMRESULT mmioCreateChunk(HMMIO handle, LPMMCKINFO chunk, UINT flags)
{
    MONOPOLY_WEB_MMIO *stream = (MONOPOLY_WEB_MMIO *) handle;
    LONG headerOffset;
    DWORD chunkId;

    if (!stream || !chunk || stream->chunkDepth >= 32)
        return 1;

    headerOffset = stream->position;
    if (headerOffset + ((flags & MMIO_CREATERIFF) ? 12 : 8) > stream->capacity)
        return 1;

    chunkId = (flags & MMIO_CREATERIFF) ? mmioFOURCC('R','I','F','F') : chunk->ckid;
    monopoly_web_mmio_write32(stream->buffer + headerOffset, chunkId);
    monopoly_web_mmio_write32(stream->buffer + headerOffset + 4, 0);

    stream->position = headerOffset + 8;
    if (flags & MMIO_CREATERIFF)
    {
        monopoly_web_mmio_write32(stream->buffer + stream->position, chunk->fccType);
        stream->position += 4;
        chunk->ckid = mmioFOURCC('R','I','F','F');
        chunk->dwDataOffset = (DWORD) (headerOffset + 8);
    }
    else
    {
        chunk->dwDataOffset = (DWORD) stream->position;
    }

    chunk->cksize = 0;
    stream->chunkStack[stream->chunkDepth].headerOffset = headerOffset;
    stream->chunkStack[stream->chunkDepth].dataOffset = (LONG) chunk->dwDataOffset;
    stream->chunkStack[stream->chunkDepth].ckid = chunk->ckid;
    stream->chunkStack[stream->chunkDepth].flags = flags;
    stream->chunkDepth++;
    if (stream->position > stream->size)
        stream->size = stream->position;
    return MMSYSERR_NOERROR;
}

static inline MMRESULT mmioAscend(HMMIO handle, LPMMCKINFO chunk, UINT)
{
    MONOPOLY_WEB_MMIO *stream = (MONOPOLY_WEB_MMIO *) handle;
    LONG headerOffset;
    LONG dataStart;
    LONG end;
    DWORD size;

    if (!stream || !chunk)
        return 1;

    if (stream->chunkDepth > 0)
    {
        headerOffset = stream->chunkStack[stream->chunkDepth - 1].headerOffset;
        dataStart = stream->chunkStack[stream->chunkDepth - 1].dataOffset;
        if (stream->chunkStack[stream->chunkDepth - 1].flags & MMIO_CREATERIFF)
            dataStart = headerOffset + 8;
        size = (DWORD) (stream->position - dataStart);
        monopoly_web_mmio_write32(stream->buffer + headerOffset + 4, size);
        chunk->cksize = size;
        stream->chunkDepth--;

        if (size & 1)
        {
            if (stream->position >= stream->capacity)
                return 1;
            stream->buffer[stream->position++] = 0;
        }
        if (stream->position > stream->size)
            stream->size = stream->position;
        return MMSYSERR_NOERROR;
    }

    end = monopoly_web_mmio_chunk_end(chunk);
    if (end > stream->size)
        end = stream->size;
    stream->position = end;
    return MMSYSERR_NOERROR;
}

static inline LONG mmioRead(HMMIO handle, char *dest, LONG bytes)
{
    MONOPOLY_WEB_MMIO *stream = (MONOPOLY_WEB_MMIO *) handle;
    LONG available;

    if (!stream || !dest || bytes <= 0)
        return 0;
    available = stream->size - stream->position;
    if (available <= 0)
        return 0;
    if (bytes > available)
        bytes = available;
    memcpy(dest, stream->buffer + stream->position, (size_t) bytes);
    stream->position += bytes;
    return bytes;
}

static inline LONG mmioWrite(HMMIO handle, const char *src, LONG bytes)
{
    MONOPOLY_WEB_MMIO *stream = (MONOPOLY_WEB_MMIO *) handle;

    if (!stream || !src || bytes < 0)
        return 0;
    if (stream->position + bytes > stream->capacity)
        return 0;
    if (bytes > 0)
        memcpy(stream->buffer + stream->position, src, (size_t) bytes);
    stream->position += bytes;
    if (stream->position > stream->size)
        stream->size = stream->position;
    return bytes;
}

static inline LONG mmioSeek(HMMIO handle, LONG offset, int origin)
{
    MONOPOLY_WEB_MMIO *stream = (MONOPOLY_WEB_MMIO *) handle;
    LONG target;

    if (!stream)
        return -1;
    if (origin == 1)
        target = stream->position + offset;
    else if (origin == 2)
        target = stream->size + offset;
    else
        target = offset;
    if (target < 0)
        target = 0;
    if (target > stream->size)
        target = stream->size;
    stream->position = target;
    return stream->position;
}

static inline MMRESULT mmioDescend(HMMIO handle, LPMMCKINFO chunk, LPMMCKINFO parent, UINT flags)
{
    MONOPOLY_WEB_MMIO *stream = (MONOPOLY_WEB_MMIO *) handle;
    LONG pos;
    LONG end;

    if (!stream || !chunk)
        return 1;

    pos = stream->position;
    end = stream->size;
    if (parent)
    {
        LONG childStart = monopoly_web_mmio_chunk_data_start(parent);
        LONG parentEnd = monopoly_web_mmio_chunk_end(parent);
        if (pos < childStart)
            pos = childStart;
        if (parentEnd < end)
            end = parentEnd;
    }

    while (pos + 8 <= end)
    {
        FOURCC ckid = monopoly_web_mmio_read32(stream->buffer + pos);
        DWORD cksize = monopoly_web_mmio_read32(stream->buffer + pos + 4);
        DWORD fccType = 0;
        LONG dataOffset = pos + 8;
        LONG next = dataOffset + (LONG) cksize;
        BOOL match = FALSE;

        if (next > end)
            return 1;

        if (ckid == mmioFOURCC('R','I','F','F') || ckid == mmioFOURCC('L','I','S','T'))
        {
            if (dataOffset + 4 > end)
                return 1;
            fccType = monopoly_web_mmio_read32(stream->buffer + dataOffset);
        }

        if ((flags & MMIO_FINDRIFF) && ckid == mmioFOURCC('R','I','F','F'))
            match = (chunk->fccType == 0 || chunk->fccType == fccType);
        else if ((flags & MMIO_FINDLIST) && ckid == mmioFOURCC('L','I','S','T'))
            match = (chunk->fccType == 0 || chunk->fccType == fccType);
        else if ((flags & MMIO_FINDCHUNK) && ckid == chunk->ckid)
            match = TRUE;
        else if (!(flags & (MMIO_FINDRIFF | MMIO_FINDLIST | MMIO_FINDCHUNK)))
            match = TRUE;

        if (match)
        {
            chunk->ckid = ckid;
            chunk->cksize = cksize;
            chunk->fccType = fccType;
            chunk->dwDataOffset = (DWORD) dataOffset;
            chunk->dwFlags = 0;
            stream->position = dataOffset;
            if (ckid == mmioFOURCC('R','I','F','F') || ckid == mmioFOURCC('L','I','S','T'))
                stream->position += 4;
            return MMSYSERR_NOERROR;
        }

        pos = next + (cksize & 1);
    }

    return 1;
}
static inline MMRESULT timeGetDevCaps(void *caps, UINT)
{
    if (caps) {
        TIMECAPS *timeCaps = (TIMECAPS *)caps;
        timeCaps->wPeriodMin = 1;
        timeCaps->wPeriodMax = 1000;
    }
    return TIMERR_NOERROR;
}
static inline MMRESULT timeBeginPeriod(UINT) { return 0; }
static inline MMRESULT timeEndPeriod(UINT) { return 0; }
static inline DWORD timeGetTime(void) { return GetTickCount(); }
static inline MMRESULT timeKillEvent(UINT) { return TIMERR_NOERROR; }
static inline MMRESULT timeSetEvent(UINT, UINT, LPTIMECALLBACK, DWORD, UINT) { return 1; }
#endif
