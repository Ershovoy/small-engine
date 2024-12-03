#include "game.h"
#include "math.c"
#include "input.c"
#include "renderer.c"
#include "platform_api.c"

static bool32 initialize_game()
{
    offscreen.width = DEFAULT_RESOLUTION;
    offscreen.height = DEFAULT_RESOLUTION;
    offscreen.memory = buffer;

    start_time = get_time_tick();

    return 1;
}

static void render_game()
{
    clear(offscreen, BLACK);

    draw_circle(offscreen, RED, 32, 32, 8);
    draw_circle(offscreen, RED, 1000, 1000, 32);
    draw_circle(offscreen, RED, 0, 1000, 32);
    //draw_circle(offscreen, RED, 256 / 2, 256 / 2, 128);
    draw_circle(offscreen, RED, 64 / 2, 64 / 2, 32);
    present_offscreen(offscreen);
}

static void update_game()
{
    if (is_button_pressed(BUTTON_WHEEL_UP))
    {
        offscreen.width /= 2;
        offscreen.height /= 2;

        if (offscreen.width == 0)
        {
            offscreen.width = 1;
            offscreen.height = 1;
        }
    }
    if (is_button_pressed(BUTTON_WHEEL_DOWN))
    {
        offscreen.width *= 2;
        offscreen.height *= 2;

        if (offscreen.width > MAX_RESOLUTION)
        {
            offscreen.width = MAX_RESOLUTION;
            offscreen.height = MAX_RESOLUTION;
        }
    }

    game.tick += 1;

    adjust_input();
}

static void game_loop()
{
    current_time = get_time_tick() - start_time;
    uint64 delta_time = current_time - previous_time;
    previous_time = current_time;
    accumulator += delta_time;
    while (accumulator >= time_per_update)
    {
        if (accumulator > 1'000'000'000)
        {
            accumulator = time_per_update;
        }

        update_game();

        accumulator -= time_per_update;
        if (accumulator < time_per_update)
        {
            render_game();
        }
    }
}
