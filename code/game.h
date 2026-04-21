#include "types.h"
#include "fixed_point.h"
#include "macros.h"
#include "math.h"
#include "input.h"
#include "renderer.h"
#include "platform_api.h"
#include "arena.h"
#include "pool.h"
#include "string.h"
#include "game_state.h"
#include "game_render.h"
#include "network.h"
#include "sound.h"

#define MAX_BUFFERED_TICKS (64)

// 0x7f000001 = 127.0.0.1
// 0x5DAB0267 = 93.171.2.103
// 0xC0A8006A = 192.168.0.106
#define SERVER_IP 0x7f000001
#define SERVER_PORT 0xFFFF

typedef enum {
	GAME_MODE_EMPTY,
    GAME_MODE_OFFLINE,
    GAME_MODE_SERVER,
    GAME_MODE_CLIENT,
	GAME_MODE_COUNT
} Game_mode;

typedef enum {
    GAME_SCREEN_EMPTY,
    GAME_SCREEN_MENU,
    GAME_SCREEN_MAIN,
    GAME_SCREEN_COUNT
} Game_screen;

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

    int64 update_time;
    int64 frame_time;

    Game_state state;
    Game_state previous_state;
    Tick_input previous_tick_input;

    Sound test_sound;
    Image font;

    Image background;
    Image ball;
    Image player;

    uint32 ips[MAX_PLAYERS];
    uint16 ports[MAX_PLAYERS];
    bool32 is_connected[MAX_PLAYERS];
    int64 last_packet_time[MAX_PLAYERS];

	Game_mode mode;
    Game_screen screen;

    Tick_input tick_input_server_batch[TICK_INPUT_BATCH_SIZE];

    Tick_input tick_input_buffer[MAX_BUFFERED_TICKS];
    bool32 tick_input_valid[MAX_BUFFERED_TICKS];

    Arena string_arena;
    Arena image_arena;
    Arena sound_arena;
} Game;

static Game* game = { 0 };
