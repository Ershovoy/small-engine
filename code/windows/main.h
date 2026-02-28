static HINSTANCE instance = { 0 };
static HWND window = { 0 };
static HDC device_context = { 0 };

static int32 client_width = GAME_DEFAULT_HORIZONTAL_RESOLUTION * ((640 + GAME_DEFAULT_HORIZONTAL_RESOLUTION - 1) / GAME_DEFAULT_HORIZONTAL_RESOLUTION);
static int32 client_height = GAME_DEFAULT_VERTICAL_RESOLUTION * ((640 + GAME_DEFAULT_HORIZONTAL_RESOLUTION - 1) / GAME_DEFAULT_HORIZONTAL_RESOLUTION);

static bool32 is_running = { 0 };

static HDC memory_device_context = { 0 };
static uint32* device_bitmap_memory = 0;

static DWORD window_style = WS_OVERLAPPEDWINDOW | WS_VISIBLE;
static DWORD window_extended_style = 0;

static bool32 is_fullscreen = 0;
static RECT previous_windowed_rect = { 0 };
static LONG_PTR previous_window_style = { 0 };

static SOCKET sock = { 0 };

#define SCRATCH_BUFFER_SIZE MEGABYTES(1)
static byte scratch_buffer[SCRATCH_BUFFER_SIZE];

static int32 game_vertical_resolution = GAME_DEFAULT_VERTICAL_RESOLUTION;
static int32 game_horizontal_resoultion = GAME_DEFAULT_HORIZONTAL_RESOLUTION;
