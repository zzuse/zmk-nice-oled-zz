#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/battery.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>
#include <zmk/split/bluetooth/peripheral.h>
#include <zmk/usb.h>

#include "screen_peripheral.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct peripheral_labels {
    lv_obj_t *battery;
    lv_obj_t *status;
};

static struct peripheral_labels labels;

static void vertical_text(char *dst, const char *src, size_t dst_len) {
    size_t i = 0, j = 0;
    while (src[i] != '\0' && j + 2 < dst_len) {
        dst[j++] = src[i++];
        dst[j++] = '\n';
    }
    if (j > 0) dst[j-1] = '\0';
    else dst[0] = '\0';
}

static void update_battery_label(lv_obj_t *label, uint8_t level, bool charging) {
    if (!label) return;
    char raw[16], vert[32];
    snprintf(raw, sizeof(raw), "%d%%", level);
    vertical_text(vert, raw, sizeof(vert));
    lv_label_set_text(label, vert);
}

static void update_status_label(lv_obj_t *label, bool connected) {
    if (!label) return;
    char vert[32];
    vertical_text(vert, connected ? "CON" : "DIS", sizeof(vert));
    lv_label_set_text(label, vert);
}

static void battery_status_update_cb(struct battery_status_state state) {
    update_battery_label(labels.battery, state.level, state.usb_present);
}

static struct battery_status_state battery_status_get_state(const zmk_event_t *eh) {
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    return (struct battery_status_state){
        .level = (ev != NULL) ? ev->state_of_charge : zmk_battery_state_of_charge(),
        .usb_present = zmk_usb_is_powered(),
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state,
                            battery_status_update_cb, battery_status_get_state);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);

static void peripheral_status_update_cb(struct peripheral_status_state state) {
    update_status_label(labels.status, state.connected);
}

static struct peripheral_status_state get_state(const zmk_event_t *_eh) {
    return (struct peripheral_status_state){.connected = zmk_split_bt_peripheral_is_connected()};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_peripheral_status, struct peripheral_status_state,
                            peripheral_status_update_cb, get_state)
ZMK_SUBSCRIPTION(widget_peripheral_status, zmk_split_peripheral_status_changed);

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 128, 32);
    lv_obj_set_style_bg_color(widget->obj, lv_color_white(), 0);
    lv_obj_set_style_bg_opa(widget->obj, LV_OPA_COVER, 0);

    labels.battery = lv_label_create(widget->obj);
    lv_obj_set_style_text_color(labels.battery, lv_color_black(), 0);
    lv_obj_align(labels.battery, LV_ALIGN_LEFT_MID, 20, 0);

    labels.status = lv_label_create(widget->obj);
    lv_obj_set_style_text_color(labels.status, lv_color_black(), 0);
    lv_obj_align(labels.status, LV_ALIGN_CENTER, 0, 0);

    sys_slist_append(&widgets, &widget->node);

    update_battery_label(labels.battery, zmk_battery_state_of_charge(), zmk_usb_is_powered());
    update_status_label(labels.status, zmk_split_bt_peripheral_is_connected());

    widget_battery_status_init();
    widget_peripheral_status_init();

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
