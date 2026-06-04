#include "../artlib/l_inc.h"
#include "../artlib/L_Blt.h"

static inline int web_min4(INT32 a, INT32 b, INT32 c, INT32 d)
{
    int m = (int)a;
    if (b < m) m = (int)b;
    if (c < m) m = (int)c;
    if (d < m) m = (int)d;
    return m;
}

static inline int web_max4(INT32 a, INT32 b, INT32 c, INT32 d)
{
    int m = (int)a;
    if (b > m) m = (int)b;
    if (c > m) m = (int)c;
    if (d > m) m = (int)d;
    return m;
}

static inline DWORD web_palette_colour(LE_BLT_AlphaPaletteEntryRecord *palette, UNS8 index)
{
    return palette ? palette[index].lowDWORD.dwColour : 0;
}

static inline bool web_palette_visible(LE_BLT_AlphaPaletteEntryRecord *palette, UNS8 index, UNS16 numAlpha)
{
    return !palette || numAlpha == 0 || palette[index].dwAlpha != 0;
}

static inline UNS16 web_rgb_to_565(DWORD colour)
{
    UNS8 blue = (UNS8)(colour & 0xff);
    UNS8 green = (UNS8)((colour >> 8) & 0xff);
    UNS8 red = (UNS8)((colour >> 16) & 0xff);
    return (UNS16)(((red >> 3) << 11) | ((green >> 2) << 5) | (blue >> 3));
}

static void web_bounds(INT32 x0, INT32 y0, INT32 x1, INT32 y1, INT32 x2, INT32 y2, INT32 x3, INT32 y3,
    UNS16 destWidth, UNS16 destHeight, int *left, int *top, int *right, int *bottom)
{
    *left = web_min4(x0, x1, x2, x3);
    *top = web_min4(y0, y1, y2, y3);
    *right = web_max4(x0, x1, x2, x3);
    *bottom = web_max4(y0, y1, y2, y3);
    if (*left < 0) *left = 0;
    if (*top < 0) *top = 0;
    if (*right > destWidth) *right = destWidth;
    if (*bottom > destHeight) *bottom = destHeight;
}

template <typename WritePixel>
static void web_warp8(void *srcPixels, UNS16 srcPitch, UNS16 srcWidth, UNS16 srcHeight,
    LE_BLT_AlphaPaletteEntryRecord *srcPalette, UNS16 numAlpha,
    void *destPixels, UNS16 destPitch, UNS16 destWidth, UNS16 destHeight,
    INT32 x0, INT32 y0, INT32 x1, INT32 y1, INT32 x2, INT32 y2, INT32 x3, INT32 y3,
    WritePixel writePixel)
{
    if (!srcPixels || !destPixels || srcWidth == 0 || srcHeight == 0) return;
    int left, top, right, bottom;
    web_bounds(x0, y0, x1, y1, x2, y2, x3, y3, destWidth, destHeight, &left, &top, &right, &bottom);
    int width = right - left;
    int height = bottom - top;
    if (width <= 0 || height <= 0) return;
    UNS8 *src = (UNS8 *)srcPixels;
    UNS8 *dest = (UNS8 *)destPixels;
    for (int y = top; y < bottom; ++y) {
        int sy = (int)(((int64_t)(y - top) * srcHeight) / height);
        if (sy >= srcHeight) sy = srcHeight - 1;
        for (int x = left; x < right; ++x) {
            int sx = (int)(((int64_t)(x - left) * srcWidth) / width);
            if (sx >= srcWidth) sx = srcWidth - 1;
            UNS8 index = src[sy * srcPitch + sx];
            if (!web_palette_visible(srcPalette, index, numAlpha)) continue;
            writePixel(dest + y * destPitch, x, web_palette_colour(srcPalette, index));
        }
    }
}

template <int BytesPerPixel>
static void web_warp_native(void *srcPixels, UNS16 srcPitch, UNS16 srcWidth, UNS16 srcHeight,
    void *destPixels, UNS16 destPitch, UNS16 destWidth, UNS16 destHeight,
    INT32 x0, INT32 y0, INT32 x1, INT32 y1, INT32 x2, INT32 y2, INT32 x3, INT32 y3)
{
    if (!srcPixels || !destPixels || srcWidth == 0 || srcHeight == 0) return;
    int left, top, right, bottom;
    web_bounds(x0, y0, x1, y1, x2, y2, x3, y3, destWidth, destHeight, &left, &top, &right, &bottom);
    int width = right - left;
    int height = bottom - top;
    if (width <= 0 || height <= 0) return;
    UNS8 *src = (UNS8 *)srcPixels;
    UNS8 *dest = (UNS8 *)destPixels;
    for (int y = top; y < bottom; ++y) {
        int sy = (int)(((int64_t)(y - top) * srcHeight) / height);
        if (sy >= srcHeight) sy = srcHeight - 1;
        for (int x = left; x < right; ++x) {
            int sx = (int)(((int64_t)(x - left) * srcWidth) / width);
            if (sx >= srcWidth) sx = srcWidth - 1;
            memcpy(dest + y * destPitch + x * BytesPerPixel, src + sy * srcPitch + sx * BytesPerPixel, BytesPerPixel);
        }
    }
}

void LE_BLTWARP_8to16(void *src, UNS16 srcPitch, UNS16 srcWidth, UNS16 srcHeight,
    LE_BLT_AlphaPaletteEntryRecord *palette, UNS16, UNS16 numAlpha,
    void *dest, UNS16 destPitch, UNS16 destWidth, UNS16 destHeight,
    INT32 x0, INT32 y0, INT32 x1, INT32 y1, INT32 x2, INT32 y2, INT32 x3, INT32 y3)
{
    web_warp8(src, srcPitch, srcWidth, srcHeight, palette, numAlpha, dest, destPitch, destWidth, destHeight,
        x0, y0, x1, y1, x2, y2, x3, y3,
        [](UNS8 *row, int x, DWORD colour) { ((UNS16 *)row)[x] = web_rgb_to_565(colour); });
}

void LE_BLTWARP_8to24(void *src, UNS16 srcPitch, UNS16 srcWidth, UNS16 srcHeight,
    LE_BLT_AlphaPaletteEntryRecord *palette, UNS16, UNS16 numAlpha,
    void *dest, UNS16 destPitch, UNS16 destWidth, UNS16 destHeight,
    INT32 x0, INT32 y0, INT32 x1, INT32 y1, INT32 x2, INT32 y2, INT32 x3, INT32 y3)
{
    web_warp8(src, srcPitch, srcWidth, srcHeight, palette, numAlpha, dest, destPitch, destWidth, destHeight,
        x0, y0, x1, y1, x2, y2, x3, y3,
        [](UNS8 *row, int x, DWORD colour) {
            UNS8 *pixel = row + x * 3;
            pixel[0] = (UNS8)(colour & 0xff);
            pixel[1] = (UNS8)((colour >> 8) & 0xff);
            pixel[2] = (UNS8)((colour >> 16) & 0xff);
        });
}

void LE_BLTWARP_8to32(void *src, UNS16 srcPitch, UNS16 srcWidth, UNS16 srcHeight,
    LE_BLT_AlphaPaletteEntryRecord *palette, UNS16, UNS16 numAlpha,
    void *dest, UNS16 destPitch, UNS16 destWidth, UNS16 destHeight,
    INT32 x0, INT32 y0, INT32 x1, INT32 y1, INT32 x2, INT32 y2, INT32 x3, INT32 y3)
{
    web_warp8(src, srcPitch, srcWidth, srcHeight, palette, numAlpha, dest, destPitch, destWidth, destHeight,
        x0, y0, x1, y1, x2, y2, x3, y3,
        [](UNS8 *row, int x, DWORD colour) { ((DWORD *)row)[x] = colour; });
}

void LE_BLTWARP_32to32(void *src, UNS16 srcPitch, UNS16 srcWidth, UNS16 srcHeight,
    void *dest, UNS16 destPitch, UNS16 destWidth, UNS16 destHeight,
    INT32 x0, INT32 y0, INT32 x1, INT32 y1, INT32 x2, INT32 y2, INT32 x3, INT32 y3)
{
    web_warp_native<4>(src, srcPitch, srcWidth, srcHeight, dest, destPitch, destWidth, destHeight,
        x0, y0, x1, y1, x2, y2, x3, y3);
}

void LE_BLTWARP_16to16(void *src, UNS16 srcPitch, UNS16 srcWidth, UNS16 srcHeight,
    void *dest, UNS16 destPitch, UNS16 destWidth, UNS16 destHeight,
    INT32 x0, INT32 y0, INT32 x1, INT32 y1, INT32 x2, INT32 y2, INT32 x3, INT32 y3)
{
    web_warp_native<2>(src, srcPitch, srcWidth, srcHeight, dest, destPitch, destWidth, destHeight,
        x0, y0, x1, y1, x2, y2, x3, y3);
}

void LE_BLTWARP_24to24(void *src, UNS16 srcPitch, UNS16 srcWidth, UNS16 srcHeight,
    void *dest, UNS16 destPitch, UNS16 destWidth, UNS16 destHeight,
    INT32 x0, INT32 y0, INT32 x1, INT32 y1, INT32 x2, INT32 y2, INT32 x3, INT32 y3)
{
    web_warp_native<3>(src, srcPitch, srcWidth, srcHeight, dest, destPitch, destWidth, destHeight,
        x0, y0, x1, y1, x2, y2, x3, y3);
}
