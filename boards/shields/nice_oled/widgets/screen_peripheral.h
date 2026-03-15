#pragma once
#ifndef SCREEN_PERIPHERAL_H_
#define SCREEN_PERIPHERAL_H_

#include "util.h"
#include <lvgl.h>
#include <zephyr/kernel.h>

struct peripheral_status_state {
    bool connected;
};

struct zmk_widget_screen {
    sys_snode_t node;
    lv_obj_t *obj;
    struct status_state state;
};

int zmk_widget_screen_init(struct zmk_widget_screen *widget, lv_obj_t *parent);
lv_obj_t *zmk_widget_screen_obj(struct zmk_widget_screen *widget);
#endif
