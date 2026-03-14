#pragma once

#include "util.h"
#include <lvgl.h>

// Layer and Heartbeat (Max 8 chars: L0*)
void draw_layer_status(lv_obj_t *canvas, const struct status_state *state, int draw_count, int x, int y);