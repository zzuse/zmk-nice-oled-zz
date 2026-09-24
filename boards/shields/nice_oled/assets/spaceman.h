#ifndef _5_OUTPUT_IMAGES_ROTATE_FLIP_H
#define _5_OUTPUT_IMAGES_ROTATE_FLIP_H
#ifdef __cplusplus
extern "C" {
#endif
#include "lvgl.h"
// -------- Image Descriptors Declarations --------
LV_IMG_DECLARE(spaceman_00);
LV_IMG_DECLARE(spaceman_01);
LV_IMG_DECLARE(spaceman_02);
LV_IMG_DECLARE(spaceman_03);
LV_IMG_DECLARE(spaceman_04);
LV_IMG_DECLARE(spaceman_05);
LV_IMG_DECLARE(spaceman_06);
LV_IMG_DECLARE(spaceman_07);
LV_IMG_DECLARE(spaceman_08);
LV_IMG_DECLARE(spaceman_09);
LV_IMG_DECLARE(spaceman_10);
LV_IMG_DECLARE(spaceman_11);
LV_IMG_DECLARE(spaceman_12);
LV_IMG_DECLARE(spaceman_13);
LV_IMG_DECLARE(spaceman_14);
LV_IMG_DECLARE(spaceman_15);
LV_IMG_DECLARE(spaceman_16);
LV_IMG_DECLARE(spaceman_17);
LV_IMG_DECLARE(spaceman_18);
LV_IMG_DECLARE(spaceman_19);

// -------- Array of Pointers to Image Descriptors --------
// Provides easy access to all images defined in the corresponding .c file
#define SPACEMAN_FRAME_COUNT 20
extern const lv_img_dsc_t *spaceman_flip_images[SPACEMAN_FRAME_COUNT];

#ifdef __cplusplus
} /* extern "C" */
#endif
#endif /* _5_OUTPUT_IMAGES_ROTATE_FLIP_H */
