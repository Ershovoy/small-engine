static void render_game_state(Game_state* state)
{
    Color clear_color = { 0 };
    clear_color.red = (uint8)(41);
    clear_color.green = (uint8)(173);
    clear_color.blue = (uint8)(255);
    clear(game->offscreen, clear_color);

    //draw_rectangle(game->offscreen, (Color){255, 163, 0}, 0, 0, GAME_HORIZONTAL_RESOLUTION, GROUND_HEIGHT);

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
