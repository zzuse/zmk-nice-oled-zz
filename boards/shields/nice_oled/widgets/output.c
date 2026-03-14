#include "output.h"
#include "../assets/custom_fonts.h"
#include <zephyr/kernel.h>

LV_IMG_DECLARE(bt_no_signal);
LV_IMG_DECLARE(bt_unbonded);
LV_IMG_DECLARE(bt);
LV_IMG_DECLARE(usb);

static void draw_usb_connected(lv_obj_t *canvas, int x, int y) { draw_img(canvas, x, y, &usb); }

static void draw_ble_unbonded(lv_obj_t *canvas, int x, int y) { draw_img(canvas, x, y, &bt_unbonded); }

static void draw_ble_disconnected(lv_obj_t *canvas, int x, int y) { draw_img(canvas, x, y, &bt_no_signal); }

static void draw_ble_connected(lv_obj_t *canvas, int x, int y) { draw_img(canvas, x, y, &bt); }

static void draw_ble_profile_number(lv_obj_t *canvas, int x, int y, int profile_number)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%d", profile_number);
    draw_text(canvas, x, y, buf);
}

void draw_output_status(lv_obj_t *canvas, const struct status_state *state, int x, int y)
{
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    switch (state->selected_endpoint.transport) {
        case ZMK_TRANSPORT_USB:
            draw_usb_connected(canvas, x, y);
            break;

        case ZMK_TRANSPORT_BLE:
            if (state->active_profile_bonded) {
                if (state->active_profile_connected) {
                    draw_ble_connected(canvas, x, y);
                    draw_ble_profile_number(canvas, x + 15, y, state->active_profile_index + 1);
                } else {
                    draw_ble_disconnected(canvas, x, y);
                }
            } else {
                draw_ble_unbonded(canvas, x, y);
            }
            break;
        case ZMK_TRANSPORT_NONE:
            break;
    }
#else
    if (state->connected) {
        draw_ble_connected(canvas);
    } else {
        draw_ble_disconnected(canvas);
    }
#endif
}
