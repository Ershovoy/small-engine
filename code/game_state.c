static void initialize_game_state(Game_state* state)
{
    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        state->player_positions[i].e1 = (i * i * 2);
        state->player_positions[i].e2 = (i * i * 2) & 0x01111111;
    }

    state->position.e1 = GAME_HORIZONTAL_RESOLUTION / 2.0f;
    state->position.e2 = GAME_VERTICAL_RESOLUTION / 2.0f;
}

static void update_game_state(Game_state* state, Player_input input, float32 delta_time)
{
    Player_input player_input = input;

    float32 speed = 500.0f;

    if (player_input.is_up)
    {
        state->position.e2 += speed * delta_time;
    }
    if (player_input.is_left)
    {
        state->position.e1 -= speed * delta_time;
    }
    if (player_input.is_down)
    {
        state->position.e2 -= speed * delta_time;
    }
    if (player_input.is_right)
    {
        state->position.e1 += speed * delta_time;
    }

    state->mouse_position.e1 = (float32)input.mouse_x;
    state->mouse_position.e2 = (float32)input.mouse_y;

    state->tick += 1;
}

static void deinitialize_game_state(Game_state* state)
{

}
