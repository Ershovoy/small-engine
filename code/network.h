typedef enum
{
    PACKET_CONNECT,
    PACKET_DISCONNECT,
    PACKET_PING,
    PACKET_PLAYER_INPUT,
    PACKET_TICK_INPUT,
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
    uint8 padding[8];
} Packet_connect;

typedef struct
{
    uint8 padding[8];
} Packet_disconnect;

typedef struct
{
    Player_input player_input;
} Packet_input;

typedef struct
{
    State state;
} Packet_game_state;
