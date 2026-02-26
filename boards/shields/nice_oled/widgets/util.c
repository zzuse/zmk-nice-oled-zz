#include "util.h"
#include <zephyr/kernel.h>

void rotate_canvas(lv_obj_t *canvas, lv_color_t cbuf[]) {
    // cbuf is 128x128 logical square including 8-byte palette
    // target is 128x32 physical including 8-byte palette
    uint8_t *src_pixels = ((uint8_t *)cbuf) + 8;
    uint8_t *dst_pixels = ((uint8_t *)lv_canvas_get_draw_buf(canvas)->data) + 8;
    
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
  lv_canvas_fill_bg(canvas, lv_color_white(), LV_OPA_COVER);
}

void init_label_dsc(lv_draw_label_dsc_t *label_dsc, lv_color_t color,
                    const lv_font_t *font, lv_text_align_t align) {
  lv_draw_label_dsc_init(label_dsc);
  label_dsc->color = color;
  label_dsc->font = font;
  label_dsc->align = align;
}
