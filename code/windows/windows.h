//
// SAL stubs
//

#define _In_
#define _In_opt_
#define _Out_
#define _Out_opt_
#define _Pre_
#define _Notnull_
#define _Pre_notnull_
#define _In_reads_bytes_opt_(s)

//
// Calling conventions
//

#define WINAPI      __stdcall
#define CALLBACK    __stdcall
#define NTAPI       __stdcall
#define PASCAL      __stdcall
#define WSAAPI      __stdcall

//
// Qualifiers
//

#define CONST       const
#define VOID        void

#define DECLSPEC_IMPORT __declspec(dllimport)
#define WINBASEAPI  DECLSPEC_IMPORT
#define WINUSERAPI  DECLSPEC_IMPORT
#define WINGDIAPI   DECLSPEC_IMPORT

//
// Primitive types
//

typedef short              SHORT;
typedef int                INT;
typedef long               LONG;
typedef unsigned short     USHORT;
typedef unsigned int       UINT;
typedef unsigned long      ULONG;
typedef int                BOOL;
typedef long               HRESULT;
typedef unsigned char      BYTE;
typedef unsigned char      BOOLEAN;
typedef unsigned short     WORD;
typedef unsigned long      DWORD;
typedef WORD               ATOM;
typedef float              FLOAT;

typedef __int64            LONGLONG;
typedef unsigned __int64   ULONGLONG;

typedef __int64            LONG_PTR;
typedef unsigned __int64   ULONG_PTR;
typedef unsigned __int64   UINT_PTR;
typedef __int64            INT_PTR;
typedef ULONG_PTR          SIZE_T;

typedef UINT_PTR           WPARAM;
typedef LONG_PTR           LPARAM;
typedef LONG_PTR           LRESULT;

//
// Char types
//

typedef char               CHAR;
typedef unsigned short     WCHAR;

//
// Void pointer types
//

typedef void*              PVOID;
typedef void*              LPVOID;

//
// Handles
//

typedef PVOID              HANDLE;
typedef HANDLE             HINSTANCE;
typedef HINSTANCE          HMODULE;
typedef HANDLE             HDC;
typedef HANDLE             HBITMAP;
typedef HANDLE             HBRUSH;
typedef HANDLE             HICON;
typedef HICON              HCURSOR;
typedef HANDLE             HWND;
typedef HANDLE             HMENU;
typedef HANDLE             HGDIOBJ;

//
// Function pointer types
//

typedef INT_PTR (WINAPI *FARPROC)();
typedef INT_PTR (WINAPI *NEARPROC)();
typedef INT_PTR (WINAPI *PROC)();

//
// Boolean constants
//

#define TRUE    1
#define FALSE   0

//
// Window class styles
//

#define CS_VREDRAW      0x0001
#define CS_HREDRAW      0x0002
#define CS_OWNDC        0x0020

//
// Window styles
//

#define WS_OVERLAPPED       0x00000000L
#define WS_POPUP            0x80000000L
#define WS_CHILD            0x40000000L
#define WS_VISIBLE          0x10000000L
#define WS_CAPTION          0x00C00000L
#define WS_BORDER           0x00800000L
#define WS_DLGFRAME         0x00400000L
#define WS_SYSMENU          0x00080000L
#define WS_THICKFRAME       0x00040000L
#define WS_MINIMIZEBOX      0x00020000L
#define WS_MAXIMIZEBOX      0x00010000L

#define WS_OVERLAPPEDWINDOW (WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU | WS_THICKFRAME | WS_MINIMIZEBOX | WS_MAXIMIZEBOX)

#define CW_USEDEFAULT       ((int)0x80000000)

//
// Window messages
//

#define WM_DESTROY          0x0002
#define WM_MOVE             0x0003
#define WM_SIZE             0x0005
#define WM_KILLFOCUS        0x0008
#define WM_PAINT            0x000F
#define WM_CLOSE            0x0010
#define WM_QUIT             0x0012
#define WM_GETMINMAXINFO    0x0024
#define WM_KEYDOWN          0x0100
#define WM_KEYUP            0x0101
#define WM_SYSKEYDOWN       0x0104
#define WM_SYSKEYUP         0x0105
#define WM_MOUSEMOVE        0x0200
#define WM_LBUTTONDOWN      0x0201
#define WM_LBUTTONUP        0x0202
#define WM_RBUTTONDOWN      0x0204
#define WM_RBUTTONUP        0x0205
#define WM_MBUTTONDOWN      0x0207
#define WM_MBUTTONUP        0x0208
#define WM_MOUSEWHEEL       0x020A
#define WM_ENTERSIZEMOVE    0x0231
#define WM_EXITSIZEMOVE     0x0232

//
// Key / message flags
//

#define KF_REPEAT       0x4000
#define KF_UP           0x8000

#define PM_REMOVE       0x0001

#define VK_ESCAPE       0x1B
#define VK_F11          0x7A

//
// SetWindowPos flags
//

#define HWND_TOP        ((HWND)0)
#define HWND_TOPMOST    ((HWND)-1)
#define HWND_NOTOPMOST  ((HWND)-2)

#define SWP_NOSIZE          0x0001
#define SWP_NOMOVE          0x0002
#define SWP_NOZORDER        0x0004
#define SWP_FRAMECHANGED    0x0020
#define SWP_SHOWWINDOW      0x0040

//
// GetWindowLong / SetWindowLong indices
//

#define GWL_STYLE       (-16)
#define GWL_EXSTYLE     (-20)
#define GWL_USERDATA    (-21)

//
// GetSystemMetrics
//

#define SM_CXSCREEN     0
#define SM_CYSCREEN     1

//
// GDI
//

#define BI_RGB          0L
#define DIB_RGB_COLORS  0
#define SRCCOPY         (DWORD)0x00CC0020
#define BLACKNESS       (DWORD)0x00000042

#define WHITE_BRUSH     0
#define BLACK_BRUSH     4
#define NULL_BRUSH      5

//
// Memory
//

#define PAGE_NOACCESS   0x01
#define PAGE_READWRITE  0x04

#define MEM_COMMIT      0x00001000
#define MEM_RESERVE     0x00002000
#define MEM_DECOMMIT    0x00004000
#define MEM_RELEASE     0x00008000

//
// File system
//

#define GENERIC_READ    (0x80000000L)
#define GENERIC_WRITE   (0x40000000L)
#define FILE_SHARE_READ 0x00000001

#define CREATE_NEW          1
#define CREATE_ALWAYS       2
#define OPEN_EXISTING       3
#define OPEN_ALWAYS         4
#define TRUNCATE_EXISTING   5

#define INVALID_HANDLE_VALUE    ((HANDLE)(LONG_PTR)-1)
#define INVALID_FILE_SIZE       ((DWORD)0xFFFFFFFF)

#define INFINITE            0xFFFFFFFF

//
// Console
//

#define STD_INPUT_HANDLE    ((DWORD)-10)
#define STD_OUTPUT_HANDLE   ((DWORD)-11)
#define STD_ERROR_HANDLE    ((DWORD)-12)

//
// Misc
//

#define NO_ERROR    0L

#define LOWORD(l)   ((WORD)(((ULONG_PTR)(l)) & 0xffff))
#define HIWORD(l)   ((WORD)((((ULONG_PTR)(l)) >> 16) & 0xffff))

#define GET_X_LPARAM(lp)    ((int)(short)LOWORD(lp))
#define GET_Y_LPARAM(lp)    ((int)(short)HIWORD(lp))

#define MAKEINTRESOURCEW(i) ((const WCHAR*)((ULONG_PTR)((WORD)(i))))

//
// Structs
//

typedef LRESULT (CALLBACK* WNDPROC)(HWND, UINT, WPARAM, LPARAM);

typedef struct tagWNDCLASSEXW
{
    UINT          cbSize;
    UINT          style;
    WNDPROC       lpfnWndProc;
    int           cbClsExtra;
    int           cbWndExtra;
    HINSTANCE     hInstance;
    HICON         hIcon;
    HCURSOR       hCursor;
    HBRUSH        hbrBackground;
    const WCHAR*  lpszMenuName;
    const WCHAR*  lpszClassName;
    HICON         hIconSm;
} WNDCLASSEXW;

typedef struct tagRECT
{
    LONG left;
    LONG top;
    LONG right;
    LONG bottom;
} RECT;

typedef struct tagPOINT
{
    LONG x;
    LONG y;
} POINT;

typedef struct tagMSG
{
    HWND   hwnd;
    UINT   message;
    WPARAM wParam;
    LPARAM lParam;
    DWORD  time;
    POINT  pt;
} MSG;

typedef struct tagMINMAXINFO
{
    POINT ptReserved;
    POINT ptMaxSize;
    POINT ptMaxPosition;
    POINT ptMinTrackSize;
    POINT ptMaxTrackSize;
} MINMAXINFO, *LPMINMAXINFO;

typedef struct tagPAINTSTRUCT
{
    HDC   hdc;
    BOOL  fErase;
    RECT  rcPaint;
    BOOL  fRestore;
    BOOL  fIncUpdate;
    BYTE  rgbReserved[32];
} PAINTSTRUCT, *LPPAINTSTRUCT;

typedef struct tagBITMAPINFOHEADER
{
    DWORD biSize;
    LONG  biWidth;
    LONG  biHeight;
    WORD  biPlanes;
    WORD  biBitCount;
    DWORD biCompression;
    DWORD biSizeImage;
    LONG  biXPelsPerMeter;
    LONG  biYPelsPerMeter;
    DWORD biClrUsed;
    DWORD biClrImportant;
} BITMAPINFOHEADER;

typedef struct tagRGBQUAD
{
    BYTE rgbBlue;
    BYTE rgbGreen;
    BYTE rgbRed;
    BYTE rgbReserved;
} RGBQUAD;

typedef struct tagBITMAPINFO
{
    BITMAPINFOHEADER bmiHeader;
    RGBQUAD          bmiColors[1];
} BITMAPINFO;

typedef struct _SECURITY_ATTRIBUTES
{
    DWORD nLength;
    void* lpSecurityDescriptor;
    BOOL  bInheritHandle;
} SECURITY_ATTRIBUTES, *LPSECURITY_ATTRIBUTES;

typedef struct _OVERLAPPED
{
    ULONG_PTR Internal;
    ULONG_PTR InternalHigh;
    struct { DWORD Offset; DWORD OffsetHigh; } DUMMYSTRUCTNAME;
    HANDLE hEvent;
} OVERLAPPED, *LPOVERLAPPED;

typedef union _LARGE_INTEGER
{
    struct { DWORD LowPart; LONG HighPart; } DUMMYSTRUCTNAME;
    LONGLONG QuadPart;
} LARGE_INTEGER;

typedef struct _FILETIME
{
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;

typedef struct _WIN32_FILE_ATTRIBUTE_DATA
{
    DWORD    dwFileAttributes;
    FILETIME ftCreationTime;
    FILETIME ftLastAccessTime;
    FILETIME ftLastWriteTime;
    DWORD    nFileSizeHigh;
    DWORD    nFileSizeLow;
} WIN32_FILE_ATTRIBUTE_DATA;

typedef enum _GET_FILEEX_INFO_LEVELS
{
    GetFileExInfoStandard,
    GetFileExMaxInfoLevel
} GET_FILEEX_INFO_LEVELS;

typedef void   (WINAPI *LPOVERLAPPED_COMPLETION_ROUTINE)(DWORD, DWORD, LPOVERLAPPED);
typedef DWORD  (WINAPI *PTHREAD_START_ROUTINE)(void* lpThreadParameter);
typedef PTHREAD_START_ROUTINE LPTHREAD_START_ROUTINE;

//
// Winsock types
//

typedef UINT_PTR SOCKET;
typedef unsigned char  u_char;
typedef unsigned short u_short;
typedef unsigned int   u_int;
typedef unsigned long  u_long;
typedef BYTE           UCHAR;
typedef USHORT         ADDRESS_FAMILY;

#define INVALID_SOCKET  (SOCKET)(~0)
#define SOCKET_ERROR    (-1)

#define AF_INET         2

#define SOCK_STREAM     1
#define SOCK_DGRAM      2
#define SOCK_RAW        3

#define INADDR_ANY       (ULONG)0x00000000
#define INADDR_LOOPBACK  0x7f000001
#define INADDR_BROADCAST (ULONG)0xffffffff
#define INADDR_NONE      0xffffffff

#define FIONBIO     (0x80000000 | (((long)sizeof(u_long) & 0x7f) << 16) | (('f') << 8) | 126)

typedef enum
{
    IPPROTO_ICMP = 1,
    IPPROTO_TCP  = 6,
    IPPROTO_UDP  = 17,
    IPPROTO_RAW  = 255,
} IPPROTO;

typedef struct in_addr
{
    union
    {
        struct { UCHAR s_b1, s_b2, s_b3, s_b4; } S_un_b;
        struct { USHORT s_w1, s_w2; }             S_un_w;
        ULONG                                     S_addr;
    } S_un;
    #define s_addr S_un.S_addr
} IN_ADDR;

typedef struct sockaddr_in
{
    short    sin_family;
    USHORT   sin_port;
    IN_ADDR  sin_addr;
    CHAR     sin_zero[8];
} SOCKADDR_IN;

typedef struct sockaddr
{
    u_short sa_family;
    CHAR    sa_data[14];
} SOCKADDR;

#define WSADESCRIPTION_LEN  256
#define WSASYS_STATUS_LEN   128

typedef struct WSAData
{
    WORD           wVersion;
    WORD           wHighVersion;
    unsigned short iMaxSockets;
    unsigned short iMaxUdpDg;
    char           lpVendorInfo;
    char           szDescription[WSADESCRIPTION_LEN + 1];
    char           szSystemStatus[WSASYS_STATUS_LEN + 1];
} WSADATA;

//
// -----------------------------------------------------------------------
// Function declarations
// -----------------------------------------------------------------------
//

// Process
WINBASEAPI __declspec(noreturn) void WINAPI ExitProcess(UINT exit_code);
WINBASEAPI HANDLE WINAPI GetModuleHandleW(const WCHAR* lpModuleName);

// Thread
WINBASEAPI HANDLE WINAPI CreateThread(LPSECURITY_ATTRIBUTES lpThreadAttributes, ULONG_PTR dwStackSize, LPTHREAD_START_ROUTINE lpStartAddress, void* lpParameter, DWORD dwCreationFlags, DWORD* lpThreadId);
WINBASEAPI DWORD  WINAPI WaitForSingleObject(HANDLE hHandle, DWORD dwMilliseconds);
WINBASEAPI BOOL   WINAPI CloseHandle(HANDLE hObject);
WINBASEAPI __declspec(noreturn) void WINAPI ExitThread(DWORD dwExitCode);
WINBASEAPI VOID   WINAPI Sleep(DWORD dwMilliseconds);

// Memory
WINBASEAPI LPVOID WINAPI VirtualAlloc(LPVOID lpAddress, SIZE_T dwSize, DWORD flAllocationType, DWORD flProtect);
WINBASEAPI BOOL   WINAPI VirtualFree(LPVOID lpAddress, SIZE_T dwSize, DWORD dwFreeType);

// File
WINBASEAPI HANDLE WINAPI CreateFileW(const WCHAR* lpFileName, DWORD dwDesiredAccess, DWORD dwShareMode, LPSECURITY_ATTRIBUTES lpSecurityAttributes, DWORD dwCreationDisposition, DWORD dwFlagsAndAttributes, HANDLE hTemplateFile);
WINBASEAPI DWORD  WINAPI GetFileSize(HANDLE hFile, DWORD* lpFileSizeHigh);
WINBASEAPI BOOL   WINAPI ReadFile(HANDLE hFile, void* lpBuffer, DWORD nNumberOfBytesToRead, DWORD* lpNumberOfBytesRead, LPOVERLAPPED lpOverlapped);
WINBASEAPI BOOL   WINAPI WriteFile(HANDLE hFile, void* lpBuffer, DWORD nNumberOfBytesToWrite, DWORD* lpNumberOfBytesWritten, LPOVERLAPPED lpOverlapped);
WINBASEAPI BOOL   WINAPI CopyFileW(const WCHAR* lpExistingFileName, const WCHAR* lpNewFileName, BOOL bFailIfExists);
WINBASEAPI BOOL   WINAPI GetFileAttributesExW(const WCHAR* lpFileName, GET_FILEEX_INFO_LEVELS fInfoLevelId, LPVOID lpFileInformation);
WINBASEAPI LONG   WINAPI CompareFileTime(const FILETIME* lpFileTime1, const FILETIME* lpFileTime2);

// Library
WINBASEAPI HMODULE  WINAPI LoadLibraryW(const WCHAR* lpLibFileName);
WINBASEAPI BOOL     WINAPI FreeLibrary(HMODULE hLibModule);
WINBASEAPI FARPROC  WINAPI GetProcAddress(HMODULE hModule, const CHAR* lpProcName);

// Console
WINBASEAPI HANDLE WINAPI GetStdHandle(DWORD nStdHandle);
WINBASEAPI BOOL   WINAPI AllocConsole(VOID);
WINBASEAPI BOOL   WINAPI WriteConsoleW(HANDLE hConsoleOutput, const VOID* lpBuffer, DWORD nNumberOfCharsToWrite, DWORD* lpNumberOfCharsWritten, LPVOID lpReserved);

// Time
WINBASEAPI BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER* lpPerformanceCount);
WINBASEAPI BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER* lpFrequency);

// NT
__declspec(dllimport) LONG NTAPI NtDelayExecution(BOOLEAN Alertable, LARGE_INTEGER* DelayInterval);

// Window class & window
WINUSERAPI ATOM   WINAPI RegisterClassExW(const WNDCLASSEXW*);
WINUSERAPI HWND   WINAPI CreateWindowExW(DWORD dwExStyle, const WCHAR* lpClassName, const WCHAR* lpWindowName, DWORD dwStyle, int X, int Y, int nWidth, int nHeight, HWND hWndParent, HMENU hMenu, HINSTANCE hInstance, void* lpParam);
WINUSERAPI BOOL   WINAPI DestroyWindow(HWND hWnd);
WINUSERAPI BOOL   WINAPI AdjustWindowRectEx(RECT* lpRect, DWORD dwStyle, BOOL bMenu, DWORD dwExStyle);
WINUSERAPI BOOL   WINAPI GetClientRect(HWND hWnd, RECT* lpRect);
WINUSERAPI BOOL   WINAPI GetWindowRect(HWND hWnd, RECT* lpRect);
WINUSERAPI int    WINAPI GetSystemMetrics(int nIndex);
WINUSERAPI BOOL   WINAPI SetWindowPos(HWND hWnd, HWND hWndInsertAfter, int X, int Y, int cx, int cy, UINT uFlags);
WINUSERAPI LONG_PTR WINAPI GetWindowLongPtrW(HWND hWnd, int nIndex);
WINUSERAPI LONG_PTR WINAPI SetWindowLongPtrW(HWND hWnd, int nIndex, LONG_PTR dwNewLong);

// Cursor & icon
WINUSERAPI HCURSOR WINAPI LoadCursorW(HINSTANCE hInstance, const WCHAR* lpCursorName);
WINUSERAPI HICON   WINAPI LoadIconW(HINSTANCE hInstance, const WCHAR* lpIconName);

// Message loop
WINUSERAPI LRESULT WINAPI DefWindowProcW(HWND hWnd, UINT Msg, WPARAM wParam, LPARAM lParam);
WINUSERAPI BOOL    WINAPI GetMessageW(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax);
WINUSERAPI BOOL    WINAPI PeekMessageW(MSG* lpMsg, HWND hWnd, UINT wMsgFilterMin, UINT wMsgFilterMax, UINT wRemoveMsg);
WINUSERAPI BOOL    WINAPI TranslateMessage(const MSG* lpMsg);
WINUSERAPI LRESULT WINAPI DispatchMessageW(const MSG* lpMsg);
WINUSERAPI void    WINAPI PostQuitMessage(int nExitCode);
WINUSERAPI BOOL    WINAPI PostThreadMessageW(DWORD idThread, UINT Msg, WPARAM wParam, LPARAM lParam);

// Paint
WINUSERAPI HDC  WINAPI BeginPaint(HWND hWnd, LPPAINTSTRUCT lpPaint);
WINUSERAPI BOOL WINAPI EndPaint(HWND hWnd, const PAINTSTRUCT* lpPaint);
WINUSERAPI BOOL WINAPI InvalidateRect(HWND hWnd, const RECT* lpRect, BOOL bErase);
WINUSERAPI BOOL WINAPI UpdateWindow(HWND hWnd);
WINUSERAPI int  WINAPI FillRect(HDC hDC, const RECT* lprc, HBRUSH hbr);

// DC
WINUSERAPI HDC  WINAPI GetDC(HWND hWnd);
WINUSERAPI int  WINAPI ReleaseDC(HWND hWnd, HDC hDC);

// GDI
WINGDIAPI HDC     WINAPI CreateCompatibleDC(HDC hdc);
WINGDIAPI HBITMAP WINAPI CreateDIBSection(HDC hdc, const BITMAPINFO* pbmi, UINT usage, void** ppvBits, HANDLE hSection, DWORD offset);
WINGDIAPI HGDIOBJ WINAPI SelectObject(HDC hdc, HGDIOBJ h);
WINGDIAPI BOOL    WINAPI DeleteObject(HGDIOBJ ho);
WINGDIAPI BOOL    WINAPI DeleteDC(HDC hdc);
WINGDIAPI BOOL    WINAPI StretchBlt(HDC hdcDest, int xDest, int yDest, int wDest, int hDest, HDC hdcSrc, int xSrc, int ySrc, int wSrc, int hSrc, DWORD rop);
WINGDIAPI BOOL    WINAPI PatBlt(HDC hdc, int x, int y, int w, int h, DWORD rop);

// Winsock
__declspec(dllimport) int     WSAAPI WSAStartup(WORD wVersionRequested, WSADATA* lpWSAData);
__declspec(dllimport) int     WSAAPI WSACleanup(void);
__declspec(dllimport) SOCKET  WSAAPI socket(int af, int type, int protocol);
__declspec(dllimport) int     WSAAPI bind(SOCKET s, SOCKADDR* name, int namelen);
__declspec(dllimport) int     WSAAPI getsockname(SOCKET s, SOCKADDR* name, int* namelen);
__declspec(dllimport) int     WSAAPI sendto(SOCKET s, const char* buf, int len, int flags, const SOCKADDR* to, int tolen);
__declspec(dllimport) int     WSAAPI recvfrom(SOCKET s, char* buf, int len, int flags, SOCKADDR* from, int* fromlen);
__declspec(dllimport) int     WSAAPI ioctlsocket(SOCKET s, long cmd, u_long* argp);
__declspec(dllimport) u_short WSAAPI htons(u_short hostshort);
__declspec(dllimport) u_long  WSAAPI htonl(u_long hostlong);
__declspec(dllimport) u_short WSAAPI ntohs(u_short netshort);
__declspec(dllimport) u_long  WSAAPI ntohl(u_long netlong);

#pragma once

// Requires windows.h to be included before this file.
// windows.h provides: HRESULT, DWORD, WORD, BYTE, WCHAR, WINAPI etc.

//
// Calling convention
//

#define STDMETHODCALLTYPE __stdcall

//
// Interface helpers
//

#ifndef interface
#define interface struct
#endif

#define FWD_DECLARE(x) typedef interface x x

FWD_DECLARE(IXAudio2);
FWD_DECLARE(IXAudio2Voice);
FWD_DECLARE(IXAudio2SourceVoice);
FWD_DECLARE(IXAudio2SubmixVoice);
FWD_DECLARE(IXAudio2MasteringVoice);
FWD_DECLARE(IXAudio2EngineCallback);
FWD_DECLARE(IXAudio2VoiceCallback);

//
// UINT32
//

#ifndef _UINT32_DEFINED
#define _UINT32_DEFINED
typedef unsigned int UINT32, *PUINT32;
#endif

//
// AudioStreamCategory
//

typedef enum AUDIO_STREAM_CATEGORY
{
    AudioCategory_Other               = 0,
    AudioCategory_ForegroundOnlyMedia = 1,
    AudioCategory_Communications      = 3,
    AudioCategory_Alerts              = 4,
    AudioCategory_SoundEffects        = 5,
    AudioCategory_GameEffects         = 6,
    AudioCategory_GameMedia           = 7,
    AudioCategory_GameChat            = 8,
    AudioCategory_Speech              = 9,
    AudioCategory_Movie               = 10,
    AudioCategory_Media               = 11,
} AUDIO_STREAM_CATEGORY;

//
// XAudio2 processor
//

typedef UINT32 XAUDIO2_PROCESSOR;
#define Processor1                0x00000001
#define XAUDIO2_DEFAULT_PROCESSOR Processor1

//
// XAudio2 constants
//

#define XAUDIO2_COMMIT_NOW            0
#define XAUDIO2_COMMIT_ALL            0
#define XAUDIO2_INVALID_OPSET         (UINT32)(-1)
#define XAUDIO2_NO_LOOP_REGION        0
#define XAUDIO2_LOOP_INFINITE         255
#define XAUDIO2_DEFAULT_CHANNELS      0
#define XAUDIO2_DEFAULT_SAMPLERATE    0
#define XAUDIO2_DEFAULT_FREQ_RATIO    2.0f
#define XAUDIO2_MAX_FREQ_RATIO        1024.0f
#define XAUDIO2_END_OF_STREAM         0x0040
#define XAUDIO2_SEND_USEFILTER        0x0080
#define XAUDIO2_VOICE_NOPITCH         0x0002
#define XAUDIO2_VOICE_NOSRC           0x0004
#define XAUDIO2_VOICE_USEFILTER       0x0008
#define XAUDIO2_DEBUG_ENGINE          0x0001
#define XAUDIO2_VOICE_NOSAMPLESPLAYED 0x0100

//
// WAVEFORMATEX
//

#define WAVE_FORMAT_PCM 1

typedef struct tWAVEFORMATEX
{
    WORD  wFormatTag;
    WORD  nChannels;
    DWORD nSamplesPerSec;
    DWORD nAvgBytesPerSec;
    WORD  nBlockAlign;
    WORD  wBitsPerSample;
    WORD  cbSize;
} WAVEFORMATEX;

//
// XAudio2 data structs
//

typedef struct XAUDIO2_VOICE_DETAILS
{
    UINT32 CreationFlags;
    UINT32 ActiveFlags;
    UINT32 InputChannels;
    UINT32 InputSampleRate;
} XAUDIO2_VOICE_DETAILS;

typedef struct XAUDIO2_SEND_DESCRIPTOR
{
    UINT32         Flags;
    IXAudio2Voice* pOutputVoice;
} XAUDIO2_SEND_DESCRIPTOR;

typedef struct XAUDIO2_VOICE_SENDS
{
    UINT32                   SendCount;
    XAUDIO2_SEND_DESCRIPTOR* pSends;
} XAUDIO2_VOICE_SENDS;

typedef struct XAUDIO2_EFFECT_DESCRIPTOR
{
    void*  pEffect;
    int    InitialState;
    UINT32 OutputChannels;
} XAUDIO2_EFFECT_DESCRIPTOR;

typedef struct XAUDIO2_EFFECT_CHAIN
{
    UINT32                     EffectCount;
    XAUDIO2_EFFECT_DESCRIPTOR* pEffectDescriptors;
} XAUDIO2_EFFECT_CHAIN;

typedef enum XAUDIO2_FILTER_TYPE
{
    LowPassFilter         = 0,
    BandPassFilter        = 1,
    HighPassFilter        = 2,
    NotchFilter           = 3,
    LowPassOnePoleFilter  = 4,
    HighPassOnePoleFilter = 5,
} XAUDIO2_FILTER_TYPE;

typedef struct XAUDIO2_FILTER_PARAMETERS
{
    XAUDIO2_FILTER_TYPE Type;
    float               Frequency;
    float               OneOverQ;
} XAUDIO2_FILTER_PARAMETERS;

typedef struct XAUDIO2_BUFFER
{
    UINT32       Flags;
    UINT32       AudioBytes;
    const BYTE*  pAudioData;
    UINT32       PlayBegin;
    UINT32       PlayLength;
    UINT32       LoopBegin;
    UINT32       LoopLength;
    UINT32       LoopCount;
    void*        pContext;
} XAUDIO2_BUFFER;

typedef struct XAUDIO2_BUFFER_WMA
{
    const UINT32* pDecodedPacketCumulativeBytes;
    UINT32        PacketCount;
} XAUDIO2_BUFFER_WMA;

typedef struct XAUDIO2_VOICE_STATE
{
    void*            pCurrentBufferContext;
    UINT32           BuffersQueued;
    unsigned __int64 SamplesPlayed;
} XAUDIO2_VOICE_STATE;

typedef struct XAUDIO2_PERFORMANCE_DATA
{
    unsigned __int64 AudioCyclesSinceLastQuery;
    unsigned __int64 TotalCyclesSinceLastQuery;
    UINT32           MinimumCyclesPerQuantum;
    UINT32           MaximumCyclesPerQuantum;
    UINT32           MemoryUsageInBytes;
    UINT32           CurrentLatencyInSamples;
    UINT32           GlitchesSinceEngineStarted;
    UINT32           ActiveSourceVoiceCount;
    UINT32           TotalSourceVoiceCount;
    UINT32           ActiveSubmixVoiceCount;
    UINT32           ActiveResamplerCount;
    UINT32           ActiveMatrixMixCount;
    UINT32           ActiveXmaSourceVoices;
    UINT32           ActiveXmaStreams;
} XAUDIO2_PERFORMANCE_DATA;

typedef struct XAUDIO2_DEBUG_CONFIGURATION
{
    UINT32 TraceMask;
    UINT32 BreakMask;
    int    LogThreadID;
    int    LogFileline;
    int    LogFunctionName;
    int    LogTiming;
} XAUDIO2_DEBUG_CONFIGURATION;

// -----------------------------------------------------------------------
// IXAudio2VoiceCallback
// -----------------------------------------------------------------------

typedef struct IXAudio2VoiceCallbackVtbl
{
    void (STDMETHODCALLTYPE* OnVoiceProcessingPassStart)(IXAudio2VoiceCallback* This, UINT32 BytesRequired);
    void (STDMETHODCALLTYPE* OnVoiceProcessingPassEnd  )(IXAudio2VoiceCallback* This);
    void (STDMETHODCALLTYPE* OnStreamEnd               )(IXAudio2VoiceCallback* This);
    void (STDMETHODCALLTYPE* OnBufferStart             )(IXAudio2VoiceCallback* This, void* pBufferContext);
    void (STDMETHODCALLTYPE* OnBufferEnd               )(IXAudio2VoiceCallback* This, void* pBufferContext);
    void (STDMETHODCALLTYPE* OnLoopEnd                 )(IXAudio2VoiceCallback* This, void* pBufferContext);
    void (STDMETHODCALLTYPE* OnVoiceError              )(IXAudio2VoiceCallback* This, void* pBufferContext, HRESULT Error);
} IXAudio2VoiceCallbackVtbl;

struct IXAudio2VoiceCallback
{
    IXAudio2VoiceCallbackVtbl* lpVtbl;
};

// -----------------------------------------------------------------------
// IXAudio2EngineCallback
// -----------------------------------------------------------------------

typedef struct IXAudio2EngineCallbackVtbl
{
    void (STDMETHODCALLTYPE* OnProcessingPassStart)(IXAudio2EngineCallback* This);
    void (STDMETHODCALLTYPE* OnProcessingPassEnd  )(IXAudio2EngineCallback* This);
    void (STDMETHODCALLTYPE* OnCriticalError      )(IXAudio2EngineCallback* This, HRESULT Error);
} IXAudio2EngineCallbackVtbl;

struct IXAudio2EngineCallback
{
    IXAudio2EngineCallbackVtbl* lpVtbl;
};

// -----------------------------------------------------------------------
// IXAudio2Voice  (base)
// -----------------------------------------------------------------------

typedef struct IXAudio2VoiceVtbl
{
    void    (STDMETHODCALLTYPE* GetVoiceDetails           )(IXAudio2Voice*, XAUDIO2_VOICE_DETAILS*);
    HRESULT (STDMETHODCALLTYPE* SetOutputVoices           )(IXAudio2Voice*, const XAUDIO2_VOICE_SENDS*);
    HRESULT (STDMETHODCALLTYPE* SetEffectChain            )(IXAudio2Voice*, const XAUDIO2_EFFECT_CHAIN*);
    HRESULT (STDMETHODCALLTYPE* EnableEffect              )(IXAudio2Voice*, UINT32, UINT32);
    HRESULT (STDMETHODCALLTYPE* DisableEffect             )(IXAudio2Voice*, UINT32, UINT32);
    void    (STDMETHODCALLTYPE* GetEffectState            )(IXAudio2Voice*, UINT32, int*);
    HRESULT (STDMETHODCALLTYPE* SetEffectParameters      )(IXAudio2Voice*, UINT32, const void*, UINT32, UINT32);
    HRESULT (STDMETHODCALLTYPE* GetEffectParameters      )(IXAudio2Voice*, UINT32, void*, UINT32);
    HRESULT (STDMETHODCALLTYPE* SetFilterParameters      )(IXAudio2Voice*, const XAUDIO2_FILTER_PARAMETERS*, UINT32);
    void    (STDMETHODCALLTYPE* GetFilterParameters      )(IXAudio2Voice*, XAUDIO2_FILTER_PARAMETERS*);
    HRESULT (STDMETHODCALLTYPE* SetOutputFilterParameters)(IXAudio2Voice*, IXAudio2Voice*, const XAUDIO2_FILTER_PARAMETERS*, UINT32);
    void    (STDMETHODCALLTYPE* GetOutputFilterParameters)(IXAudio2Voice*, IXAudio2Voice*, XAUDIO2_FILTER_PARAMETERS*);
    HRESULT (STDMETHODCALLTYPE* SetVolume                )(IXAudio2Voice*, float, UINT32);
    void    (STDMETHODCALLTYPE* GetVolume                )(IXAudio2Voice*, float*);
    HRESULT (STDMETHODCALLTYPE* SetChannelVolumes        )(IXAudio2Voice*, UINT32, const float*, UINT32);
    void    (STDMETHODCALLTYPE* GetChannelVolumes        )(IXAudio2Voice*, UINT32, float*);
    HRESULT (STDMETHODCALLTYPE* SetOutputMatrix          )(IXAudio2Voice*, IXAudio2Voice*, UINT32, UINT32, const float*, UINT32);
    void    (STDMETHODCALLTYPE* GetOutputMatrix          )(IXAudio2Voice*, IXAudio2Voice*, UINT32, UINT32, float*);
    void    (STDMETHODCALLTYPE* DestroyVoice             )(IXAudio2Voice*);
} IXAudio2VoiceVtbl;

struct IXAudio2Voice
{
    IXAudio2VoiceVtbl* lpVtbl;
};

#define IXAudio2Voice_GetVoiceDetails(This,pVoiceDetails)                                                                          ((This)->lpVtbl->GetVoiceDetails(This,pVoiceDetails))
#define IXAudio2Voice_SetOutputVoices(This,pSendList)                                                                              ((This)->lpVtbl->SetOutputVoices(This,pSendList))
#define IXAudio2Voice_SetEffectChain(This,pEffectChain)                                                                            ((This)->lpVtbl->SetEffectChain(This,pEffectChain))
#define IXAudio2Voice_EnableEffect(This,EffectIndex,OperationSet)                                                                  ((This)->lpVtbl->EnableEffect(This,EffectIndex,OperationSet))
#define IXAudio2Voice_DisableEffect(This,EffectIndex,OperationSet)                                                                 ((This)->lpVtbl->DisableEffect(This,EffectIndex,OperationSet))
#define IXAudio2Voice_GetEffectState(This,EffectIndex,pEnabled)                                                                    ((This)->lpVtbl->GetEffectState(This,EffectIndex,pEnabled))
#define IXAudio2Voice_SetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize,OperationSet)                            ((This)->lpVtbl->SetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize,OperationSet))
#define IXAudio2Voice_GetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize)                                         ((This)->lpVtbl->GetEffectParameters(This,EffectIndex,pParameters,ParametersByteSize))
#define IXAudio2Voice_SetFilterParameters(This,pParameters,OperationSet)                                                           ((This)->lpVtbl->SetFilterParameters(This,pParameters,OperationSet))
#define IXAudio2Voice_GetFilterParameters(This,pParameters)                                                                        ((This)->lpVtbl->GetFilterParameters(This,pParameters))
#define IXAudio2Voice_SetOutputFilterParameters(This,pDestinationVoice,pParameters,OperationSet)                                   ((This)->lpVtbl->SetOutputFilterParameters(This,pDestinationVoice,pParameters,OperationSet))
#define IXAudio2Voice_GetOutputFilterParameters(This,pDestinationVoice,pParameters)                                                ((This)->lpVtbl->GetOutputFilterParameters(This,pDestinationVoice,pParameters))
#define IXAudio2Voice_SetVolume(This,Volume,OperationSet)                                                                          ((This)->lpVtbl->SetVolume(This,Volume,OperationSet))
#define IXAudio2Voice_GetVolume(This,pVolume)                                                                                      ((This)->lpVtbl->GetVolume(This,pVolume))
#define IXAudio2Voice_SetChannelVolumes(This,Channels,pVolumes,OperationSet)                                                       ((This)->lpVtbl->SetChannelVolumes(This,Channels,pVolumes,OperationSet))
#define IXAudio2Voice_GetChannelVolumes(This,Channels,pVolumes)                                                                    ((This)->lpVtbl->GetChannelVolumes(This,Channels,pVolumes))
#define IXAudio2Voice_SetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix,OperationSet)         ((This)->lpVtbl->SetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix,OperationSet))
#define IXAudio2Voice_GetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix)                      ((This)->lpVtbl->GetOutputMatrix(This,pDestinationVoice,SourceChannels,DestinationChannels,pLevelMatrix))
#define IXAudio2Voice_DestroyVoice(This)                                                                                           ((This)->lpVtbl->DestroyVoice(This))

// -----------------------------------------------------------------------
// IXAudio2SourceVoice
// -----------------------------------------------------------------------

typedef struct IXAudio2SourceVoiceVtbl
{
    void    (STDMETHODCALLTYPE* GetVoiceDetails           )(IXAudio2SourceVoice*, XAUDIO2_VOICE_DETAILS*);
    HRESULT (STDMETHODCALLTYPE* SetOutputVoices           )(IXAudio2SourceVoice*, const XAUDIO2_VOICE_SENDS*);
    HRESULT (STDMETHODCALLTYPE* SetEffectChain            )(IXAudio2SourceVoice*, const XAUDIO2_EFFECT_CHAIN*);
    HRESULT (STDMETHODCALLTYPE* EnableEffect              )(IXAudio2SourceVoice*, UINT32, UINT32);
    HRESULT (STDMETHODCALLTYPE* DisableEffect             )(IXAudio2SourceVoice*, UINT32, UINT32);
    void    (STDMETHODCALLTYPE* GetEffectState            )(IXAudio2SourceVoice*, UINT32, int*);
    HRESULT (STDMETHODCALLTYPE* SetEffectParameters      )(IXAudio2SourceVoice*, UINT32, const void*, UINT32, UINT32);
    HRESULT (STDMETHODCALLTYPE* GetEffectParameters      )(IXAudio2SourceVoice*, UINT32, void*, UINT32);
    HRESULT (STDMETHODCALLTYPE* SetFilterParameters      )(IXAudio2SourceVoice*, const XAUDIO2_FILTER_PARAMETERS*, UINT32);
    void    (STDMETHODCALLTYPE* GetFilterParameters      )(IXAudio2SourceVoice*, XAUDIO2_FILTER_PARAMETERS*);
    HRESULT (STDMETHODCALLTYPE* SetOutputFilterParameters)(IXAudio2SourceVoice*, IXAudio2Voice*, const XAUDIO2_FILTER_PARAMETERS*, UINT32);
    void    (STDMETHODCALLTYPE* GetOutputFilterParameters)(IXAudio2SourceVoice*, IXAudio2Voice*, XAUDIO2_FILTER_PARAMETERS*);
    HRESULT (STDMETHODCALLTYPE* SetVolume                )(IXAudio2SourceVoice*, float, UINT32);
    void    (STDMETHODCALLTYPE* GetVolume                )(IXAudio2SourceVoice*, float*);
    HRESULT (STDMETHODCALLTYPE* SetChannelVolumes        )(IXAudio2SourceVoice*, UINT32, const float*, UINT32);
    void    (STDMETHODCALLTYPE* GetChannelVolumes        )(IXAudio2SourceVoice*, UINT32, float*);
    HRESULT (STDMETHODCALLTYPE* SetOutputMatrix          )(IXAudio2SourceVoice*, IXAudio2Voice*, UINT32, UINT32, const float*, UINT32);
    void    (STDMETHODCALLTYPE* GetOutputMatrix          )(IXAudio2SourceVoice*, IXAudio2Voice*, UINT32, UINT32, float*);
    void    (STDMETHODCALLTYPE* DestroyVoice             )(IXAudio2SourceVoice*);
    HRESULT (STDMETHODCALLTYPE* Start              )(IXAudio2SourceVoice*, UINT32 Flags, UINT32 OperationSet);
    HRESULT (STDMETHODCALLTYPE* Stop               )(IXAudio2SourceVoice*, UINT32 Flags, UINT32 OperationSet);
    HRESULT (STDMETHODCALLTYPE* SubmitSourceBuffer )(IXAudio2SourceVoice*, const XAUDIO2_BUFFER*, const XAUDIO2_BUFFER_WMA*);
    HRESULT (STDMETHODCALLTYPE* FlushSourceBuffers )(IXAudio2SourceVoice*);
    HRESULT (STDMETHODCALLTYPE* Discontinuity      )(IXAudio2SourceVoice*);
    HRESULT (STDMETHODCALLTYPE* ExitLoop           )(IXAudio2SourceVoice*, UINT32 OperationSet);
    void    (STDMETHODCALLTYPE* GetState           )(IXAudio2SourceVoice*, XAUDIO2_VOICE_STATE*, UINT32 Flags);
    HRESULT (STDMETHODCALLTYPE* SetFrequencyRatio  )(IXAudio2SourceVoice*, float Ratio, UINT32 OperationSet);
    void    (STDMETHODCALLTYPE* GetFrequencyRatio  )(IXAudio2SourceVoice*, float* pRatio);
    HRESULT (STDMETHODCALLTYPE* SetSourceSampleRate)(IXAudio2SourceVoice*, UINT32 NewSourceSampleRate);
} IXAudio2SourceVoiceVtbl;

struct IXAudio2SourceVoice
{
    IXAudio2SourceVoiceVtbl* lpVtbl;
};

#define IXAudio2SourceVoice_GetVoiceDetails           IXAudio2Voice_GetVoiceDetails
#define IXAudio2SourceVoice_SetOutputVoices           IXAudio2Voice_SetOutputVoices
#define IXAudio2SourceVoice_SetEffectChain            IXAudio2Voice_SetEffectChain
#define IXAudio2SourceVoice_EnableEffect              IXAudio2Voice_EnableEffect
#define IXAudio2SourceVoice_DisableEffect             IXAudio2Voice_DisableEffect
#define IXAudio2SourceVoice_GetEffectState            IXAudio2Voice_GetEffectState
#define IXAudio2SourceVoice_SetEffectParameters       IXAudio2Voice_SetEffectParameters
#define IXAudio2SourceVoice_GetEffectParameters       IXAudio2Voice_GetEffectParameters
#define IXAudio2SourceVoice_SetFilterParameters       IXAudio2Voice_SetFilterParameters
#define IXAudio2SourceVoice_GetFilterParameters       IXAudio2Voice_GetFilterParameters
#define IXAudio2SourceVoice_SetOutputFilterParameters IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2SourceVoice_GetOutputFilterParameters IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2SourceVoice_SetVolume                 IXAudio2Voice_SetVolume
#define IXAudio2SourceVoice_GetVolume                 IXAudio2Voice_GetVolume
#define IXAudio2SourceVoice_SetChannelVolumes         IXAudio2Voice_SetChannelVolumes
#define IXAudio2SourceVoice_GetChannelVolumes         IXAudio2Voice_GetChannelVolumes
#define IXAudio2SourceVoice_SetOutputMatrix           IXAudio2Voice_SetOutputMatrix
#define IXAudio2SourceVoice_GetOutputMatrix           IXAudio2Voice_GetOutputMatrix
#define IXAudio2SourceVoice_DestroyVoice              IXAudio2Voice_DestroyVoice
#define IXAudio2SourceVoice_Start(This,Flags,OperationSet)                  ((This)->lpVtbl->Start(This,Flags,OperationSet))
#define IXAudio2SourceVoice_Stop(This,Flags,OperationSet)                   ((This)->lpVtbl->Stop(This,Flags,OperationSet))
#define IXAudio2SourceVoice_SubmitSourceBuffer(This,pBuffer,pBufferWMA)     ((This)->lpVtbl->SubmitSourceBuffer(This,pBuffer,pBufferWMA))
#define IXAudio2SourceVoice_FlushSourceBuffers(This)                        ((This)->lpVtbl->FlushSourceBuffers(This))
#define IXAudio2SourceVoice_Discontinuity(This)                             ((This)->lpVtbl->Discontinuity(This))
#define IXAudio2SourceVoice_ExitLoop(This,OperationSet)                     ((This)->lpVtbl->ExitLoop(This,OperationSet))
#define IXAudio2SourceVoice_GetState(This,pVoiceState,Flags)                ((This)->lpVtbl->GetState(This,pVoiceState,Flags))
#define IXAudio2SourceVoice_SetFrequencyRatio(This,Ratio,OperationSet)      ((This)->lpVtbl->SetFrequencyRatio(This,Ratio,OperationSet))
#define IXAudio2SourceVoice_GetFrequencyRatio(This,pRatio)                  ((This)->lpVtbl->GetFrequencyRatio(This,pRatio))
#define IXAudio2SourceVoice_SetSourceSampleRate(This,NewSourceSampleRate)   ((This)->lpVtbl->SetSourceSampleRate(This,NewSourceSampleRate))

// -----------------------------------------------------------------------
// IXAudio2SubmixVoice
// -----------------------------------------------------------------------

typedef struct IXAudio2SubmixVoiceVtbl
{
    void    (STDMETHODCALLTYPE* GetVoiceDetails           )(IXAudio2SubmixVoice*, XAUDIO2_VOICE_DETAILS*);
    HRESULT (STDMETHODCALLTYPE* SetOutputVoices           )(IXAudio2SubmixVoice*, const XAUDIO2_VOICE_SENDS*);
    HRESULT (STDMETHODCALLTYPE* SetEffectChain            )(IXAudio2SubmixVoice*, const XAUDIO2_EFFECT_CHAIN*);
    HRESULT (STDMETHODCALLTYPE* EnableEffect              )(IXAudio2SubmixVoice*, UINT32, UINT32);
    HRESULT (STDMETHODCALLTYPE* DisableEffect             )(IXAudio2SubmixVoice*, UINT32, UINT32);
    void    (STDMETHODCALLTYPE* GetEffectState            )(IXAudio2SubmixVoice*, UINT32, int*);
    HRESULT (STDMETHODCALLTYPE* SetEffectParameters      )(IXAudio2SubmixVoice*, UINT32, const void*, UINT32, UINT32);
    HRESULT (STDMETHODCALLTYPE* GetEffectParameters      )(IXAudio2SubmixVoice*, UINT32, void*, UINT32);
    HRESULT (STDMETHODCALLTYPE* SetFilterParameters      )(IXAudio2SubmixVoice*, const XAUDIO2_FILTER_PARAMETERS*, UINT32);
    void    (STDMETHODCALLTYPE* GetFilterParameters      )(IXAudio2SubmixVoice*, XAUDIO2_FILTER_PARAMETERS*);
    HRESULT (STDMETHODCALLTYPE* SetOutputFilterParameters)(IXAudio2SubmixVoice*, IXAudio2Voice*, const XAUDIO2_FILTER_PARAMETERS*, UINT32);
    void    (STDMETHODCALLTYPE* GetOutputFilterParameters)(IXAudio2SubmixVoice*, IXAudio2Voice*, XAUDIO2_FILTER_PARAMETERS*);
    HRESULT (STDMETHODCALLTYPE* SetVolume                )(IXAudio2SubmixVoice*, float, UINT32);
    void    (STDMETHODCALLTYPE* GetVolume                )(IXAudio2SubmixVoice*, float*);
    HRESULT (STDMETHODCALLTYPE* SetChannelVolumes        )(IXAudio2SubmixVoice*, UINT32, const float*, UINT32);
    void    (STDMETHODCALLTYPE* GetChannelVolumes        )(IXAudio2SubmixVoice*, UINT32, float*);
    HRESULT (STDMETHODCALLTYPE* SetOutputMatrix          )(IXAudio2SubmixVoice*, IXAudio2Voice*, UINT32, UINT32, const float*, UINT32);
    void    (STDMETHODCALLTYPE* GetOutputMatrix          )(IXAudio2SubmixVoice*, IXAudio2Voice*, UINT32, UINT32, float*);
    void    (STDMETHODCALLTYPE* DestroyVoice             )(IXAudio2SubmixVoice*);
} IXAudio2SubmixVoiceVtbl;

struct IXAudio2SubmixVoice
{
    IXAudio2SubmixVoiceVtbl* lpVtbl;
};

#define IXAudio2SubmixVoice_GetVoiceDetails           IXAudio2Voice_GetVoiceDetails
#define IXAudio2SubmixVoice_SetOutputVoices           IXAudio2Voice_SetOutputVoices
#define IXAudio2SubmixVoice_SetEffectChain            IXAudio2Voice_SetEffectChain
#define IXAudio2SubmixVoice_EnableEffect              IXAudio2Voice_EnableEffect
#define IXAudio2SubmixVoice_DisableEffect             IXAudio2Voice_DisableEffect
#define IXAudio2SubmixVoice_GetEffectState            IXAudio2Voice_GetEffectState
#define IXAudio2SubmixVoice_SetEffectParameters       IXAudio2Voice_SetEffectParameters
#define IXAudio2SubmixVoice_GetEffectParameters       IXAudio2Voice_GetEffectParameters
#define IXAudio2SubmixVoice_SetFilterParameters       IXAudio2Voice_SetFilterParameters
#define IXAudio2SubmixVoice_GetFilterParameters       IXAudio2Voice_GetFilterParameters
#define IXAudio2SubmixVoice_SetOutputFilterParameters IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2SubmixVoice_GetOutputFilterParameters IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2SubmixVoice_SetVolume                 IXAudio2Voice_SetVolume
#define IXAudio2SubmixVoice_GetVolume                 IXAudio2Voice_GetVolume
#define IXAudio2SubmixVoice_SetChannelVolumes         IXAudio2Voice_SetChannelVolumes
#define IXAudio2SubmixVoice_GetChannelVolumes         IXAudio2Voice_GetChannelVolumes
#define IXAudio2SubmixVoice_SetOutputMatrix           IXAudio2Voice_SetOutputMatrix
#define IXAudio2SubmixVoice_GetOutputMatrix           IXAudio2Voice_GetOutputMatrix
#define IXAudio2SubmixVoice_DestroyVoice              IXAudio2Voice_DestroyVoice

// -----------------------------------------------------------------------
// IXAudio2MasteringVoice
// -----------------------------------------------------------------------

typedef struct IXAudio2MasteringVoiceVtbl
{
    void    (STDMETHODCALLTYPE* GetVoiceDetails           )(IXAudio2MasteringVoice*, XAUDIO2_VOICE_DETAILS*);
    HRESULT (STDMETHODCALLTYPE* SetOutputVoices           )(IXAudio2MasteringVoice*, const XAUDIO2_VOICE_SENDS*);
    HRESULT (STDMETHODCALLTYPE* SetEffectChain            )(IXAudio2MasteringVoice*, const XAUDIO2_EFFECT_CHAIN*);
    HRESULT (STDMETHODCALLTYPE* EnableEffect              )(IXAudio2MasteringVoice*, UINT32, UINT32);
    HRESULT (STDMETHODCALLTYPE* DisableEffect             )(IXAudio2MasteringVoice*, UINT32, UINT32);
    void    (STDMETHODCALLTYPE* GetEffectState            )(IXAudio2MasteringVoice*, UINT32, int*);
    HRESULT (STDMETHODCALLTYPE* SetEffectParameters      )(IXAudio2MasteringVoice*, UINT32, const void*, UINT32, UINT32);
    HRESULT (STDMETHODCALLTYPE* GetEffectParameters      )(IXAudio2MasteringVoice*, UINT32, void*, UINT32);
    HRESULT (STDMETHODCALLTYPE* SetFilterParameters      )(IXAudio2MasteringVoice*, const XAUDIO2_FILTER_PARAMETERS*, UINT32);
    void    (STDMETHODCALLTYPE* GetFilterParameters      )(IXAudio2MasteringVoice*, XAUDIO2_FILTER_PARAMETERS*);
    HRESULT (STDMETHODCALLTYPE* SetOutputFilterParameters)(IXAudio2MasteringVoice*, IXAudio2Voice*, const XAUDIO2_FILTER_PARAMETERS*, UINT32);
    void    (STDMETHODCALLTYPE* GetOutputFilterParameters)(IXAudio2MasteringVoice*, IXAudio2Voice*, XAUDIO2_FILTER_PARAMETERS*);
    HRESULT (STDMETHODCALLTYPE* SetVolume                )(IXAudio2MasteringVoice*, float, UINT32);
    void    (STDMETHODCALLTYPE* GetVolume                )(IXAudio2MasteringVoice*, float*);
    HRESULT (STDMETHODCALLTYPE* SetChannelVolumes        )(IXAudio2MasteringVoice*, UINT32, const float*, UINT32);
    void    (STDMETHODCALLTYPE* GetChannelVolumes        )(IXAudio2MasteringVoice*, UINT32, float*);
    HRESULT (STDMETHODCALLTYPE* SetOutputMatrix          )(IXAudio2MasteringVoice*, IXAudio2Voice*, UINT32, UINT32, const float*, UINT32);
    void    (STDMETHODCALLTYPE* GetOutputMatrix          )(IXAudio2MasteringVoice*, IXAudio2Voice*, UINT32, UINT32, float*);
    void    (STDMETHODCALLTYPE* DestroyVoice             )(IXAudio2MasteringVoice*);
    HRESULT (STDMETHODCALLTYPE* GetChannelMask           )(IXAudio2MasteringVoice*, DWORD* pChannelMask);
} IXAudio2MasteringVoiceVtbl;

struct IXAudio2MasteringVoice
{
    IXAudio2MasteringVoiceVtbl* lpVtbl;
};

#define IXAudio2MasteringVoice_GetVoiceDetails           IXAudio2Voice_GetVoiceDetails
#define IXAudio2MasteringVoice_SetOutputVoices           IXAudio2Voice_SetOutputVoices
#define IXAudio2MasteringVoice_SetEffectChain            IXAudio2Voice_SetEffectChain
#define IXAudio2MasteringVoice_EnableEffect              IXAudio2Voice_EnableEffect
#define IXAudio2MasteringVoice_DisableEffect             IXAudio2Voice_DisableEffect
#define IXAudio2MasteringVoice_GetEffectState            IXAudio2Voice_GetEffectState
#define IXAudio2MasteringVoice_SetEffectParameters       IXAudio2Voice_SetEffectParameters
#define IXAudio2MasteringVoice_GetEffectParameters       IXAudio2Voice_GetEffectParameters
#define IXAudio2MasteringVoice_SetFilterParameters       IXAudio2Voice_SetFilterParameters
#define IXAudio2MasteringVoice_GetFilterParameters       IXAudio2Voice_GetFilterParameters
#define IXAudio2MasteringVoice_SetOutputFilterParameters IXAudio2Voice_SetOutputFilterParameters
#define IXAudio2MasteringVoice_GetOutputFilterParameters IXAudio2Voice_GetOutputFilterParameters
#define IXAudio2MasteringVoice_SetVolume                 IXAudio2Voice_SetVolume
#define IXAudio2MasteringVoice_GetVolume                 IXAudio2Voice_GetVolume
#define IXAudio2MasteringVoice_SetChannelVolumes         IXAudio2Voice_SetChannelVolumes
#define IXAudio2MasteringVoice_GetChannelVolumes         IXAudio2Voice_GetChannelVolumes
#define IXAudio2MasteringVoice_SetOutputMatrix           IXAudio2Voice_SetOutputMatrix
#define IXAudio2MasteringVoice_GetOutputMatrix           IXAudio2Voice_GetOutputMatrix
#define IXAudio2MasteringVoice_DestroyVoice              IXAudio2Voice_DestroyVoice
#define IXAudio2MasteringVoice_GetChannelMask(This,pChannelMask) ((This)->lpVtbl->GetChannelMask(This,pChannelMask))

// -----------------------------------------------------------------------
// IXAudio2
// -----------------------------------------------------------------------

typedef struct IXAudio2Vtbl
{
    HRESULT (STDMETHODCALLTYPE* QueryInterface        )(IXAudio2*, void* riid, void** ppvInterface);
    ULONG   (STDMETHODCALLTYPE* AddRef                )(IXAudio2*);
    ULONG   (STDMETHODCALLTYPE* Release               )(IXAudio2*);
    HRESULT (STDMETHODCALLTYPE* RegisterForCallbacks  )(IXAudio2*, IXAudio2EngineCallback* pCallback);
    void    (STDMETHODCALLTYPE* UnregisterForCallbacks)(IXAudio2*, IXAudio2EngineCallback* pCallback);
    HRESULT (STDMETHODCALLTYPE* CreateSourceVoice     )(IXAudio2*, IXAudio2SourceVoice** ppSourceVoice, const WAVEFORMATEX* pSourceFormat, UINT32 Flags, float MaxFrequencyRatio, IXAudio2VoiceCallback* pCallback, const XAUDIO2_VOICE_SENDS* pSendList, const XAUDIO2_EFFECT_CHAIN* pEffectChain);
    HRESULT (STDMETHODCALLTYPE* CreateSubmixVoice     )(IXAudio2*, IXAudio2SubmixVoice** ppSubmixVoice, UINT32 InputChannels, UINT32 InputSampleRate, UINT32 Flags, UINT32 ProcessingStage, const XAUDIO2_VOICE_SENDS* pSendList, const XAUDIO2_EFFECT_CHAIN* pEffectChain);
    HRESULT (STDMETHODCALLTYPE* CreateMasteringVoice  )(IXAudio2*, IXAudio2MasteringVoice** ppMasteringVoice, UINT32 InputChannels, UINT32 InputSampleRate, UINT32 Flags, const WCHAR* DeviceId, const XAUDIO2_EFFECT_CHAIN* pEffectChain, AUDIO_STREAM_CATEGORY StreamCategory);
    HRESULT (STDMETHODCALLTYPE* StartEngine           )(IXAudio2*);
    void    (STDMETHODCALLTYPE* StopEngine            )(IXAudio2*);
    HRESULT (STDMETHODCALLTYPE* CommitChanges         )(IXAudio2*, UINT32 OperationSet);
    void    (STDMETHODCALLTYPE* GetPerformanceData    )(IXAudio2*, XAUDIO2_PERFORMANCE_DATA* pPerfData);
    void    (STDMETHODCALLTYPE* SetDebugConfiguration )(IXAudio2*, const XAUDIO2_DEBUG_CONFIGURATION* pDebugConfiguration, void* pReserved);
} IXAudio2Vtbl;

struct IXAudio2
{
    IXAudio2Vtbl* lpVtbl;
};

#define IXAudio2_QueryInterface(This,riid,ppvInterface)                                                                                                                                ((This)->lpVtbl->QueryInterface(This,riid,ppvInterface))
#define IXAudio2_AddRef(This)                                                                                                                                                          ((This)->lpVtbl->AddRef(This))
#define IXAudio2_Release(This)                                                                                                                                                         ((This)->lpVtbl->Release(This))
#define IXAudio2_RegisterForCallbacks(This,pCallback)                                                                                                                                  ((This)->lpVtbl->RegisterForCallbacks(This,pCallback))
#define IXAudio2_UnregisterForCallbacks(This,pCallback)                                                                                                                                ((This)->lpVtbl->UnregisterForCallbacks(This,pCallback))
#define IXAudio2_CreateSourceVoice(This,ppSourceVoice,pSourceFormat,Flags,MaxFrequencyRatio,pCallback,pSendList,pEffectChain)                                                          ((This)->lpVtbl->CreateSourceVoice(This,ppSourceVoice,pSourceFormat,Flags,MaxFrequencyRatio,pCallback,pSendList,pEffectChain))
#define IXAudio2_CreateSubmixVoice(This,ppSubmixVoice,InputChannels,InputSampleRate,Flags,ProcessingStage,pSendList,pEffectChain)                                                      ((This)->lpVtbl->CreateSubmixVoice(This,ppSubmixVoice,InputChannels,InputSampleRate,Flags,ProcessingStage,pSendList,pEffectChain))
#define IXAudio2_CreateMasteringVoice(This,ppMasteringVoice,InputChannels,InputSampleRate,Flags,DeviceId,pEffectChain,StreamCategory)                                                 ((This)->lpVtbl->CreateMasteringVoice(This,ppMasteringVoice,InputChannels,InputSampleRate,Flags,DeviceId,pEffectChain,StreamCategory))
#define IXAudio2_StartEngine(This)                                                                                                                                                     ((This)->lpVtbl->StartEngine(This))
#define IXAudio2_StopEngine(This)                                                                                                                                                      ((This)->lpVtbl->StopEngine(This))
#define IXAudio2_CommitChanges(This,OperationSet)                                                                                                                                      ((This)->lpVtbl->CommitChanges(This,OperationSet))
#define IXAudio2_GetPerformanceData(This,pPerfData)                                                                                                                                    ((This)->lpVtbl->GetPerformanceData(This,pPerfData))
#define IXAudio2_SetDebugConfiguration(This,pDebugConfiguration,pReserved)                                                                                                            ((This)->lpVtbl->SetDebugConfiguration(This,pDebugConfiguration,pReserved))

// -----------------------------------------------------------------------
// COM init
// -----------------------------------------------------------------------

typedef enum tagCOINIT
{
    COINIT_APARTMENTTHREADED = 0x2,
    COINIT_MULTITHREADED     = 0x0,
    COINIT_DISABLE_OLE1DDE   = 0x4,
    COINIT_SPEED_OVER_MEMORY = 0x8,
} COINIT;

// -----------------------------------------------------------------------
// Function declarations
// -----------------------------------------------------------------------

__declspec(dllimport) HRESULT __stdcall CoInitializeEx(void* pvReserved, DWORD dwCoInit);
__declspec(dllimport) HRESULT __stdcall XAudio2Create(IXAudio2** ppXAudio2, UINT32 Flags, XAUDIO2_PROCESSOR XAudio2Processor);
