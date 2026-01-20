#include "types.h"
#include "macros.h"
#include "math.h"
#include "input.h"
#include "renderer.h"
#include "platform_api.h"
#include "arena.h"
#include "string.h"
#include "state.h"
#include "network.h"
#include "sound.h"

#define MAX_PLAYERS 8
#define MAX_TICK_INPUT_BUFFER 256

#define BALL_RADIUS 10
#define GROUND_HEIGHT 10

typedef struct
{
    Image_view offscreen;

    uint64 start_time;
    uint64 time_per_update;
    uint64 accumulator;
    uint64 previous_time;
    uint64 current_time;

    State client_state;
    State server_state;

    uint32 ips[MAX_PLAYERS];
    uint16 ports[MAX_PLAYERS];
    bool32 is_connected[MAX_PLAYERS];

    bool32 is_server;
    bool32 is_client;
    bool32 is_offline;

    Tick_input tick_input;
    Tick_input tick_inputs[MAX_TICK_INPUT_BUFFER];
    bool32 has_tick_input[MAX_TICK_INPUT_BUFFER];
} Game;

static Game* game = { 0 };
