static uint32 get_file_size_implementation(char16* file_name)
{
    uint32 result = 0;

    HANDLE file_handle = CreateFileW(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        DWORD file_size = GetFileSize(file_handle, 0);
        if(file_size != INVALID_FILE_SIZE)
        {
            result = file_size;
        }

        CloseHandle(file_handle);
    }

    return result;
}

static bool32 read_file_implementation(char16* file_name, void* memory)
{
    bool32 result = 0;

    HANDLE file_handle = CreateFileW(file_name, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        DWORD file_size = GetFileSize(file_handle, 0);
        if (file_size != INVALID_FILE_SIZE)
        {
            // TODO: Read file function can read maximum 2048 megabytes, due to size of its third parameter.
            //       How can we get around it?
            DWORD readed_file_size;
            if (ReadFile(file_handle, memory, file_size, (DWORD*)&readed_file_size, 0))
            {
                if (readed_file_size == file_size)
                {
                    result = 1;
                }
            }
        }

        CloseHandle(file_handle);
    }

    return result;
}

static void* reserve_memory_implementation(uint64 size)
{
    return VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS);
}

static void* commit_memory_implementation(void* memory, uint64 size)
{
    return VirtualAlloc(0, size, MEM_COMMIT, PAGE_READWRITE);
}

static void* allocate_memory_implementation(uint64 size)
{
    return VirtualAlloc(0, size, MEM_RESERVE | MEM_COMMIT, PAGE_READWRITE);
}

// TODO: Can decommit only the end of memory block
static void decommit_memory_implementation(void* memory, uint64 size)
{
    VirtualFree(memory, 0, MEM_DECOMMIT);
}

static void release_memory_implementation(void* memory)
{
    VirtualFree(memory, 0, MEM_RELEASE);
}

static void sleep_implementation(int32 nanoseconds)
{
    if (nanoseconds > 1'000)
    {
        LARGE_INTEGER delay;
        delay.QuadPart = -nanoseconds / 100;

        NtDelayExecution(0, &delay);
    }
}

static uint64 get_time_tick_implementation()
{
    int64 perfomance_counter_frequency;
    int64 current_counter;

    QueryPerformanceFrequency((LARGE_INTEGER*)&perfomance_counter_frequency);
    QueryPerformanceCounter((LARGE_INTEGER*)&current_counter);

    int64 quotient = current_counter / perfomance_counter_frequency;
    int64 remainder = current_counter % perfomance_counter_frequency;

    return (quotient * 1'000'000'000) + (remainder * 1'000'000'000) / perfomance_counter_frequency;
}

static void present_offscreen_implementation(void* memory, int32 width, int32 height)
{
    uint32* source_row = (uint32*)memory;
    uint32* destination_row = (uint32*)device_bitmap_memory;
    for (int32 y = 0; y < height; y += 1)
    {
        uint32* source_pixel = source_row;
        uint32* destination_pixel = (uint32*)destination_row;
        for (int32 x = 0; x < width; x += 1)
        {
            uint8 red = (uint8)(*source_pixel >> 0);
            uint8 green = (uint8)(*source_pixel >> 8);
            uint8 blue = (uint8)(*source_pixel >> 16);
            uint8 alpha = (uint8)(*source_pixel >> 24);

            *destination_pixel = (alpha << 24) | (red << 16) | (green << 8) | blue;

            source_pixel += 1;
            destination_pixel += 1;
        }

        source_row += width;
        destination_row += MAX_GAME_HORIZONTAL_RESOLUTION;
    }

    InvalidateRect(window, 0, 0);
    UpdateWindow(window);
}
