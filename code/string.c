static void revert_buffer(char8* buffer, int64 size)
{
    int64 begin = 0;
    int64 end = size - 1;
    while (begin < end)
    {
        char8 temporary = buffer[begin];
        buffer[begin] = buffer[end];
        buffer[end] = temporary;

        begin += 1;
        end -= 1;
    }
}

static int64 uint64_to_buffer(char8* buffer, int64 size, uint64 value)
{
    int64 index = 0;

    if (size > 0)
    {
        do
        {
            buffer[index] = (char8)('0' + (value % 10));
            index += 1;
            value /= 10;

        } while (value > 0 && index < size);
    }

    revert_buffer(buffer, index);

    return index;
}

static int64 uint32_to_buffer(char8* buffer, int64 size, uint32 value)
{
    return uint64_to_buffer(buffer, size, value);
}

static int64 uint16_to_buffer(char8* buffer, int64 size, uint16 value)
{
    return uint64_to_buffer(buffer, size, value);
}

static int64 uint8_to_buffer(char8* buffer, int64 size, uint8 value)
{
    return uint64_to_buffer(buffer, size, value);
}

static int64 int64_to_buffer(char8* buffer, int64 size, int64 value)
{
    int64 index = 0;

    if (size > 0)
    {
        if (value < 0)
        {
            // TODO: Undefined behavior in case value is equal to INT64_MIN.
            value = -value;
            buffer[index] = '-';
            index += 1;
        }

        index += uint64_to_buffer(buffer + index, size - index, value);
    }

    return index;
}

static int64 int32_to_buffer(char8* buffer, int64 size, int32 value)
{
    return int64_to_buffer(buffer, size, value);
}

static int64 int16_to_buffer(char8* buffer, int64 size, int16 value)
{
    return int64_to_buffer(buffer, size, value);
}

static int64 int8_to_buffer(char8* buffer, int64 size, int8 value)
{
    return int64_to_buffer(buffer, size, value);
}

static int64 combine_buffers()
{
    return 0;
}

static String add_string(String first, String second)
{
    String result = { 0 };

    int64 required_size = first.length + second.length + 1;
    char8* buffer = arena_allocate(&game->string_arena, required_size);
    for (int64 i = 0; i < first.length; i += 1)
    {
        buffer[i] = first.data[i];
    }
    for (int64 i = 0; i < second.length; i += 1)
    {
        buffer[i + first.length] = second.data[i];
    }
    buffer[first.length + second.length] = '\0';

    result.data = buffer;
    result.length = first.length + second.length;

    return result;
}

static int64 digits_count(int64 value)
{
    int64 result = 0;

    if (value < 0)
    {
        // TODO: Undefined behavior in case value is equal to INT64_MIN.
        value = -value;
        result += 1;
    }

    do
    {
        value /= 10;
        result += 1;
    } while (value > 0);

    return result;
}

static String uint64_to_string(uint64 value)
{

}

static String uint32_to_string(uint32 value)
{
    return uint64_to_string(value);
}

static String uint16_to_string(uint16 value)
{
    return uint64_to_string(value);
}

static String uint8_to_string(uint8 value)
{
    return uint64_to_string(value);
}

static String int64_to_string(int64 value)
{
    String result = { 0 };

    int64 required_size = digits_count(value);
    char8* buffer = arena_allocate(&game->string_arena, required_size);
    result.length = int64_to_buffer(buffer, required_size, value);
    result.data = buffer;

    return result;
}

static String int32_to_string(int32 value)
{
    return int64_to_string(value);
}

static String int16_to_string(int16 value)
{
    return int64_to_string(value);
}

static String int8_to_string(int8 value)
{
    return int64_to_string(value);
}
