#include "battery.h"
#include <zephyr/kernel.h>

LV_IMG_DECLARE(bolt);

void draw_battery_status(lv_obj_t *canvas, const struct status_state *state, int x, int y)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%d%%", state->battery);
    draw_text(canvas, x, y, buf);
    if (state->charging) {
        draw_img(canvas, 24, y, &bolt);
    }
}
