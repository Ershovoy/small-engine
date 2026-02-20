static void render_game_state(Game_state* state)
{
    // ── 1. Background & table ─────────────────────────────────────────────
    clear(15, 15, 20);
    draw_rectangle(22, 28, 38, TABLE_LEFT, TABLE_TOP, TABLE_RIGHT, TABLE_BOTTOM);

    // ── 2. Center markings ────────────────────────────────────────────────
    draw_horizontal_line(55, 65, 80, (float32)TABLE_CY);
    draw_circle(55, 65, 80, (float32)TABLE_CX, (float32)TABLE_CY, 14.0f);
    draw_circle(180, 190, 200, (float32)TABLE_CX, (float32)TABLE_CY, 3.0f);

    // ── 3. Walls & goals ──────────────────────────────────────────────────
    // Левая / правая
    draw_rectangle(70, 80, 100, TABLE_LEFT,     TABLE_TOP, TABLE_LEFT + 2,  TABLE_BOTTOM);
    draw_rectangle(70, 80, 100, TABLE_RIGHT - 2, TABLE_TOP, TABLE_RIGHT,    TABLE_BOTTOM);

    // Верхняя стена
    draw_rectangle(70, 80, 100, TABLE_LEFT, TABLE_TOP, GOAL_LEFT,  TABLE_TOP + 2);
    draw_rectangle(70, 80, 100, GOAL_RIGHT, TABLE_TOP, TABLE_RIGHT, TABLE_TOP + 2);
    draw_rectangle(180, 80, 80, GOAL_LEFT,  TABLE_TOP, GOAL_RIGHT,  TABLE_TOP + 2);

    // Нижняя стена
    draw_rectangle(70, 80, 100, TABLE_LEFT, TABLE_BOTTOM - 2, GOAL_LEFT,   TABLE_BOTTOM);
    draw_rectangle(70, 80, 100, GOAL_RIGHT, TABLE_BOTTOM - 2, TABLE_RIGHT, TABLE_BOTTOM);
    draw_rectangle(80, 80, 180, GOAL_LEFT,  TABLE_BOTTOM - 2, GOAL_RIGHT,  TABLE_BOTTOM);

    // ── 4. Score ──────────────────────────────────────────────────────────
    static const uint16 digit_segments[10] =
    {
        0b111101101101111,
        0b010010010010010,
        0b111001111100111,
        0b111001111001111,
        0b101101111001001,
        0b111100111001111,
        0b111100111101111,
        0b111001001001001,
        0b111101111101111,
        0b111101111001111,
    };

    #define DRAW_DIGIT(digit, px, py, r, g, b)                           \
    do {                                                                   \
        uint16 _bits = digit_segments[(digit) % 10];                      \
        for (int32 _row = 0; _row < 5; _row += 1)                         \
        {                                                                  \
            for (int32 _col = 0; _col < 3; _col += 1)                     \
            {                                                              \
                if (_bits & (1 << ((4 - _row) * 3 + (2 - _col))))         \
                {                                                          \
                    draw_rectangle((r), (g), (b),                         \
                        (px) + _col * 2, (py) + _row * 2,                 \
                        (px) + _col * 2 + 2, (py) + _row * 2 + 2);       \
                }                                                          \
            }                                                              \
        }                                                                  \
    } while(0)

    #define DRAW_SCORE(score, px, py, r, g, b)  \
        DRAW_DIGIT((score) / 10, (px),     (py), r, g, b); \
        DRAW_DIGIT((score) % 10, (px) + 8, (py), r, g, b)

    DRAW_SCORE(state->score[0], TABLE_CX - 8, TABLE_TOP    + 5,  220, 120, 100);
    DRAW_SCORE(state->score[1], TABLE_CX - 8, TABLE_BOTTOM - 15, 100, 120, 220);

    #undef DRAW_SCORE
    #undef DRAW_DIGIT

    // ── 5. Paddles ────────────────────────────────────────────────────────
    #define DRAW_PADDLE(px, py, or, og, ob, ir, ig, ib)          \
    do {                                                           \
        draw_circle(10, 12, 16, (px) + 1.5f, (py) + 2.0f,        \
                    (float32)PADDLE_RADIUS);                       \
        draw_circle(or, og, ob, (px), (py),                       \
                    (float32)PADDLE_RADIUS);                       \
        draw_circle(ir, ig, ib, (px), (py),                       \
                    (float32)(PADDLE_RADIUS - 3));                 \
        draw_circle(255, 230, 210, (px), (py), 2.5f);             \
    } while(0)

    DRAW_PADDLE(
        fixed32_to_float32(state->paddle_position_x[0]),
        fixed32_to_float32(state->paddle_position_y[0]),
        200, 80, 70, 230, 120, 100);

    DRAW_PADDLE(
        fixed32_to_float32(state->paddle_position_x[1]),
        fixed32_to_float32(state->paddle_position_y[1]),
        70, 80, 200, 100, 120, 230);

    #undef DRAW_PADDLE

    // ── 6. Puck ───────────────────────────────────────────────────────────
    #define DRAW_PUCK(px, py)                                                        \
    do {                                                                              \
        draw_circle(10, 12, 16,    (px) + 1.0f, (py) + 2.0f, (float32)PUCK_RADIUS); \
        draw_circle(200, 205, 215, (px), (py),  (float32)PUCK_RADIUS);               \
        draw_circle(150, 155, 165, (px), (py),  (float32)(PUCK_RADIUS - 2));         \
        draw_circle(90,  95,  105, (px), (py),  1.5f);                               \
    } while(0)

    DRAW_PUCK(
        fixed32_to_float32(state->puck_position_x),
        fixed32_to_float32(state->puck_position_y));

    #undef DRAW_PUCK

    present_offscreen(game->offscreen);
}
