#include "game.h"
#include "fixed_point.c"
#include "math.c"
#include "input.c"
#include "platform_api.c"
#include "arena.c"
#include "renderer.c"
#include "string.c"
#include "pool.c"
#include "game_state.c"
#include "game_render.c"
#include "network.c"
#include "sound.c"

static bool32 initialize_game()
{
    Arena arena = { 0 };
    arena_initialize(&arena, MEGABYTES(256 * 2));
    game = arena_allocate(&arena, sizeof(Game));

    Image image = { 0 };
    image.size = GAME_MAX_HORIZONTAL_RESOLUTION * GAME_MAX_VERTICAL_RESOLUTION * 32;
    image.memory = arena_allocate(&arena, image.size);
    image.width = GAME_MAX_HORIZONTAL_RESOLUTION;
    image.height = GAME_MAX_VERTICAL_RESOLUTION;

    Image_view image_view = { 0 };
    image_view.image = image;
    image_view.width = GAME_DEFAULT_HORIZONTAL_RESOLUTION;
    image_view.height = GAME_DEFAULT_VERTICAL_RESOLUTION;

    game->offscreen = image_view;

    game->time_per_update = (int64)1'000'000'000 / 60;
    game->start_time = get_time_tick();

    game->time_per_frame = (int64)1'000'000'000 / 240;


    arena_initialize(&game->sound_arena, MEGABYTES(32));
    game->test_sound = read_sound_file(STRING_LITERAL("sample.wav"));

    arena_initialize(&game->image_arena, MEGABYTES(128));

    game->font = load_bitmap(STRING_LITERAL("font.bmp"));
    game->background = load_bitmap(STRING_LITERAL("background.bmp"));
    game->ball = load_bitmap(STRING_LITERAL("ball.bmp"));
    game->player = load_bitmap(STRING_LITERAL("blobby.bmp"));

    game->mode = GAME_MODE_OFFLINE;
    game->screen = GAME_SCREEN_MENU;

    arena_initialize(&game->string_arena, MEGABYTES(16));

    initialize_game_state(&game->state);

    return 1;
}

static void deinitialize_game()
{

}

static Player_input collect_second_player_input()
{
    Player_input player_input = { 0 };

    if (is_button_down(KEY_LEFT))
    {
        player_input.left = 1;
    }
    if (is_button_down(KEY_UP))
    {
        player_input.up = 1;
    }
    if (is_button_down(KEY_RIGHT))
    {
        player_input.right = 1;
    }

    return player_input;
}

static Player_input collect_player_input()
{
    Player_input player_input = { 0 };

    if (is_button_pressed(KEY_E))
    {
        play_sound(game->test_sound);
    }

    if (is_button_down(KEY_W))
    {
        player_input.up = 1;
    }
    if (is_button_down(KEY_A))
    {
        player_input.left = 1;
    }
    if (is_button_down(KEY_D))
    {
        player_input.right = 1;
    }

    if (is_button_pressed(BUTTON_WHEEL_UP))
    {
        game->offscreen.width -= 10;
        game->offscreen.height -= 10;
    }
    if (is_button_pressed(BUTTON_WHEEL_DOWN))
    {
        game->offscreen.width += 10;
        game->offscreen.height += 10;
    }

    if (game->offscreen.width > GAME_MAX_HORIZONTAL_RESOLUTION)
    {
        game->offscreen.width = GAME_MAX_HORIZONTAL_RESOLUTION;
    }
    if (game->offscreen.width < GAME_MIN_HORIZONTAL_RESOLUTION)
    {
        game->offscreen.width = GAME_MIN_HORIZONTAL_RESOLUTION;
    }

    if (game->offscreen.height > GAME_MAX_VERTICAL_RESOLUTION)
    {
        game->offscreen.height = GAME_MAX_VERTICAL_RESOLUTION;
    }
    if (game->offscreen.height < GAME_MIN_VERTICAL_RESOLUTION)
    {
        game->offscreen.height = GAME_MIN_VERTICAL_RESOLUTION;
    }

    return player_input;
}

static bool32 should_offline_tick(Tick_input* tick_input)
{
    tick_input->player_inputs[0] = collect_player_input();
    tick_input->player_inputs[1] = collect_second_player_input();

    return 1;
}

static bool32 should_server_tick(Tick_input* tick_input)
{
	*tick_input = game->previous_tick_input;
	tick_input->tick = game->state.tick;
	tick_input->hash = hash_fnv1a(&game->state, sizeof(Game_state));

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
				for (int64 i = 0; i < MAX_PLAYERS; i += 1)
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
				for (int64 i = 0; i < MAX_PLAYERS; i += 1)
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

				for (int64 i = 0; i < MAX_PLAYERS; i += 1)
				{
					if (game->ips[i] == out_ip && game->ports[i] == out_port && game->is_connected[i])
					{
						tick_input->player_inputs[i] = packet_player_input->player_input;
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

	for (int64 i = 0; i < MAX_PLAYERS; i += 1)
	{
		Packet_disconnect packet_disconnect = { 0 };
		packet_disconnect.header.type = PACKET_DISCONNECT;

		if (game->is_connected[i] && game->last_packet_time[i] + game->time_per_update * MAX_BUFFERED_TICKS < game->current_time)
		{
			net_send(&packet_disconnect, sizeof(Packet_disconnect), game->ips[i], game->ports[i]);

			game->ips[i] = 0;
			game->ports[i] = 0;
			game->is_connected[i] = 0;
			game->last_packet_time[i] = 0;
		}
	}

	game->tick_input_server_batch[tick_input->tick % TICK_INPUT_BATCH_SIZE] = *tick_input;

	for (int64 i = 0; i < MAX_PLAYERS; i += 1)
	{
		Packet_tick_input_batch packet_tick_input_batch = { 0 };
		packet_tick_input_batch.header.type = PACKET_TICK_INPUT_BATCH;
		for (int64 i = 0; i < TICK_INPUT_BATCH_SIZE; i += 1)
		{
			packet_tick_input_batch.tick_inputs[i] = game->tick_input_server_batch[i];
		}

		// Packet_tick_input packet_tick_input = { 0 };
		// packet_tick_input.header.type = PACKET_TICK_INPUT;
		// packet_tick_input.tick_input = tick_input;

		if (game->is_connected[i] && (tick_input->tick % 32) > 16)
			// if (game->is_connected[i])
		{
			net_send(&packet_tick_input_batch, sizeof(Packet_tick_input_batch), game->ips[i], game->ports[i]);
			// net_send(&packet_tick_input, sizeof(Packet_tick_input), game->ips[i], game->ports[i]);
		}
	}

	return 1;
}

static bool32 should_client_tick(Tick_input* tick_input)
{
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

                for (int64 i = 0; i < TICK_INPUT_BATCH_SIZE; i += 1)
                {
                    if (game->state.tick <= packet_tick_input_batch->tick_inputs[i].tick)
                    {
                        int64 index = packet_tick_input_batch->tick_inputs[i].tick % MAX_BUFFERED_TICKS;
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

                int64 index = packet_tick_input->tick_input.tick % MAX_BUFFERED_TICKS;

                if (!game->tick_input_valid[index])
                {
                    game->tick_input_buffer[index] = packet_tick_input->tick_input;
                    game->tick_input_valid[index] = 1;
                }
                else
                {
                    *(int64*)0 = 0;
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

	Player_input player_input = collect_player_input();
	Packet_player_input packet_player_input = { 0 };
	packet_player_input.header.type = PACKET_PLAYER_INPUT;
	packet_player_input.player_input = player_input;
	net_send(&packet_player_input, sizeof(Packet_player_input), SERVER_IP, SERVER_PORT);

	int64 index = game->state.tick % MAX_BUFFERED_TICKS;
	if (game->tick_input_valid[index] && game->tick_input_buffer[index].tick == game->state.tick)
	{
		if (hash_fnv1a(&game->state, sizeof(Game_state)) != game->tick_input_buffer[index].hash)
		{
			*(int64*)0 = 0;
		}

		*tick_input = game->tick_input_buffer[index];

		game->tick_input_valid[index] = 0;

		return 1;
	}

	return 0;
}

static int64 calculate_time_scale()
{
	int64 result = 1;
	if (game->mode == GAME_MODE_CLIENT)
	{
		result = 2;
	}
	return result;
}

static void game_loop()
{
    if (game->mode == GAME_MODE_OFFLINE)
    {
        if (is_button_pressed(KEY_F1))
        {
            game->mode = GAME_MODE_CLIENT;

            Packet_connect packet_connet = { 0 };
            packet_connet.header.type = PACKET_CONNECT;

            net_send(&packet_connet, sizeof(Packet_connect), SERVER_IP, SERVER_PORT);

        }
        if (is_button_pressed(KEY_F2))
        {
            game->mode = GAME_MODE_SERVER;
            uint16 binded_port = net_bind(0xFFFF);
            initialize_game_state(&game->state);
        }
    }

    game->current_time = get_time_tick() - game->start_time;
    int64 delta_time = game->current_time - game->previous_time;
    if (delta_time > game->time_per_update)
    {
        delta_time = game->time_per_update;
    }

    int64 time_scale = calculate_time_scale();
    game->update_accumulator += delta_time * time_scale;
    if (game->update_accumulator >= game->time_per_update)
    {
        Tick_input tick_input = { 0 };

        bool32 should_update = 0;

        if (game->mode == GAME_MODE_SERVER)
            should_update = should_server_tick(&tick_input);
        else if (game->mode == GAME_MODE_CLIENT)
            should_update = should_client_tick(&tick_input);
        else
            should_update = should_offline_tick(&tick_input);

        if (should_update)
        {
            game->previous_state = game->state;
            game->previous_tick_input = tick_input;

            int64 update_start_time = get_time_tick();
            update_game_state(&game->state, tick_input, game->time_per_update / 1'000'000'000.0f);
            game->update_time = get_time_tick() - update_start_time;
            if (game->mode == GAME_MODE_SERVER)
            {
                render_game_state(&game->state);
            }
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
        if (game->mode != GAME_MODE_SERVER)
        {
            int64 frame_start_time = get_time_tick();
            update_game_state(&game->previous_state, game->previous_tick_input,
                (game->current_time - game->previous_frame_time) / 1'000'000'000.0f);

            render_game_state(&game->previous_state);
            game->frame_time = get_time_tick() - frame_start_time;
        }

        game->previous_frame_time = game->current_time;
        game->frame_accumulator -= game->time_per_frame;
    }

    game->previous_time = game->current_time;

    //sleep(1'000'000);
}
