#ifndef MONOPOLY_WEB_COMPAT_WINDOWS_H
#define MONOPOLY_WEB_COMPAT_WINDOWS_H

#include <ctype.h>
#include <dirent.h>
#include <emscripten.h>
#include <errno.h>
#include <float.h>
#include <stdarg.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include <wchar.h>
#include <wctype.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef TRUE
#define TRUE 1
#endif
#ifndef FALSE
#define FALSE 0
#endif
#ifndef NULL
#define NULL 0
#endif

#define WINAPI
#define CALLBACK
#define APIENTRY
#define PASCAL
#define __stdcall
#define __cdecl
#define FAR
#define NEAR
#define IN
#define OUT
#define CONST const
#define VOID void

typedef int BOOL;
typedef unsigned char BYTE;
typedef unsigned char UCHAR;
typedef int16_t SHORT;
typedef unsigned short WORD;
typedef unsigned long DWORD;
typedef int32_t LONG;
typedef unsigned long ULONG;
typedef unsigned int UINT;
typedef float FLOAT;
typedef int64_t LONGLONG;
typedef uint64_t DWORDLONG;
typedef LONG *LPLONG;
typedef uintptr_t DWORD_PTR;
typedef intptr_t LONG_PTR;
typedef uintptr_t UINT_PTR;
typedef intptr_t INT_PTR;
typedef DWORD LCID;
typedef DWORD LCTYPE;
typedef uintptr_t WPARAM;
typedef intptr_t LPARAM;
typedef intptr_t LRESULT;
typedef int32_t HRESULT;
typedef void *HANDLE;
typedef void *HINSTANCE;
typedef HINSTANCE HMODULE;
typedef void *HGLOBAL;
typedef void *HICON;
typedef void *HCURSOR;
typedef void *HBRUSH;
typedef void *HBITMAP;
typedef void *HFONT;
typedef void *HDC;
typedef void *HKEY;
typedef void *HMENU;
typedef void *HWND;
typedef void *HGDIOBJ;
typedef void *HPALETTE;
typedef void *HRGN;
typedef void *HMONITOR;
typedef void *LPVOID;
typedef void *PVOID;
typedef const void *LPCVOID;
typedef char CHAR;
typedef char *LPSTR;
typedef const char *LPCSTR;
typedef char TCHAR;
typedef char *LPTSTR;
typedef const char *LPCTSTR;
typedef wchar_t WCHAR;
typedef WCHAR *LPWSTR;
typedef const WCHAR *LPCWSTR;
typedef WORD *LPWORD;
typedef WORD *PWORD;
typedef BYTE *LPBYTE;
typedef DWORD *LPDWORD;
typedef unsigned short ATOM;
typedef LRESULT (CALLBACK *WNDPROC)(HWND, UINT, WPARAM, LPARAM);
typedef BOOL (CALLBACK *DLGPROC)(HWND, UINT, WPARAM, LPARAM);
typedef LONGLONG LARGE_INTEGER;
typedef struct DLGTEMPLATE {
    DWORD style;
    DWORD dwExtendedStyle;
    WORD cdit;
    SHORT x;
    SHORT y;
    SHORT cx;
    SHORT cy;
} DLGTEMPLATE, *LPDLGTEMPLATE;

typedef struct _GUID {
    DWORD Data1;
    WORD Data2;
    WORD Data3;
    BYTE Data4[8];
} GUID, IID, CLSID, *LPGUID, *LPIID, *LPCLSID;
typedef const GUID *LPCGUID;

typedef struct tagPOINT { LONG x; LONG y; } POINT, *PPOINT, *LPPOINT;
typedef struct tagSIZE { LONG cx; LONG cy; } SIZE, *PSIZE, *LPSIZE;
typedef struct tagRECT { LONG left; LONG top; LONG right; LONG bottom; } RECT, *PRECT, *LPRECT;
typedef const RECT *LPCRECT;
typedef struct _SMALL_RECT { SHORT Left; SHORT Top; SHORT Right; SHORT Bottom; } SMALL_RECT;
typedef DWORD COLORREF;
typedef struct tagRGBQUAD {
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD, *LPRGBQUAD;
typedef struct tagCOLORMAP {
    COLORREF from;
    COLORREF to;
} COLORMAP, *LPCOLORMAP;
#define LF_FACESIZE 32
typedef struct tagLOGFONTA {
    LONG lfHeight;
    LONG lfWidth;
    LONG lfEscapement;
    LONG lfOrientation;
    LONG lfWeight;
    BYTE lfItalic;
    BYTE lfUnderline;
    BYTE lfStrikeOut;
    BYTE lfCharSet;
    BYTE lfOutPrecision;
    BYTE lfClipPrecision;
    BYTE lfQuality;
    BYTE lfPitchAndFamily;
    CHAR lfFaceName[LF_FACESIZE];
} LOGFONTA, LOGFONT, *LPLOGFONTA, *LPLOGFONT;
typedef struct tagTEXTMETRICA {
    LONG tmHeight;
    LONG tmAscent;
    LONG tmDescent;
    LONG tmInternalLeading;
    LONG tmExternalLeading;
    LONG tmAveCharWidth;
    LONG tmMaxCharWidth;
    LONG tmWeight;
    LONG tmOverhang;
    LONG tmDigitizedAspectX;
    LONG tmDigitizedAspectY;
    BYTE tmFirstChar;
    BYTE tmLastChar;
    BYTE tmDefaultChar;
    BYTE tmBreakChar;
    BYTE tmItalic;
    BYTE tmUnderlined;
    BYTE tmStruckOut;
    BYTE tmPitchAndFamily;
    BYTE tmCharSet;
} TEXTMETRICA, NEWTEXTMETRICA, NEWTEXTMETRIC, *LPTEXTMETRICA;
typedef int (CALLBACK *FONTENUMPROC)(LOGFONT *, NEWTEXTMETRIC *, int, LPARAM);
typedef struct tagBITMAP {
    LONG bmType;
    LONG bmWidth;
    LONG bmHeight;
    LONG bmWidthBytes;
    WORD bmPlanes;
    WORD bmBitsPixel;
    LPVOID bmBits;
} BITMAP, *LPBITMAP;
typedef struct __attribute__((packed)) tagBITMAPFILEHEADER {
    WORD bfType;
    DWORD bfSize;
    WORD bfReserved1;
    WORD bfReserved2;
    DWORD bfOffBits;
} BITMAPFILEHEADER, *LPBITMAPFILEHEADER;
typedef struct tagBITMAPINFOHEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER, *LPBITMAPINFOHEADER;
typedef struct tagBITMAPV4HEADER {
    DWORD biSize;
    LONG biWidth;
    LONG biHeight;
    WORD biPlanes;
    WORD biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG biXPelsPerMeter;
    LONG biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
    DWORD bV4RedMask;
    DWORD bV4GreenMask;
    DWORD bV4BlueMask;
    DWORD bV4AlphaMask;
    DWORD bV4CSType;
    BYTE bV4Endpoints[36];
    DWORD bV4GammaRed;
    DWORD bV4GammaGreen;
    DWORD bV4GammaBlue;
} BITMAPV4HEADER, *LPBITMAPV4HEADER;
#define bV4Size biSize
#define bV4Width biWidth
#define bV4Height biHeight
#define bV4Planes biPlanes
#define bV4BitCount biBitCount
#define bV4V4Compression biCompression
#define bV4SizeImage biSizeImage
#define bV4XPelsPerMeter biXPelsPerMeter
#define bV4YPelsPerMeter biYPelsPerMeter
#define bV4ClrUsed biClrUsed
#define bV4ClrImportant biClrImportant
typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD bmiColors[1];
} BITMAPINFO, *LPBITMAPINFO;
typedef struct tagICONINFO {
    BOOL fIcon;
    DWORD xHotspot;
    DWORD yHotspot;
    HBITMAP hbmMask;
    HBITMAP hbmColor;
} ICONINFO, *PICONINFO, *LPICONINFO;
typedef struct tagPAINTSTRUCT {
    HDC hdc;
    BOOL fErase;
    RECT rcPaint;
    BOOL fRestore;
    BOOL fIncUpdate;
    BYTE rgbReserved[32];
} PAINTSTRUCT, *LPPAINTSTRUCT;
typedef struct tagMSG {
    HWND hwnd;
    UINT message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD time;
    POINT pt;
} MSG, *LPMSG;
typedef struct tagMINMAXINFO {
    POINT ptReserved;
    POINT ptMaxSize;
    POINT ptMaxPosition;
    POINT ptMinTrackSize;
    POINT ptMaxTrackSize;
} MINMAXINFO, *LPMINMAXINFO;
typedef struct tagWINDOWPOS {
    HWND hwnd;
    HWND hwndInsertAfter;
    int x;
    int y;
    int cx;
    int cy;
    UINT flags;
} WINDOWPOS, *LPWINDOWPOS;
typedef struct _RGNDATAHEADER {
    DWORD dwSize;
    DWORD iType;
    DWORD nCount;
    DWORD nRgnSize;
    RECT rcBound;
} RGNDATAHEADER, *LPRGNDATAHEADER;
typedef struct _RGNDATA {
    RGNDATAHEADER rdh;
    char Buffer[1];
} RGNDATA, *LPRGNDATA;
typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    LPVOID lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;
typedef struct _CRITICAL_SECTION {
    int unused;
} CRITICAL_SECTION, *LPCRITICAL_SECTION;
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME, *LPFILETIME;
typedef struct _WIN32_FIND_DATAA {
    DWORD dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD nFileSizeHigh;
    DWORD nFileSizeLow;
    DWORD dwReserved0;
    DWORD dwReserved1;
    CHAR cFileName[260];
    CHAR cAlternateFileName[14];
} WIN32_FIND_DATAA, WIN32_FIND_DATA, *LPWIN32_FIND_DATAA, *LPWIN32_FIND_DATA;
typedef struct tagOPENFILENAMEA {
    DWORD lStructSize;
    HWND hwndOwner;
    HINSTANCE hInstance;
    LPCSTR lpstrFilter;
    LPSTR lpstrCustomFilter;
    DWORD nMaxCustFilter;
    DWORD nFilterIndex;
    LPSTR lpstrFile;
    DWORD nMaxFile;
    LPSTR lpstrFileTitle;
    DWORD nMaxFileTitle;
    LPCSTR lpstrInitialDir;
    LPCSTR lpstrTitle;
    DWORD Flags;
    WORD nFileOffset;
    WORD nFileExtension;
    LPCSTR lpstrDefExt;
    LPARAM lCustData;
    void *lpfnHook;
    LPCSTR lpTemplateName;
} OPENFILENAMEA, OPENFILENAME, *LPOPENFILENAMEA, *LPOPENFILENAME;
typedef struct tagNUMBERFMTA {
    UINT NumDigits;
    UINT LeadingZero;
    UINT Grouping;
    LPSTR lpDecimalSep;
    LPSTR lpThousandSep;
    UINT NegativeOrder;
} NUMBERFMTA, NUMBERFMT, *LPNUMBERFMTA, *LPNUMBERFMT;

typedef struct MONOPOLY_WEB_FILE_HANDLE {
    FILE *file;
    DIR *dir;
    char directory[512];
} MONOPOLY_WEB_FILE_HANDLE;
typedef struct MONOPOLY_WEB_HEAP_HEADER {
    size_t size;
    uint64_t magic;
} MONOPOLY_WEB_HEAP_HEADER;
typedef struct MONOPOLY_WEB_RGN {
    RECT rect;
} MONOPOLY_WEB_RGN;

#define MAX_PATH 260
#define _MAX_PATH MAX_PATH
#define _MAX_DRIVE 8
#define _MAX_DIR 256
#define _MAX_FNAME 256
#define _MAX_EXT 256
#define INVALID_HANDLE_VALUE ((HANDLE)(intptr_t)-1)
#define INVALID_FILE_ATTRIBUTES ((DWORD)-1)
#define HKEY_LOCAL_MACHINE ((HKEY)(intptr_t)1)
#define ERROR_SUCCESS 0L
#define KEY_READ 0x00020019u
#define REG_BINARY 3u
#define GENERIC_READ 0x80000000u
#define GENERIC_WRITE 0x40000000u
#define FILE_SHARE_READ 0x00000001u
#define FILE_SHARE_WRITE 0x00000002u
#define CREATE_NEW 1
#define CREATE_ALWAYS 2
#define OPEN_EXISTING 3
#define OPEN_ALWAYS 4
#define TRUNCATE_EXISTING 5
#define FILE_ATTRIBUTE_READONLY 0x00000001u
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010u
#define FILE_ATTRIBUTE_NORMAL 0x00000080u
#define FILE_FLAG_SEQUENTIAL_SCAN 0x08000000u
#define DRIVE_CDROM 5
#define HEAP_NO_SERIALIZE 0x00000001u
#define GMEM_MOVEABLE 0x0002
#define GMEM_ZEROINIT 0x0040
#define LPTR GMEM_ZEROINIT
#define _RC_NEAR 0x00000000u
#define _MCW_RC 0x00000300u

#define S_OK ((HRESULT)0)
#define S_FALSE ((HRESULT)1)
#define E_FAIL ((HRESULT)0x80004005)
#define FAILED(hr) ((HRESULT)(hr) < 0)
#define SUCCEEDED(hr) ((HRESULT)(hr) >= 0)

#define WM_USER 0x0400
#define WM_CLOSE 0x0010
#define WM_SIZE 0x0005
#define WM_ACTIVATEAPP 0x001C
#define WM_GETMINMAXINFO 0x0024
#define WM_WINDOWPOSCHANGING 0x0046
#define WM_NCPAINT 0x0085
#define WM_SYSCOMMAND 0x0112
#define WM_ENTERMENULOOP 0x0211
#define WM_EXITMENULOOP 0x0212
#define WM_DESTROY 0x0002
#define WM_ERASEBKGND 0x0014
#define WM_PAINT 0x000F
#define WM_QUIT 0x0012
#define WM_CHAR 0x0102
#define WM_KEYDOWN 0x0100
#define WM_KEYUP 0x0101
#define WM_LBUTTONDOWN 0x0201
#define WM_LBUTTONUP 0x0202
#define WM_RBUTTONDOWN 0x0204
#define WM_RBUTTONUP 0x0205
#define WM_MBUTTONDOWN 0x0207
#define WM_MBUTTONUP 0x0208
#define WM_MOUSEMOVE 0x0200
#define WM_SETFONT 0x0030
#define VK_SHIFT 0x10
#define VK_CONTROL 0x11
#define VK_MENU 0x12
#define VK_CAPITAL 0x14
#define VK_ESCAPE 0x1B
#define VK_SPACE 0x20
#define PM_REMOVE 0x0001
#define MB_OK 0x00000000u
#define MB_OKCANCEL 0x00000001u
#define MB_ABORTRETRYIGNORE 0x00000002u
#define MB_YESNO 0x00000004u
#define MB_RETRYCANCEL 0x00000005u
#define MB_ICONHAND 0x00000010u
#define MB_ICONERROR MB_ICONHAND
#define MB_ICONQUESTION 0x00000020u
#define MB_ICONEXCLAMATION 0x00000030u
#define MB_ICONWARNING MB_ICONEXCLAMATION
#define MB_ICONINFORMATION 0x00000040u
#define MB_DEFBUTTON1 0x00000000u
#define MB_DEFBUTTON2 0x00000100u
#define MB_SYSTEMMODAL 0x00001000u
#define MB_TASKMODAL 0x00002000u
#define MB_SETFOREGROUND 0x00010000u
#define IDOK 1
#define IDCANCEL 2
#define IDABORT 3
#define IDRETRY 4
#define IDIGNORE 5
#define IDYES 6
#define IDNO 7
#define WM_INITDIALOG 0x0110
#define WM_COMMAND 0x0111
#define WM_TIMER 0x0113
#define GWL_WNDPROC (-4)
#define WS_POPUP 0x80000000u
#define WS_POPUPWINDOW 0x80880000u
#define WS_CAPTION 0x00C00000u
#define WS_VISIBLE 0x10000000u
#define WS_CHILD 0x40000000u
#define WS_VSCROLL 0x00200000u
#define WS_TABSTOP 0x00010000u
#define DS_MODALFRAME 0x80
#define DS_CENTER 0x0800
#define BS_DEFPUSHBUTTON 0x00000001u
#define BS_AUTOCHECKBOX 0x00000003u
#define CBS_DROPDOWNLIST 0x00000003u
#define ES_AUTOHSCROLL 0x0080u
#define HELP_CONTENTS 0x0003u
#define EM_SETSEL 0x00B1
#define EM_LIMITTEXT 0x00C5
#define EM_SETREADONLY 0x00CF
#define EM_SETMARGINS 0x00D3
#define EC_LEFTMARGIN 0x0001
#define EC_RIGHTMARGIN 0x0002
#define LBN_DBLCLK 2
#define LB_GETCOUNT 0x018B
#define LOBYTE(value) ((BYTE)((WORD)(value) & 0xff))
#define HIBYTE(value) ((BYTE)(((WORD)(value) >> 8) & 0xff))
#define LB_ADDSTRING 0x0180
#define LB_RESETCONTENT 0x0184
#define LB_GETCURSEL 0x0188
#define LB_SETCURSEL 0x0186
#define LB_SETITEMDATA 0x019A
#define LB_GETITEMDATA 0x0199
#define LB_ERR (-1)
#define BST_UNCHECKED 0
#define BST_CHECKED 1
#define IDC_ARROW ((LPCSTR)32512)
#define IDC_IBEAM ((LPCSTR)32513)
#define SW_HIDE 0
#define SW_SHOWNORMAL 1
#define SW_SHOW 5
#define SW_MINIMIZE 6
#define SW_RESTORE 9
#define SW_SHOWDEFAULT 10
#define SWP_NOSIZE 0x0001
#define SWP_NOMOVE 0x0002
#define MF_UNCHECKED 0x00000000u
#define MF_CHECKED 0x00000008u
#define MF_GRAYED 0x00000001u
#define SC_CLOSE 0xF060u
#define THREAD_PRIORITY_LOWEST -2
#define THREAD_PRIORITY_BELOW_NORMAL -1
#define INFINITE 0xffffffffu
#define WAIT_OBJECT_0 0
#define RGN_AND 1
#define RGN_OR 2
#define RGN_XOR 3
#define RGN_DIFF 4
#define RGN_COPY 5
#define RGN_ERROR 0
#define NULLREGION 1
#define SIMPLEREGION 2
#define COMPLEXREGION 3
#define RDH_RECTANGLES 1
#define BI_RGB 0
#define BI_BITFIELDS 3
#define DIB_RGB_COLORS 0
#define DIB_PAL_COLORS 1
#define OFN_READONLY 0x00000001u
#define OFN_OVERWRITEPROMPT 0x00000002u
#define OFN_HIDEREADONLY 0x00000004u
#define OFN_NOCHANGEDIR 0x00000008u
#define OFN_NOREADONLYRETURN 0x00008000u
#define OFN_NOTESTFILECREATE 0x00010000u
#define OFN_NOVALIDATE 0x00000100u
#define OFN_FILEMUSTEXIST 0x00001000u
#define OFN_PATHMUSTEXIST 0x00000800u
#define OFN_LONGNAMES 0x00200000u
#define LOCALE_USER_DEFAULT ((LCID)0x0400)
#define LOCALE_SDECIMAL 0x0000000Eu
#define LOCALE_STHOUSAND 0x0000000Fu
#define LOCALE_SGROUPING 0x00000010u
#define LOCALE_INEGNUMBER 0x00001010u
#define LOCALE_SENGLANGUAGE 0x00001001u
#define LOCALE_SENGCOUNTRY 0x00001002u
#define ETO_CLIPPED 0x0004
#define ETO_OPAQUE 0x0002
#define TRANSPARENT 1
#define OPAQUE 2
#define SRCCOPY 0x00CC0020
#define IMAGE_BITMAP 0
#define LR_LOADFROMFILE 0x00000010
#define LR_CREATEDIBSECTION 0x00002000
#define DEVICE_FONTTYPE 0x002
#define RASTER_FONTTYPE 0x001
#define LOGPIXELSX 88
#define LOGPIXELSY 90
#define HORZRES 8
#define VERTRES 10
#define BITSPIXEL 12
#define ANSI_CHARSET 0
#define DEFAULT_CHARSET 1
#define OUT_DEFAULT_PRECIS 0
#define OUT_TT_PRECIS 4
#define CLIP_DEFAULT_PRECIS 0
#define DEFAULT_QUALITY 0
#define DRAFT_QUALITY 1
#define NONANTIALIASED_QUALITY 3
#define DEFAULT_PITCH 0
#define VARIABLE_PITCH 2
#define FF_DONTCARE 0
#define FW_NORMAL 400
#define RGB(r, g, b) ((COLORREF)(((BYTE)(r)) | ((WORD)((BYTE)(g)) << 8) | (((DWORD)(BYTE)(b)) << 16)))
#define GetRValue(rgb) ((BYTE)((rgb) & 0xff))
#define GetGValue(rgb) ((BYTE)(((rgb) >> 8) & 0xff))
#define GetBValue(rgb) ((BYTE)(((rgb) >> 16) & 0xff))
#define DIBINDEX(index) ((COLORREF)(index))

#define ZeroMemory(ptr, size) memset((ptr), 0, (size))
#define CopyMemory(dst, src, size) memcpy((dst), (src), (size))
#define FillMemory(dst, size, val) memset((dst), (val), (size))
#define LOWORD(value) ((WORD)((DWORD_PTR)(value) & 0xffff))
#define HIWORD(value) ((WORD)((DWORD_PTR)(value) >> 16))
#define MAKELONG(low, high) ((LONG)(((WORD)(low)) | ((DWORD)((WORD)(high))) << 16))
#define MAKELPARAM(low, high) ((LPARAM)MAKELONG((low), (high)))
#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif
#define MAKEINTRESOURCE(value) ((LPCSTR)((uintptr_t)((WORD)(value))))
#define GET_WM_COMMAND_ID(wp, lp) LOWORD(wp)
#define GET_WM_COMMAND_CMD(wp, lp) HIWORD(wp)
#define GET_WM_COMMAND_HWND(wp, lp) ((HWND)(lp))
#define lstrcpy strcpy
#define lstrcpyn strncpy
#define lstrcat strcat
#define lstrlen strlen
#define wsprintf sprintf
#define wvsprintf(buffer, format, args) vsprintf((buffer), (format), (args))
#define hmemcpy memcpy
#define _strupr strupr
#define _tcsrchr strrchr
#define _tcsstr strstr
#define _tcslen strlen
#define _tcscpy strcpy
#define _tcscat strcat
#define _tcstoul strtoul
#define _tcstok strtok
#define _stprintf sprintf
#define _ultot _ultoa
#define stricmp strcasecmp
#define _stricmp strcasecmp
#define strnicmp strncasecmp
#define _strnicmp strncasecmp
#define wcslen monopoly_web_wcslen
#define wcscpy monopoly_web_wcscpy
#define wcsncpy monopoly_web_wcsncpy
#define wcscat monopoly_web_wcscat
#define wcsncat monopoly_web_wcsncat
#define wcscmp monopoly_web_wcscmp
#define wcsicmp monopoly_web_wcsicmp
#define _wcsicmp monopoly_web_wcsicmp
#define mbstowcs monopoly_web_mbstowcs
#define wcstombs monopoly_web_wcstombs
#define swprintf(buffer, format, ...) monopoly_web_swprintf((buffer), 1024, (format), ##__VA_ARGS__)
#define vswprintf(buffer, format, args) monopoly_web_vswprintf((buffer), 1024, (format), (args))
#define TEXT(value) value
#define OutputDebugStringA(msg) monopoly_web_OutputDebugStringA(msg)
#define OutputDebugString OutputDebugStringA
#define GetPrivateProfileString GetPrivateProfileStringA
#define GetPrivateProfileInt GetPrivateProfileIntA
#define WritePrivateProfileString WritePrivateProfileStringA
#define GetModuleFileName GetModuleFileNameA
#define GetWindowsDirectory GetWindowsDirectoryA
#define GetFullPathName GetFullPathNameA
#define GetLogicalDriveStrings GetLogicalDriveStringsA
#define GetDriveType GetDriveTypeA
#define GetVolumeInformation GetVolumeInformationA
#define GetLocaleInfo GetLocaleInfoA
#define GetNumberFormat GetNumberFormatA
#define GetCurrencyFormat GetCurrencyFormatA
#define GetComputerName GetComputerNameA
#define GetUserName GetUserNameA
#define RegOpenKeyEx RegOpenKeyExA
#define RegQueryValueEx RegQueryValueExA
#define RegCloseKey RegCloseKeyA
#define CreateFile CreateFileA
#define CreateDirectory CreateDirectoryA
#define DeleteFile DeleteFileA
#define FindFirstFile FindFirstFileA
#define SetWindowText SetWindowTextA
#define GetWindowText GetWindowTextA
#define MessageBox MessageBoxA
#define GetOpenFileName GetOpenFileNameA
#define GetSaveFileName GetSaveFileNameA
#define CreateWindow CreateWindowA
#define CreateWindowEx CreateWindowExA
#define CreateFont CreateFontA
#define CreateFontIndirect CreateFontIndirectA
#define GetTextMetrics GetTextMetricsA
#define GetTextExtentPoint32 GetTextExtentPoint32A
#define GetTextExtentPoint GetTextExtentPoint32A
#define ExtTextOut ExtTextOutA
#define GetObject GetObjectA
#define LoadImage LoadImageA
#define EnumFontFamilies EnumFontFamiliesA
#define AddFontResource AddFontResourceA
#define RemoveFontResource RemoveFontResourceA
#define SendDlgItemMessage SendDlgItemMessageA
#define SetDlgItemText SetDlgItemTextA
#define GetDlgItemText GetDlgItemTextA
#define DialogBox DialogBoxA
#define DialogBoxParam DialogBoxParamA
#define DialogBoxIndirect DialogBoxIndirectA
#define GetModuleHandle GetModuleHandleA
#define WinHelp WinHelpA
#define SendMessage SendMessageA
#define PostMessage PostMessageA
#define PeekMessage PeekMessageA
#define GetMessage GetMessageA
#define DispatchMessage DispatchMessageA
#define DefWindowProc DefWindowProcA
#define GetWindowLong GetWindowLongA
#define SetWindowLong SetWindowLongA
#define CallWindowProc CallWindowProcA
#define DeleteObject monopoly_web_DeleteObject
#define GlobalAllocPtr(flags, bytes) calloc(1, (bytes))
#define GlobalFreePtr(ptr) free(ptr)
#define GlobalPtrHandle(ptr) (ptr)
#define GHND 0
#define CLSCTX_INPROC_SERVER 1

extern void monopoly_web_log(const char *message);
extern int monopoly_web_profile_get(const char *file, const char *section, const char *key, char *buffer, size_t size);
extern int monopoly_web_profile_set(const char *file, const char *section, const char *key, const char *value);
extern void monopoly_web_profile_clear(const char *file);
extern int monopoly_web_canvas_measure_text_width(const char *text, int count, int pixelHeight, int weight);
extern int monopoly_web_canvas_raster_text(const char *text, int count, int pixelHeight, int weight,
                                           unsigned int colorRef, unsigned char *outRgba,
                                           int outWidth, int outHeight);

static const GUID GUID_NULL = {0};

#define IsEqualGUID(a, b) (memcmp(&(a), &(b), sizeof(GUID)) == 0)

static inline size_t monopoly_web_wcslen(const wchar_t *value)
{
    size_t length = 0;
    if (!value) return 0;
    while (value[length]) ++length;
    return length;
}

static inline wchar_t *monopoly_web_wcscpy(wchar_t *dest, const wchar_t *src)
{
    wchar_t *out = dest;
    if (!dest) return NULL;
    if (!src) {
        dest[0] = 0;
        return out;
    }
    while ((*dest++ = *src++) != 0) {}
    return out;
}

static inline wchar_t *monopoly_web_wcsncpy(wchar_t *dest, const wchar_t *src, size_t count)
{
    size_t i = 0;
    if (!dest) return NULL;
    if (src) {
        for (; i < count && src[i]; ++i) dest[i] = src[i];
    }
    for (; i < count; ++i) dest[i] = 0;
    return dest;
}

static inline wchar_t *monopoly_web_wcscat(wchar_t *dest, const wchar_t *src)
{
    if (!dest) return NULL;
    monopoly_web_wcscpy(dest + monopoly_web_wcslen(dest), src);
    return dest;
}

static inline wchar_t *monopoly_web_wcsncat(wchar_t *dest, const wchar_t *src, size_t count)
{
    size_t offset;
    size_t i = 0;
    if (!dest) return NULL;
    offset = monopoly_web_wcslen(dest);
    if (src) {
        for (; i < count && src[i]; ++i) dest[offset + i] = src[i];
    }
    dest[offset + i] = 0;
    return dest;
}

static inline int monopoly_web_wcscmp(const wchar_t *left, const wchar_t *right)
{
    if (!left && !right) return 0;
    if (!left) return -1;
    if (!right) return 1;
    while (*left && *right && *left == *right) {
        ++left;
        ++right;
    }
    return (int)*left - (int)*right;
}

static inline int monopoly_web_wcsicmp(const wchar_t *left, const wchar_t *right)
{
    if (!left && !right) return 0;
    if (!left) return -1;
    if (!right) return 1;
    while (*left && *right) {
        wchar_t a = *left++;
        wchar_t b = *right++;
        if (a >= L'A' && a <= L'Z') a = (wchar_t)(a - L'A' + L'a');
        if (b >= L'A' && b <= L'Z') b = (wchar_t)(b - L'A' + L'a');
        if (a != b) return (int)a - (int)b;
    }
    wchar_t a = *left;
    wchar_t b = *right;
    if (a >= L'A' && a <= L'Z') a = (wchar_t)(a - L'A' + L'a');
    if (b >= L'A' && b <= L'Z') b = (wchar_t)(b - L'A' + L'a');
    return (int)a - (int)b;
}

static inline size_t monopoly_web_mbstowcs(wchar_t *dest, const char *src, size_t count)
{
    size_t i = 0;
    if (!src) return (size_t)-1;
    if (!dest) return strlen(src);
    if (count == 0) return 0;
    while (i + 1 < count && src[i]) {
        dest[i] = (wchar_t)(unsigned char)src[i];
        ++i;
    }
    dest[i] = 0;
    while (src[i]) ++i;
    return i;
}

static inline size_t monopoly_web_wcstombs(char *dest, const wchar_t *src, size_t count)
{
    size_t i = 0;
    if (!src) return (size_t)-1;
    if (!dest) return monopoly_web_wcslen(src);
    if (count == 0) return 0;
    while (i + 1 < count && src[i]) {
        wchar_t c = src[i];
        dest[i] = (c >= 0 && c < 256) ? (char)c : '?';
        ++i;
    }
    dest[i] = '\0';
    while (src[i]) ++i;
    return i;
}

static inline WCHAR *_ltow(long value, WCHAR *buffer, int radix)
{
    char tmp[64];
    if (!buffer) return NULL;
    if (radix == 16)
        snprintf(tmp, sizeof(tmp), "%lx", value);
    else
        snprintf(tmp, sizeof(tmp), "%ld", value);
    mbstowcs(buffer, tmp, strlen(tmp) + 1);
    return buffer;
}

static inline char *_ultoa(unsigned long value, char *buffer, int radix)
{
    if (!buffer) return NULL;
    if (radix == 16)
        sprintf(buffer, "%lx", value);
    else
        sprintf(buffer, "%lu", value);
    return buffer;
}

static inline int _wtoi(const wchar_t *value)
{
    int sign = 1;
    int result = 0;
    if (!value) return 0;
    while (*value == L' ' || *value == L'\t') ++value;
    if (*value == L'-') {
        sign = -1;
        ++value;
    }
    while (*value >= L'0' && *value <= L'9') {
        result = result * 10 + (int)(*value - L'0');
        ++value;
    }
    return result * sign;
}

static inline void monopoly_web_wide_to_narrow(char *out, size_t outSize, const wchar_t *in)
{
    if (!out || outSize == 0) return;
    size_t i = 0;
    if (in) {
        while (i + 1 < outSize && in[i]) {
            out[i] = (char)(in[i] & 0xff);
            i++;
        }
    }
    out[i] = '\0';
}

static inline void monopoly_web_narrow_to_wide(wchar_t *out, size_t outSize, const char *in)
{
    if (!out || outSize == 0) return;
    size_t i = 0;
    if (in) {
        while (i + 1 < outSize && in[i]) {
            out[i] = (wchar_t)(unsigned char)in[i];
            i++;
        }
    }
    out[i] = 0;
}

static inline int monopoly_web_vswprintf(wchar_t *buffer, size_t bufferCount, const wchar_t *format, va_list args)
{
    char narrowFormat[512];
    char narrowOutput[1024];
    monopoly_web_wide_to_narrow(narrowFormat, sizeof(narrowFormat), format);
    int written = vsnprintf(narrowOutput, sizeof(narrowOutput), narrowFormat, args);
    monopoly_web_narrow_to_wide(buffer, bufferCount, narrowOutput);
    return written;
}

static inline int monopoly_web_swprintf(wchar_t *buffer, size_t bufferCount, const wchar_t *format, ...)
{
    va_list args;
    va_start(args, format);
    int written = monopoly_web_vswprintf(buffer, bufferCount, format, args);
    va_end(args);
    return written;
}

static inline int GetLocaleInfoA(LCID, LCTYPE type, LPSTR buffer, int size)
{
    const char *value = "";
    switch (type) {
        case LOCALE_SGROUPING: value = "3"; break;
        case LOCALE_SDECIMAL: value = "."; break;
        case LOCALE_STHOUSAND: value = ","; break;
        case LOCALE_INEGNUMBER: value = "1"; break;
        case LOCALE_SENGLANGUAGE: value = "English"; break;
        case LOCALE_SENGCOUNTRY: value = "United States"; break;
        default: value = ""; break;
    }
    if (buffer && size > 0) {
        strncpy(buffer, value, (size_t)size);
        buffer[size - 1] = '\0';
    }
    return (int)strlen(value) + 1;
}

static inline int GetNumberFormatA(LCID, DWORD, LPCSTR value, const NUMBERFMTA *, LPSTR output, int size)
{
    const char *text = value ? value : "";
    if (output && size > 0) {
        strncpy(output, text, (size_t)size);
        output[size - 1] = '\0';
    }
    return (int)strlen(text) + 1;
}

static inline int GetCurrencyFormatA(LCID, DWORD, LPCSTR value, const void *, LPSTR output, int size)
{
    char formatted[256];
    snprintf(formatted, sizeof(formatted), "$%s", value ? value : "0");
    if (output && size > 0) {
        strncpy(output, formatted, (size_t)size);
        output[size - 1] = '\0';
    }
    return (int)strlen(formatted) + 1;
}

static inline SHORT GetKeyState(int)
{
    return 0;
}

static inline DWORD GetTickCount(void)
{
    return (DWORD)emscripten_get_now();
}

static inline void _splitpath(const char *path, char *drive, char *dir, char *fname, char *ext)
{
    if (drive) drive[0] = '\0';
    if (dir) dir[0] = '\0';
    if (fname) fname[0] = '\0';
    if (ext) ext[0] = '\0';
    if (!path) return;

    const char *start = path;
    if (isalpha((unsigned char)path[0]) && path[1] == ':') {
        if (drive) {
            drive[0] = path[0];
            drive[1] = ':';
            drive[2] = '\0';
        }
        start = path + 2;
    }

    const char *lastSlash = strrchr(start, '/');
    const char *lastBackslash = strrchr(start, '\\');
    if (!lastSlash || (lastBackslash && lastBackslash > lastSlash)) lastSlash = lastBackslash;
    const char *nameStart = lastSlash ? lastSlash + 1 : start;

    if (dir) {
        size_t dirLen = lastSlash ? (size_t)(lastSlash + 1 - start) : 0;
        if (dirLen >= _MAX_DIR) dirLen = _MAX_DIR - 1;
        memcpy(dir, start, dirLen);
        dir[dirLen] = '\0';
    }

    const char *lastDot = strrchr(nameStart, '.');
    if (lastDot && lastDot != nameStart) {
        if (fname) {
            size_t nameLen = (size_t)(lastDot - nameStart);
            if (nameLen >= _MAX_FNAME) nameLen = _MAX_FNAME - 1;
            memcpy(fname, nameStart, nameLen);
            fname[nameLen] = '\0';
        }
        if (ext) {
            strncpy(ext, lastDot, _MAX_EXT - 1);
            ext[_MAX_EXT - 1] = '\0';
        }
    } else if (fname) {
        strncpy(fname, nameStart, _MAX_FNAME - 1);
        fname[_MAX_FNAME - 1] = '\0';
    }
}

static inline void monopoly_web_normalize_path(char *out, size_t outSize, LPCSTR path)
{
    if (!out || outSize == 0) return;
    out[0] = '\0';
    if (!path || !path[0]) return;

    if (strncasecmp(path, "CD:", 3) == 0) {
        const char *suffix = path + 3;
        while (*suffix == '\\' || *suffix == '/') suffix++;
        snprintf(out, outSize, "/cdrom/%s", suffix);
    } else if (isalpha((unsigned char)path[0]) && path[1] == ':') {
        const char *suffix = path + 2;
        while (*suffix == '\\' || *suffix == '/') suffix++;
        snprintf(out, outSize, "/monopoly-user/%s", suffix);
    } else {
        strncpy(out, path, outSize - 1);
        out[outSize - 1] = '\0';
    }

    for (char *p = out; *p; ++p) {
        if (*p == '\\') *p = '/';
    }

    char collapsed[MAX_PATH * 4];
    size_t write = 0;
    for (size_t read = 0; out[read] && write + 1 < sizeof(collapsed); ++read) {
        if (out[read] == '/' && write > 0 && collapsed[write - 1] == '/') continue;
        collapsed[write++] = out[read];
    }
    collapsed[write] = '\0';
    strncpy(out, collapsed, outSize - 1);
    out[outSize - 1] = '\0';

    if (strncasecmp(out, "/cdrom", 6) == 0) {
        for (char *p = out; *p; ++p) *p = (char)tolower((unsigned char)*p);
    }
}

static inline BOOL QueryPerformanceFrequency(LARGE_INTEGER *frequency)
{
    if (frequency) *frequency = 1000;
    return TRUE;
}

static inline BOOL QueryPerformanceCounter(LARGE_INTEGER *counter)
{
    if (counter) *counter = (LARGE_INTEGER)emscripten_get_now();
    return TRUE;
}

static inline void Sleep(DWORD milliseconds)
{
    (void)milliseconds;
}

static inline unsigned int _controlfp(unsigned int newValue, unsigned int)
{
    return newValue;
}

static inline HANDLE HeapCreate(DWORD, size_t, size_t)
{
    return malloc(1);
}

static inline BOOL HeapDestroy(HANDLE heap)
{
    free(heap);
    return TRUE;
}

static inline LPVOID HeapAlloc(HANDLE, DWORD flags, size_t bytes)
{
    size_t payloadSize = bytes ? bytes : 1;
    MONOPOLY_WEB_HEAP_HEADER *header =
        (MONOPOLY_WEB_HEAP_HEADER *)malloc(sizeof(*header) + payloadSize);
    if (!header) return NULL;
    header->size = bytes;
    header->magic = 0x4d4f4e4f48454150ULL;
    void *payload = header + 1;
    if (flags & GMEM_ZEROINIT) memset(payload, 0, payloadSize);
    return payload;
}

static inline BOOL HeapFree(HANDLE, DWORD, LPVOID ptr)
{
    if (!ptr) return TRUE;
    MONOPOLY_WEB_HEAP_HEADER *header = ((MONOPOLY_WEB_HEAP_HEADER *)ptr) - 1;
    if (header->magic != 0x4d4f4e4f48454150ULL) {
        free(ptr);
        return TRUE;
    }
    header->magic = 0;
    free(header);
    return TRUE;
}

static inline size_t HeapSize(HANDLE, DWORD, LPCVOID ptr)
{
    if (!ptr) return (size_t)-1;
    const MONOPOLY_WEB_HEAP_HEADER *header = ((const MONOPOLY_WEB_HEAP_HEADER *)ptr) - 1;
    return header->magic == 0x4d4f4e4f48454150ULL ? header->size : 0;
}

static inline size_t HeapCompact(HANDLE, DWORD)
{
    return 0;
}

static inline HMODULE LoadLibraryA(LPCSTR)
{
    return NULL;
}

static inline BOOL FreeLibrary(HMODULE)
{
    return TRUE;
}

static inline void *GetProcAddress(HMODULE, LPCSTR)
{
    return NULL;
}

static inline DWORD GetLastError(void)
{
    return 0;
}

#define LoadLibrary LoadLibraryA
#define LoadCursor LoadCursorA

static inline void monopoly_web_OutputDebugStringA(LPCSTR text)
{
    if (text) monopoly_web_log(text);
}

static inline int MessageBoxA(HWND, LPCSTR text, LPCSTR caption, UINT)
{
    if (caption) monopoly_web_log(caption);
    if (text) monopoly_web_log(text);
    return IDOK;
}

static inline BOOL MessageBeep(UINT) { return TRUE; }

static inline BOOL SetWindowTextA(HWND, LPCSTR text)
{
    if (text) monopoly_web_log(text);
    return TRUE;
}

static inline HWND CreateWindowExA(DWORD, LPCSTR, LPCSTR, DWORD, int, int, int, int, HWND, HMENU, HINSTANCE, LPVOID)
{
    return calloc(1, 1);
}
static inline HWND CreateWindowA(LPCSTR className, LPCSTR title, DWORD style, int x, int y, int width, int height, HWND parent, HMENU menu, HINSTANCE instance, LPVOID param)
{
    return CreateWindowExA(0, className, title, style, x, y, width, height, parent, menu, instance, param);
}
static inline BOOL DestroyWindow(HWND hwnd) { if (hwnd) free(hwnd); return TRUE; }
static inline BOOL IsWindow(HWND hwnd) { return hwnd != NULL; }
static inline BOOL UpdateWindow(HWND hwnd) { (void)hwnd; return TRUE; }
static inline BOOL PostMessageA(HWND, UINT, WPARAM, LPARAM) { return TRUE; }
static inline LRESULT SendMessageA(HWND, UINT, WPARAM, LPARAM) { return 0; }
static inline UINT GetWindowTextA(HWND, LPSTR buffer, int size) { if (buffer && size > 0) buffer[0] = '\0'; return 0; }
static inline HWND GetActiveWindow(void) { return NULL; }
static inline LONG GetWindowLongA(HWND, int) { return 0; }
static inline LONG SetWindowLongA(HWND, int, LONG value) { return value; }
static inline LRESULT CallWindowProcA(WNDPROC proc, HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    return proc ? proc(hwnd, message, wParam, lParam) : 0;
}
static inline LRESULT SendDlgItemMessageA(HWND, int, UINT, WPARAM, LPARAM) { return LB_ERR; }
static inline BOOL SetDlgItemTextA(HWND, int, LPCSTR) { return TRUE; }
static inline UINT GetDlgItemTextA(HWND, int, LPSTR buffer, int size) { if (buffer && size > 0) buffer[0] = '\0'; return 0; }
static inline BOOL SetDlgItemInt(HWND, int, UINT, BOOL) { return TRUE; }
static inline UINT GetDlgItemInt(HWND, int, BOOL *, BOOL) { return 0; }
static inline BOOL CheckDlgButton(HWND, int, UINT) { return TRUE; }
static inline UINT IsDlgButtonChecked(HWND, int) { return BST_UNCHECKED; }
static inline HWND GetDlgItem(HWND, int) { return NULL; }
static inline BOOL EnableWindow(HWND, BOOL) { return TRUE; }
static inline BOOL MoveWindow(HWND, int, int, int, int, BOOL) { return TRUE; }
static inline BOOL ShowWindow(HWND, int) { return TRUE; }
static inline UINT CheckMenuItem(HMENU, UINT, UINT) { return 0; }
static inline BOOL EnableMenuItem(HMENU, UINT, UINT) { return TRUE; }
static inline HMENU GetSystemMenu(HWND, BOOL) { return NULL; }
static inline UINT_PTR SetTimer(HWND, UINT_PTR id, UINT, void *) { return id; }
static inline BOOL KillTimer(HWND, UINT_PTR) { return TRUE; }
static inline HWND SetFocus(HWND hwnd) { return hwnd; }
static inline int DialogBoxA(HINSTANCE, LPCSTR, HWND, DLGPROC) { return FALSE; }
static inline int DialogBoxParamA(HINSTANCE, LPCSTR, HWND, DLGPROC proc, LPARAM param)
{
    return proc ? proc(NULL, WM_INITDIALOG, 0, param) : FALSE;
}
static inline int DialogBoxIndirectA(HINSTANCE, LPDLGTEMPLATE, HWND, DLGPROC proc)
{
    return proc ? proc(NULL, WM_INITDIALOG, 0, 0) : FALSE;
}
static inline HMODULE GetModuleHandleA(LPCSTR) { return (HMODULE)1; }
static inline BOOL GetOpenFileNameA(LPOPENFILENAMEA) { return FALSE; }
static inline BOOL GetSaveFileNameA(LPOPENFILENAMEA) { return FALSE; }
static inline BOOL EndDialog(HWND, int) { return TRUE; }
static inline BOOL IsBadReadPtr(const void *ptr, UINT_PTR bytes) { return ptr == NULL && bytes != 0; }
static inline BOOL IsBadWritePtr(LPVOID ptr, UINT_PTR bytes) { return ptr == NULL && bytes != 0; }
static inline BOOL PeekMessageA(LPMSG, HWND, UINT, UINT, UINT) { return FALSE; }
static inline BOOL GetMessageA(LPMSG, HWND, UINT, UINT) { return FALSE; }
static inline void PostQuitMessage(int) {}
static inline BOOL TranslateMessage(const MSG *) { return TRUE; }
static inline LRESULT DispatchMessageA(const MSG *) { return 0; }
static inline LRESULT DefWindowProcA(HWND, UINT, WPARAM, LPARAM) { return 0; }
static inline BOOL InvalidateRect(HWND, const RECT *, BOOL) { return TRUE; }
static inline HDC BeginPaint(HWND, LPPAINTSTRUCT paint)
{
    if (paint) ZeroMemory(paint, sizeof(*paint));
    return NULL;
}
static inline BOOL EndPaint(HWND, const PAINTSTRUCT *) { return TRUE; }

#define MONOPOLY_WEB_GDI_DC_MAGIC 0x57444331u
#define MONOPOLY_WEB_GDI_BITMAP_MAGIC 0x57424d31u
#define MONOPOLY_WEB_GDI_FONT_MAGIC 0x57464e31u

typedef struct MONOPOLY_WEB_GDI_BITMAP {
    DWORD magic;
    LONG width;
    LONG height;
    WORD bpp;
    BOOL top_down;
    BOOL owns_bits;
    UINT palette_entries;
    RGBQUAD palette[256];
    size_t stride;
    size_t size;
    void *bits;
} MONOPOLY_WEB_GDI_BITMAP;

typedef struct MONOPOLY_WEB_GDI_FONT {
    DWORD magic;
    LOGFONTA logfont;
    int pixel_height;
    int weight;
} MONOPOLY_WEB_GDI_FONT;

typedef struct MONOPOLY_WEB_GDI_DC {
    DWORD magic;
    MONOPOLY_WEB_GDI_BITMAP *bitmap;
    MONOPOLY_WEB_GDI_FONT *font;
    COLORREF text_color;
    COLORREF bk_color;
    int bk_mode;
} MONOPOLY_WEB_GDI_DC;

static inline int monopoly_web_gdi_font_height(const MONOPOLY_WEB_GDI_DC *dc)
{
    int height = 16;
    if (dc && dc->font && dc->font->magic == MONOPOLY_WEB_GDI_FONT_MAGIC && dc->font->pixel_height > 0) {
        height = dc->font->pixel_height;
    }
    if (height < 7) height = 7;
    if (height > 48) height = 48;
    return height;
}

static inline int monopoly_web_gdi_font_scale(const MONOPOLY_WEB_GDI_DC *dc)
{
    int scale = monopoly_web_gdi_font_height(dc) / 8;
    return scale < 1 ? 1 : scale;
}

static inline int monopoly_web_gdi_cell_width(const MONOPOLY_WEB_GDI_DC *dc)
{
    int scale = monopoly_web_gdi_font_scale(dc);
    return 5 * scale + scale;
}

static inline int monopoly_web_gdi_text_height(const MONOPOLY_WEB_GDI_DC *dc)
{
    return monopoly_web_gdi_font_height(dc);
}

static inline BYTE monopoly_web_gdi_expand_bits(DWORD value, DWORD bits)
{
    if (bits == 0) return 0;
    DWORD max_value = (1u << bits) - 1u;
    return (BYTE)((value * 255u + (max_value / 2u)) / max_value);
}

static inline DWORD monopoly_web_gdi_mask_shift(DWORD mask)
{
    DWORD shift = 0;
    if (!mask) return 0;
    while ((mask & 1u) == 0) {
        mask >>= 1;
        ++shift;
    }
    return shift;
}

static inline DWORD monopoly_web_gdi_mask_bits(DWORD mask)
{
    DWORD bits = 0;
    if (!mask) return 0;
    mask >>= monopoly_web_gdi_mask_shift(mask);
    while (mask & 1u) {
        ++bits;
        mask >>= 1;
    }
    return bits;
}

static inline BYTE *monopoly_web_gdi_row(MONOPOLY_WEB_GDI_BITMAP *bitmap, int y)
{
    if (!bitmap || !bitmap->bits || y < 0 || y >= bitmap->height) return NULL;
    int storage_y = bitmap->top_down ? y : (int)bitmap->height - 1 - y;
    return (BYTE *)bitmap->bits + (size_t)storage_y * bitmap->stride;
}

static inline const BYTE *monopoly_web_gdi_const_row(const MONOPOLY_WEB_GDI_BITMAP *bitmap, int y)
{
    if (!bitmap || !bitmap->bits || y < 0 || y >= bitmap->height) return NULL;
    int storage_y = bitmap->top_down ? y : (int)bitmap->height - 1 - y;
    return (const BYTE *)bitmap->bits + (size_t)storage_y * bitmap->stride;
}

static inline WORD monopoly_web_gdi_colorref_to_565(COLORREF color)
{
    unsigned int r = color & 0xff;
    unsigned int g = (color >> 8) & 0xff;
    unsigned int b = (color >> 16) & 0xff;
    return (WORD)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
}

static inline DWORD monopoly_web_gdi_colorref_to_32(COLORREF color)
{
    unsigned int r = color & 0xff;
    unsigned int g = (color >> 8) & 0xff;
    unsigned int b = (color >> 16) & 0xff;
    return 0xff000000u | (r << 16) | (g << 8) | b;
}

static inline COLORREF monopoly_web_gdi_get_pixel(const MONOPOLY_WEB_GDI_BITMAP *bitmap, int x, int y)
{
    const BYTE *row = monopoly_web_gdi_const_row(bitmap, y);
    if (!row || x < 0 || x >= bitmap->width) return 0;
    if (bitmap->bpp == 8) {
        BYTE value = row[x];
        if (value < bitmap->palette_entries) {
            const RGBQUAD *q = &bitmap->palette[value];
            return RGB(q->rgbRed, q->rgbGreen, q->rgbBlue);
        }
        return RGB(value, value, value);
    }
    if (bitmap->bpp == 16) {
        WORD value = ((const WORD *)row)[x];
        BYTE r = monopoly_web_gdi_expand_bits((value & 0xf800u) >> 11, 5);
        BYTE g = monopoly_web_gdi_expand_bits((value & 0x07e0u) >> 5, 6);
        BYTE b = monopoly_web_gdi_expand_bits(value & 0x001fu, 5);
        return RGB(r, g, b);
    }
    if (bitmap->bpp == 24) {
        const BYTE *p = row + (size_t)x * 3u;
        return RGB(p[2], p[1], p[0]);
    }
    const DWORD value = ((const DWORD *)row)[x];
    return RGB((value >> 16) & 0xff, (value >> 8) & 0xff, value & 0xff);
}

static inline void monopoly_web_gdi_put_pixel(MONOPOLY_WEB_GDI_BITMAP *bitmap, int x, int y, COLORREF color)
{
    if (!bitmap || !bitmap->bits || x < 0 || y < 0 || x >= bitmap->width || y >= bitmap->height) return;
    BYTE *row = monopoly_web_gdi_row(bitmap, y);
    if (!row) return;
    if (bitmap->bpp == 8) {
        row[x] = (BYTE)(color & 0xff);
    } else if (bitmap->bpp == 16) {
        ((WORD *)row)[x] = monopoly_web_gdi_colorref_to_565(color);
    } else if (bitmap->bpp == 24) {
        BYTE *p = row + (size_t)x * 3;
        p[0] = (BYTE)((color >> 16) & 0xff);
        p[1] = (BYTE)((color >> 8) & 0xff);
        p[2] = (BYTE)(color & 0xff);
    } else {
        ((DWORD *)row)[x] = monopoly_web_gdi_colorref_to_32(color);
    }
}

static inline void monopoly_web_gdi_blend_pixel(MONOPOLY_WEB_GDI_BITMAP *bitmap, int x, int y,
                                                BYTE srcR, BYTE srcG, BYTE srcB, BYTE srcA)
{
    if (srcA == 0) return;
    if (srcA == 255) {
        monopoly_web_gdi_put_pixel(bitmap, x, y, RGB(srcR, srcG, srcB));
        return;
    }
    COLORREF dst = monopoly_web_gdi_get_pixel(bitmap, x, y);
    unsigned int invA = 255u - srcA;
    BYTE dstR = (BYTE)(dst & 0xff);
    BYTE dstG = (BYTE)((dst >> 8) & 0xff);
    BYTE dstB = (BYTE)((dst >> 16) & 0xff);
    BYTE outR = (BYTE)(((unsigned int)srcR * srcA + (unsigned int)dstR * invA + 127u) / 255u);
    BYTE outG = (BYTE)(((unsigned int)srcG * srcA + (unsigned int)dstG * invA + 127u) / 255u);
    BYTE outB = (BYTE)(((unsigned int)srcB * srcA + (unsigned int)dstB * invA + 127u) / 255u);
    monopoly_web_gdi_put_pixel(bitmap, x, y, RGB(outR, outG, outB));
}

static inline void monopoly_web_gdi_fill_rect(MONOPOLY_WEB_GDI_BITMAP *bitmap, const RECT *rect, COLORREF color)
{
    if (!bitmap || !bitmap->bits || !rect) return;
    LONG left = rect->left < 0 ? 0 : rect->left;
    LONG top = rect->top < 0 ? 0 : rect->top;
    LONG right = rect->right > bitmap->width ? bitmap->width : rect->right;
    LONG bottom = rect->bottom > bitmap->height ? bitmap->height : rect->bottom;
    for (LONG y = top; y < bottom; ++y) {
        for (LONG x = left; x < right; ++x) {
            monopoly_web_gdi_put_pixel(bitmap, (int)x, (int)y, color);
        }
    }
}

static inline const BYTE *monopoly_web_gdi_glyph(char c)
{
    static const BYTE blank[7] = {0,0,0,0,0,0,0};
    static const BYTE question[7] = {14,17,1,2,4,0,4};
    if ((unsigned char)c < 32 || (unsigned char)c == 127) return blank;
    switch (c >= 'a' && c <= 'z' ? (char)(c - 32) : c) {
        case 'A': { static const BYTE g[7] = {14,17,17,31,17,17,17}; return g; }
        case 'B': { static const BYTE g[7] = {30,17,17,30,17,17,30}; return g; }
        case 'C': { static const BYTE g[7] = {14,17,16,16,16,17,14}; return g; }
        case 'D': { static const BYTE g[7] = {30,17,17,17,17,17,30}; return g; }
        case 'E': { static const BYTE g[7] = {31,16,16,30,16,16,31}; return g; }
        case 'F': { static const BYTE g[7] = {31,16,16,30,16,16,16}; return g; }
        case 'G': { static const BYTE g[7] = {14,17,16,23,17,17,15}; return g; }
        case 'H': { static const BYTE g[7] = {17,17,17,31,17,17,17}; return g; }
        case 'I': { static const BYTE g[7] = {14,4,4,4,4,4,14}; return g; }
        case 'J': { static const BYTE g[7] = {1,1,1,1,17,17,14}; return g; }
        case 'K': { static const BYTE g[7] = {17,18,20,24,20,18,17}; return g; }
        case 'L': { static const BYTE g[7] = {16,16,16,16,16,16,31}; return g; }
        case 'M': { static const BYTE g[7] = {17,27,21,21,17,17,17}; return g; }
        case 'N': { static const BYTE g[7] = {17,25,21,19,17,17,17}; return g; }
        case 'O': { static const BYTE g[7] = {14,17,17,17,17,17,14}; return g; }
        case 'P': { static const BYTE g[7] = {30,17,17,30,16,16,16}; return g; }
        case 'Q': { static const BYTE g[7] = {14,17,17,17,21,18,13}; return g; }
        case 'R': { static const BYTE g[7] = {30,17,17,30,20,18,17}; return g; }
        case 'S': { static const BYTE g[7] = {15,16,16,14,1,1,30}; return g; }
        case 'T': { static const BYTE g[7] = {31,4,4,4,4,4,4}; return g; }
        case 'U': { static const BYTE g[7] = {17,17,17,17,17,17,14}; return g; }
        case 'V': { static const BYTE g[7] = {17,17,17,17,17,10,4}; return g; }
        case 'W': { static const BYTE g[7] = {17,17,17,21,21,21,10}; return g; }
        case 'X': { static const BYTE g[7] = {17,17,10,4,10,17,17}; return g; }
        case 'Y': { static const BYTE g[7] = {17,17,10,4,4,4,4}; return g; }
        case 'Z': { static const BYTE g[7] = {31,1,2,4,8,16,31}; return g; }
        case '0': { static const BYTE g[7] = {14,17,19,21,25,17,14}; return g; }
        case '1': { static const BYTE g[7] = {4,12,4,4,4,4,14}; return g; }
        case '2': { static const BYTE g[7] = {14,17,1,2,4,8,31}; return g; }
        case '3': { static const BYTE g[7] = {30,1,1,14,1,1,30}; return g; }
        case '4': { static const BYTE g[7] = {2,6,10,18,31,2,2}; return g; }
        case '5': { static const BYTE g[7] = {31,16,16,30,1,1,30}; return g; }
        case '6': { static const BYTE g[7] = {14,16,16,30,17,17,14}; return g; }
        case '7': { static const BYTE g[7] = {31,1,2,4,8,8,8}; return g; }
        case '8': { static const BYTE g[7] = {14,17,17,14,17,17,14}; return g; }
        case '9': { static const BYTE g[7] = {14,17,17,15,1,1,14}; return g; }
        case ' ': return blank;
        case '.': { static const BYTE g[7] = {0,0,0,0,0,12,12}; return g; }
        case ',': { static const BYTE g[7] = {0,0,0,0,0,12,8}; return g; }
        case ':': { static const BYTE g[7] = {0,12,12,0,12,12,0}; return g; }
        case ';': { static const BYTE g[7] = {0,12,12,0,12,4,8}; return g; }
        case '\'': { static const BYTE g[7] = {4,4,8,0,0,0,0}; return g; }
        case '"': { static const BYTE g[7] = {10,10,0,0,0,0,0}; return g; }
        case '-': { static const BYTE g[7] = {0,0,0,31,0,0,0}; return g; }
        case '_': { static const BYTE g[7] = {0,0,0,0,0,0,31}; return g; }
        case '+': { static const BYTE g[7] = {0,4,4,31,4,4,0}; return g; }
        case '/': { static const BYTE g[7] = {1,1,2,4,8,16,16}; return g; }
        case '\\': { static const BYTE g[7] = {16,16,8,4,2,1,1}; return g; }
        case '(': { static const BYTE g[7] = {2,4,8,8,8,4,2}; return g; }
        case ')': { static const BYTE g[7] = {8,4,2,2,2,4,8}; return g; }
        case '[': { static const BYTE g[7] = {14,8,8,8,8,8,14}; return g; }
        case ']': { static const BYTE g[7] = {14,2,2,2,2,2,14}; return g; }
        case '&': { static const BYTE g[7] = {12,18,20,8,21,18,13}; return g; }
        case '$': { static const BYTE g[7] = {4,15,20,14,5,30,4}; return g; }
        case '%': { static const BYTE g[7] = {24,25,2,4,8,19,3}; return g; }
        case '!': { static const BYTE g[7] = {4,4,4,4,4,0,4}; return g; }
        case '?': return question;
        default: return question;
    }
}

static inline int monopoly_web_gdi_effective_count_a(LPCSTR text, int count)
{
    if (!text) return 0;
    if (count < 0) count = (int)strlen(text);
    int n = 0;
    while (n < count && text[n]) ++n;
    while (n > 0 && ((unsigned char)text[n - 1] <= 32 || (unsigned char)text[n - 1] == 127)) --n;
    return n;
}

static inline int monopoly_web_gdi_effective_count_w(LPCWSTR text, int count)
{
    if (!text) return 0;
    if (count < 0) count = (int)wcslen(text);
    int n = 0;
    while (n < count && text[n]) ++n;
    while (n > 0 && (text[n - 1] <= 32 || text[n - 1] == 127 || text[n - 1] == 160)) --n;
    return n;
}

static inline void monopoly_web_gdi_draw_ascii(MONOPOLY_WEB_GDI_DC *dc, int x, int y, const char *text, UINT count)
{
    if (!dc || dc->magic != MONOPOLY_WEB_GDI_DC_MAGIC || !dc->bitmap || !text) return;
    int n = monopoly_web_gdi_effective_count_a(text, (int)count);
    if (n <= 0) return;

    int fontHeight = monopoly_web_gdi_font_height(dc);
    int width = monopoly_web_canvas_measure_text_width(text, n, fontHeight, dc->font ? dc->font->weight : FW_NORMAL);
    int height = monopoly_web_gdi_text_height(dc);
    if (width > 0 && height > 0 && width <= 4096 && height <= 256) {
        size_t rgbaSize = (size_t)width * (size_t)height * 4u;
        unsigned char *rgba = (unsigned char *)malloc(rgbaSize ? rgbaSize : 1u);
        if (rgba) {
            memset(rgba, 0, rgbaSize);
            if (monopoly_web_canvas_raster_text(text, n, fontHeight, dc->font ? dc->font->weight : FW_NORMAL,
                                                (unsigned int)dc->text_color, rgba, width, height)) {
                for (int py = 0; py < height; ++py) {
                    for (int px = 0; px < width; ++px) {
                        unsigned char *p = rgba + ((size_t)py * (size_t)width + (size_t)px) * 4u;
                        monopoly_web_gdi_blend_pixel(dc->bitmap, x + px, y + py, p[0], p[1], p[2], p[3]);
                    }
                }
                free(rgba);
                return;
            }
            free(rgba);
        }
    }

    int scale = monopoly_web_gdi_font_scale(dc);
    int cell = monopoly_web_gdi_cell_width(dc);
    int cursor = x;
    for (int i = 0; i < n; ++i, cursor += cell) {
        char ch = ((unsigned char)text[i] <= 32 || (unsigned char)text[i] == 127) ? ' ' : text[i];
        const BYTE *glyph = monopoly_web_gdi_glyph(ch);
        for (int gy = 0; gy < 7; ++gy) {
            for (int gx = 0; gx < 5; ++gx) {
                if ((glyph[gy] & (1 << (4 - gx))) == 0) continue;
                for (int sy = 0; sy < scale; ++sy) {
                    for (int sx = 0; sx < scale; ++sx) {
                        monopoly_web_gdi_put_pixel(dc->bitmap, cursor + gx * scale + sx, y + gy * scale + sy, dc->text_color);
                    }
                }
            }
        }
        if (dc->font && dc->font->weight >= 650) {
            const BYTE *glyph = monopoly_web_gdi_glyph(ch);
            for (int gy = 0; gy < 7; ++gy) {
                for (int gx = 0; gx < 5; ++gx) {
                    if ((glyph[gy] & (1 << (4 - gx))) == 0) continue;
                    for (int sy = 0; sy < scale; ++sy) {
                        monopoly_web_gdi_put_pixel(dc->bitmap, cursor + gx * scale + scale, y + gy * scale + sy, dc->text_color);
                    }
                }
            }
        }
    }
}

static inline HDC monopoly_web_gdi_create_dc(void)
{
    MONOPOLY_WEB_GDI_DC *dc = (MONOPOLY_WEB_GDI_DC *)calloc(1, sizeof(*dc));
    if (!dc) return NULL;
    dc->magic = MONOPOLY_WEB_GDI_DC_MAGIC;
    dc->text_color = RGB(255, 255, 255);
    dc->bk_color = RGB(0, 0, 0);
    dc->bk_mode = OPAQUE;
    return (HDC)dc;
}

static inline HDC GetDC(HWND) { return monopoly_web_gdi_create_dc(); }
static inline int ReleaseDC(HWND, HDC dc) { if (dc) free(dc); return 1; }
static inline HDC CreateCompatibleDC(HDC) { return monopoly_web_gdi_create_dc(); }
static inline BOOL DeleteDC(HDC dc) { if (dc) free(dc); return TRUE; }
static inline BOOL BitBlt(HDC dstHdc, int dstX, int dstY, int width, int height, HDC srcHdc, int srcX, int srcY, DWORD)
{
    MONOPOLY_WEB_GDI_DC *dst = (MONOPOLY_WEB_GDI_DC *)dstHdc;
    MONOPOLY_WEB_GDI_DC *src = (MONOPOLY_WEB_GDI_DC *)srcHdc;
    if (!dst || !src || dst->magic != MONOPOLY_WEB_GDI_DC_MAGIC || src->magic != MONOPOLY_WEB_GDI_DC_MAGIC) return TRUE;
    if (!dst->bitmap || !src->bitmap || width <= 0 || height <= 0) return TRUE;
    for (int y = 0; y < height; ++y) {
        for (int x = 0; x < width; ++x) {
            COLORREF color = monopoly_web_gdi_get_pixel(src->bitmap, srcX + x, srcY + y);
            monopoly_web_gdi_put_pixel(dst->bitmap, dstX + x, dstY + y, color);
        }
    }
    return TRUE;
}
static inline BOOL StretchBlt(HDC dstHdc, int dstX, int dstY, int dstW, int dstH, HDC srcHdc, int srcX, int srcY, int srcW, int srcH, DWORD)
{
    MONOPOLY_WEB_GDI_DC *dst = (MONOPOLY_WEB_GDI_DC *)dstHdc;
    MONOPOLY_WEB_GDI_DC *src = (MONOPOLY_WEB_GDI_DC *)srcHdc;
    if (!dst || !src || dst->magic != MONOPOLY_WEB_GDI_DC_MAGIC || src->magic != MONOPOLY_WEB_GDI_DC_MAGIC) return TRUE;
    if (!dst->bitmap || !src->bitmap || dstW <= 0 || dstH <= 0 || srcW <= 0 || srcH <= 0) return TRUE;
    for (int y = 0; y < dstH; ++y) {
        int sy = srcY + (int)(((int64_t)y * srcH) / dstH);
        for (int x = 0; x < dstW; ++x) {
            int sx = srcX + (int)(((int64_t)x * srcW) / dstW);
            COLORREF color = monopoly_web_gdi_get_pixel(src->bitmap, sx, sy);
            monopoly_web_gdi_put_pixel(dst->bitmap, dstX + x, dstY + y, color);
        }
    }
    return TRUE;
}
static inline HBRUSH CreateSolidBrush(COLORREF) { return NULL; }
static inline int FillRect(HDC, const RECT *, HBRUSH) { return TRUE; }
static inline BOOL GdiFlush(void) { return TRUE; }
static inline HGDIOBJ SelectObject(HDC hdc, HGDIOBJ object)
{
    MONOPOLY_WEB_GDI_DC *dc = (MONOPOLY_WEB_GDI_DC *)hdc;
    if (!dc || dc->magic != MONOPOLY_WEB_GDI_DC_MAGIC) return object;
    if (!object) {
        HGDIOBJ previous = (HGDIOBJ)dc->font;
        dc->font = NULL;
        return previous;
    }
    DWORD magic = *((DWORD *)object);
    if (magic == MONOPOLY_WEB_GDI_BITMAP_MAGIC) {
        HGDIOBJ previous = (HGDIOBJ)dc->bitmap;
        dc->bitmap = (MONOPOLY_WEB_GDI_BITMAP *)object;
        return previous;
    }
    if (magic == MONOPOLY_WEB_GDI_FONT_MAGIC) {
        HGDIOBJ previous = (HGDIOBJ)dc->font;
        dc->font = (MONOPOLY_WEB_GDI_FONT *)object;
        return previous;
    }
    return object;
}
static inline int GetObjectA(HGDIOBJ handle, int, LPVOID object)
{
    if (object) ZeroMemory(object, sizeof(BITMAP));
    if (!handle || !object) return sizeof(BITMAP);
    DWORD magic = *((DWORD *)handle);
    if (magic == MONOPOLY_WEB_GDI_BITMAP_MAGIC) {
        MONOPOLY_WEB_GDI_BITMAP *bitmap = (MONOPOLY_WEB_GDI_BITMAP *)handle;
        BITMAP *out = (BITMAP *)object;
        out->bmWidth = bitmap->width;
        out->bmHeight = bitmap->height;
        out->bmWidthBytes = (LONG)bitmap->stride;
        out->bmPlanes = 1;
        out->bmBitsPixel = bitmap->bpp;
        out->bmBits = bitmap->bits;
    }
    return sizeof(BITMAP);
}

static inline WORD monopoly_web_read_le16(const BYTE *p)
{
    return (WORD)((WORD)p[0] | ((WORD)p[1] << 8));
}

static inline DWORD monopoly_web_read_le32(const BYTE *p)
{
    return (DWORD)p[0] | ((DWORD)p[1] << 8) | ((DWORD)p[2] << 16) | ((DWORD)p[3] << 24);
}

static inline LONG monopoly_web_read_le32s(const BYTE *p)
{
    return (LONG)(int32_t)monopoly_web_read_le32(p);
}

static inline BYTE monopoly_web_scale_masked_component(DWORD pixel, DWORD mask)
{
    if (!mask) return 0;
    DWORD shift = monopoly_web_gdi_mask_shift(mask);
    DWORD bits = monopoly_web_gdi_mask_bits(mask);
    return monopoly_web_gdi_expand_bits((pixel & mask) >> shift, bits);
}

static inline LPCSTR monopoly_web_bitmap_resource_path(LPCSTR name)
{
    uintptr_t id = (uintptr_t)name;
    if (id > 0xffffu) return name;
    switch ((WORD)id) {
        case 104: return "/monopoly/BackGround.bmp";
        case 117: return "/monopoly/BackGroundEuro.bmp";
        default: return NULL;
    }
}

static inline HANDLE LoadImageA(HINSTANCE, LPCSTR name, UINT type, int, int, UINT flags)
{
    if (type != IMAGE_BITMAP || !name) return NULL;
    name = monopoly_web_bitmap_resource_path(name);
    if (!name) return NULL;

    char normalized[MAX_PATH * 4];
    monopoly_web_normalize_path(normalized, sizeof(normalized), name);
    if (!normalized[0]) return NULL;

    FILE *file = fopen(normalized, "rb");
    if (!file && !(flags & LR_LOADFROMFILE)) file = fopen(name, "rb");
    if (!file) return NULL;

    BYTE header[64];
    size_t header_read = fread(header, 1, sizeof(header), file);
    if (header_read < 54 || header[0] != 'B' || header[1] != 'M') {
        fclose(file);
        return NULL;
    }

    DWORD pixel_offset = monopoly_web_read_le32(header + 10);
    DWORD dib_size = monopoly_web_read_le32(header + 14);
    if (dib_size < 40) {
        fclose(file);
        return NULL;
    }

    LONG width = monopoly_web_read_le32s(header + 18);
    LONG raw_height = monopoly_web_read_le32s(header + 22);
    WORD planes = monopoly_web_read_le16(header + 26);
    WORD source_bpp = monopoly_web_read_le16(header + 28);
    DWORD compression = monopoly_web_read_le32(header + 30);
    DWORD colors_used = monopoly_web_read_le32(header + 46);
    if (width <= 0 || raw_height == 0 || planes != 1) {
        fclose(file);
        return NULL;
    }
    if (source_bpp != 8 && source_bpp != 16 && source_bpp != 24 && source_bpp != 32) {
        fclose(file);
        return NULL;
    }
    if (compression != BI_RGB && compression != BI_BITFIELDS) {
        fclose(file);
        return NULL;
    }

    LONG height = raw_height < 0 ? -raw_height : raw_height;
    BOOL source_top_down = raw_height < 0;
    DWORD red_mask = 0x00ff0000u;
    DWORD green_mask = 0x0000ff00u;
    DWORD blue_mask = 0x000000ffu;
    DWORD alpha_mask = source_bpp == 32 ? 0xff000000u : 0;
    long after_info = 14L + (long)dib_size;
    if (source_bpp == 16) {
        red_mask = 0x0000f800u;
        green_mask = 0x000007e0u;
        blue_mask = 0x0000001fu;
    }
    if (compression == BI_BITFIELDS && header_read >= 14 + dib_size + 12) {
        red_mask = monopoly_web_read_le32(header + 14 + dib_size);
        green_mask = monopoly_web_read_le32(header + 18 + dib_size);
        blue_mask = monopoly_web_read_le32(header + 22 + dib_size);
        if (source_bpp == 32 && header_read >= 14 + dib_size + 16) {
            alpha_mask = monopoly_web_read_le32(header + 26 + dib_size);
        }
    }

    RGBQUAD palette[256];
    memset(palette, 0, sizeof(palette));
    if (source_bpp == 8) {
        DWORD palette_count = colors_used ? colors_used : 256u;
        if (palette_count > 256u) palette_count = 256u;
        if (fseek(file, after_info, SEEK_SET) != 0 ||
            fread(palette, sizeof(RGBQUAD), palette_count, file) != palette_count) {
            fclose(file);
            return NULL;
        }
    }

    MONOPOLY_WEB_GDI_BITMAP *bitmap = (MONOPOLY_WEB_GDI_BITMAP *)calloc(1, sizeof(*bitmap));
    if (!bitmap) {
        fclose(file);
        return NULL;
    }
    bitmap->magic = MONOPOLY_WEB_GDI_BITMAP_MAGIC;
    bitmap->width = width;
    bitmap->height = height;
    bitmap->top_down = TRUE;
    bitmap->owns_bits = TRUE;
    bitmap->bpp = 32;
    bitmap->stride = (size_t)width * 4u;
    bitmap->size = bitmap->stride * (size_t)height;
    bitmap->bits = calloc(1, bitmap->size ? bitmap->size : 1);
    if (!bitmap->bits) {
        free(bitmap);
        fclose(file);
        return NULL;
    }

    size_t source_stride = (((size_t)width * (size_t)source_bpp + 31u) / 32u) * 4u;
    BYTE *row = (BYTE *)malloc(source_stride ? source_stride : 1);
    if (!row || fseek(file, (long)pixel_offset, SEEK_SET) != 0) {
        free(row);
        free(bitmap->bits);
        free(bitmap);
        fclose(file);
        return NULL;
    }

    for (LONG file_y = 0; file_y < height; ++file_y) {
        if (fread(row, 1, source_stride, file) != source_stride) break;
        LONG y = source_top_down ? file_y : (height - 1 - file_y);
        DWORD *dst = (DWORD *)monopoly_web_gdi_row(bitmap, (int)y);
        if (!dst) continue;
        for (LONG x = 0; x < width; ++x) {
            BYTE r = 0, g = 0, b = 0, a = 255;
            if (source_bpp == 8) {
                RGBQUAD q = palette[row[x]];
                r = q.rgbRed; g = q.rgbGreen; b = q.rgbBlue;
            } else if (source_bpp == 16) {
                DWORD pixel = monopoly_web_read_le16(row + (size_t)x * 2u);
                r = monopoly_web_scale_masked_component(pixel, red_mask);
                g = monopoly_web_scale_masked_component(pixel, green_mask);
                b = monopoly_web_scale_masked_component(pixel, blue_mask);
            } else if (source_bpp == 24) {
                const BYTE *p = row + (size_t)x * 3u;
                b = p[0]; g = p[1]; r = p[2];
            } else {
                DWORD pixel = monopoly_web_read_le32(row + (size_t)x * 4u);
                r = monopoly_web_scale_masked_component(pixel, red_mask);
                g = monopoly_web_scale_masked_component(pixel, green_mask);
                b = monopoly_web_scale_masked_component(pixel, blue_mask);
                if (alpha_mask) a = monopoly_web_scale_masked_component(pixel, alpha_mask);
            }
            dst[x] = ((DWORD)a << 24) | ((DWORD)r << 16) | ((DWORD)g << 8) | b;
        }
    }

    free(row);
    fclose(file);
    return (HANDLE)bitmap;
}
static inline HFONT CreateFontA(int height, int, int, int, int weight, DWORD italic, DWORD underline, DWORD strikeout, DWORD, DWORD, DWORD, DWORD, DWORD, LPCSTR face)
{
    MONOPOLY_WEB_GDI_FONT *font = (MONOPOLY_WEB_GDI_FONT *)calloc(1, sizeof(*font));
    if (!font) return NULL;
    font->magic = MONOPOLY_WEB_GDI_FONT_MAGIC;
    font->logfont.lfHeight = height;
    font->logfont.lfWeight = weight;
    font->logfont.lfItalic = (BYTE)italic;
    font->logfont.lfUnderline = (BYTE)underline;
    font->logfont.lfStrikeOut = (BYTE)strikeout;
    if (face) strncpy(font->logfont.lfFaceName, face, LF_FACESIZE - 1);
    font->pixel_height = height < 0 ? -height : height;
    if (font->pixel_height <= 0) font->pixel_height = 16;
    font->weight = weight;
    return (HFONT)font;
}
static inline HFONT CreateFontIndirectA(const LOGFONTA *logfont)
{
    MONOPOLY_WEB_GDI_FONT *font = (MONOPOLY_WEB_GDI_FONT *)calloc(1, sizeof(*font));
    if (!font) return NULL;
    font->magic = MONOPOLY_WEB_GDI_FONT_MAGIC;
    if (logfont) font->logfont = *logfont;
    font->pixel_height = font->logfont.lfHeight < 0 ? -font->logfont.lfHeight : font->logfont.lfHeight;
    if (font->pixel_height <= 0) font->pixel_height = 16;
    font->weight = font->logfont.lfWeight;
    return (HFONT)font;
}
static inline BOOL GetTextMetricsA(HDC hdc, LPTEXTMETRICA metrics)
{
    if (!metrics) return FALSE;
    ZeroMemory(metrics, sizeof(*metrics));
    MONOPOLY_WEB_GDI_DC *dc = (MONOPOLY_WEB_GDI_DC *)hdc;
    metrics->tmHeight = monopoly_web_gdi_text_height(dc);
    metrics->tmAveCharWidth = monopoly_web_canvas_measure_text_width("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz", 52,
                                                                     monopoly_web_gdi_font_height(dc),
                                                                     dc && dc->font ? dc->font->weight : FW_NORMAL) / 52;
    if (metrics->tmAveCharWidth <= 0) metrics->tmAveCharWidth = monopoly_web_gdi_cell_width(dc);
    metrics->tmMaxCharWidth = monopoly_web_canvas_measure_text_width("W", 1, monopoly_web_gdi_font_height(dc),
                                                                     dc && dc->font ? dc->font->weight : FW_NORMAL);
    if (metrics->tmMaxCharWidth <= 0) metrics->tmMaxCharWidth = metrics->tmAveCharWidth;
    metrics->tmWeight = dc && dc->font ? dc->font->weight : FW_NORMAL;
    metrics->tmFirstChar = 32;
    metrics->tmLastChar = 126;
    return TRUE;
}
static inline BOOL GetTextExtentPoint32A(HDC hdc, LPCSTR text, int count, LPSIZE size)
{
    if (!size) return FALSE;
    MONOPOLY_WEB_GDI_DC *dc = (MONOPOLY_WEB_GDI_DC *)hdc;
    count = monopoly_web_gdi_effective_count_a(text, count);
    size->cx = monopoly_web_canvas_measure_text_width(text, count, monopoly_web_gdi_font_height(dc),
                                                      dc && dc->font ? dc->font->weight : FW_NORMAL);
    if (size->cx <= 0) size->cx = count * monopoly_web_gdi_cell_width(dc);
    size->cy = monopoly_web_gdi_text_height(dc);
    return TRUE;
}
static inline BOOL GetTextExtentPoint32W(HDC hdc, LPCWSTR text, int count, LPSIZE size)
{
    if (!size) return FALSE;
    count = monopoly_web_gdi_effective_count_w(text, count);
    char buffer[1024];
    if (count >= (int)sizeof(buffer)) count = (int)sizeof(buffer) - 1;
    for (int i = 0; i < count; ++i) {
        wchar_t c = text[i];
        if (c <= 32 || c == 127 || c == 160) buffer[i] = ' ';
        else if (c >= 32 && c < 127) buffer[i] = (char)c;
        else buffer[i] = '?';
    }
    buffer[count] = '\0';
    return GetTextExtentPoint32A(hdc, buffer, count, size);
}
static inline int SetBkMode(HDC hdc, int mode)
{
    MONOPOLY_WEB_GDI_DC *dc = (MONOPOLY_WEB_GDI_DC *)hdc;
    int old = dc && dc->magic == MONOPOLY_WEB_GDI_DC_MAGIC ? dc->bk_mode : mode;
    if (dc && dc->magic == MONOPOLY_WEB_GDI_DC_MAGIC) dc->bk_mode = mode;
    return old;
}
static inline int AddFontResourceA(LPCSTR) { return 1; }
static inline BOOL RemoveFontResourceA(LPCSTR) { return TRUE; }
static inline int MulDiv(int number, int numerator, int denominator)
{
    return denominator ? (int)(((int64_t)number * numerator) / denominator) : 0;
}
static inline int GetDeviceCaps(HDC, int index)
{
    switch (index) {
        case HORZRES: return 800;
        case VERTRES: return 600;
        case BITSPIXEL: return 32;
        case LOGPIXELSX:
        case LOGPIXELSY: return 96;
        default: return 0;
    }
}
static inline int EnumFontFamiliesA(HDC, LPCSTR, FONTENUMPROC proc, LPARAM param)
{
    if (proc) {
        LOGFONT font;
        NEWTEXTMETRIC metric;
        ZeroMemory(&font, sizeof(font));
        ZeroMemory(&metric, sizeof(metric));
        strcpy(font.lfFaceName, "Arial");
        proc(&font, &metric, RASTER_FONTTYPE, param);
    }
    return 1;
}
static inline COLORREF SetTextColor(HDC hdc, COLORREF color)
{
    MONOPOLY_WEB_GDI_DC *dc = (MONOPOLY_WEB_GDI_DC *)hdc;
    COLORREF old = dc && dc->magic == MONOPOLY_WEB_GDI_DC_MAGIC ? dc->text_color : color;
    if (dc && dc->magic == MONOPOLY_WEB_GDI_DC_MAGIC) dc->text_color = color;
    return old;
}
static inline COLORREF SetBkColor(HDC hdc, COLORREF color)
{
    MONOPOLY_WEB_GDI_DC *dc = (MONOPOLY_WEB_GDI_DC *)hdc;
    COLORREF old = dc && dc->magic == MONOPOLY_WEB_GDI_DC_MAGIC ? dc->bk_color : color;
    if (dc && dc->magic == MONOPOLY_WEB_GDI_DC_MAGIC) dc->bk_color = color;
    return old;
}
static inline BOOL ExtTextOutA(HDC hdc, int x, int y, UINT options, const RECT *rect, LPCSTR text, UINT count, const int *)
{
    MONOPOLY_WEB_GDI_DC *dc = (MONOPOLY_WEB_GDI_DC *)hdc;
    if (!dc || dc->magic != MONOPOLY_WEB_GDI_DC_MAGIC || !dc->bitmap || !text) return TRUE;
    if ((options & ETO_OPAQUE) && rect) monopoly_web_gdi_fill_rect(dc->bitmap, rect, dc->bk_color);
    monopoly_web_gdi_draw_ascii(dc, x, y, text, (UINT)monopoly_web_gdi_effective_count_a(text, (int)count));
    return TRUE;
}
static inline BOOL ExtTextOutW(HDC hdc, int x, int y, UINT options, const RECT *rect, LPCWSTR text, UINT count, const int *)
{
    if (!text) return TRUE;
    char buffer[1024];
    UINT n = (UINT)monopoly_web_gdi_effective_count_w(text, (int)count);
    if (n >= sizeof(buffer)) n = sizeof(buffer) - 1;
    for (UINT i = 0; i < n; ++i) {
        wchar_t c = text[i];
        if (c <= 32 || c == 127 || c == 160) buffer[i] = ' ';
        else if (c >= 32 && c < 127) buffer[i] = (char)c;
        else buffer[i] = '?';
    }
    buffer[n] = '\0';
    return ExtTextOutA(hdc, x, y, options, rect, buffer, n, NULL);
}
static inline HBITMAP CreateDIBSection(HDC, const BITMAPINFO *info, UINT, void **bits, HANDLE, DWORD)
{
    if (bits) *bits = NULL;
    if (!info || info->bmiHeader.biWidth <= 0 || info->bmiHeader.biHeight == 0) return NULL;
    MONOPOLY_WEB_GDI_BITMAP *bitmap = (MONOPOLY_WEB_GDI_BITMAP *)calloc(1, sizeof(*bitmap));
    if (!bitmap) return NULL;
    bitmap->magic = MONOPOLY_WEB_GDI_BITMAP_MAGIC;
    bitmap->width = info->bmiHeader.biWidth;
    bitmap->height = info->bmiHeader.biHeight < 0 ? -info->bmiHeader.biHeight : info->bmiHeader.biHeight;
    bitmap->top_down = info->bmiHeader.biHeight < 0;
    bitmap->owns_bits = TRUE;
    bitmap->bpp = info->bmiHeader.biBitCount ? info->bmiHeader.biBitCount : 32;
    if (bitmap->bpp <= 8) {
        UINT entries = info->bmiHeader.biClrUsed ? (UINT)info->bmiHeader.biClrUsed : (1u << bitmap->bpp);
        if (entries > 256u) entries = 256u;
        bitmap->palette_entries = entries;
        if (entries) memcpy(bitmap->palette, info->bmiColors, sizeof(RGBQUAD) * entries);
    }
    bitmap->stride = (((size_t)bitmap->width * (size_t)bitmap->bpp + 31u) / 32u) * 4u;
    if (bitmap->stride == 0) bitmap->stride = (size_t)bitmap->width * 4u;
    bitmap->size = bitmap->stride * (size_t)bitmap->height;
    bitmap->bits = calloc(1, bitmap->size ? bitmap->size : 1);
    if (!bitmap->bits) {
        free(bitmap);
        return NULL;
    }
    if (bits) *bits = bitmap->bits;
    return (HBITMAP)bitmap;
}
static inline UINT GetDIBColorTable(HDC hdc, UINT start, UINT count, RGBQUAD *colors)
{
    MONOPOLY_WEB_GDI_DC *dc = (MONOPOLY_WEB_GDI_DC *)hdc;
    if (!dc || dc->magic != MONOPOLY_WEB_GDI_DC_MAGIC || !dc->bitmap || !colors) return 0;
    if (start >= dc->bitmap->palette_entries) return 0;
    UINT available = dc->bitmap->palette_entries - start;
    if (count > available) count = available;
    memcpy(colors, dc->bitmap->palette + start, sizeof(RGBQUAD) * count);
    return count;
}
static inline HCURSOR LoadCursorA(HINSTANCE, LPCSTR) { return NULL; }
static inline HCURSOR GetCursor(void) { return NULL; }
static inline HCURSOR SetCursor(HCURSOR cursor) { return cursor; }
static inline int ShowCursor(BOOL) { return 0; }
static inline BOOL GetCursorPos(LPPOINT point) { if (point) { point->x = 0; point->y = 0; } return TRUE; }
static inline BOOL GetIconInfo(HICON, PICONINFO info) { if (info) ZeroMemory(info, sizeof(*info)); return TRUE; }
static inline BOOL DrawIcon(HDC, int, int, HICON) { return TRUE; }
static inline BOOL GetClientRect(HWND, LPRECT rect)
{
    if (!rect) return FALSE;
    rect->left = 0; rect->top = 0; rect->right = 800; rect->bottom = 600;
    return TRUE;
}
static inline BOOL GetWindowRect(HWND, LPRECT rect)
{
    return GetClientRect(NULL, rect);
}
static inline BOOL IsIconic(HWND) { return FALSE; }
static inline BOOL IsZoomed(HWND) { return FALSE; }
static inline BOOL ClientToScreen(HWND, LPPOINT) { return TRUE; }
static inline BOOL ScreenToClient(HWND, LPPOINT) { return TRUE; }
static inline BOOL SetRect(LPRECT rect, int left, int top, int right, int bottom)
{
    if (!rect) return FALSE;
    rect->left = left; rect->top = top; rect->right = right; rect->bottom = bottom;
    return TRUE;
}
static inline BOOL OffsetRect(LPRECT rect, int dx, int dy)
{
    if (!rect) return FALSE;
    rect->left += dx; rect->right += dx; rect->top += dy; rect->bottom += dy;
    return TRUE;
}
static inline BOOL PtInRect(const RECT *rect, POINT point)
{
    return rect && point.x >= rect->left && point.x < rect->right &&
        point.y >= rect->top && point.y < rect->bottom;
}

static inline BOOL IntersectRect(LPRECT dest, const RECT *left, const RECT *right)
{
    if (!dest || !left || !right) return FALSE;
    dest->left = left->left > right->left ? left->left : right->left;
    dest->top = left->top > right->top ? left->top : right->top;
    dest->right = left->right < right->right ? left->right : right->right;
    dest->bottom = left->bottom < right->bottom ? left->bottom : right->bottom;
    if (dest->right <= dest->left || dest->bottom <= dest->top) {
        dest->left = dest->top = dest->right = dest->bottom = 0;
        return FALSE;
    }
    return TRUE;
}

static inline int OffsetRgn(HRGN handle, int dx, int dy)
{
    MONOPOLY_WEB_RGN *rgn = (MONOPOLY_WEB_RGN *)handle;
    if (!rgn) return RGN_ERROR;
    rgn->rect.left += dx;
    rgn->rect.right += dx;
    rgn->rect.top += dy;
    rgn->rect.bottom += dy;
    return (rgn->rect.right <= rgn->rect.left || rgn->rect.bottom <= rgn->rect.top)
        ? NULLREGION : SIMPLEREGION;
}

static inline int GetRgnBox(HRGN handle, LPRECT rect)
{
    MONOPOLY_WEB_RGN *rgn = (MONOPOLY_WEB_RGN *)handle;
    if (!rgn || !rect) return RGN_ERROR;
    *rect = rgn->rect;
    return (rect->right <= rect->left || rect->bottom <= rect->top)
        ? NULLREGION : SIMPLEREGION;
}

static inline BOOL RectInRegion(HRGN handle, const RECT *rect)
{
    MONOPOLY_WEB_RGN *rgn = (MONOPOLY_WEB_RGN *)handle;
    RECT intersection;
    return rgn && rect && IntersectRect(&intersection, &rgn->rect, rect);
}

static inline DWORD GetRegionData(HRGN handle, DWORD dataSize, LPRGNDATA data)
{
    MONOPOLY_WEB_RGN *rgn = (MONOPOLY_WEB_RGN *)handle;
    if (!rgn) return 0;
    DWORD required = sizeof(RGNDATAHEADER) + sizeof(RECT);
    if (!data || dataSize < required) return required;
    ZeroMemory(data, dataSize);
    data->rdh.dwSize = sizeof(RGNDATAHEADER);
    data->rdh.iType = RDH_RECTANGLES;
    data->rdh.nCount = 1;
    data->rdh.nRgnSize = sizeof(RECT);
    data->rdh.rcBound = rgn->rect;
    memcpy(data->Buffer, &rgn->rect, sizeof(RECT));
    return required;
}

static inline HRGN ExtCreateRegion(const void *, DWORD dataSize, const RGNDATA *data)
{
    if (!data || dataSize < sizeof(RGNDATAHEADER) + sizeof(RECT) ||
        data->rdh.iType != RDH_RECTANGLES || data->rdh.nCount == 0) {
        return NULL;
    }
    const RECT *rect = (const RECT *)data->Buffer;
    MONOPOLY_WEB_RGN *rgn = (MONOPOLY_WEB_RGN *)calloc(1, sizeof(*rgn));
    if (!rgn) return NULL;
    rgn->rect = *rect;
    return (HRGN)rgn;
}

static inline void InitializeCriticalSection(LPCRITICAL_SECTION) {}
static inline void DeleteCriticalSection(LPCRITICAL_SECTION) {}
static inline void EnterCriticalSection(LPCRITICAL_SECTION) {}
static inline void LeaveCriticalSection(LPCRITICAL_SECTION) {}
static inline BOOL SetThreadPriority(HANDLE, int) { return TRUE; }
static inline HANDLE CreateThread(LPSECURITY_ATTRIBUTES, DWORD, DWORD (WINAPI *)(LPVOID), LPVOID, DWORD, LPDWORD) { return NULL; }
static inline HANDLE CreateEventA(LPSECURITY_ATTRIBUTES, BOOL, BOOL, LPCSTR)
{
    return calloc(1, sizeof(MONOPOLY_WEB_FILE_HANDLE));
}
static inline HANDLE CreateMutexA(LPSECURITY_ATTRIBUTES, BOOL, LPCSTR)
{
    return calloc(1, sizeof(MONOPOLY_WEB_FILE_HANDLE));
}
static inline BOOL SetEvent(HANDLE) { return TRUE; }
static inline BOOL ResetEvent(HANDLE) { return TRUE; }
static inline BOOL PulseEvent(HANDLE handle) { return SetEvent(handle); }
static inline DWORD WaitForSingleObject(HANDLE, DWORD) { return WAIT_OBJECT_0; }
static inline BOOL ReleaseMutex(HANDLE) { return TRUE; }
#define CreateEvent CreateEventA
#define CreateMutex CreateMutexA
static inline HRESULT monopoly_web_CoCreateInstance(const GUID *, void *, DWORD, const GUID *, void **) { return E_FAIL; }
#define CoCreateInstance(clsid, unknown, context, iid, out) monopoly_web_CoCreateInstance(&(clsid), (unknown), (context), &(iid), (void **)(out))
static inline HRESULT CoInitialize(void *) { return S_OK; }
static inline void CoUninitialize(void) {}
static inline BOOL GetComputerNameA(LPSTR buffer, LPDWORD size)
{
    const char *name = "Web";
    DWORD needed = (DWORD)strlen(name);
    if (!buffer || !size || *size <= needed) {
        if (size) *size = needed + 1;
        return FALSE;
    }
    strcpy(buffer, name);
    *size = needed;
    return TRUE;
}
static inline BOOL GetUserNameA(LPSTR buffer, LPDWORD size)
{
    const char *name = "Player";
    DWORD needed = (DWORD)strlen(name);
    if (!buffer || !size || *size <= needed) {
        if (size) *size = needed + 1;
        return FALSE;
    }
    strcpy(buffer, name);
    *size = needed;
    return TRUE;
}

static inline HGLOBAL GlobalAlloc(UINT flags, size_t bytes)
{
    return (flags & GMEM_ZEROINIT) ? calloc(1, bytes) : malloc(bytes);
}
static inline HGLOBAL GlobalFree(HGLOBAL handle)
{
    free(handle);
    return NULL;
}
static inline size_t GlobalSize(HGLOBAL) { return 0; }
static inline BOOL GlobalUnlock(HGLOBAL) { return TRUE; }
static inline LPVOID GlobalLock(HGLOBAL handle) { return handle; }
static inline HGLOBAL GlobalHandle(LPCVOID ptr) { return (HGLOBAL)ptr; }
static inline HGLOBAL GlobalReAlloc(HGLOBAL handle, size_t bytes, UINT)
{
    return realloc(handle, bytes);
}
static inline HGLOBAL LocalAlloc(UINT flags, size_t bytes) { return GlobalAlloc(flags, bytes); }
static inline HGLOBAL LocalFree(HGLOBAL handle) { return GlobalFree(handle); }
static inline HGLOBAL LocalHandle(LPCVOID ptr) { return GlobalHandle(ptr); }

static inline HRGN CreateRectRgn(int left, int top, int right, int bottom)
{
    MONOPOLY_WEB_RGN *rgn = (MONOPOLY_WEB_RGN *)calloc(1, sizeof(*rgn));
    if (!rgn) return NULL;
    rgn->rect.left = left; rgn->rect.top = top; rgn->rect.right = right; rgn->rect.bottom = bottom;
    return (HRGN)rgn;
}

static inline HRGN CreateRectRgnIndirect(const RECT *rect)
{
    if (!rect) return NULL;
    return CreateRectRgn(rect->left, rect->top, rect->right, rect->bottom);
}

static inline BOOL SetRectRgn(HRGN handle, int left, int top, int right, int bottom)
{
    MONOPOLY_WEB_RGN *rgn = (MONOPOLY_WEB_RGN *)handle;
    if (!rgn) return FALSE;
    rgn->rect.left = left; rgn->rect.top = top; rgn->rect.right = right; rgn->rect.bottom = bottom;
    return TRUE;
}

static inline int CombineRgn(HRGN destHandle, HRGN src1Handle, HRGN src2Handle, int mode)
{
    MONOPOLY_WEB_RGN *dest = (MONOPOLY_WEB_RGN *)destHandle;
    MONOPOLY_WEB_RGN *src1 = (MONOPOLY_WEB_RGN *)src1Handle;
    MONOPOLY_WEB_RGN *src2 = (MONOPOLY_WEB_RGN *)src2Handle;
    if (!dest || !src1) return NULLREGION;
    if (mode == RGN_COPY || !src2) {
        dest->rect = src1->rect;
    } else if (mode == RGN_OR) {
        dest->rect.left = src1->rect.left < src2->rect.left ? src1->rect.left : src2->rect.left;
        dest->rect.top = src1->rect.top < src2->rect.top ? src1->rect.top : src2->rect.top;
        dest->rect.right = src1->rect.right > src2->rect.right ? src1->rect.right : src2->rect.right;
        dest->rect.bottom = src1->rect.bottom > src2->rect.bottom ? src1->rect.bottom : src2->rect.bottom;
    } else if (mode == RGN_AND) {
        dest->rect.left = src1->rect.left > src2->rect.left ? src1->rect.left : src2->rect.left;
        dest->rect.top = src1->rect.top > src2->rect.top ? src1->rect.top : src2->rect.top;
        dest->rect.right = src1->rect.right < src2->rect.right ? src1->rect.right : src2->rect.right;
        dest->rect.bottom = src1->rect.bottom < src2->rect.bottom ? src1->rect.bottom : src2->rect.bottom;
    }
    if (dest->rect.right <= dest->rect.left || dest->rect.bottom <= dest->rect.top) return NULLREGION;
    return SIMPLEREGION;
}

static inline BOOL monopoly_web_DeleteObject(HGDIOBJ object)
{
    if (object) {
        DWORD magic = *((DWORD *)object);
        if (magic == MONOPOLY_WEB_GDI_BITMAP_MAGIC) {
            MONOPOLY_WEB_GDI_BITMAP *bitmap = (MONOPOLY_WEB_GDI_BITMAP *)object;
            if (bitmap->owns_bits) free(bitmap->bits);
            bitmap->bits = NULL;
        }
    }
    free(object);
    return TRUE;
}

static inline DWORD GetFileAttributesA(LPCSTR path)
{
    char normalized[MAX_PATH * 4];
    monopoly_web_normalize_path(normalized, sizeof(normalized), path);
    struct stat st;
    if (!path || stat(normalized, &st) != 0) return INVALID_FILE_ATTRIBUTES;
    DWORD attrs = FILE_ATTRIBUTE_NORMAL;
    if (S_ISDIR(st.st_mode)) attrs |= FILE_ATTRIBUTE_DIRECTORY;
    return attrs;
}

static inline BOOL CreateDirectoryA(LPCSTR path, LPSECURITY_ATTRIBUTES)
{
    char normalized[MAX_PATH * 4];
    monopoly_web_normalize_path(normalized, sizeof(normalized), path);
    if (!path) return FALSE;
    if (mkdir(normalized, 0777) == 0 || errno == EEXIST) return TRUE;
    return FALSE;
}

static inline BOOL DeleteFileA(LPCSTR path)
{
    char normalized[MAX_PATH * 4];
    monopoly_web_normalize_path(normalized, sizeof(normalized), path);
    monopoly_web_profile_clear(path);
    monopoly_web_profile_clear(normalized);
    return path && unlink(normalized) == 0;
}

static inline BOOL WinHelpA(HWND, LPCSTR, UINT, DWORD_PTR) { return FALSE; }
static inline LONG RegOpenKeyExA(HKEY, LPCSTR, DWORD, DWORD, HKEY *) { return 1; }
static inline LONG RegQueryValueExA(HKEY, LPCSTR, LPDWORD, LPDWORD, LPBYTE, LPDWORD) { return 1; }
static inline LONG RegCloseKeyA(HKEY) { return ERROR_SUCCESS; }

static inline DWORD GetLogicalDriveStringsA(DWORD bufferLength, LPSTR buffer)
{
    const char drives[] = "CD:\\\0\0";
    if (buffer && bufferLength > 0) {
        size_t copy = sizeof(drives);
        if (copy > bufferLength) copy = bufferLength;
        memcpy(buffer, drives, copy);
        if (bufferLength >= 2) {
            buffer[bufferLength - 1] = '\0';
            buffer[bufferLength - 2] = '\0';
        }
    }
    return (DWORD)(sizeof(drives) - 1);
}

static inline UINT GetDriveTypeA(LPCSTR rootPathName)
{
    return (rootPathName && strncasecmp(rootPathName, "CD:", 3) == 0) ? DRIVE_CDROM : 0;
}

static inline BOOL GetVolumeInformationA(LPCSTR rootPathName, LPSTR volumeNameBuffer,
    DWORD volumeNameSize, LPDWORD volumeSerialNumber, LPDWORD maximumComponentLength,
    LPDWORD fileSystemFlags, LPSTR fileSystemNameBuffer, DWORD fileSystemNameSize)
{
    if (!rootPathName || strncasecmp(rootPathName, "CD:", 3) != 0) return FALSE;
    if (volumeNameBuffer && volumeNameSize > 0) {
        strncpy(volumeNameBuffer, "MONOPOLY", volumeNameSize);
        volumeNameBuffer[volumeNameSize - 1] = '\0';
    }
    if (volumeSerialNumber) *volumeSerialNumber = 0x19990930u;
    if (maximumComponentLength) *maximumComponentLength = MAX_PATH - 1;
    if (fileSystemFlags) *fileSystemFlags = 0;
    if (fileSystemNameBuffer && fileSystemNameSize > 0) {
        strncpy(fileSystemNameBuffer, "ISO9660", fileSystemNameSize);
        fileSystemNameBuffer[fileSystemNameSize - 1] = '\0';
    }
    return TRUE;
}

static inline UINT GetWindowsDirectoryA(LPSTR buffer, UINT size)
{
    const char *path = "C:\\Windows";
    if (buffer && size > 0) {
        strncpy(buffer, path, size);
        buffer[size - 1] = '\0';
    }
    return (UINT)strlen(path);
}

static inline DWORD GetCurrentDirectoryA(DWORD size, LPSTR buffer)
{
    char cwd[MAX_PATH];
    if (!getcwd(cwd, sizeof(cwd))) return 0;
    if (buffer && size) {
        strncpy(buffer, cwd, size);
        buffer[size - 1] = '\0';
    }
    return (DWORD)strlen(cwd);
}

static inline BOOL SetCurrentDirectoryA(LPCSTR path)
{
    char normalized[MAX_PATH * 4];
    monopoly_web_normalize_path(normalized, sizeof(normalized), path);
    return path && chdir(normalized) == 0;
}

static inline DWORD GetModuleFileNameA(HINSTANCE, LPSTR buffer, DWORD size)
{
    const char *name = "C:\\monopoly\\monopoly.wasm";
    if (buffer && size) {
        strncpy(buffer, name, size);
        buffer[size - 1] = '\0';
    }
    return (DWORD)strlen(name);
}

static inline DWORD GetFullPathNameA(LPCSTR fileName, DWORD bufferLength, LPSTR buffer, LPSTR *filePart)
{
    char normalized[MAX_PATH * 4];
    monopoly_web_normalize_path(normalized, sizeof(normalized), fileName);
    if (normalized[0] != '/' && normalized[0] != '\0') {
        char withRoot[MAX_PATH * 4];
        snprintf(withRoot, sizeof(withRoot), "/monopoly-user/%s", normalized);
        strncpy(normalized, withRoot, sizeof(normalized) - 1);
        normalized[sizeof(normalized) - 1] = '\0';
    }
    DWORD needed = (DWORD)strlen(normalized);
    if (buffer && bufferLength > 0) {
        strncpy(buffer, normalized, bufferLength);
        buffer[bufferLength - 1] = '\0';
        if (filePart) {
            char *slash = strrchr(buffer, '/');
            *filePart = slash ? slash + 1 : buffer;
        }
    }
    return needed;
}

static inline HANDLE CreateFileA(LPCSTR path, DWORD access, DWORD, LPSECURITY_ATTRIBUTES, DWORD disposition, DWORD, HANDLE)
{
    char normalized[MAX_PATH * 4];
    monopoly_web_normalize_path(normalized, sizeof(normalized), path);
    const char *mode = "rb";
    if (access & GENERIC_WRITE) {
        mode = (disposition == OPEN_EXISTING) ? "r+b" : "w+b";
    }
    FILE *file = path ? fopen(normalized, mode) : NULL;
    if (!file) return INVALID_HANDLE_VALUE;
    MONOPOLY_WEB_FILE_HANDLE *handle = (MONOPOLY_WEB_FILE_HANDLE *)calloc(1, sizeof(*handle));
    if (!handle) {
        fclose(file);
        return INVALID_HANDLE_VALUE;
    }
    handle->file = file;
    return (HANDLE)handle;
}

static inline BOOL ReadFile(HANDLE raw, LPVOID buffer, DWORD bytesToRead, LPDWORD bytesRead, LPVOID)
{
    MONOPOLY_WEB_FILE_HANDLE *handle = (MONOPOLY_WEB_FILE_HANDLE *)raw;
    if (!handle || !handle->file || !buffer) return FALSE;
    size_t n = fread(buffer, 1, bytesToRead, handle->file);
    if (bytesRead) *bytesRead = (DWORD)n;
    return TRUE;
}

static inline BOOL WriteFile(HANDLE raw, LPCVOID buffer, DWORD bytesToWrite, LPDWORD bytesWritten, LPVOID)
{
    MONOPOLY_WEB_FILE_HANDLE *handle = (MONOPOLY_WEB_FILE_HANDLE *)raw;
    if (!handle || !handle->file || !buffer) return FALSE;
    size_t n = fwrite(buffer, 1, bytesToWrite, handle->file);
    if (bytesWritten) *bytesWritten = (DWORD)n;
    return n == bytesToWrite;
}

static inline BOOL CloseHandle(HANDLE raw)
{
    if (!raw || raw == INVALID_HANDLE_VALUE) return FALSE;
    MONOPOLY_WEB_FILE_HANDLE *handle = (MONOPOLY_WEB_FILE_HANDLE *)raw;
    if (handle->file) fclose(handle->file);
    if (handle->dir) closedir(handle->dir);
    free(handle);
    return TRUE;
}

static inline BOOL monopoly_web_fill_find_data(const char *directory, const char *name, LPWIN32_FIND_DATAA data)
{
    if (!data || !name) return FALSE;
    ZeroMemory(data, sizeof(*data));
    strncpy(data->cFileName, name, sizeof(data->cFileName) - 1);
    char path[1024];
    snprintf(path, sizeof(path), "%s/%s", directory ? directory : ".", name);
    struct stat st;
    if (stat(path, &st) == 0) {
        if (S_ISDIR(st.st_mode)) data->dwFileAttributes |= FILE_ATTRIBUTE_DIRECTORY;
        data->nFileSizeLow = (DWORD)(st.st_size & 0xffffffffu);
        data->nFileSizeHigh = (DWORD)(((uint64_t)st.st_size >> 32) & 0xffffffffu);
    }
    return TRUE;
}

static inline HANDLE FindFirstFileA(LPCSTR pattern, LPWIN32_FIND_DATAA data)
{
    char normalized[MAX_PATH * 4];
    monopoly_web_normalize_path(normalized, sizeof(normalized), pattern);
    if (!pattern || normalized[0] == '\0') return INVALID_HANDLE_VALUE;

    if (!strchr(normalized, '*') && !strchr(normalized, '?')) {
        struct stat st;
        if (stat(normalized, &st) != 0) return INVALID_HANDLE_VALUE;
        const char *slash = strrchr(normalized, '/');
        const char *name = slash ? slash + 1 : normalized;
        char directory[MAX_PATH * 4] = ".";
        if (slash) {
            size_t len = (size_t)(slash - normalized);
            if (len >= sizeof(directory)) len = sizeof(directory) - 1;
            memcpy(directory, normalized, len);
            directory[len] = '\0';
        }
        if (!monopoly_web_fill_find_data(directory, name, data)) return INVALID_HANDLE_VALUE;
        MONOPOLY_WEB_FILE_HANDLE *handle = (MONOPOLY_WEB_FILE_HANDLE *)calloc(1, sizeof(*handle));
        return handle ? (HANDLE)handle : INVALID_HANDLE_VALUE;
    }

    const char *slash = strrchr(normalized, '/');
    char directory[512] = ".";
    if (slash) {
        size_t len = (size_t)(slash - normalized);
        if (len >= sizeof(directory)) len = sizeof(directory) - 1;
        memcpy(directory, normalized, len);
        directory[len] = '\0';
    }
    DIR *dir = opendir(directory);
    if (!dir) return INVALID_HANDLE_VALUE;
    MONOPOLY_WEB_FILE_HANDLE *handle = (MONOPOLY_WEB_FILE_HANDLE *)calloc(1, sizeof(*handle));
    if (!handle) {
        closedir(dir);
        return INVALID_HANDLE_VALUE;
    }
    handle->dir = dir;
    strncpy(handle->directory, directory, sizeof(handle->directory) - 1);
    struct dirent *entry;
    while ((entry = readdir(dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        monopoly_web_fill_find_data(handle->directory, entry->d_name, data);
        return (HANDLE)handle;
    }
    CloseHandle((HANDLE)handle);
    return INVALID_HANDLE_VALUE;
}

static inline BOOL FindNextFileA(HANDLE raw, LPWIN32_FIND_DATAA data)
{
    MONOPOLY_WEB_FILE_HANDLE *handle = (MONOPOLY_WEB_FILE_HANDLE *)raw;
    if (!handle || !handle->dir) return FALSE;
    struct dirent *entry;
    while ((entry = readdir(handle->dir)) != NULL) {
        if (entry->d_name[0] == '.') continue;
        return monopoly_web_fill_find_data(handle->directory, entry->d_name, data);
    }
    return FALSE;
}

static inline BOOL FindClose(HANDLE raw) { return CloseHandle(raw); }

static inline UINT GetPrivateProfileIntA(LPCSTR section, LPCSTR key, int defaultValue, LPCSTR file)
{
    char value[32];
    if (monopoly_web_profile_get(file, section, key, value, sizeof(value))) {
        return (UINT)strtoul(value, NULL, 10);
    }
    return (UINT)defaultValue;
}

static inline DWORD GetPrivateProfileStringA(LPCSTR section, LPCSTR key, LPCSTR defaultValue, LPSTR returned, DWORD size, LPCSTR file)
{
    const char *value = defaultValue ? defaultValue : "";
    char stored[MAX_PATH * 2];
    if (monopoly_web_profile_get(file, section, key, stored, sizeof(stored))) {
        value = stored;
    }
    if (returned && size) {
        size_t length = strlen(value);
        if (length >= size) {
            length = size - 1;
        }
        memcpy(returned, value, length);
        returned[length] = '\0';
    }
    return (DWORD)strlen(value);
}

static inline BOOL WritePrivateProfileStringA(LPCSTR section, LPCSTR key, LPCSTR value, LPCSTR file)
{
    return monopoly_web_profile_set(file, section, key, value) ? TRUE : FALSE;
}

#ifdef __cplusplus
}
#endif

#endif
