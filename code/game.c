#include "game.h"
#include "math.c"
#include "string.c"
#include "input.c"
#include "renderer.c"
#include "platform_api.c"
#include "arena.c"
// #include "sound.c"

#define IS_SERVER 0

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

    game->time_per_update = (uint64)1'000'000'000 / 75;
    game->start_time = get_time_tick();

#if IS_SERVER
    uint16 binded_port = net_bind(0xFFFF);
#endif

    // Blobby Volley 3
    game->player_positions[0] = (Vec2){ 200.0f, GROUND_PLANE_HEIGHT };
    game->player_positions[1] = (Vec2){ 600.0f, GROUND_PLANE_HEIGHT };
    game->ball_position = (Vec2){ 200.0f, STANDARD_BALL_HEIGHT };

    return 1;
}

static void handle_blob(Vec2* blob_position, Vec2* blob_velocity,
                        bool32 key_left, bool32 key_right, bool32 key_up)
{
    float32 current_gravitation = GRAVITATION;
    if (key_up)
    {
        if (blob_position->e2 >= GROUND_PLANE_HEIGHT)
        {
            blob_velocity->e2 = BLOBBY_JUMP_ACCELERATION;
        }
        current_gravitation -= BLOBBY_JUMP_BUFFER;
    }

    blob_velocity->e1 = (1.0f * key_right - 1.0f * key_left) * BLOBBY_SPEED;

    blob_position->e1 += blob_velocity->e1;
    blob_position->e2 += 0.5f * current_gravitation + blob_velocity->e2;

    blob_velocity->e2 += current_gravitation;

    if (blob_position->e2 > GROUND_PLANE_HEIGHT)
    {
        blob_position->e2 = GROUND_PLANE_HEIGHT;
        blob_velocity->e2 = 0.0f;
    }
}

static bool32 circle_to_circle(Vec2 first_position,  float32 first_radius,
                               Vec2 second_position, float32 second_radius)
{
    Vec2 distance = { 0 };
    distance.e1 = first_position.e1 - second_position.e1;
    distance.e2 = first_position.e2 - second_position.e2;
    float32 radius_sum = first_radius + second_radius;
    return (distance.e1 * distance.e1 + distance.e2 * distance.e2) < (radius_sum * radius_sum);
}

static Vec2 normalize(Vec2 vector)
{
    float32 length = square_root(vector.e1 * vector.e1 + vector.e2 * vector.e2);
    return (Vec2){vector.e1 / length, vector.e2 / length};
}

static float32 length(Vec2 vector)
{
    return square_root(vector.e1 * vector.e1 + vector.e2 * vector.e2);
}

static float32 dot_product(Vec2 first_vector, Vec2 second_vector)
{
    return first_vector.e1 * second_vector.e1 + first_vector.e2 * second_vector.e2;
}

static void handle_blob_ball_collision(Vec2* ball_position, Vec2* ball_velocity,
                                       Vec2 blob_position, Vec2 blob_velocity)
{
    Vec2 collision_center = blob_position;

    // check for impact
    bool32 isCollide = 0;
    Vec2 lower_sphere_position = (Vec2){ blob_position.e1, blob_position.e2 + BLOBBY_LOWER_SPHERE };
    Vec2 upper_sphere_position = (Vec2){ blob_position.e1, blob_position.e2 - BLOBBY_UPPER_SPHERE };
    if (circle_to_circle(*ball_position, BALL_RADIUS, lower_sphere_position, BLOBBY_LOWER_RADIUS))
    {
        collision_center.e2 += BLOBBY_LOWER_SPHERE;
        isCollide = 1;
    }
    else if (circle_to_circle(*ball_position, BALL_RADIUS, upper_sphere_position, BLOBBY_UPPER_RADIUS))
    {
        collision_center.e2 -= BLOBBY_UPPER_SPHERE;
        isCollide = 1;
    }

    if (isCollide)
    {
        // ok, if we get here, there actually was a collision

        // calculate hit intensity
        Vec2 temp = { 0 };
        temp.e1 = blob_velocity.e1 - ball_velocity->e1;
        temp.e2 = blob_velocity.e2 - ball_velocity->e2;
        float32 intensity = min(1.0f, square_root(temp.e1 * temp.e1 + temp.e2 * temp.e2) / 25.0f);

        // set ball velocity
        ball_velocity->e1 = -(collision_center.e1 - ball_position->e1);
        ball_velocity->e2 = -(collision_center.e2 - ball_position->e2);
        *ball_velocity = normalize(*ball_velocity);
        ball_velocity->e1 = ball_velocity->e1 * BALL_COLLISION_VELOCITY;
        ball_velocity->e2 = ball_velocity->e2 * BALL_COLLISION_VELOCITY;
        ball_position->e1 += ball_velocity->e1;
        ball_position->e2 += ball_velocity->e2;
    }
}

static void render_game()
{
    Color clear_color = { 0 };
    clear_color.blue = (uint8)(game->tick / 2 % (256 - 96));
    clear_color.red = (uint8)(game->tick / 3 % (256 - 64));
    clear_color.green = (uint8)(game->tick / 4 % (256 - 128));
    clear(game->offscreen, clear_color);

    Vec2 position;
    position.e1 = (float32)game->x[0];
    position.e2 = (float32)game->y[0];
    draw_circle(255, 0, 255,position.e1, position.e2, 16);

    position.e1 = (float32)game->x[1];
    position.e2 = (float32)game->y[1];
    draw_circle(0, 255, 255, position.e1, position.e2, 8);

    draw_horizontal_line(255, 255, 255, 0);
    draw_horizontal_line(255, 255, 255, (float32)game->offscreen.height);
    draw_vertical_line(255, 255, 255, 0);
    draw_vertical_line(255, 255, 255, (float32)game->offscreen.width);

    // Blobby Volley 3
    draw_circle(0, 255, 0, game->ball_position.e1, game->ball_position.e2, BALL_RADIUS);
    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        uint8 red = (uint8)(255 * (1 - i));
        uint8 blue = (uint8)(255 * i);
        draw_circle(red, 0, blue, game->player_positions[i].e1, game->player_positions[i].e2 + BLOBBY_LOWER_SPHERE, BLOBBY_LOWER_RADIUS);
        draw_circle(red, 0, blue, game->player_positions[i].e1, game->player_positions[i].e2 - BLOBBY_UPPER_SPHERE, BLOBBY_UPPER_RADIUS);
    }
    draw_horizontal_line(255, 255, 255, GROUND_PLANE_HEIGHT + BLOBBY_HEIGHT / 2);
    draw_horizontal_line(255, 255, 255, GROUND_PLANE_HEIGHT + BLOBBY_HEIGHT / 1.75f);
    draw_horizontal_line(255, 255, 255, GROUND_PLANE_HEIGHT + BLOBBY_HEIGHT / 1.5f);
    draw_horizontal_line(255, 255, 255, GROUND_PLANE_HEIGHT + BLOBBY_HEIGHT / 1.25f);
    draw_horizontal_line(255, 255, 255, GROUND_PLANE_HEIGHT + BLOBBY_HEIGHT / 1.0f);
    draw_horizontal_line(255, 255, 255, GROUND_PLANE_HEIGHT + BLOBBY_HEIGHT / 0.75f);
    draw_horizontal_line(255, 255, 255, GROUND_PLANE_HEIGHT + BLOBBY_HEIGHT / 0.5f);
    for (float32 y = NET_SPHERE_POSITION; y < 800.0f; y += 10.0f)
    {
        draw_circle(255, 255, 255, NET_POSITION_X, y, NET_RADIUS);
    }

    present_offscreen(game->offscreen);
}

static float32 fabs(float32 value)
{
    if (value < 0.0f)
    {
        return -value;
    }
    return value;
}

static void update_game()
{
    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        Player_input player_input = game->tick_input.player_inputs[i];
        handle_blob(&game->player_positions[i], &game->player_velocities[i], player_input.is_left, player_input.is_right, player_input.is_up);
    }

    // Move ball
    game->ball_position.e1 += game->ball_velocity.e1,
    game->ball_position.e2 += 0.5f * BALL_GRAVITATION + game->ball_velocity.e2;
    game->ball_velocity.e2 += BALL_GRAVITATION;

    // Handle ball and blobby collision
    handle_blob_ball_collision(&game->ball_position, &game->ball_velocity, game->player_positions[0], game->player_velocities[0]);
    handle_blob_ball_collision(&game->ball_position, &game->ball_velocity, game->player_positions[1], game->player_velocities[1]);

    // Handle ball and world collision
    {
        // Ball to roof collision
        if(game->ball_position.e2 - BALL_RADIUS < 0)
        {
            game->ball_velocity = (Vec2){ game->ball_velocity.e1, -game->ball_velocity.e2 };
            game->ball_velocity.e1 *= 0.95f;
            game->ball_velocity.e2 *= 0.95f;
            game->ball_position.e2 = BALL_RADIUS;
        }

        // Ball to ground Collision
        if (game->ball_position.e2 + BALL_RADIUS > GROUND_PLANE_HEIGHT_MAX)
        {
            game->ball_velocity = (Vec2){ game->ball_velocity.e1, -game->ball_velocity.e2 };
            game->ball_velocity.e1 *= 0.95f * 0.6f;
            game->ball_velocity.e2 *= 0.95f * 0.6f;
            game->ball_position.e2 = GROUND_PLANE_HEIGHT_MAX - BALL_RADIUS;
        }

        // Border Collision
        if ((game->ball_position.e1 - BALL_RADIUS <= LEFT_PLANE) && (game->ball_velocity.e1 < 0.0))
        {
            game->ball_velocity = (Vec2){-game->ball_velocity.e1, game->ball_velocity.e2};
            // set the ball's position
            game->ball_position.e1 = LEFT_PLANE + BALL_RADIUS;
        }
        else if (game->ball_position.e1 + BALL_RADIUS >= RIGHT_PLANE && game->ball_velocity.e1 > 0.0)
        {
            game->ball_velocity = (Vec2){-game->ball_velocity.e1, game->ball_velocity.e2};
            // set the ball's position
            game->ball_position.e1 = RIGHT_PLANE - BALL_RADIUS;
        }
        else if (game->ball_position.e2 > NET_SPHERE_POSITION && fabs(game->ball_position.e1 - NET_POSITION_X) < BALL_RADIUS + NET_RADIUS)
        {
            bool32 right = game->ball_position.e1 - NET_POSITION_X > 0;
            game->ball_velocity = (Vec2){-game->ball_velocity.e1, game->ball_velocity.e2};
            // set the ball's position so that it touches the net
            game->ball_position.e1 = NET_POSITION_X + (right ? (BALL_RADIUS + NET_RADIUS) : (-BALL_RADIUS - NET_RADIUS));
        }
        else
        {
            // Net Collisions
            Vec2 temp2 = { 0 };
            temp2.e1 = NET_POSITION_X - game->ball_position.e1;
            temp2.e2 = NET_SPHERE_POSITION - game->ball_position.e2;
            float32 ball_net_distance = length(temp2);
            if (ball_net_distance < NET_RADIUS + BALL_RADIUS)
            {
                // calculate
                Vec2 temp4 = { 0 };
                temp4.e1 = NET_POSITION_X - game->ball_position.e1;
                temp4.e2 = NET_SPHERE_POSITION - game->ball_position.e2;
                Vec2 normal = normalize(temp4);

                // normal component of kinetic energy
                float32 perp_ekin = dot_product(normal, game->ball_velocity);
                perp_ekin *= perp_ekin;
                // parallel component of kinetic energy
                float32 para_ekin = 0.0f;
                para_ekin = (game->ball_velocity.e1 * game->ball_velocity.e1 + game->ball_velocity.e2 * game->ball_velocity.e2) - perp_ekin;
                // the normal component is damped stronger than the parallel component
                // the values are ~ 0.85 and ca. 0.95, because speed is sqrt(ekin)
                perp_ekin *= 0.7f;
                para_ekin *= 0.9f;

                float32 new_speed = square_root(perp_ekin + para_ekin);


                Vec2 reflect = { 0 };
                reflect.e1 = game->ball_velocity.e1 - (normal.e1 * 2 * dot_product(game->ball_velocity, normal));
                reflect.e2 = game->ball_velocity.e2 - (normal.e2 * 2 * dot_product(game->ball_velocity, normal));
                game->ball_velocity.e1 = normalize(reflect).e1 * new_speed;
                game->ball_velocity.e2 = normalize(reflect).e2 * new_speed;

                // pushes the ball out of the net
                Vec2 temp3 = { 0 };
                temp3.e1 = NET_POSITION_X - normal.e1 * (NET_RADIUS + BALL_RADIUS);
                temp3.e2 = NET_SPHERE_POSITION - normal.e2 * (NET_RADIUS + BALL_RADIUS);
                game->ball_position = temp3;
            }
        }
    }

    // Collision between blobby and the net
    if (game->player_positions[0].e1 + BLOBBY_LOWER_RADIUS > NET_POSITION_X - NET_RADIUS)
        game->player_positions[0].e1 = NET_POSITION_X - NET_RADIUS - BLOBBY_LOWER_RADIUS;

    if (game->player_positions[1].e1 - BLOBBY_LOWER_RADIUS < NET_POSITION_X + NET_RADIUS)
        game->player_positions[1].e1 = NET_POSITION_X + NET_RADIUS + BLOBBY_LOWER_RADIUS;

    // Collision between blobby and the border
    if (game->player_positions[0].e1 < LEFT_PLANE)
        game->player_positions[0].e1 = LEFT_PLANE;

    if (game->player_positions[1].e1 > RIGHT_PLANE)
        game->player_positions[1].e1 = RIGHT_PLANE;

    game->tick += 1;
    adjust_input();
}

static void server()
{
    bool32 is_receive = 0;
    do
    {
        uint32 out_ip = { 0 };
        uint16 out_port = { 0 };
        Player_input player_input = { 0 };
        is_receive = net_receive(&player_input, sizeof(Player_input), &out_ip, &out_port);
        if (is_receive && out_port != 0xFFFF)
        {
            for (int32 i = 0; i < MAX_PLAYERS; i += 1)
            {
                if (game->ips[i] == out_ip && game->ports[i] == out_port)
                {
                    game->tick_input.player_inputs[i] = player_input;
                    break;
                }

                if (!game->ips[i] && !game->ports[i])
                {
                    console_write(STRING_LITERAL("New player connected!\n"));
                    game->ips[i] = out_ip;
                    game->ports[i] = out_port;
                    game->tick_input.player_inputs[i] = player_input;
                    break;
                }
            }
        }
    } while (is_receive);

    bool32 is_all_players_connected = 1;
    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        if (!game->ips[i])
        {
            is_all_players_connected = 0;
        }
    }

    if (is_all_players_connected)
    {
        game->tick_input.tick = game->tick;
        for (int32 i = 0; i < MAX_PLAYERS; i += 1)
        {
            if (game->ips[i])
            {
                net_send((void*)&game->tick_input, sizeof(Tick_input), game->ips[i], game->ports[i]);
            }
        }

        update_game();
    }
}

static void client()
{
    uint32 out_ip = { 0 };
    uint16 out_port = { 0 };
    Tick_input tick_input = { 0 };
    bool32 is_receive = net_receive(&tick_input, sizeof(Tick_input), &out_ip, &out_port);

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

    net_send((void*)&player_input, sizeof(Player_input), 0x7f000001, 0xFFFF);

    if (is_receive)
    {
        game->tick_input = tick_input;
        update_game();
    }
}

static void game_loop()
{
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

#if IS_SERVER
        server();
#else
        client();
#endif

        game->accumulator -= game->time_per_update;
        if (game->accumulator < game->time_per_update)
        {
            render_game();
        }
    }

    sleep(1000);
}
