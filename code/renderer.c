// void resize_offscreen(int32 target_widht, int32 target_height)
// {

// }

static void clear(uint8 red, uint8 green, uint8 blue)
{
    uint32* image_row = game->offscreen.image.memory;
    for (int32 y = 0; y < game->offscreen.height; y += 1)
    {
        uint32* image_pixel = image_row;
        for (int32 x = 0; x < game->offscreen.width; x += 1)
        {
            *image_pixel = (uint32)(red | green << 8 | blue << 16);

            image_pixel += 1;
        }
        image_row += game->offscreen.width;
    }
}

static void draw_pixel(uint8 red, uint8 green, uint8 blue, float32 position_x, float32 position_y)
{
    int32 x = round_float32_to_int32(position_x);
    int32 y = round_float32_to_int32(position_y);

    if (x >= 0 && y >= 0 && x < game->offscreen.image.width && y < game->offscreen.image.height)
    {
        game->offscreen.image.memory[y * game->offscreen.width + x] = (uint32)(red | green << 8 | blue << 16);
    }
}

// static void draw_image(Image destination, Image source, int32 left, int32 bottom)
// {

// }

static void draw_rectangle(uint8 red, uint8 green, uint8 blue, int32 left, int32 bottom, int32 right, int32 top)
{
    for (int32 y = bottom; y < top; y += 1)
    {
        for (int32 x = left; x < right; x += 1)
        {
            if (x >= 0 && y >= 0 && x < game->offscreen.width && y < game->offscreen.height)
            {
                game->offscreen.image.memory[y * game->offscreen.width + x] = (uint32)(red | green << 8 | blue << 16);
            }
        }
    }
}

// static void draw_line(Image image,  Color color, int32 x1, int32 y1, int32 x2, int32 y2)
// {

// }

static void draw_horizontal_line(uint8 red, uint8 green, uint8 blue, float32 y)
{
    y += 100;
    int32 y2 = round_float32_to_int32(y);
    for (int32 x = 0; x < game->offscreen.width; x += 1)
    {
        game->offscreen.image.memory[y2 * game->offscreen.width + x] = (uint32)(red | green << 8 | blue << 16);
    }
}

static void draw_vertical_line(uint8 red, uint8 green, uint8 blue, float32 x)
{
    int32 x2 = round_float32_to_int32(x);
    for (int32 y = 0; y < game->offscreen.height; y += 1)
    {
        game->offscreen.image.memory[y * game->offscreen.width + x2] = (uint32)(red | green << 8 | blue << 16);
    }
}

//typedef struct
//{
//	int32 min_x;
//	int32 min_y;
//	int32 max_x;
//	int32 max_y;
//} Rectangle;

typedef struct
{
	Image image;
	int32 width;
	int32 height;
} Glyph;

typedef struct
{
	Glyph glyphs[256];
} Font;



static void draw_character(char8 character, float32 position_x, float32 position_y)
{
    int32 min_x = round_float32_to_int32(position_x);
    int32 min_y = round_float32_to_int32(position_y);
    int32 max_x = round_float32_to_int32(position_x + 8.0f);
    int32 max_y = round_float32_to_int32(position_y + 8.0f);

    int32 bitmap_offset_x = 8 * (character % 16);
    int32 bitmap_offset_y = 8 * (15 - character / 16);

    if (min_x < 0)
    {
        bitmap_offset_x += -min_x;
        min_x = 0;
    }
    if (min_y < 0)
    {
        bitmap_offset_y += -min_y;
        min_y = 0;
    }
    if (max_x > game->offscreen.width)
    {
        max_x = game->offscreen.width;
    }
    if (max_y > game->offscreen.height)
    {
        max_y = game->offscreen.height;
    }

    uint32* offscreen_row = (uint32*)game->offscreen.image.memory + min_y * game->offscreen.width + min_x;
    uint32* bitmap_row = (uint32*)game->font.memory + bitmap_offset_y * game->font.width + bitmap_offset_x;
    for (int32 y = min_y; y < max_y; y += 1)
    {
        uint32* offscreen_pixel = offscreen_row;
        uint32* bitmap_pixel = bitmap_row;
        for (int32 x = min_x; x < max_x; x += 1)
        {
            if (*bitmap_pixel & 0x00FFFFFF)
            {
                uint32 pixel = *bitmap_pixel;
                *offscreen_pixel = (pixel & 0xFF00FF00)
                                | ((pixel & 0x000000FF) << 16)
                                | ((pixel & 0x00FF0000) >> 16);
            }

            offscreen_pixel += 1;
            bitmap_pixel += 1;
        }
        offscreen_row += game->offscreen.width;
        bitmap_row += game->font.width;
    }

}

static void draw_text(char8* message, int64 length, float32 position_x, float32 position_y)
{
    for (int32 i = 0; i < length; i += 1)
    {
        draw_character((char8)message[i], position_x, position_y);
        position_x += 8.0f;
    }
}

static void draw_circle(uint8 red, uint8 green, uint8 blue, float32 x, float32 y, float32 radius)
{
    int32 left = round_float32_to_int32(x - radius);
    int32 bottom = round_float32_to_int32(y - radius);
    int32 right = round_float32_to_int32(x + radius);
    int32 top = round_float32_to_int32(y + radius);

    if (left < 0)
    {
        left = 0;
    }
    if (bottom < 0)
    {
        bottom = 0;
    }
    if (right > game->offscreen.width)
    {
        right = game->offscreen.width;
    }
    if (top > game->offscreen.height)
    {
        top = game->offscreen.height;
    }

    for (int32 i = bottom; i < top; i += 1)
    {
        for (int32 j = left; j < right; j += 1)
        {
            float32 distance = square_root(square((j - x + 0.5f)) + square((i - y + 0.5f)));

            if (distance < radius)
            {
                game->offscreen.image.memory[i * game->offscreen.width + j] = (uint32)(red | green << 8 | blue << 16);
            }
        }
    }
}

static Image load_bitmap(char8* file_name, int64 file_name_length, void* buffer)
{
    Image result = { 0 };

    int64 file_size = get_file_size(file_name, file_name_length);
    if (read_file(file_name, file_name_length, buffer))
    {
        Bitmap_file_header bitmap_file_header = { 0 };

        bitmap_file_header.bitmap_offset = *(uint32*)((char8*)buffer + 10);
        bitmap_file_header.bitmap_width = *(int32*)((char8*)buffer + 18);
        bitmap_file_header.bitmap_height = *(int32*)((char8*)buffer + 22);
        bitmap_file_header.bitmap_size = *(uint32*)((char8*)buffer + 34);

        result.memory = (uint32*)((char8*)buffer + bitmap_file_header.bitmap_offset);
        result.size = bitmap_file_header.bitmap_size;
        result.width = bitmap_file_header.bitmap_width;
        result.height = bitmap_file_header.bitmap_height;
    }

    return result;
}

static void render_bitmap(Image bitmap, float32 xx, float32 yy)
{
    int32 position_x = round_float32_to_int32(xx);
    int32 position_y = round_float32_to_int32(yy);

    int32 min_x = position_x - bitmap.width / 2;
    int32 min_y = position_y - bitmap.height / 2;
    int32 max_x = position_x + bitmap.width / 2;
    int32 max_y = position_y + bitmap.height / 2;

    int32 bitmap_offset_x = 0;
    int32 bitmap_offset_y = 0;

    if (min_x < 0)
    {
        bitmap_offset_x = -min_x;
        min_x = 0;
        //bitmap_offset_x = -(position_x - bitmap.width / 2);
    }
    if (min_y < 0)
    {
        bitmap_offset_y = -min_y;
        min_y = 0;
        //bitmap_offset_y = -(position_y - bitmap.height / 2);
    }
    if (max_x > game->offscreen.width)
    {
        max_x = game->offscreen.width;
    }
    if (max_y > game->offscreen.height)
    {
        max_y = game->offscreen.height;
    }

    uint32* offscreen_row = (uint32*)game->offscreen.image.memory + min_y * game->offscreen.width + min_x;
    uint32* bitmap_row = (uint32*)bitmap.memory + bitmap_offset_y * bitmap.width + bitmap_offset_x;
    for (int32 y = min_y; y < max_y; y += 1)
    {
        uint32* offscreen_pixel = offscreen_row;
        uint32* bitmap_pixel = bitmap_row;
        for (int32 x = min_x; x < max_x; x += 1)
        {
            if (*bitmap_pixel & 0xFF000000)
            {
                uint32 pixel = *bitmap_pixel;
                *offscreen_pixel = (pixel & 0xFF00FF00)
                                | ((pixel & 0x000000FF) << 16)
                                | ((pixel & 0x00FF0000) >> 16);
            }

            offscreen_pixel += 1;
            bitmap_pixel += 1;
        }
        offscreen_row += game->offscreen.width;
        bitmap_row += bitmap.width;
    }
}
