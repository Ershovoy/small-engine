static void char8_to_char16(char8* source, int64 length, char16* destination)
{
    for (int32 i = 0; i < length; i += 1)
    {
        destination[i] = source[i];
    }
    destination[length] = 0;
}

static int64 get_file_size_implementation(char8* file_name, int64 file_name_length)
{
    int64 result = 0;

    char8_to_char16(file_name, file_name_length, (char16*)scratch_buffer);
    HANDLE file_handle = CreateFileW((char16*)scratch_buffer, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        DWORD file_size = GetFileSize(file_handle, 0);
        if (file_size != INVALID_FILE_SIZE)
        {
            result = file_size;
        }

        CloseHandle(file_handle);
    }

    return result;
}

static bool32 read_file_implementation(char8* file_name, int64 file_name_length, void* buffer)
{
    bool32 result = 0;

    char8_to_char16(file_name, file_name_length, (char16*)scratch_buffer);
    HANDLE file_handle = CreateFileW((char16*)scratch_buffer, GENERIC_READ, FILE_SHARE_READ, 0, OPEN_EXISTING, 0, 0);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        DWORD file_size = GetFileSize(file_handle, 0);
        if (file_size != INVALID_FILE_SIZE)
        {
            // TODO: Read file function can read maximum 2048 megabytes, due to size of its third parameter.
            //       How can we get around it?
            DWORD readed_file_size = 0;
            if (ReadFile(file_handle, buffer, file_size, (DWORD*)&readed_file_size, 0))
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

static bool32 write_file_implementation(char8* file_name, int64 file_name_length, void* data, int64 size)
{
    bool32 result = 0;

    char8_to_char16(file_name, file_name_length, (char16*)scratch_buffer);
    HANDLE file_handle = CreateFileW((char16*)scratch_buffer, GENERIC_WRITE, 0, 0, CREATE_ALWAYS, 0, 0);
    if (file_handle != INVALID_HANDLE_VALUE)
    {
        // TODO: Write file function can write maximum 2048 megabytes, due to size of its fourth parameter.
        //       How can we get around it?
        DWORD bytes_written;
        if (WriteFile(file_handle, data, (DWORD)size, (DWORD*)&bytes_written, 0))
        {
            if (bytes_written == size)
            {
                result = 1;
            }
        }

        CloseHandle(file_handle);
    }

    return result;
}

static void* reserve_memory_implementation(int64 size)
{
    return VirtualAlloc(0, size, MEM_RESERVE, PAGE_NOACCESS);
}

static void* commit_memory_implementation(void* memory, int64 size)
{
    return VirtualAlloc(memory, size, MEM_COMMIT, PAGE_READWRITE);
}

// TODO: Can decommit only the end of memory block
static void decommit_memory_implementation(void* memory, int64 size)
{
    VirtualFree(memory, 0, MEM_DECOMMIT);
}

static void release_memory_implementation(void* memory)
{
    VirtualFree(memory, 0, MEM_RELEASE);
}

static void sleep_implementation(int64 nanoseconds)
{
    if (nanoseconds > 0)
    {
        if (nanoseconds >= 1'000'000)
        {
            HANDLE timer = CreateWaitableTimerW(0, 1, 0);
            LARGE_INTEGER delay;
            delay.QuadPart = -nanoseconds / 100;
            SetWaitableTimer(timer, &delay, 0, 0, 0, 0);
            WaitForSingleObject(timer, INFINITE);
            CloseHandle(timer);
        }
        else
        {
            int64 perfomance_counter_frequency;
            QueryPerformanceFrequency((LARGE_INTEGER*)&perfomance_counter_frequency);
            int64 target_counter = (nanoseconds * perfomance_counter_frequency) / 1'000'000'000;

            int64 start_counter;
            QueryPerformanceCounter((LARGE_INTEGER*)&start_counter);
            int64 current_counter = start_counter;
            while (current_counter - start_counter < target_counter)
            {
                YieldProcessor();
                QueryPerformanceCounter((LARGE_INTEGER*)&current_counter);
            }
        }
    }
}

static int64 get_time_tick_implementation()
{
    int64 perfomance_counter_frequency;
    int64 current_counter;

    QueryPerformanceFrequency((LARGE_INTEGER*)&perfomance_counter_frequency);
    QueryPerformanceCounter((LARGE_INTEGER*)&current_counter);

    int64 quotient = current_counter / perfomance_counter_frequency;
    int64 remainder = current_counter % perfomance_counter_frequency;

    return (quotient * 1'000'000'000) + (remainder * 1'000'000'000) / perfomance_counter_frequency;
}


static void console_write_implementation(char8* string, int64 length)
{
    HANDLE output_console_handle = GetStdHandle(STD_OUTPUT_HANDLE);
    char8_to_char16(string, length, (char16*)scratch_buffer);
    WriteConsoleW(output_console_handle, (char16*)scratch_buffer, (DWORD)length, 0, 0);
}

static int32 console_read_implementation(char8* buffer, int64 length)
{

}

static uint16 net_bind_implementation(uint16 port)
{
    SOCKADDR_IN local_address = { 0 };
    local_address.sin_family = AF_INET;
    local_address.sin_port = htons(port);
    local_address.sin_addr.s_addr = htonl(INADDR_ANY);

    int32 bind_result = bind(sock, (SOCKADDR*)&local_address, sizeof(local_address));
    int32 get_sock_name_result = 0;
    SOCKADDR_IN bound_address = { 0 };
    if (bind_result != SOCKET_ERROR)
    {
        int32 address_size = sizeof(bound_address);

        get_sock_name_result = !getsockname(sock, (SOCKADDR*)&bound_address, &address_size);
    }

    uint16 binded_port = 0;
    if (get_sock_name_result != SOCKET_ERROR)
    {
        binded_port = ntohs(bound_address.sin_port);
    }

    return binded_port;
}

static int64 net_send_implementation(void* data, int64 size, uint32 ip, uint16 port)
{
    SOCKADDR_IN server_address = { 0 };
    server_address.sin_family = AF_INET;
    server_address.sin_port = htons(port);
    server_address.sin_addr.S_un.S_addr = htonl(ip);

    int64 bytes_send = sendto(sock, (char*)data, (int32)size, 0,
                              (SOCKADDR*)&server_address, sizeof(server_address));

    if (bytes_send < 0)
    {
        bytes_send = 0;
    }

    return bytes_send;
}

static int64 net_receive_implementation(void* buffer, int64 size, uint32* out_ip, uint16* out_port)
{
    SOCKADDR_IN from;
    int32 from_size = sizeof(from);

    int64 bytes_received = recvfrom(sock, (char*)buffer, (int32)size, 0,
                                    (SOCKADDR*)&from, &from_size);

    if (out_ip)
    {
        *out_ip = ntohl(from.sin_addr.S_un.S_addr);
    }

    if (out_port)
    {
        *out_port = ntohs(from.sin_port);
    }

    if (bytes_received < 0)
    {
        bytes_received = 0;
    }

    return bytes_received;
}

static void present_offscreen_implementation(void* memory, int32 width, int32 height)
{
    game_horizontal_resoultion = width;
    game_vertical_resolution = height;

#ifdef SOFTWARE
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
        destination_row += GAME_MAX_HORIZONTAL_RESOLUTION;
    }

    InvalidateRect(window, 0, 0);
    UpdateWindow(window);
#else
    directx_draw(memory, width, height);
#endif
}
