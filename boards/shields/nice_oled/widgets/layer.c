#include "layer.h"
#include <zephyr/kernel.h>

void draw_layer_status(lv_obj_t *canvas, const struct status_state *state, int draw_count, int x, int y)
{
    char buf[16];
    snprintf(buf, sizeof(buf), "%s%s", state->layer_label, (draw_count % 2) ? "*" : " ");
    draw_text(canvas, x, y, buf);
}
