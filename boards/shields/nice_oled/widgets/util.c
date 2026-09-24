#include "util.h"
#include <zephyr/kernel.h>

#include <zephyr/devicetree.h>
#include <zephyr/drivers/sensor.h>
#include <zmk/battery.h>

extern const lv_font_t pixel_operator_mono_8;
extern const lv_font_t pixel_operator_mono_12;
extern const lv_font_t pixel_operator_mono;

void rotate_canvas(lv_obj_t *phys_canvas, lv_obj_t *logic_canvas)
{
    lv_draw_buf_t *src_buf = lv_canvas_get_draw_buf(logic_canvas);
    lv_draw_buf_t *dst_buf = lv_canvas_get_draw_buf(phys_canvas);

    // In LVGL 9, the palette is at the START of the data buffer for indexed formats.
    // I1 has 2 palette entries of lv_color32_t (4 bytes each) = 8 bytes.
    uint8_t *src_pixels = ((uint8_t *)src_buf->data) + 8;
    uint8_t *dst_pixels = ((uint8_t *)dst_buf->data) + 8;

    uint32_t src_stride = src_buf->header.stride;
    uint32_t dst_stride = dst_buf->header.stride;

    // Clear destination to Index 1 (White background)
    // Index 1 is represented by setting bits to 1.
    for (int y = 0; y < 32; y++) {
        memset(dst_pixels + (y * dst_stride), 0xFF, dst_stride);
    }

    for (int y = 0; y < 128; y++) {
        for (int x = 0; x < 32; x++) {
            int src_byte = (y * src_stride) + (x / 8);
            int src_bit = 7 - (x % 8);

            // Check if source bit is 0 (Index 0 / Black)
            if (!(src_pixels[src_byte] & (1 << src_bit))) {
                // Pixel is Black. Rotate 90 deg clockwise.
                int px = 127 - y;
                int py = x;

                int dst_byte = (py * dst_stride) + (px / 8);
                int dst_bit = 7 - (px % 8);

                // Set the destination bit to 0 (Index 0 / Black)
                dst_pixels[dst_byte] &= ~(1 << dst_bit);
            }
        }
    }
}

void _draw_text(lv_obj_t *target_canvas, int x, int y, const char *text, bool small)
{
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_color_make(0, 0, 0); // Black text (Index 0)
    label_dsc.font = small ? &pixel_operator_mono_8 : &pixel_operator_mono_12;

    lv_layer_t layer;
    lv_canvas_init_layer(target_canvas, &layer);

    // Width is exactly 32px. Limit height to prevent wrapping.
    lv_area_t area = {x, y, CANVAS_WIDTH - 1, y + 10};
    label_dsc.text = text;
    lv_draw_label(&layer, &label_dsc, &area);

    lv_canvas_finish_layer(target_canvas, &layer);
}

void draw_text_big(lv_obj_t *target_canvas, int x, int y, const char *text)
{
    lv_draw_label_dsc_t label_dsc;
    lv_draw_label_dsc_init(&label_dsc);
    label_dsc.color = lv_color_make(0, 0, 0); // Black text (Index 0)
    label_dsc.font = &pixel_operator_mono;

    lv_layer_t layer;
    lv_canvas_init_layer(target_canvas, &layer);

    // Width is exactly 32px. Limit height to prevent wrapping.
    lv_area_t area = {x, y, CANVAS_WIDTH - 1, y + 14};
    label_dsc.text = text;
    lv_draw_label(&layer, &label_dsc, &area);

    lv_canvas_finish_layer(target_canvas, &layer);
}

void draw_img(lv_obj_t *target_canvas, int x, int y, const lv_img_dsc_t *img_dsc)
{
    lv_draw_image_dsc_t image_dsc;
    lv_draw_image_dsc_init(&image_dsc);

    lv_layer_t layer;
    lv_canvas_init_layer(target_canvas, &layer);

    lv_area_t area = {x, y, x + img_dsc->header.w - 1, y + img_dsc->header.h - 1};
    image_dsc.src = img_dsc;
    lv_draw_image(&layer, &image_dsc, &area);
    lv_canvas_finish_layer(target_canvas, &layer);
}

void draw_background(lv_obj_t *canvas) { lv_canvas_fill_bg(canvas, LVGL_BACKGROUND, LV_OPA_COVER); }

uint8_t get_natural_battery_level(void)
{
    const struct device *batt_dev = DEVICE_DT_GET_OR_NULL(DT_CHOSEN(zmk_battery));
    if (batt_dev == NULL || !device_is_ready(batt_dev)) return zmk_battery_state_of_charge();

    struct sensor_value voltage;
    if (sensor_sample_fetch_chan(batt_dev, SENSOR_CHAN_GAUGE_VOLTAGE) != 0) {
        return zmk_battery_state_of_charge();
    }
    sensor_channel_get(batt_dev, SENSOR_CHAN_GAUGE_VOLTAGE, &voltage);

    int16_t mv = voltage.val1 * 1000 + (voltage.val2 / 1000);

    if (mv >= 4150) return 100;
    if (mv >= 4000) return 90 + (mv - 4000) * 10 / 150;
    if (mv >= 3700) return 50 + (mv - 3700) * 40 / 300;
    if (mv >= 3500) return 10 + (mv - 3500) * 40 / 200;
    if (mv >= 3400) return (mv - 3400) * 10 / 100;
    return 0;
}
