#include "util.h"
#include <zephyr/kernel.h>

void rotate_canvas(lv_obj_t *phys_canvas, lv_obj_t *logic_canvas) {
    // logic_canvas is 128x128 logical square
    // phys_canvas is 128x32 physical
    uint8_t *src_pixels = ((uint8_t *)lv_canvas_get_draw_buf(logic_canvas)->data) + 8;
    uint8_t *dst_pixels = ((uint8_t *)lv_canvas_get_draw_buf(phys_canvas)->data) + 8;
    
    // Clear destination pixels to white (unlit)
    memset(dst_pixels, 0xFF, 512);

    for (int y = 0; y < 128; y++) {
        for (int x = 0; x < 32; x++) {
            // Logical 32x128 slice within the 128x128 src buffer
            // Each row in src is 16 bytes (128 bits)
            int src_byte = (y * 16) + (x / 8);
            int src_bit = 7 - (x % 8);
            
            // Check if bit is 0 (Lit in logic canvas)
            if (!(src_pixels[src_byte] & (1 << src_bit))) {
                // Pixel is LIT. Rotate 90 deg clockwise.
                int px = 127 - y;
                int py = x;
                
                // Physical row is 128 bits (16 bytes)
                int dst_byte = (py * 16) + (px / 8);
                int dst_bit = 7 - (px % 8);
                dst_pixels[dst_byte] &= ~(1 << dst_bit);
            }
        }
    }
}

void draw_background(lv_obj_t *canvas) {
  lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER);
}
