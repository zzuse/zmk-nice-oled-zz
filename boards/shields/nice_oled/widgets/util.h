#pragma once

#include <lvgl.h>
#include <zmk/endpoints.h>

#define CANVAS_WIDTH 32
#define CANVAS_HEIGHT 128
#define ROTATION_SIZE 128

#define LVGL_BACKGROUND lv_color_make(0, 0, 1)
#define LVGL_FOREGROUND lv_color_make(0, 0, 0)

struct status_state {
  uint8_t battery;
  bool charging;
#if !IS_ENABLED(CONFIG_ZMK_SPLIT) || IS_ENABLED(CONFIG_ZMK_SPLIT_ROLE_CENTRAL)
  struct zmk_endpoint_instance selected_endpoint;
  int active_profile_index;
  bool active_profile_connected;
  bool active_profile_bonded;
  uint8_t layer_index;
  const char *layer_label;
#else
  bool connected;
#endif
};

void rotate_canvas(lv_obj_t *phys_canvas, lv_obj_t *logic_canvas);
void draw_background(lv_obj_t *canvas);
uint8_t get_natural_battery_level(void);
