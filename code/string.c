
static void revert_string(String string)
{

}

//static void str_reverse(char16* buf, int32 len)
//{
//    int32 i = 0, j = len - 1;
//    while (i < j)
//    {
//        char16 t = buf[i];
//        buf[i] = buf[j];
//        buf[j] = t;
//        ++i; --j;
//    }
//}

//static int32 u32_to_str(uint32 value, char16* buffer, int32 buffer_size)
//{
//    int32 pos = 0;

//    if (buffer_size < 2) return 0;

//    if (value == 0)
//    {
//        buffer[0] = '0';
//        buffer[1] = 0;
//        return 1;
//    }

//    while (value != 0 && pos < buffer_size - 1)
//    {
//        buffer[pos++] = (char16)(L'0' + (value % 10));
//        value /= 10;
//    }
//    buffer[pos] = 0;
//    str_reverse(buffer, pos);
//    return pos;
//}

// static int32 int32_to_string(int32 value, char16* buffer, int32 buffer_size)
// {
//     int32 pos = 0;
//     uint32 uval = (uint32)value;

//     if (buffer_size < 2) return 0;

//     if (value < 0)
//     {
//         if (pos < buffer_size - 1)
//             buffer[pos++] = L'-';
//         uval = (uint32)(-value);  /* работает даже для INT32_MIN на two's complement */
//     }

//     int32 digits = u32_to_str(uval, buffer + pos, buffer_size - pos);
//     if (digits == 0) return 0;
//     return pos + digits;
// }
