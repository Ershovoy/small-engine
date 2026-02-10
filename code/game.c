#include "game.h"
#include "math.c"
#include "string.c"
#include "input.c"
#include "platform_api.c"
#include "renderer.c"
#include "arena.c"
#include "game_state.c"
#include "game_render.c"
#include "network.c"
#include "sound.c"

static bool32 initialize_game()
{
    Arena arena = { 0 };
    arena_initialize(&arena, MEGABYTES(32));
    game = arena_allocate(&arena, sizeof(Game));

    Image image = { 0 };
    image.size = MAX_GAME_HORIZONTAL_RESOLUTION * MAX_GAME_VERTICAL_RESOLUTION;
    image.memory = arena_allocate(&arena, image.size);
    image.width = MAX_GAME_HORIZONTAL_RESOLUTION;
    image.height = MAX_GAME_VERTICAL_RESOLUTION;

    Image_view image_view = { 0 };
    image_view.image = image;
    image_view.width = GAME_HORIZONTAL_RESOLUTION;
    image_view.height = GAME_VERTICAL_RESOLUTION;

    game->offscreen = image_view;

    game->time_per_update = (int64)1'000'000'000 / 60;
    game->start_time = get_time_tick();

    game->time_per_frame = (int64)1'000'000'000 / 240;

    char8 file_name[] = "sample.wav";
    int64 file_size = get_file_size(file_name, lengthof(file_name));
    void* buffer = arena_allocate(&arena, file_size);
    game->test_sound = read_sound_file(file_name, lengthof(file_name), buffer);

    return 1;
}

static void deinitialize_game()
{
    // if (game->is_client)
    // {
    //     Packet_disconnect packet_disconnect = { 0 };
    //     packet_disconnect.header.type = PACKET_DISCONNECT;
    //     net_send(&packet_disconnect, sizeof(Packet_disconnect), 0x7f000001, 0xFFFF);
    // }
}

static Player_input update_game()
{
    Player_input player_input = { 0 };
    if (is_button_pressed(KEY_E))
    {
        play_sound(game->test_sound);
    }
    if (is_button_down(KEY_W))
    {
        player_input.is_up = 1;
    }
    if (is_button_down(KEY_A))
    {
        player_input.is_left = 1;
    }
    if (is_button_down(KEY_S))
    {
        player_input.is_down = 1;
    }
    if (is_button_down(KEY_D))
    {
        player_input.is_right = 1;
    }

    player_input.mouse_x = (int32)input.mouse.x;
    player_input.mouse_y = (int32)input.mouse.y;

    return player_input;
}

static void game_loop()
{
    game->current_time = get_time_tick() - game->start_time;
    int64 delta_time = game->current_time - game->previous_time;
    if (delta_time > game->time_per_update)
    {
        delta_time = game->time_per_update;
    }

    game->update_accumulator += delta_time;
    if (game->update_accumulator >= game->time_per_update)
    {
        Player_input input = update_game();

        game->previous_state = game->state;
        game->previous_input = input;

        update_game_state(&game->state, input, game->time_per_update / 1'000'000'000.0f);

        adjust_input();

        game->previous_frame_time = game->current_time;
        game->update_accumulator -= game->time_per_update;
    }

    if (delta_time > game->time_per_frame)
    {
        delta_time = game->time_per_frame;
    }

    game->frame_accumulator += delta_time;
    if (game->frame_accumulator >= game->time_per_frame)
    {
        update_game_state(&game->previous_state, game->previous_input, (game->current_time - game->previous_frame_time) / 1'000'000'000.0f);
        render_game_state(&game->previous_state);

        game->previous_frame_time = game->current_time;
        game->frame_accumulator -= game->time_per_frame;
    }

    if (game->frame_accumulator < game->time_per_frame / 4 &&
        game->update_accumulator < game->time_per_update / 4)
    {
        // Need to know how long each tick and frame takes
        // sleep(1'000);
    }

    game->previous_time = game->current_time;
}
