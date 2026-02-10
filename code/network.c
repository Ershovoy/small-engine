
// static void server_handle_packet(void* packet, uint32 ip, uint16 port)
// {
//     Packet_header* packet_header = (Packet_header*)packet;
//     void* packet_payload = (uint8*)packet;
//     switch (packet_header->type)
//     {
//         case PACKET_CONNECT:
//         {
//             for (int32 i = 0; i < MAX_PLAYERS; i += 1)
//             {
//                 if (!game->is_connected[i])
//                 {
//                     Packet_game_state packet_game_state = { 0 };
//                     packet_game_state.header.type = PACKET_GAME_STATE;
//                     packet_game_state.game_state = game->server_state;
//                     net_send(&packet_game_state, sizeof(Packet_game_state), ip, port);

//                     game->ips[i] = ip;
//                     game->ports[i] = port;
//                     game->is_connected[i] = 1;

//                     break;
//                 }
//             }

//             break;
//         }
//         case PACKET_DISCONNECT:
//         {
//             for (int32 i = 0; i < MAX_PLAYERS; i += 1)
//             {
//                 if (game->ips[i] == ip, game->ports[i] == port)
//                 {
//                     game->ips[i] = 0;
//                     game->ports[i] = 0;
//                     game->is_connected[i] = 0;

//                     break;
//                 }
//             }

//             break;
//         }
//         case PACKET_PLAYER_INPUT:
//         {
//             Packet_player_input* packet_player_input = (Packet_player_input*)packet_payload;

//             for (int32 i = 0; i < MAX_PLAYERS; i += 1)
//             {
//                 if (game->ips[i] == ip, game->ports[i] == port)
//                 {
//                     game->tick_input.player_inputs[i] = packet_player_input->player_input;

//                     break;
//                 }
//             }

//             break;
//         }
//         default:
//         {
//             break;
//         }
//     }
// }

// static void client_handle_packet(void* packet)
// {
//     Packet_header* packet_header = (Packet_header*)packet;
//     void* packet_payload = (uint8*)packet;
//     switch (packet_header->type)
//     {
//         case PACKET_TICK_INPUT:
//         {
//             Packet_tick_input* packet_tick_input = (Packet_tick_input*)packet_payload;

//             update_game_state(&game->client_state, packet_tick_input->tick_input);
//             render_game_state(&game->client_state);

//             break;
//         }
//         case PACKET_GAME_STATE:
//         {
//             Packet_game_state* packet_game_state = (Packet_game_state*)packet_payload;

//             game->client_state = packet_game_state->game_state;

//             break;
//         }
//         default:
//         {
//             break;
//         }
//     }
// }

// static void client()
// {
//     uint32 out_ip = { 0 };
//     uint16 out_port = { 0 };
//     uint8 packet_buffer[2048] = { 0 };
//     while (net_receive(&packet_buffer, sizeof(packet_buffer), &out_ip, &out_port))
//     {
//         client_handle_packet(packet_buffer);
//     }

//     Player_input player_input = update_game();
//     Packet_player_input packet_player_input = { 0 };
//     packet_player_input.header.type = PACKET_PLAYER_INPUT;
//     packet_player_input.player_input = player_input;
//     net_send(&packet_player_input, sizeof(Packet_player_input), 0x7f000001, 0xFFFF);
// }

// static void server()
// {
//     game->current_time = get_time_tick() - game->start_time;
//     int64 delta_time = game->current_time - game->previous_time;
//     if (delta_time > game->time_per_update)
//     {
//         delta_time = game->time_per_update;
//     }

//     game->previous_time = game->current_time;
//     game->accumulator += delta_time;
//     if (game->accumulator >= game->time_per_update)
//     {
//         uint32 out_ip = { 0 };
//         uint16 out_port = { 0 };
//         uint8 packet_buffer[2048] = { 0 };
//         while(net_receive(&packet_buffer, sizeof(packet_buffer), &out_ip, &out_port))
//         {
//             server_handle_packet((void*)&packet_buffer, out_ip, out_port);
//         }

//         game->tick_input.tick = game->server_state.tick;
//         Packet_tick_input packet_tick_input = { 0 };
//         packet_tick_input.header.type = PACKET_TICK_INPUT;
//         packet_tick_input.tick_input = game->tick_input;
//         for (int32 i = 0; i < MAX_PLAYERS; i += 1)
//         {
//             if (game->is_connected[i])
//             {
//                 net_send(&packet_tick_input, sizeof(Packet_tick_input), game->ips[i], game->ports[i]);
//             }
//         }

//         update_game_state(&game->server_state, game->tick_input);
//         render_game_state(&game->server_state);
//         game->tick_input = (Tick_input){ 0 };

//         game->accumulator -= game->time_per_update;
//     }
// }


// static void game_loop()
// {
//     if (is_button_pressed(KEY_F1))
//     {
//         if (game->is_offline)
//         {
//             game->is_client = 1;
//             game->is_offline = 0;
//             Packet_connect packet_connect = { 0 };
//             packet_connect.header.type = PACKET_CONNECT;
//             net_send(&packet_connect, sizeof(Packet_connect), 0x7f000001, 0xFFFF);
//         }
//     }
//     if (is_button_pressed(KEY_F2))
//     {
//         if (game->is_offline)
//         {
//             game->is_server = 1;
//             game->is_offline = 0;
//             uint16 binded_port = net_bind(0xFFFF);
//             initialize_game_state(&game->server_state);
//         }
//     }
//     if (is_button_pressed(KEY_F3))
//     {
//         if (game->is_offline)
//         {
//         //     game->is_server = 1;
//         //     game->is_client = 1;
//         //     game->is_offline = 0;
//         //     uint16 binded_port = net_bind(0xFFFF);
//         //     initialize_game_state(&game->server_state);

//         //     Packet_connect packet_connect = { 0 };
//         //     packet_connect.header.type = PACKET_CONNECT;
//         //     net_send(&packet_connect, sizeof(Packet_connect), 0x7f000001, 0xFFFF);
//         }
//     }

//     if (game->is_server)
//     {
//         server();
//     }
//     if (game->is_client)
//     {
//         client();
//     }

//     sleep(1'000);
// }

// static void deinitialize_game()
// {
//     // if (game->is_client)
//     // {
//     //     Packet_disconnect packet_disconnect = { 0 };
//     //     packet_disconnect.header.type = PACKET_DISCONNECT;
//     //     net_send(&packet_disconnect, sizeof(Packet_disconnect), 0x7f000001, 0xFFFF);
//     // }
// }
