static void revert_string(char8* buffer, int64 length)
{
    int64 begin = 0;
    int64 end = length - 1;
    while (begin < end)
    {
        char8 temporary = buffer[begin];
        buffer[begin] = buffer[end];
        buffer[end] = temporary;

        begin += 1;
        end -= 1;
    }
}

static int64 uint64_to_string(char8* buffer, int64 size, uint64 value)
{
    int64 index = 0;

    do
    {
        buffer[index] = (char8)('0' + (value % 10));
        index += 1;
        value /= 10;
    } while (value > 0 && index < size - 1);

    buffer[index] = '\0';

    revert_string(buffer, index);

    return index;
}

static int64 uint32_to_string(char8* buffer, int64 size, uint32 value)
{
    return uint64_to_string(buffer, size, value);
}

static int64 uint16_to_string(char8* buffer, int64 size, uint16 value)
{
    return uint64_to_string(buffer, size, value);
}

static int64 uint8_to_string(char8* buffer, int64 size, uint8 value)
{
    return uint64_to_string(buffer, size, value);
}

static int64 int64_to_string(char8* buffer, int64 size, int64 value)
{
    int64 index = 0;

    if (value < 0)
    {
        // TODO: Undefined behavior in case value is equal to INT64_MIN.
        value = -value;
        buffer[index] = '-';
        index += 1;
    }

    index += uint64_to_string(buffer + index, size - index, value);

    return index;
}

static int64 int32_to_string(char8* buffer, int64 size, int32 value)
{
    return int64_to_string(buffer, size, value);
}

static int64 int16_to_string(char8* buffer, int64 size, int16 value)
{
    return int64_to_string(buffer, size, value);
}

static int64 int8_to_string(char8* buffer, int64 size, int8 value)
{
    return int64_to_string(buffer, size, value);
}
