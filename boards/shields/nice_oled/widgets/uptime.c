#include "uptime.h"
#include <zephyr/kernel.h>

void draw_uptime(lv_obj_t *canvas, const struct status_state *state, int x, int y)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%u", state->uptime_seconds % 100000);
    // draw_text(canvas, x, y, "UPTIME:");
    draw_text(canvas, x, y, buf);
}
