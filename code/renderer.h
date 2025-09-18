#define MAX_GAME_HORIZONTAL_RESOLUTION 3840
#define MAX_GAME_VERTICAL_RESOLUTION 2160
#define GAME_HORIZONTAL_RESOLUTION 160
#define GAME_VERTICAL_RESOLUTION   120

typedef struct
{
    uint8 red;
    uint8 green;
    uint8 blue;
    uint8 alpha;
} Color;

typedef struct
{
    int32 width;
    int32 height;
    uint32* memory;
} Image;

typedef struct
{
    Image* image;
    int32 base_width;
    int32 base_height;
    int32 width;
    int32 height;
    int32 scale;
    int32 target_width;
    int32 target_height;
    int32 translate_x;
    int32 translate_y;
    int32 horizontal_padding;
    int32 vertical_padding;
} Image_view;

static uint32 offscreen_buffer[MAX_GAME_HORIZONTAL_RESOLUTION * MAX_GAME_VERTICAL_RESOLUTION] = { 0 };
static Image offscreen = { 0 };
static Image_view offscreen_view = { 0 };
