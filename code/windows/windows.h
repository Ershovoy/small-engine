//
// No SAL
//

#define _In_opt_
#define _In_
#define _Out_
#define _Pre_
#define _Notnull_
#define _Pre_notnull_ _Pre_ _Notnull_

//
// Defines
//

#define VOID void
#define CONST const
#define WINAPI   __stdcall
#define CALLBACK __stdcall

//
// Data types
//

typedef short SHORT;
typedef int   INT;
typedef long  LONG;

typedef unsigned short USHORT;
typedef unsigned int   UINT;
typedef unsigned long  ULONG;

typedef int BOOL;

//
//
//

typedef void *PVOID;
typedef void *LPVOID;

//
// Handles
//

typedef PVOID HANDLE;
typedef HANDLE HINSTANCE;
typedef HINSTANCE HMODULE;
typedef HANDLE HDC;
typedef HANDLE HBITMAP;
typedef HANDLE HBRUSH;
typedef HANDLE HICON;
typedef HICON HCURSOR;
typedef HANDLE HWND;
typedef HANDLE HMENU;
typedef HANDLE HGDIOBJ;

//
//
//

typedef char CHAR;
typedef unsigned short wchar_t;
typedef wchar_t WCHAR;

typedef __int64 LONG_PTR;
typedef unsigned __int64 ULONG_PTR;

typedef unsigned __int64 UINT_PTR;
typedef __int64 LONGLONG;

typedef unsigned char BYTE;
typedef BYTE BOOLEAN;

typedef unsigned long DWORD;
typedef unsigned short WORD;
typedef WORD ATOM;

typedef UINT_PTR WPARAM;
typedef LONG_PTR LPARAM;
typedef LONG_PTR LRESULT;

typedef ULONG_PTR SIZE_T;

//

#define DECLSPEC_IMPORT __declspec(dllimport)
#define WINBASEAPI DECLSPEC_IMPORT

#define CS_VREDRAW          0x0001
#define CS_HREDRAW          0x0002
#define CS_OWNDC            0x0020

#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_MINIMIZE         0x20000000L
#define WS_VISIBLE          0x10000000L
#define WS_DISABLED         0x08000000L
#define WS_CLIPSIBLINGS     0x04000000L
#define WS_CLIPCHILDREN     0x02000000L
#define WS_MAXIMIZE         0x01000000L
#define WS_CAPTION          0x00C00000L
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_VSCROLL          0x00200000L
#define WS_HSCROLL          0x00100000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_GROUP            0x00020000L
#define WS_TABSTOP          0x00010000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L

#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED     | \
                             WS_CAPTION        | \
                             WS_SYSMENU        | \
                             WS_THICKFRAME     | \
                             WS_MINIMIZEBOX    | \
                             WS_MAXIMIZEBOX)

#define CW_USEDEFAULT       ((int)0x80000000)

//

#define WM_PAINT                        0x000F
#define WM_CLOSE                        0x0010
#define WM_DESTROY                      0x0002
#define WM_MOVE                         0x0003
#define WM_SIZE                         0x0005
#define WM_EXITSIZEMOVE                 0x0232
#define WM_GETMINMAXINFO                0x0024
#define WM_KILLFOCUS                    0x0008
#define WM_ENTERSIZEMOVE                0x0231

#define PM_REMOVE           0x0001

#define WM_KEYDOWN                      0x0100
#define WM_KEYUP                        0x0101
#define WM_SYSKEYDOWN                   0x0104
#define WM_SYSKEYUP                     0x0105

#define KF_REPEAT         0x4000
#define KF_UP             0x8000

#define LOWORD(l)           ((WORD)(((ULONG_PTR)(l)) & 0xffff))
#define HIWORD(l)           ((WORD)((((ULONG_PTR)(l)) >> 16) & 0xffff))

#define INFINITE            0xFFFFFFFF


//


//

#define MAKEINTRESOURCEA(i) ((const CHAR*)((ULONG_PTR)((WORD)(i))))
#define MAKEINTRESOURCEW(i) ((const WCHAR*)((ULONG_PTR)((WORD)(i))))


//

typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct tagWNDCLASSEXA
{
    UINT        cbSize;
    /* Win 3.x */
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    const CHAR*      lpszMenuName;
    const CHAR*      lpszClassName;
    /* Win 4.0 */
    HICON       hIconSm;
} WNDCLASSEXA, *PWNDCLASSEXA, *NPWNDCLASSEXA, *LPWNDCLASSEXA;

typedef struct tagWNDCLASSEXW
{
    UINT        cbSize;
    /* Win 3.x */
    UINT        style;
    WNDPROC     lpfnWndProc;
    int         cbClsExtra;
    int         cbWndExtra;
    HINSTANCE   hInstance;
    HICON       hIcon;
    HCURSOR     hCursor;
    HBRUSH      hbrBackground;
    const WCHAR*     lpszMenuName;
    const WCHAR*     lpszClassName;
    /* Win 4.0 */
    HICON       hIconSm;
} WNDCLASSEXW, *PWNDCLASSEXW, *NPWNDCLASSEXW, *LPWNDCLASSEXW;

typedef struct tagRECT
{
    LONG    left;
    LONG    top;
    LONG    right;
    LONG    bottom;
} RECT;

typedef struct tagPOINT
{
    LONG  x;
    LONG  y;
} POINT;

typedef struct tagMSG
{
    HWND        hwnd;
    UINT        message;
    WPARAM      wParam;
    LPARAM      lParam;
    DWORD       time;
    POINT       pt;
} MSG;

typedef struct _SECURITY_ATTRIBUTES {
    DWORD nLength;
    void* lpSecurityDescriptor;
    BOOL bInheritHandle;
} SECURITY_ATTRIBUTES, *PSECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct tagMINMAXINFO {
    POINT ptReserved;
    POINT ptMaxSize;
    POINT ptMaxPosition;
    POINT ptMinTrackSize;
    POINT ptMaxTrackSize;
} MINMAXINFO, *PMINMAXINFO, *LPMINMAXINFO;

typedef struct tagPAINTSTRUCT {
    HDC         hdc;
    BOOL        fErase;
    RECT        rcPaint;
    BOOL        fRestore;
    BOOL        fIncUpdate;
    BYTE        rgbReserved[32];
} PAINTSTRUCT, *PPAINTSTRUCT, *NPPAINTSTRUCT, *LPPAINTSTRUCT;

typedef struct tagBITMAPINFOHEADER{
        DWORD      biSize;
        LONG       biWidth;
        LONG       biHeight;
        WORD       biPlanes;
        WORD       biBitCount;
        DWORD      biCompression;
        DWORD      biSizeImage;
        LONG       biXPelsPerMeter;
        LONG       biYPelsPerMeter;
        DWORD      biClrUsed;
        DWORD      biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD {
        BYTE    rgbBlue;
        BYTE    rgbGreen;
        BYTE    rgbRed;
        BYTE    rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO {
    BITMAPINFOHEADER    bmiHeader;
    RGBQUAD             bmiColors[1];
} BITMAPINFO;

typedef struct _OVERLAPPED {
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    union {
        struct {
            DWORD Offset;
            DWORD OffsetHigh;
        } DUMMYSTRUCTNAME;
        void* Pointer;
    } DUMMYUNIONNAME;

    HANDLE  hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef union _LARGE_INTEGER {
    struct {
        DWORD LowPart;
        LONG HighPart;
    } DUMMYSTRUCTNAME;
    struct {
        DWORD LowPart;
        LONG HighPart;
    } u;
    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef void (WINAPI* LPOVERLAPPED_COMPLETION_ROUTINE)(DWORD dwErrorCode, DWORD dwNumberOfBytesTransfered, LPOVERLAPPED lpOverlapped);

typedef DWORD (WINAPI *PTHREAD_START_ROUTINE)(void* lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

//
// Mouse input
//

#define WM_MOUSEFIRST                   0x0200
#define WM_MOUSEMOVE                    0x0200
#define WM_LBUTTONDOWN                  0x0201
#define WM_LBUTTONUP                    0x0202
#define WM_LBUTTONDBLCLK                0x0203
#define WM_RBUTTONDOWN                  0x0204
#define WM_RBUTTONUP                    0x0205
#define WM_RBUTTONDBLCLK                0x0206
#define WM_MBUTTONDOWN                  0x0207
#define WM_MBUTTONUP                    0x0208
#define WM_MBUTTONDBLCLK                0x0209
#define WM_MOUSEWHEEL                   0x020A

//
// Memory managment
//

#define PAGE_NOACCESS  0x01
#define PAGE_READWRITE 0x04

#define MEM_COMMIT   0x00001000
#define MEM_RESERVE  0x00002000
#define MEM_DECOMMIT 0x00004000
#define MEM_RELEASE  0x00008000

WINBASEAPI LPVOID WINAPI VirtualAlloc(_In_opt_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD flAllocationType, _In_ DWORD flProtect);
WINBASEAPI BOOL WINAPI VirtualFree(_Pre_notnull_ LPVOID lpAddress, _In_ SIZE_T dwSize, _In_ DWORD dwFreeType);

//
// File system
//

#define GENERIC_READ                     (0x80000000L)
#define GENERIC_WRITE                    (0x40000000L)
#define FILE_SHARE_READ                 0x00000001

#define OPEN_EXISTING       3

#define INVALID_HANDLE_VALUE ((HANDLE)(LONG_PTR)-1)

#define INVALID_FILE_SIZE ((DWORD)0xFFFFFFFF)

__declspec(dllimport) HANDLE __stdcall CreateFileA(const CHAR* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
__declspec(dllimport) HANDLE __stdcall CreateFileW(const WCHAR* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);

__declspec(dllimport) DWORD __stdcall GetFileSize(HANDLE hFile, DWORD* lpFileSizeHigh);

__declspec(dllimport) BOOL __stdcall ReadFile(HANDLE hFile, void* lpBuffer, DWORD nNumberOfBytesToRead, DWORD* lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
__declspec(dllimport) BOOL __stdcall ReadFileEx(HANDLE hFile, void* lpBuffer, DWORD nNumberOfBytesToRead, LPOVERLAPPED lpOverlapped, LPOVERLAPPED_COMPLETION_ROUTINE lpCompletionRoutine);

//

__declspec(dllimport) HMODULE __stdcall GetModuleHandleA(const CHAR* module_name);
__declspec(dllimport) HMODULE __stdcall GetModuleHandleW(const WCHAR* module_name);

__declspec(dllimport) HCURSOR WINAPI LoadCursorA(HINSTANCE hInstance, const CHAR* lpCursorName);
__declspec(dllimport) HCURSOR WINAPI LoadCursorW(HINSTANCE hInstance, const WCHAR* lpCursorName);

__declspec(dllimport) HICON WINAPI LoadIconA(HINSTANCE hInstance, const CHAR* lpIconName);
__declspec(dllimport) HICON WINAPI LoadIconW(HINSTANCE hInstance, const WCHAR* lpIconName);

__declspec(dllimport) ATOM WINAPI RegisterClassExA(const WNDCLASSEXA*);
__declspec(dllimport) ATOM WINAPI RegisterClassExW(const WNDCLASSEXW*);

__declspec(dllimport) BOOL WINAPI AdjustWindowRectEx(RECT* lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);

__declspec(dllimport) HWND WINAPI CreateWindowExA(DWORD dwExStyle, const CHAR* lpClassName, const CHAR* lpWindowName, DWORD dwStyle,
                                                  int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, void* lpParam);
__declspec(dllimport) HWND WINAPI CreateWindowExW(DWORD dwExStyle, const WCHAR* lpClassName, const WCHAR* lpWindowName, DWORD dwStyle,
                                                  int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, void* lpParam);

__declspec(dllimport) LRESULT WINAPI DefWindowProcA(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
__declspec(dllimport) LRESULT WINAPI DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);

__declspec(dllimport) BOOL WINAPI GetMessageA(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
__declspec(dllimport) BOOL WINAPI GetMessageW(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);

__declspec(dllimport) BOOL WINAPI TranslateMessage(const MSG* lpMsg);

__declspec(dllimport) LRESULT WINAPI DispatchMessageA(const MSG* lpMsg);
__declspec(dllimport) LRESULT WINAPI DispatchMessageW(const MSG* lpMsg);

__declspec(dllimport) HDC WINAPI GetDC(HWND hWnd);

__declspec(dllimport) void WINAPI PostQuitMessage(int nExitCode);

__declspec(dllimport) __declspec(noreturn) void WINAPI ExitProcess(UINT exit_code);

__declspec(dllimport) HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, ULONG_PTR dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, void* lpParameter, DWORD dwCreationFlags, DWORD* lpThreadId);

__declspec(dllimport) BOOL WINAPI CloseHandle(HANDLE hObject);

__declspec(dllimport) BOOL WINAPI PeekMessageA(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
__declspec(dllimport) BOOL WINAPI PeekMessageW(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);

__declspec(dllimport) __declspec(noreturn) void WINAPI ExitThread(DWORD dwExitCode);

__declspec(dllimport) BOOL WINAPI PostThreadMessageA(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);
__declspec(dllimport) BOOL WINAPI PostThreadMessageW(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);

__declspec(dllimport) DWORD WINAPI WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);

__declspec(dllimport) int WINAPI ReleaseDC(HWND hWnd, HDC hDC);

__declspec(dllimport) BOOL WINAPI DestroyWindow(HWND hWnd);

__declspec(dllimport) BOOL WINAPI GetClientRect(HWND hWnd, RECT* lpRect);
__declspec(dllimport) HDC WINAPI BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);

__declspec(dllimport) BOOL WINAPI EndPaint(HWND hWnd, const PAINTSTRUCT *lpPaint);

__declspec(dllimport) BOOL WINAPI InvalidateRect(HWND hWnd, const RECT* lpRect, BOOL bErase);
__declspec(dllimport) BOOL WINAPI UpdateWindow(HWND hWnd);

__declspec(dllimport) BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount);
__declspec(dllimport) BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency);

WINBASEAPI VOID WINAPI Sleep(_In_ DWORD dwMilliseconds);

// gdi

#define WINGDIAPI DECLSPEC_IMPORT

#define NULL_BRUSH          5
#define HOLLOW_BRUSH        NULL_BRUSH
#define BI_RGB        0L
#define DIB_RGB_COLORS      0
#define SRCCOPY             (DWORD)0x00CC0020

WINGDIAPI BOOL WINAPI DeleteObject(_In_ HGDIOBJ ho);
__declspec(dllimport) void* WINAPI GetStockObject(int i);
__declspec(dllimport) BOOL WINAPI StretchBlt(HDC hdcDest, int xDest, int yDest, int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop);
__declspec(dllimport) HGDIOBJ WINAPI SelectObject(HDC hdc, HGDIOBJ h);
__declspec(dllimport) BOOL WINAPI DeleteDC(HDC hdc);
__declspec(dllimport) HBITMAP WINAPI CreateDIBSection(HDC hdc, const BITMAPINFO* pbmi, UINT usage, void** ppvBits, HANDLE hSection, DWORD offset);
__declspec(dllimport) HDC WINAPI CreateCompatibleDC(HDC hdc);

//

#define NTAPI __stdcall
__declspec(dllimport) LONG NTAPI NtDelayExecution(BOOLEAN Alertable, LARGE_INTEGER* DelayInterval);
