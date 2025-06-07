#pragma once

#include "util.h"
#include <lvgl.h>

struct keycode_status_state {
    uint32_t keycode;
};
void draw_keycode_status(lv_obj_t *canvas, const struct status_state* state);