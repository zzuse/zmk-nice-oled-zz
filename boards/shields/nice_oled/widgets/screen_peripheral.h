#pragma once
#ifndef SCREEN_PERIPHERAL_H_
#define SCREEN_PERIPHERAL_H_

#include "util.h"
#include <lvgl.h>
#include <zephyr/kernel.h>

struct battery_status_state {
    uint8_t level;
    bool usb_present;
};

struct peripheral_status_state {
    bool connected;
};

struct zmk_widget_screen {
    sys_snode_t node;
    lv_obj_t *obj;
    uint8_t cbuf[LV_CANVAS_BUF_SIZE(CANVAS_HEIGHT, CANVAS_HEIGHT, 16, LV_DRAW_BUF_STRIDE_ALIGN)];
    struct status_state state;
};

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget);
#endif
