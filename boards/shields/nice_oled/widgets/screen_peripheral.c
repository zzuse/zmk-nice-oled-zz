#include <zephyr/kernel.h>
#include <zmk/battery.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>
#include <zmk/split/bluetooth/peripheral.h>
#include <zmk/usb.h>

#include "screen_peripheral.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);
static lv_obj_t *global_logic_canvas = NULL;
static lv_obj_t *global_phys_canvas = NULL;
static int global_draw_count = 0;

static void draw_text(lv_obj_t *target_canvas, int x, int y, const char *text)
{
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_color_make(0, 0, 0); // Black text (Index 0)
    label_dsc.font = &lv_font_unscii_8;

    lv_layer_t layer;
    lv_canvas_init_layer(target_canvas, &layer);

    // Width is exactly 32px. Limit height to prevent wrapping.
    lv_area_t area = {x, y, 31, y + 10};
    label_dsc.text = text;
    lv_draw_label(&layer, &label_dsc, &area);

    lv_canvas_finish_layer(target_canvas, &layer);
}

static void draw_canvas(void)
{
    if (!global_logic_canvas || !global_phys_canvas) return;

    global_draw_count++;

    // 1. Fill logic canvas White (Index 1)
    lv_canvas_fill_bg(global_logic_canvas, lv_color_make(0, 0, 1), LV_OPA_COVER);

    char buf[16];

    // Top: Peripheral and Heartbeat
    snprintf(buf, sizeof(buf), "PER%s", (global_draw_count % 2) ? "*" : " ");
    draw_text(global_logic_canvas, 0, 0, buf);

    // Connection Section
    bool connected = zmk_split_bt_peripheral_is_connected();
    draw_text(global_logic_canvas, 0, 20, "LNK");
    draw_text(global_logic_canvas, 0, 30, connected ? "ON" : "OFF");

    // Battery Section
    draw_text(global_logic_canvas, 0, 50, "BAT");
    snprintf(buf, sizeof(buf), "%d%%", get_natural_battery_level());
    draw_text(global_logic_canvas, 0, 60, buf);

    // Uptime Section
    draw_text(global_logic_canvas, 0, 80, "UP:");
    long long uptime_s = k_uptime_get() / 1000;
    snprintf(buf, sizeof(buf), "%lld", uptime_s);
    draw_text(global_logic_canvas, 0, 90, buf);

    // 2. Rotate to physical
    rotate_canvas(global_phys_canvas, global_logic_canvas);
}

static void debug_work_handler(struct k_work *work) { draw_canvas(); }
K_WORK_DEFINE(debug_work, debug_work_handler);

static void debug_timer_handler(struct k_timer *timer) { k_work_submit(&debug_work); }
K_TIMER_DEFINE(debug_timer, debug_timer_handler, NULL);

LV_DRAW_BUF_DEFINE_STATIC(logic_draw_buf_peri, 128, 128, LV_COLOR_FORMAT_I1);
LV_DRAW_BUF_DEFINE_STATIC(phys_draw_buf_peri, 128, 32, LV_COLOR_FORMAT_I1);

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent)
{
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 128, 32);

    lv_obj_t *phys_canvas = lv_canvas_create(widget->obj);
    lv_obj_align(phys_canvas, LV_ALIGN_TOP_LEFT, 0, 0);
    LV_DRAW_BUF_INIT_STATIC(phys_draw_buf_peri);
    lv_canvas_set_draw_buf(phys_canvas, &phys_draw_buf_peri);

    lv_canvas_set_palette(phys_canvas, 0, lv_color32_make(0, 0, 0, 255));
    lv_canvas_set_palette(phys_canvas, 1, lv_color32_make(255, 255, 255, 255));

    lv_obj_t *logic_canvas = lv_canvas_create(NULL);
    LV_DRAW_BUF_INIT_STATIC(logic_draw_buf_peri);
    lv_canvas_set_draw_buf(logic_canvas, &logic_draw_buf_peri);
    lv_canvas_set_palette(logic_canvas, 0, lv_color32_make(0, 0, 0, 255));
    lv_canvas_set_palette(logic_canvas, 1, lv_color32_make(255, 255, 255, 255));

    global_phys_canvas = phys_canvas;
    global_logic_canvas = logic_canvas;

    widget->node.next = NULL;
    sys_slist_append(&widgets, &widget->node);

    draw_canvas();
    k_timer_start(&debug_timer, K_SECONDS(1), K_SECONDS(1));

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
