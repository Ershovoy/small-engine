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

static void* arena_allocate(Arena* arena, int64 size)
{
    int32 align = 8;
    int32 remainder = arena->offset % align;
    if (remainder != 0)
    {
        int32 padding = align - remainder;
        arena->offset += padding;
    }

    if (arena->committed < arena->offset + size)
    {
        int32 commit_size = (int32)size;
        int32 remainder = size % MEMORY_PAGE_SIZE;
        if (remainder != 0)
        {
            int32 padding = MEMORY_PAGE_SIZE - remainder;
            commit_size += padding;
        }
        commit_memory(arena->memory + arena->committed, commit_size);
        arena->committed += commit_size;
    }

    void* result = arena->memory + arena->offset;

    arena->offset += size;

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
