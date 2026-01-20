static void revert_string(char8* buffer, int32 length)
{
    int32 start = 0;
    int32 end = length - 1;
    while (start < end)
    {
        char8 temporary = buffer[start];
        buffer[start] = buffer[end];
        buffer[end] = temporary;

        start += 1;
        end -= 1;
    }
}

static int32 uint32_to_string(char8* buffer, int32 size, uint32 value)
{
    int32 index = 0;

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

static int32 int32_to_string(char8* buffer, int32 size, int32 value)
{
    int32 index = 0;

    if (value < 0)
    {
        value = -value;
        buffer[index] = '-';
        index += 1;
    }

    index += uint32_to_string(buffer + index, size - index, value);

    return index;
}
