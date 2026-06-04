#include <emscripten/html5.h>
#include <GLES3/gl3.h>

#include "compat/web_dx_compat.h"

#include <cstdio>
#include <cstdlib>
#include <math.h>

extern "C" void monopoly_web_log(const char *message);

static EMSCRIPTEN_WEBGL_CONTEXT_HANDLE g_context = 0;
static GLuint g_program = 0;
static GLuint g_vao = 0;
static GLuint g_vbo = 0;
static GLuint g_ibo = 0;
static GLuint g_fbo = 0;
static GLuint g_color_texture = 0;
static GLuint g_depth_buffer = 0;
static GLuint g_active_texture = 0;
static int g_framebuffer_width = 0;
static int g_framebuffer_height = 0;
static bool g_frame_started = false;
static bool g_scene_has_draw = false;
static bool g_logged_skipped_readback = false;
static bool g_has_active_texture = false;
static bool g_active_texture_discard_zero_alpha = false;
static bool g_logged_first_texture = false;
static int g_viewport_x = 0;
static int g_viewport_y = 0;
static int g_viewport_width = 800;
static int g_viewport_height = 600;
static unsigned g_draw_count = 0;
static unsigned g_readback_count = 0;
static bool g_depth_enabled = true;
static bool g_depth_write_enabled = true;
static bool g_alpha_blend_enabled = false;
static DWORD g_src_blend = D3DBLEND_SRCALPHA;
static DWORD g_dest_blend = D3DBLEND_INVSRCALPHA;
static DWORD g_z_bias = 0;
static bool g_color_key_enabled = false;

struct WebVertex {
    float x;
    float y;
    float z;
    float r;
    float g;
    float b;
    float a;
    float u;
    float v;
    bool valid;
};

static const D3DMATRIX k_identity_matrix = D3DMATRIX(
    1.0f, 0.0f, 0.0f, 0.0f,
    0.0f, 1.0f, 0.0f, 0.0f,
    0.0f, 0.0f, 1.0f, 0.0f,
    0.0f, 0.0f, 0.0f, 1.0f);

static bool make_current()
{
    return g_context && emscripten_webgl_make_context_current(g_context) == EMSCRIPTEN_RESULT_SUCCESS;
}

static GLuint compile_shader(GLenum type, const char *source)
{
    GLuint shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, nullptr);
    glCompileShader(shader);
    GLint ok = GL_FALSE;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &ok);
    if (!ok) {
        char log[512];
        GLsizei len = 0;
        glGetShaderInfoLog(shader, sizeof(log), &len, log);
        monopoly_web_log(log);
        glDeleteShader(shader);
        return 0;
    }
    return shader;
}

static bool ensure_program()
{
    if (g_program) return true;
    if (!make_current()) return false;

    static const char *vertex_shader =
        "#version 300 es\n"
        "layout(location=0) in vec3 a_position;\n"
        "layout(location=1) in vec4 a_color;\n"
        "layout(location=2) in vec2 a_texcoord;\n"
        "out vec4 v_color;\n"
        "out vec2 v_texcoord;\n"
        "void main() {\n"
        "  gl_Position = vec4(a_position, 1.0);\n"
        "  v_color = a_color;\n"
        "  v_texcoord = a_texcoord;\n"
        "}\n";
    static const char *fragment_shader =
        "#version 300 es\n"
        "precision mediump float;\n"
        "in vec4 v_color;\n"
        "in vec2 v_texcoord;\n"
        "uniform sampler2D u_texture;\n"
        "uniform int u_has_texture;\n"
        "uniform int u_discard_zero_alpha;\n"
        "out vec4 out_color;\n"
        "void main() {\n"
        "  vec4 texel = u_has_texture != 0 ? texture(u_texture, v_texcoord) : vec4(1.0);\n"
        "  if (u_has_texture != 0 && u_discard_zero_alpha != 0 && texel.a <= 0.0039) discard;\n"
        "  out_color = u_has_texture != 0 ? vec4(texel.rgb, texel.a * v_color.a) : v_color;\n"
        "}\n";

    GLuint vs = compile_shader(GL_VERTEX_SHADER, vertex_shader);
    GLuint fs = compile_shader(GL_FRAGMENT_SHADER, fragment_shader);
    if (!vs || !fs) return false;

    g_program = glCreateProgram();
    glAttachShader(g_program, vs);
    glAttachShader(g_program, fs);
    glLinkProgram(g_program);
    glDeleteShader(vs);
    glDeleteShader(fs);

    GLint ok = GL_FALSE;
    glGetProgramiv(g_program, GL_LINK_STATUS, &ok);
    if (!ok) {
        char log[512];
        GLsizei len = 0;
        glGetProgramInfoLog(g_program, sizeof(log), &len, log);
        monopoly_web_log(log);
        glDeleteProgram(g_program);
        g_program = 0;
        return false;
    }

    glGenVertexArrays(1, &g_vao);
    glGenBuffers(1, &g_vbo);
    glGenBuffers(1, &g_ibo);
    glBindVertexArray(g_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(WebVertex), (const void *)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, sizeof(WebVertex), (const void *)(3 * sizeof(float)));
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(WebVertex), (const void *)(7 * sizeof(float)));
    glBindVertexArray(0);
    return true;
}

static void destroy_framebuffer()
{
    if (g_depth_buffer) glDeleteRenderbuffers(1, &g_depth_buffer);
    if (g_color_texture) glDeleteTextures(1, &g_color_texture);
    if (g_fbo) glDeleteFramebuffers(1, &g_fbo);
    g_depth_buffer = 0;
    g_color_texture = 0;
    g_fbo = 0;
    g_framebuffer_width = 0;
    g_framebuffer_height = 0;
}

static bool ensure_framebuffer(int width, int height)
{
    if (!make_current() || width <= 0 || height <= 0) return false;
    if (g_fbo && g_framebuffer_width == width && g_framebuffer_height == height) {
        return true;
    }

    destroy_framebuffer();

    glGenTextures(1, &g_color_texture);
    glBindTexture(GL_TEXTURE_2D, g_color_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);

    glGenRenderbuffers(1, &g_depth_buffer);
    glBindRenderbuffer(GL_RENDERBUFFER, g_depth_buffer);
    glRenderbufferStorage(GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

    glGenFramebuffers(1, &g_fbo);
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, g_color_texture, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, g_depth_buffer);

    const GLenum status = glCheckFramebufferStatus(GL_FRAMEBUFFER);
    if (status != GL_FRAMEBUFFER_COMPLETE) {
        char message[128];
        std::snprintf(message, sizeof(message), "WebGL offscreen framebuffer incomplete: 0x%04X.", (unsigned int)status);
        monopoly_web_log(message);
        destroy_framebuffer();
        return false;
    }

    g_framebuffer_width = width;
    g_framebuffer_height = height;
    return true;
}

static void active_viewport(int framebuffer_width,
                            int framebuffer_height,
                            int &x,
                            int &y,
                            int &width,
                            int &height)
{
    x = g_viewport_x;
    y = g_viewport_y;
    width = g_viewport_width;
    height = g_viewport_height;
    if (width <= 0 || height <= 0) {
        x = 0;
        y = 0;
        width = framebuffer_width;
        height = framebuffer_height;
    }
    if (x < 0) {
        width += x;
        x = 0;
    }
    if (y < 0) {
        height += y;
        y = 0;
    }
    if (x + width > framebuffer_width) width = framebuffer_width - x;
    if (y + height > framebuffer_height) height = framebuffer_height - y;
    if (width < 0) width = 0;
    if (height < 0) height = 0;
}

static void apply_active_viewport(int framebuffer_width, int framebuffer_height)
{
    int x, y, width, height;
    active_viewport(framebuffer_width, framebuffer_height, x, y, width, height);
    if (width <= 0 || height <= 0) return;
    glViewport(x, framebuffer_height - y - height, width, height);
}

static GLenum primitive_to_gl(D3DPRIMITIVETYPE primitive_type)
{
    switch (primitive_type) {
        case D3DPT_POINTLIST: return GL_POINTS;
        case D3DPT_LINELIST: return GL_LINES;
        case D3DPT_LINESTRIP: return GL_LINE_STRIP;
        case D3DPT_TRIANGLESTRIP: return GL_TRIANGLE_STRIP;
        case D3DPT_TRIANGLEFAN: return GL_TRIANGLE_FAN;
        case D3DPT_TRIANGLELIST:
        default: return GL_TRIANGLES;
    }
}

static GLenum blend_to_gl(DWORD blend)
{
    switch (blend) {
        case D3DBLEND_ZERO: return GL_ZERO;
        case D3DBLEND_ONE: return GL_ONE;
        case D3DBLEND_SRCCOLOR: return GL_SRC_COLOR;
        case D3DBLEND_INVSRCCOLOR: return GL_ONE_MINUS_SRC_COLOR;
        case D3DBLEND_SRCALPHA: return GL_SRC_ALPHA;
        case D3DBLEND_INVSRCALPHA: return GL_ONE_MINUS_SRC_ALPHA;
        case D3DBLEND_DESTALPHA: return GL_DST_ALPHA;
        case D3DBLEND_INVDESTALPHA: return GL_ONE_MINUS_DST_ALPHA;
        case D3DBLEND_DESTCOLOR: return GL_DST_COLOR;
        case D3DBLEND_INVDESTCOLOR: return GL_ONE_MINUS_DST_COLOR;
        default: return GL_ONE;
    }
}

static void apply_render_state()
{
    if (g_depth_enabled) glEnable(GL_DEPTH_TEST);
    else glDisable(GL_DEPTH_TEST);
    glDepthMask(g_depth_write_enabled ? GL_TRUE : GL_FALSE);

    if (g_alpha_blend_enabled) {
        glEnable(GL_BLEND);
        glBlendFunc(blend_to_gl(g_src_blend), blend_to_gl(g_dest_blend));
    } else {
        glDisable(GL_BLEND);
    }

    if (g_z_bias != 0) {
        glEnable(GL_POLYGON_OFFSET_FILL);
        glPolygonOffset(0.0f, -(GLfloat)g_z_bias);
    } else {
        glDisable(GL_POLYGON_OFFSET_FILL);
    }
}

static DWORD mask_shift(DWORD mask)
{
    DWORD shift = 0;
    if (!mask) return 0;
    while ((mask & 1u) == 0) {
        mask >>= 1;
        ++shift;
    }
    return shift;
}

static DWORD mask_bits(DWORD mask)
{
    DWORD bits = 0;
    if (!mask) return 0;
    mask >>= mask_shift(mask);
    while (mask & 1u) {
        ++bits;
        mask >>= 1;
    }
    return bits;
}

static unsigned char expand_masked_component(DWORD pixel, DWORD mask, unsigned char fallback)
{
    if (!mask) return fallback;
    DWORD bits = mask_bits(mask);
    if (!bits) return fallback;
    DWORD value = (pixel & mask) >> mask_shift(mask);
    DWORD max_value = (1u << bits) - 1u;
    return (unsigned char)((value * 255u + (max_value / 2u)) / max_value);
}

static DWORD read_pixel_value(const unsigned char *src, int bpp, int x)
{
    if (bpp == 8) return src[x];
    if (bpp == 16) return (DWORD)((WORD)src[x * 2 + 0] | ((WORD)src[x * 2 + 1] << 8));
    if (bpp == 24) {
        const unsigned char *p = src + (size_t)x * 3u;
        return (DWORD)p[0] | ((DWORD)p[1] << 8) | ((DWORD)p[2] << 16);
    }
    const unsigned char *p = src + (size_t)x * 4u;
    return (DWORD)p[0] | ((DWORD)p[1] << 8) | ((DWORD)p[2] << 16) | ((DWORD)p[3] << 24);
}

static D3DVECTOR transform_point(const D3DVECTOR &v, const D3DMATRIX &m)
{
    float w = v.x * m._14 + v.y * m._24 + v.z * m._34 + m._44;
    if (fabsf(w) < 0.000001f) w = 1.0f;
    return D3DVECTOR(
        (v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41) / w,
        (v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42) / w,
        (v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43) / w);
}

static D3DVECTOR project_point(const D3DVECTOR &v, const D3DMATRIX &m, bool &valid)
{
    float w = v.x * m._14 + v.y * m._24 + v.z * m._34 + m._44;
    valid = w > 0.0001f;
    if (fabsf(w) < 0.000001f) w = 1.0f;
    D3DVECTOR out(
        (v.x * m._11 + v.y * m._21 + v.z * m._31 + m._41) / w,
        (v.x * m._12 + v.y * m._22 + v.z * m._32 + m._42) / w,
        (v.x * m._13 + v.y * m._23 + v.z * m._33 + m._43) / w);
    valid = valid &&
            out.x == out.x && out.y == out.y && out.z == out.z &&
            fabsf(out.x) < 64.0f && fabsf(out.y) < 64.0f && fabsf(out.z) < 64.0f;
    return out;
}

static void unpack_color(DWORD color, WebVertex &out)
{
    out.a = ((color >> 24) & 0xff) / 255.0f;
    out.r = ((color >> 16) & 0xff) / 255.0f;
    out.g = ((color >> 8) & 0xff) / 255.0f;
    out.b = (color & 0xff) / 255.0f;
    if (out.a == 0.0f) out.a = 1.0f;
}

static void read_strided_texcoord(const D3DSTRIDEDATA *stride, DWORD index, WebVertex &out)
{
    if (!stride || !stride->textureCoords[0].lpvData) return;
    DWORD step = stride->textureCoords[0].dwStride ? stride->textureCoords[0].dwStride : (DWORD)(sizeof(D3DVALUE) * 2u);
    const D3DVALUE *uv = (const D3DVALUE *)((const BYTE *)stride->textureCoords[0].lpvData + (size_t)index * step);
    out.u = uv[0];
    out.v = uv[1];
}

static const BYTE *strided_ptr(const D3DSTRIDEDATA *stride, const void *base, DWORD index, DWORD fallback_stride)
{
    if (!base) return nullptr;
    DWORD step = stride ? fallback_stride : fallback_stride;
    return (const BYTE *)base + index * step;
}

static WebVertex read_vertex(DWORD vertex_type,
                             const void *vertices,
                             const D3DSTRIDEDATA *stride,
                             DWORD index,
                             const D3DMATRIX &world,
                             const D3DMATRIX &view,
                             const D3DMATRIX &projection)
{
    WebVertex out = {0, 0, 0, 1, 1, 1, 1, 0, 0, true};
    D3DVECTOR position(0, 0, 0);
    DWORD diffuse = 0xffffffffu;

    if (stride && stride->position.lpvData) {
        const BYTE *pos = (const BYTE *)stride->position.lpvData + index * stride->position.dwStride;
        position = *(const D3DVECTOR *)pos;
        if ((vertex_type & D3DFVF_DIFFUSE) && stride->diffuse.lpvData) {
            diffuse = *(const DWORD *)((const BYTE *)stride->diffuse.lpvData + index * stride->diffuse.dwStride);
        }
        read_strided_texcoord(stride, index, out);
    } else if (vertices) {
        if (vertex_type & D3DFVF_XYZRHW) {
            const D3DTLVERTEX *v = (const D3DTLVERTEX *)strided_ptr(nullptr, vertices, index, sizeof(D3DTLVERTEX));
            out.x = (v->sx / 400.0f) - 1.0f;
            out.y = 1.0f - (v->sy / 300.0f);
            out.z = v->sz * 2.0f - 1.0f;
            out.u = v->tu;
            out.v = v->tv;
            unpack_color(v->color ? v->color : 0xffffffffu, out);
            return out;
        } else if (vertex_type & D3DFVF_DIFFUSE) {
            const D3DLVERTEX *v = (const D3DLVERTEX *)strided_ptr(nullptr, vertices, index, sizeof(D3DLVERTEX));
            position = v->v;
            diffuse = v->color ? v->color : 0xffffffffu;
            out.u = v->tu;
            out.v = v->tv;
        } else {
            const D3DVERTEX *v = (const D3DVERTEX *)strided_ptr(nullptr, vertices, index, sizeof(D3DVERTEX));
            position = v->v;
            out.u = v->tu;
            out.v = v->tv;
        }
    }

    bool projected_valid = true;
    D3DVECTOR camera = transform_point(transform_point(position, world), view);
    D3DVECTOR clip = project_point(camera, projection, projected_valid);
    out.x = clip.x;
    out.y = clip.y;
    out.z = clip.z * 2.0f - 1.0f;
    out.valid = projected_valid;
    unpack_color(diffuse, out);
    return out;
}

static DWORD effective_vertex_count(DWORD vertex_count, const WORD *indices, DWORD index_count)
{
    if (!indices || !index_count) return vertex_count;
    DWORD max_index = 0;
    for (DWORD i = 0; i < index_count; ++i) {
        if (indices[i] > max_index) max_index = indices[i];
    }
    return vertex_count > max_index + 1 ? vertex_count : max_index + 1;
}

extern "C" int monopoly_webgl_init(const char *canvas_selector, int width, int height)
{
    if (g_context) return 1;

    EmscriptenWebGLContextAttributes attrs;
    emscripten_webgl_init_context_attributes(&attrs);
    attrs.majorVersion = 2;
    attrs.minorVersion = 0;
    attrs.alpha = EM_TRUE;
    attrs.depth = EM_TRUE;
    attrs.stencil = EM_FALSE;
    attrs.antialias = EM_TRUE;
    attrs.preserveDrawingBuffer = EM_TRUE;

    g_context = emscripten_webgl_create_context(canvas_selector ? canvas_selector : "#canvas", &attrs);
    if (g_context <= 0) {
        monopoly_web_log("Unable to create WebGL2 context.");
        g_context = 0;
        return 0;
    }

    emscripten_webgl_make_context_current(g_context);
    emscripten_set_canvas_element_size(canvas_selector ? canvas_selector : "#canvas", width, height);
    glViewport(0, 0, width, height);
    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LEQUAL);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glDisable(GL_CULL_FACE);
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    if (!ensure_framebuffer(width, height)) {
        monopoly_web_log("Unable to create WebGL offscreen framebuffer.");
        return 0;
    }
    ensure_program();
    monopoly_web_log("WebGL2 context initialized.");
    return 1;
}

extern "C" void monopoly_webgl_shutdown()
{
    if (!g_context) return;
    make_current();
    if (g_ibo) glDeleteBuffers(1, &g_ibo);
    if (g_vbo) glDeleteBuffers(1, &g_vbo);
    if (g_vao) glDeleteVertexArrays(1, &g_vao);
    if (g_program) glDeleteProgram(g_program);
    if (g_active_texture) glDeleteTextures(1, &g_active_texture);
    destroy_framebuffer();
    g_ibo = 0;
    g_vbo = 0;
    g_vao = 0;
    g_program = 0;
    g_active_texture = 0;
    g_has_active_texture = false;
    emscripten_webgl_destroy_context(g_context);
    g_context = 0;
    g_frame_started = false;
}

extern "C" void monopoly_webgl_begin_frame()
{
    if (!make_current()) return;
    if (!ensure_framebuffer(800, 600)) return;
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
    apply_active_viewport(800, 600);
    if (!g_frame_started) {
        int x, y, width, height;
        active_viewport(800, 600, x, y, width, height);
        if (width <= 0 || height <= 0) return;
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, 600 - y - height, width, height);
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClearDepthf(1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glDisable(GL_SCISSOR_TEST);
        g_scene_has_draw = false;
        g_frame_started = true;
    }
}

extern "C" void monopoly_webgl_begin_frame_from_rgb565(const unsigned char *pixels, int width, int height, int pitch)
{
    if (!pixels || width <= 0 || height <= 0 || pitch <= 0) {
        monopoly_webgl_begin_frame();
        return;
    }
    if (!make_current()) return;
    if (!ensure_framebuffer(width, height)) return;
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
    apply_active_viewport(width, height);
    if (g_frame_started) return;

    const size_t row_bytes = (size_t)width * 4u;
    const size_t rgba_size = row_bytes * (size_t)height;
    unsigned char *rgba = (unsigned char *)malloc(rgba_size ? rgba_size : 1u);
    if (!rgba) {
        monopoly_webgl_begin_frame();
        return;
    }

    for (int y = 0; y < height; ++y) {
        const unsigned char *src = pixels + (size_t)(height - 1 - y) * (size_t)pitch;
        unsigned char *dst = rgba + (size_t)y * row_bytes;
        for (int x = 0; x < width; ++x) {
            WORD value = (WORD)src[x * 2 + 0] | ((WORD)src[x * 2 + 1] << 8);
            dst[x * 4 + 0] = (unsigned char)((((value >> 11) & 0x1f) * 255u) / 31u);
            dst[x * 4 + 1] = (unsigned char)((((value >> 5) & 0x3f) * 255u) / 63u);
            dst[x * 4 + 2] = (unsigned char)(((value & 0x1f) * 255u) / 31u);
            dst[x * 4 + 3] = 255;
        }
    }

    glBindTexture(GL_TEXTURE_2D, g_color_texture);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    free(rgba);

    int x, y, viewport_width, viewport_height;
    active_viewport(width, height, x, y, viewport_width, viewport_height);
    if (viewport_width <= 0 || viewport_height <= 0) return;
    glEnable(GL_SCISSOR_TEST);
    glScissor(x, height - y - viewport_height, viewport_width, viewport_height);
    glClearDepthf(1.0f);
    glClear(GL_DEPTH_BUFFER_BIT);
    glDisable(GL_SCISSOR_TEST);
    g_scene_has_draw = false;
    g_frame_started = true;
}

extern "C" void monopoly_webgl_clear(DWORD flags, DWORD color, float depth)
{
    if (!make_current()) return;
    if (!ensure_framebuffer(800, 600)) return;
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
    apply_active_viewport(800, 600);
    GLbitfield bits = 0;
    if (flags & D3DCLEAR_TARGET) {
        float r = ((color >> 16) & 0xff) / 255.0f;
        float g = ((color >> 8) & 0xff) / 255.0f;
        float b = (color & 0xff) / 255.0f;
        glClearColor(r, g, b, 1.0f);
        bits |= GL_COLOR_BUFFER_BIT;
        if (!g_frame_started) {
            g_scene_has_draw = false;
            g_frame_started = true;
        }
    }
    if (flags & D3DCLEAR_ZBUFFER) {
        glClearDepthf(depth);
        bits |= GL_DEPTH_BUFFER_BIT;
    }
    if (bits) {
        int x, y, width, height;
        active_viewport(800, 600, x, y, width, height);
        if (width <= 0 || height <= 0) return;
        glEnable(GL_SCISSOR_TEST);
        glScissor(x, 600 - y - height, width, height);
        glClear(bits);
        glDisable(GL_SCISSOR_TEST);
    }
}

extern "C" void monopoly_webgl_read_rgb565(unsigned char *pixels, int width, int height, int pitch)
{
    if (!pixels || width <= 0 || height <= 0 || pitch <= 0) return;
    if (!make_current()) return;
    if (!ensure_framebuffer(width, height)) return;
    if (!g_scene_has_draw) {
        if (!g_logged_skipped_readback) {
            monopoly_web_log("WebGL readback skipped: scene had no draws.");
            g_logged_skipped_readback = true;
        }
        g_frame_started = false;
        return;
    }
    int viewport_x, viewport_y, viewport_width, viewport_height;
    active_viewport(width, height, viewport_x, viewport_y, viewport_width, viewport_height);
    if (viewport_width <= 0 || viewport_height <= 0) {
        g_scene_has_draw = false;
        g_frame_started = false;
        return;
    }
    glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
    if (g_readback_count++ == 0) {
        monopoly_web_log("WebGL readback: first RGB565 frame.");
    }

    const size_t row_bytes = (size_t)viewport_width * 4u;
    const size_t buffer_size = row_bytes * (size_t)viewport_height;
    unsigned char *rgba = (unsigned char *)malloc(buffer_size);
    if (!rgba) return;

    glPixelStorei(GL_PACK_ALIGNMENT, 1);
    glReadPixels(viewport_x, height - viewport_y - viewport_height,
                 viewport_width, viewport_height, GL_RGBA, GL_UNSIGNED_BYTE, rgba);

    for (int y = 0; y < viewport_height; ++y) {
        const unsigned char *src = rgba + (size_t)(viewport_height - 1 - y) * row_bytes;
        unsigned char *dst = pixels + (size_t)(viewport_y + y) * (size_t)pitch + (size_t)viewport_x * 2u;
        for (int x = 0; x < viewport_width; ++x) {
            const unsigned char r = src[x * 4 + 0];
            const unsigned char g = src[x * 4 + 1];
            const unsigned char b = src[x * 4 + 2];
            const WORD rgb565 = (WORD)(((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3));
            dst[x * 2 + 0] = (unsigned char)(rgb565 & 0xff);
            dst[x * 2 + 1] = (unsigned char)(rgb565 >> 8);
        }
    }

    free(rgba);
    g_scene_has_draw = false;
    g_frame_started = false;
}

extern "C" void monopoly_webgl_set_viewport(int x, int y, int width, int height)
{
    g_viewport_x = x;
    g_viewport_y = y;
    g_viewport_width = width;
    g_viewport_height = height;
    if (make_current() && g_fbo) {
        glBindFramebuffer(GL_FRAMEBUFFER, g_fbo);
        apply_active_viewport(g_framebuffer_width ? g_framebuffer_width : 800,
                              g_framebuffer_height ? g_framebuffer_height : 600);
    }
}

extern "C" void monopoly_webgl_set_render_state(DWORD state, DWORD value)
{
    switch (state) {
        case D3DRENDERSTATE_ZENABLE:
            g_depth_enabled = value != 0;
            break;
        case D3DRENDERSTATE_ZWRITEENABLE:
            g_depth_write_enabled = value != 0;
            break;
        case D3DRENDERSTATE_ALPHABLENDENABLE:
            g_alpha_blend_enabled = value != 0;
            break;
        case D3DRENDERSTATE_SRCBLEND:
            g_src_blend = value;
            break;
        case D3DRENDERSTATE_DESTBLEND:
            g_dest_blend = value;
            break;
        case D3DRENDERSTATE_ZBIAS:
            g_z_bias = value;
            break;
        case D3DRENDERSTATE_COLORKEYENABLE:
            g_color_key_enabled = value != 0;
            break;
        default:
            break;
    }
}

extern "C" void monopoly_webgl_set_texture(const unsigned char *pixels, int width, int height, int pitch, int bpp,
                                           const DDPIXELFORMAT *format,
                                           const PALETTEENTRY *palette, int paletteEntries,
                                           int hasSrcColorKey, DWORD colorKeyLow, DWORD colorKeyHigh)
{
    if (!make_current()) return;
    if (!pixels || width <= 0 || height <= 0 || pitch <= 0 || bpp <= 0) {
        g_has_active_texture = false;
        g_active_texture_discard_zero_alpha = false;
        return;
    }
    if (bpp != 8 && bpp != 16 && bpp != 24 && bpp != 32) {
        g_has_active_texture = false;
        g_active_texture_discard_zero_alpha = false;
        return;
    }
    if (!g_active_texture) {
        glGenTextures(1, &g_active_texture);
    }
    const size_t rgba_size = (size_t)width * (size_t)height * 4u;
    unsigned char *rgba = (unsigned char *)malloc(rgba_size ? rgba_size : 1u);
    if (!rgba) {
        g_has_active_texture = false;
        g_active_texture_discard_zero_alpha = false;
        return;
    }

    DWORD rmask = format ? format->dwRBitMask : 0;
    DWORD gmask = format ? format->dwGBitMask : 0;
    DWORD bmask = format ? format->dwBBitMask : 0;
    DWORD amask = format ? format->dwRGBAlphaBitMask : 0;
    DWORD flags = format ? format->dwFlags : 0;
    if (!rmask && !gmask && !bmask) {
        if (bpp == 16) {
            rmask = 0x0000f800u;
            gmask = 0x000007e0u;
            bmask = 0x0000001fu;
        } else {
            rmask = 0x00ff0000u;
            gmask = 0x0000ff00u;
            bmask = 0x000000ffu;
            if (bpp == 32 && (flags & DDPF_ALPHAPIXELS)) amask = 0xff000000u;
        }
    }

    for (int y = 0; y < height; ++y) {
        const unsigned char *src = pixels + (size_t)y * (size_t)pitch;
        unsigned char *dst = rgba + (size_t)y * (size_t)width * 4u;
        for (int x = 0; x < width; ++x) {
            unsigned char r = 255, g = 255, b = 255, a = 255;
            if (bpp == 8) {
                DWORD pixel = src[x];
                if (palette && pixel < (DWORD)paletteEntries) {
                    const PALETTEENTRY &entry = palette[pixel];
                    r = entry.peRed;
                    g = entry.peGreen;
                    b = entry.peBlue;
                    if (flags & DDPF_ALPHAPREMULT) {
                        a = entry.peFlags;
                    }
                } else {
                    r = g = b = (unsigned char)pixel;
                }
                if (hasSrcColorKey && pixel >= colorKeyLow && pixel <= colorKeyHigh) {
                    a = 0;
                }
            } else {
                DWORD pixel = read_pixel_value(src, bpp, x);
                r = expand_masked_component(pixel, rmask, 255);
                g = expand_masked_component(pixel, gmask, 255);
                b = expand_masked_component(pixel, bmask, 255);
                if ((flags & DDPF_ALPHAPIXELS) && amask) {
                    a = expand_masked_component(pixel, amask, 255);
                }
                if (hasSrcColorKey && pixel >= colorKeyLow && pixel <= colorKeyHigh) {
                    a = 0;
                }
            }
            dst[x * 4 + 0] = r;
            dst[x * 4 + 1] = g;
            dst[x * 4 + 2] = b;
            dst[x * 4 + 3] = a;
        }
    }

    glBindTexture(GL_TEXTURE_2D, g_active_texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, rgba);
    free(rgba);
    g_has_active_texture = true;
    g_active_texture_discard_zero_alpha = hasSrcColorKey != 0;
    if (!g_logged_first_texture) {
        char message[128];
        std::snprintf(message, sizeof(message), "WebGL texture upload: first texture %dx%d x %d bpp.", width, height, bpp);
        monopoly_web_log(message);
        g_logged_first_texture = true;
    }
}

extern "C" int monopoly_webgl_draw_primitive(D3DPRIMITIVETYPE primitive_type,
                                             DWORD vertex_type,
                                             const void *vertices,
                                             DWORD vertex_count,
                                             const WORD *indices,
                                             DWORD index_count,
                                             const D3DSTRIDEDATA *stride_data,
                                             const D3DMATRIX *world,
                                             const D3DMATRIX *view,
                                             const D3DMATRIX *projection)
{
    if ((!vertices && !stride_data) || (!vertex_count && !index_count)) return 1;
    if (!ensure_program()) return 0;
    monopoly_webgl_begin_frame();
    if (g_draw_count++ == 0) {
        monopoly_web_log("WebGL draw: first primitive.");
    }
    g_scene_has_draw = true;

    const D3DMATRIX &world_matrix = world ? *world : k_identity_matrix;
    const D3DMATRIX &view_matrix = view ? *view : k_identity_matrix;
    const D3DMATRIX &projection_matrix = projection ? *projection : k_identity_matrix;

    DWORD count = effective_vertex_count(vertex_count, indices, index_count);
    WebVertex *web_vertices = (WebVertex *)malloc(sizeof(WebVertex) * count);
    if (!web_vertices) return 0;
    for (DWORD i = 0; i < count; ++i) {
        web_vertices[i] = read_vertex(vertex_type, vertices, stride_data, i, world_matrix, view_matrix, projection_matrix);
    }

    glUseProgram(g_program);
    apply_render_state();
    GLint texture_uniform = glGetUniformLocation(g_program, "u_texture");
    GLint has_texture_uniform = glGetUniformLocation(g_program, "u_has_texture");
    GLint discard_zero_alpha_uniform = glGetUniformLocation(g_program, "u_discard_zero_alpha");
    glActiveTexture(GL_TEXTURE0);
    if (g_has_active_texture && g_active_texture) {
        glBindTexture(GL_TEXTURE_2D, g_active_texture);
        if (has_texture_uniform >= 0) glUniform1i(has_texture_uniform, 1);
    } else {
        glBindTexture(GL_TEXTURE_2D, 0);
        if (has_texture_uniform >= 0) glUniform1i(has_texture_uniform, 0);
    }
    if (texture_uniform >= 0) glUniform1i(texture_uniform, 0);
    if (discard_zero_alpha_uniform >= 0) {
        glUniform1i(discard_zero_alpha_uniform, g_active_texture_discard_zero_alpha ? 1 : 0);
    }
    glBindVertexArray(g_vao);
    glBindBuffer(GL_ARRAY_BUFFER, g_vbo);
    glBufferData(GL_ARRAY_BUFFER, count * sizeof(WebVertex), web_vertices, GL_DYNAMIC_DRAW);

    GLenum primitive = primitive_to_gl(primitive_type);
    if (indices && index_count) {
        WORD *filtered_indices = nullptr;
        const WORD *draw_indices = indices;
        DWORD draw_index_count = index_count;
        if (primitive_type == D3DPT_TRIANGLELIST) {
            filtered_indices = (WORD *)malloc(sizeof(WORD) * index_count);
            if (filtered_indices) {
                draw_index_count = 0;
                for (DWORD i = 0; i + 2 < index_count; i += 3) {
                    WORD i0 = indices[i + 0];
                    WORD i1 = indices[i + 1];
                    WORD i2 = indices[i + 2];
                    if (i0 >= count || i1 >= count || i2 >= count) continue;
                    if (!web_vertices[i0].valid || !web_vertices[i1].valid || !web_vertices[i2].valid) continue;
                    filtered_indices[draw_index_count++] = i0;
                    filtered_indices[draw_index_count++] = i1;
                    filtered_indices[draw_index_count++] = i2;
                }
                draw_indices = filtered_indices;
            }
        }
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, g_ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, draw_index_count * sizeof(WORD), draw_indices, GL_DYNAMIC_DRAW);
        if (draw_index_count > 0) glDrawElements(primitive, (GLsizei)draw_index_count, GL_UNSIGNED_SHORT, 0);
        if (filtered_indices) free(filtered_indices);
    } else {
        glDrawArrays(primitive, 0, (GLsizei)count);
    }
    glBindVertexArray(0);
    free(web_vertices);
    return 1;
}
