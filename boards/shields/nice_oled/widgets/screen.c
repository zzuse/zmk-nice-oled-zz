#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/usb.h>

#include "screen.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

struct screen_labels {
    lv_obj_t *battery;
    lv_obj_t *layer;
    lv_obj_t *output;
};

static struct screen_labels labels;

// Helper to make text vertical using newlines
static void vertical_text(char *dst, const char *src, size_t dst_len) {
    size_t i = 0, j = 0;
    while (src[i] != '\0' && j + 2 < dst_len) {
        dst[j++] = src[i++];
        dst[j++] = '\n';
    }
    if (j > 0) dst[j-1] = '\0'; // Remove last newline
    else dst[0] = '\0';
}

static void update_battery_label(lv_obj_t *label, uint8_t level, bool charging) {
    if (!label) return;
    char raw[16], vert[32];
    snprintf(raw, sizeof(raw), "%d%%", level);
    vertical_text(vert, raw, sizeof(vert));
    lv_label_set_text(label, vert);
}

static void update_layer_label(lv_obj_t *label, uint8_t index, const char *label_text) {
    if (!label) return;
    char raw[16], vert[32];
    if (label_text != NULL) {
        snprintf(raw, sizeof(raw), "%c", label_text[0]);
    } else {
        snprintf(raw, sizeof(raw), "%d", index);
    }
    vertical_text(vert, raw, sizeof(vert));
    lv_label_set_text(label, vert);
}

static void update_output_label(lv_obj_t *label) {
    if (!label) return;
    struct zmk_endpoint_instance endpoint = zmk_endpoint_get_selected();
    char vert[32];
    switch (endpoint.transport) {
    case ZMK_TRANSPORT_USB: vertical_text(vert, "USB", sizeof(vert)); break;
    case ZMK_TRANSPORT_BLE: {
        char raw[8];
        snprintf(raw, sizeof(raw), "B%d", endpoint.ble.profile_index + 1);
        vertical_text(vert, raw, sizeof(vert));
        break;
    }
    default: vertical_text(vert, "OFF", sizeof(vert)); break;
    }
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

static void layer_status_update_cb(struct layer_status_state state) {
    update_layer_label(labels.layer, state.index, state.label);
}

static struct layer_status_state layer_status_get_state(const zmk_event_t *eh) {
    uint8_t index = zmk_keymap_highest_layer_active();
    return (struct layer_status_state){.index = index, .label = zmk_keymap_layer_name(index)};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_layer_status, struct layer_status_state, layer_status_update_cb,
                            layer_status_get_state)
ZMK_SUBSCRIPTION(widget_layer_status, zmk_layer_state_changed);

static void output_status_update_cb(struct output_status_state state) {
    update_output_label(labels.output);
}

static struct output_status_state output_status_get_state(const zmk_event_t *_eh) {
    return (struct output_status_state){};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_output_status, struct output_status_state,
                            output_status_update_cb, output_status_get_state)
ZMK_SUBSCRIPTION(widget_output_status, zmk_endpoint_changed);
ZMK_SUBSCRIPTION(widget_output_status, zmk_ble_active_profile_changed);

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 128, 32);
    lv_obj_set_style_bg_color(widget->obj, lv_color_white(), 0); // Black due to inversion
    lv_obj_set_style_bg_opa(widget->obj, LV_OPA_COVER, 0);

    labels.battery = lv_label_create(widget->obj);
    lv_obj_set_style_text_color(labels.battery, lv_color_black(), 0); 
    lv_obj_align(labels.battery, LV_ALIGN_LEFT_MID, 10, 0);

    labels.output = lv_label_create(widget->obj);
    lv_obj_set_style_text_color(labels.output, lv_color_black(), 0);
    lv_obj_align(labels.output, LV_ALIGN_CENTER, 0, 0);

    labels.layer = lv_label_create(widget->obj);
    lv_obj_set_style_text_color(labels.layer, lv_color_black(), 0);
    lv_obj_align(labels.layer, LV_ALIGN_RIGHT_MID, -10, 0);

    sys_slist_append(&widgets, &widget->node);

    update_battery_label(labels.battery, zmk_battery_state_of_charge(), zmk_usb_is_powered());
    update_layer_label(labels.layer, zmk_keymap_highest_layer_active(), zmk_keymap_layer_name(zmk_keymap_highest_layer_active()));
    update_output_label(labels.output);

    widget_battery_status_init();
    widget_layer_status_init();
    widget_output_status_init();

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
