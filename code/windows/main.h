static HINSTANCE instance = { 0 };
static HWND window = { 0 };
static HDC device_context = { 0 };

static int32 window_width = 256 * 3;
static int32 window_height = 256 * 3;

static bool32 is_running = { 0 };

static HDC memory_device_context = { 0 };
static uint32* device_bitmap_memory = 0;
