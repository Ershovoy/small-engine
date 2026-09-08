#define MAX_PLAYERS 2

// ── Константы для новой системы координат (Y=0 снизу, вверх = +Y) ─────────────

#define LEFT_PLANE   0.0f
#define RIGHT_PLANE  800.0f

#define BLOBBY_HEIGHT        89.0f
#define BLOBBY_UPPER_SPHERE  19.0f
#define BLOBBY_UPPER_RADIUS  25.0f
#define BLOBBY_LOWER_SPHERE  13.0f
#define BLOBBY_LOWER_RADIUS  33.0f

// Земля: центр блоба стоит на высоте половины его роста
#define GROUND_PLANE_HEIGHT     (BLOBBY_HEIGHT / 2.0f)   // 44.5
#define GROUND_PLANE_HEIGHT_MAX  500.0f                   // потолок

// Прыжок: +15.1 = вверх, гравитация отрицательная (тянет вниз)
#define BLOBBY_MAX_JUMP_HEIGHT   249.125f
#define BLOBBY_JUMP_ACCELERATION  15.1f
#define GRAVITATION              (-(BLOBBY_JUMP_ACCELERATION * BLOBBY_JUMP_ACCELERATION \
                                    / BLOBBY_MAX_JUMP_HEIGHT))      // ≈ -0.915
#define BLOBBY_JUMP_BUFFER       (GRAVITATION / 2.0f)
#define BLOBBY_SPEED              4.5f

#define BALL_RADIUS             31.5f
#define BALL_GRAVITATION       -0.287f                    // отрицательная — тянет вниз
#define BALL_COLLISION_VELOCITY 13.1224998f

#define NET_POSITION_X      (RIGHT_PLANE / 2.0f)
#define NET_RADIUS           7.0f
// Сфера на вершине сетки: в старой системе Y=284 сверху → в новой 500-284 = 216
#define NET_SPHERE_POSITION  216.0f

// Мяч при старте: в старой системе Y=300.5 сверху → в новой 500-300.5 = 199.5
#define STANDARD_BALL_HEIGHT 200.0f

typedef struct
{
    bool32 left;
    bool32 right;
    bool32 up;
} Player_input;

typedef struct
{
    uint64 tick;
    uint64 hash;
    Player_input player_inputs[MAX_PLAYERS];
} Tick_input;

// ── Game_state ────────────────────────────────────────────────────────────────

typedef struct
{
    fixed32 player_position_x[MAX_PLAYERS];
    fixed32 player_position_y[MAX_PLAYERS];
    fixed32 player_velocity_x[MAX_PLAYERS];
    fixed32 player_velocity_y[MAX_PLAYERS];

    fixed32 ball_position_x;
    fixed32 ball_position_y;
    fixed32 ball_velocity_x;
    fixed32 ball_velocity_y;

    int32 score[MAX_PLAYERS];

    bool32 is_ball_on_ground;
    bool32 is_ball_active;

    uint64 tick;
} Game_state;
