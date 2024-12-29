#include "../game.c"

#include "windows.h"
// #include <Windows.h>
// #include <Windowsx.h>
int32 _fltused;

#include "main.h"
#include "api.c"

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
            is_running = 0;
            DestroyWindow(window);
            break;
        }
        case WM_DESTROY:
        {
            PostQuitMessage(0);
            break;
        }
        case WM_SIZE:
        case WM_EXITSIZEMOVE:
        {
            RECT client_rectangle;
            GetClientRect(window, &client_rectangle);

            int32 client_width = client_rectangle.right - client_rectangle.left;
            int32 client_height = client_rectangle.bottom - client_rectangle.top;

            window_width = client_width;
            window_height = client_height;

            break;
        }
        case WM_GETMINMAXINFO:
        {
            MINMAXINFO* min_max_info = (MINMAXINFO*)lParam;
            min_max_info->ptMinTrackSize.x = 320 / 2;
            min_max_info->ptMinTrackSize.y = 240 / 2;

            break;
        }
        case WM_PAINT:
        {
            PAINTSTRUCT paint_struct;
            HDC device_context = BeginPaint(window, &paint_struct);

            StretchBlt(device_context, 0, 0, window_width, window_height,
                       memory_device_context, 0, offscreen.height - 1, offscreen.width, -offscreen.height, SRCCOPY);

            EndPaint(window, &paint_struct);

            break;
        }
        case WM_KILLFOCUS:
        case WM_ENTERSIZEMOVE:
        {
            reset_input();

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

HWND initialize_window(HINSTANCE instance, int32 width, int32 height)
{
    WNDCLASSEXW window_class = { 0 };
    window_class.cbSize = sizeof(WNDCLASSEXW);
    window_class.style = CS_HREDRAW | CS_VREDRAW | CS_OWNDC;
    window_class.lpfnWndProc = window_procedure;
    window_class.hInstance = instance;
    window_class.hCursor = LoadCursorW(0, MAKEINTRESOURCEW(32512));
    window_class.hIcon = LoadIconW(0, MAKEINTRESOURCEW(32513));
    window_class.hbrBackground = GetStockObject(HOLLOW_BRUSH);
    window_class.lpszClassName = L"Redungeon Classic";

    RegisterClassExW(&window_class);

    DWORD window_extended_style = 0;
    DWORD window_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;

    RECT client_rectangle = { 0 };
    client_rectangle.right = width;
    client_rectangle.bottom = height;
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
            case WM_LBUTTONUP:
            case WM_RBUTTONDOWN:
            case WM_RBUTTONUP:
            case WM_MBUTTONDOWN:
            case WM_MBUTTONUP:
            {


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
                int32 position_x = GET_X_LPARAM(message.lParam);
                int32 position_y = window_height - GET_Y_LPARAM(message.lParam);

                process_mouse(position_x, position_y, window_width, window_height);

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
    platform_api.reserve_memory = reserve_memory_implementation;
    platform_api.commit_memory = commit_memory_implementation;
    platform_api.decommit_memory = decommit_memory_implementation;
    platform_api.release_memory = release_memory_implementation;
    platform_api.present_offscreen = present_offscreen_implementation;

    memory_device_context = CreateCompatibleDC(device_context);

    BITMAPINFOHEADER bitmap_info = { 0 };
    bitmap_info.biSize = sizeof(BITMAPINFOHEADER);
    bitmap_info.biWidth = MAX_RESOLUTION;
    bitmap_info.biHeight = -MAX_RESOLUTION;
    bitmap_info.biPlanes = 1;
    bitmap_info.biBitCount = 32;
    bitmap_info.biCompression = BI_RGB;

    HBITMAP device_bitmap = CreateDIBSection(0, (BITMAPINFO*)&bitmap_info, DIB_RGB_COLORS, (void**)&device_bitmap_memory, 0, 0);
    HGDIOBJ previous_object = SelectObject(memory_device_context, device_bitmap);

    is_running = initialize_game();
    while (is_running)
    {
        process_window_messages();

        game_loop();
    }

    SelectObject(memory_device_context, previous_object);
    DeleteObject(device_bitmap);
    DeleteDC(memory_device_context);

    ExitThread(0);
}

void __stdcall wWinMainCRTStartup()
{
    instance = GetModuleHandleW(0);
    if (!instance)
        ExitProcess(1);

    window = initialize_window(instance, window_width, window_height);
    if (!window)
        ExitProcess(1);

    device_context = GetDC(window);
    if (!device_context)
        ExitProcess(1);

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

    ReleaseDC(window, device_context);

    ExitProcess(0);
}
