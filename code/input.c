static void process_button(Button button, bool32 is_down)
{
    if (is_down)
    {
        if (!input.buttons[button].is_down)
        {
            input.buttons[button].is_pressed = 1;
            input.buttons[button].is_down = 1;
        }
    }
    else
    {
        if (input.buttons[button].is_down)
        {
            input.buttons[button].is_released = 1;
        }
    }
}

static void process_mouse(int32 x, int32 y, int32 width, int32 height)
{
    input.mouse.x = (float32)(x * game->offscreen.width) / width;
    input.mouse.y = (float32)(y * game->offscreen.height) / height;
}

static bool32 is_button_pressed(Button button)
{
    return input.buttons[button].is_pressed;
}

static bool32 is_button_down(Button button)
{
    return input.buttons[button].is_down;
}

static bool32 is_button_release(Button button)
{
    return input.buttons[button].is_released;
}

static void reset_input()
{
    for (int32 i = 0; i < BUTTONS_COUNT; i += 1)
    {
        input.buttons[i] = (Button_state){ 0 };
    }
}

static void adjust_input()
{
    for (int32 i = 0; i < BUTTONS_COUNT; i += 1)
    {
        if (input.buttons[i].is_down)
        {
            if (input.buttons[i].is_pressed)
            {
                input.buttons[i].is_pressed = 0;
            }

            if (input.buttons[i].is_released)
            {
                input.buttons[i].is_down = 0;
                input.buttons[i].is_released = 0;
            }
        }
    }
}
