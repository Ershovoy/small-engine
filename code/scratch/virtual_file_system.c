static int32 vfs_get_file_size(char8* file_path)
{
    
}

static byte* vfs_read_file(char8* file_path)
{
    for (int32 file_index = 0; file_index < countof(vfs_file_paths); file_index += 1)
    {
        //if (lengthof(file_path) != lengthof(vfs_file_paths[file_index]))
        //{
        //    continue;
        //}
        bool32 a = 1;
        for (int32 string_index = 0; string_index < lengthof(vfs_file_paths[file_index]); string_index += 1)
        {
            if (file_path[string_index] != vfs_file_paths[file_index][string_index])
            {
                a = 0;
                break;
            }
        }
        if (a)
        return (byte*)&vfs_file_data[vfs_file_offsets[file_index]];
    }

    return 0;
}
