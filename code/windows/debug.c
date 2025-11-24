void unload_game_dll()
{
    if (game_dll)
    {
        game = **(Game***)GetProcAddress(game_dll, "game_dll");

        FreeLibrary(game_dll);
    }
}

void load_game_dll()
{
    CopyFileW(GAME_DLL_FILE_PATH, GAME_DLL_TEMPORARY_FILE_PATH, FALSE);

    game_dll = LoadLibraryW(GAME_DLL_TEMPORARY_FILE_PATH);

    if (game_dll)
    {
        process_button = (process_button_function*)GetProcAddress(game_dll, "process_button_dll");
        reset_input_function_pointer = (reset_input_function*)GetProcAddress(game_dll, "reset_input_dll");
        initialize_game = (initialize_game_function*)GetProcAddress(game_dll, "initialize_game_dll");
        game_loop = (game_loop_function*)GetProcAddress(game_dll, "game_loop_dll");

        Platform_api** platform_api_dll = (Platform_api**)GetProcAddress(game_dll, "platform_api_dll");
        **platform_api_dll = platform_api;
        Game*** game_state_dll = (Game***)GetProcAddress(game_dll, "game_dll");
        **game_state_dll = game;

        int a = 0;
        a += 1;
    }
}

FILETIME GetLastFileWriteTime(WCHAR* file_name)
{
    FILETIME last_file_write_time = { 0 };

    WIN32_FILE_ATTRIBUTE_DATA file_attribute_data;
    if (GetFileAttributesExW(file_name, GetFileExInfoStandard, &file_attribute_data))
    {
        last_file_write_time = file_attribute_data.ftLastWriteTime;
    }

    return last_file_write_time;
}

void initialize_debug()
{
    load_game_dll();
    previous_file_write_time = GetLastFileWriteTime(GAME_DLL_FILE_PATH);
}

void update_debug()
{
    current_file_write_time = GetLastFileWriteTime(GAME_DLL_FILE_PATH);
    if (CompareFileTime(&previous_file_write_time, &current_file_write_time) != 0)
    {
        unload_game_dll();
        load_game_dll();
        current_file_write_time = GetLastFileWriteTime(GAME_DLL_FILE_PATH);
        previous_file_write_time = current_file_write_time;
    }
}

void deinitialize_debug()
{
    unload_game_dll();
}

void reset_input()
{
    if (reset_input_function_pointer)
    {
        reset_input_function_pointer();
    }
}
