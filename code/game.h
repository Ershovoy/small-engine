#include "types.h"
#include "defines.h"
#include "math.h"
#include "input.h"
#include "renderer.h"
#include "platform_api.h"

static uint64 start_time = 0;
static uint64 time_per_update = (uint64)1'000'000'000 / 60;
static uint64 accumulator = 0;
static uint64 previous_time = 0;
static uint64 current_time = 0;

typedef struct
{
    uint64 tick;
} Game;

static Game game = { 0 };
