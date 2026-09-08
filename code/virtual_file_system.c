static int64 vfs_get_file_size(char8* file_name, int64 file_name_length)
{
    int64 result = 0;
    for (int32 file_index = 0; file_index < countof(vfs_file_paths); file_index += 1)
    {
        bool32 is_equal = 1;
        for (int32 string_index = 0; string_index < lengthof(vfs_file_paths[file_index]); string_index += 1)
        {
            if (file_name[string_index] != vfs_file_paths[file_index][string_index])
            {
                is_equal = 0;

                break;
            }
        }
        if (is_equal)
        {
            // TODO:
            // if (file_index > 0)
            {
                result = vfs_file_offsets[file_index + 1];
            }
            result -= vfs_file_offsets[file_index];
            break;
        }
    }
    return result;
}

static bool32 vfs_read_file(char8* file_name, int64 file_name_length, void* buffer)
{
    bool32 result = 0;
    for (int32 file_index = 0; file_index < countof(vfs_file_paths); file_index += 1)
    {
        bool32 is_equal = 1;
        for (int32 string_index = 0; string_index < lengthof(vfs_file_paths[file_index]); string_index += 1)
        {
            if (file_name[string_index] != vfs_file_paths[file_index][string_index])
            {
                is_equal = 0;

                break;
            }
        }
        if (is_equal)
        {
            int64 size = vfs_get_file_size(file_name, file_name_length);
            for (int32 i = 0; i < size; i += 1)
            {
                *(byte*)buffer = vfs_file_data[vfs_file_offsets[file_index] + i];
                buffer = (byte*)buffer + 1;
            }
            result = 1;
        }
    }

    return result;
}
