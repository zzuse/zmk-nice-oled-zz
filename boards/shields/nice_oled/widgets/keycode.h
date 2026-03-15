#pragma once

#include "util.h"
#include <lvgl.h>

void draw_keycode_status(lv_obj_t* canvas, const struct status_state* state, int x, int y);
void draw_keycode_status_peripheral(lv_obj_t* canvas, const struct status_state* state, int x, int y);