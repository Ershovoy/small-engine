#include "types.h"
#include "macros.h"
#include "math.h"
#include "input.h"
#include "renderer.h"
#include "platform_api.h"
#include "arena.h"
#include "string.h"
#include "sound.h"

#define MAX_PLAYERS 2

// Border Settings
#define LEFT_PLANE 0.0f
#define RIGHT_PLANE 800.0f

// Blobby settings
#define BLOBBY_HEIGHT 89.0f
#define BLOBBY_UPPER_SPHERE 19.0f
#define BLOBBY_UPPER_RADIUS 25.0f
#define BLOBBY_LOWER_SPHERE 13.0f
#define BLOBBY_LOWER_RADIUS 33.0f

// Ground Settings
#define GROUND_PLANE_HEIGHT_MAX 500.0f
#define GROUND_PLANE_HEIGHT ((GROUND_PLANE_HEIGHT_MAX) - (BLOBBY_HEIGHT) / 2.0f)

// This is exactly the half of the gravitation, I checked it in the original code
#define BLOBBY_MAX_JUMP_HEIGHT ((GROUND_PLANE_HEIGHT) - 206.375f)
#define BLOBBY_JUMP_ACCELERATION -15.1f

// these values are calculated from the other two
#define GRAVITATION ((BLOBBY_JUMP_ACCELERATION) * (BLOBBY_JUMP_ACCELERATION) / (BLOBBY_MAX_JUMP_HEIGHT))
#define BLOBBY_JUMP_BUFFER ((GRAVITATION) / 2.0f)

// Ball Settings
#define BALL_RADIUS 31.5f
#define BALL_GRAVITATION 0.287f
//#define BALL_COLLISION_VELOCITY sqrt(0.75f * RIGHT_PLANE * BALL_GRAVITATION);
#define BALL_COLLISION_VELOCITY 13.1224998f

// Volley Ball Net
#define NET_POSITION_X ((RIGHT_PLANE) / 2)
#define NET_POSITION_Y 438
#define NET_RADIUS 7
#define NET_SPHERE 154 // what is the meaning of this value ???????
#define NET_SPHERE_POSITION 284

#define STANDARD_BALL_HEIGHT (269 + (BALL_RADIUS))

// BLOBBY_SPEED is necessary to determine the size of the input buffer
#define BLOBBY_SPEED 4.5f
#define STANDARD_BALL_ANGULAR_VELOCITY 0.1f

typedef struct
{
    bool32 is_right;
    bool32 is_left;
    bool32 is_up;
    bool32 is_down;
} Player_input;

typedef struct
{
    uint64 tick;
    Player_input player_inputs[MAX_PLAYERS];
} Tick_input;

typedef struct
{
    Image_view offscreen;

    uint64 start_time;
    uint64 time_per_update;
    uint64 accumulator;
    uint64 previous_time;
    uint64 current_time;

    uint64 tick;

    Tick_input tick_input;

    float32 ball_x;

    Vec2 ball_position;
    Vec2 ball_velocity;
    Vec2 player_positions[MAX_PLAYERS];
    Vec2 player_velocities[MAX_PLAYERS];

    int32 x[MAX_PLAYERS];
    int32 y[MAX_PLAYERS];
    uint32 ips[MAX_PLAYERS];
    uint16 ports[MAX_PLAYERS];
} Game;

static Game* game = { 0 };
