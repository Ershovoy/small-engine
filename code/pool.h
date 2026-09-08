typedef struct
{
    int64 index;
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
    byte* base;
    int64  slot_size;
    int64  watermark;
    int64  first_free; // free_slot?
    int64  committed;
    int64  capacity;
} Pool;
