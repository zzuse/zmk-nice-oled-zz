/*
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/kernel.h>

#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>

#include "key_status.h"
#include "../../nice_oled/widgets/keycode_name.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct key_status_state {
    uint32_t keycode;
    uint16_t usage_page;
};

static struct key_status_state last_state = {.keycode = 0, .usage_page = 0};

static void key_status_update_cb(struct key_status_state state) {
    char text[16];
    if (state.keycode == 0) {
        snprintf(text, sizeof(text), "-");
    } else {
        snprintf(text, sizeof(text), "%s",
                 keycode_to_string(state.keycode, state.usage_page));
    }

    struct zmk_widget_key_status *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { lv_label_set_text(widget->obj, text); }
}

static struct key_status_state key_status_get_state(const zmk_event_t *eh) {
    // eh is NULL when the listener fetches the initial state
    const struct zmk_keycode_state_changed *ev = eh ? as_zmk_keycode_state_changed(eh) : NULL;
    if (ev && ev->state) { // Only on key press, not release
        last_state.keycode = ev->keycode;
        last_state.usage_page = ev->usage_page;
    }
    return last_state;
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_key_status, struct key_status_state, key_status_update_cb,
                            key_status_get_state)
ZMK_SUBSCRIPTION(widget_key_status, zmk_keycode_state_changed);

int zmk_widget_key_status_init(struct zmk_widget_key_status *widget, lv_obj_t *parent) {
    widget->obj = lv_label_create(parent);
    lv_obj_set_width(widget->obj, CONFIG_ZMK_DONGLE_DISPLAY_KEY_STATUS_WIDTH);
    lv_label_set_long_mode(widget->obj, LV_LABEL_LONG_CLIP);

    sys_slist_append(&widgets, &widget->node);

    widget_key_status_init();
    return 0;
}

lv_obj_t *zmk_widget_key_status_obj(struct zmk_widget_key_status *widget) { return widget->obj; }
