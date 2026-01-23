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

    uint64 start_time;
    uint64 time_per_update;
    uint64 accumulator;
    uint64 previous_time;
    uint64 current_time;

    Game_state client_state;
    Game_state server_state;

    uint32 ips[MAX_PLAYERS];
    uint16 ports[MAX_PLAYERS];
    bool32 is_connected[MAX_PLAYERS];

    bool32 is_server;
    bool32 is_client;
    bool32 is_offline;

    Tick_input tick_input;
} Game;

static Game* game = { 0 };
