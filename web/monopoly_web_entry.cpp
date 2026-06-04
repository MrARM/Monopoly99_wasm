#include <emscripten.h>
#include <emscripten/html5.h>

#include "compat/windows.h"
#include "../artlib/L_Main.h"
#include "../artlib/L_Keybrd.h"
#include "../monopoly/GameInc.h"
#include "../artlib/L_Mouse.h"

extern BOOL GameStartup(void);
extern BOOL GameUpdateCycle(void);
extern void GameShutdown(void);
extern void CALLBACK LI_TIMERS_TimerCallBack(UINT idTimer, UINT msg, DWORD dwUser, DWORD dw1, DWORD dw2);
extern void LI_TIMER_CallCyclicFunctions(void);

HINSTANCE LE_MAIN_Hinst = NULL;
HWND LE_MAIN_HwndMainWindow = NULL;
char LE_MAIN_ApplicationNameString[] = CE_ARTLIB_ApplicationName;
ACHAR *LE_MAIN_CommandLine = NULL;
BOOL LE_MAIN_ApplicationActive = TRUE;

static bool g_system_initialized = false;
static bool g_game_started = false;
static bool g_stop_requested = false;
static bool g_created_main_window = false;
static double g_timer_last_ms = 0.0;
static double g_timer_carry_ms = 0.0;

extern "C" void monopoly_web_log(const char *message);
extern "C" int monopoly_web_mount_idbfs(void);
extern "C" int monopoly_web_sync_idbfs(int populate);
extern "C" void monopoly_web_audio_unlock(void);
extern "C" int monopoly_webgl_init(const char *canvas_selector, int width, int height);
extern "C" void monopoly_webgl_shutdown(void);

static int monopoly_web_ensure_main_window()
{
    if (LE_MAIN_HwndMainWindow) {
        return 1;
    }

    LE_MAIN_Hinst = reinterpret_cast<HINSTANCE>(1);
    LE_MAIN_HwndMainWindow = CreateWindowA(
        "MonopolyWebWindow",
        LE_MAIN_ApplicationNameString,
        0,
        0,
        0,
        800,
        600,
        NULL,
        NULL,
        LE_MAIN_Hinst,
        NULL);

    if (!LE_MAIN_HwndMainWindow) {
        monopoly_web_log("Unable to create web main window shim.");
        return 0;
    }

    g_created_main_window = true;
    return 1;
}

static void monopoly_web_destroy_main_window()
{
    if (g_created_main_window && LE_MAIN_HwndMainWindow) {
        DestroyWindow(LE_MAIN_HwndMainWindow);
    }
    LE_MAIN_HwndMainWindow = NULL;
    LE_MAIN_Hinst = NULL;
    g_created_main_window = false;
}

static void monopoly_web_pump_timers()
{
    const double tick_ms = 1000.0 / (double)CE_ARTLIB_TIME_BASIC_CLOCK_RATE_HZ;
    static bool logged_cyclic = false;
    double now_ms = emscripten_get_now();
    double elapsed_ms = now_ms - g_timer_last_ms;
    int ticks = 0;

    if (g_timer_last_ms <= 0.0) {
        g_timer_last_ms = now_ms;
        return;
    }

    g_timer_last_ms = now_ms;
    if (elapsed_ms < 0.0) elapsed_ms = 0.0;
    if (elapsed_ms > 250.0) elapsed_ms = 250.0;
    g_timer_carry_ms += elapsed_ms;

    while (g_timer_carry_ms >= tick_ms && ticks < 8) {
        LI_TIMERS_TimerCallBack(0, 0, 0, 0, 0);
        LI_TIMER_CallCyclicFunctions();
        if (!logged_cyclic) {
            monopoly_web_log("Web timer cyclic animation pump active.");
            logged_cyclic = true;
        }
        g_timer_carry_ms -= tick_ms;
        ticks++;
    }

    if (ticks >= 8) {
        g_timer_carry_ms = 0.0;
    }
}

static void monopoly_web_tick()
{
    if (!g_game_started || g_stop_requested) {
        emscripten_cancel_main_loop();
        return;
    }

    monopoly_web_pump_timers();

    if (!GameUpdateCycle()) {
        g_stop_requested = true;
        GameShutdown();
        LE_INIT_SystemRemoval();
        monopoly_webgl_shutdown();
        monopoly_web_destroy_main_window();
        g_game_started = false;
        g_system_initialized = false;
        emscripten_cancel_main_loop();
        monopoly_web_sync_idbfs(0);
        monopoly_web_log("Monopoly runtime stopped.");
    }
}

extern "C" EMSCRIPTEN_KEEPALIVE int monopoly_web_start()
{
    if (g_game_started) {
        return 1;
    }

    LE_MAIN_ApplicationActive = TRUE;
    LE_MAIN_CommandLine = const_cast<ACHAR *>(A_T(""));
    monopoly_web_audio_unlock();

    monopoly_web_mount_idbfs();
    monopoly_web_sync_idbfs(1);

    if (!monopoly_web_ensure_main_window()) {
        return 0;
    }

    if (!monopoly_webgl_init("#canvas", 800, 600)) {
        monopoly_web_log("WebGL2 initialization failed.");
        monopoly_web_destroy_main_window();
        return 0;
    }

    monopoly_web_log("Initializing ArtLib systems.");
    LE_INIT_SystemInit();
    g_system_initialized = true;

    monopoly_web_log("Starting Monopoly game.");
    if (!GameStartup()) {
        monopoly_web_log("GameStartup failed.");
        if (g_system_initialized) {
            LE_INIT_SystemRemoval();
            g_system_initialized = false;
        }
        monopoly_webgl_shutdown();
        monopoly_web_destroy_main_window();
        return 0;
    }

    g_timer_last_ms = emscripten_get_now();
    g_timer_carry_ms = 0.0;
    g_game_started = true;
    g_stop_requested = false;
    emscripten_set_main_loop(monopoly_web_tick, 0, 0);
    return 1;
}

extern "C" EMSCRIPTEN_KEEPALIVE void monopoly_web_stop()
{
    g_stop_requested = true;
}

extern "C" EMSCRIPTEN_KEEPALIVE void monopoly_web_key_down(int code)
{
    if (!g_system_initialized) return;
    BYTE key = LE_KEYBRD_ConvertAsciiToOurCode((char)(code & 0xff));
    LE_UIMSG_SendEvent(UIMSG_KEYBOARD_PRESSED, key, code, 0, 0, 0, NULL, 0);
}

extern "C" EMSCRIPTEN_KEEPALIVE void monopoly_web_key_up(int code)
{
    if (!g_system_initialized) return;
    BYTE key = LE_KEYBRD_ConvertAsciiToOurCode((char)(code & 0xff));
    LE_UIMSG_SendEvent(UIMSG_KEYBOARD_RELEASED, key, code, 0, 0, 0, NULL, 0);
}

extern "C" EMSCRIPTEN_KEEPALIVE void monopoly_web_char(int code)
{
    if (!g_system_initialized || code <= 0) return;
    LE_UIMSG_SendEvent(UIMSG_KEYBOARD_ACHAR, code, 0, 0, 0, 0, NULL, 0);
}

extern "C" EMSCRIPTEN_KEEPALIVE void monopoly_web_mouse_event(int type, int x, int y, int button)
{
    if (!g_system_initialized) return;

    const int dx = x - (int)LE_MOUSE_MouseX;
    const int dy = y - (int)LE_MOUSE_MouseY;
    if (dx != 0 || dy != 0) {
        LE_MOUSE_MouseDeltaX = (short)(LE_MOUSE_MouseDeltaX + dx);
        LE_MOUSE_MouseDeltaY = (short)(LE_MOUSE_MouseDeltaY + dy);
        LE_MOUSE_MouseX = (short)x;
        LE_MOUSE_MouseY = (short)y;
    }

    LE_UIMSG_MessageType msg = UIMSG_MOUSE_MOVED;
    if (type == 1 && button == 0) msg = UIMSG_MOUSE_LEFT_DOWN;
    else if (type == 2 && button == 0) msg = UIMSG_MOUSE_LEFT_UP;
    else if (type == 1 && button == 1) msg = UIMSG_MOUSE_MIDDLE_DOWN;
    else if (type == 2 && button == 1) msg = UIMSG_MOUSE_MIDDLE_UP;
    else if (type == 1 && button == 2) msg = UIMSG_MOUSE_RIGHT_DOWN;
    else if (type == 2 && button == 2) msg = UIMSG_MOUSE_RIGHT_UP;

#if CE_ARTLIB_MousePressedAndLatchedArray
    if (type == 1 && button >= 0 && button < LI_MOUSE_NUM_BUTTONS) {
        LE_MOUSE_LatchedMouseX = LE_MOUSE_MouseX;
        LE_MOUSE_LatchedMouseY = LE_MOUSE_MouseY;
        LE_MOUSE_MousePressed[button] = 1;
        LE_MOUSE_MouseLatched[button] = 1;
    } else if (type == 2 && button >= 0 && button < LI_MOUSE_NUM_BUTTONS) {
        LE_MOUSE_MousePressed[button] = 0;
    }
#endif

    if (msg == UIMSG_MOUSE_LEFT_DOWN) {
        char buffer[128];
        snprintf(buffer, sizeof(buffer), "web mouse down x=%d y=%d phase=%d view=%d desired=%d",
            x, y,
            DISPLAY_state.currentPlayerSetupPhase,
            DISPLAY_state.current2DView,
            DISPLAY_state.desired2DView);
        monopoly_web_log(buffer);
    }
    LE_UIMSG_SendEvent(msg, x, y, dx, dy, 0, NULL, 0);
}

extern "C" EMSCRIPTEN_KEEPALIVE int monopoly_web_debug_state(int field)
{
    switch (field) {
    case 0:
        return DISPLAY_state.currentPlayerSetupPhase;
    case 1:
        return DISPLAY_state.current2DView;
    case 2:
        return DISPLAY_state.desired2DView;
    default:
        return 0;
    }
}

int main(int, char **)
{
    monopoly_web_log("Monopoly WASM module loaded.");
    return 0;
}
