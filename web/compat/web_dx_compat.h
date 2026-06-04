#ifndef MONOPOLY_WEB_DX_COMPAT_H
#define MONOPOLY_WEB_DX_COMPAT_H

#include "windows.h"
#include <math.h>

extern "C" void monopoly_web_present_rgb565(const unsigned char *pixels, int width, int height, int pitch);

typedef float D3DVALUE;
typedef D3DVALUE *LPD3DVALUE;
#ifndef MAKE_DDHRESULT
#define MAKE_DDHRESULT(code) ((HRESULT)(0x88760000u | (DWORD)(code)))
#endif
typedef struct _D3DVECTOR {
    D3DVALUE x, y, z;
    _D3DVECTOR() : x(0), y(0), z(0) {}
    _D3DVECTOR(D3DVALUE X, D3DVALUE Y, D3DVALUE Z) : x(X), y(Y), z(Z) {}
    _D3DVECTOR operator +(const _D3DVECTOR& o) const { return _D3DVECTOR(x+o.x, y+o.y, z+o.z); }
    _D3DVECTOR operator -(const _D3DVECTOR& o) const { return _D3DVECTOR(x-o.x, y-o.y, z-o.z); }
    _D3DVECTOR& operator +=(const _D3DVECTOR& o) { x+=o.x; y+=o.y; z+=o.z; return *this; }
    _D3DVECTOR& operator -=(const _D3DVECTOR& o) { x-=o.x; y-=o.y; z-=o.z; return *this; }
    _D3DVECTOR& operator *=(float s) { x*=s; y*=s; z*=s; return *this; }
    _D3DVECTOR& operator /=(float s) { x/=s; y/=s; z/=s; return *this; }
    bool operator ==(const _D3DVECTOR& o) const { return x == o.x && y == o.y && z == o.z; }
    bool operator !=(const _D3DVECTOR& o) const { return !(*this == o); }
    D3DVALUE& operator [](int index) { return index == 0 ? x : (index == 1 ? y : z); }
    const D3DVALUE& operator [](int index) const { return index == 0 ? x : (index == 1 ? y : z); }
} D3DVECTOR, *LPD3DVECTOR;
static inline D3DVECTOR operator *(float s, const D3DVECTOR& v) { return D3DVECTOR(s*v.x, s*v.y, s*v.z); }
static inline D3DVECTOR operator *(const D3DVECTOR& v, float s) { return D3DVECTOR(s*v.x, s*v.y, s*v.z); }
static inline D3DVECTOR operator /(const D3DVECTOR& v, float s) { return D3DVECTOR(v.x/s, v.y/s, v.z/s); }
static inline D3DVALUE DotProduct(const D3DVECTOR& a, const D3DVECTOR& b) { return a.x * b.x + a.y * b.y + a.z * b.z; }
static inline D3DVECTOR CrossProduct(const D3DVECTOR& a, const D3DVECTOR& b) { return D3DVECTOR(a.y * b.z - a.z * b.y, a.z * b.x - a.x * b.z, a.x * b.y - a.y * b.x); }
static inline D3DVALUE Magnitude(const D3DVECTOR& v) { return (D3DVALUE)sqrtf(v.x * v.x + v.y * v.y + v.z * v.z); }
static inline D3DVECTOR Normalize(const D3DVECTOR& v) {
    D3DVALUE length = Magnitude(v);
    return length > 0.0f ? D3DVECTOR(v.x / length, v.y / length, v.z / length) : D3DVECTOR(0, 0, 0);
}

typedef struct _D3DMATRIX {
    float _11,_12,_13,_14;
    float _21,_22,_23,_24;
    float _31,_32,_33,_34;
    float _41,_42,_43,_44;
    _D3DMATRIX() {}
    _D3DMATRIX(float m11, float m12, float m13, float m14,
               float m21, float m22, float m23, float m24,
               float m31, float m32, float m33, float m34,
               float m41, float m42, float m43, float m44)
        : _11(m11), _12(m12), _13(m13), _14(m14),
          _21(m21), _22(m22), _23(m23), _24(m24),
          _31(m31), _32(m32), _33(m33), _34(m34),
          _41(m41), _42(m42), _43(m43), _44(m44) {}
} D3DMATRIX, *LPD3DMATRIX;

static inline void monopoly_web_dx_identity_matrix(D3DMATRIX *matrix)
{
    if (!matrix) return;
    memset(matrix, 0, sizeof(*matrix));
    matrix->_11 = 1.0f;
    matrix->_22 = 1.0f;
    matrix->_33 = 1.0f;
    matrix->_44 = 1.0f;
}

typedef DWORD D3DCOLOR;
static inline BYTE monopoly_web_d3d_color_channel(float value)
{
    if (value < 0.0f) return 0;
    if (value > 1.0f) return 255;
    return (BYTE)(value * 255.0f);
}
#define D3DRGB(r,g,b) ((D3DCOLOR)(((DWORD)monopoly_web_d3d_color_channel((float)(r)) << 16) | ((DWORD)monopoly_web_d3d_color_channel((float)(g)) << 8) | (DWORD)monopoly_web_d3d_color_channel((float)(b))))
#define D3DCOLOR_MONO 1
#define D3DCOLOR_RGB 2
#define D3DLIGHTINGMODEL_MONO 1
#define D3DLIGHTINGMODEL_RGB 2
typedef struct _D3DCOLORVALUE { float r,g,b,a; } D3DCOLORVALUE;
typedef struct _D3DVERTEX {
    union { D3DVECTOR v; struct { D3DVALUE x, y, z; }; };
    union { D3DVECTOR n; struct { D3DVALUE nx, ny, nz; }; };
    float tu,tv;
    _D3DVERTEX() {}
    _D3DVERTEX(D3DVECTOR V, D3DVECTOR N, float U, float T) : v(V), n(N), tu(U), tv(T) {}
} D3DVERTEX;
typedef struct _D3DLVERTEX { D3DVECTOR v; DWORD color; DWORD specular; float tu,tv; } D3DLVERTEX;
typedef struct _D3DTLVERTEX { D3DVALUE sx,sy,sz,rhw; DWORD color; DWORD specular; float tu,tv; } D3DTLVERTEX;
typedef struct _D3DHVERTEX { DWORD dwFlags; D3DVALUE hx, hy, hz; } D3DHVERTEX, *LPD3DHVERTEX;
typedef struct _D3DRECT { LONG x1,y1,x2,y2; } D3DRECT, *LPD3DRECT;
typedef struct _D3DTRANSFORMDATA {
    DWORD dwSize;
    LPVOID lpIn;
    DWORD dwInSize;
    LPVOID lpOut;
    DWORD dwOutSize;
    LPD3DHVERTEX lpHOut;
    DWORD dwClip;
    DWORD dwClipIntersection;
    DWORD dwClipUnion;
    D3DRECT drExtent;
} D3DTRANSFORMDATA, *LPD3DTRANSFORMDATA;

typedef enum _D3DPRIMITIVETYPE { D3DPT_POINTLIST=1, D3DPT_LINELIST=2, D3DPT_LINESTRIP=3, D3DPT_TRIANGLELIST=4, D3DPT_TRIANGLESTRIP=5, D3DPT_TRIANGLEFAN=6 } D3DPRIMITIVETYPE;
typedef enum _D3DTRANSFORMSTATETYPE { D3DTRANSFORMSTATE_WORLD=1, D3DTRANSFORMSTATE_VIEW=2, D3DTRANSFORMSTATE_PROJECTION=3 } D3DTRANSFORMSTATETYPE;
typedef enum _D3DRENDERSTATETYPE { D3DRENDERSTATE_ZENABLE=7, D3DRENDERSTATE_ZWRITEENABLE=14, D3DRENDERSTATE_ALPHABLENDENABLE=27, D3DRENDERSTATE_SRCBLEND=19, D3DRENDERSTATE_DESTBLEND=20, D3DRENDERSTATE_ZBIAS=47, D3DRENDERSTATE_DITHERENABLE=26, D3DRENDERSTATE_TEXTUREPERSPECTIVE=4, D3DRENDERSTATE_SHADEMODE=9, D3DRENDERSTATE_FILLMODE=8, D3DRENDERSTATE_SPECULARENABLE=29, D3DRENDERSTATE_ANTIALIAS=2, D3DRENDERSTATE_FOGENABLE=28, D3DRENDERSTATE_COLORKEYENABLE=41 } D3DRENDERSTATETYPE;
typedef enum _D3DLIGHTTYPE { D3DLIGHT_POINT=1, D3DLIGHT_SPOT=2, D3DLIGHT_DIRECTIONAL=3, D3DLIGHT_PARALLELPOINT=4, D3DLIGHT_GLSPOT=5 } D3DLIGHTTYPE;
typedef enum _D3DLIGHTSTATETYPE { D3DLIGHTSTATE_AMBIENT=0, D3DLIGHTSTATE_MATERIAL=1 } D3DLIGHTSTATETYPE;
typedef enum _D3DTEXTURESTAGESTATETYPE { D3DTSS_MINFILTER=5, D3DTSS_MAGFILTER=6, D3DTSS_MIPFILTER=7 } D3DTEXTURESTAGESTATETYPE;

#define D3DFVF_XYZ 0x002
#define D3DFVF_XYZRHW 0x004
#define D3DFVF_NORMAL 0x010
#define D3DFVF_DIFFUSE 0x040
#define D3DFVF_SPECULAR 0x080
#define D3DFVF_TEX1 0x100
#define D3DFVF_TEXCOUNT_SHIFT 8
#define D3DFVF_TEXCOUNT_MASK 0x0f00
#define D3DFVF_VERTEX (D3DFVF_XYZ | D3DFVF_NORMAL | D3DFVF_TEX1)
#define D3DFVF_LVERTEX (D3DFVF_XYZ | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)
#define D3DFVF_TLVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_SPECULAR | D3DFVF_TEX1)
#define D3DANTIALIAS_NONE 0
#define D3DANTIALIAS_SORTINDEPENDENT 1
#define D3DPRASTERCAPS_ANTIALIASSORTINDEPENDENT 0x1
#define D3DSHADE_GOURAUD 2
#define D3DFILL_SOLID 3
#define D3DTFN_POINT 1
#define D3DTFN_LINEAR 2
#define D3DTFG_POINT 1
#define D3DTFG_LINEAR 2
#define D3DTFP_NONE 1
#define D3DTFP_POINT 2
#define D3DTFP_LINEAR 3
#define D3DBLEND_ZERO 1
#define D3DBLEND_ONE 2
#define D3DBLEND_SRCCOLOR 3
#define D3DBLEND_INVSRCCOLOR 4
#define D3DBLEND_SRCALPHA 5
#define D3DBLEND_INVSRCALPHA 6
#define D3DBLEND_DESTALPHA 7
#define D3DBLEND_INVDESTALPHA 8
#define D3DBLEND_DESTCOLOR 9
#define D3DBLEND_INVDESTCOLOR 10
#define D3DCLEAR_TARGET 0x1
#define D3DCLEAR_ZBUFFER 0x2
#define D3DTRANSFORM_CLIPPED 0x1
#define D3DTRANSFORM_UNCLIPPED 0x2
#define D3DLIGHT_ACTIVE 0x1
#define D3DLIGHT_RANGE_MAX 1000000.0f
#define D3D_OK 0
#define D3DERR_BADMAJORVERSION ((HRESULT)0x88760200)
#define D3DERR_BADMINORVERSION ((HRESULT)0x88760201)
#define D3DERR_CLIPPED_FAILED ((HRESULT)0x88760202)
#define D3DERR_EXECUTE_CLIPPED_FAILED ((HRESULT)0x88760203)
#define D3DERR_EXECUTE_CREATE_FAILED ((HRESULT)0x88760204)
#define D3DERR_EXECUTE_DESTROY_FAILED ((HRESULT)0x88760205)
#define D3DERR_EXECUTE_FAILED ((HRESULT)0x88760206)
#define D3DERR_EXECUTE_LOCKED ((HRESULT)0x88760207)
#define D3DERR_EXECUTE_LOCK_FAILED ((HRESULT)0x88760208)
#define D3DERR_EXECUTE_NOT_LOCKED ((HRESULT)0x88760209)
#define D3DERR_EXECUTE_UNLOCK_FAILED ((HRESULT)0x8876020A)
#define D3DERR_INBEGIN ((HRESULT)0x8876020B)
#define D3DERR_INITFAILED ((HRESULT)0x8876020C)
#define D3DERR_INVALIDCURRENTVIEWPORT ((HRESULT)0x8876020D)
#define D3DERR_INVALIDPALETTE ((HRESULT)0x8876020E)
#define D3DERR_INVALIDPRIMITIVETYPE ((HRESULT)0x8876020F)
#define D3DERR_INVALIDVERTEXTYPE ((HRESULT)0x88760210)
#define D3DERR_INVALID_DEVICE ((HRESULT)0x88760211)
#define D3DERR_LIGHTHASVIEWPORT ((HRESULT)0x88760212)
#define D3DERR_LIGHTNOTINTHISVIEWPORT ((HRESULT)0x88760213)
#define D3DERR_LIGHT_SET_FAILED ((HRESULT)0x88760214)
#define D3DERR_LOAD_FAILED ((HRESULT)0x88760215)
#define D3DERR_MATERIAL_CREATE_FAILED ((HRESULT)0x88760216)
#define D3DERR_MATERIAL_DESTROY_FAILED ((HRESULT)0x88760217)
#define D3DERR_MATERIAL_GETDATA_FAILED ((HRESULT)0x88760218)
#define D3DERR_MATERIAL_SETDATA_FAILED ((HRESULT)0x88760219)
#define D3DERR_MATRIX_CREATE_FAILED ((HRESULT)0x8876021A)
#define D3DERR_MATRIX_DESTROY_FAILED ((HRESULT)0x8876021B)
#define D3DERR_MATRIX_GETDATA_FAILED ((HRESULT)0x8876021C)
#define D3DERR_MATRIX_SETDATA_FAILED ((HRESULT)0x8876021D)
#define D3DERR_MAXTRIX_GETDATA_FAILED ((HRESULT)0x8876021E)
#define D3DERR_NOTINBEGIN ((HRESULT)0x8876021F)
#define D3DERR_NOVIEWPORTS ((HRESULT)0x88760220)
#define D3DERR_SCENE_BEGIN_FAILED ((HRESULT)0x88760221)
#define D3DERR_SCENE_END_FAILED ((HRESULT)0x88760222)
#define D3DERR_SCENE_IN_SCENE ((HRESULT)0x88760223)
#define D3DERR_SCENE_NOT_IN_SCENE ((HRESULT)0x88760224)
#define D3DERR_SETVIEWPORTDATA_FAILED ((HRESULT)0x88760225)
#define D3DERR_SURFACENOTINVIDMEM ((HRESULT)0x88760226)
#define D3DERR_SWAP_FAILED ((HRESULT)0x88760227)
#define D3DERR_TEXTURE_BADSIZE ((HRESULT)0x88760228)
#define D3DERR_TEXTURE_CREATE_FAILED ((HRESULT)0x88760229)
#define D3DERR_TEXTURE_DESTROY_FAILED ((HRESULT)0x8876022A)
#define D3DERR_TEXTURE_GETSURF_FAILED ((HRESULT)0x8876022B)
#define D3DERR_TEXTURE_LOAD_FAILED ((HRESULT)0x8876022C)
#define D3DERR_TEXTURE_LOCKED ((HRESULT)0x8876022D)
#define D3DERR_TEXTURE_LOCK_FAILED ((HRESULT)0x8876022E)
#define D3DERR_TEXTURE_NOT_LOCKED ((HRESULT)0x8876022F)
#define D3DERR_TEXTURE_NO_SUPPORT ((HRESULT)0x88760230)
#define D3DERR_TEXTURE_SWAP_FAILED ((HRESULT)0x88760231)
#define D3DERR_TEXTURE_UNLOCK_FAILED ((HRESULT)0x88760232)
#define D3DERR_VIEWPORTDATANOTSET ((HRESULT)0x88760233)
#define D3DERR_VIEWPORTHASNODEVICE ((HRESULT)0x88760234)
#define D3DERR_ZBUFF_NEEDS_SYSTEMMEMORY ((HRESULT)0x88760235)
#define D3DERR_ZBUFF_NEEDS_VIDEOMEMORY ((HRESULT)0x88760236)

typedef struct _D3DVIEWPORT { DWORD dwSize; DWORD dwX; DWORD dwY; DWORD dwWidth; DWORD dwHeight; D3DVALUE dvScaleX; D3DVALUE dvScaleY; D3DVALUE dvMaxX; D3DVALUE dvMaxY; D3DVALUE dvMinZ; D3DVALUE dvMaxZ; } D3DVIEWPORT, *LPD3DVIEWPORT;
typedef struct _D3DVIEWPORT2 { DWORD dwSize; DWORD dwX; DWORD dwY; DWORD dwWidth; DWORD dwHeight; D3DVALUE dvClipX; D3DVALUE dvClipY; D3DVALUE dvClipWidth; D3DVALUE dvClipHeight; D3DVALUE dvMinZ; D3DVALUE dvMaxZ; } D3DVIEWPORT2, *LPD3DVIEWPORT2;
typedef struct _D3DCLIPSTATUS { DWORD dwFlags; DWORD dwStatus; float minx,maxx,miny,maxy,minz,maxz; } D3DCLIPSTATUS, *LPD3DCLIPSTATUS;
typedef struct _D3DPRIMCAPS { DWORD dwSize; DWORD dwMiscCaps; DWORD dwRasterCaps; DWORD dwZCmpCaps; DWORD dwSrcBlendCaps; DWORD dwDestBlendCaps; DWORD dwAlphaCmpCaps; DWORD dwShadeCaps; DWORD dwTextureCaps; DWORD dwTextureFilterCaps; DWORD dwTextureBlendCaps; DWORD dwTextureAddressCaps; DWORD dwStippleWidth; DWORD dwStippleHeight; } D3DPRIMCAPS, *LPD3DPRIMCAPS;
typedef struct _D3DDEVICEDESC { DWORD dwSize; DWORD dcmColorModel; DWORD dwDeviceRenderBitDepth; DWORD dwDeviceZBufferBitDepth; D3DPRIMCAPS dpcLineCaps; D3DPRIMCAPS dpcTriCaps; } D3DDEVICEDESC, *LPD3DDEVICEDESC;
typedef struct _D3DMATERIAL { DWORD dwSize; D3DCOLORVALUE diffuse; D3DCOLORVALUE ambient; D3DCOLORVALUE specular; D3DCOLORVALUE emissive; float power; DWORD dwRampSize; } D3DMATERIAL, *LPD3DMATERIAL;
typedef struct _D3DLIGHT2 {
    DWORD dwSize;
    D3DLIGHTTYPE dltType;
    D3DCOLORVALUE dcvColor;
    D3DVECTOR dvPosition;
    D3DVECTOR dvDirection;
    float dvRange;
    float dvFalloff;
    float dvAttenuation0;
    float dvAttenuation1;
    float dvAttenuation2;
    float dvTheta;
    float dvPhi;
    DWORD dwFlags;
} D3DLIGHT2, D3DLIGHT, *LPD3DLIGHT2, *LPD3DLIGHT;

typedef struct _D3DDRAWPRIMITIVESTRIDEDDATA {
    struct { void *lpvData; DWORD dwStride; } position;
    struct { void *lpvData; DWORD dwStride; } normal;
    struct { void *lpvData; DWORD dwStride; } diffuse;
    struct { void *lpvData; DWORD dwStride; } specular;
    struct { void *lpvData; DWORD dwStride; } textureCoords[8];
} D3DDRAWPRIMITIVESTRIDEDDATA, *LPD3DDRAWPRIMITIVESTRIDEDDATA;
typedef D3DDRAWPRIMITIVESTRIDEDDATA D3DSTRIDEDATA, *PD3DSTRIDEDATA, *LPD3DSTRIDEDATA;

typedef struct _PALETTEENTRY { BYTE peRed; BYTE peGreen; BYTE peBlue; BYTE peFlags; } PALETTEENTRY, *LPPALETTEENTRY;
typedef struct _DDCOLORKEY { DWORD dwColorSpaceLowValue; DWORD dwColorSpaceHighValue; } DDCOLORKEY, *LPDDCOLORKEY;
typedef struct _DDPIXELFORMAT { DWORD dwSize; DWORD dwFlags; DWORD dwFourCC; DWORD dwRGBBitCount; DWORD dwRBitMask; DWORD dwGBitMask; DWORD dwBBitMask; DWORD dwRGBAlphaBitMask; DWORD dwZBufferBitDepth; } DDPIXELFORMAT, *LPDDPIXELFORMAT;
typedef struct _DDSCAPS { DWORD dwCaps; } DDSCAPS, *LPDDSCAPS;
typedef struct _DDSCAPS2 { DWORD dwCaps; DWORD dwCaps2; DWORD dwCaps3; DWORD dwCaps4; } DDSCAPS2, *LPDDSCAPS2;
typedef struct _DDSURFACEDESC { DWORD dwSize; DWORD dwFlags; DWORD dwHeight; DWORD dwWidth; LONG lPitch; LPVOID lpSurface; DDPIXELFORMAT ddpfPixelFormat; DDSCAPS ddsCaps; DWORD dwBackBufferCount; DWORD dwMipMapCount; } DDSURFACEDESC, *LPDDSURFACEDESC;
typedef struct _DDSURFACEDESC2 { DWORD dwSize; DWORD dwFlags; DWORD dwHeight; DWORD dwWidth; LONG lPitch; LPVOID lpSurface; DDPIXELFORMAT ddpfPixelFormat; DDSCAPS2 ddsCaps; DWORD dwBackBufferCount; DWORD dwMipMapCount; } DDSURFACEDESC2, *LPDDSURFACEDESC2;
typedef struct _DDCAPS { DWORD dwSize; DWORD dwCaps; DWORD dwCaps2; DWORD dwSVBCaps; } DDCAPS, *LPDDCAPS;
typedef struct _DDDEVICEIDENTIFIER { char szDriver[512]; char szDescription[512]; GUID guidDeviceIdentifier; } DDDEVICEIDENTIFIER, *LPDDDEVICEIDENTIFIER;
typedef struct _DDBLTFX { DWORD dwSize; DWORD dwFillColor; DWORD dwROP; DDCOLORKEY ddckDestColorkey; DDCOLORKEY ddckSrcColorkey; } DDBLTFX, *LPDDBLTFX;
typedef struct _DDBLTBATCH { LPRECT lprDest; struct IDirectDrawSurface4 *lpDDSSrc; LPRECT lprSrc; DWORD dwFlags; LPDDBLTFX lpDDBltFx; } DDBLTBATCH, *LPDDBLTBATCH;

#ifdef __cplusplus
extern "C" {
#endif
void monopoly_webgl_begin_frame(void);
void monopoly_webgl_begin_frame_from_rgb565(const unsigned char *pixels, int width, int height, int pitch);
void monopoly_webgl_clear(DWORD flags, DWORD color, float depth);
void monopoly_webgl_read_rgb565(unsigned char *pixels, int width, int height, int pitch);
void monopoly_webgl_set_render_state(DWORD state, DWORD value);
void monopoly_webgl_set_texture(const unsigned char *pixels, int width, int height, int pitch, int bpp,
                                const DDPIXELFORMAT *format,
                                const PALETTEENTRY *palette, int paletteEntries,
                                int hasSrcColorKey, DWORD colorKeyLow, DWORD colorKeyHigh);
void monopoly_webgl_set_viewport(int x, int y, int width, int height);
int monopoly_webgl_draw_primitive(D3DPRIMITIVETYPE primitiveType,
                                  DWORD vertexType,
                                  const void *vertices,
                                  DWORD vertexCount,
                                  const WORD *indices,
                                  DWORD indexCount,
                                  const D3DSTRIDEDATA *strideData,
                                  const D3DMATRIX *world,
                                  const D3DMATRIX *view,
                                  const D3DMATRIX *projection);
#ifdef __cplusplus
}
#endif

#define DD_OK 0
#define DDERR_SURFACEBUSY ((HRESULT)0x88760001)
#define DDERR_WASSTILLDRAWING ((HRESULT)0x88760002)
#define DDERR_SURFACELOST ((HRESULT)0x88760003)
#define DDERR_UNSUPPORTED ((HRESULT)0x88760004)
#ifndef DDERR_ALREADYINITIALIZED
#define DDERR_ALREADYINITIALIZED ((HRESULT)0x88760100)
#define DDERR_BLTFASTCANTCLIP ((HRESULT)0x88760101)
#define DDERR_CANNOTATTACHSURFACE ((HRESULT)0x88760102)
#define DDERR_CANNOTDETACHSURFACE ((HRESULT)0x88760103)
#define DDERR_CANTCREATEDC ((HRESULT)0x88760104)
#define DDERR_CANTDUPLICATE ((HRESULT)0x88760105)
#define DDERR_CANTLOCKSURFACE ((HRESULT)0x88760106)
#define DDERR_CANTPAGELOCK ((HRESULT)0x88760107)
#define DDERR_CANTPAGEUNLOCK ((HRESULT)0x88760108)
#define DDERR_CLIPPERISUSINGHWND ((HRESULT)0x88760109)
#define DDERR_COLORKEYNOTSET ((HRESULT)0x8876010A)
#define DDERR_CURRENTLYNOTAVAIL ((HRESULT)0x8876010B)
#define DDERR_DCALREADYCREATED ((HRESULT)0x8876010C)
#define DDERR_DEVICEDOESNTOWNSURFACE ((HRESULT)0x8876010D)
#define DDERR_DIRECTDRAWALREADYCREATED ((HRESULT)0x8876010E)
#define DDERR_EXCEPTION ((HRESULT)0x8876010F)
#define DDERR_EXCLUSIVEMODEALREADYSET ((HRESULT)0x88760110)
#define DDERR_EXPIRED ((HRESULT)0x88760111)
#define DDERR_GENERIC ((HRESULT)0x88760112)
#define DDERR_HEIGHTALIGN ((HRESULT)0x88760113)
#define DDERR_HWNDALREADYSET ((HRESULT)0x88760114)
#define DDERR_HWNDSUBCLASSED ((HRESULT)0x88760115)
#define DDERR_IMPLICITLYCREATED ((HRESULT)0x88760116)
#define DDERR_INCOMPATIBLEPRIMARY ((HRESULT)0x88760117)
#define DDERR_INVALIDCAPS ((HRESULT)0x88760118)
#define DDERR_INVALIDCLIPLIST ((HRESULT)0x88760119)
#define DDERR_INVALIDDIRECTDRAWGUID ((HRESULT)0x8876011A)
#define DDERR_INVALIDMODE ((HRESULT)0x8876011B)
#define DDERR_INVALIDOBJECT ((HRESULT)0x8876011C)
#define DDERR_INVALIDPARAMS ((HRESULT)0x8876011D)
#define DDERR_INVALIDPIXELFORMAT ((HRESULT)0x8876011E)
#define DDERR_INVALIDPOSITION ((HRESULT)0x8876011F)
#define DDERR_INVALIDRECT ((HRESULT)0x88760120)
#define DDERR_INVALIDSTREAM ((HRESULT)0x88760121)
#define DDERR_INVALIDSURFACETYPE ((HRESULT)0x88760122)
#define DDERR_LOCKEDSURFACES ((HRESULT)0x88760123)
#define DDERR_MOREDATA ((HRESULT)0x88760124)
#define DDERR_NO3D ((HRESULT)0x88760125)
#define DDERR_NOALPHAHW ((HRESULT)0x88760126)
#define DDERR_NOBLTHW ((HRESULT)0x88760127)
#define DDERR_NOCLIPLIST ((HRESULT)0x88760128)
#define DDERR_NOCLIPPERATTACHED ((HRESULT)0x88760129)
#define DDERR_NOCOLORCONVHW ((HRESULT)0x8876012A)
#define DDERR_NOCOLORKEY ((HRESULT)0x8876012B)
#define DDERR_NOCOLORKEYHW ((HRESULT)0x8876012C)
#define DDERR_NOCOOPERATIVELEVELSET ((HRESULT)0x8876012D)
#define DDERR_NODC ((HRESULT)0x8876012E)
#define DDERR_NODDROPSHW ((HRESULT)0x8876012F)
#define DDERR_NODIRECTDRAWHW ((HRESULT)0x88760130)
#define DDERR_NODIRECTDRAWSUPPORT ((HRESULT)0x88760131)
#define DDERR_NOEMULATION ((HRESULT)0x88760132)
#define DDERR_NOEXCLUSIVEMODE ((HRESULT)0x88760133)
#define DDERR_NOFLIPHW ((HRESULT)0x88760134)
#define DDERR_NOFOCUSWINDOW ((HRESULT)0x88760135)
#define DDERR_NOGDI ((HRESULT)0x88760136)
#define DDERR_NOHWND ((HRESULT)0x88760137)
#define DDERR_NOMIPMAPHW ((HRESULT)0x88760138)
#define DDERR_NOMIRRORHW ((HRESULT)0x88760139)
#define DDERR_NONONLOCALVIDMEM ((HRESULT)0x8876013A)
#define DDERR_NOOPTIMIZEHW ((HRESULT)0x8876013B)
#define DDERR_NOOVERLAYDEST ((HRESULT)0x8876013C)
#define DDERR_NOOVERLAYHW ((HRESULT)0x8876013D)
#define DDERR_NOPALETTEATTACHED ((HRESULT)0x8876013E)
#define DDERR_NOPALETTEHW ((HRESULT)0x8876013F)
#define DDERR_NORASTEROPHW ((HRESULT)0x88760140)
#define DDERR_NOROTATIONHW ((HRESULT)0x88760141)
#define DDERR_NOSTRETCHHW ((HRESULT)0x88760142)
#define DDERR_NOT4BITCOLOR ((HRESULT)0x88760143)
#define DDERR_NOT4BITCOLORINDEX ((HRESULT)0x88760144)
#define DDERR_NOT8BITCOLOR ((HRESULT)0x88760145)
#define DDERR_NOTAOVERLAYSURFACE ((HRESULT)0x88760146)
#define DDERR_NOTEXTUREHW ((HRESULT)0x88760147)
#define DDERR_NOTFLIPPABLE ((HRESULT)0x88760148)
#define DDERR_NOTFOUND ((HRESULT)0x88760149)
#define DDERR_NOTINITIALIZED ((HRESULT)0x8876014A)
#define DDERR_NOTLOADED ((HRESULT)0x8876014B)
#define DDERR_NOTLOCKED ((HRESULT)0x8876014C)
#define DDERR_NOTPAGELOCKED ((HRESULT)0x8876014D)
#define DDERR_NOTPALETTIZED ((HRESULT)0x8876014E)
#define DDERR_NOVSYNCHW ((HRESULT)0x8876014F)
#define DDERR_NOZBUFFERHW ((HRESULT)0x88760150)
#define DDERR_NOZOVERLAYHW ((HRESULT)0x88760151)
#define DDERR_OUTOFCAPS ((HRESULT)0x88760152)
#define DDERR_OUTOFMEMORY ((HRESULT)0x88760153)
#define DDERR_OUTOFVIDEOMEMORY ((HRESULT)0x88760154)
#define DDERR_OVERLAPPINGRECTS ((HRESULT)0x88760155)
#define DDERR_OVERLAYCANTCLIP ((HRESULT)0x88760156)
#define DDERR_OVERLAYCOLORKEYONLYONEACTIVE ((HRESULT)0x88760157)
#define DDERR_OVERLAYNOTVISIBLE ((HRESULT)0x88760158)
#define DDERR_PALETTEBUSY ((HRESULT)0x88760159)
#define DDERR_PRIMARYSURFACEALREADYEXISTS ((HRESULT)0x8876015A)
#define DDERR_REGIONTOOSMALL ((HRESULT)0x8876015B)
#define DDERR_SURFACEALREADYATTACHED ((HRESULT)0x8876015C)
#define DDERR_SURFACEALREADYDEPENDENT ((HRESULT)0x8876015D)
#define DDERR_SURFACEISOBSCURED ((HRESULT)0x8876015E)
#define DDERR_SURFACENOTATTACHED ((HRESULT)0x8876015F)
#define DDERR_TOOBIGHEIGHT ((HRESULT)0x88760160)
#define DDERR_TOOBIGSIZE ((HRESULT)0x88760161)
#define DDERR_TOOBIGWIDTH ((HRESULT)0x88760162)
#define DDERR_UNSUPPORTEDFORMAT ((HRESULT)0x88760163)
#define DDERR_UNSUPPORTEDMASK ((HRESULT)0x88760164)
#define DDERR_UNSUPPORTEDMODE ((HRESULT)0x88760165)
#define DDERR_VERTICALBLANKINPROGRESS ((HRESULT)0x88760166)
#define DDERR_VIDEONOTACTIVE ((HRESULT)0x88760167)
#define DDERR_WRONGMODE ((HRESULT)0x88760168)
#define DDERR_XALIGN ((HRESULT)0x88760169)
#endif
#define DDSD_CAPS 0x1
#define DDSD_HEIGHT 0x2
#define DDSD_WIDTH 0x4
#define DDSD_PITCH 0x8
#define DDSD_BACKBUFFERCOUNT 0x20
#define DDSD_MIPMAPCOUNT 0x20000
#define DDSD_PIXELFORMAT 0x1000
#define DDSCAPS_3DDEVICE 0x1
#define DDSCAPS_TEXTURE 0x2
#define DDSCAPS_VIDEOMEMORY 0x4
#define DDSCAPS_NONLOCALVIDMEM 0x8
#define DDSCAPS_LOCALVIDMEM 0x10
#define DDSCAPS_SYSTEMMEMORY 0x20
#define DDSCAPS_BACKBUFFER 0x40
#define DDSCAPS_OFFSCREENPLAIN 0x80
#define DDSCAPS_PRIMARYSURFACE 0x100
#define DDSCAPS_FLIP 0x200
#define DDSCAPS_COMPLEX 0x400
#define DDSCAPS_ZBUFFER 0x800
#define DDSCAPS_MIPMAP 0x1000
#define DDSCAPS2_TEXTUREMANAGE 0x00000001
#define MAX_DDDEVICEID_STRING 512
#define DDCAPS_BLT 0x1
#define DDCAPS_BLTSTRETCH 0x2
#define DDCAPS2_CANRENDERWINDOWED 0x00080000
#define DDPCAPS_8BIT 0x4
#define DDPCAPS_ALLOW256 0x40
#define DDPCAPS_ALPHA 0x400
#define DDPCAPS_INITIALIZE 0x1
#define DDSDM_STANDARDVGAMODE 0x1
#define DDENUMRET_CANCEL 0
#define DDENUMRET_OK 1
#define DDENUM_ATTACHEDSECONDARYDEVICES 0x1
#define DDENUM_NONDISPLAYDEVICES 0x2
#define D3DENUMRET_CANCEL 0
#define D3DENUMRET_OK 1
#define DDLOCK_WAIT 0x1
#define DDLOCK_SURFACEMEMORYPTR 0x2
#define DDLOCK_NOSYSLOCK 0x4
#define DDLOCK_WRITEONLY 0x8
#define DDLOCK_READONLY 0x10
#define DDBLT_WAIT 0x1
#define DDBLT_ROP 0x20000
#define DDBLT_COLORFILL 0x400
#define DDBLTFAST_WAIT 0x10
#define DDBLTFAST_NOCOLORKEY 0x0
#define DDBLTFAST_SRCCOLORKEY 0x1
#define DDCKEY_SRCBLT 0x8
#define DDBD_1 0x1
#define DDBD_2 0x2
#define DDBD_4 0x4
#define DDBD_8 0x8
#define DDBD_16 0x10
#define DDBD_24 0x20
#define DDBD_32 0x40
#define DDSCL_NORMAL 0x1
#define DDSCL_EXCLUSIVE 0x2
#define DDSCL_FULLSCREEN 0x4
#define DDSCL_ALLOWREBOOT 0x8
#define DDSCL_MULTITHREADED 0x10
#define DDFLIP_WAIT 0x1
#define DDPF_ALPHAPIXELS 0x1
#define DDPF_ALPHA 0x2
#define DDPF_ALPHAPREMULT 0x4
#define DDPF_PALETTEINDEXED1 0x00000800
#define DDPF_PALETTEINDEXED2 0x00001000
#define DDPF_PALETTEINDEXED4 0x8
#define DDPF_PALETTEINDEXEDTO8 0x10
#define DDPF_PALETTEINDEXED8 0x20
#define DDPF_RGB 0x40

struct IDirectDraw;
struct IDirectDraw2;
struct IDirectDraw4;
struct IDirectDrawSurface;
struct IDirectDrawSurface2;
struct IDirectDrawSurface3;
struct IDirectDrawSurface4;
struct IDirectDrawPalette;
struct IDirectDrawClipper;
struct IDirect3D;
struct IDirect3D2;
struct IDirect3D3;
struct IDirect3DDevice;
struct IDirect3DDevice2;
struct IDirect3DDevice3;
struct IDirect3DTexture;
struct IDirect3DTexture2;
struct IDirect3DViewport;
struct IDirect3DViewport2;
struct IDirect3DViewport3;
struct IDirect3DMaterial;
struct IDirect3DMaterial2;
struct IDirect3DMaterial3;
struct IDirect3DLight;
struct IDirect3DVertexBuffer;

typedef IDirectDraw *LPDIRECTDRAW;
typedef IDirectDraw2 *LPDIRECTDRAW2;
typedef IDirectDraw4 *LPDIRECTDRAW4;
typedef IDirectDrawSurface *LPDIRECTDRAWSURFACE;
typedef IDirectDrawSurface2 *LPDIRECTDRAWSURFACE2;
typedef IDirectDrawSurface3 *LPDIRECTDRAWSURFACE3;
typedef IDirectDrawSurface4 *LPDIRECTDRAWSURFACE4;
typedef IDirectDrawPalette *LPDIRECTDRAWPALETTE;
typedef IDirectDrawClipper *LPDIRECTDRAWCLIPPER;
typedef IDirect3D *LPDIRECT3D;
typedef IDirect3D2 *LPDIRECT3D2;
typedef IDirect3D3 *LPDIRECT3D3;
typedef IDirect3DDevice *LPDIRECT3DDEVICE;
typedef IDirect3DDevice2 *LPDIRECT3DDEVICE2;
typedef IDirect3DDevice3 *LPDIRECT3DDEVICE3;
typedef IDirect3DTexture *LPDIRECT3DTEXTURE;
typedef IDirect3DTexture2 *LPDIRECT3DTEXTURE2;
typedef IDirect3DViewport *LPDIRECT3DVIEWPORT;
typedef IDirect3DViewport2 *LPDIRECT3DVIEWPORT2;
typedef IDirect3DViewport3 *LPDIRECT3DVIEWPORT3;
typedef IDirect3DMaterial *LPDIRECT3DMATERIAL;
typedef IDirect3DMaterial2 *LPDIRECT3DMATERIAL2;
typedef IDirect3DMaterial3 *LPDIRECT3DMATERIAL3;
typedef IDirect3DLight *LPDIRECT3DLIGHT;
typedef IDirect3DVertexBuffer *LPDIRECT3DVERTEXBUFFER;

typedef HRESULT (WINAPI *LPDDENUMMODESCALLBACK2)(LPDDSURFACEDESC2, LPVOID);
typedef HRESULT (WINAPI *LPDDENUMSURFACESCALLBACK2)(LPDIRECTDRAWSURFACE4, LPDDSURFACEDESC2, LPVOID);
typedef BOOL (WINAPI *LPDDENUMCALLBACKEXA)(GUID FAR *, LPSTR, LPSTR, LPVOID, HMONITOR);
typedef HRESULT (CALLBACK *LPD3DENUMDEVICESCALLBACK)(LPGUID, LPSTR, LPSTR, LPD3DDEVICEDESC, LPD3DDEVICEDESC, LPVOID);
typedef HRESULT (CALLBACK *LPD3DENUMPIXELFORMATSCALLBACK)(LPDDPIXELFORMAT, LPVOID);

typedef DWORD D3DTEXTUREHANDLE;
typedef DWORD D3DMATERIALHANDLE;
typedef struct _D3DVERTEXBUFFERDESC { DWORD dwSize; DWORD dwCaps; DWORD dwFVF; DWORD dwNumVertices; } D3DVERTEXBUFFERDESC, *LPD3DVERTEXBUFFERDESC;

struct IDirectDrawPalette {
    PALETTEENTRY m_entries[256];
    DWORD m_count;
    IDirectDrawPalette(LPPALETTEENTRY entries = NULL, DWORD count = 256) : m_count(count > 256 ? 256 : count) {
        memset(m_entries, 0, sizeof(m_entries));
        if (entries) {
            memcpy(m_entries, entries, sizeof(PALETTEENTRY) * m_count);
        } else {
            for (DWORD i = 0; i < 256; ++i) {
                m_entries[i].peRed = (BYTE)i;
                m_entries[i].peGreen = (BYTE)i;
                m_entries[i].peBlue = (BYTE)i;
                m_entries[i].peFlags = 255;
            }
            m_count = 256;
        }
    }
    ULONG AddRef() { return 1; }
    ULONG Release() { return 0; }
    HRESULT GetEntries(DWORD, DWORD start, DWORD count, LPPALETTEENTRY entries) {
        if (!entries) return DD_OK;
        memset(entries, 0, sizeof(PALETTEENTRY) * count);
        if (start >= m_count) return DD_OK;
        DWORD available = m_count - start;
        if (count > available) count = available;
        memcpy(entries, m_entries + start, sizeof(PALETTEENTRY) * count);
        return DD_OK;
    }
    HRESULT SetEntries(DWORD, DWORD start, DWORD count, LPPALETTEENTRY entries) {
        if (!entries || start >= 256) return DD_OK;
        if (count > 256 - start) count = 256 - start;
        memcpy(m_entries + start, entries, sizeof(PALETTEENTRY) * count);
        if (m_count < start + count) m_count = start + count;
        return DD_OK;
    }
};

struct IDirectDrawClipper {
    ULONG AddRef() { return 1; }
    ULONG Release() { return 0; }
    HRESULT SetHWnd(DWORD, HWND) { return DD_OK; }
    HRESULT GetClipList(LPRECT rect, LPRGNDATA data, LPDWORD size) {
        RECT full;
        if (rect) full = *rect;
        else { full.left = 0; full.top = 0; full.right = 800; full.bottom = 600; }
        DWORD required = sizeof(RGNDATAHEADER) + sizeof(RECT);
        if (size) {
            if (!data || *size < required) {
                *size = required;
                return DD_OK;
            }
            *size = required;
        }
        if (data) {
            ZeroMemory(data, required);
            data->rdh.dwSize = sizeof(RGNDATAHEADER);
            data->rdh.iType = RDH_RECTANGLES;
            data->rdh.nCount = 1;
            data->rdh.nRgnSize = sizeof(RECT);
            data->rdh.rcBound = full;
            memcpy(data->Buffer, &full, sizeof(RECT));
        }
        return DD_OK;
    }
    HRESULT SetClipList(LPRGNDATA, DWORD) { return DD_OK; }
};

struct IDirect3DTexture2 {
    ULONG m_refs;
    LPDIRECTDRAWSURFACE4 m_surface;

    IDirect3DTexture2(LPDIRECTDRAWSURFACE4 surface = NULL);
    ULONG AddRef();
    ULONG Release();
    HRESULT GetHandle(LPDIRECT3DDEVICE3, D3DTEXTUREHANDLE *handle) { if (handle) *handle = 0; return DD_OK; }
    HRESULT Load(LPDIRECT3DTEXTURE2 source);
};

static inline void monopoly_web_dx_fill_pixel_format(LPDDPIXELFORMAT format, DWORD bpp)
{
    if (!format) return;
    memset(format, 0, sizeof(*format));
    format->dwSize = sizeof(*format);
    format->dwFlags = DDPF_RGB;
    format->dwRGBBitCount = bpp;
    if (bpp == 16) {
        format->dwRBitMask = 0x0000f800;
        format->dwGBitMask = 0x000007e0;
        format->dwBBitMask = 0x0000001f;
    } else {
        format->dwRBitMask = 0x00ff0000;
        format->dwGBitMask = 0x0000ff00;
        format->dwBBitMask = 0x000000ff;
        if (bpp == 32) {
            format->dwRGBAlphaBitMask = 0xff000000;
        }
    }
}

static inline void monopoly_web_dx_fill_surface_desc(LPDDSURFACEDESC2 desc, DWORD width, DWORD height, DWORD bpp)
{
    if (!desc) return;
    memset(desc, 0, sizeof(*desc));
    desc->dwSize = sizeof(*desc);
    desc->dwFlags = DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_CAPS;
    desc->dwWidth = width;
    desc->dwHeight = height;
    desc->lPitch = (LONG)(width * ((bpp + 7) / 8));
    monopoly_web_dx_fill_pixel_format(&desc->ddpfPixelFormat, bpp);
}

struct IDirectDrawSurface4 {
    DDSURFACEDESC2 m_desc;
    BYTE *m_pixels;
    size_t m_pixel_size;
    MONOPOLY_WEB_GDI_BITMAP m_gdi_bitmap;
    DDCOLORKEY m_src_color_key;
    BOOL m_has_src_color_key;
    LPDIRECTDRAWPALETTE m_palette;

    void normalize_desc() {
        if (!(m_desc.dwFlags & DDSD_WIDTH) || m_desc.dwWidth == 0) m_desc.dwWidth = 800;
        if (!(m_desc.dwFlags & DDSD_HEIGHT) || m_desc.dwHeight == 0) m_desc.dwHeight = 600;
        if (!(m_desc.dwFlags & DDSD_PIXELFORMAT) || m_desc.ddpfPixelFormat.dwRGBBitCount == 0) {
            monopoly_web_dx_fill_pixel_format(&m_desc.ddpfPixelFormat, 16);
        }
        m_desc.dwFlags |= DDSD_WIDTH | DDSD_HEIGHT | DDSD_PIXELFORMAT | DDSD_CAPS | DDSD_PITCH;
        m_desc.lPitch = (LONG)(((m_desc.dwWidth * ((m_desc.ddpfPixelFormat.dwRGBBitCount + 7) / 8)) + 3) & ~3u);
    }

    void ensure_pixels() {
        normalize_desc();
        size_t needed = (size_t)m_desc.lPitch * (size_t)m_desc.dwHeight;
        if (needed == 0) needed = 1;
        if (needed != m_pixel_size) {
            BYTE *new_pixels = (BYTE *)calloc(1, needed);
            if (m_pixels) {
                size_t copy_size = needed < m_pixel_size ? needed : m_pixel_size;
                memcpy(new_pixels, m_pixels, copy_size);
                free(m_pixels);
            }
            m_pixels = new_pixels;
            m_pixel_size = needed;
        }
        m_desc.lpSurface = m_pixels;
    }

    bool is_primary() const {
        return (m_desc.ddsCaps.dwCaps & DDSCAPS_PRIMARYSURFACE) != 0;
    }

    void present_if_primary() {
        if (is_primary() && m_pixels && m_desc.ddpfPixelFormat.dwRGBBitCount == 16) {
            monopoly_web_present_rgb565(m_pixels, (int)m_desc.dwWidth, (int)m_desc.dwHeight, (int)m_desc.lPitch);
        }
    }

    void update_gdi_bitmap() {
        ensure_pixels();
        memset(&m_gdi_bitmap, 0, sizeof(m_gdi_bitmap));
        m_gdi_bitmap.magic = MONOPOLY_WEB_GDI_BITMAP_MAGIC;
        m_gdi_bitmap.width = (LONG)m_desc.dwWidth;
        m_gdi_bitmap.height = (LONG)m_desc.dwHeight;
        m_gdi_bitmap.bpp = (WORD)m_desc.ddpfPixelFormat.dwRGBBitCount;
        m_gdi_bitmap.top_down = TRUE;
        m_gdi_bitmap.owns_bits = FALSE;
        m_gdi_bitmap.stride = (size_t)m_desc.lPitch;
        m_gdi_bitmap.size = m_pixel_size;
        m_gdi_bitmap.bits = m_pixels;
    }

    IDirectDrawSurface4() : m_pixels(NULL), m_pixel_size(0), m_has_src_color_key(FALSE), m_palette(NULL) {
        memset(&m_gdi_bitmap, 0, sizeof(m_gdi_bitmap));
        memset(&m_src_color_key, 0, sizeof(m_src_color_key));
        monopoly_web_dx_fill_surface_desc(&m_desc, 800, 600, 16);
        ensure_pixels();
    }
    IDirectDrawSurface4(LPDDSURFACEDESC2 desc) : m_pixels(NULL), m_pixel_size(0), m_has_src_color_key(FALSE), m_palette(NULL) {
        memset(&m_gdi_bitmap, 0, sizeof(m_gdi_bitmap));
        memset(&m_src_color_key, 0, sizeof(m_src_color_key));
        if (desc) {
            memcpy(&m_desc, desc, sizeof(m_desc));
            m_desc.dwSize = sizeof(m_desc);
        } else {
            monopoly_web_dx_fill_surface_desc(&m_desc, 800, 600, 16);
        }
        ensure_pixels();
    }

    ULONG AddRef() { return 1; }
    ULONG Release() { return 0; }
    HRESULT QueryInterface(const GUID &, void **out) {
        if (out) *out = NULL;
        if ((m_desc.ddsCaps.dwCaps & DDSCAPS_TEXTURE) == 0) return E_FAIL;
        if (out) *out = new IDirect3DTexture2(this);
        return DD_OK;
    }
    HRESULT AddAttachedSurface(LPDIRECTDRAWSURFACE4) { return DD_OK; }
    HRESULT DeleteAttachedSurface(DWORD, LPDIRECTDRAWSURFACE4) { return DD_OK; }
    HRESULT EnumAttachedSurfaces(LPVOID, LPDDENUMSURFACESCALLBACK2) { return DD_OK; }
    HRESULT Flip(LPDIRECTDRAWSURFACE4, DWORD) { return DD_OK; }
    HRESULT GetAttachedSurface(LPDDSCAPS2, LPDIRECTDRAWSURFACE4 *out) { if (out) *out = NULL; return E_FAIL; }
    HRESULT GetBltStatus(DWORD) { return DD_OK; }
    HRESULT GetCaps(LPDDSCAPS2 caps) { if (caps) memcpy(caps, &m_desc.ddsCaps, sizeof(*caps)); return DD_OK; }
    HRESULT GetClipper(LPDIRECTDRAWCLIPPER *out) { if (out) *out = NULL; return E_FAIL; }
    HRESULT GetColorKey(DWORD flags, LPDDCOLORKEY key) {
        if (!key) return DD_OK;
        if ((flags & DDCKEY_SRCBLT) && m_has_src_color_key) {
            *key = m_src_color_key;
            return DD_OK;
        }
        memset(key, 0, sizeof(*key));
        return DDERR_NOCOLORKEY;
    }
    HRESULT GetDC(HDC *out) {
        if (!out) return DD_OK;
        update_gdi_bitmap();
        MONOPOLY_WEB_GDI_DC *dc = (MONOPOLY_WEB_GDI_DC *)monopoly_web_gdi_create_dc();
        if (!dc) {
            *out = NULL;
            return E_FAIL;
        }
        dc->bitmap = &m_gdi_bitmap;
        *out = (HDC)dc;
        return DD_OK;
    }
    HRESULT GetFlipStatus(DWORD) { return DD_OK; }
    HRESULT GetPalette(LPDIRECTDRAWPALETTE *out) {
        if (!out) return DD_OK;
        *out = m_palette;
        if (*out) {
            (*out)->AddRef();
            return DD_OK;
        }
        return E_FAIL;
    }
    HRESULT GetPixelFormat(LPDDPIXELFORMAT format) {
        if (format) {
            memcpy(format, &m_desc.ddpfPixelFormat, sizeof(*format));
        }
        return DD_OK;
    }
    HRESULT GetSurfaceDesc(LPDDSURFACEDESC2 desc) {
        ensure_pixels();
        if (desc) {
            memcpy(desc, &m_desc, sizeof(*desc));
            desc->dwSize = sizeof(*desc);
            desc->lpSurface = m_pixels;
        }
        return DD_OK;
    }
    HRESULT IsLost() { return DD_OK; }
    HRESULT Lock(LPRECT, LPDDSURFACEDESC2 desc, DWORD, HANDLE) { return GetSurfaceDesc(desc); }
    HRESULT PageLock(DWORD) { return DD_OK; }
    HRESULT PageUnlock(DWORD) { return DD_OK; }
    HRESULT Unlock(LPRECT) { present_if_primary(); return DD_OK; }
    HRESULT Restore() { return DD_OK; }
    HRESULT ReleaseDC(HDC dc) { if (dc) free(dc); return DD_OK; }
    HRESULT SetClipper(LPDIRECTDRAWCLIPPER) { return DD_OK; }
    HRESULT SetColorKey(DWORD flags, LPDDCOLORKEY key) {
        if ((flags & DDCKEY_SRCBLT) && key) {
            m_src_color_key = *key;
            m_has_src_color_key = TRUE;
        }
        return DD_OK;
    }
    HRESULT SetPalette(LPDIRECTDRAWPALETTE palette) {
        m_palette = palette;
        if (m_palette) m_palette->AddRef();
        return DD_OK;
    }
    HRESULT SetSurfaceDesc(LPDDSURFACEDESC2 desc, DWORD) { if (desc) memcpy(&m_desc, desc, sizeof(m_desc)); ensure_pixels(); return DD_OK; }
    HRESULT Blt(LPRECT destRect, LPDIRECTDRAWSURFACE4 source, LPRECT sourceRect, DWORD flags, LPDDBLTFX effects) {
        ensure_pixels();
        RECT dest = {0, 0, (LONG)m_desc.dwWidth, (LONG)m_desc.dwHeight};
        if (destRect) dest = *destRect;
        if (dest.left < 0) dest.left = 0;
        if (dest.top < 0) dest.top = 0;
        if (dest.right > (LONG)m_desc.dwWidth) dest.right = (LONG)m_desc.dwWidth;
        if (dest.bottom > (LONG)m_desc.dwHeight) dest.bottom = (LONG)m_desc.dwHeight;
        if (dest.right <= dest.left || dest.bottom <= dest.top) return DD_OK;

        int bytes_per_pixel = (int)((m_desc.ddpfPixelFormat.dwRGBBitCount + 7) / 8);
        if ((flags & DDBLT_COLORFILL) != 0) {
            DWORD color = effects ? effects->dwFillColor : 0;
            for (LONG y = dest.top; y < dest.bottom; ++y) {
                BYTE *row = m_pixels + (size_t)y * (size_t)m_desc.lPitch + (size_t)dest.left * (size_t)bytes_per_pixel;
                for (LONG x = dest.left; x < dest.right; ++x) {
                    if (bytes_per_pixel == 2) {
                        ((WORD *)row)[x - dest.left] = (WORD)color;
                    } else {
                        memcpy(row + (size_t)(x - dest.left) * (size_t)bytes_per_pixel, &color, (size_t)bytes_per_pixel);
                    }
                }
            }
            present_if_primary();
            return DD_OK;
        }

        if (source) {
            source->ensure_pixels();
            RECT original_dest = dest;
            if (destRect) original_dest = *destRect;
            RECT src = {0, 0, (LONG)source->m_desc.dwWidth, (LONG)source->m_desc.dwHeight};
            if (sourceRect) src = *sourceRect;
            if (src.left < 0) src.left = 0;
            if (src.top < 0) src.top = 0;
            if (src.right > (LONG)source->m_desc.dwWidth) src.right = (LONG)source->m_desc.dwWidth;
            if (src.bottom > (LONG)source->m_desc.dwHeight) src.bottom = (LONG)source->m_desc.dwHeight;

            LONG original_width = original_dest.right - original_dest.left;
            LONG original_height = original_dest.bottom - original_dest.top;
            LONG src_width = src.right - src.left;
            LONG src_height = src.bottom - src.top;
            LONG dest_width = dest.right - dest.left;
            LONG dest_height = dest.bottom - dest.top;
            if (original_width > 0 && original_height > 0 &&
                src_width > 0 && src_height > 0 &&
                dest_width > 0 && dest_height > 0) {
                int src_bpp = (int)((source->m_desc.ddpfPixelFormat.dwRGBBitCount + 7) / 8);
                int copy_bpp = bytes_per_pixel < src_bpp ? bytes_per_pixel : src_bpp;
                BOOL use_color_key = source->m_has_src_color_key && (flags & DDBLTFAST_SRCCOLORKEY);

                if (!use_color_key && original_width == src_width && original_height == src_height) {
                    for (LONG y = dest.top; y < dest.bottom; ++y) {
                        LONG src_y = src.top + (y - original_dest.top);
                        if (src_y < src.top || src_y >= src.bottom) continue;

                        LONG src_x = src.left + (dest.left - original_dest.left);
                        LONG copy_width = dest_width;
                        if (src_x < src.left) {
                            LONG skip = src.left - src_x;
                            src_x += skip;
                            copy_width -= skip;
                        }
                        if (src_x + copy_width > src.right) copy_width = src.right - src_x;
                        if (copy_width <= 0) continue;

                        BYTE *dst = m_pixels + (size_t)y * (size_t)m_desc.lPitch + (size_t)dest.left * (size_t)bytes_per_pixel;
                        BYTE *srcp = source->m_pixels + (size_t)src_y * (size_t)source->m_desc.lPitch + (size_t)src_x * (size_t)src_bpp;
                        memcpy(dst, srcp, (size_t)copy_width * (size_t)copy_bpp);
                    }
                } else {
                    for (LONG y = dest.top; y < dest.bottom; ++y) {
                        LONG rel_y = y - original_dest.top;
                        LONG src_y = src.top + (LONG)(((long long)rel_y * (long long)src_height) / (long long)original_height);
                        if (src_y < src.top) src_y = src.top;
                        if (src_y >= src.bottom) src_y = src.bottom - 1;

                        BYTE *dst = m_pixels + (size_t)y * (size_t)m_desc.lPitch + (size_t)dest.left * (size_t)bytes_per_pixel;
                        BYTE *src_row = source->m_pixels + (size_t)src_y * (size_t)source->m_desc.lPitch;
                        for (LONG x = dest.left; x < dest.right; ++x) {
                            LONG rel_x = x - original_dest.left;
                            LONG src_x = src.left + (LONG)(((long long)rel_x * (long long)src_width) / (long long)original_width);
                            if (src_x < src.left) src_x = src.left;
                            if (src_x >= src.right) src_x = src.right - 1;

                            BYTE *srcp = src_row + (size_t)src_x * (size_t)src_bpp;
                            if (use_color_key) {
                                DWORD pixel = 0;
                                memcpy(&pixel, srcp, (size_t)src_bpp);
                                if (pixel >= source->m_src_color_key.dwColorSpaceLowValue &&
                                    pixel <= source->m_src_color_key.dwColorSpaceHighValue) {
                                    continue;
                                }
                            }
                            memcpy(dst + (size_t)(x - dest.left) * (size_t)bytes_per_pixel,
                                   srcp,
                                   (size_t)copy_bpp);
                        }
                    }
                }
            }
        }

        present_if_primary();
        return DD_OK;
    }
    HRESULT BltFast(DWORD x, DWORD y, LPDIRECTDRAWSURFACE4 source, LPRECT sourceRect, DWORD flags) {
        RECT dest = {(LONG)x, (LONG)y, (LONG)m_desc.dwWidth, (LONG)m_desc.dwHeight};
        if (sourceRect) {
            dest.right = dest.left + (sourceRect->right - sourceRect->left);
            dest.bottom = dest.top + (sourceRect->bottom - sourceRect->top);
        }
        return Blt(&dest, source, sourceRect, flags, NULL);
    }
};

inline IDirect3DTexture2::IDirect3DTexture2(LPDIRECTDRAWSURFACE4 surface) : m_refs(1), m_surface(surface)
{
    if (m_surface) m_surface->AddRef();
}

inline ULONG IDirect3DTexture2::AddRef()
{
    return ++m_refs;
}

inline ULONG IDirect3DTexture2::Release()
{
    if (m_refs > 1) return --m_refs;
    if (m_surface) {
        m_surface->Release();
        m_surface = NULL;
    }
    delete this;
    return 0;
}

inline HRESULT IDirect3DTexture2::Load(LPDIRECT3DTEXTURE2 source)
{
    if (!m_surface || !source || !source->m_surface) return DD_OK;
    RECT rect = {0, 0, (LONG)source->m_surface->m_desc.dwWidth, (LONG)source->m_surface->m_desc.dwHeight};
    HRESULT result = m_surface->BltFast(0, 0, source->m_surface, &rect, DDBLTFAST_WAIT);
    if (source->m_surface->m_has_src_color_key) {
        m_surface->m_src_color_key = source->m_surface->m_src_color_key;
        m_surface->m_has_src_color_key = TRUE;
    }
    return result;
}

struct IDirect3DVertexBuffer {
    ULONG AddRef() { return 1; }
    ULONG Release() { return 0; }
};

struct IDirect3DViewport3 {
    D3DVIEWPORT2 m_viewport;
    IDirect3DViewport3() {
        memset(&m_viewport, 0, sizeof(m_viewport));
        m_viewport.dwSize = sizeof(m_viewport);
        m_viewport.dwX = 0;
        m_viewport.dwY = 0;
        m_viewport.dwWidth = 800;
        m_viewport.dwHeight = 600;
        m_viewport.dvMinZ = 0.0f;
        m_viewport.dvMaxZ = 1.0f;
    }
    ULONG AddRef() { return 1; }
    ULONG Release() { return 0; }
    HRESULT Clear(DWORD, LPD3DRECT, DWORD flags) { monopoly_webgl_clear(flags, 0, 1.0f); return D3D_OK; }
    HRESULT Clear2(DWORD, LPD3DRECT, DWORD flags, DWORD color, D3DVALUE depth, DWORD) { monopoly_webgl_clear(flags, color, depth); return D3D_OK; }
    HRESULT SetViewport2(LPD3DVIEWPORT2 viewport) {
        if (viewport) {
            memcpy(&m_viewport, viewport, sizeof(m_viewport));
            monopoly_webgl_set_viewport((int)m_viewport.dwX,
                                        (int)m_viewport.dwY,
                                        (int)m_viewport.dwWidth,
                                        (int)m_viewport.dwHeight);
        }
        return D3D_OK;
    }
    HRESULT GetViewport2(LPD3DVIEWPORT2 viewport) { if (viewport) memcpy(viewport, &m_viewport, sizeof(m_viewport)); return D3D_OK; }
    HRESULT SetBackground(D3DMATERIALHANDLE) { return D3D_OK; }
    HRESULT AddLight(LPDIRECT3DLIGHT) { return D3D_OK; }
    HRESULT DeleteLight(LPDIRECT3DLIGHT) { return D3D_OK; }
    HRESULT TransformVertices(DWORD count, LPD3DTRANSFORMDATA data, DWORD, DWORD *offscreen) {
        if (offscreen) *offscreen = FALSE;
        if (!data || !data->lpIn || !data->lpOut) return D3D_OK;
        LONG minX = 800, minY = 600, maxX = 0, maxY = 0;
        for (DWORD i = 0; i < count; ++i) {
            const BYTE *inBase = (const BYTE *)data->lpIn + i * data->dwInSize;
            BYTE *outBase = (BYTE *)data->lpOut + i * data->dwOutSize;
            const D3DVECTOR *in = (const D3DVECTOR *)inBase;
            D3DTLVERTEX *out = (D3DTLVERTEX *)outBase;
            out->sx = 400.0f + in->x;
            out->sy = 300.0f - in->y;
            out->sz = in->z;
            out->rhw = 1.0f;
            out->color = 0xffffffffu;
            out->specular = 0;
            out->tu = 0.0f;
            out->tv = 0.0f;
            if (data->lpHOut) {
                data->lpHOut[i].dwFlags = 0;
                data->lpHOut[i].hx = out->sx;
                data->lpHOut[i].hy = out->sy;
                data->lpHOut[i].hz = out->sz;
            }
            LONG sx = (LONG)out->sx;
            LONG sy = (LONG)out->sy;
            if (sx < minX) minX = sx;
            if (sy < minY) minY = sy;
            if (sx > maxX) maxX = sx;
            if (sy > maxY) maxY = sy;
        }
        data->drExtent.x1 = minX;
        data->drExtent.y1 = minY;
        data->drExtent.x2 = maxX;
        data->drExtent.y2 = maxY;
        return D3D_OK;
    }
};

struct IDirect3DLight {
    ULONG AddRef() { return 1; }
    ULONG Release() { return 0; }
    HRESULT SetLight(LPD3DLIGHT2) { return D3D_OK; }
    HRESULT GetLight(LPD3DLIGHT2) { return D3D_OK; }
};

struct IDirect3DMaterial3 {
    ULONG AddRef() { return 1; }
    ULONG Release() { return 0; }
    HRESULT SetMaterial(LPD3DMATERIAL) { return D3D_OK; }
    HRESULT GetMaterial(LPD3DMATERIAL) { return D3D_OK; }
    HRESULT GetHandle(LPDIRECT3DDEVICE3, D3DMATERIALHANDLE *handle) { if (handle) *handle = 0; return D3D_OK; }
};

struct IDirect3DDevice3 {
    D3DMATRIX m_world;
    D3DMATRIX m_view;
    D3DMATRIX m_projection;
    LPDIRECTDRAWSURFACE4 m_render_target;
    LPDIRECT3DTEXTURE2 m_texture0;
    DWORD m_render_states[128];

    IDirect3DDevice3(LPDIRECTDRAWSURFACE4 renderTarget = NULL) : m_render_target(renderTarget), m_texture0(NULL) {
        monopoly_web_dx_identity_matrix(&m_world);
        monopoly_web_dx_identity_matrix(&m_view);
        monopoly_web_dx_identity_matrix(&m_projection);
        memset(m_render_states, 0, sizeof(m_render_states));
        m_render_states[D3DRENDERSTATE_ZENABLE] = TRUE;
        m_render_states[D3DRENDERSTATE_ZWRITEENABLE] = TRUE;
        m_render_states[D3DRENDERSTATE_ALPHABLENDENABLE] = FALSE;
        m_render_states[D3DRENDERSTATE_SRCBLEND] = D3DBLEND_SRCALPHA;
        m_render_states[D3DRENDERSTATE_DESTBLEND] = D3DBLEND_INVSRCALPHA;
        m_render_states[D3DRENDERSTATE_ZBIAS] = 0;
        m_render_states[D3DRENDERSTATE_COLORKEYENABLE] = FALSE;
        if (m_render_target) m_render_target->AddRef();
    }

    ULONG AddRef() { return 1; }
    ULONG Release() {
        if (m_texture0) { m_texture0->Release(); m_texture0 = NULL; }
        if (m_render_target) { m_render_target->Release(); m_render_target = NULL; }
        return 0;
    }
    HRESULT AddViewport(LPDIRECT3DVIEWPORT3) { return D3D_OK; }
    HRESULT DeleteViewport(LPDIRECT3DVIEWPORT3) { return D3D_OK; }
    HRESULT BeginScene() {
        if (m_render_target) {
            m_render_target->ensure_pixels();
            if (m_render_target->m_desc.ddpfPixelFormat.dwRGBBitCount == 16 && m_render_target->m_pixels) {
                monopoly_webgl_begin_frame_from_rgb565(m_render_target->m_pixels,
                                                       (int)m_render_target->m_desc.dwWidth,
                                                       (int)m_render_target->m_desc.dwHeight,
                                                       (int)m_render_target->m_desc.lPitch);
                return D3D_OK;
            }
        }
        monopoly_webgl_begin_frame();
        return D3D_OK;
    }
    HRESULT EndScene() {
        if (m_render_target) {
            m_render_target->ensure_pixels();
            if (m_render_target->m_desc.ddpfPixelFormat.dwRGBBitCount == 16 && m_render_target->m_pixels) {
                monopoly_webgl_read_rgb565(m_render_target->m_pixels,
                                           (int)m_render_target->m_desc.dwWidth,
                                           (int)m_render_target->m_desc.dwHeight,
                                           (int)m_render_target->m_desc.lPitch);
            }
        }
        return D3D_OK;
    }
    HRESULT DrawIndexedPrimitive(D3DPRIMITIVETYPE primitiveType, DWORD vertexType, LPVOID vertices, DWORD vertexCount, LPWORD indices, DWORD indexCount, DWORD) {
        monopoly_webgl_draw_primitive(primitiveType, vertexType, vertices, vertexCount, indices, indexCount, NULL, &m_world, &m_view, &m_projection);
        return D3D_OK;
    }
    HRESULT DrawIndexedPrimitiveStrided(D3DPRIMITIVETYPE primitiveType, DWORD vertexType, PD3DSTRIDEDATA vertexArray, DWORD vertexCount, LPWORD indices, DWORD indexCount, DWORD) {
        monopoly_webgl_draw_primitive(primitiveType, vertexType, NULL, vertexCount, indices, indexCount, vertexArray, &m_world, &m_view, &m_projection);
        return D3D_OK;
    }
    HRESULT DrawIndexedPrimitiveVB(D3DPRIMITIVETYPE, LPDIRECT3DVERTEXBUFFER, LPWORD, DWORD, DWORD) { return D3D_OK; }
    HRESULT DrawPrimitive(D3DPRIMITIVETYPE primitiveType, DWORD vertexType, LPVOID vertices, DWORD vertexCount, DWORD) {
        monopoly_webgl_draw_primitive(primitiveType, vertexType, vertices, vertexCount, NULL, 0, NULL, &m_world, &m_view, &m_projection);
        return D3D_OK;
    }
    HRESULT DrawPrimitiveStrided(D3DPRIMITIVETYPE primitiveType, DWORD vertexType, PD3DSTRIDEDATA vertexArray, DWORD vertexCount, DWORD) {
        monopoly_webgl_draw_primitive(primitiveType, vertexType, NULL, vertexCount, NULL, 0, vertexArray, &m_world, &m_view, &m_projection);
        return D3D_OK;
    }
    HRESULT DrawPrimitiveVB(D3DPRIMITIVETYPE, LPDIRECT3DVERTEXBUFFER, DWORD, DWORD, DWORD) { return D3D_OK; }
    HRESULT GetClipStatus(LPD3DCLIPSTATUS status) { if (status) memset(status, 0, sizeof(*status)); return D3D_OK; }
    HRESULT GetCurrentViewport(LPDIRECT3DVIEWPORT3 *out) { if (out) *out = NULL; return D3D_OK; }
    HRESULT NextViewport(LPDIRECT3DVIEWPORT3, LPDIRECT3DVIEWPORT3 *out, DWORD) { if (out) *out = NULL; return D3D_OK; }
    HRESULT SetCurrentViewport(LPDIRECT3DVIEWPORT3) { return D3D_OK; }
    HRESULT GetRenderTarget(LPDIRECTDRAWSURFACE4 *out) { if (out) *out = m_render_target; return D3D_OK; }
    HRESULT SetRenderTarget(LPDIRECTDRAWSURFACE4 target, DWORD) {
        if (m_render_target) m_render_target->Release();
        m_render_target = target;
        if (m_render_target) m_render_target->AddRef();
        return D3D_OK;
    }
    HRESULT GetTexture(DWORD stage, LPDIRECT3DTEXTURE2 *out) {
        if (!out) return D3D_OK;
        *out = stage == 0 ? m_texture0 : NULL;
        if (*out) (*out)->AddRef();
        return D3D_OK;
    }
    HRESULT SetTexture(DWORD stage, LPDIRECT3DTEXTURE2 texture) {
        if (stage != 0) return D3D_OK;
        if (texture) texture->AddRef();
        if (m_texture0) m_texture0->Release();
        m_texture0 = texture;
        if (m_texture0 && m_texture0->m_surface) {
            m_texture0->m_surface->ensure_pixels();
            monopoly_webgl_set_texture(m_texture0->m_surface->m_pixels,
                                       (int)m_texture0->m_surface->m_desc.dwWidth,
                                       (int)m_texture0->m_surface->m_desc.dwHeight,
                                       (int)m_texture0->m_surface->m_desc.lPitch,
                                       (int)m_texture0->m_surface->m_desc.ddpfPixelFormat.dwRGBBitCount,
                                       &m_texture0->m_surface->m_desc.ddpfPixelFormat,
                                       m_texture0->m_surface->m_palette ? m_texture0->m_surface->m_palette->m_entries : NULL,
                                       m_texture0->m_surface->m_palette ? (int)m_texture0->m_surface->m_palette->m_count : 0,
                                       m_texture0->m_surface->m_has_src_color_key,
                                       m_texture0->m_surface->m_src_color_key.dwColorSpaceLowValue,
                                       m_texture0->m_surface->m_src_color_key.dwColorSpaceHighValue);
        } else {
            monopoly_webgl_set_texture(NULL, 0, 0, 0, 0, NULL, NULL, 0, 0, 0, 0);
        }
        return D3D_OK;
    }
    HRESULT GetLightState(D3DLIGHTSTATETYPE, DWORD *value) { if (value) *value = 0; return D3D_OK; }
    HRESULT SetLightState(D3DLIGHTSTATETYPE, DWORD) { return D3D_OK; }
    HRESULT GetRenderState(D3DRENDERSTATETYPE state, DWORD *value) {
        if (value) *value = ((DWORD)state < 128u) ? m_render_states[state] : 0;
        return D3D_OK;
    }
    HRESULT SetRenderState(D3DRENDERSTATETYPE state, DWORD value) {
        if ((DWORD)state < 128u) m_render_states[state] = value;
        monopoly_webgl_set_render_state((DWORD)state, value);
        return D3D_OK;
    }
    HRESULT GetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD *value) { if (value) *value = 0; return D3D_OK; }
    HRESULT SetTextureStageState(DWORD, D3DTEXTURESTAGESTATETYPE, DWORD) { return D3D_OK; }
    HRESULT GetTransform(D3DTRANSFORMSTATETYPE state, LPD3DMATRIX matrix) {
        if (!matrix) return D3D_OK;
        switch (state) {
            case D3DTRANSFORMSTATE_WORLD: memcpy(matrix, &m_world, sizeof(*matrix)); break;
            case D3DTRANSFORMSTATE_VIEW: memcpy(matrix, &m_view, sizeof(*matrix)); break;
            case D3DTRANSFORMSTATE_PROJECTION: memcpy(matrix, &m_projection, sizeof(*matrix)); break;
            default: monopoly_web_dx_identity_matrix(matrix); break;
        }
        return D3D_OK;
    }
    HRESULT SetTransform(D3DTRANSFORMSTATETYPE state, LPD3DMATRIX matrix) {
        D3DMATRIX replacement;
        if (!matrix) {
            monopoly_web_dx_identity_matrix(&replacement);
            matrix = &replacement;
        }
        switch (state) {
            case D3DTRANSFORMSTATE_WORLD: memcpy(&m_world, matrix, sizeof(m_world)); break;
            case D3DTRANSFORMSTATE_VIEW: memcpy(&m_view, matrix, sizeof(m_view)); break;
            case D3DTRANSFORMSTATE_PROJECTION: memcpy(&m_projection, matrix, sizeof(m_projection)); break;
            default: break;
        }
        return D3D_OK;
    }
    HRESULT MultiplyTransform(D3DTRANSFORMSTATETYPE, LPD3DMATRIX) { return D3D_OK; }
    HRESULT SetClipStatus(LPD3DCLIPSTATUS) { return D3D_OK; }
    HRESULT EnumTextureFormats(LPD3DENUMPIXELFORMATSCALLBACK callback, LPVOID context) {
        if (callback) {
            DDPIXELFORMAT format;
            memset(&format, 0, sizeof(format));
            format.dwSize = sizeof(format);
            format.dwFlags = DDPF_ALPHAPIXELS | DDPF_RGB;
            format.dwRGBBitCount = 16;
            format.dwRGBAlphaBitMask = 0x0000f000;
            format.dwRBitMask = 0x00000f00;
            format.dwGBitMask = 0x000000f0;
            format.dwBBitMask = 0x0000000f;
            callback(&format, context);
            memset(&format, 0, sizeof(format));
            format.dwSize = sizeof(format);
            format.dwFlags = DDPF_RGB;
            format.dwRGBBitCount = 16;
            format.dwRBitMask = 0x0000f800;
            format.dwGBitMask = 0x000007e0;
            format.dwBBitMask = 0x0000001f;
            callback(&format, context);
            memset(&format, 0, sizeof(format));
            format.dwSize = sizeof(format);
            format.dwFlags = DDPF_ALPHAPIXELS | DDPF_RGB;
            format.dwRGBBitCount = 32;
            format.dwRBitMask = 0x00ff0000;
            format.dwGBitMask = 0x0000ff00;
            format.dwBBitMask = 0x000000ff;
            format.dwRGBAlphaBitMask = 0xff000000;
            callback(&format, context);
        }
        return D3D_OK;
    }
};

struct IDirect3D3 {
    ULONG AddRef() { return 1; }
    ULONG Release() { return 0; }
    HRESULT EvictManagedTextures() { return D3D_OK; }
    HRESULT EnumDevices(LPD3DENUMDEVICESCALLBACK callback, LPVOID context) {
        if (callback) {
            GUID guid = GUID_NULL;
            D3DDEVICEDESC hw;
            D3DDEVICEDESC sw;
            memset(&hw, 0, sizeof(hw));
            memset(&sw, 0, sizeof(sw));
            hw.dwSize = sizeof(hw);
            sw.dwSize = sizeof(sw);
            hw.dcmColorModel = D3DCOLOR_RGB;
            hw.dwDeviceRenderBitDepth = DDBD_16 | DDBD_24 | DDBD_32;
            hw.dwDeviceZBufferBitDepth = DDBD_16 | DDBD_24 | DDBD_32;
            sw.dwDeviceRenderBitDepth = DDBD_16 | DDBD_24 | DDBD_32;
            sw.dwDeviceZBufferBitDepth = DDBD_16 | DDBD_24 | DDBD_32;
            callback(&guid, (LPSTR)"WebGL2", (LPSTR)"WebGL2", &hw, &sw, context);
        }
        return D3D_OK;
    }
    HRESULT CreateDevice(GUID &, LPDIRECTDRAWSURFACE4 renderTarget, LPDIRECT3DDEVICE3 *out, void *) {
        if (out) *out = new IDirect3DDevice3(renderTarget);
        return D3D_OK;
    }
    HRESULT CreateLight(LPDIRECT3DLIGHT *out, void *) { if (out) *out = new IDirect3DLight(); return D3D_OK; }
    HRESULT CreateMaterial(LPDIRECT3DMATERIAL3 *out, void *) { if (out) *out = new IDirect3DMaterial3(); return D3D_OK; }
    HRESULT CreateViewport(LPDIRECT3DVIEWPORT3 *out, void *) { if (out) *out = new IDirect3DViewport3(); return D3D_OK; }
    HRESULT CreateVertexBuffer(LPD3DVERTEXBUFFERDESC, LPDIRECT3DVERTEXBUFFER *out, DWORD, void *) { if (out) *out = new IDirect3DVertexBuffer(); return D3D_OK; }
    HRESULT EnumZBufferFormats(GUID &, LPD3DENUMPIXELFORMATSCALLBACK callback, LPVOID context) {
        if (callback) {
            DDPIXELFORMAT format;
            memset(&format, 0, sizeof(format));
            format.dwSize = sizeof(format);
            format.dwRGBBitCount = 16;
            format.dwZBufferBitDepth = 16;
            callback(&format, context);
        }
        return D3D_OK;
    }
};

struct IDirectDraw4 {
    DWORD m_width;
    DWORD m_height;
    DWORD m_bpp;

    IDirectDraw4() : m_width(800), m_height(600), m_bpp(16) {}

    ULONG AddRef() { return 1; }
    ULONG Release() { return 0; }
    HRESULT QueryInterface(const GUID &, void **out) { if (out) *out = new IDirect3D3(); return D3D_OK; }
    HRESULT GetDeviceIdentifier(LPDDDEVICEIDENTIFIER id, DWORD) { if (id) memset(id, 0, sizeof(*id)); return DD_OK; }
    HRESULT GetDisplayMode(LPDDSURFACEDESC2 desc) { monopoly_web_dx_fill_surface_desc(desc, m_width, m_height, m_bpp); return DD_OK; }
    HRESULT GetAvailableVidMem(LPDDSCAPS2, LPDWORD total, LPDWORD freeMem) { if (total) *total = 64 * 1024 * 1024; if (freeMem) *freeMem = 64 * 1024 * 1024; return DD_OK; }
    HRESULT FlipToGDISurface() { return DD_OK; }
    HRESULT SetCooperativeLevel(HWND, DWORD) { return DD_OK; }
    HRESULT GetSurfaceFromDC(HDC, LPDIRECTDRAWSURFACE4 *out) { if (out) *out = NULL; return E_FAIL; }
    HRESULT RestoreDisplayMode() { return DD_OK; }
    HRESULT RestoreAllSurfaces() { return DD_OK; }
    HRESULT GetCaps(LPDDCAPS hw, LPDDCAPS sw) { if (hw) { memset(hw, 0, sizeof(*hw)); hw->dwCaps = DDCAPS_BLT | DDCAPS_BLTSTRETCH; hw->dwCaps2 = DDCAPS2_CANRENDERWINDOWED; hw->dwSVBCaps = DDCAPS_BLT; } if (sw) memset(sw, 0, sizeof(*sw)); return DD_OK; }
    HRESULT EnumDisplayModes(DWORD, LPDDSURFACEDESC2, LPVOID context, LPDDENUMMODESCALLBACK2 callback) {
        if (callback) {
            const DWORD modes[][3] = {
                {800, 600, 16},
                {800, 600, 32},
                {640, 480, 16},
                {640, 480, 32}
            };
            for (int i = 0; i < 4; ++i) {
                DDSURFACEDESC2 desc;
                monopoly_web_dx_fill_surface_desc(&desc, modes[i][0], modes[i][1], modes[i][2]);
                callback(&desc, context);
            }
        }
        return DD_OK;
    }
    HRESULT CreateSurface(LPDDSURFACEDESC2 desc, LPDIRECTDRAWSURFACE4 *out, void *) {
        DDSURFACEDESC2 fallback;
        if (!desc) {
            monopoly_web_dx_fill_surface_desc(&fallback, m_width, m_height, m_bpp);
            desc = &fallback;
        } else if (!(desc->dwFlags & DDSD_PIXELFORMAT) || desc->ddpfPixelFormat.dwRGBBitCount == 0) {
            monopoly_web_dx_fill_pixel_format(&desc->ddpfPixelFormat, m_bpp);
            desc->dwFlags |= DDSD_PIXELFORMAT;
        }
        if (out) *out = new IDirectDrawSurface4(desc);
        return DD_OK;
    }
    HRESULT CreatePalette(DWORD, LPPALETTEENTRY entries, LPDIRECTDRAWPALETTE *out, void *) { if (out) *out = new IDirectDrawPalette(entries); return DD_OK; }
    HRESULT CreateClipper(DWORD, LPDIRECTDRAWCLIPPER *out, void *) { if (out) *out = new IDirectDrawClipper(); return DD_OK; }
    HRESULT SetDisplayMode(DWORD width, DWORD height, DWORD bpp, DWORD, DWORD) { m_width = width; m_height = height; m_bpp = bpp; return DD_OK; }
};

struct IDirectDraw {
    ULONG AddRef() { return 1; }
    ULONG Release() { return 0; }
    HRESULT QueryInterface(const GUID &, void **out) { if (out) *out = new IDirectDraw4(); return DD_OK; }
    HRESULT CreateSurface(LPDDSURFACEDESC, LPDIRECTDRAWSURFACE *out, void *) { if (out) *out = reinterpret_cast<LPDIRECTDRAWSURFACE>(new IDirectDrawSurface4()); return DD_OK; }
};

static const GUID IID_IDirectDraw = {0};
static const GUID IID_IDirectDraw2 = {0};
static const GUID IID_IDirectDraw4 = {0};
static const GUID IID_IDirect3D = {0};
static const GUID IID_IDirect3D2 = {0};
static const GUID IID_IDirect3D3 = {0};
static const GUID IID_IDirect3DTexture = {0};
static const GUID IID_IDirect3DTexture2 = {0};

static inline HRESULT DirectDrawCreate(void *, LPDIRECTDRAW *out, void *) { if (out) *out = new IDirectDraw(); return DD_OK; }
static inline HRESULT DirectDrawCreateClipper(DWORD, LPDIRECTDRAWCLIPPER *out, void *) { if (out) *out = new IDirectDrawClipper(); return DD_OK; }
static inline HRESULT DirectDrawEnumerateExA(LPDDENUMCALLBACKEXA callback, LPVOID context, DWORD)
{
    if (callback) {
        GUID guid = {0};
        char driverName[] = "webgl";
        char driverDescription[] = "Monopoly WebGL display";
        callback(&guid, driverDescription, driverName, context, NULL);
    }
    return DD_OK;
}
#define DirectDrawEnumerateEx DirectDrawEnumerateExA

#endif
