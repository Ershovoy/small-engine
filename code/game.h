#include "types.h"
#include "macros.h"
#include "math.h"
#include "input.h"
#include "renderer.h"
#include "platform_api.h"
#include "arena.h"
#include "string.h"
#include "game_state.h"
#include "game_render.h"
#include "network.h"
#include "sound.h"

typedef struct
{
    Image_view offscreen;

    int64 start_time;
    int64 current_time;
    int64 previous_time;
    int64 time_per_update;
    int64 update_accumulator;
    int64 previous_update_time;
    int64 time_per_frame;
    int64 frame_accumulator;
    int64 previous_frame_time;

    Game_state state;
    Game_state previous_state;
    Player_input previous_input;

    Sound test_sound;

    // uint32 ips[MAX_PLAYERS];
    // uint16 ports[MAX_PLAYERS];
    // bool32 is_connected[MAX_PLAYERS];

    // bool32 is_server;
    // bool32 is_client;
    // bool32 is_offline;

    // Tick_input tick_input;
} Game;

static Game* game = { 0 };
