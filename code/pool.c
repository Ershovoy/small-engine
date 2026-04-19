static Header* extract_pool_header(Pool* pool, int64 index)
{
    return (Header*)(pool->base + index * pool->slot_size);
}

static void* extract_pool_data(Pool* pool, int64 index)
{
    return (void*)(pool->base + index * pool->slot_size + sizeof(Header));
}

static void pool_init(Pool* pool, int64 capacity, int64 data_size)
{
    pool->base = reserve_memory(capacity);
    pool->slot_size = round_up_to_multiple(sizeof(Header) + data_size, 8);
    pool->watermark = 1;
    pool->first_free = 0;
    pool->capacity = capacity;
    pool->committed = 0;
}

static Reference pool_add(Pool* pool)
{
    Reference result = { 0 };

    int64 slot = 0;
    if (pool->first_free)
    {
        slot = pool->first_free;
        pool->first_free = extract_pool_header(pool, slot)->next;
    }
    else
    {
        slot = pool->watermark;
        pool->watermark += 1;
        int64 needed = (slot + 1) * pool->slot_size;
        if (pool->committed < needed)
        {
            int64 commit_size = round_up_to_multiple(needed, MEMORY_PAGE_SIZE);
            if (pool->committed + commit_size <= pool->capacity)
            {
                commit_memory(pool->base + pool->committed, commit_size);
                pool->committed += commit_size;
            }
        }
    }

    Header* header = extract_pool_header(pool, slot);
    int64 previous = extract_pool_header(pool, 0)->previous;

    header->previous = previous;
    header->next = 0;
    extract_pool_header(pool, previous)->next = slot;
    extract_pool_header(pool, 0)->previous = slot;

    result.index = slot;
    result.generation = header->generation;

    return result;
}

static int64 pool_dereference(Pool* pool, Reference reference)
{
    int64 result = 0;
    if (reference.index > 0 && reference.index < pool->watermark)
    {
        Header* header = extract_pool_header(pool, reference.index);
        if (reference.generation == header->generation)
        {
            result = reference.index;
        }
    }

    return result;
}

static int64 pool_iterator(Pool* pool)
{
    return extract_pool_header(pool, 0)->next;
}

static int64 pool_iterate(Pool* pool, int64 iterator)
{
    return extract_pool_header(pool, iterator)->next;
}

static void* pool_get(Pool* pool, Reference reference)
{
    int64 index = pool_dereference(pool, reference);
    void* data = extract_pool_data(pool, index);
    return data;
}

static void pool_remove(Pool* pool, Reference reference)
{
    int64 index = pool_dereference(pool, reference);
    if (index)
    {
        Header* header = extract_pool_header(pool, index);
        int64 next = header->next;
        int64 previous = header->previous;

        extract_pool_header(pool, previous)->next = next;
        extract_pool_header(pool, next)->previous = previous;

        header->next = pool->first_free;
        header->generation += 1;
        pool->first_free = index;
    }
}
