typedef enum
{
    BUTTON_NULL = 0x00,

    // Mouse buttons

    BUTTON_LEFT       = 0x01,
    BUTTON_MIDDLE     = 0x02,
    BUTTON_RIGHT      = 0x03,
    BUTTON_FORWARD    = 0x04,
    BUTTON_BACK       = 0x05,
    BUTTON_WHEEL_UP   = 0x06,
    BUTTON_WHEEL_DOWN = 0x07,

    // Keyboard keys

    KEY_0 = 0x30,
    KEY_1 = 0x31,
    KEY_2 = 0x32,
    KEY_3 = 0x33,
    KEY_4 = 0x34,
    KEY_5 = 0x35,
    KEY_6 = 0x36,
    KEY_7 = 0x37,
    KEY_8 = 0x38,
    KEY_9 = 0x39,

    KEY_F1  = 0x70,
    KEY_F2  = 0x71,
    KEY_F3  = 0x72,
    KEY_F4  = 0x73,
    KEY_F5  = 0x74,
    KEY_F6  = 0x75,
    KEY_F7  = 0x76,
    KEY_F8  = 0x77,
    KEY_F9  = 0x78,
    KEY_F10 = 0x79,
    KEY_F11 = 0x7A,
    KEY_F12 = 0x7B,

    KEY_A = 0x41,
    KEY_B = 0x42,
    KEY_C = 0x43,
    KEY_D = 0x44,
    KEY_E = 0x45,
    KEY_F = 0x46,
    KEY_G = 0x47,
    KEY_H = 0x48,
    KEY_I = 0x49,
    KEY_J = 0x4A,
    KEY_K = 0x4B,
    KEY_L = 0x4C,
    KEY_M = 0x4D,
    KEY_N = 0x4E,
    KEY_O = 0x4F,
    KEY_P = 0x50,
    KEY_Q = 0x51,
    KEY_R = 0x52,
    KEY_S = 0x53,
    KEY_T = 0x54,
    KEY_U = 0x55,
    KEY_V = 0x56,
    KEY_W = 0x57,
    KEY_X = 0x58,
    KEY_Y = 0x59,
    KEY_Z = 0x5A,

    KEY_LEFT_BRACKET  = 0xDB, // Key: [
    KEY_RIGHT_BRACKET = 0xDD, // Key: ]
    KEY_BACKSLASH     = 0xBF, // Key: '\'
    KEY_GRAVE         = 0xC0, // Key: `
    KEY_APOSTROPHE    = 0xDE, // Key: '
    KEY_COMMA         = 0xBC, // Key: ,
    KEY_MINUS         = 0xBD, // Key: -
    KEY_PERIOD        = 0xBE, // Key: .
    KEY_SLASH         = 0xBF, // Key: /
    KEY_SEMICOLON     = 0xBA, // Key: ;
    KEY_EQUAL         = 0xBB, // Key: =

    KEY_LEFT  = 0x25, // Key: Arrow left
    KEY_UP    = 0x26, // Key: Arrow up
    KEY_RIGHT = 0x27, // Key: Arrow right
    KEY_DOWN  = 0x28, // Key: Arrow down

    KEY_SPACE        = 0x20, // Key: Space
    KEY_ESCAPE       = 0x1B, // Key: Esc
    KEY_ENTER        = 0x0D, // Key: Enter
    KEY_TAB          = 0x09, // Key: Tab
    KEY_BACKSPACE    = 0x08, // Key: Backspace
    KEY_INSERT       = 0x2D, // Key: Ins
    KEY_DELETE       = 0x2E, // Key: Del
    KEY_PAGE_UP      = 0x21, // Key: Page up
    KEY_PAGE_DOWN    = 0x22, // Key: Page down
    KEY_HOME         = 0x24, // Key: Home
    KEY_END          = 0x23, // Key: End
    KEY_CAPS_LOCK    = 0x14, // Key: Caps lock
    KEY_SCROLL_LOCK  = 0x91, // Key: Scroll down
    KEY_PRINT_SCREEN = 0x2C, // Key: Print screen
    KEY_PAUSE        = 0x13, // Key: Pause

    KEY_SHIFT_LEFT    = 0x10, // Key: Shift left
    KEY_SHIFT_RIGHT   = 0x10, // Key: Shift right
    KEY_ALT_LEFT      = 0x12, // Key: Alt left
    KEY_ALT_RIGHT     = 0x12, // Key: Alt right
    KEY_CONTROL_LEFT  = 0x11, // Key: Control left
    KEY_CONTROL_RIGHT = 0x11, // Key: Control right
    KEY_SUPER_LEFT    = 0x5B, // Key: Super left
    KEY_SUPER_RIGHT   = 0x5C, // Key: Super right

    KEY_MENU = 0x5D, // Key: Menu (???)

    BUTTONS_COUNT = 0xFF
} Button;

typedef struct
{
    bool32 is_pressed;
    bool32 is_down;
    bool32 is_released;
} Button_state;

typedef struct
{
    Button_state buttons[BUTTONS_COUNT];
} Input;

static Input input = { 0 };
