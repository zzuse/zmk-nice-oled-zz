#include <zephyr/kernel.h>
#include <zmk/battery.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/split_peripheral_status_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/split/bluetooth/peripheral.h>
#include <zmk/usb.h>

#include "animation.h"
#include "battery.h"
#include "screen_peripheral.h"
#include "uptime.h"
#include "util.h"

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);
static lv_obj_t *global_logic_canvas = NULL;
static struct zmk_widget_screen *global_widget = NULL;
static int global_draw_count = 0;

static void draw_canvas(lv_obj_t *widget, struct status_state *state)
{
    lv_obj_t *phys_canvas = lv_obj_get_child(widget, 0);
    if (!phys_canvas) return;

    global_draw_count++;

    // clear
    lv_canvas_fill_bg(global_logic_canvas, lv_color_make(0, 0, 1), LV_OPA_COVER);

    char buf[16];

    // Top: Peripheral and Heartbeat
    snprintf(buf, sizeof(buf), "PER%s", (global_draw_count % 2) ? "*" : " ");
    draw_text_big(global_logic_canvas, 0, 0, buf);

    // Uptime Section
    draw_uptime(global_logic_canvas, state, 0, 16);

    // Connection Section
    bool connected = zmk_split_bt_peripheral_is_connected();
    draw_text(global_logic_canvas, 0, 106, connected ? "L<=ON" : "L<OFF");

    // Battery Section
    draw_battery_status(global_logic_canvas, state, 0, 118);

    // Rotate to physical
    rotate_canvas(phys_canvas, global_logic_canvas);

    // Draw animation
    draw_spaceman(phys_canvas);
    lv_obj_invalidate(phys_canvas);
}

/** -----------------------------------------------------
 * Uptime timer
 **/

static void uptime_work_handler(struct k_work *work)
{
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node)
    {
        // Update the uptime in the shared state
        widget->state.uptime_seconds = (uint32_t)(k_uptime_get() / 1000);
        // Trigger a full redraw (same as any event)
        draw_canvas(widget->obj, &widget->state);
    }
}
K_WORK_DEFINE(uptime_work, uptime_work_handler);
static void uptime_timer_handler(struct k_timer *timer) { k_work_submit_to_queue(zmk_display_work_q(), &uptime_work); }
K_TIMER_DEFINE(uptime_timer, uptime_timer_handler, NULL);

/** -----------------------------------------------------

 * Battery status
 **/

static void set_battery_status(struct zmk_widget_screen *widget, struct status_state state)
{
    widget->state.battery = state.battery;
    widget->state.charging = state.charging;

    draw_canvas(widget->obj, &widget->state);
}

static void battery_status_update_cb(struct status_state state)
{
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_battery_status(widget, state); }
}

static struct status_state battery_status_get_state(const zmk_event_t *eh)
{
    return (struct status_state){.battery = get_natural_battery_level(), .charging = zmk_usb_is_powered()};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_battery_status, struct status_state, battery_status_update_cb,
                            battery_status_get_state)

ZMK_SUBSCRIPTION(widget_battery_status, zmk_battery_state_changed);
ZMK_SUBSCRIPTION(widget_battery_status, zmk_usb_conn_state_changed);

/** -----------------------------------------------------
 * Initialization
 **/

LV_DRAW_BUF_DEFINE_STATIC(logic_draw_buf, 128, 128, LV_COLOR_FORMAT_I1);
LV_DRAW_BUF_DEFINE_STATIC(phys_draw_buf, 128, 32, LV_COLOR_FORMAT_I1);

int zmk_widget_screen_init(struct zmk_widget_screen *screen, lv_obj_t *parent)
{
    screen->obj = lv_obj_create(parent);
    lv_obj_set_size(screen->obj, 128, 32);

    lv_obj_t *phys_canvas = lv_canvas_create(screen->obj);
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

    global_logic_canvas = logic_canvas;
    global_widget = screen;

    screen->node.next = NULL;
    sys_slist_append(&widgets, &screen->node);

    widget_battery_status_init();

    draw_canvas(screen->obj, &screen->state);
    k_timer_start(&uptime_timer, K_SECONDS(1), K_SECONDS(1));

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
