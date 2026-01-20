// void resize_offscreen(int32 target_widht, int32 target_height)
// {

// }

static void clear(Image_view image_view, Color color)
{
    uint32* image_row = image_view.image.memory;
    for (int32 y = 0; y < image_view.height; y += 1)
    {
        uint32* image_pixel = image_row;
        for (int32 x = 0; x < image_view.width; x += 1)
        {
            *image_pixel = *(uint32*)&color;

            image_pixel += 1;
        }
        image_row += image_view.width;
    }
}

static void draw_pixel(Image_view image_view, Color color, Vec2 position)
{
    int32 x = round_float32_to_int32(position.e1);
    int32 y = round_float32_to_int32(position.e2);

    if (x >= 0 && y >= 0 && x < image_view.width && y < image_view.height)
    {
        image_view.image.memory[y * image_view.width + x] = *(uint32*)&color;
    }
}

// static void draw_image(Image destination, Image source, int32 left, int32 bottom)
// {

// }

static void draw_rectangle(Image_view image_view, Color color, int32 left, int32 bottom, int32 right, int32 top)
{
    for (int32 y = bottom; y < top; y += 1)
    {
        for (int32 x = left; x < right; x += 1)
        {
            if (x >= 0 && y >= 0 && x < image_view.width && y < image_view.height)
            {
                image_view.image.memory[y * image_view.width + x] = *(uint32*)&color;
            }
        }
    }
}

// static void draw_line(Image image,  Color color, int32 x1, int32 y1, int32 x2, int32 y2)
// {

// }

static void draw_horizontal_line(uint8 red, uint8 green, uint8 blue, float32 y)
{
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

// static void draw_circle(Color color, Vec2 position, float32 radius)
// {
//     int32 left = round_float32_to_int32(position.e1 - radius);
//     int32 bottom = round_float32_to_int32(position.e2 - radius);
//     int32 right = round_float32_to_int32(position.e1 + radius);
//     int32 top = round_float32_to_int32(position.e2 + radius);

//     if (left < 0)
//     {
//         left = 0;
//     }
//     if (bottom < 0)
//     {
//         bottom = 0;
//     }
//     if(right > offscreen_view.width)
//     {
//         right = offscreen_view.width;
//     }
//     if(top > offscreen_view.height)
//     {
//         top = offscreen_view.height;
//     }

//     for (int32 y = bottom; y < top; y += 1)
//     {
//         for (int32 x = left; x < right; x += 1)
//         {
//             float32 distance_x = (x - position.e1 + 0.5f);
//             float32 distance_y = (y - position.e2 + 0.5f);
//             float32 distance = square_root(distance_x * distance_x + distance_y * distance_y);

//             if(distance < radius)
//             {
//                 float32 alpha = (radius - distance) / 2.0f;
//                 if(alpha > 1.0f)
//                 {
//                     alpha = 1.0f;
//                 }
//                 Color color2 = *(Color*)&offscreen_view.image->memory[y * offscreen_view.width + x];
//                 Color blended = { 0 };
//                 blended.red = (uint8)round_float32_to_int32(color.red * alpha + color2.red * (1.0f - alpha));
//                 blended.green = (uint8)round_float32_to_int32(color.green * alpha + color2.green * (1.0f - alpha));
//                 blended.blue = (uint8)round_float32_to_int32(color.blue * alpha + color2.blue * (1.0f - alpha));

//                 offscreen_view.image->memory[y * offscreen_view.width + x] = *(uint32*)&blended;
//             }
//         }
//     }
// }
