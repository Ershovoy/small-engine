static void render_game_state(Game_state* state)
{
    // ── Background ────────────────────────────────────────────────────────────
    Color sky = { 0 };
    sky.red   = 80;
    sky.green = 140;
    sky.blue  = 200;
    clear(sky.red, sky.green, sky.blue);

    render_bitmap(game->background, 400, 300);

    // ── Мяч ───────────────────────────────────────────────────────────────────
    float32 bx = fixed32_to_float32(state->ball_position_x);
    float32 by = fixed32_to_float32(state->ball_position_y);

    draw_circle(20, 40, 20, bx + 3.0f, by - 3.0f + 100.0f, BALL_RADIUS);

    render_bitmap(game->ball, bx, by + 100.0f);

     // ── Блобы ─────────────────────────────────────────────────────────────────
    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        float32 px = fixed32_to_float32(state->player_position_x[i]);
        float32 py = fixed32_to_float32(state->player_position_y[i]);

        uint8 red  = (uint8)(255 * (1 - i));
        uint8 blue = (uint8)(255 * i);

        // Тень
        draw_circle(20, 40, 20,
            px + 2.0f, py - BLOBBY_LOWER_SPHERE - 2.0f + 100.0f, BLOBBY_LOWER_RADIUS);
        // draw_circle(20, 40, 20,
        //     px + 3.0f, py + BLOBBY_UPPER_SPHERE - 3.0f + 100.0f, BLOBBY_UPPER_RADIUS);

        render_bitmap(game->player, px, py + 100.0f);
    }

    //render_bitmap(game->font, game->font.width / 2.0f, game->font.height / 2.0f);

    // String s = STRING_LITERAL("Hello, world!");
    // draw_text(s.data, s.length, 16.0f, 128.0f, 255, 0, 0);
    // s = STRING_LITERAL("Update time: ");
    // String ss = int64_to_string(game->update_time / 1'000'000);
    // String sss = add_string(s, ss);
    // draw_text(sss.data, sss.length, 0.0f, 148.0f, 0,0,255);

    // String string = int64_to_string(MAXIMUM(game->frame_time, 1) / 1'000'000);
    // s = STRING_LITERAL("Frame time: ");
    // s = add_string(s, string);
    // draw_text(s.data, s.length, 0.0f, 164.0f, 0, 255, 255);

    // draw_line(255, 255, 255, game->offscreen.width / 2, game->offscreen.height / 2, (int32)(input.mouse.x), (int32)(input.mouse.y));

    // float32 a = input.mouse.x * 1000.0f;
    // string = int32_to_string((int32)(a));
    // s = STRING_LITERAL("Mouse X: ");
    // s = add_string(s, string);
    // draw_text(s.data, s.length, 0.0f, 64.0f, 0, 255, 255);

    // string = int32_to_string((int32)(input.mouse.y * 1000.0f));
    // s = STRING_LITERAL("Mouse Y: ");
    // s = add_string(s, string);
    // draw_text(s.data, s.length, 0.0f, 48.0f, 0, 255, 255);

    // string = int32_to_string((int32)(game->offscreen.width));
    // s = STRING_LITERAL("Width: ");
    // s = add_string(s, string);
    // draw_text(s.data, s.length, 0.0f, 32.0f, 0, 255, 255);

    // s = add_string(STRING_LITERAL("Height: "), int32_to_string((int32)(game->offscreen.height)));
    // draw_text(s.data, s.length, 0.0f, 16.0f, 0, 255, 255);

    // s = add_string(STRING_LITERAL("sleep ns: "), int32_to_string((int32)(game->sleep_time / 1)));
    // draw_text(s.data, s.length, 0.0f, 0.0f, 255, 127, 64);

    // draw_pixel_i(255,255,255,(int32)input.mouse.x, (int32)input.mouse.y);
    arena_clear(&game->string_arena);

    render_bitmap(game->cursor, (float32)((int32)input.mouse.x + 22), (float32)((int32)input.mouse.y - 24));

    present_offscreen(game->offscreen);
}
