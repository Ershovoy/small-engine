#include "game.h"
#include "fixed_point.c"
#include "math.c"
#include "input.c"
#include "virtual_file_system.c"
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
    image_view.width = GAME_DEFAULT_HORIZONTAL_RESOLUTION / 4;
    image_view.height = GAME_DEFAULT_VERTICAL_RESOLUTION / 4;

    game->offscreen = image_view;

    game->time_per_update = (int64)1'000'000'000 / 75;
    game->start_time = get_time_tick();

    game->time_per_frame = (int64)1'000'000'000 / 75;


    arena_initialize(&game->sound_arena, MEGABYTES(64));
    // game->pfiff_sound = read_sound_file(STRING_LITERAL("pfiff.wav"));
    game->bums_sound = read_sound_file(STRING_LITERAL("bums.wav"));
    game->chat_sound = read_sound_file(STRING_LITERAL("chat.wav"));

    arena_initialize(&game->image_arena, MEGABYTES(128));

    game->cursor = load_bitmap(STRING_LITERAL("cursor.bmp"));
    game->font = load_bitmap(STRING_LITERAL("font.bmp"));
    game->background = load_bitmap(STRING_LITERAL("background.bmp"));
    game->ball = load_bitmap(STRING_LITERAL("ball.bmp"));
    game->player = load_bitmap(STRING_LITERAL("blobby.bmp"));

    // game->mode = GAME_MODE_OFFLINE;
    game->screen = GAME_SCREEN_MENU;

    game->ip_digits[0][0] = '1';
    game->ip_digits[0][1] = '2';
    game->ip_digits[0][2] = '7';
    game->ip_digits[3][0] = '1';
    game->port_digits[0] = '6';
    game->port_digits[1] = '5';
    game->port_digits[2] = '5';
    game->port_digits[3] = '3';
    game->port_digits[4] = '5';

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
        play_sound(game->pfiff_sound);
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
        // game->offscreen.width -= 10;
        // game->offscreen.height -= 10;
    }
    if (is_button_pressed(BUTTON_WHEEL_DOWN))
    {
        // game->offscreen.width += 10;
        // game->offscreen.height += 10;
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

		// if (game->is_connected[i] && (tick_input->tick % 32) > 16)
		if (game->is_connected[i])
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
        else if (game->mode == GAME_MODE_OFFLINE)
            should_update = should_offline_tick(&tick_input);

        if (should_update)
        {
            game->previous_state = game->state;
            game->previous_tick_input = tick_input;

            int64 update_start_time = get_time_tick();
            update_game_state(&game->state, tick_input, game->time_per_update / 1'000'000'000.0f, 1);
            game->update_time = get_time_tick() - update_start_time;
            if (game->mode == GAME_MODE_SERVER)
            {
                render_game_state(&game->state);
            }
        }

        // TODO:
        if (game->mode == GAME_MODE_EMPTY)
        {
            if (game->selected_index == 1)
            {
                if (is_button_pressed(KEY_D) || is_button_pressed(KEY_RIGHT) || is_button_pressed(KEY_TAB))
                {
                    game->selected_cell = MINIMUM(game->selected_cell + 1, 4);
                }
                if (is_button_pressed(KEY_A) || is_button_pressed(KEY_LEFT))
                {
                    game->selected_cell = MAXIMUM(game->selected_cell - 1, 0);
                }

                char8 digit = 0;
                if (is_button_pressed(KEY_0)) digit = '0';
                if (is_button_pressed(KEY_1)) digit = '1';
                if (is_button_pressed(KEY_2)) digit = '2';
                if (is_button_pressed(KEY_3)) digit = '3';
                if (is_button_pressed(KEY_4)) digit = '4';
                if (is_button_pressed(KEY_5)) digit = '5';
                if (is_button_pressed(KEY_6)) digit = '6';
                if (is_button_pressed(KEY_7)) digit = '7';
                if (is_button_pressed(KEY_8)) digit = '8';
                if (is_button_pressed(KEY_9)) digit = '9';

                if (game->selected_cell < 4)
                {
                    if (digit)
                    {
                        for (int32 i = 0; i < 3; i += 1)
                        {
                            if (!game->ip_digits[game->selected_cell][i])
                            {
                                game->ip_digits[game->selected_cell][i] = digit;

                                break;
                            }
                        }
                    }

                    if (is_button_pressed(KEY_BACKSPACE))
                    {
                        for (int32 i = 2; i >= 0; i -= 1)
                        {
                            if (game->ip_digits[game->selected_cell][i])
                            {
                                game->ip_digits[game->selected_cell][i] = 0;
                                break;
                            }
                        }
                    }
                }

                if (game->selected_cell == 4)
                {
                    if (digit)
                    {
                        for (int32 i = 0; i < 5; i += 1)
                        {
                            if (!game->port_digits[i])
                            {
                                game->port_digits[i] = digit;

                                break;
                            }
                        }
                    }
                    if (is_button_pressed(KEY_BACKSPACE))
                    {
                        for (int32 i = 4; i >= 0; i -= 1)
                        {
                            if (game->port_digits[i])
                            {
                                game->port_digits[i] = 0;
                                break;
                            }
                        }
                    }
                }
            }

            for (int32 i = 0; i < 4; i += 1)
            {
                uint16 value = 0;
                uint8 pow = 1;
                if (game->ip_digits[i][1]) pow = 10;
                if (game->ip_digits[i][2]) pow = 100;
                for (int32 j = 0; j < 3; j += 1)
                {
                    if (game->ip_digits[i][j])
                    {
                        value += (uint16)(game->ip_digits[i][j] - '0') * (pow);
                        pow /= 10;
                    }
                }
                if (value > 255)
                {
                    value = 255;
                }
                game->cells[i] = (uint8)value;
            }
            uint32 value = 0;
            uint16 pow = 1;
            if (game->port_digits[1]) pow = 10;
            if (game->port_digits[2]) pow = 100;
            if (game->port_digits[3]) pow = 1000;
            if (game->port_digits[4]) pow = 10000;
            for (int32 j = 0; j < 5; j += 1)
            {
                if (game->port_digits[j])
                {
                    value += (uint32)(game->port_digits[j] - '0') * (pow);
                    pow /= 10;
                }
            }
            if (value > 65535)
            {
                value = 65535;
            }
            game->port_cell = (uint16)value;

            if (is_button_pressed(KEY_W) || is_button_pressed(KEY_UP))
            {
                game->selected_index = MAXIMUM(game->selected_index - 1, 0);
            }

            if (is_button_pressed(KEY_S) || is_button_pressed(KEY_DOWN))
            {
                game->selected_index = MINIMUM(game->selected_index + 1, 3);
            }

            if (is_button_pressed(KEY_ENTER) || is_button_pressed(KEY_E) || is_button_pressed(KEY_SPACE))
            {
                if (game->selected_index != 1)
                {
                    game->screen = GAME_SCREEN_MAIN;
                    game->offscreen.width = GAME_DEFAULT_HORIZONTAL_RESOLUTION;
                    game->offscreen.height = GAME_DEFAULT_VERTICAL_RESOLUTION;
                }

                if (game->selected_index == 0)
                {
                    game->mode = GAME_MODE_OFFLINE;
                }
                if (game->selected_index == 2)
                {
                    game->mode = GAME_MODE_CLIENT;

                    Packet_connect packet_connet = { 0 };
                    packet_connet.header.type = PACKET_CONNECT;
                    game->ip = ((uint32)game->cells[0] << 24) | ((uint32)game->cells[1] << 16) | ((uint32)game->cells[2] << 8) | ((uint32)game->cells[3]);
                    game->port = game->port_cell;
                    net_send(&packet_connet, sizeof(Packet_connect), game->ip, game->port);
                }
                if (game->selected_index == 3)
                {
                    game->mode = GAME_MODE_SERVER;
                    game->port = game->port_cell;
                    uint16 binded_port = net_bind(game->port);
                    initialize_game_state(&game->state);
                }
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
        if (game->mode == GAME_MODE_OFFLINE || game->mode == GAME_MODE_CLIENT)
        {
            int64 frame_start_time = get_time_tick();
            update_game_state(&game->previous_state, game->previous_tick_input,
                              (game->current_time - game->previous_frame_time) / 1'000'000'000.0f, 0);
            render_game_state(&game->previous_state);

            game->frame_time = get_time_tick() - frame_start_time;
        }

        // TODO:
        if (game->mode == GAME_MODE_EMPTY)
        {
            uint8 red[4] = { 100,100,100, 100 };
            uint8 green[4] = { 100,100,100, 100 };
            uint8 blue[4] = { 100,100,100, 100 };
            red[game->selected_index] = 255;
            green[game->selected_index] = 255;
            blue[game->selected_index] = 255;

            clear(0, 0, 0);

            draw_text_string(STRING_LITERAL("Solo"), (float32)game->offscreen.width / 8, (float32)game->offscreen.height / 2 + 16, red[0], green[0], blue[0]);
            draw_text_string(STRING_LITERAL("Join"), (float32)game->offscreen.width / 8, (float32)game->offscreen.height / 2 - 16, red[2], green[2], blue[2]);
            draw_text_string(STRING_LITERAL("Host"), (float32)game->offscreen.width / 8, (float32)game->offscreen.height / 2 - 32, red[3], green[3], blue[3]);

            uint8 color[5] = { 100,100,100,100,100 };
            if (game->selected_index == 1)
            {
                color[game->selected_cell] = 255;
            }

            draw_text_string(uint8_to_string(game->cells[0]), (float32)game->offscreen.width / 8, (float32)game->offscreen.height / 2, color[0], color[0], color[0]);
            draw_text_string(STRING_LITERAL("."), (float32)game->offscreen.width / 8 + 32 - 8, (float32)game->offscreen.height / 2, 100, 100, 100);
            draw_text_string(uint8_to_string(game->cells[1]), (float32)game->offscreen.width / 8 + 32, (float32)game->offscreen.height / 2, color[1], color[1], color[1]);
            draw_text_string(STRING_LITERAL("."), (float32)game->offscreen.width / 8 + 64 - 8, (float32)game->offscreen.height / 2, 100, 100, 100);
            draw_text_string(uint8_to_string(game->cells[2]), (float32)game->offscreen.width / 8 + 64, (float32)game->offscreen.height / 2, color[2], color[2], color[2]);
            draw_text_string(STRING_LITERAL("."), (float32)game->offscreen.width / 8 + 96 - 8, (float32)game->offscreen.height / 2, 100, 100, 100);
            draw_text_string(uint8_to_string(game->cells[3]), (float32)game->offscreen.width / 8 + 96, (float32)game->offscreen.height / 2, color[3], color[3], color[3]);
            draw_text_string(STRING_LITERAL(":"), (float32)game->offscreen.width / 8 + 128 - 8, (float32)game->offscreen.height / 2, 100, 100, 100);
            draw_text_string(uint16_to_string(game->port_cell), (float32)game->offscreen.width / 8 + 128, (float32)game->offscreen.height / 2, color[4], color[4], color[4]);


            arena_clear(&game->string_arena);

            present_offscreen(game->offscreen);
        }

        game->previous_frame_time = game->current_time;
        game->frame_accumulator -= game->time_per_frame;
    }

    game->previous_time = game->current_time;

    int64 sleep_time = get_time_tick();
    // sleep(MINIMUM(MINIMUM(game->time_per_frame - game->frame_accumulator,
    //               game->time_per_update - game->update_accumulator),
    //               MINIMUM(game->time_per_frame, game->time_per_update) / 2));
    sleep(1'000'000);
    game->sleep_time = get_time_tick() - sleep_time;
}
