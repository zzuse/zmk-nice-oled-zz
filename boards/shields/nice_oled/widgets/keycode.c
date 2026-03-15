#include "keycode.h"
#include <zephyr/kernel.h>
#include <zmk/hid.h>

#include <dt-bindings/zmk/hid_usage_pages.h>
#include <dt-bindings/zmk/modifiers.h>

extern const lv_img_dsc_t control_0;
extern const lv_img_dsc_t shift_0;
extern const lv_img_dsc_t opt_0;
extern const lv_img_dsc_t cmd_0;

static const char* keycode_to_string(uint32_t keycode, uint16_t usage_page)
{
    // Both 0x07 and 0x00 are common for keyboard usage page in ZMK events
    if (usage_page == HID_USAGE_KEY || usage_page == 0x00) {
        switch (keycode) {
            case HID_USAGE_KEY_KEYBOARD_A:
                return "A";
            case HID_USAGE_KEY_KEYBOARD_B:
                return "B";
            case HID_USAGE_KEY_KEYBOARD_C:
                return "C";
            case HID_USAGE_KEY_KEYBOARD_D:
                return "D";
            case HID_USAGE_KEY_KEYBOARD_E:
                return "E";
            case HID_USAGE_KEY_KEYBOARD_F:
                return "F";
            case HID_USAGE_KEY_KEYBOARD_G:
                return "G";
            case HID_USAGE_KEY_KEYBOARD_H:
                return "H";
            case HID_USAGE_KEY_KEYBOARD_I:
                return "I";
            case HID_USAGE_KEY_KEYBOARD_J:
                return "J";
            case HID_USAGE_KEY_KEYBOARD_K:
                return "K";
            case HID_USAGE_KEY_KEYBOARD_L:
                return "L";
            case HID_USAGE_KEY_KEYBOARD_M:
                return "M";
            case HID_USAGE_KEY_KEYBOARD_N:
                return "N";
            case HID_USAGE_KEY_KEYBOARD_O:
                return "O";
            case HID_USAGE_KEY_KEYBOARD_P:
                return "P";
            case HID_USAGE_KEY_KEYBOARD_Q:
                return "Q";
            case HID_USAGE_KEY_KEYBOARD_R:
                return "R";
            case HID_USAGE_KEY_KEYBOARD_S:
                return "S";
            case HID_USAGE_KEY_KEYBOARD_T:
                return "T";
            case HID_USAGE_KEY_KEYBOARD_U:
                return "U";
            case HID_USAGE_KEY_KEYBOARD_V:
                return "V";
            case HID_USAGE_KEY_KEYBOARD_W:
                return "W";
            case HID_USAGE_KEY_KEYBOARD_X:
                return "X";
            case HID_USAGE_KEY_KEYBOARD_Y:
                return "Y";
            case HID_USAGE_KEY_KEYBOARD_Z:
                return "Z";

            case HID_USAGE_KEY_KEYBOARD_1_AND_EXCLAMATION:
                return "1";
            case HID_USAGE_KEY_KEYBOARD_2_AND_AT:
                return "2";
            case HID_USAGE_KEY_KEYBOARD_3_AND_HASH:
                return "3";
            case HID_USAGE_KEY_KEYBOARD_4_AND_DOLLAR:
                return "4";
            case HID_USAGE_KEY_KEYBOARD_5_AND_PERCENT:
                return "5";
            case HID_USAGE_KEY_KEYBOARD_6_AND_CARET:
                return "6";
            case HID_USAGE_KEY_KEYBOARD_7_AND_AMPERSAND:
                return "7";
            case HID_USAGE_KEY_KEYBOARD_8_AND_ASTERISK:
                return "8";
            case HID_USAGE_KEY_KEYBOARD_9_AND_LEFT_PARENTHESIS:
                return "9";
            case HID_USAGE_KEY_KEYBOARD_0_AND_RIGHT_PARENTHESIS:
                return "0";

            case HID_USAGE_KEY_KEYBOARD_SPACEBAR:
                return "SPAC";
            case HID_USAGE_KEY_KEYBOARD_RETURN_ENTER:
                return "ENTR";
            case HID_USAGE_KEY_KEYBOARD_ESCAPE:
                return "ESC";
            case HID_USAGE_KEY_KEYBOARD_TAB:
                return "TAB";
            case HID_USAGE_KEY_KEYBOARD_MINUS_AND_UNDERSCORE:
                return "_";
            case HID_USAGE_KEY_KEYBOARD_EQUAL_AND_PLUS:
                return "+";
            case HID_USAGE_KEY_KEYBOARD_LEFT_BRACKET_AND_LEFT_BRACE:
                return "()";
            case HID_USAGE_KEY_KEYBOARD_RIGHT_BRACKET_AND_RIGHT_BRACE:
                return "()";
            case HID_USAGE_KEY_KEYBOARD_BACKSLASH_AND_PIPE:
                return "|";
            case HID_USAGE_KEY_KEYBOARD_NON_US_HASH_AND_TILDE:
                return "#";
            case HID_USAGE_KEY_KEYBOARD_SEMICOLON_AND_COLON:
                return ":";
            case HID_USAGE_KEY_KEYBOARD_APOSTROPHE_AND_QUOTE:
                return "'";
            case HID_USAGE_KEY_KEYBOARD_GRAVE_ACCENT_AND_TILDE:
                return "~";
            case HID_USAGE_KEY_KEYBOARD_COMMA_AND_LESS_THAN:
                return ",";
            case HID_USAGE_KEY_KEYBOARD_PERIOD_AND_GREATER_THAN:
                return ".";
            case HID_USAGE_KEY_KEYBOARD_SLASH_AND_QUESTION_MARK:
                return "/";
            case HID_USAGE_KEY_KEYBOARD_CAPS_LOCK:
                return "CAPS";
            case HID_USAGE_KEY_KEYBOARD_DELETE_BACKSPACE:
                return "BSPC";
            case HID_USAGE_KEY_KEYBOARD_DELETE_FORWARD:
                return "DEL";
            case HID_USAGE_KEY_KEYBOARD_UPARROW:
                return "UP";
            case HID_USAGE_KEY_KEYBOARD_DOWNARROW:
                return "DOWN";
            case HID_USAGE_KEY_KEYBOARD_LEFTARROW:
                return "LEFT";
            case HID_USAGE_KEY_KEYBOARD_RIGHTARROW:
                return "RIGHT";

            case HID_USAGE_KEY_KEYBOARD_F1:
                return "F1";
            case HID_USAGE_KEY_KEYBOARD_F2:
                return "F2";
            case HID_USAGE_KEY_KEYBOARD_F3:
                return "F3";
            case HID_USAGE_KEY_KEYBOARD_F4:
                return "F4";
            case HID_USAGE_KEY_KEYBOARD_F5:
                return "F5";
            case HID_USAGE_KEY_KEYBOARD_F6:
                return "F6";
            case HID_USAGE_KEY_KEYBOARD_F7:
                return "F7";
            case HID_USAGE_KEY_KEYBOARD_F8:
                return "F8";
            case HID_USAGE_KEY_KEYBOARD_F9:
                return "F9";
            case HID_USAGE_KEY_KEYBOARD_F10:
                return "F10";
            case HID_USAGE_KEY_KEYBOARD_F11:
                return "F11";
            case HID_USAGE_KEY_KEYBOARD_F12:
                return "F12";

            case HID_USAGE_KEY_KEYBOARD_HOME:
                return "HOME";
            case HID_USAGE_KEY_KEYBOARD_END:
                return "END";
            case HID_USAGE_KEY_KEYBOARD_PAGEUP:
                return "PGUP";
            case HID_USAGE_KEY_KEYBOARD_PAGEDOWN:
                return "PGDN";
            case HID_USAGE_KEY_KEYBOARD_INSERT:
                return "INS";

            case HID_USAGE_KEY_KEYBOARD_UNDO:
                return "UNDO";
            case HID_USAGE_KEY_KEYBOARD_CUT:
                return "CUT";
            case HID_USAGE_KEY_KEYBOARD_COPY:
                return "COPY";
            case HID_USAGE_KEY_KEYBOARD_PASTE:
                return "PSTE";

            case HID_USAGE_KEY_KEYBOARD_LEFTCONTROL:
                return "LCTL";
            case HID_USAGE_KEY_KEYBOARD_LEFTSHIFT:
                return "LSFT";
            case HID_USAGE_KEY_KEYBOARD_LEFTALT:
                return "LALT";
            case HID_USAGE_KEY_KEYBOARD_LEFT_GUI:
                return "LGUI";
            case HID_USAGE_KEY_KEYBOARD_RIGHTCONTROL:
                return "RCTL";
            case HID_USAGE_KEY_KEYBOARD_RIGHTSHIFT:
                return "RSFT";
            case HID_USAGE_KEY_KEYBOARD_RIGHTALT:
                return "RALT";
            case HID_USAGE_KEY_KEYBOARD_RIGHT_GUI:
                return "RGUI";

            default:
                return NULL;
        }
    }

    if (usage_page == HID_USAGE_CONSUMER) {
        switch (keycode) {
            case HID_USAGE_CONSUMER_VOLUME_INCREMENT:
                return "VOL+";
            case HID_USAGE_CONSUMER_VOLUME_DECREMENT:
                return "VOL-";
            case HID_USAGE_CONSUMER_MUTE:
                return "MUTE";
            case HID_USAGE_CONSUMER_PLAY_PAUSE:
                return "PLAY";
            case HID_USAGE_CONSUMER_SCAN_NEXT_TRACK:
                return "NEXT";
            case HID_USAGE_CONSUMER_SCAN_PREVIOUS_TRACK:
                return "PREV";
            default:
                return NULL;
        }
    }

    return NULL;
}

void draw_keycode_status(lv_obj_t* canvas, const struct status_state* state, int x, int y)
{
    const char* key_name = keycode_to_string(state->keycode, state->usage_page);
    char buf[20] = {0};
    uint8_t mods = state->implicit_modifiers | zmk_hid_get_explicit_mods();

    int x_mod = x;

    if (mods == 0) {
        // No modifiers pressed, display just the key name on the top line
        snprintf(buf, sizeof(buf), "%s %02X:%02X", key_name, state->usage_page, state->keycode);
        draw_text(canvas, x, y, buf, true);
    } else {
        // Modifiers pressed, draw icons
        if (mods & (MOD_LCTL | MOD_RCTL)) {
            draw_img(canvas, x_mod, y, &control_0);
        }
        if (mods & (MOD_LSFT | MOD_RSFT)) {
            draw_img(canvas, x_mod + 7, y, &shift_0);
        }
        if (mods & (MOD_LALT | MOD_RALT)) {
            draw_img(canvas, x_mod + 14, y, &opt_0);
        }
        if (mods & (MOD_LGUI | MOD_RGUI)) {
            draw_img(canvas, x_mod + 21, y, &cmd_0);
        }

        // Draw the key string below the modifier icons
        snprintf(buf, sizeof(buf), "%s %02X:%02X", key_name, state->usage_page, state->keycode);
        draw_text(canvas, x, y + 7, buf, true);
    }
}