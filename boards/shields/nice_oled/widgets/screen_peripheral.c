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

static lv_obj_t *logic_canvas_peri;
static uint8_t logic_buffer_peri[2064] __attribute__((aligned(4)));

static void draw_text(int x, int y, const char *text) {
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_color_black();
    label_dsc.font = &lv_font_unscii_8;
    
    lv_layer_t layer;
    lv_canvas_init_layer(logic_canvas_peri, &layer);
    lv_area_t coords = {x, y, 30, y + 20};
    label_dsc.text = text;
    lv_draw_label(&layer, &label_dsc, &coords);
    lv_canvas_finish_layer(logic_canvas_peri, &layer);
}

static void draw_full_canvas(lv_obj_t *widget_obj) {
    if (!logic_canvas_peri) return;
    lv_obj_t *phys_canvas = lv_obj_get_child(widget_obj, 0);
    lv_canvas_fill_bg(logic_canvas_peri, lv_color_white(), LV_OPA_COVER);
    
    char bat_buf[16];
    snprintf(bat_buf, sizeof(bat_buf), "%d%%", zmk_battery_state_of_charge());
    draw_text(2, 5, bat_buf);
    
    draw_text(2, 55, zmk_split_bt_peripheral_is_connected() ? "CONN" : "DISC");

    rotate_canvas(phys_canvas, (lv_color_t*)logic_buffer_peri);
}

static void battery_status_update_cb(struct battery_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { draw_full_canvas(widget->obj); }
}
static struct battery_status_state battery_status_get_state(const zmk_event_t *eh) { 
    return (struct battery_status_state){.level = zmk_battery_state_of_charge()};
}
ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct battery_status_state, battery_status_update_cb, battery_status_get_state);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);

static void peripheral_status_update_cb(struct peripheral_status_state state) {
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { draw_full_canvas(widget->obj); }
}
static struct peripheral_status_state get_state(const zmk_event_t *_eh) { return (struct peripheral_status_state){.connected = zmk_split_bt_peripheral_is_connected()}; }
ZMK_DISPLAY_WIDGET_LISTENER(widget_peripheral_status, struct peripheral_status_state, peripheral_status_update_cb, get_state)
ZMK_SUBSCRIPTION(widget_peripheral_status, zmk_split_peripheral_status_changed);

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent) {
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 128, 32);
    lv_obj_set_style_bg_color(widget->obj, lv_color_white(), 0);

    lv_obj_t *phys_canvas = lv_canvas_create(widget->obj);
    lv_obj_align(phys_canvas, LV_ALIGN_TOP_LEFT, 0, 0);
    lv_canvas_set_buffer(phys_canvas, widget->cbuf, 128, 32, LV_COLOR_FORMAT_I1);
    
    lv_color32_t c_white = {.red=255, .green=255, .blue=255, .alpha=255};
    lv_color32_t c_black = {.red=0, .green=0, .blue=0, .alpha=255};
    lv_canvas_set_palette(phys_canvas, 0, c_white);
    lv_canvas_set_palette(phys_canvas, 1, c_black);

    if (!logic_canvas_peri) {
        logic_canvas_peri = lv_canvas_create(NULL);
        lv_canvas_set_buffer(logic_canvas_peri, (lv_color_t*)logic_buffer_peri, 128, 128, LV_COLOR_FORMAT_I1);
        lv_canvas_set_palette(logic_canvas_peri, 0, c_white);
        lv_canvas_set_palette(logic_canvas_peri, 1, c_black);
    }

    sys_slist_append(&widgets, &widget->node);
    draw_full_canvas(widget->obj);

    widget_battery_status_init();
    widget_peripheral_status_init();

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
