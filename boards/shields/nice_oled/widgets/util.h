#pragma once

#include <lvgl.h>
#include <zmk/endpoints.h>

#define CANVAS_WIDTH 32
#define CANVAS_HEIGHT 128
#define ROTATION_SIZE 128

#define LVGL_BACKGROUND lv_color_white()
#define LVGL_FOREGROUND lv_color_black()

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

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]);
void draw_background(lv_obj_t *canvas);
void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color,
                    const lv_font_t *font, lv_text_align_t align);
