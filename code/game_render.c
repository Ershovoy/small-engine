static void render_game_state(Game_state* state)
{
    // ── Background ────────────────────────────────────────────────────────────
    Color sky = { 0 };
    sky.red   = 80;
    sky.green = 140;
    sky.blue  = 200;
    clear(sky.red, sky.green, sky.blue);

    // ── Земля (горизонтальные полосы у основания) ─────────────────────────────
    float32 ground_top = GROUND_PLANE_HEIGHT + BLOBBY_HEIGHT / 2.0f;
    draw_rectangle(34, 139, 34, 0, 90, 800, (int32)100);

    // ── Сетка: столб снизу вверх до NET_SPHERE_POSITION ──────────────────────
    for (float32 y = 0.0f; y <= NET_SPHERE_POSITION; y += 25.0f)
    {
        draw_circle(200, 200, 200, NET_POSITION_X, y, NET_RADIUS);
    }
    // Сфера вершины сетки
    draw_circle(220, 220, 220, NET_POSITION_X, NET_SPHERE_POSITION, NET_RADIUS * 1.5f);

    // ── Блобы ─────────────────────────────────────────────────────────────────
    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        float32 px = fixed32_to_float32(state->player_position_x[i]);
        float32 py = fixed32_to_float32(state->player_position_y[i]);

        uint8 red  = (uint8)(255 * (1 - i));
        uint8 blue = (uint8)(255 * i);

        // Тень
        draw_circle(20, 40, 20,
            px + 3.0f, py - BLOBBY_LOWER_SPHERE - 3.0f, BLOBBY_LOWER_RADIUS);

        // Нижняя сфера (ниже центра в Y-up)
        draw_circle(red, 0, blue,
            px, py - BLOBBY_LOWER_SPHERE, BLOBBY_LOWER_RADIUS);
        // Верхняя сфера (выше центра в Y-up)
        draw_circle(red, 0, blue,
            px, py + BLOBBY_UPPER_SPHERE, BLOBBY_UPPER_RADIUS);
    }

    // ── Мяч ───────────────────────────────────────────────────────────────────
    float32 bx = fixed32_to_float32(state->ball_position_x);
    float32 by = fixed32_to_float32(state->ball_position_y);

    draw_circle(20, 40, 20,      bx + 3.0f, by - 3.0f, BALL_RADIUS);         // тень
    draw_circle(240, 240, 30,    bx, by, BALL_RADIUS);                        // мяч


    // ── Счёт (кружки вверху экрана) ───────────────────────────────────────────
    float32 score_y = GROUND_PLANE_HEIGHT_MAX - 20.0f;
    for (int32 s = 0; s < state->score[0]; s += 1)
        draw_circle(255, 80, 80, 30.0f + s * 18.0f, score_y, 7.0f);
    for (int32 s = 0; s < state->score[1]; s += 1)
        draw_circle(80, 80, 255, RIGHT_PLANE - 30.0f - s * 18.0f, score_y, 7.0f);

    render_bitmap(game->test_image, 100, 100);

    present_offscreen(game->offscreen);
}
