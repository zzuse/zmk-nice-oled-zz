/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#pragma once

#include "util.h"
#include <lvgl.h>
#include <zephyr/kernel.h>

struct zmk_widget_modifiers {
  sys_snode_t node;
  lv_obj_t *obj;
};

int zmk_widget_modifiers_init(struct zmk_widget_modifiers *widget,
                              lv_obj_t *parent);
