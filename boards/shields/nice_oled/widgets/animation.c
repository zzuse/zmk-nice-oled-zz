#include "animation.h"
#include "../assets/crystal.h"
#include "../assets/spaceman.h"
#include <zephyr/kernel.h>

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

#define ANIM_X 24
#define ANIM_Y -18
#define ANIM_PERIOD_MS 60

static const lv_img_dsc_t *const *anim_frames = NULL;
static int anim_frame_count = 0;

static void anim_timer_cb(lv_timer_t *timer)
{
    if (!global_phys_canvas || !anim_frames) return;

    // Advance, then draw (Opaque to clean last frame)
    animation_frame = (animation_frame + 1) % anim_frame_count;
    const lv_img_dsc_t *img = anim_frames[animation_frame];
    blit_i1_to_canvas_opaque(global_phys_canvas, img, ANIM_X, ANIM_Y);

    // Only flush the columns the animation covers, not the whole screen (LVGL clips to the canvas)
    lv_area_t area;
    lv_obj_get_coords(global_phys_canvas, &area);
    area.x1 += ANIM_X;
    area.x2 = area.x1 + img->header.w - 1;
    lv_obj_invalidate_area(global_phys_canvas, &area);
}

static void start_animation(lv_obj_t *phys_canvas, const lv_img_dsc_t *const *frames, int frame_count)
{
    global_phys_canvas = phys_canvas;
    anim_frames = frames;
    anim_frame_count = frame_count;

    // rotate_canvas() just wiped the whole canvas; restore the current frame right away
    // instead of leaving the area blank until the next timer tick (visible 1 Hz stutter).
    blit_i1_to_canvas_opaque(phys_canvas, anim_frames[animation_frame], ANIM_X, ANIM_Y);

    if (anim_timer == NULL) {
        anim_timer = lv_timer_create(anim_timer_cb, ANIM_PERIOD_MS, NULL);
    }
}

void draw_animation(lv_obj_t *phys_canvas) { start_animation(phys_canvas, crystal_imgs, CRYSTAL_FRAME_COUNT); }

void draw_spaceman(lv_obj_t *phys_canvas)
{
    start_animation(phys_canvas, spaceman_flip_images, SPACEMAN_FRAME_COUNT);
}