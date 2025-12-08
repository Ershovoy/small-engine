#include "game.h"
#include "math.c"
#include "string.c"
#include "input.c"
#include "renderer.c"
#include "platform_api.c"
#include "arena.c"
// #include "sound.c"

static bool32 initialize_game()
{
    Arena arena = { 0 };
    arena_initialize(&arena, GIGABYTES(1));
    game = arena_allocate(&arena, sizeof(Game));

    Image image = { 0 };
    image.memory = arena_allocate(&arena, MAX_GAME_HORIZONTAL_RESOLUTION * MAX_GAME_VERTICAL_RESOLUTION);
    image.width = MAX_GAME_HORIZONTAL_RESOLUTION;
    image.height = MAX_GAME_VERTICAL_RESOLUTION;

    Image_view image_view = { 0 };
    image_view.image = image;
    image_view.width = GAME_HORIZONTAL_RESOLUTION;
    image_view.height = GAME_VERTICAL_RESOLUTION;

    game->offscreen = image_view;

    game->time_per_update = (uint64)1'000'000'000 / 60;
    game->start_time = get_time_tick();

    #define SOCKET_BUFFER_SIZE 256
    char8 socket_message[SOCKET_BUFFER_SIZE] = "Hello, Socket!\n";
    bool32 result1 = platform_api.net_send(socket_message, SOCKET_BUFFER_SIZE);

    char8 socket_buffer[SOCKET_BUFFER_SIZE];
    bool32 result2 = platform_api.net_receive(socket_buffer, SOCKET_BUFFER_SIZE);
    result2 = platform_api.net_receive(socket_buffer, SOCKET_BUFFER_SIZE);

    return 1;
}

static void render_game()
{
    Color clear_color = { 0 };
    clear_color.blue = (uint8)(game->tick % 256);
    clear_color.red = (uint8)(game->tick / 2 % 256);
    clear_color.green = (uint8)(game->tick / 4 % 256);
    clear(game->offscreen, clear_color);

    Vec2 position;
    position.e1 = game->x;
    position.e2 = game->y;

    draw_circle(255, 0, 255,position.e1, position.e2, 10);

    //draw_horizontal_line(255, 255, 255, 0);
    //draw_horizontal_line(255, 255, 255, (float32)game->offscreen.height);
    draw_vertical_line(255, 255, 255, 0);
    draw_vertical_line(255, 255, 255, (float32)game->offscreen.width);

    // draw_circle(offscreen, RED, 32, 32, 8);
    // draw_circle(offscreen, RED, 1000, 1000, 32);
    // draw_circle(offscreen, RED, 0, 1000, 32);
    // draw_circle(offscreen, RED, 64 / 2, 64 / 2, 32);
    // for(int32 i = 0; i < 32; i += 1)
    // {
    //     draw_rectangle(offscreen, MID_GREEN, 8 * (i - 1), 8 * (i - 1), 8 * i, 8 * i);
    // }

    // //draw_circle(offscreen, RED, 256 / 2, 256 / 2, 128);
    present_offscreen(game->offscreen);
}

static void update_game()
{
    console_write(STRING_LITERAL("Hello, World!\n"));
    if (is_button_pressed(BUTTON_WHEEL_UP))
    {
        // camera.width /= 2;
        // camera.height /= 2;

        // if (camera.width == 0)
        // {
        //     camera.width = 1;
        //     camera.height = 1;
        // }
    }
    if (is_button_pressed(BUTTON_WHEEL_DOWN))
    {
        // camera.width *= 2;
        // camera.height *= 2;

        // if (offscreen.width > MAX_RESOLUTION)
        // {
        //     offscreen.width = MAX_RESOLUTION;
        //     offscreen.height = MAX_RESOLUTION;
        // }
    }
    if (is_button_down(KEY_W))
    {
        game->y += 1.13f;
    }
    if (is_button_down(KEY_S))
    {
        game->y -= 1.13f;
    }
    if (is_button_down(KEY_D))
    {
        game->x += 1.13f;
    }
    if (is_button_down(KEY_A))
    {
        game->x -= 1.13f;
    }

    if (is_button_down(KEY_UP))
    {
        //camera.y += 1.0f;
    }
    if (is_button_down(KEY_DOWN))
    {
       // camera.y -= 1.0f;
    }
    if (is_button_down(KEY_LEFT))
    {
        //camera.x -= 1.0f;
    }
    if (is_button_down(KEY_RIGHT))
    {
        //camera.x += 1.0f;
    }


    game->tick += 1;

    adjust_input();
}

static void game_loop()
{
    // Сервер
    // получаем инпуты игроков
    // ждем наступление тика
    // отправляем инпуты

    // Клиент
    // Ждем инпут
    // симуляция
    game->current_time = get_time_tick() - game->start_time;
    uint64 delta_time = game->current_time - game->previous_time;
    game->previous_time = game->current_time;
    game->accumulator += delta_time;
    while (game->accumulator >= game->time_per_update)
    {
        if (game->accumulator > 1'000'000'000)
        {
            game->accumulator = game->time_per_update;
        }

        update_game();

        game->accumulator -= game->time_per_update;
        if (game->accumulator < game->time_per_update)
        {
            render_game();
        }
    }
}
