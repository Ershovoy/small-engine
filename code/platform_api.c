static void sleep(int32 nanoseconds)
{
    platform_api.sleep(nanoseconds);
}

static uint64 get_time_tick()
{
    return platform_api.get_time_tick();
}

static uint32 get_file_size(char8* file_name)
{
    return platform_api.get_file_size(file_name);
}

static bool32 read_file(char8* file_name, void* memory)
{
    return platform_api.read_file(file_name, memory);
}

static bool32 write_file(char8* file_name, void* data, uint64 size)
{
    return platform_api.write_file(file_name, data, size);
}

static void* reserve_memory(uint64 size)
{
    return platform_api.reserve_memory(size);
}

static void* commit_memory(void* memory, uint64 size)
{
    return platform_api.commit_memory(memory, size);
}

static void decommit_memory(void* memory, uint64 size)
{
    platform_api.decommit_memory(memory, size);
}

static void release_memory(void* memory)
{
    platform_api.release_memory(memory);
}

static void console_write(String string)
{
    platform_api.console_write(string.data, string.length);
}

static uint16 net_bind(uint16 port)
{
    return platform_api.net_bind(port);
}

static int32 net_send(void* data, uint64 size, uint32 ip, uint16 port)
{
    return platform_api.net_send(data, size, ip, port);
}

static int32 net_receive(void* buffer, uint64 size, uint32* out_ip, uint16* out_port)
{
    return platform_api.net_receive(buffer, size, out_ip, out_port);
}

static void play_sound(Sound sound)
{
    if (sound.is_initialized)
    {
        platform_api.play_sound(sound.memory, sound.size, sound.sample_rate,
                                sound.bits_per_sample, sound.number_of_channels);
    }
}

static void present_offscreen(Image_view offscreen_view)
{
    platform_api.present_offscreen(offscreen_view.image.memory, offscreen_view.width, offscreen_view.height);
}
