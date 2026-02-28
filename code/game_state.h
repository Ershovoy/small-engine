#define MAX_PLAYERS 2

#define TABLE_PADDING_X      42
#define TABLE_PADDING_Y      8

#define PUCK_RADIUS          4
#define PUCK_FRICTION        0.985f
#define PUCK_MAX_SPEED       400.0f

#define PADDLE_RADIUS        6
#define PADDLE_SPRING        250.0f
#define PADDLE_DAMPING       0.75f
#define PADDLE_IMPULSE_SCALE 1.0f
#define PADDLE_RESTITUTION   1.15f

#define GOAL_WIDTH           40

// Вычисляемая геометрия
#define TABLE_LEFT           (TABLE_PADDING_X)
#define TABLE_RIGHT          (GAME_DEFAULT_HORIZONTAL_RESOLUTION - TABLE_PADDING_X)
#define TABLE_TOP            (TABLE_PADDING_Y)
#define TABLE_BOTTOM         (GAME_DEFAULT_VERTICAL_RESOLUTION   - TABLE_PADDING_Y)
#define TABLE_CX             ((TABLE_LEFT  + TABLE_RIGHT)  / 2)
#define TABLE_CY             ((TABLE_TOP   + TABLE_BOTTOM) / 2)
#define TABLE_WIDTH          (TABLE_RIGHT  - TABLE_LEFT)
#define TABLE_HEIGHT         (TABLE_BOTTOM - TABLE_TOP)

#define GOAL_LEFT            (TABLE_CX - GOAL_WIDTH / 2)
#define GOAL_RIGHT           (TABLE_CX + GOAL_WIDTH / 2)

#define PADDLE_MIN_X(radius) (TABLE_LEFT  + (radius))
#define PADDLE_MAX_X(radius) (TABLE_RIGHT - (radius))

#define PADDLE_0_MIN_Y(radius) (TABLE_TOP  + (radius))
#define PADDLE_0_MAX_Y(radius) (TABLE_CY   - (radius))
#define PADDLE_1_MIN_Y(radius) (TABLE_CY   + (radius))
#define PADDLE_1_MAX_Y(radius) (TABLE_BOTTOM - (radius))

typedef struct
{
    fixed32 cursor_x;
    fixed32 cursor_y;
} Player_input;

typedef struct
{
    uint64 tick;
    uint64 hash;
    Player_input player_inputs[MAX_PLAYERS];
} Tick_input;

typedef struct
{
    fixed32 paddle_position_x[MAX_PLAYERS];
    fixed32 paddle_position_y[MAX_PLAYERS];
    fixed32 paddle_velocity_x[MAX_PLAYERS];
    fixed32 paddle_velocity_y[MAX_PLAYERS];

    fixed32 puck_position_x;
    fixed32 puck_position_y;
    fixed32 puck_velocity_x;
    fixed32 puck_velocity_y;

    int32 score[MAX_PLAYERS];

    uint64 tick;
} Game_state;
