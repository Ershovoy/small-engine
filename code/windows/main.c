#ifdef INTERNAL
#include "../game.c"
#endif

#ifdef NO_C_EXTENTION
#include "windows.h"
#else
#undef max
#undef min
#define VC_EXTRALEAN
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <windowsx.h>
#define _WINSOCK_DEPRECATED_NO_WARNINGS
#include <winsock2.h>
#include <mmsystem.h>
#include <d3d11.h>
#endif

#ifndef INTERNAL
#include "debug.h"
#include "debug.c"
#endif

#ifdef NO_C_RUNTIME
int32 _fltused;
#endif

#include "main.h"
#include "xaudio2.c"
#ifndef SOFTWARE
#include "directx.c"
#endif
#include "api.c"

void toggle_fullscreen()
{
    if (!is_fullscreen)
    {
        GetWindowRect(window, &previous_windowed_rect);
        previous_window_style = GetWindowLongPtrW(window, GWL_STYLE);

        int32 screen_width = GetSystemMetrics(SM_CXSCREEN);
        int32 screen_height = GetSystemMetrics(SM_CYSCREEN);

        SetWindowLongPtrW(window, GWL_STYLE, WS_VISIBLE | WS_POPUP);

        SetWindowPos(window, HWND_TOP, 0, 0, screen_width, screen_height, SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }
    else
    {
        SetWindowLongPtrW(window, GWL_STYLE, previous_window_style);
        SetWindowPos(window, HWND_TOP,
                     previous_windowed_rect.left,
                     previous_windowed_rect.top,
                     previous_windowed_rect.right - previous_windowed_rect.left,
                     previous_windowed_rect.bottom - previous_windowed_rect.top,
                     SWP_FRAMECHANGED | SWP_SHOWWINDOW);
    }

    is_fullscreen = !is_fullscreen;
}

void calculate_presented_region()
{
    int32 scale_x = client_width / game_horizontal_resoultion;
    int32 scale_y = client_height / game_vertical_resolution;

    int32 scale = scale_x;
    if (scale_y < scale_x)
    {
        scale = scale_y;
    }

    if (scale < 1)
    {
        scale = 1;
    }

    present_width = game_horizontal_resoultion * scale;
    if (present_width > client_width)
    {
        present_width = client_width;
    }
    present_height = game_vertical_resolution * scale;
    if (present_height > client_height)
    {
        present_height = client_height;
    }

    present_min_x = (client_width - present_width) / 2;
    present_max_x = (client_width + present_width) / 2;
    present_min_y = (client_height - present_height) / 2;
    present_max_y = (client_height + present_height) / 2;
}

LRESULT CALLBACK window_procedure(HWND   window,
                                  UINT   message,
                                  WPARAM wParam,
                                  LPARAM lParam)
{
    LRESULT result = { 0 };
    switch (message)
    {
        case WM_CLOSE:
        {
            DestroyWindow(window);
            break;
        }
        case WM_DESTROY:
        {
            is_running = 0;
            PostQuitMessage(0);
            break;
        }
        case WM_SIZE:
        case WM_EXITSIZEMOVE:
        {
            RECT client_rectangle;
            GetClientRect(window, &client_rectangle);

            client_width = client_rectangle.right - client_rectangle.left;
            client_height = client_rectangle.bottom - client_rectangle.top;

            break;
        }
        case WM_GETMINMAXINFO:
        {
            RECT client_rectangle = { 0 };
            client_rectangle.right = MINIMUM(game_horizontal_resoultion, GAME_DEFAULT_HORIZONTAL_RESOLUTION);
            client_rectangle.bottom = MINIMUM(game_vertical_resolution, GAME_DEFAULT_VERTICAL_RESOLUTION);
            RECT window_rectangle = client_rectangle;
            AdjustWindowRectEx(&window_rectangle, window_style, 0, window_extended_style);
            LONG minimal_window_width = window_rectangle.right - window_rectangle.left;
            LONG minimal_window_height = window_rectangle.bottom - window_rectangle.top;

            MINMAXINFO* min_max_info = (MINMAXINFO*)lParam;
            min_max_info->ptMinTrackSize.x = minimal_window_width;
            min_max_info->ptMinTrackSize.y = minimal_window_height;

            break;
        }
        // case WM_SETCURSOR:
        // {
        //     if ((mouse_position_x >= present_min_x && mouse_position_y >= present_min_y && mouse_position_x < present_max_x && mouse_position_y < present_max_y) &&
        //         (LOWORD(lParam) == HTCLIENT))
        //     {
        //         // SetCursor(0);
        //     }
        //     else
        //     {
        //         // result = DefWindowProcW(window, message, wParam, lParam);;
        //     }

        //     break;
        // }
        case WM_PAINT:
        {
            PAINTSTRUCT paint_struct;
            HDC device_context = BeginPaint(window, &paint_struct);

            calculate_presented_region();

#ifdef SOFTWARE
            PatBlt(device_context, 0, 0, present_min_x, client_height, BLACKNESS);
            PatBlt(device_context, present_max_x, 0, client_width - present_max_x, client_height, BLACKNESS);
            PatBlt(device_context, 0, 0, client_width, present_min_y, BLACKNESS);
            PatBlt(device_context, 0, present_max_y, client_width, client_height - present_max_y, BLACKNESS);

            StretchBlt(device_context, present_min_x , present_min_y, present_width, present_height,
             		   memory_device_context, 0, game_vertical_resolution - 1, game_horizontal_resoultion, -game_vertical_resolution, SRCCOPY);
#endif

            EndPaint(window, &paint_struct);

            break;
        }
        case WM_KILLFOCUS:
        case WM_ENTERSIZEMOVE:
        {
            reset_input();

            break;
        }
        case WM_KEYDOWN:
        case WM_KEYUP:
        case WM_SYSKEYDOWN:
        case WM_SYSKEYUP:
        {
            WORD key_code = LOWORD(wParam);
            WORD key_flags = HIWORD(lParam);

            BOOL is_key_down = (key_flags & KF_UP) != KF_UP;
            BOOL was_key_down = (key_flags & KF_REPEAT) == KF_REPEAT;

            if ((key_code == VK_ESCAPE) && (is_key_down != was_key_down) && (is_key_down))
            {
                DestroyWindow(window);
            }

            if ((key_code == VK_F11) && (is_key_down != was_key_down) && (is_key_down))
            {
                toggle_fullscreen();
            }

            if (key_code == VK_RETURN && (lParam & 0x60000000) == 0x20000000)
            {
                toggle_fullscreen();
            }

            break;
        }
        default:
        {
            result = DefWindowProcW(window, message, wParam, lParam);
            break;
        }
    }

    return result;
}

HWND initialize_window(HINSTANCE instance, int32 client_width, int32 client_height)
{
    WNDCLASSEXW window_class = { 0 };
    window_class.cbSize = sizeof(WNDCLASSEXW);
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = window_procedure;
    window_class.hInstance = instance;
    window_class.hCursor = LoadCursorW(0, MAKEINTRESOURCEW(32512));
    window_class.hIcon = LoadIconW(0, MAKEINTRESOURCEW(32513));
    window_class.lpszClassName = L"Blobby Volley 3 by Qvil";

    RegisterClassExW(&window_class);

    RECT client_rectangle = { 0 };
    client_rectangle.right = client_width;
    client_rectangle.bottom = client_height;
    RECT window_rectangle = client_rectangle;
    AdjustWindowRectEx(&window_rectangle, window_style, 0, window_extended_style);
    LONG required_window_width = window_rectangle.right - window_rectangle.left;
    LONG required_window_height = window_rectangle.bottom - window_rectangle.top;

    HWND window = CreateWindowExW(window_extended_style, window_class.lpszClassName,
                                  window_class.lpszClassName, window_style,
                                  CW_USEDEFAULT, CW_USEDEFAULT, required_window_width,
                                  required_window_height, 0, 0, instance, 0);

    return window;
}

void process_window_messages()
{
    MSG message = { 0 };
    while (PeekMessageW(&message, 0, 0, 0, PM_REMOVE))
    {
        switch (message.message)
        {
            case WM_QUIT:
            {
                is_running = 0;
                break;
            }
            case WM_KEYDOWN:
            case WM_KEYUP:
            case WM_SYSKEYDOWN:
            case WM_SYSKEYUP:
            {
                WORD key_code = LOWORD(message.wParam);
                WORD key_flags = HIWORD(message.lParam);

                BOOL is_key_down = (key_flags & KF_UP) != KF_UP;
                BOOL was_key_down = (key_flags & KF_REPEAT) == KF_REPEAT;

                process_button(key_code, is_key_down);

                break;
            }
            case WM_LBUTTONDOWN:
            {
                process_button(BUTTON_LEFT, 1);

                break;
            }
            case WM_MBUTTONDOWN:
            {
                process_button(BUTTON_MIDDLE, 1);

                break;
            }
            case WM_RBUTTONDOWN:
            {
                process_button(BUTTON_RIGHT, 1);

                break;
            }
            case WM_LBUTTONUP:
            {
                 process_button(BUTTON_LEFT, 0);

                 break;
            }
            case WM_MBUTTONUP:
            {
                 process_button(BUTTON_MIDDLE, 0);

                 break;
            }
            case WM_RBUTTONUP:
            {
                process_button(BUTTON_RIGHT, 0);

                break;
            }
            case WM_MOUSEWHEEL:
            {
                int16 delta = (int16)HIWORD(message.wParam);

                if (delta > 0)
                {
                    process_button(BUTTON_WHEEL_UP, 1);
                    process_button(BUTTON_WHEEL_UP, 0);
                }
                else
                {
                    process_button(BUTTON_WHEEL_DOWN, 1);
                    process_button(BUTTON_WHEEL_DOWN, 0);
                }

                break;
            }
            case WM_MOUSEMOVE:
            {
                mouse_position_x = GET_X_LPARAM(message.lParam);
                mouse_position_y = client_height - GET_Y_LPARAM(message.lParam) - 1;

                if (mouse_position_x >= present_min_x && mouse_position_y >= present_min_y && mouse_position_x < present_max_x && mouse_position_y < present_max_y)
                {
                    process_mouse(mouse_position_x - present_min_x, mouse_position_y - present_min_y, present_width, present_height);
                }

                break;
            }
            default:
            {
                break;
            }
        }
    }
}

DWORD WINAPI game_loop_handle(void* lpParameter)
{
    platform_api.sleep = sleep_implementation;
    platform_api.get_time_tick = get_time_tick_implementation;
    platform_api.get_file_size = get_file_size_implementation;
    platform_api.read_file = read_file_implementation;
    platform_api.write_file = write_file_implementation;
    platform_api.reserve_memory = reserve_memory_implementation;
    platform_api.commit_memory = commit_memory_implementation;
    platform_api.decommit_memory = decommit_memory_implementation;
    platform_api.release_memory = release_memory_implementation;
    platform_api.console_write = console_write_implementation;
    platform_api.net_bind = net_bind_implementation;
    platform_api.net_send = net_send_implementation;
    platform_api.net_receive = net_receive_implementation;
    platform_api.play_sound = xaudio2_play_sound;
    platform_api.present_offscreen = present_offscreen_implementation;

#ifdef SOFTWARE
    memory_device_context = CreateCompatibleDC(device_context);

    BITMAPINFOHEADER bitmap_info = { 0 };
    bitmap_info.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info.biWidth = GAME_MAX_HORIZONTAL_RESOLUTION;
    bitmap_info.biHeight = -GAME_MAX_VERTICAL_RESOLUTION;
    bitmap_info.biPlanes = 1;
    bitmap_info.biBitCount = 32;
    bitmap_info.biCompression = BI_RGB;

    HBITMAP device_bitmap = CreateDIBSection(0, (BITMAPINFO*)&bitmap_info, DIB_RGB_COLORS, (void**)&device_bitmap_memory, 0, 0);
    HGDIOBJ previous_object = SelectObject(memory_device_context, device_bitmap);
#endif

#ifndef INTERNAL
    initialize_debug();
#endif

    is_running = initialize_game();
    while (is_running)
    {
#ifndef INTERNAL
        update_debug();
#endif

        process_window_messages();

        game_loop();
    }

    deinitialize_game();

#ifndef INTERNAL
    deinitialize_debug();
#endif

#ifdef SOFTWARE
    SelectObject(memory_device_context, previous_object);
    DeleteObject(device_bitmap);
    DeleteDC(memory_device_context);
#endif

    ExitThread(0);
}

#ifdef NO_C_RUNTIME
void __stdcall wWinMainCRTStartup(void)
#else
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
#endif
{
    instance = GetModuleHandleW(0);
    if (!instance)
        ExitProcess(1);

    // if (!AllocConsole())
    //     ExitProcess(1);

    if (!initialize_xaudio2())
       ExitProcess(1);

    WSADATA winsock_data;
    if (WSAStartup(0x202, &winsock_data))
        ExitProcess(1);

    sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
    if (sock == INVALID_SOCKET)
        ExitProcess(1);

    uint64 non_blocking = 1;
    if (ioctlsocket(sock, FIONBIO, (u_long*)&non_blocking) != NO_ERROR)
        ExitProcess(1);

    window = initialize_window(instance, client_width, client_height);
    if (!window)
        ExitProcess(1);

    device_context = GetDC(window);
    if (!device_context)
        ExitProcess(1);

#ifndef SOFTWARE
    if (!initialize_directx(window))
        ExitProcess(1);
#endif

    timeBeginPeriod(1);

    DWORD thread_id;
    HANDLE thread_handle = CreateThread(0, 0, (LPTHREAD_START_ROUTINE)game_loop_handle, 0, 0, &thread_id);

    MSG message = { 0 };
    while (GetMessageW(&message, 0, 0, 0))
    {
        PostThreadMessageW(thread_id, message.message, message.wParam, message.lParam);

        TranslateMessage(&message);
        DispatchMessageW(&message);
    }

    WaitForSingleObject(thread_handle, INFINITE);
    CloseHandle(thread_handle);

    timeEndPeriod(1);

#ifndef SOTFWARE
    deinitialize_directx();
#endif

    destroy_xaudio2();

    WSACleanup();

    ReleaseDC(window, device_context);

    ExitProcess(0);
}
