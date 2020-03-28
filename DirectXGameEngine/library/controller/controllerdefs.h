/************************************************************************
*    FILE NAME:       controllerdefs.h
*
*    DESCRIPTION:     game controller defines
************************************************************************/

#ifndef __controller_defs_h__
#define __controller_defs_h__

#define DIRECTINPUT_VERSION 0x0800

// DirectX lib dependencies
#include <dinput.h>

// Standard lib dependencies
#include <vector>
#include <string>

namespace NDevice
{
    static const std::string UNBOUND_KEYBINDING_ID("---");

    enum EDeviceId
    {
        DEVICE_NULL=-1,
        KEYBOARD,
        MOUSE,
        JOYPAD,
        MAX_UNIQUE_DEVICES
    };

    enum EDevicePlayerId
    {
        DEVICE_PLAYER_NULL=-1,
        MOUSE_KEYBOARD,
        JOYPAD1, // If the value of this changes, need to update CGameController::CreateJoyPadDevice 
        JOYPAD2, // So that the right multiplier ID is set
        JOYPAD3,
        JOYPAD4,
        JOYPAD5,
        JOYPAD6,
        JOYPAD7,
        JOYPAD8,
    };

    enum EMultiPlayerId
    {
        MULTIPLAYER_NULL = -1,
        PLAYER_1,
        PLAYER_2,
        PLAYER_3,
        PLAYER_4,
        PLAYER_5,
        PLAYER_6,
        PLAYER_7,
        PLAYER_8,
    };

    enum EJoyPadComponetId
    {
        JOYPAD_BUTTON_NULL = -1,
        BUTTON_01,
        BUTTON_02,
        BUTTON_03,
        BUTTON_04,
        BUTTON_05,
        BUTTON_06,
        BUTTON_07,
        BUTTON_08,
        BUTTON_09,
        BUTTON_10,
        BUTTON_11,
        BUTTON_12,
        BUTTON_13,
        BUTTON_14,
        BUTTON_15,
        BUTTON_16,
        BUTTON_17,
        BUTTON_18,
        BUTTON_19,
        BUTTON_20,
        BUTTON_21,
        BUTTON_22,
        BUTTON_23,
        BUTTON_24,
        BUTTON_25,
        BUTTON_26,
        BUTTON_27,
        BUTTON_28,
        BUTTON_29,
        BUTTON_30,
        BUTTON_31,
        BUTTON_32,
        MAX_JOYPAD_BTNS,

        XBOX_360_TRIGGER_LEFT = 500,
        XBOX_360_TRIGGER_RIGHT,
        MAX_360_TRIGGERS,

        POV_UP         = 1000,
        POV_UP_RIGHT   = 4500,
        POV_RIGHT      = 9000,
        POV_DOWN_RIGHT = 13500,
        POV_DOWN       = 18000,
        POV_DOWN_LEFT  = 22500,
        POV_LEFT       = 27000,
        POV_UP_LEFT    = 31500,
        MAX_POV_BTNS,
        POV_IDLE = 0xFFFFFFFF,

        ANALOG1_UP = 50000,
        ANALOG1_DOWN,
        ANALOG1_LEFT,
        ANALOG1_RIGHT,
        ANALOG2_UP,
        ANALOG2_DOWN,
        ANALOG2_LEFT,
        ANALOG2_RIGHT,
        MAX_ANALOG_DIR,
    };

    enum EMouseComponetId
    {
        MOUSE_BUTTON_NULL = -1,
        MOUSE_BUTTON_LEFT,
        MOUSE_BUTTON_RIGHT,
        MOUSE_BUTTON_CENTER,
        MOUSE_BUTTON_04,
        MOUSE_BUTTON_05,
        MOUSE_BUTTON_06,
        MOUSE_BUTTON_07,
        MOUSE_BUTTON_08,
        MAX_MOUSE_BTNS,
        MOUSE_MOVE_X_Y,
        MOUSE_MOVE_Z,
        MAX_MOUSE_MOVE
    };

    enum EKeyboardComponetId
    {
        KEY_NULL=-1,
        KEY_ESCAPE = DIK_ESCAPE,       
        KEY_1,       
        KEY_2,
        KEY_3,
        KEY_4,
        KEY_5,
        KEY_6,
        KEY_7,
        KEY_8,
        KEY_9,
        KEY_0,
        KEY_MINUS,
        KEY_EQUAL,
        KEY_BACK,
        KEY_TAB,     
        KEY_Q,  
        KEY_W,
        KEY_E,
        KEY_R,
        KEY_T,
        KEY_Y,
        KEY_U,
        KEY_I,
        KEY_O,
        KEY_P, 
        KEY_LBRACKET,
        KEY_RBRACKET,
        KEY_RETURN,  
        KEY_LCONTROL,
        KEY_A,
        KEY_S,
        KEY_D,
        KEY_F,
        KEY_G,
        KEY_H,
        KEY_J,
        KEY_K,
        KEY_L,
        KEY_SEMICOLON,
        KEY_APOSTROPHE,
        KEY_GRAVE,
        KEY_LSHIFT,
        KEY_BACKSLASH,
        KEY_Z,
        KEY_X,
        KEY_C,
        KEY_V,
        KEY_B,   
        KEY_N, 
        KEY_M,
        KEY_COMMA,
        KEY_PERIOD,
        KEY_SLASH,
        KEY_RSHIFT,
        KEY_MULTIPLY,
        KEY_LMENU,
        KEY_SPACE,
        KEY_CAPITAL,
        KEY_F1, 
        KEY_F2,
        KEY_F3,
        KEY_F4,
        KEY_F5, 
        KEY_F6,   
        KEY_F7,
        KEY_F8,
        KEY_F9,
        KEY_F10,
        KEY_NUMLOCK,    
        KEY_SCROLL,  
        KEY_NUMPAD7,  
        KEY_NUMPAD8, 
        KEY_NUMPAD9,
        KEY_SUBTRACT,
        KEY_NUMPAD4, 
        KEY_NUMPAD5,
        KEY_NUMPAD6,
        KEY_ADD,     
        KEY_NUMPAD1,  
        KEY_NUMPAD2, 
        KEY_NUMPAD3, 
        KEY_NUMPAD0, 
        KEY_DECIMAL, 
        KEY_OEM_102 = DIK_OEM_102,
        KEY_F11,   
        KEY_F12,   
        KEY_F13 = DIK_F13, 
        KEY_F14,    
        KEY_F15,
    
        KEY_KANA         = DIK_KANA,   
        KEY_ABNT_C1      = DIK_ABNT_C1,  
        KEY_CONVERT      = DIK_CONVERT, 
        KEY_NOCONVERT    = DIK_NOCONVERT,
        KEY_YEN          = DIK_YEN,     
        KEY_ABNT_C2      = DIK_ABNT_C2,
        KEY_NUMPADEQUAL  = DIK_NUMPADEQUALS,
        KEY_PREVTRACK    = DIK_PREVTRACK,
        KEY_AT           = DIK_AT,
        KEY_COLON        = DIK_COLON,
        KEY_UNDERLINE    = DIK_UNDERLINE,
        KEY_KANJI        = DIK_KANJI,
        KEY_STOP         = DIK_STOP,
        KEY_AX           = DIK_AX,
        KEY_UNLABELED    = DIK_UNLABELED,
        KEY_NEXTTRACK    = DIK_NEXTTRACK,
        KEY_NUMPADENTER  = DIK_NUMPADENTER,
        KEY_RCONTROL     = DIK_RCONTROL,
        KEY_MUTE         = DIK_MUTE,
        KEY_CALCULATOR   = DIK_CALCULATOR,
        KEY_PLAYPAUSE    = DIK_PLAYPAUSE,
        KEY_MEDIASTOP    = DIK_MEDIASTOP,
        KEY_VOLUMEDOWN   = DIK_VOLUMEDOWN,
        KEY_VOLUMEUP     = DIK_VOLUMEUP,
        KEY_WEBHOME      = DIK_WEBHOME,
        KEY_NUMPADCOMMA  = DIK_NUMPADCOMMA,
        KEY_DIVIDE       = DIK_DIVIDE,
        KEY_SYSRQ        = DIK_SYSRQ,
        KEY_RMENU        = DIK_RMENU,
        KEY_PAUSE        = DIK_PAUSE,
        KEY_HOME         = DIK_HOME,
        KEY_UP           = DIK_UP,
        KEY_PRIOR        = DIK_PRIOR,
        KEY_LEFT         = DIK_LEFT,
        KEY_RIGHT        = DIK_RIGHT,
        KEY_END          = DIK_END,
        KEY_DOWN         = DIK_DOWN,
        KEY_NEXT         = DIK_NEXT,
        KEY_INSERT       = DIK_INSERT,
        KEY_DELETE       = DIK_DELETE,
        KEY_LWIN         = DIK_LWIN,
        KEY_RWIN         = DIK_RWIN,    
        KEY_APPS         = DIK_APPS,
        KEY_POWER        = DIK_POWER,
        KEY_SLEEP        = DIK_SLEEP,  
        KEY_WAKE         = DIK_WAKE,
        KEY_WEBSEARCH    = DIK_WEBSEARCH,
        KEY_WEBFAVORITES = DIK_WEBFAVORITES,
        KEY_WEBREFRESH   = DIK_WEBREFRESH,
        KEY_WEBSTOP      = DIK_WEBSTOP,
        KEY_WEBFORWARD   = DIK_WEBFORWARD,
        KEY_WEBBACK      = DIK_WEBBACK,
        KEY_MYCOMPUTER   = DIK_MYCOMPUTER,
        KEY_MAIL         = DIK_MAIL,
        KEY_MEDIASELECT  = DIK_MEDIASELECT,

        MAX_KEYS = 256
    };

    enum EActionPress
    {
        EAP_IDLE=0,
        EAP_DOWN,
        EAP_HOLD,
        EAP_UP
    };
}

#endif  // __controller_defs_h__
