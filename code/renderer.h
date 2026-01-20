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
    Image image;
    int32 width;
    int32 height;
    int32 offset_x;
    int32 offset_y;
} Image_view;
