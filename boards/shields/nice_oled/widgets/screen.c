#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/usb.h>

#include "screen.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);
static lv_obj_t *global_canvas = NULL;

static void draw_canvas(void)
{
    if (!global_canvas) return;

    // 1. Fill background White
    lv_canvas_fill_bg(global_canvas, lv_color_white(), LV_OPA_COVER);

    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_color_black();
    label_dsc.font = &lv_font_unscii_8;

    lv_layer_t layer;
    lv_canvas_init_layer(global_canvas, &layer);

    char buf[64];
    lv_area_t area = {2, 2, 125, 30};

    uint8_t l_idx = zmk_keymap_highest_layer_active();
    int b_idx = zmk_ble_active_profile_index() + 1;
    bool b_conn = zmk_ble_active_profile_is_connected();

    snprintf(buf, sizeof(buf), "ZZUSE UP:%llds L:%d B:%d %s \nBAT:%d%%", k_uptime_get() / 1000, l_idx, b_idx,
             b_conn ? "ON" : "OFF", get_natural_battery_level());

    label_dsc.text = buf;
    lv_draw_label(&layer, &label_dsc, &area);

    lv_canvas_finish_layer(global_canvas, &layer);
    lv_obj_invalidate(global_canvas);
}

static void debug_work_handler(struct k_work *work) { draw_canvas(); }
K_WORK_DEFINE(debug_work, debug_work_handler);

static void debug_timer_handler(struct k_timer *timer) { k_work_submit(&debug_work); }
K_TIMER_DEFINE(debug_timer, debug_timer_handler, NULL);

LV_DRAW_BUF_DEFINE_STATIC(phys_draw_buf, 128, 32, LV_COLOR_FORMAT_I1);

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent)
{
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 128, 32);

    lv_obj_t *canvas = lv_canvas_create(widget->obj);
    lv_obj_align(canvas, LV_ALIGN_TOP_LEFT, 0, 0);
    LV_DRAW_BUF_INIT_STATIC(phys_draw_buf);
    lv_canvas_set_draw_buf(canvas, &phys_draw_buf);

    lv_canvas_set_palette(canvas, 0, lv_color32_make(0, 0, 0, 255));
    lv_canvas_set_palette(canvas, 1, lv_color32_make(255, 255, 255, 255));

    global_canvas = canvas;
    widget->node.next = NULL;
    sys_slist_append(&widgets, &widget->node);

    draw_canvas();
    k_timer_start(&debug_timer, K_SECONDS(1), K_SECONDS(1));

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
