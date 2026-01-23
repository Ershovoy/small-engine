static void initialize_game_state(Game_state* state)
{
    for (int32 i = 0; i < MAX_PLAYERS; i += 1)
    {
        state->player_positions[i].e1 = (i * i * 2);
        state->player_positions[i].e2 = (i * i * 2) & 0x01111111;
    }
}

static void update_game_state(Game_state* state, Tick_input input)
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
