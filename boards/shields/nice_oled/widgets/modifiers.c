/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 */

#include <zephyr/bluetooth/services/bas.h>
#include <zephyr/kernel.h>
#include <zephyr/logging/log.h>
LOG_MODULE_DECLARE(zmk, CONFIG_ZMK_LOG_LEVEL);

#include <dt-bindings/zmk/modifiers.h>
#include <zmk/display.h>
#include <zmk/event_manager.h>
#include <zmk/events/keycode_state_changed.h>
#include <zmk/hid.h>

#include "modifiers.h"

struct modifiers_state {
  uint8_t modifiers;
};

static sys_slist_t widgets = SYS_SLIST_STATIC_INIT(&widgets);

LV_IMG_DECLARE(dog_sit1_90);
LV_IMG_DECLARE(dog_sit2_90);
LV_IMG_DECLARE(dog_walk1_90);
LV_IMG_DECLARE(dog_walk2_90);
LV_IMG_DECLARE(dog_run1_90);
LV_IMG_DECLARE(dog_run2_90);
LV_IMG_DECLARE(dog_sneak1_90);
LV_IMG_DECLARE(dog_sneak2_90);

const lv_img_dsc_t *luna_imgs_sit_90[] = {&dog_sit1_90, &dog_sit2_90};
const lv_img_dsc_t *luna_imgs_walk_90[] = {&dog_walk1_90, &dog_walk2_90};
const lv_img_dsc_t *luna_imgs_run_90[] = {&dog_run1_90, &dog_run2_90};
const lv_img_dsc_t *luna_imgs_sneak_90[] = {&dog_sneak1_90, &dog_sneak2_90};

static lv_obj_t *luna_imgs = NULL;

static void set_modifiers_text(lv_obj_t *label,
                               struct modifiers_state ignored) {
  uint8_t mods = zmk_hid_get_explicit_mods();

  lv_label_set_text(label, "");

  if (mods & (MOD_LGUI | MOD_RGUI)) {

    if (!luna_imgs) {

      luna_imgs = lv_animimg_create(label);
      lv_obj_center(luna_imgs);

      lv_animimg_set_src(luna_imgs, (const void **)luna_imgs_sit_90, 2);
      lv_animimg_set_duration(
          luna_imgs,
          CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS);
      lv_animimg_set_repeat_count(luna_imgs, LV_ANIM_REPEAT_INFINITE);
      lv_animimg_start(luna_imgs);
      lv_obj_align(luna_imgs, LV_ALIGN_TOP_LEFT, 36, 0);
    }
  } else if (mods & (MOD_LALT | MOD_RALT)) {
    if (!luna_imgs) {

      luna_imgs = lv_animimg_create(label);
      lv_obj_center(luna_imgs);

      lv_animimg_set_src(luna_imgs, (const void **)luna_imgs_walk_90, 2);
      lv_animimg_set_duration(
          luna_imgs,
          CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS);
      lv_animimg_set_repeat_count(luna_imgs, LV_ANIM_REPEAT_INFINITE);
      lv_animimg_start(luna_imgs);
      lv_obj_align(luna_imgs, LV_ALIGN_TOP_LEFT, 36, 0);
    }
  } else if (mods & (MOD_LCTL | MOD_RCTL)) {
    if (!luna_imgs) {

      luna_imgs = lv_animimg_create(label);
      lv_obj_center(luna_imgs);

      lv_animimg_set_src(luna_imgs, (const void **)luna_imgs_run_90, 2);
      lv_animimg_set_duration(
          luna_imgs,
          CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS);
      lv_animimg_set_repeat_count(luna_imgs, LV_ANIM_REPEAT_INFINITE);
      lv_animimg_start(luna_imgs);
      lv_obj_align(luna_imgs, LV_ALIGN_TOP_LEFT, 36, 0);
    }
  } else if (mods & (MOD_LSFT | MOD_RSFT)) {
    if (!luna_imgs) {

      luna_imgs = lv_animimg_create(label);
      lv_obj_center(luna_imgs);

      lv_animimg_set_src(luna_imgs, (const void **)luna_imgs_sneak_90, 2);
      lv_animimg_set_duration(
          luna_imgs,
          CONFIG_NICE_OLED_WIDGET_MODIFIERS_INDICATORS_LUNA_ANIMATION_MS);
      lv_animimg_set_repeat_count(luna_imgs, LV_ANIM_REPEAT_INFINITE);
      lv_animimg_start(luna_imgs);
      lv_obj_align(luna_imgs, LV_ALIGN_TOP_LEFT, 36, 0);
    }
  } else {
    if (luna_imgs) {
      lv_obj_del(luna_imgs);
      luna_imgs = NULL;
    }
  }
}

static void modifiers_update_cb(struct modifiers_state state) {
  struct zmk_widget_modifiers *widget;
  SYS_SLIST_FOR_EACH_CONTAINER(&widgets, widget, node) {
    set_modifiers_text(widget->obj, state);
  }
}

static struct modifiers_state modifiers_get_state(const zmk_event_t *eh) {
  return (struct modifiers_state){.modifiers = zmk_hid_get_explicit_mods()};
}

ZMK_DISPLAY_WIDGET_LISTENER(widget_modifiers, struct modifiers_state,
                            modifiers_update_cb, modifiers_get_state)
ZMK_SUBSCRIPTION(widget_modifiers, zmk_keycode_state_changed);

int zmk_widget_modifiers_init(struct zmk_widget_modifiers *widget,
                              lv_obj_t *parent) {
  widget->obj = lv_label_create(parent);
  sys_slist_append(&widgets, &widget->node);
  widget_modifiers_init();
  return 0;
}
