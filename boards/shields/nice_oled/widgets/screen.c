#include <zephyr/kernel.h>
#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/usb.h>

#include "battery.h"
#include "keycode.h"
#include "screen.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);
static lv_obj_t *global_logic_canvas = NULL;
static lv_obj_t *global_phys_canvas = NULL;
static struct zmk_widget_screen *global_widget = NULL;
static int global_draw_count = 0;

static void draw_canvas(void)
{
    if (!global_logic_canvas || !global_phys_canvas) return;

    global_draw_count++;

    lv_canvas_fill_bg(global_logic_canvas, lv_color_make(0, 0, 1), LV_OPA_COVER);

    char buf[16];

    // Top: Layer and Heartbeat (Max 8 chars: L0*)
    uint8_t l_idx = zmk_keymap_highest_layer_active();
    const char *l_name = zmk_keymap_layer_name(l_idx);
    snprintf(buf, sizeof(buf), "%s%s", l_name, (global_draw_count % 2) ? "*" : " ");
    draw_text(global_logic_canvas, 0, 0, buf);

    // Uptime Section
    draw_text(global_logic_canvas, 0, 7, "UP:");
    long long uptime_s = k_uptime_get() / 1000;
    snprintf(buf, sizeof(buf), "%lld", uptime_s);
    draw_text(global_logic_canvas, 0, 14, buf);

    // Keycode Name
    if (global_widget) {
        global_widget->state.keycode = 44; // test only
        draw_keycode_status(global_logic_canvas, &(global_widget->state), 0, 21);
    }

    bool usb_present = zmk_usb_is_powered();
    if (usb_present) {
        // USB 106+11
        snprintf(buf, sizeof(buf), "USB %s", usb_present ? "ON" : "OFF");
        draw_text(global_logic_canvas, 0, 106, buf);
    } else {
        // Bluetooth Section 104 + 15 = 119
        int b_idx = zmk_ble_active_profile_index() + 1;
        bool b_conn = zmk_ble_active_profile_is_connected();
        snprintf(buf, sizeof(buf), "BT%d%s", b_idx, b_conn ? "ON" : "OFF");
        draw_text(global_logic_canvas, 0, 104, buf);
    }

    // Battery percentage 120 - 127 text (on logic canvas, will be rotated with other text)
    uint8_t batt_level = get_natural_battery_level();
    snprintf(buf, sizeof(buf), "%d%%", batt_level);
    draw_text(global_logic_canvas, 0, 120, buf);

    rotate_canvas(global_phys_canvas, global_logic_canvas);

    struct status_state temp_state = {
        .battery = batt_level,
        .charging = true, // TODO: pull from real USB/charging status
    };
    // Crystal GEM 35+69=104
    // Battery bolt 116 - 127
    draw_battery_status(global_phys_canvas, &temp_state);

    lv_obj_invalidate(global_phys_canvas);
}

static void debug_work_handler(struct k_work *work) { draw_canvas(); }
K_WORK_DEFINE(debug_work, debug_work_handler);

static void debug_timer_handler(struct k_timer *timer) { k_work_submit_to_queue(zmk_display_work_q(), &debug_work); }
K_TIMER_DEFINE(debug_timer, debug_timer_handler, NULL);

LV_DRAW_BUF_DEFINE_STATIC(logic_draw_buf, 128, 128, LV_COLOR_FORMAT_I1);
LV_DRAW_BUF_DEFINE_STATIC(phys_draw_buf, 128, 32, LV_COLOR_FORMAT_I1);

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent)
{
    widget->obj = lv_obj_create(parent);
    lv_obj_set_size(widget->obj, 128, 32);

    lv_obj_t *phys_canvas = lv_canvas_create(widget->obj);
    lv_obj_align(phys_canvas, LV_ALIGN_TOP_LEFT, 0, 0);
    LV_DRAW_BUF_INIT_STATIC(phys_draw_buf);
    lv_canvas_set_draw_buf(phys_canvas, &phys_draw_buf);

    lv_canvas_set_palette(phys_canvas, 0, lv_color32_make(0, 0, 0, 255));
    lv_canvas_set_palette(phys_canvas, 1, lv_color32_make(255, 255, 255, 255));

    lv_obj_t *logic_canvas = lv_canvas_create(NULL);
    LV_DRAW_BUF_INIT_STATIC(logic_draw_buf);
    lv_canvas_set_draw_buf(logic_canvas, &logic_draw_buf);
    lv_canvas_set_palette(logic_canvas, 0, lv_color32_make(0, 0, 0, 255));
    lv_canvas_set_palette(logic_canvas, 1, lv_color32_make(255, 255, 255, 255));

    global_phys_canvas = phys_canvas;
    global_logic_canvas = logic_canvas;
    global_widget = widget;

    widget->node.next = NULL;
    sys_slist_append(&widgets, &widget->node);

    draw_canvas();
    k_timer_start(&debug_timer, K_MSEC(60), K_MSEC(60));

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
