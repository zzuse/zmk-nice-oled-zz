#include "keycode.h"
#include "../assets/custom_fonts.h"
#include <zephyr/kernel.h>
#include <zmk/hid.h>

static const char* keycode_to_string(uint32_t keycode, uint16_t usage_page) {
    // Handle standard keyboard keys (HID Usage Page 0x07)
    if (usage_page == HID_USAGE_KEY) {
        switch (keycode) {
            case HID_USAGE_KEY_KEYBOARD_A: return "A";
            case HID_USAGE_KEY_KEYBOARD_B: return "B";
            case HID_USAGE_KEY_KEYBOARD_C: return "C";
            case HID_USAGE_KEY_KEYBOARD_D: return "D";
            case HID_USAGE_KEY_KEYBOARD_E: return "E";
            case HID_USAGE_KEY_KEYBOARD_F: return "F";
            case HID_USAGE_KEY_KEYBOARD_G: return "G";
            case HID_USAGE_KEY_KEYBOARD_H: return "H";
            case HID_USAGE_KEY_KEYBOARD_I: return "I";
            case HID_USAGE_KEY_KEYBOARD_J: return "J";
            case HID_USAGE_KEY_KEYBOARD_K: return "K";
            case HID_USAGE_KEY_KEYBOARD_L: return "L";
            case HID_USAGE_KEY_KEYBOARD_M: return "M";
            case HID_USAGE_KEY_KEYBOARD_N: return "N";
            case HID_USAGE_KEY_KEYBOARD_O: return "O";
            case HID_USAGE_KEY_KEYBOARD_P: return "P";
            case HID_USAGE_KEY_KEYBOARD_Q: return "Q";
            case HID_USAGE_KEY_KEYBOARD_R: return "R";
            case HID_USAGE_KEY_KEYBOARD_S: return "S";
            case HID_USAGE_KEY_KEYBOARD_T: return "T";
            case HID_USAGE_KEY_KEYBOARD_U: return "U";
            case HID_USAGE_KEY_KEYBOARD_V: return "V";
            case HID_USAGE_KEY_KEYBOARD_W: return "W";
            case HID_USAGE_KEY_KEYBOARD_X: return "X";
            case HID_USAGE_KEY_KEYBOARD_Y: return "Y";
            case HID_USAGE_KEY_KEYBOARD_Z: return "Z";
            
            case HID_USAGE_KEY_KEYBOARD_1_AND_EXCLAMATION: return "1";
            case HID_USAGE_KEY_KEYBOARD_2_AND_AT: return "2";
            case HID_USAGE_KEY_KEYBOARD_3_AND_HASH: return "3";
            case HID_USAGE_KEY_KEYBOARD_4_AND_DOLLAR: return "4";
            case HID_USAGE_KEY_KEYBOARD_5_AND_PERCENT: return "5";
            case HID_USAGE_KEY_KEYBOARD_6_AND_CARET: return "6";
            case HID_USAGE_KEY_KEYBOARD_7_AND_AMPERSAND: return "7";
            case HID_USAGE_KEY_KEYBOARD_8_AND_ASTERISK: return "8";
            case HID_USAGE_KEY_KEYBOARD_9_AND_LEFT_PARENTHESIS: return "9";
            case HID_USAGE_KEY_KEYBOARD_0_AND_RIGHT_PARENTHESIS: return "0";
            
            case HID_USAGE_KEY_KEYBOARD_SPACEBAR: return "SPAC";
            case HID_USAGE_KEY_KEYBOARD_RETURN_ENTER: return "ENTR";
            case HID_USAGE_KEY_KEYBOARD_ESCAPE: return "ESC";
            case HID_USAGE_KEY_KEYBOARD_TAB: return "TAB";
            case HID_USAGE_KEY_KEYBOARD_DELETE_BACKSPACE: return "DEL";
            case HID_USAGE_KEY_KEYBOARD_MINUS_AND_UNDERSCORE: return "_";
            case HID_USAGE_KEY_KEYBOARD_EQUAL_AND_PLUS: return "+";
            case HID_USAGE_KEY_KEYBOARD_LEFT_BRACKET_AND_LEFT_BRACE: return "()";
            case HID_USAGE_KEY_KEYBOARD_RIGHT_BRACKET_AND_RIGHT_BRACE: return "()";
            case HID_USAGE_KEY_KEYBOARD_BACKSLASH_AND_PIPE: return "|";
            case HID_USAGE_KEY_KEYBOARD_NON_US_HASH_AND_TILDE: return "#";
            case HID_USAGE_KEY_KEYBOARD_SEMICOLON_AND_COLON: return ":";
            case HID_USAGE_KEY_KEYBOARD_APOSTROPHE_AND_QUOTE: return "'";
            case HID_USAGE_KEY_KEYBOARD_GRAVE_ACCENT_AND_TILDE: return "~";
            case HID_USAGE_KEY_KEYBOARD_COMMA_AND_LESS_THAN: return ",";
            case HID_USAGE_KEY_KEYBOARD_PERIOD_AND_GREATER_THAN: return ".";
            case HID_USAGE_KEY_KEYBOARD_SLASH_AND_QUESTION_MARK: return "/";
            case HID_USAGE_KEY_KEYBOARD_CAPS_LOCK: return "CAPS";
            
            case HID_USAGE_KEY_KEYBOARD_LEFTSHIFT: return "LSHFT";
            case HID_USAGE_KEY_KEYBOARD_RIGHTSHIFT: return "RSHFT";
            case HID_USAGE_KEY_KEYBOARD_LEFTCONTROL: return "LCTRL";
            case HID_USAGE_KEY_KEYBOARD_RIGHTCONTROL: return "RCTRL";
            case HID_USAGE_KEY_KEYBOARD_LEFTALT: return "LALT";
            case HID_USAGE_KEY_KEYBOARD_RIGHTALT: return "RALT";
            case HID_USAGE_KEY_KEYBOARD_LEFT_GUI: return "LGUI";
            case HID_USAGE_KEY_KEYBOARD_RIGHT_GUI: return "RGUI";
            
            case HID_USAGE_KEY_KEYBOARD_UPARROW: return "UP";
            case HID_USAGE_KEY_KEYBOARD_DOWNARROW: return "DOWN";
            case HID_USAGE_KEY_KEYBOARD_LEFTARROW: return "LEFT";
            case HID_USAGE_KEY_KEYBOARD_RIGHTARROW: return "RIGHT";
            
            default: return "UNK";
        }
    }
    
    // Handle consumer keys (HID Usage Page 0x0C)
    if (usage_page == HID_USAGE_CONSUMER) {
        switch (keycode) {
            case HID_USAGE_CONSUMER_VOLUME_INCREMENT: return "VOL+";
            case HID_USAGE_CONSUMER_VOLUME_DECREMENT: return "VOL-";
            case HID_USAGE_CONSUMER_MUTE: return "MUTE";
            case HID_USAGE_CONSUMER_PLAY_PAUSE: return "PLAY";
            case HID_USAGE_CONSUMER_SCAN_NEXT_TRACK: return "NEXT";
            case HID_USAGE_CONSUMER_SCAN_PREVIOUS_TRACK: return "PREV";
            default: return "CONS";
        }
    }
    
    return "UNK";
}

void draw_keycode_status(lv_obj_t *canvas, const struct status_state* state) {
    lv_draw_label_dsc_t label_dsc;
    init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_CENTER);

    char text[7];
    snprintf(text, sizeof(text), "%d", state->keycode);
    lv_canvas_draw_text(canvas, 0, 50, 32, &label_dsc, text);

    
    char btext[9];
    const char* key_name = keycode_to_string(state->keycode, HID_USAGE_KEY); // HID_USAGE_CONSUMER
    snprintf(btext, sizeof(btext), "%s", key_name);
    lv_canvas_draw_text(canvas, 0, 66, 32, &label_dsc, btext);
}
