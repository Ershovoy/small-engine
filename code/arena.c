void copy_memory(void* destination, void* source, int64 size)
{
    while (size > 0)
    {
        *(byte*)destination = *(byte*)source;
        destination = (byte*)destination + 1;
        source = (byte*)source + 1;
        size -= 1;
    }
}

static void arena_initialize(Arena* arena, int64 capacity)
{
    void* memory = reserve_memory(capacity);
    if (memory)
    {
        arena->memory = memory;
        arena->capacity = capacity;
        arena->offset = 0;
        arena->committed = 0;
    }
}

static bool32 is_power_of_two(int64 address)
{
    return (address & (address - 1)) == 0;
}

static void* arena_allocate(Arena* arena, int64 size)
{
    void* result = 0;

    arena->offset = round_up_to_multiple(arena->offset, 8);
    if (arena->committed < arena->offset + size)
    {
        int64 commit_size = round_up_to_multiple(size, MEMORY_PAGE_SIZE);
        if (arena->capacity >= arena->committed + commit_size)
        {
            if (commit_memory(arena->memory + arena->committed, commit_size))
            {
                arena->committed += commit_size;
            }
        }
    }

    if (arena->committed >= arena->offset + size)
    {
        result = arena->memory + arena->offset;
        arena->offset += size;
    }

    return result;
}

static void arena_pop_to()
{
    // [commited_memory] + ARENA_DECOMMIT_THRESHOLD + [release_memory]
    //
}

static void arena_clear(Arena* arena)
{
    arena->offset = 0;
}

static void arena_release(Arena* arena)
{
    release_memory(arena->memory);
}
