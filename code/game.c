#include "game.h"
#include "fixed_point.c"
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

    game->is_offline = 1;

    initialize_game_state(&game->state);

    return 1;
}

static void deinitialize_game()
{

}

static Player_input collect_player_input()
{
    Player_input player_input = { 0 };
    player_input.cursor_x = float32_to_fixed32(input.mouse.x);
    player_input.cursor_y = float32_to_fixed32(input.mouse.y);
    if (is_button_pressed(KEY_E))
    {
        play_sound(game->test_sound);
    }

    return player_input;
}

// static Tick_input collect_server_tick_input()
// {

// }

// static bool32 collect_client_tick_input()
// {

// }

// static bool32 collect_offline_tick_input()
// {

// }

static void server()
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
        Tick_input tick_input = { 0 };
        tick_input.tick = game->state.tick;

        uint32 out_ip = { 0 };
        uint16 out_port = { 0 };
        Player_input player_input = { 0 };
        while(net_receive(&player_input, sizeof(Player_input), &out_ip, &out_port))
        {
            for (int32 i = 0; i < MAX_PLAYERS; i += 1)
            {
                if (game->ips[i] == out_ip && game->ports[i] == out_port)
                {
                    tick_input.player_inputs[i] = player_input;

                    break;
                }

                if (!game->is_connected[i])
                {
                    game->ips[i] = out_ip;
                    game->ports[i] = out_port;
                    game->is_connected[i] = 1;

                    tick_input.player_inputs[i] = player_input;

                    break;
                }
            }
        }

        bool32 is_all_players_connected = 1;
        for (int32 i = 0; i < MAX_PLAYERS; i += 1)
        {
            if (!game->is_connected[i])
            {
                is_all_players_connected = 0;
            }
        }

        if (is_all_players_connected)
        {
            for (int32 i = 0; i < MAX_PLAYERS; i += 1)
            {
                if (game->is_connected[i])
                {
                    net_send(&tick_input, sizeof(Tick_input), game->ips[i], game->ports[i]);
                }
            }

            update_game_state(&game->state, tick_input, game->time_per_update / 1'000'000'000.0f);
            render_game_state(&game->state);
        }

        game->update_accumulator -= game->time_per_update;
    }

    game->previous_time = game->current_time;
}

static void client()
{
    game->current_time = get_time_tick() - game->start_time;
    int64 delta_time = game->current_time - game->previous_time;
    if (delta_time > game->time_per_update)
    {
        delta_time = game->time_per_update;
    }

    uint32 out_ip = { 0 };
    uint16 out_port = { 0 };
    Tick_input tick_input = { 0 };
    while (net_receive(&tick_input, sizeof(Tick_input), &out_ip, &out_port))
    {
        int32 index = tick_input.tick % MAX_BUFFERED_TICKS;
        if (!game->tick_input_valid[index])
        {
            game->tick_input_buffer[index] = tick_input;
            game->tick_input_valid[index] = 1;
        }
        else
        {
            *(int32*)0 = 0;
        }
    }

    int64 time_scale = 0;


    game->update_accumulator += delta_time;
    if (game->update_accumulator >= game->time_per_update)
    {
        Player_input player_input = collect_player_input();
        net_send(&player_input, sizeof(Player_input), 0x7f000001, 0xFFFF); // 0x7f000001 = 127.0.0.1 // 0x5DAB0267 = 93.171.2.103 // 0xC0A8006A = 192.168.0.106

        int32 index = game->state.tick % MAX_BUFFERED_TICKS;
        if (game->tick_input_valid[index])
        {
            game->previous_state = game->state;
            game->previous_tick_input = game->tick_input_buffer[index];

            update_game_state(&game->state, game->tick_input_buffer[index], game->time_per_update / 1'000'000'000.0f);

            game->tick_input_valid[index] = 0;
        }

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
        update_game_state(&game->previous_state, game->previous_tick_input, (game->current_time - game->previous_frame_time) / 1'000'000'000.0f);
        render_game_state(&game->previous_state);

        game->previous_frame_time = game->current_time;
        game->frame_accumulator -= game->time_per_frame;
    }

    game->previous_time = game->current_time;

}

static void offline()
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
        Player_input input = collect_player_input();
        Tick_input tick_input = { 0 };
        tick_input.player_inputs[0] = input;

        game->previous_state = game->state;
        game->previous_tick_input = tick_input;

        update_game_state(&game->state, tick_input, game->time_per_update / 1'000'000'000.0f);

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
        update_game_state(&game->previous_state, game->previous_tick_input, (game->current_time - game->previous_frame_time) / 1'000'000'000.0f);
        render_game_state(&game->previous_state);

        game->previous_frame_time = game->current_time;
        game->frame_accumulator -= game->time_per_frame;
    }

    game->previous_time = game->current_time;
}

static void game_loop()
{
    if (is_button_pressed(KEY_F1))
    {
        if (game->is_offline)
        {
            game->is_client = 1;
            game->is_offline = 0;
            initialize_game_state(&game->state);
        }
    }
    if (is_button_pressed(KEY_F2))
    {
        if (game->is_offline)
        {
            game->is_server = 1;
            game->is_offline = 0;
            uint16 binded_port = net_bind(0xFFFF);
            initialize_game_state(&game->state);
        }
    }

    if (game->is_server)
    {
        server();
    }
    if (game->is_client)
    {
        client();
    }
    if (game->is_offline)
    {
        offline();
    }
}
