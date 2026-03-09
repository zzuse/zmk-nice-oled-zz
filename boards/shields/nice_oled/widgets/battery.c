#include "battery.h"
#include <zephyr/kernel.h>

LV_IMG_DECLARE(bolt);

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

static const lv_img_dsc_t *crystal_imgs[] = {
    &crystal_01, &crystal_02, &crystal_03, &crystal_04, &crystal_05, &crystal_06, &crystal_07, &crystal_08,
    &crystal_09, &crystal_10, &crystal_11, &crystal_12, &crystal_13, &crystal_14, &crystal_15, &crystal_16,
};

#define CRYSTAL_FRAME_COUNT 16

/* Animation state */
static int animation_frame = 0;

/**
 * Direct pixel blit from an I1 image onto a canvas buffer.
 * Bypasses LVGL's draw/layer system entirely — no memory allocation.
 * Only draws foreground (black) pixels; white pixels are transparent.
 * Handles clipping for out-of-bounds coordinates.
 *
 * Crystal palette: index 0 = white, index 1 = black
 * Canvas palette:  index 0 = black, index 1 = white
 * So crystal bit=1 (black) → clear canvas bit (index 0 = black)
 */
static void blit_i1_to_canvas(lv_obj_t *canvas, const lv_img_dsc_t *img, int dest_x, int dest_y)
{
    lv_draw_buf_t *buf = lv_canvas_get_draw_buf(canvas);
    uint8_t *dst = ((uint8_t *)buf->data) + 8;
    uint32_t dst_stride = buf->header.stride;
    int canvas_w = buf->header.w;
    int canvas_h = buf->header.h;

    const uint8_t *src = img->data + 8;
    int img_w = img->header.w;
    int img_h = img->header.h;
    int src_stride = (img_w + 7) / 8;

    int sy_start = (dest_y < 0) ? -dest_y : 0;
    int sy_end = (dest_y + img_h > canvas_h) ? (canvas_h - dest_y) : img_h;
    int sx_start = (dest_x < 0) ? -dest_x : 0;
    int sx_end = (dest_x + img_w > canvas_w) ? (canvas_w - dest_x) : img_w;

    for (int sy = sy_start; sy < sy_end; sy++) {
        int py = dest_y + sy;
        for (int sx = sx_start; sx < sx_end; sx++) {
            int px = dest_x + sx;

            int s_idx = sy * src_stride + (sx >> 3);
            int s_bit = 7 - (sx & 7);

            if (src[s_idx] & (1 << s_bit)) {
                int d_idx = py * dst_stride + (px >> 3);
                int d_bit = 7 - (px & 7);
                dst[d_idx] &= ~(1 << d_bit);
            }
        }
    }
}

static void draw_crystal(lv_obj_t *phys_canvas, const lv_img_dsc_t *img)
{
    blit_i1_to_canvas(phys_canvas, img, 24, -18);
}

static void animation_smart_battery_on(lv_obj_t *phys_canvas)
{
    draw_crystal(phys_canvas, crystal_imgs[animation_frame]);

    /* Advance frame — called once per draw tick, so each frame
     * lasts exactly one timer period (60ms). Full cycle = 16 × 60ms = 960ms. */
    animation_frame = (animation_frame + 1) % CRYSTAL_FRAME_COUNT;
}

static void animation_smart_battery_off(lv_obj_t *phys_canvas)
{
    animation_frame = 0;
    draw_crystal(phys_canvas, SET_ANIMATION_SMART_BATTERY_OFF);
}

void draw_battery_status(lv_obj_t *phys_canvas, const struct status_state *state)
{
    if (state->charging) {
        animation_smart_battery_on(phys_canvas);
    } else {
        animation_smart_battery_off(phys_canvas);
    }
}
