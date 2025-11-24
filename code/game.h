#include "types.h"
#include "macros.h"
#include "math.h"
#include "input.h"
#include "renderer.h"
#include "platform_api.h"
#include "arena.h"
#include "string.h"

typedef struct
{
    Image_view offscreen;

    uint64 start_time;
    uint64 time_per_update;
    uint64 accumulator;
    uint64 previous_time;
    uint64 current_time;

    uint64 tick;
    float32 x;
    float32 y;
} Game;

static Game* game = { 0 };
