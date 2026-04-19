typedef signed long long int64;
typedef float  float32;

typedef struct
{
    int64 index;
    // TOOD: Maybe valid generation should start from one, like index.
    int64 generation;
} Reference;

typedef struct
{
    int64 generation;
    int64 next;
    int64 previous;
} Header;

typedef struct
{
    float32 x;
    float32 y;
} Thing;

typedef struct
{
    Header header;
    Thing thing;
} Wrapper;

#define MAX_THINGS 256

typedef struct
{
    Wrapper wrappers[MAX_THINGS];
    int64 thing_size;
    int64 watermark;
    int64 first_free;
    int64 begin;
    int64 end;
} Pool;

static void initialize_pool(Pool* pool, int64 thing_size)
{
    pool->watermark = 1;
    pool->thing_size = thing_size;
}

static int64 find_empty_slot(Pool* pool)
{
    int64 result = 0;

    if(pool->first_free)
    {
        result = pool->first_free;
    }
    else
    {
        result = pool->watermark;
    }

    // TODO: Simplify
    if (MAX_THINGS <= result)
    {
        result = 0;
    }

    return result;
}

static Reference add_thing_to_pool(Pool* pool)
{
    Reference result = { 0 };

    int64 slot = find_empty_slot(pool);
    if (slot)
    {
        if (slot == pool->watermark)
        {
            pool->watermark += 1;
        }
        else
        {
            pool->first_free = pool->wrappers[slot].header.next;
        }

        if (!pool->begin)
        {
            pool->begin = slot;
        }
        if (pool->end)
        {
            pool->wrappers[pool->end].header.next = slot;
        }

        pool->wrappers[slot].thing = (Thing){ 0 };
        // pool->wrappers[slot].header.generation += 1;
        // TOOD: Maybe should be traversal
        pool->wrappers[slot].header.next = 0;
        pool->wrappers[slot].header.previous = pool->end;

        pool->end = slot;

        result.index = slot;
        result.generation = pool->wrappers[slot].header.generation;
    }

    return result;
}

static int64 dereference(Pool* pool, Reference reference)
{
    int64 result = 0;
    if (0 < reference.index && reference.index < MAX_THINGS)
    {
        // TODO: Should generation be 0?
        if (reference.generation == pool->wrappers[reference.index].header.generation)
        {
            result = reference.index;
        }
    }

    return result;
}

static Thing get_thing(Pool* pool, Reference reference)
{
    return pool->wrappers[dereference(pool, reference)].thing;
}

static void remove_thing_from_pool(Pool* pool, Reference reference)
{
    int64 index = dereference(pool, reference);
    if (index)
    {
        int64 next = pool->wrappers[index].header.next;
        int64 previous = pool->wrappers[index].header.previous;
        if (next)
        {
            pool->wrappers[next].header.previous = previous;
        }
        else
        {
            pool->end = pool->wrappers[index].header.previous;
        }
        if (previous)
        {
            pool->wrappers[previous].header.next = next;
        }
        else
        {
            pool->begin = pool->wrappers[index].header.next;
        }

        pool->wrappers[index].header.next = pool->first_free;
        pool->first_free = index;
        pool->wrappers[index].header.generation += 1;
    }
}

int main()
{
    Pool pool;
    for (int64 i = pool.begin; i != 0; i = pool.wrappers[i].header.next)
    {

    }
}
