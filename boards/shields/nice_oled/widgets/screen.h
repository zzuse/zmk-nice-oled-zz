#pragma once
#ifndef SCREEN_H_
#define SCREEN_H_

#include "util.h"
#include <lvgl.h>
#include <zephyr/kernel.h>

struct battery_status_state {
    uint8_t level;
    bool usb_present;
};

struct layer_status_state {
    uint8_t index;
    const char *label;
};

struct output_status_state {
};

struct zmk_widget_screen {
  sys_snode_t node;
  lv_obj_t *obj;
  lv_color_t cbuf[528 / sizeof(lv_color_t)]; // 128*32/8 + 8 palette + padding
  struct status_state state;
};

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget);
#endif
