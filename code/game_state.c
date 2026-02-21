static void initialize_game_state(Game_state* state)
{
    *state = (Game_state){ 0 };

    state->paddle_position_x[0] = int32_to_fixed32(TABLE_CX);
    state->paddle_position_y[0] = int32_to_fixed32(TABLE_TOP + TABLE_HEIGHT / 4);

    state->paddle_position_x[1] = int32_to_fixed32(TABLE_CX);
    state->paddle_position_y[1] = int32_to_fixed32(TABLE_TOP + TABLE_HEIGHT * 3 / 4);

    state->puck_position_x = int32_to_fixed32(TABLE_CX);
    state->puck_position_y = int32_to_fixed32(TABLE_CY);
    state->puck_velocity_x = float32_to_fixed32(40.0f);
    state->puck_velocity_y = float32_to_fixed32(60.0f);

    state->tick = 0;
}

static void reset_puck(Game_state* state, int32 scored_player)
{
    state->puck_position_x = int32_to_fixed32(TABLE_CX);
    state->puck_position_y = int32_to_fixed32(TABLE_CY);

    float32 dir = (scored_player == 0) ? -1.0f : 1.0f;
    state->puck_velocity_x = float32_to_fixed32(30.0f);
    state->puck_velocity_y = float32_to_fixed32(dir * 60.0f);
}

static void update_game_state(Game_state* state, Tick_input input, float32 delta_time)
{
    fixed32 dt_fixed = float32_to_fixed32(delta_time);

    // ── 1. Paddle spring ──────────────────────────────────────────────────
    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        fixed32 target_x = input.player_inputs[i].cursor_x;
        fixed32 target_y = input.player_inputs[i].cursor_y;

        // Clamp X
        if (target_x < int32_to_fixed32(PADDLE_MIN_X(PADDLE_RADIUS)))
            target_x = int32_to_fixed32(PADDLE_MIN_X(PADDLE_RADIUS));
        if (target_x > int32_to_fixed32(PADDLE_MAX_X(PADDLE_RADIUS)))
            target_x = int32_to_fixed32(PADDLE_MAX_X(PADDLE_RADIUS));

        // Clamp Y по половинам
        if (i == 0)
        {
            if (target_y < int32_to_fixed32(PADDLE_0_MIN_Y(PADDLE_RADIUS)))
                target_y = int32_to_fixed32(PADDLE_0_MIN_Y(PADDLE_RADIUS));
            if (target_y > int32_to_fixed32(PADDLE_0_MAX_Y(PADDLE_RADIUS)))
                target_y = int32_to_fixed32(PADDLE_0_MAX_Y(PADDLE_RADIUS));
        }
        else
        {
            if (target_y < int32_to_fixed32(PADDLE_1_MIN_Y(PADDLE_RADIUS)))
                target_y = int32_to_fixed32(PADDLE_1_MIN_Y(PADDLE_RADIUS));
            if (target_y > int32_to_fixed32(PADDLE_1_MAX_Y(PADDLE_RADIUS)))
                target_y = int32_to_fixed32(PADDLE_1_MAX_Y(PADDLE_RADIUS));
        }

        fixed32 dx     = target_x - state->paddle_position_x[i];
        fixed32 dy     = target_y - state->paddle_position_y[i];
        fixed32 spring = float32_to_fixed32(PADDLE_SPRING);

        state->paddle_velocity_x[i] += multiply_fixed32(multiply_fixed32(spring, dx), dt_fixed);
        state->paddle_velocity_y[i] += multiply_fixed32(multiply_fixed32(spring, dy), dt_fixed);

        fixed32 damping = float32_to_fixed32(PADDLE_DAMPING);
        state->paddle_velocity_x[i] = multiply_fixed32(state->paddle_velocity_x[i], damping);
        state->paddle_velocity_y[i] = multiply_fixed32(state->paddle_velocity_y[i], damping);

        state->paddle_position_x[i] += multiply_fixed32(state->paddle_velocity_x[i], dt_fixed);
        state->paddle_position_y[i] += multiply_fixed32(state->paddle_velocity_y[i], dt_fixed);
    }

    // ── 2. Puck movement ──────────────────────────────────────────────────
    state->puck_position_x += multiply_fixed32(state->puck_velocity_x, dt_fixed);
    state->puck_position_y += multiply_fixed32(state->puck_velocity_y, dt_fixed);

    state->puck_velocity_x = multiply_fixed32(state->puck_velocity_x, float32_to_fixed32(PUCK_FRICTION));
    state->puck_velocity_y = multiply_fixed32(state->puck_velocity_y, float32_to_fixed32(PUCK_FRICTION));

    // ── 3. Wall & goal collisions ─────────────────────────────────────────

    // Левая / правая
    if (state->puck_position_x < int32_to_fixed32(TABLE_LEFT + PUCK_RADIUS))
    {
        state->puck_position_x = int32_to_fixed32(TABLE_LEFT + PUCK_RADIUS);
        state->puck_velocity_x = -state->puck_velocity_x;
    }
    if (state->puck_position_x > int32_to_fixed32(TABLE_RIGHT - PUCK_RADIUS))
    {
        state->puck_position_x = int32_to_fixed32(TABLE_RIGHT - PUCK_RADIUS);
        state->puck_velocity_x = -state->puck_velocity_x;
    }

    // Верхняя стена / ворота p0
    if (state->puck_position_y < int32_to_fixed32(TABLE_TOP + PUCK_RADIUS))
    {
        int32 px = fixed32_to_int32(state->puck_position_x);
        if (px > GOAL_LEFT && px < GOAL_RIGHT)
        {
            state->score[1] += 1;
            reset_puck(state, 0);
        }
        else
        {
            state->puck_position_y = int32_to_fixed32(TABLE_TOP + PUCK_RADIUS);
            state->puck_velocity_y = -state->puck_velocity_y;
        }
    }

    // Нижняя стена / ворота p1
    if (state->puck_position_y > int32_to_fixed32(TABLE_BOTTOM - PUCK_RADIUS))
    {
        int32 px = fixed32_to_int32(state->puck_position_x);
        if (px > GOAL_LEFT && px < GOAL_RIGHT)
        {
            state->score[0] += 1;
            reset_puck(state, 1);
        }
        else
        {
            state->puck_position_y = int32_to_fixed32(TABLE_BOTTOM - PUCK_RADIUS);
            state->puck_velocity_y = -state->puck_velocity_y;
        }
    }

    // ── 4. Paddle–puck collision ──────────────────────────────────────────
    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        float32 fdx      = fixed32_to_float32(state->puck_position_x - state->paddle_position_x[i]);
        float32 fdy      = fixed32_to_float32(state->puck_position_y - state->paddle_position_y[i]);
        float32 dist2    = fdx * fdx + fdy * fdy;
        float32 min_dist = (float32)(PADDLE_RADIUS + PUCK_RADIUS);

        if (dist2 < min_dist * min_dist && dist2 > 0.0001f)
        {
            float32 dist = square_root(dist2);
            float32 nx   = fdx / dist;
            float32 ny   = fdy / dist;

            float32 overlap = min_dist - dist;
            state->puck_position_x += float32_to_fixed32(nx * overlap);
            state->puck_position_y += float32_to_fixed32(ny * overlap);

            float32 rel_vx = fixed32_to_float32(state->puck_velocity_x - state->paddle_velocity_x[i]);
            float32 rel_vy = fixed32_to_float32(state->puck_velocity_y - state->paddle_velocity_y[i]);
            float32 dot    = rel_vx * nx + rel_vy * ny;

            if (dot < 0.0f)
            {
                float32 j = (1.0f + PADDLE_RESTITUTION) * dot;
                state->puck_velocity_x -= float32_to_fixed32(j * nx);
                state->puck_velocity_y -= float32_to_fixed32(j * ny);

                state->puck_velocity_x += multiply_fixed32(state->paddle_velocity_x[i],
                                              float32_to_fixed32(PADDLE_IMPULSE_SCALE));
                state->puck_velocity_y += multiply_fixed32(state->paddle_velocity_y[i],
                                              float32_to_fixed32(PADDLE_IMPULSE_SCALE));

                float32 pvx    = fixed32_to_float32(state->puck_velocity_x);
                float32 pvy    = fixed32_to_float32(state->puck_velocity_y);
                float32 speed2 = pvx * pvx + pvy * pvy;
                if (speed2 > PUCK_MAX_SPEED * PUCK_MAX_SPEED)
                {
                    float32 inv = PUCK_MAX_SPEED / square_root(speed2);
                    state->puck_velocity_x = float32_to_fixed32(pvx * inv);
                    state->puck_velocity_y = float32_to_fixed32(pvy * inv);
                }
            }
        }
    }

    state->tick += 1;
}

static void deinitialize_game_state(Game_state* state)
{

}
