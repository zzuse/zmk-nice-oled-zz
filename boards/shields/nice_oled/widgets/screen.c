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

static lv_obj_t *logic_canvas;
LV_DRAW_BUF_DEFINE_STATIC(logic_draw_buf, 128, 128, LV_COLOR_FORMAT_I1);

static void draw_text(int x, int y, const char *text) {
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = LVGL_FOREGROUND;
    label_dsc.font = &lv_font_unscii_8;
    label_dsc.align = LV_TEXT_ALIGN_LEFT;
    
    lv_layer_t layer;
    lv_canvas_init_layer(logic_canvas, &layer);
    // Logical drawing area is 32px wide, canvas is 128px high
    lv_area_t coords = {x, y, 31, (y + 20 > 127) ? 127 : y + 20};
    label_dsc.text = text;
    lv_draw_label(&layer, &label_dsc, &coords);
    lv_canvas_finish_layer(logic_canvas, &layer);
}

static void draw_canvas(lv_obj_t *widget_obj, const struct status_state *state) {
    if (!logic_canvas) return;
    lv_obj_t *phys_canvas = lv_obj_get_child(widget_obj, 0);

    // 1. Clear logic canvas
    lv_canvas_fill_bg(logic_canvas, LVGL_BACKGROUND, LV_OPA_COVER);
    
    // 2. Draw Battery
    char bat_buf[16];
    snprintf(bat_buf, sizeof(bat_buf), "%d%%", state->battery);
    draw_text(2, 5, bat_buf);
    draw_text(2, 15, bat_buf);
    draw_text(2, 25, bat_buf);
    draw_text(2, 35, bat_buf);
    draw_text(2, 45, bat_buf);
    draw_text(2, 55, bat_buf);
    draw_text(2, 65, bat_buf);
    draw_text(2, 75, bat_buf);
    draw_text(2, 85, bat_buf);
    draw_text(2, 95, bat_buf);
    draw_text(2, 105, bat_buf);
    draw_text(2, 115, bat_buf);

    rotate_canvas(phys_canvas, logic_canvas);
}

static void set_battery_status(struct zmk_widget_screen *widget, struct battery_status_state state) {
    widget->state.battery = state.level;
    widget->state.charging = state.usb_present;
    draw_canvas(widget->obj, &widget->state);
}

static void battery_status_update_cb(struct battery_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_status(widget, state); }
}
static struct battery_status_state battery_status_get_state(const zmk_event_t *eh) { 
    const struct zmk_battery_state_changed *ev = as_zmk_battery_state_changed(eh);
    return (struct battery_status_state){.level = (ev != NULL) ? ev->state_of_charge : zmk_battery_state_of_charge()};
}
ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state, battery_status_update_cb, battery_status_get_state);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);

LV_DRAW_BUF_DEFINE_STATIC(phys_draw_buf, 128, 32, LV_COLOR_FORMAT_I1);

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 128, 32);
    // lv_obj_set_style_bg_color(widget->obj, lv_color_white(), 0);

    lv_obj_t *phys_canvas = lv_canvas_create(widget->obj);
    lv_obj_align(phys_canvas, LV_ALIGN_TOP_LEFT, 0, 0);
    LV_DRAW_BUF_INIT_STATIC(phys_draw_buf);
    lv_canvas_set_draw_buf(phys_canvas, &phys_draw_buf);
    
    lv_color32_t c_white = {.red=255, .green=255, .blue=255, .alpha=255};
    lv_color32_t c_black = {.red=0, .green=0, .blue=0, .alpha=255};
    lv_canvas_set_palette(phys_canvas, 0, c_white);
    lv_canvas_set_palette(phys_canvas, 1, c_black);

    if (!logic_canvas) {
        LV_DRAW_BUF_INIT_STATIC(logic_draw_buf);
        logic_canvas = lv_canvas_create(NULL);
        lv_canvas_set_draw_buf(logic_canvas, &logic_draw_buf);
        lv_canvas_set_palette(logic_canvas, 0, c_white);
        lv_canvas_set_palette(logic_canvas, 1, c_black);
    }

    sys_slist_append(&widgets, &widget->node);

    widget_battery_status_init();

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
