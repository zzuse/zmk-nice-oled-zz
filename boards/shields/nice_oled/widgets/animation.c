#include "animation.h"
#include "../assets/spaceman.h"
#include <zephyr/kernel.h>

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

#define CRYSTAL_FRAME_COUNT 16
static const lv_img_dsc_t *crystal_imgs[] = {
    &crystal_01, &crystal_02, &crystal_03, &crystal_04, &crystal_05, &crystal_06, &crystal_07, &crystal_08,
    &crystal_09, &crystal_10, &crystal_11, &crystal_12, &crystal_13, &crystal_14, &crystal_15, &crystal_16,
};

static int animation_frame = 0;
static lv_timer_t *anim_timer = NULL;
static lv_obj_t *global_phys_canvas = NULL;

/**
 * Direct pixel blit from an I1 image onto a canvas buffer.
 * Draws both Black (Index 1) and White (Index 0) to replace previous frames.
 */
static void blit_i1_to_canvas_opaque(lv_obj_t *canvas, const lv_img_dsc_t *img, int dest_x, int dest_y)
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

            int d_idx = py * dst_stride + (px >> 3);
            int d_bit = 7 - (px & 7);

            if (src[s_idx] & (1 << s_bit)) {
                // Pixel is Black (Index 1 in source). Draw as Black (bit 0 in dst).
                dst[d_idx] &= ~(1 << d_bit);
            } else {
                // Pixel is White (Index 0 in source). Draw as White (bit 1 in dst).
                dst[d_idx] |= (1 << d_bit);
            }
        }
    }
}

static void anim_timer_cb(lv_timer_t *timer)
{
    if (!global_phys_canvas) return;

    // Draw current frame (Opaque to clean last frame)
    blit_i1_to_canvas_opaque(global_phys_canvas, crystal_imgs[animation_frame], 24, -18);
    lv_obj_invalidate(global_phys_canvas);

    // Advance frame
    animation_frame = (animation_frame + 1) % CRYSTAL_FRAME_COUNT;
}

static void spaceman_timer_cb(lv_timer_t *timer)
{
    if (!global_phys_canvas) return;

    // Draw current frame (Opaque to clean last frame)
    blit_i1_to_canvas_opaque(global_phys_canvas, spaceman_flip_images[animation_frame], 24, -18);
    lv_obj_invalidate(global_phys_canvas);

    // Advance frame
    animation_frame = (animation_frame + 1) % SPACEMAN_FLIP_IMAGES_NUM_IMAGES;
}

void draw_animation(lv_obj_t *phys_canvas)
{
    global_phys_canvas = phys_canvas;

    if (anim_timer == NULL) {
        anim_timer = lv_timer_create(anim_timer_cb, 60, NULL);
    }
}

void draw_spaceman(lv_obj_t *phys_canvas)
{
    global_phys_canvas = phys_canvas;

    if (anim_timer == NULL) {
        anim_timer = lv_timer_create(spaceman_timer_cb, 60, NULL);
    }
}