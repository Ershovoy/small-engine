#include "game.h"
#include "math.c"
#include "string.c"
#include "input.c"
#include "renderer.c"
#include "platform_api.c"
#include "arena.c"
#include "state.c"
#include "network.c"
//#include "sound.c"

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

    game->is_offline = 1;

    return 1;
}

static void render_game(State* state)
{
    Color clear_color = { 0 };
    clear_color.red = (uint8)(41);
    clear_color.green = (uint8)(173);
    clear_color.blue = (uint8)(255);
    clear(game->offscreen, clear_color);

    draw_rectangle(game->offscreen, (Color){255, 163, 0}, 0, 0, GAME_HORIZONTAL_RESOLUTION, GROUND_HEIGHT);

    draw_circle(255, 0, 77, 50, 50, 10);
    draw_circle(255, 0, 77, 50, 60, 7);

    draw_circle(0, 228, 54, 100, 70, 14);
    draw_circle(0, 228, 54, 100, 80, 10);

    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        if (game->is_connected)
        {
            Vec2i position = state->player_positions[i];
            draw_circle(255, 255, 255, (float32)position.e1, (float32)position.e2, 16.0f);
        }
    }

    present_offscreen(game->offscreen);
}

static void update_game(State* state, Tick_input input)
{
    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        Player_input player_input = input.player_inputs[i];

        if (player_input.is_up)
        {
            state->player_positions[i].e2 += 1;
        }
        if (player_input.is_left)
        {
            state->player_positions[i].e1 -= 1;
        }
        if (player_input.is_down)
        {
            state->player_positions[i].e2 -= 1;
        }
        if (player_input.is_right)
        {
            state->player_positions[i].e1 += 1;
        }
    }

    state->tick += 1;
}

static void server_handle_packet(void* packet)
{
    Packet_header* packet_header = (Packet_header*)packet;
    switch (packet_header->type)
    {
        case PACKET_CONNECT:
        {
            break;
        }
        case PACKET_DISCONNECT:
        {
            break;
        }
        case PACKET_PING:
        {
            break;
        }
        case PACKET_PLAYER_INPUT:
        {
            break;
        }
        case PACKET_TICK_INPUT:
        {
            break;
        }
        default:
        {
            break;
        }
    }
}

static void client_handle_packet(void* packet)
{
    Packet_header* packet_header = (Packet_header*)packet;
    switch (packet_header->type)
    {
        int64 size = sizeof(packet_header);
        packet = (uint8*)packet + sizeof(packet_header);
        case PACKET_TICK_INPUT:
        {
            Packet_input* packet_input = (Packet_input*)packet;



            break;
        }
        case PACKET_GAME_STATE:
        {
            Packet_game_state* packet_game_state = (Packet_game_state*)packet;

            break;
        }
        default:
        {
            break;
        }
    }
}

static void client()
{
    console_write(STRING_LITERAL("\nClinet\n"));

    char8 buffer[256] = { 0 };
    String string = { 0 };
    string.data = buffer;

    uint32 out_ip = { 0 };
    uint16 out_port = { 0 };
    Tick_input tick_input = { 0 };
    console_write(STRING_LITERAL("\nGetting Tick_input from the server\n"));
    while (net_receive(&tick_input, sizeof(Tick_input), &out_ip, &out_port) > 0)
    {
        console_write(STRING_LITERAL("Got new Tick_input for "));
        string.length = uint32_to_string(buffer, 256, (uint32)tick_input.tick);
        console_write(string);
        console_write(STRING_LITERAL(" tick\n"));
        for (int32 i = 0; i < MAX_TICK_INPUT_BUFFER; i += 1)
        {
            if (!game->has_tick_input[i])
            {
                game->tick_inputs[i] = tick_input;
                game->has_tick_input[i] = 1;
                break;
            }
        }
    }

    console_write(STRING_LITERAL("Starting processing tick inputs\n"));
    bool32 has_tick_input = 1;
    while (has_tick_input)
    {
        has_tick_input = 0;
        for (int32 i = 0; i < MAX_TICK_INPUT_BUFFER; i += 1)
        {
            if (game->has_tick_input[i])
            {
                if (game->tick_inputs[i].tick == game->client_state.tick)
                {
                    Tick_input tick_input = game->tick_inputs[i];
                    console_write(STRING_LITERAL("Process tick input for "));
                    string.length = uint32_to_string(buffer, 256, (uint32)tick_input.tick);
                    console_write(STRING_LITERAL(" tick\n"));

                    update_game(&game->client_state, tick_input);
                    render_game(&game->client_state);

                    game->has_tick_input[i] = 0;
                }
                has_tick_input = 1;
            }
        }
    }


    Player_input player_input = { 0 };
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

    net_send(&player_input, sizeof(Player_input), 0x7f000001, 0xFFFF);

    adjust_input();
}

static void server()
{
    console_write(STRING_LITERAL("\nServer\n"));
    char8 buffer[256] = { 0 };
    String string = { 0 };
    string.data = buffer;

    console_write(STRING_LITERAL("Getting input from players\n"));
    int32 is_receive = 0;
    do
    {
        uint32 out_ip = { 0 };
        uint16 out_port = { 0 };
        Player_input player_input = { 0 };
        is_receive = net_receive(&player_input, sizeof(Player_input), &out_ip, &out_port);
        if (is_receive > 0)
        {
            for (int32 i = 0; i < MAX_PLAYERS; i += 1)
            {
                if (game->ips[i] == out_ip && game->ports[i] == out_port)
                {
                    console_write(STRING_LITERAL("Got player input from "));
                    string.length = uint32_to_string(buffer, 256, out_ip);
                    console_write(string);
                    console_write(STRING_LITERAL(" ip and "));
                    string.length = uint32_to_string(buffer, 256, out_port);
                    console_write(string);
                    console_write(STRING_LITERAL(" port\n"));

                    game->tick_input.player_inputs[i] = player_input;

                    break;
                }

                if (!game->ips[i] && !game->ports[i])
                {
                    console_write(STRING_LITERAL("New player with "));
                    string.length = uint32_to_string(buffer, 256, out_ip);
                    console_write(string);
                    console_write(STRING_LITERAL(" ip and "));
                    string.length = uint32_to_string(buffer, 256, out_port);
                    console_write(string);
                    console_write(STRING_LITERAL(" port connected\n"));
                    game->ips[i] = out_ip;
                    game->ports[i] = out_port;
                    game->is_connected[i] = 1;

                    game->tick_input.player_inputs[i] = player_input;

                    break;
                }
            }
        }
    } while (is_receive > 0);

    game->current_time = get_time_tick() - game->start_time;
    uint64 delta_time = game->current_time - game->previous_time;
    if (delta_time > game->time_per_update)
    {
        delta_time = game->time_per_update;
    }

    game->previous_time = game->current_time;
    game->accumulator += delta_time;
    if (game->accumulator >= game->time_per_update)
    {
        console_write(STRING_LITERAL("\nTick: "));
        string.length = uint32_to_string(buffer, 256, (uint32)game->server_state.tick);
        console_write(string);
        console_write(STRING_LITERAL("\n"));
        console_write(STRING_LITERAL("Sending Tick_input to all connected players:\n"));
        game->tick_input.tick = game->server_state.tick;
        for (int32 i = 0; i < MAX_PLAYERS; i += 1)
        {
            if (game->is_connected[i])
            {
                net_send(&game->tick_input, sizeof(Tick_input), game->ips[i], game->ports[i]);

                console_write(STRING_LITERAL("Send Tick_input to "));
                string.length = uint32_to_string(buffer, 256, game->ips[i]);
                console_write(string);
                console_write(STRING_LITERAL(" ip and "));
                string.length = uint32_to_string(buffer, 256, game->ports[i]);
                console_write(string);
                console_write(STRING_LITERAL("port\n"));
            }
        }

        console_write(STRING_LITERAL("Server game state update started\n"));
        update_game(&game->server_state, game->tick_input);

        game->accumulator -= game->time_per_update;
    }


}

static void game_loop()
{
    if (is_button_pressed(KEY_F1))
    {
        if (game->is_offline)
        {
            game->is_client = 1;
            game->is_offline = 0;
        }
    }
    if (is_button_pressed(KEY_F2))
    {
        if (game->is_offline)
        {
            game->is_server = 1;
            game->is_offline = 0;
            uint16 binded_port = net_bind(0xFFFF);
        }
    }
    if (is_button_pressed(KEY_F3))
    {
        if (game->is_offline)
        {
            game->is_server = 1;
            game->is_client = 1;
            game->is_offline = 0;
            uint16 binded_port = net_bind(0xFFFF);
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

    sleep(1'000);
}
