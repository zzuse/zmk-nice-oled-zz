#include <zephyr/kernel.h>
#include <zmk/battery.h>
#include <zmk/ble.h>
#include <zmk/display.h>
#include <zmk/endpoints.h>
#include <zmk/event_manager.h>
#include <zmk/events/battery_state_changed.h>
#include <zmk/events/ble_active_profile_changed.h>
#include <zmk/events/endpoint_changed.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/events/layer_state_changed.h>
#include <zmk/events/usb_conn_state_changed.h>
#include <zmk/keymap.h>
#include <zmk/usb.h>

#include "animation.h"
#include "battery.h"
#include "keycode.h"
#include "layer.h"
#include "output.h"
#include "screen.h"
#include "uptime.h"

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
    // layer
    draw_layer_status(global_logic_canvas, state, global_draw_count, 0, 0);

    // Uptime Section
    draw_uptime(global_logic_canvas, state, 0, 10);

    // Keycode Name
    draw_keycode_status(global_logic_canvas, state, 0, 20);

    // OUTPUT USB/BT
    draw_output_status(global_logic_canvas, state, 6, 104);

    // Battery percentage and bolt 120 - 127 text (on logic canvas, will be rotated with other text)
    draw_battery_status(global_logic_canvas, state, 0, 118);

    // logic canvas to physical canvas rotate
    rotate_canvas(phys_canvas, global_logic_canvas);

    // Crystal direct draw on physical canvas 35+69=104
    draw_animation(phys_canvas);

    lv_obj_invalidate(phys_canvas);
}

/** -----------------------------------------------------
 * Layer status
 **/

static void set_layer_status(struct zmk_widget_screen *widget, struct status_state state)
{
    widget->state.layer_index = state.layer_index;
    widget->state.layer_label = state.layer_label;

    draw_canvas(widget->obj, &widget->state);
}

static void layer_status_update_cb(struct status_state state)
{
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_layer_status(widget, state); }
}

static struct status_state layer_status_get_state(const zmk_event_t *eh)
{
    uint8_t index = zmk_keymap_highest_layer_active();
    return (struct status_state){.layer_index = index, .layer_label = zmk_keymap_layer_name(index)};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_layer_status, struct status_state, layer_status_update_cb, layer_status_get_state)

ZMK_SUBSCRIPTION(widget_layer_status, zmk_layer_state_changed);

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
 * KeyCode
 **/

#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
static void set_keycode_status(struct zmk_widget_screen *widget, struct status_state state)
{
    widget->state.keycode = state.keycode;
    widget->state.usage_page = state.usage_page;
    widget->state.implicit_modifiers = state.implicit_modifiers;
    draw_canvas(widget->obj, &widget->state);
}

static void keycode_status_update_cb(struct status_state state)
{
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_keycode_status(widget, state); }
}

static uint32_t last_keycode = 0;
static uint16_t last_usage_page = 0x07;
static uint8_t last_implicit_modifiers = 0;

struct status_state keycode_status_get_state(const zmk_event_t *eh)
{
    const struct zmk_keycode_state_changed *ev = as_zmk_keycode_state_changed(eh);
    if (ev->state) { // Only on key press, not release
        last_keycode = ev->keycode;
        last_usage_page = ev->usage_page;
        last_implicit_modifiers = ev->implicit_modifiers;
    }
    return (struct status_state){
        .keycode = last_keycode, .usage_page = last_usage_page, .implicit_modifiers = last_implicit_modifiers};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_keycode_status, struct status_state, keycode_status_update_cb,
                            keycode_status_get_state);
ZMK_SUBSCRIPTION(widget_keycode_status, zmk_keycode_state_changed);
#endif

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
 * Output status
 **/

static void set_output_status(struct zmk_widget_screen *widget, struct status_state state)
{
    widget->state.selected_endpoint = state.selected_endpoint;
    widget->state.active_profile_index = state.active_profile_index;
    widget->state.active_profile_connected = state.active_profile_connected;
    widget->state.active_profile_bonded = state.active_profile_bonded;

    draw_canvas(widget->obj, &widget->state);
}

static void output_status_update_cb(struct status_state state)
{
    struct zmk_widget_screen *widget;
    SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) { set_output_status(widget, state); }
}

static struct status_state output_status_get_state(const zmk_event_t *_eh)
{
    return (struct status_state){
        .selected_endpoint = zmk_endpoint_get_selected(),
        .active_profile_index = zmk_ble_active_profile_index(),
        .active_profile_connected = zmk_ble_active_profile_is_connected(),
        .active_profile_bonded = !zmk_ble_active_profile_is_open(),
    };
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_output_status, struct status_state, output_status_update_cb, output_status_get_state)

ZMK_SUBSCRIPTION(widget_output_status, zmk_endpoint_changed);
ZMK_SUBSCRIPTION(widget_output_status, zmk_usb_conn_state_changed);
ZMK_SUBSCRIPTION(widget_output_status, zmk_ble_active_profile_changed);

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

    widget_layer_status_init();
    widget_keycode_status_init();
    widget_battery_status_init();
    widget_output_status_init();

    draw_canvas(screen->obj, &screen->state);

    k_timer_start(&uptime_timer, K_MSEC(1000), K_MSEC(1000));

    return 0;
}

lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget) { return widget->obj; }
