#pragma once

#include "util.h"
#include <lvgl.h>
#include <zmk/endpoints.h>

void draw_output_status(lv_obj_t *phys_canvas, const struct status_state *state, int x, int y);
