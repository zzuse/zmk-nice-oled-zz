#pragma once

#include "util.h"
#include <lvgl.h>

struct battery_status_state {
    uint8_t level;
    bool usb_present;
};
void draw_battery_status(lv_obj_t *canvas, const struct status_state *state);
