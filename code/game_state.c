// ── Константы в fixed32 (вычисляются компилятором из float-дефайнов) ──────────

#define FX_ZERO                     int32_to_fixed32(0)
#define FX_ONE                      int32_to_fixed32(1)
#define FX_TWO                      int32_to_fixed32(2)
#define FX_HALF                     float32_to_fixed32(0.5f)

#define FX_GRAVITATION              float32_to_fixed32(GRAVITATION)
#define FX_BLOBBY_JUMP_BUFFER       float32_to_fixed32(BLOBBY_JUMP_BUFFER)
#define FX_BLOBBY_JUMP_ACCELERATION float32_to_fixed32(BLOBBY_JUMP_ACCELERATION)
#define FX_BLOBBY_SPEED             float32_to_fixed32(BLOBBY_SPEED)
#define FX_BLOBBY_LOWER_SPHERE      float32_to_fixed32(BLOBBY_LOWER_SPHERE)
#define FX_BLOBBY_UPPER_SPHERE      float32_to_fixed32(BLOBBY_UPPER_SPHERE)
#define FX_BLOBBY_LOWER_RADIUS      float32_to_fixed32(BLOBBY_LOWER_RADIUS)
#define FX_BLOBBY_UPPER_RADIUS      float32_to_fixed32(BLOBBY_UPPER_RADIUS)

#define FX_BALL_GRAVITATION         float32_to_fixed32(BALL_GRAVITATION)
#define FX_BALL_RADIUS              float32_to_fixed32(BALL_RADIUS)
#define FX_BALL_COLLISION_VELOCITY  float32_to_fixed32(BALL_COLLISION_VELOCITY)

#define FX_NET_POSITION_X           float32_to_fixed32(NET_POSITION_X)
#define FX_NET_RADIUS               float32_to_fixed32(NET_RADIUS)
#define FX_NET_SPHERE_POSITION      float32_to_fixed32(NET_SPHERE_POSITION)

#define FX_GROUND_PLANE_HEIGHT      float32_to_fixed32(GROUND_PLANE_HEIGHT)
#define FX_GROUND_PLANE_HEIGHT_MAX  float32_to_fixed32(GROUND_PLANE_HEIGHT_MAX)
#define FX_LEFT_PLANE               float32_to_fixed32(LEFT_PLANE)
#define FX_RIGHT_PLANE              float32_to_fixed32(RIGHT_PLANE)

#define FX_STANDARD_BALL_HEIGHT     float32_to_fixed32(STANDARD_BALL_HEIGHT)
#define FX_BALL_RESET_LEFT          float32_to_fixed32(200.0f)
#define FX_BALL_RESET_RIGHT         float32_to_fixed32(600.0f)

// Суммы радиусов для проверки столкновений
#define FX_BALL_LOWER_RADIUS_SUM    float32_to_fixed32(BALL_RADIUS + BLOBBY_LOWER_RADIUS)
#define FX_BALL_UPPER_RADIUS_SUM    float32_to_fixed32(BALL_RADIUS + BLOBBY_UPPER_RADIUS)
#define FX_BALL_NET_RADIUS_SUM      float32_to_fixed32(BALL_RADIUS + NET_RADIUS)
#define FX_BLOB_NET_RADIUS_SUM      float32_to_fixed32(BLOBBY_LOWER_RADIUS + NET_RADIUS)

// Коэффициенты затухания
#define FX_DAMP_095                 float32_to_fixed32(0.95f)
#define FX_DAMP_057                 float32_to_fixed32(0.95f * 0.6f)
#define FX_DAMP_05                 float32_to_fixed32(0.5f)
#define FX_DAMP_033                 float32_to_fixed32(0.33f)
#define FX_DAMP_01                 float32_to_fixed32(0.1f)
#define FX_DAMP_07                  float32_to_fixed32(0.7f)
#define FX_DAMP_09                  float32_to_fixed32(0.9f)

// ── Вспомогательные функции (только fixed32) ──────────────────────────────────

// Проверка перекрытия двух окружностей без деления и sqrt
// Принимает уже вычисленные dx, dy и сумму радиусов — всё в fixed32
static bool32 fx_circle_overlap(fixed32 dx, fixed32 dy, fixed32 radius_sum)
{
    int64 dx64 = dx;
    int64 dy64 = dy;
    int64 r64  = radius_sum;
    return (dx64 * dx64 + dy64 * dy64) < (r64 * r64);
}

// Нормализация вектора в fixed32
// Безопасно для векторов длиной до ~255 единиц (все игровые расстояния)
static void fx_normalize(fixed32 x, fixed32 y, fixed32* out_nx, fixed32* out_ny)
{
    fixed32 len_sq = multiply_fixed32(x, x) + multiply_fixed32(y, y);
    fixed32 len    = sqrt_fixed32(len_sq);
    if (len == 0) { *out_nx = 0; *out_ny = 0; return; }
    *out_nx = divide_fixed32(x, len);
    *out_ny = divide_fixed32(y, len);
}

// Скалярное произведение в fixed32
static fixed32 fx_dot(fixed32 ax, fixed32 ay, fixed32 bx, fixed32 by)
{
    return multiply_fixed32(ax, bx) + multiply_fixed32(ay, by);
}

static fixed32 fx_abs(fixed32 v) { return v < 0 ? -v : v; }

// ── initialize_game_state ─────────────────────────────────────────────────────

static void initialize_game_state(Game_state* state)
{
    *state = (Game_state){ 0 };

    // Блобы стоят на земле
    state->player_position_x[0] = float32_to_fixed32(200.0f);
    state->player_position_y[0] = float32_to_fixed32(GROUND_PLANE_HEIGHT);
    state->player_position_x[1] = float32_to_fixed32(600.0f);
    state->player_position_y[1] = float32_to_fixed32(GROUND_PLANE_HEIGHT);

    // Мяч — над левым блобом
    state->ball_position_x = float32_to_fixed32(200.0f);
    state->ball_position_y = float32_to_fixed32(STANDARD_BALL_HEIGHT);

    play_sound(game->pfiff_sound);
}

static void do_menu()
{
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
}

// ── deinitialize_game_state ───────────────────────────────────────────────────

static void deinitialize_game_state(Game_state* state)
{

}

// ── update_game_state ─────────────────────────────────────────────────────────

static void update_game_state(Game_state* state, Tick_input input, float32 delta_time, bool32 is_fixed_update)
{
    if (game->screen == GAME_SCREEN_MENU)
    {
        do_menu();
    }
    else if (game->screen == GAME_SCREEN_MAIN)
    {
        fixed32 dt = multiply_fixed32(float32_to_fixed32(delta_time), float32_to_fixed32(75.0f));

        // ── 1. Блобы ──────────────────────────────────────────────────────────────
        for (int32 i = 0; i < MAX_PLAYERS; i += 1)
        {
            fixed32 pos_x = state->player_position_x[i];
            fixed32 pos_y = state->player_position_y[i];
            fixed32 vel_x = state->player_velocity_x[i];
            fixed32 vel_y = state->player_velocity_y[i];

            bool32 key_left = input.player_inputs[i].left;
            bool32 key_right = input.player_inputs[i].right;
            bool32 key_up = input.player_inputs[i].up;

            fixed32 cur_grav = FX_GRAVITATION;
            if (key_up)
            {
                if (pos_y <= FX_GROUND_PLANE_HEIGHT)
                    vel_y = FX_BLOBBY_JUMP_ACCELERATION;

                // FX_BLOBBY_JUMP_BUFFER = GRAVITATION/2 < 0
                // cur_grav -= отрицательное → гравитация слабее при прыжке
                cur_grav -= FX_BLOBBY_JUMP_BUFFER;
            }

            // Горизонтальная скорость
            vel_x = (int32_to_fixed32(key_right) - int32_to_fixed32(key_left));
            vel_x = multiply_fixed32(vel_x, FX_BLOBBY_SPEED);
            vel_x = multiply_fixed32(vel_x, dt);
            // Интеграция: pos += 0.5 * grav + vel;  vel += grav
            pos_x += vel_x;
            pos_y += multiply_fixed32(multiply_fixed32(FX_HALF, cur_grav) + vel_y, dt);
            vel_y += multiply_fixed32(cur_grav, dt);

            // Приземление
            if (pos_y < FX_GROUND_PLANE_HEIGHT)
            {
                pos_y = FX_GROUND_PLANE_HEIGHT;
                vel_y = FX_ZERO;
            }

            state->player_position_x[i] = pos_x;
            state->player_position_y[i] = pos_y;
            state->player_velocity_x[i] = vel_x;
            state->player_velocity_y[i] = vel_y;
        }

        // ── 2. Движение мяча ──────────────────────────────────────────────────────
        fixed32 ball_pos_x = state->ball_position_x;
        fixed32 ball_pos_y = state->ball_position_y;
        fixed32 ball_vel_x = state->ball_velocity_x;
        fixed32 ball_vel_y = state->ball_velocity_y;

        if (state->is_ball_active)
        {
            ball_pos_x += multiply_fixed32(ball_vel_x, dt);
            ball_pos_y += multiply_fixed32(
                multiply_fixed32(FX_HALF, FX_BALL_GRAVITATION) + ball_vel_y, dt);
        }
        ball_vel_y += multiply_fixed32(FX_BALL_GRAVITATION, dt);

        // ── 3. Столкновение мяча с блобами ────────────────────────────────────────
        for (int32 i = 0; i < MAX_PLAYERS; i += 1)
        {
            if (!state->is_ball_on_ground)
            {
                fixed32 blob_pos_x = state->player_position_x[i];
                fixed32 blob_pos_y = state->player_position_y[i];

                fixed32 col_cx = blob_pos_x;
                fixed32 col_cy = blob_pos_y;
                bool32  collide = 0;

                // Нижняя сфера блоба (в Y-up: ниже центра → меньший Y)
                {
                    fixed32 sx = blob_pos_x;
                    fixed32 sy = blob_pos_y - FX_BLOBBY_LOWER_SPHERE;
                    fixed32 dx = ball_pos_x - sx;
                    fixed32 dy = ball_pos_y - sy;
                    if (fx_circle_overlap(dx, dy, FX_BALL_LOWER_RADIUS_SUM))
                    {
                        col_cy = blob_pos_y - FX_BLOBBY_LOWER_SPHERE;
                        collide = 1;
                    }
                }
                // Верхняя сфера блоба (в Y-up: выше центра → больший Y)
                if (!collide)
                {
                    fixed32 sx = blob_pos_x;
                    fixed32 sy = blob_pos_y + FX_BLOBBY_UPPER_SPHERE;
                    fixed32 dx = ball_pos_x - sx;
                    fixed32 dy = ball_pos_y - sy;
                    if (fx_circle_overlap(dx, dy, FX_BALL_UPPER_RADIUS_SUM))
                    {
                        col_cy = blob_pos_y + FX_BLOBBY_UPPER_SPHERE;
                        collide = 1;
                    }
                }

                if (collide)
                {
                    fixed32 nx, ny;
                    fx_normalize(ball_pos_x - col_cx, ball_pos_y - col_cy, &nx, &ny);

                    ball_vel_x = multiply_fixed32(nx, FX_BALL_COLLISION_VELOCITY);
                    ball_vel_y = multiply_fixed32(ny, FX_BALL_COLLISION_VELOCITY);
                    ball_pos_x += ball_vel_x;
                    ball_pos_y += ball_vel_y;

                    state->is_ball_active = 1;
                    if (is_fixed_update)
                    {
                        play_sound(game->bums_sound);
                    }
                }
            }
        }

        // ── 4. Столкновение мяча с миром ──────────────────────────────────────────

        // Земля (низкий Y) — очко
        if (ball_pos_y - FX_BALL_RADIUS < FX_ZERO)
        {
            ball_vel_y = -ball_vel_y;
            ball_vel_x = multiply_fixed32(ball_vel_x, FX_DAMP_05);
            ball_vel_y = multiply_fixed32(ball_vel_y, FX_DAMP_05);
            ball_pos_y = FX_BALL_RADIUS;

            if (is_fixed_update && !state->is_ball_on_ground && state->is_ball_active)
            {
                state->is_ball_on_ground = 1;
                play_sound(game->chat_sound);
            }
        }

        // Ресет мяча
        if (state->is_ball_on_ground && ball_vel_x > -FX_HALF && ball_vel_x < FX_HALF
            && ball_vel_y > -FX_HALF && ball_vel_y < FX_HALF
            && ball_pos_y - FX_BALL_RADIUS < FX_HALF)
        {
            int32 scorer = (ball_pos_x < FX_NET_POSITION_X) ? 1 : 0;
            state->score[scorer] += 1;
            ball_pos_x = (scorer == 1) ? FX_BALL_RESET_RIGHT : FX_BALL_RESET_LEFT;
            ball_pos_y = FX_STANDARD_BALL_HEIGHT;
            ball_vel_x = FX_ZERO;
            ball_vel_y = FX_ZERO;

            state->is_ball_active = 0;
            state->is_ball_on_ground = 0;
        }

        // Левая стена
        if (ball_pos_x - FX_BALL_RADIUS <= FX_LEFT_PLANE && ball_vel_x < FX_ZERO)
        {
            ball_vel_x = -ball_vel_x;
            ball_pos_x = FX_LEFT_PLANE + FX_BALL_RADIUS;
        }
        // Правая стена
        else if (ball_pos_x + FX_BALL_RADIUS >= FX_RIGHT_PLANE && ball_vel_x > FX_ZERO)
        {
            ball_vel_x = -ball_vel_x;
            ball_pos_x = FX_RIGHT_PLANE - FX_BALL_RADIUS;
        }
        // Столб сетки (мяч ниже вершины: ball_pos_y < NET_SPHERE_POSITION)
        else if (ball_pos_y < FX_NET_SPHERE_POSITION &&
            fx_abs(ball_pos_x - FX_NET_POSITION_X) < FX_BALL_NET_RADIUS_SUM)
        {
            bool32 right = ball_pos_x > FX_NET_POSITION_X;
            ball_vel_x = -ball_vel_x;
            ball_pos_x = FX_NET_POSITION_X +
                (right ? FX_BALL_NET_RADIUS_SUM : -FX_BALL_NET_RADIUS_SUM);
        }
        else
        {
            // Сфера вершины сетки
            fixed32 dx = ball_pos_x - FX_NET_POSITION_X;
            fixed32 dy = ball_pos_y - FX_NET_SPHERE_POSITION;
            if (fx_circle_overlap(dx, dy, FX_BALL_NET_RADIUS_SUM))
            {
                fixed32 nx, ny;
                fx_normalize(dx, dy, &nx, &ny);

                // Кинетическая энергия: нормальная и параллельная компоненты
                fixed32 perp_dot = fx_dot(nx, ny, ball_vel_x, ball_vel_y);
                fixed32 perp_ekin = multiply_fixed32(perp_dot, perp_dot);
                fixed32 speed_sq = multiply_fixed32(ball_vel_x, ball_vel_x)
                    + multiply_fixed32(ball_vel_y, ball_vel_y);
                fixed32 para_ekin = speed_sq - perp_ekin;

                // Нормальная компонента гасится сильнее (0.7 vs 0.9 по энергии)
                perp_ekin = multiply_fixed32(perp_ekin, FX_DAMP_07);
                para_ekin = multiply_fixed32(para_ekin, FX_DAMP_09);

                fixed32 new_speed = sqrt_fixed32(perp_ekin + para_ekin);

                // Отражение: v' = v - 2*(v·n)*n
                fixed32 dot2 = multiply_fixed32(FX_TWO, perp_dot);
                fixed32 ref_x = ball_vel_x - multiply_fixed32(nx, dot2);
                fixed32 ref_y = ball_vel_y - multiply_fixed32(ny, dot2);

                fixed32 rnx, rny;
                fx_normalize(ref_x, ref_y, &rnx, &rny);
                ball_vel_x = multiply_fixed32(rnx, new_speed);
                ball_vel_y = multiply_fixed32(rny, new_speed);

                // Выталкиваем мяч из сферы
                ball_pos_x = FX_NET_POSITION_X + multiply_fixed32(nx, FX_BALL_NET_RADIUS_SUM);
                ball_pos_y = FX_NET_SPHERE_POSITION + multiply_fixed32(ny, FX_BALL_NET_RADIUS_SUM);
            }
        }

        // ── 5. Ограничения блобов (сетка и стены) ────────────────────────────────
        {
            fixed32 p0x = state->player_position_x[0];
            fixed32 p1x = state->player_position_x[1];

            // Блоб 0 не переходит за сетку вправо
            if (p0x + FX_BLOBBY_LOWER_RADIUS > FX_NET_POSITION_X - FX_NET_RADIUS)
                p0x = FX_NET_POSITION_X - FX_NET_RADIUS - FX_BLOBBY_LOWER_RADIUS;
            // Блоб 1 не переходит за сетку влево
            if (p1x - FX_BLOBBY_LOWER_RADIUS < FX_NET_POSITION_X + FX_NET_RADIUS)
                p1x = FX_NET_POSITION_X + FX_NET_RADIUS + FX_BLOBBY_LOWER_RADIUS;
            // Стены
            if (p0x < FX_LEFT_PLANE)   p0x = FX_LEFT_PLANE;
            if (p1x > FX_RIGHT_PLANE)  p1x = FX_RIGHT_PLANE;

            state->player_position_x[0] = p0x;
            state->player_position_x[1] = p1x;
        }

        // ── 6. Записываем мяч ────────────────────────────────────────────────────
        state->ball_position_x = ball_pos_x;
        state->ball_position_y = ball_pos_y;
        state->ball_velocity_x = ball_vel_x;
        state->ball_velocity_y = ball_vel_y;

        state->tick += 1;
    }
}
