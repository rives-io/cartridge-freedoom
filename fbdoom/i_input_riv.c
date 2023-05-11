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
        case SAPP_KEYCODE_ENTER: return KEY_ENTER;
        case SAPP_KEYCODE_ESCAPE: return KEY_ESCAPE;
        case SAPP_KEYCODE_TAB: return KEY_TAB;
        case SAPP_KEYCODE_BACKSPACE: return KEY_BACKSPACE;
        case SAPP_KEYCODE_SPACE: return KEY_USE;
        case SAPP_KEYCODE_UP: return KEY_UPARROW;
        case SAPP_KEYCODE_DOWN: return KEY_DOWNARROW;
        case SAPP_KEYCODE_RIGHT: return KEY_RIGHTARROW;
        case SAPP_KEYCODE_LEFT: return KEY_LEFTARROW;
        case SAPP_KEYCODE_LEFT_CONTROL: return KEY_FIRE;
        case SAPP_KEYCODE_LEFT_SHIFT: return KEY_RSHIFT;
        case SAPP_KEYCODE_LEFT_ALT: return KEY_RALT;
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

