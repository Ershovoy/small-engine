typedef uint64 get_time_tick_function();

typedef void sleep_function(int32 nanoseconds);

typedef uint32 get_file_size_function(char16* file_name);
typedef bool32 read_file_function(char16* file_name, void* memory);

typedef void* reserve_memory_function(uint64 size);
typedef void* commit_memory_function(void* memory, uint64 size);
typedef void decommit_memory_function(void* memory, uint64 size);
typedef void release_memory_function(void* memory);

typedef void present_offscreen_function(void* memory, int32 width, int32 height);

typedef struct
{
    sleep_function* sleep;

    // TOOD: Need to be changed. But how? Maybe get_ticks? Maybe get_time_tick? Maybe get_time_stamp?
    get_time_tick_function* get_time_tick;

    get_file_size_function* get_file_size;
    read_file_function* read_file;

    reserve_memory_function* reserve_memory;
    commit_memory_function* commit_memory;
    decommit_memory_function* decommit_memory;
    release_memory_function* release_memory;

    present_offscreen_function* present_offscreen;
} Platform_api;

static Platform_api platform_api = { 0 };
