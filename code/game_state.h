#define MAX_PLAYERS 8

typedef struct
{
    bool32 is_right;
    bool32 is_left;
    bool32 is_up;
    bool32 is_down;
    int32 mouse_x;
    int32 mouse_y;
} Player_input;

typedef struct
{
    uint64 tick;
    Player_input player_inputs[MAX_PLAYERS];
} Tick_input;

typedef struct
{
    Vec2i player_positions[MAX_PLAYERS];
    Vec2i player_velocities[MAX_PLAYERS];
    Vec2i ball_position;
    Vec2i ball_velocity;

    Vec2 position;
    Vec2 mouse_position;

    uint64 tick;
} Game_state;
