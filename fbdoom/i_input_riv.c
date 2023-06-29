#include <stdlib.h>
#include <ctype.h>
#include <math.h>
#include <string.h>

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
#include "riv.h"

extern riv_context riv;

int vanilla_keyboard_mapping = 1;

int TranslateKey(int key) {
    switch(key) {
        case SAPP_KEYCODE_UP: return KEY_UPARROW;
        case SAPP_KEYCODE_DOWN: return KEY_DOWNARROW;
        case SAPP_KEYCODE_LEFT: return KEY_LEFTARROW;
        case SAPP_KEYCODE_RIGHT: return KEY_RIGHTARROW;

        case SAPP_KEYCODE_ESCAPE: return KEY_ESCAPE;
        case SAPP_KEYCODE_ENTER: return KEY_ENTER;
        case SAPP_KEYCODE_TAB: return KEY_TAB;
        case SAPP_KEYCODE_BACKSPACE: return KEY_BACKSPACE;

        case SAPP_KEYCODE_RIGHT_CONTROL: return KEY_RCTRL;
        case SAPP_KEYCODE_LEFT_CONTROL: return KEY_RCTRL;
        case SAPP_KEYCODE_RIGHT_SHIFT: return KEY_RCTRL;
        case SAPP_KEYCODE_LEFT_SHIFT: return KEY_RSHIFT;
        case SAPP_KEYCODE_RIGHT_ALT: return KEY_RALT;
        case SAPP_KEYCODE_LEFT_ALT: return KEY_RALT;
        case SAPP_KEYCODE_F1: return KEY_F1;
        case SAPP_KEYCODE_F2: return KEY_F2;
        case SAPP_KEYCODE_F3: return KEY_F3;
        case SAPP_KEYCODE_F4: return KEY_F4;
        case SAPP_KEYCODE_F5: return KEY_F5;
        case SAPP_KEYCODE_F6: return KEY_F6;
        case SAPP_KEYCODE_F7: return KEY_F7;
        case SAPP_KEYCODE_F8: return KEY_F8;
        case SAPP_KEYCODE_F9: return KEY_F9;
        case SAPP_KEYCODE_F10: return KEY_F10;
        case SAPP_KEYCODE_F11: return KEY_F11;
        case SAPP_KEYCODE_F12: return KEY_F12;

        case SAPP_KEYCODE_INSERT: return KEY_INS;
        case SAPP_KEYCODE_HOME: return KEY_HOME;
        case SAPP_KEYCODE_DELETE: return KEY_DEL;
        case SAPP_KEYCODE_END: return KEY_END;
        case SAPP_KEYCODE_PAGE_UP: return KEY_PGUP;
        case SAPP_KEYCODE_PAGE_DOWN: return KEY_PGDN;

        case SAPP_KEYCODE_PAUSE: return KEY_PAUSE;
        case SAPP_KEYCODE_PRINT_SCREEN: return KEY_PRTSCR;

        case SAPP_KEYCODE_KP_0: return KEYP_0;
        case SAPP_KEYCODE_KP_1: return KEYP_1;
        case SAPP_KEYCODE_KP_2: return KEYP_2;
        case SAPP_KEYCODE_KP_3: return KEYP_3;
        case SAPP_KEYCODE_KP_4: return KEYP_4;
        case SAPP_KEYCODE_KP_5: return KEYP_5;
        case SAPP_KEYCODE_KP_6: return KEYP_6;
        case SAPP_KEYCODE_KP_7: return KEYP_7;
        case SAPP_KEYCODE_KP_8: return KEYP_8;
        case SAPP_KEYCODE_KP_9: return KEYP_9;
        case SAPP_KEYCODE_KP_DECIMAL: return KEYP_PERIOD;
        case SAPP_KEYCODE_KP_DIVIDE: return KEYP_DIVIDE;
        case SAPP_KEYCODE_KP_MULTIPLY: return KEYP_MULTIPLY;
        case SAPP_KEYCODE_KP_SUBTRACT: return KEYP_MINUS;
        case SAPP_KEYCODE_KP_ADD: return KEYP_PLUS;
        case SAPP_KEYCODE_KP_ENTER: return KEYP_ENTER;
        case SAPP_KEYCODE_KP_EQUAL: return KEYP_EQUALS;

        case SAPP_KEYCODE_0: return '0';
        case SAPP_KEYCODE_1: return '1';
        case SAPP_KEYCODE_2: return '2';
        case SAPP_KEYCODE_3: return '3';
        case SAPP_KEYCODE_4: return '4';
        case SAPP_KEYCODE_5: return '5';
        case SAPP_KEYCODE_6: return '6';
        case SAPP_KEYCODE_7: return '7';
        case SAPP_KEYCODE_8: return '8';
        case SAPP_KEYCODE_9: return '9';

        case SAPP_KEYCODE_A: return 'a';
        case SAPP_KEYCODE_B: return 'b';
        case SAPP_KEYCODE_C: return 'c';
        case SAPP_KEYCODE_D: return 'd';
        case SAPP_KEYCODE_E: return 'e';
        case SAPP_KEYCODE_F: return 'f';
        case SAPP_KEYCODE_G: return 'g';
        case SAPP_KEYCODE_H: return 'h';
        case SAPP_KEYCODE_I: return 'i';
        case SAPP_KEYCODE_J: return 'j';
        case SAPP_KEYCODE_K: return 'k';
        case SAPP_KEYCODE_L: return 'l';
        case SAPP_KEYCODE_M: return 'm';
        case SAPP_KEYCODE_N: return 'n';
        case SAPP_KEYCODE_O: return 'o';
        case SAPP_KEYCODE_P: return 'p';
        case SAPP_KEYCODE_Q: return 'q';
        case SAPP_KEYCODE_R: return 'r';
        case SAPP_KEYCODE_S: return 's';
        case SAPP_KEYCODE_T: return 't';
        case SAPP_KEYCODE_U: return 'u';
        case SAPP_KEYCODE_V: return 'v';
        case SAPP_KEYCODE_W: return 'w';
        case SAPP_KEYCODE_X: return 'x';
        case SAPP_KEYCODE_Y: return 'y';
        case SAPP_KEYCODE_Z: return 'z';

        case SAPP_KEYCODE_MINUS: return KEY_MINUS;
        case SAPP_KEYCODE_EQUAL: return KEY_EQUALS;
        case SAPP_KEYCODE_SPACE: return ' ';
        case SAPP_KEYCODE_APOSTROPHE: return '\'';
        case SAPP_KEYCODE_COMMA: return ',';
        case SAPP_KEYCODE_PERIOD: return '.';
        case SAPP_KEYCODE_SLASH: return '/';
        case SAPP_KEYCODE_SEMICOLON: return ';';
        case SAPP_KEYCODE_LEFT_BRACKET: return '[';
        case SAPP_KEYCODE_BACKSLASH: return '\\';
        case SAPP_KEYCODE_RIGHT_BRACKET: return ']';
        case SAPP_KEYCODE_GRAVE_ACCENT: return '`';
    }
    return 0;
}

void I_GetEvent(void)
{
    if(!riv.desc || riv.desc->frame == 0)
        return;

    for (int i=0;i<512;++i) {
        int k = TranslateKey(i);
        if (k != 0) {
            if(riv.desc->keys[i].down_frame == riv.desc->frame) {
                event_t event = {0};
                event.type = ev_keydown;
                event.data1 = k;
                event.data2 = k;
                D_PostEvent(&event);
            }
            if(riv.desc->keys[i].up_frame == riv.desc->frame) {
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

