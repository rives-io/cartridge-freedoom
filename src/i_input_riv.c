#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

#include <riv.h>

#include "config.h"
#include "deh_str.h"
#include "doomtype.h"
#include "doomkeys.h"
#include "i_joystick.h"
#include "i_system.h"
#include "i_swap.h"
#include "i_timer.h"
#include "i_video.h"
#include "i_scale.h"
#include "m_argv.h"
#include "m_config.h"
#include "m_misc.h"
#include "tables.h"
#include "v_video.h"
#include "w_wad.h"
#include "z_zone.h"

int vanilla_keyboard_mapping = 1;

int TranslateKey(int key) {
    switch(key) {
        case RIV_GAMEPAD_UP: return KEY_UPARROW;
        case RIV_GAMEPAD_DOWN: return KEY_DOWNARROW;
        case RIV_GAMEPAD_LEFT: return KEY_LEFTARROW;
        case RIV_GAMEPAD_RIGHT: return KEY_RIGHTARROW;
        case RIV_GAMEPAD_A1: return KEY_RSHIFT; // Run
        case RIV_GAMEPAD_A2: return KEY_ENTER; // Message
        case RIV_GAMEPAD_A3: return ' '; // Use
        case RIV_GAMEPAD_A4: return KEY_BACKSPACE; // Back
        case RIV_GAMEPAD_L1: return KEY_PREVWEAPON; // Previous weapon
        case RIV_GAMEPAD_R1: return KEY_NEXTWEAPON; // Next weapon
        case RIV_GAMEPAD_L2: return KEY_RALT; // Strafe
        case RIV_GAMEPAD_R2: return KEY_RCTRL; // Fire
        case RIV_GAMEPAD_SELECT: return KEY_ESCAPE; // Menu
        case RIV_GAMEPAD_START: return KEY_TAB; // Map
        // case RIV_GAMEPAD_L3: return ;
        // case RIV_GAMEPAD_R3: return ;

        case RIV_KEYCODE_UP: return KEY_UPARROW;
        case RIV_KEYCODE_DOWN: return KEY_DOWNARROW;
        case RIV_KEYCODE_LEFT: return KEY_LEFTARROW;
        case RIV_KEYCODE_RIGHT: return KEY_RIGHTARROW;

        case RIV_KEYCODE_ESCAPE: return KEY_ESCAPE;
        case RIV_KEYCODE_ENTER: return KEY_ENTER;
        case RIV_KEYCODE_TAB: return KEY_TAB;
        case RIV_KEYCODE_BACKSPACE: return KEY_BACKSPACE;

        case RIV_KEYCODE_RIGHT_CTRL: return KEY_RCTRL;
        case RIV_KEYCODE_LEFT_CTRL: return KEY_RCTRL;
        case RIV_KEYCODE_RIGHT_SHIFT: return KEY_RSHIFT;
        case RIV_KEYCODE_LEFT_SHIFT: return KEY_RSHIFT;
        case RIV_KEYCODE_RIGHT_ALT: return KEY_RALT;
        case RIV_KEYCODE_LEFT_ALT: return KEY_RALT;
        case RIV_KEYCODE_F1: return KEY_F1;
        case RIV_KEYCODE_F2: return KEY_F2;
        case RIV_KEYCODE_F3: return KEY_F3;
        case RIV_KEYCODE_F4: return KEY_F4;
        case RIV_KEYCODE_F5: return KEY_F5;
        case RIV_KEYCODE_F6: return KEY_F6;
        case RIV_KEYCODE_F7: return KEY_F7;
        case RIV_KEYCODE_F8: return KEY_F8;
        case RIV_KEYCODE_F9: return KEY_F9;
        case RIV_KEYCODE_F10: return KEY_F10;
        case RIV_KEYCODE_F11: return KEY_F11;
        case RIV_KEYCODE_F12: return KEY_F12;

        case RIV_KEYCODE_INSERT: return KEY_INS;
        case RIV_KEYCODE_HOME: return KEY_HOME;
        case RIV_KEYCODE_DELETE: return KEY_DEL;
        case RIV_KEYCODE_END: return KEY_END;
        case RIV_KEYCODE_PAGE_UP: return KEY_PGUP;
        case RIV_KEYCODE_PAGE_DOWN: return KEY_PGDN;

        case RIV_KEYCODE_PAUSE: return KEY_PAUSE;
        case RIV_KEYCODE_PRINT_SCREEN: return KEY_PRTSCR;

        case RIV_KEYCODE_KP_0: return KEYP_0;
        case RIV_KEYCODE_KP_1: return KEYP_1;
        case RIV_KEYCODE_KP_2: return KEYP_2;
        case RIV_KEYCODE_KP_3: return KEYP_3;
        case RIV_KEYCODE_KP_4: return KEYP_4;
        case RIV_KEYCODE_KP_5: return KEYP_5;
        case RIV_KEYCODE_KP_6: return KEYP_6;
        case RIV_KEYCODE_KP_7: return KEYP_7;
        case RIV_KEYCODE_KP_8: return KEYP_8;
        case RIV_KEYCODE_KP_9: return KEYP_9;
        case RIV_KEYCODE_KP_DECIMAL: return KEYP_PERIOD;
        case RIV_KEYCODE_KP_DIVIDE: return KEYP_DIVIDE;
        case RIV_KEYCODE_KP_MULTIPLY: return KEYP_MULTIPLY;
        case RIV_KEYCODE_KP_SUBTRACT: return KEYP_MINUS;
        case RIV_KEYCODE_KP_ADD: return KEYP_PLUS;
        case RIV_KEYCODE_KP_ENTER: return KEYP_ENTER;
        case RIV_KEYCODE_KP_EQUAL: return KEYP_EQUALS;

        case RIV_KEYCODE_0: return '0';
        case RIV_KEYCODE_1: return '1';
        case RIV_KEYCODE_2: return '2';
        case RIV_KEYCODE_3: return '3';
        case RIV_KEYCODE_4: return '4';
        case RIV_KEYCODE_5: return '5';
        case RIV_KEYCODE_6: return '6';
        case RIV_KEYCODE_7: return '7';
        case RIV_KEYCODE_8: return '8';
        case RIV_KEYCODE_9: return '9';

        case RIV_KEYCODE_A: return 'a';
        case RIV_KEYCODE_B: return 'b';
        case RIV_KEYCODE_C: return 'c';
        case RIV_KEYCODE_D: return 'd';
        case RIV_KEYCODE_E: return 'e';
        case RIV_KEYCODE_F: return 'f';
        case RIV_KEYCODE_G: return 'g';
        case RIV_KEYCODE_H: return 'h';
        case RIV_KEYCODE_I: return 'i';
        case RIV_KEYCODE_J: return 'j';
        case RIV_KEYCODE_K: return 'k';
        case RIV_KEYCODE_L: return 'l';
        case RIV_KEYCODE_M: return 'm';
        case RIV_KEYCODE_N: return 'n';
        case RIV_KEYCODE_O: return 'o';
        case RIV_KEYCODE_P: return 'p';
        case RIV_KEYCODE_Q: return 'q';
        case RIV_KEYCODE_R: return 'r';
        case RIV_KEYCODE_S: return 's';
        case RIV_KEYCODE_T: return 't';
        case RIV_KEYCODE_U: return 'u';
        case RIV_KEYCODE_V: return 'v';
        case RIV_KEYCODE_W: return 'w';
        case RIV_KEYCODE_X: return 'x';
        case RIV_KEYCODE_Y: return 'y';
        case RIV_KEYCODE_Z: return 'z';

        case RIV_KEYCODE_MINUS: return KEY_MINUS;
        case RIV_KEYCODE_EQUAL: return KEY_EQUALS;
        case RIV_KEYCODE_SPACE: return ' ';
        case RIV_KEYCODE_APOSTROPHE: return '\'';
        case RIV_KEYCODE_COMMA: return ',';
        case RIV_KEYCODE_PERIOD: return '.';
        case RIV_KEYCODE_SLASH: return '/';
        case RIV_KEYCODE_SEMICOLON: return ';';
        case RIV_KEYCODE_LEFT_BRACKET: return '[';
        case RIV_KEYCODE_BACKSLASH: return '\\';
        case RIV_KEYCODE_RIGHT_BRACKET: return ']';
        case RIV_KEYCODE_GRAVE_ACCENT: return '`';
    }
    return 0;
}

void I_GetEvent(void)
{
    for (int i=0;i<RIV_NUM_KEYCODE;++i) {
        int k = TranslateKey(i);
        if (k != 0) {
            if(riv->keys[i].press) {
                event_t event = {0};
                event.type = ev_keydown;
                event.data1 = k;
                event.data2 = k;
                D_PostEvent(&event);
            }
            if(riv->keys[i].release) {
                event_t event = {0};
                event.type = ev_keyup;
                event.data1 = k;
                event.data2 = k;
                D_PostEvent(&event);
            }
        }
    }
}

void I_InitInput(void)
{
}
