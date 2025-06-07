#include "battery.h"
#include "../assets/custom_fonts.h"
#include <zephyr/kernel.h>

LV_IMG_DECLARE(bolt);

#if IS_ENABLED(CONFIG_NICE_OLED_GEM_ANIMATION_SMART_BATTERY)
// Work at how it is expected, but after disconnecting the keyboard 3 times it is fried
// CONFIG_NICE_OLED_GEM_ANIMATION
LV_IMG_DECLARE(crystal_01);
LV_IMG_DECLARE(crystal_02);
LV_IMG_DECLARE(crystal_03);
LV_IMG_DECLARE(crystal_04);
LV_IMG_DECLARE(crystal_05);
LV_IMG_DECLARE(crystal_06);
LV_IMG_DECLARE(crystal_07);
LV_IMG_DECLARE(crystal_08);
LV_IMG_DECLARE(crystal_09);
LV_IMG_DECLARE(crystal_10);
LV_IMG_DECLARE(crystal_11);
LV_IMG_DECLARE(crystal_12);
LV_IMG_DECLARE(crystal_13);
LV_IMG_DECLARE(crystal_14);
LV_IMG_DECLARE(crystal_15);
LV_IMG_DECLARE(crystal_16);

#ifndef SET_ANIMATION_SMART_BATTERY_OFF
#define SET_ANIMATION_SMART_BATTERY_OFF &crystal_01
#endif

const lv_img_dsc_t *crystal_imgs_test[] = {
    &crystal_01, &crystal_02, &crystal_03, &crystal_04, &crystal_05, &crystal_06,
    &crystal_07, &crystal_08, &crystal_09, &crystal_10, &crystal_11, &crystal_12,
    &crystal_13, &crystal_14, &crystal_15, &crystal_16,
};

static lv_obj_t *art = NULL;
static lv_obj_t *art2 = NULL;

static void animation_smart_battery_on(lv_obj_t *canvas) {
    if (art2) {
        lv_obj_del(art2);
        art2 = NULL;
    }
    art = lv_animimg_create(canvas);
    lv_obj_center(art);

    lv_animimg_set_src(art, (const void **)crystal_imgs_test, 16);
    lv_animimg_set_duration(art, CONFIG_NICE_OLED_GEM_ANIMATION_MS);
    lv_animimg_set_repeat_count(art, LV_ANIM_REPEAT_INFINITE);
    lv_animimg_start(art);
    lv_obj_align(art, LV_ALIGN_TOP_LEFT, 18, -18);
}
static void animation_smart_battery_off(lv_obj_t *canvas) {

    if (art) {
        lv_obj_del(art);
        art = NULL;
    }
    /*
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_canvas_draw_img(canvas, 18, -18, SET_ANIMATION_SMART_BATTERY_OFF, &img_dsc);
    */

    art2 = lv_img_create(canvas);
    lv_img_set_src(art2, SET_ANIMATION_SMART_BATTERY_OFF);
    lv_obj_align(art2, LV_ALIGN_TOP_LEFT, 18, -18);
}
#endif

static void draw_level(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_label_dsc_t label_right_dsc;
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    // LV_TEXT_ALIGN_RIGHT);

    char text[10] = {};

    sprintf(text, "%i%%", state->battery);
    // sprintf(text, "%i%%", state->battery);
    // x, y, width, dsc, text
    lv_canvas_draw_text(canvas, 0, 146, 42, &label_right_dsc, text);
    // lv_canvas_draw_text(canvas, -6, 50, 42, &label_right_dsc, text);
}

static void draw_charging_level(lv_obj_t *canvas, const struct status_state *state) {
    lv_draw_img_dsc_t img_dsc;
    lv_draw_img_dsc_init(&img_dsc);
    lv_draw_label_dsc_t label_right_dsc;
    init_label_dsc(&label_right_dsc, LVGL_FOREGROUND, &pixel_operator_mono, LV_TEXT_ALIGN_LEFT);
    // LV_TEXT_ALIGN_RIGHT);

    char text[10] = {};

    sprintf(text, "%i", state->battery);
    // sprintf(text, "%i%%", state->battery);
    lv_canvas_draw_text(canvas, 0, 146, 35, &label_right_dsc, text);
    // lv_canvas_draw_text(canvas, 1, 50, 35, &label_right_dsc, text);
    lv_canvas_draw_img(canvas, 25, 146, &bolt, &img_dsc);
    // lv_canvas_draw_img(canvas, 0, 50, &bolt, &img_dsc);
}

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state) {
    /*
    lv_draw_label_dsc_t label_left_dsc;
    init_label_dsc(&label_left_dsc, LVGL_FOREGROUND, &pixel_operator_mono,
                   LV_TEXT_ALIGN_LEFT);
    lv_canvas_draw_text(canvas, 0, 19, 25, &label_left_dsc, "BAT");
    */

    if (state->charging) {
        draw_charging_level(canvas, state);
#if IS_ENABLED(CONFIG_NICE_OLED_GEM_ANIMATION_SMART_BATTERY)
        animation_smart_battery_on(canvas);
#endif

    } else {
        draw_level(canvas, state);
#if IS_ENABLED(CONFIG_NICE_OLED_GEM_ANIMATION_SMART_BATTERY)
        animation_smart_battery_off(canvas);
#endif
    }
}
