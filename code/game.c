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
        Tick_input tick_input = game->previous_tick_input;
        tick_input.tick = game->state.tick;

        uint32 out_ip = { 0 };
        uint16 out_port = { 0 };
        uint8 packet_buffer[KILOBYTES(2)] = { 0 };
        while (net_receive(&packet_buffer, sizeof(packet_buffer), &out_ip, &out_port))
        {
            Packet_header* packet_header = (Packet_header*)packet_buffer;
            void* packet_payload = (uint8*)packet_buffer;
            switch (packet_header->type)
            {
                case PACKET_CONNECT:
                {
                    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
                    {
                        if (!game->is_connected[i])
                        {
                            Packet_game_state packet_game_state = { 0 };
                            packet_game_state.header.type = PACKET_GAME_STATE;
                            packet_game_state.game_state = game->state;
                            net_send(&packet_game_state, sizeof(Packet_game_state), out_ip, out_port);

                            game->ips[i] = out_ip;
                            game->ports[i] = out_port;
                            game->is_connected[i] = 1;
                            game->last_packet_time[i] = game->current_time;

                            break;
                        }
                    }

                    break;
                }
                case PACKET_DISCONNECT:
                {
                    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
                    {
                        if (game->ips[i] == out_ip && game->ports[i] == out_port && game->is_connected[i])
                        {
                            game->ips[i] = 0;
                            game->ports[i] = 0;
                            game->is_connected[i] = 0;

                            break;
                        }
                    }

                    break;
                }
                // TODO: If case client lag, server will discard all accumulated inputs except for the last one.
                case PACKET_PLAYER_INPUT:
                {
                    Packet_player_input* packet_player_input = (Packet_player_input*)packet_payload;

                    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
                    {
                        if (game->ips[i] == out_ip && game->ports[i] == out_port && game->is_connected[i])
                        {
                            tick_input.player_inputs[i] = packet_player_input->player_input;
                            game->last_packet_time[i] = game->current_time;

                            break;
                        }
                    }

                    break;
                }
                default:
                {
                    break;
                }
            }

        }

        for (int32 i = 0; i < MAX_PLAYERS; i += 1)
        {
            Packet_disconnect packet_disconnect = { 0 };
            packet_disconnect.header.type = PACKET_DISCONNECT;

            if (game->is_connected[i] && game->last_packet_time[i] + game->time_per_update * 600 < game->current_time)
            {
                net_send(&packet_disconnect, sizeof(Packet_disconnect), game->ips[i], game->ports[i]);

                game->ips[i] = 0;
                game->ports[i] = 0;
                game->is_connected[i] = 0;
                game->last_packet_time[i] = 0;
            }
        }

        game->tick_input_server_batch[tick_input.tick % TICK_INPUT_BATCH_SIZE] = tick_input;

        for (int32 i = 0; i < MAX_PLAYERS; i += 1)
        {
            Packet_tick_input_batch packet_tick_input_batch = { 0 };
            packet_tick_input_batch.header.type = PACKET_TICK_INPUT_BATCH;
            for (int32 i = 0; i < TICK_INPUT_BATCH_SIZE; i += 1)
            {
                packet_tick_input_batch.tick_inputs[i] = game->tick_input_server_batch[i];
            }

            // Packet_tick_input packet_tick_input = { 0 };
            // packet_tick_input.header.type = PACKET_TICK_INPUT;
            // packet_tick_input.tick_input = tick_input;

            if (game->is_connected[i] && tick_input.tick % 32 != 0)
            {
                net_send(&packet_tick_input_batch, sizeof(Packet_tick_input_batch), game->ips[i], game->ports[i]);
                // net_send(&packet_tick_input, sizeof(Packet_tick_input), game->ips[i], game->ports[i]);
            }
        }

        update_game_state(&game->state, tick_input, game->time_per_update / 1'000'000'000.0f);
        render_game_state(&game->state);

        game->previous_tick_input = tick_input;

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
    uint8 packet_buffer[KILOBYTES(4)] = { 0 };
    while (net_receive(&packet_buffer, sizeof(packet_buffer), &out_ip, &out_port))
    {
        Packet_header* packet_header = (Packet_header*)packet_buffer;
        switch (packet_header->type)
        {
            case PACKET_TICK_INPUT_BATCH:
            {
                Packet_tick_input_batch* packet_tick_input_batch = (Packet_tick_input_batch*)packet_buffer;

                for (int32 i = 0; i < TICK_INPUT_BATCH_SIZE; i += 1)
                {
                    if (game->state.tick <= packet_tick_input_batch->tick_inputs[i].tick)
                    {
                        int32 index = packet_tick_input_batch->tick_inputs[i].tick % MAX_BUFFERED_TICKS;
                        if (!game->tick_input_valid[index])
                        {
                            game->tick_input_buffer[index] = packet_tick_input_batch->tick_inputs[i];
                            game->tick_input_valid[index] = 1;
                        }
                        //else
                        {
                            //*(int32*)0 = 0;
                        }
                    }
                }

                break;
            }
            case PACKET_TICK_INPUT:
            {
                Packet_tick_input* packet_tick_input = (Packet_tick_input*)packet_buffer;

                int32 index = packet_tick_input->tick_input.tick % MAX_BUFFERED_TICKS;
                char8 bs[32];
                String s = { 0, bs };
                s.length = int32_to_string(bs, 32, (int32)packet_tick_input->tick_input.tick);
                console_write(s);
                console_write(STRING_LITERAL("\n"));
                if (!game->tick_input_valid[index])
                {
                    game->tick_input_buffer[index] = packet_tick_input->tick_input;
                    game->tick_input_valid[index] = 1;
                }
                else
                {
                    *(int32*)0 = 0;
                }

                break;
            }
            case PACKET_GAME_STATE:
            {
                Packet_game_state* packet_game_state = (Packet_game_state*)packet_buffer;

                game->state = packet_game_state->game_state;

                break;
            }
            case PACKET_DISCONNECT:
            {
                game->state = (Game_state){ 0 };
            }
            default:
            {
                break;
            }
        }
    }

    int32 buffered_ticks = 0;
    for (int32 i = 0; i < MAX_BUFFERED_TICKS; i += 1)
    {
        if (game->tick_input_valid[i])
        {
            buffered_ticks += 1;
        }
    }

    int64 time_scale = 1;
    if (buffered_ticks > MAX_BUFFERED_TICKS / 8 / 2)
    {
        time_scale = 2;

    }
    if (buffered_ticks == MAX_BUFFERED_TICKS)
    {
        *(int32*)0 = 0;
    }

    game->update_accumulator += delta_time * time_scale;
    if (game->update_accumulator >= game->time_per_update)
    {
        Player_input player_input = collect_player_input();

        Packet_player_input packet_player_input = { 0 };
        packet_player_input.header.type = PACKET_PLAYER_INPUT;
        packet_player_input.player_input = player_input;
        net_send(&packet_player_input, sizeof(Packet_player_input), SERVER_IP, SERVER_PORT);

        int32 index = game->state.tick % MAX_BUFFERED_TICKS;
        if (game->tick_input_valid[index] && game->tick_input_buffer[index].tick == game->state.tick)
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

// static void three_in_one()
// {
//     game->current_time = get_time_tick() - game->start_time;
//     int64 delta_time = game->current_time - game->previous_time;
//     if (delta_time > game->time_per_update)
//     {
//         delta_time = game->time_per_update;
//     }

//     game->update_accumulator += delta_time;
//     if (game->update_accumulator >= game->time_per_update)
//     {
//         Tick_input tick_input = { 0 };

//         bool32 should_update = 0;

//         if (game->is_server)
//             should_update = should_server_tick(&tick_input);
//         else if (game->is_client)
//             should_update = should_client_tick(&tick_input);
//         else
//             should_update = should_offline_tick(&tick_input);

//         if (should_update)
//         {
//             game->previous_state = game->state;
//             game->previous_tick_input = tick_input;

//             update_game_state(&game->state, tick_input, game->time_per_update / 1'000'000'000.0f);
//             if (game->is_server)
//             {
//                 render_game_state(&game->state);
//             }
//         }

//         adjust_input();

//         game->previous_frame_time = game->current_time;
//         game->update_accumulator -= game->time_per_update;
//     }


//     if (delta_time > game->time_per_frame)
//     {
//         delta_time = game->time_per_frame;
//     }

//     game->frame_accumulator += delta_time;
//     if (game->frame_accumulator >= game->time_per_frame)
//     {
//         if (!game->is_server)
//         {
//             update_game_state(&game->previous_state, game->previous_tick_input, (game->current_time - game->previous_frame_time) / 1'000'000'000.0f);
//             render_game_state(&game->previous_state);
//         }

//         game->previous_frame_time = game->current_time;
//         game->frame_accumulator -= game->time_per_frame;
//     }

//     game->previous_time = game->current_time;
// }

static void game_loop()
{
    if (is_button_pressed(KEY_F1))
    {
        if (game->is_offline)
        {
            game->is_client = 1;
            game->is_offline = 0;

            Packet_connect packet_connet = { 0 };
            packet_connet.header.type = PACKET_CONNECT;

            net_send(&packet_connet, sizeof(Packet_player_input), SERVER_IP, SERVER_PORT);
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
