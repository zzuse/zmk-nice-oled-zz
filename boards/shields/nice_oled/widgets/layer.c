#include "layer.h"
#include "../assets/custom_fonts.h"
#include <ctype.h> // Para toupper()
#include <zephyr/kernel.h>

// MC: better implementation
void draw_layer_status(lv_obj_t *canvas, const struct status_state *state) {
  lv_draw_label_dsc_t label_dsc;
  init_label_dsc(&label_dsc, LVGL_FOREGROUND, &pixel_operator_mono,
                 LV_TEXT_ALIGN_LEFT);

  char text[14] = {};
  int result;

  if (state->layer_label == NULL) {
    result = snprintf(text, sizeof(text), "Layer %i", state->layer_index);
  } else {
    result = snprintf(text, sizeof(text), "%s", state->layer_label);
    for (int i = 0; text[i] != '\0'; i++) {
      // toupper( ... ): This function, found in the ctype.h library, takes a
      // character as an argument and converts it to its uppercase equivalent.
      // If the character is already uppercase or not a letter, the function
      // returns it unchanged.
      text[i] = toupper(text[i]);
    }
  }

  if (result >= sizeof(text)) {
    LV_LOG_WARN("truncated");
  }

  lv_canvas_draw_text(canvas, 0, 32, 68, &label_dsc, text);
}
