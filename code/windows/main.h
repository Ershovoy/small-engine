static HINSTANCE instance = { 0 };
static HWND window = { 0 };
static HDC device_context = { 0 };

static int32 client_width = GAME_HORIZONTAL_RESOLUTION * 4;
static int32 client_height = GAME_VERTICAL_RESOLUTION * 4;

static bool32 is_running = { 0 };

static HDC memory_device_context = { 0 };
static uint32* device_bitmap_memory = 0;

static DWORD window_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
static DWORD window_extended_style = 0;

static bool32 is_fullscreen = 0;
static RECT previous_windowed_rect = { 0 };
static LONG_PTR previous_window_style = { 0 };

static SOCKET sock = { 0 };

#define SCRATCH_BUFFER_SIZE MEGABYTES(16)
static byte* scratch_buffer[SCRATCH_BUFFER_SIZE];
