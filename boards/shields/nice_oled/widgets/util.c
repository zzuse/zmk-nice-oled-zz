#include "util.h"
#include <zephyr/kernel.h>

void rotate_canvas(lv_obj_t *phys_canvas, lv_obj_t *logic_canvas) {
    // logic_canvas is 128x128 logical square
    // phys_canvas is 128x32 physical
    lv_draw_buf_t *src_buf = lv_canvas_get_draw_buf(logic_canvas);
    lv_draw_buf_t *dst_buf = lv_canvas_get_draw_buf(phys_canvas);
    uint8_t *src_pixels = ((uint8_t *)src_buf->data) + 8; // skip 8 byte palette
    uint8_t *dst_pixels = ((uint8_t *)dst_buf->data) + 8; // skip 8 byte palette
    
    uint32_t src_stride = src_buf->header.stride;
    uint32_t dst_stride = dst_buf->header.stride;

    // Clear destination pixels to white (unlit) - careful with stride!
    for (int y = 0; y < 32; y++) {
        memset(dst_pixels + (y * dst_stride), 0xFF, dst_stride);
    }

    for (int y = 0; y < 128; y++) {
        for (int x = 0; x < 32; x++) {
            // Logical 32x128 slice within the 128x128 src buffer
            int src_byte = (y * src_stride) + (x / 8);
            int src_bit = 7 - (x % 8);
            
            // Check if bit is 0 (Lit in logic canvas)
            if (!(src_pixels[src_byte] & (1 << src_bit))) {
                // Pixel is LIT. Rotate 90 deg clockwise.
                int px = 127 - y;
                int py = x;
                
                int dst_byte = (py * dst_stride) + (px / 8);
                int dst_bit = 7 - (px % 8);
                dst_pixels[dst_byte] &= ~(1 << dst_bit);
            }
        }
    }
    lv_obj_invalidate(phys_canvas);
}

void draw_background(lv_obj_t *canvas) {
  lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);
}
