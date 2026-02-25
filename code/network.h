typedef enum
{
    PACKET_EMPTY,
    PACKET_CONNECT,
    PACKET_DISCONNECT,
    PACKET_PING,
    PACKET_PLAYER_INPUT,
    PACKET_TICK_INPUT,
    PACKET_TICK_INPUT_BATCH,
    PACKET_GAME_STATE,
    PACKETS_COUNT
} Packet_type;

typedef struct
{
    Packet_type type;
    uint8 padding[4];
} Packet_header;

typedef struct
{
    Packet_header header;
    uint8 padding[8];
} Packet_connect;

typedef struct
{
    Packet_header header;
    uint8 padding[8];
} Packet_disconnect;

typedef struct
{
    Packet_header header;
    uint8 padding[8];
} Packet_ping;

typedef struct
{
    Packet_header header;
    Player_input player_input;
} Packet_player_input;

typedef struct
{
    Packet_header header;
    Tick_input tick_input;
} Packet_tick_input;

#define TICK_INPUT_BATCH_SIZE (32)

typedef struct
{
    Packet_header header;
    Tick_input tick_inputs[TICK_INPUT_BATCH_SIZE];
} Packet_tick_input_batch;

typedef struct
{
    Packet_header header;
    Game_state game_state;
} Packet_game_state;
