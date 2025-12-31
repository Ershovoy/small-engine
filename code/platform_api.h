typedef uint64 get_time_tick_function();

typedef void sleep_function(int32 nanoseconds);

typedef uint32 get_file_size_function(char8* file_name);
typedef bool32 read_file_function(char8* file_name, void* memory);
typedef bool32 write_file_function(char8* file_name, void* data, uint64 size);

typedef void* reserve_memory_function(uint64 size);
typedef void* commit_memory_function(void* memory, uint64 size);
typedef void decommit_memory_function(void* memory, uint64 size);
typedef void release_memory_function(void* memory);

typedef void console_read_function();
typedef void console_write_function(char8* buffer, int64 length);

typedef uint16 net_bind_function(uint16 port);
typedef bool32 net_send_function(void* data, uint64 size, uint32 ip, uint16 port);
typedef bool32 net_receive_function(void* buffer, uint64 size, uint32* ip, uint16* port);

typedef void play_sound_function(void* memory, uint32 size, int32 sample_rate, int32 bits_per_sample, int32 number_of_channels);

typedef void present_offscreen_function(void* memory, int32 width, int32 height);

typedef struct
{
    sleep_function* sleep;

    get_time_tick_function* get_time_tick;

    get_file_size_function* get_file_size;
    read_file_function* read_file;
    write_file_function* write_file;

    reserve_memory_function* reserve_memory;
    commit_memory_function* commit_memory;
    decommit_memory_function* decommit_memory;
    release_memory_function* release_memory;

    console_read_function* console_read;
    console_write_function* console_write;

    net_bind_function* net_bind;
    net_send_function* net_send;
    net_receive_function* net_receive;

    play_sound_function* play_sound;

    present_offscreen_function* present_offscreen;
} Platform_api;

static Platform_api platform_api = { 0 };
