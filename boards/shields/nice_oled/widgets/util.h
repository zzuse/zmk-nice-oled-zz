#pragma once

#include <lvgl.h>
#include <zmk/endpoints.h>

#define CANVAS_WIDTH 32
#define CANVAS_HEIGHT 128
#define ROTATION_SIZE 128

#define LVGL_BACKGROUND lv_color_make(0, 0, 0)
#define LVGL_FOREGROUND lv_color_make(0, 0, 1)

struct status_state {
    uint8_t battery;
    bool charging;
    uint32_t uptime_seconds;
    uint32_t keycode;
    uint16_t usage_page;
    uint8_t implicit_modifiers;
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
    struct zmk_endpoint_instance selected_endpoint;
    int active_profile_index;
    bool active_profile_connected;
    bool active_profile_bonded;
    uint8_t layer_index;
    const char *layer_label;
    uint8_t mod_state;
#else
    bool connected;
#endif
};

// draw_text1 is 12pixel draw_text2 is 8pixel, big is 16pixel
void _draw_text(lv_obj_t *target_canvas, int x, int y, const char *text, bool small);
#define draw_text1(canvas, x, y, text) _draw_text(canvas, x, y, text, false)
#define draw_text2(canvas, x, y, text, small) _draw_text(canvas, x, y, text, small)
#define GET_DRAW_TEXT(_1, _2, _3, _4, _5, NAME, ...) NAME
#define draw_text(...) GET_DRAW_TEXT(__VA_ARGS__, draw_text2, draw_text1)(__VA_ARGS__)
void draw_text_big(lv_obj_t *target_canvas, int x, int y, const char *text);

void rotate_canvas(lv_obj_t *phys_canvas, lv_obj_t *logic_canvas);
void draw_background(lv_obj_t *canvas);
uint8_t get_natural_battery_level(void);
void draw_img(lv_obj_t *target_canvas, int x, int y, const lv_img_dsc_t *img_dsc);
