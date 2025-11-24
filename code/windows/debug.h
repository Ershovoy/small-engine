#include "../game.h"

#define GAME_DLL_FILE_PATH L"../build/game.dll"
#define GAME_DLL_TEMPORARY_FILE_NAME L"game_temporary.dll"
#define GAME_DLL_TEMPORARY_FILE_PATH L"../build/game_temporary.dll"

static HMODULE game_dll = { 0 };

static FILETIME previous_file_write_time = { 0 };
static FILETIME current_file_write_time = { 0 };

typedef void process_button_function(Button button, bool32 is_down);
typedef void reset_input_function();
typedef bool32 initialize_game_function();
typedef void game_loop_function();

static process_button_function* process_button;
static reset_input_function* reset_input_function_pointer;
static initialize_game_function* initialize_game;
static game_loop_function* game_loop;
