#include "keycode.h"
#include "keycode_name.h"
#include <zephyr/kernel.h>
#include <zmk/hid.h>

#include <dt-bindings/zmk/modifiers.h>

extern const lv_img_dsc_t control_0;
extern const lv_img_dsc_t shift_0;
extern const lv_img_dsc_t opt_0;
extern const lv_img_dsc_t cmd_0;

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