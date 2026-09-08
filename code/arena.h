#define DEFAULT_RESERVE_SIZE GIGABYTES(1)
#define DEFAULT_COMMIT_SIZE KILOBYTES(4)

#define MEMORY_PAGE_SIZE KILOBYTES(4)

#define MEMORY_PAGE_THREADHOLD MEGABYTES(1)

typedef struct
{
    byte* memory;
    int64 capacity;
    int64 committed;
    int64 offset;
} Arena;
