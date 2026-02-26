#include "util.h"
#include <ctype.h>
#include <zephyr/kernel.h>

void to_uppercase(char *str) {
  for (int i = 0; str[i] != '\0'; i++) {
    str[i] = toupper(str[i]);
  }
}

// Manual bit-level rotation for monochrome displays
// Rotates a 32x128 vertical logical buffer to a 128x32 horizontal physical buffer
void rotate_canvas(lv_obj_t *canvas, uint8_t logic_buffer[]) {
    uint8_t *dst = (uint8_t *)lv_canvas_get_draw_buf(canvas)->data;
    
    // Clear destination to unlit (White in your inverted setup)
    memset(dst, 0xFF, 512);

    for (int y = 0; y < 128; y++) {
        for (int x = 0; x < 32; x++) {
            // Check bit in 32x128 logic_buffer
            // Logic buffer is 4 bytes wide (32 bits), 128 rows high
            int src_idx = (y * 4) + (x / 8);
            int src_bit = 7 - (x % 8);
            
            // If bit is 0 (Black/Lit in logic), we set it in destination
            if (!(logic_buffer[src_idx] & (1 << src_bit))) {
                // Pixel is LIT. Rotate 90 deg clockwise.
                // (lx, ly) -> (px, py)
                // lx: 0..31, ly: 0..127
                // px: 127 - ly, py: lx
                int px = 127 - y;
                int py = x;
                
                // Physical buffer is 16 bytes wide (128 bits), 32 rows high
                int dst_idx = (py * 16) + (px / 8);
                int dst_bit = 7 - (px % 8);
                dst[dst_idx] &= ~(1 << dst_bit); // Set bit to 0 (Lit)
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

void init_rect_dsc(lv_draw_rect_dsc_t *rect_dsc, lv_color_t bg_color) {
  lv_draw_rect_dsc_init(rect_dsc);
  rect_dsc->bg_color = bg_color;
}

void canvas_draw_text(lv_obj_t *canvas, lv_coord_t x, lv_coord_t y, lv_coord_t max_w,
                      lv_draw_label_dsc_t *draw_dsc, const char *txt) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    draw_dsc->text = txt;
    lv_area_t coords = {x, y, x + max_w - 1, y + 32}; 
    lv_draw_label(&layer, draw_dsc, &coords);
    lv_canvas_finish_layer(canvas, &layer);
}

void canvas_draw_img(lv_obj_t *canvas, lv_coord_t x, lv_coord_t y, const lv_image_dsc_t *src,
                     lv_draw_image_dsc_t *draw_dsc) {
    lv_layer_t layer;
    lv_canvas_init_layer(canvas, &layer);
    draw_dsc->src = src;
    lv_area_t coords = {x, y, x + src->header.w - 1, y + src->header.h - 1};
    lv_draw_image(&layer, draw_dsc, &coords);
    lv_canvas_finish_layer(canvas, &layer);
}
