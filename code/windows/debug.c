void unload_game_dll()
{
    if (game_dll)
    {
        game = **(Game**)GetProcAddress(game_dll, "game_dll");
        offscreen = **(Image**)GetProcAddress(game_dll, "offscreen_dll");
        offscreen_view = **(Image_view**)GetProcAddress(game_dll, "offscreen_view_dll");
        offscreen.memory = offscreen_buffer;
        //offscreen_view.image = &offscreen;

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
        reset_input = (reset_input_function*)GetProcAddress(game_dll, "reset_input_dll");
        initialize_game = (initialize_game_function*)GetProcAddress(game_dll, "initialize_game_dll");
        game_loop = (game_loop_function*)GetProcAddress(game_dll, "game_loop_dll");
        change_target_resolution = (change_target_resolution_function*)GetProcAddress(game_dll, "change_target_resolution_dll");

        Platform_api** platform_api_dll = (Platform_api**)GetProcAddress(game_dll, "platform_api_dll");
        **platform_api_dll = platform_api;
        Game** game_dll2 = (Game**)GetProcAddress(game_dll, "game_dll");
        **game_dll2 = game;
        Image** offscreen_dll = (Image**)GetProcAddress(game_dll, "offscreen_dll");
        **offscreen_dll = offscreen;
        Image_view** offscreen_view_dll = (Image_view**)GetProcAddress(game_dll, "offscreen_view_dll");
        **offscreen_view_dll = offscreen_view;
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
