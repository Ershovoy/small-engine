#define STRING_LITERAL(literal) (String){ .length = lengthof(literal), .data = (char8*)literal }

typedef struct
{
    int64 length;
    char8* data;
} String;
