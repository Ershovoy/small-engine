#define str8(literal) (Str8){ .length = length(literal), .data = *literal }

typedef struct
{
    int64 length;
    char8* data;
} Str8;
