typedef enum
{
    TRANSPARENT,
    BLACK,
    WHITE,
    RED,
    GREEN,
    BLUE,
    YELLOW,
    CYAN,
    MAGENTA,

    MID_GREEN,
    GREY,
    MERINO,
    COLOR_COUNT = 0xFF
} Color;

typedef struct
{
    int32 width;
    int32 height;
    byte* memory;
} Image;

#define MAX_RESOLUTION 256
#define DEFAULT_RESOLUTION 64+32

static Image offscreen = { 0 };
static byte buffer[MAX_RESOLUTION * MAX_RESOLUTION] = { 0 };
