static void clear(Image image, Color color)
{
    byte* image_row = image.memory;
    for (int32 y = 0; y < image.height; y += 1)
    {
        byte* image_pixel = image_row;
        for (int32 x = 0; x < image.width; x += 1)
        {
            *image_pixel = (byte)color;

            image_pixel += 1;
        }
        image_row += image.width;
    }
}

static void draw_pixel(Image image, Color color, int32 x, int32 y)
{
    if (x >= 0 && y >= 0 && x < image.width && y < image.height)
    {
        image.memory[image.width * y + x] = (byte)color;
    }
}

static void draw_image(Image destination, Image source, int32 left, int32 bottom)
{

}

static void draw_rectangle(Image image, Color color, int32 left, int32 bottom, int32 right, int32 top)
{
    for (int32 y = bottom; y < top; y += 1)
    {
        for (int32 x = left; x < right; x += 1)
        {
            draw_pixel(image, color, x, y);
        }
    }
}

static void draw_line(Image image,  Color color, int32 x1, int32 y1, int32 x2, int32 y2)
{

}

static void draw_circle(Image image, Color color, int32 center_x, int32 center_y, int32 radius)
{
    int32 left = center_x - radius;
    int32 bottom = center_y - radius;
    int32 right = center_x + radius;
    int32 top = center_y + radius;
    for (int32 y = bottom; y < top; y += 1)
    {
        for (int32 x = left; x < right; x += 1)
        {
            float32 distance_x = (x - center_x + 0.5f);
            float32 distance_y = (y - center_y + 0.5f);
            float32 distance = square_root(distance_x * distance_x + distance_y * distance_y);
            if(distance <= radius)
            {
                draw_pixel(image, color, x, y);
            }
        }
    }
}
