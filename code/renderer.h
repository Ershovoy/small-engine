#define GAME_MIN_HORIZONTAL_RESOLUTION 40
#define GAME_MIN_VERTICAL_RESOLUTION   30
#define GAME_MAX_HORIZONTAL_RESOLUTION (960 * 2) // 3840
#define GAME_MAX_VERTICAL_RESOLUTION   (540 * 2) // 2160

#define GAME_DEFAULT_HORIZONTAL_RESOLUTION (800)// (160)  // 160
#define GAME_DEFAULT_VERTICAL_RESOLUTION   (600)// (120)  // 120

typedef struct
{
    uint16 file_type;
    uint32 file_size;
    uint16 reserved[2];
    uint32 bitmap_offset;
    uint32 header_size;
    int32 bitmap_width;
    int32 bitmap_height;
    uint16 planes;
    uint16 bits_per_pixel;
    uint32 compression;
    uint32 bitmap_size;
    int32 horizontal_resolution;
    int32 vertical_resolution;
    uint32 color_number;
    uint32 important_color_number;
} Bitmap_file_header;

typedef struct
{
    uint8 red;
    uint8 green;
    uint8 blue;
    uint8 alpha;
} Color;

typedef struct
{
    uint32* memory;
    int64 size;
    int32 width;
    int32 height;
} Image;

typedef struct
{
    Image image;
    int32 width;
    int32 height;
    int32 offset_x;
    int32 offset_y;
} Image_view;
