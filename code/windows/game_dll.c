#include "../game.c"
#include "game_dll.h"

#if NO_C_EXTENTION
#include "windows.h"
#else
#undef max
#undef min
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
__declspec(dllimport) LONG NTAPI NtDelayExecution(BOOLEAN Alertable, LARGE_INTEGER* DelayInterval);
#endif

#ifdef NO_C_RUNTIME
int32 _fltused;
#endif

__declspec(dllexport) void process_button_dll(Button button, bool32 is_down)
{
    process_button(button, is_down);
}

__declspec(dllexport) void process_mouse_dll(int32 x, int32 y, int32 width, int32 height)
{
    process_mouse(x, y, width, height);
}

__declspec(dllexport) void reset_input_dll()
{
    reset_input();
}

__declspec(dllexport) bool32 initialize_game_dll()
{
    return initialize_game();
}

__declspec(dllexport) void game_loop_dll()
{
    game_loop();
}

__declspec(dllexport) void deinitialize_game_dll()
{
    deinitialize_game();
}

#ifdef NO_C_RUNTIME
BOOL WINAPI DllMainCRTStartup(HINSTANCE hinstDLL, DWORD fdwReason, LPVOID lpvReserved)
{
    return TRUE;
}
#endif
