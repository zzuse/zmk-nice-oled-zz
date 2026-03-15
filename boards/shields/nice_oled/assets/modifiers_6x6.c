/*
 * Copyright (c) 2024 The ZMK Contributors
 *
 * SPDX-License-Identifier: MIT
 *
 * Modifier symbols - Normal orientation (facing up ^)
 * Contains: control, shift, alt, win, cmd, opt (normal and white/inverted versions)
 */

#include <lvgl.h>

#ifndef LV_ATTRIBUTE_MEM_ALIGN
#define LV_ATTRIBUTE_MEM_ALIGN
#endif

#ifndef LV_ATTRIBUTE_LARGE_CONST
#define LV_ATTRIBUTE_LARGE_CONST
#endif

/* ==================== CONTROL ==================== */
#ifndef LV_ATTRIBUTE_IMG_CONTROL_0
#define LV_ATTRIBUTE_IMG_CONTROL_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_CONTROL_0 uint8_t control_0_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#else
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#endif
    0x30, 0x48, 0x84, 0x00, 0x00, 0x00, // 6 bytes for 6x6 image (1 byte per row)
};

const lv_img_dsc_t control_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = control_0_map,
};

#ifndef LV_ATTRIBUTE_IMG_CONTROL_WHITE_0
#define LV_ATTRIBUTE_IMG_CONTROL_WHITE_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_CONTROL_WHITE_0 uint8_t
    control_white_0_map[]
    = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
        0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#else
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#endif
        0x30, 0x48, 0x84, 0x00, 0x00, 0x00,
};

const lv_img_dsc_t control_white_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = control_white_0_map,
};

/* ==================== SHIFT ==================== */
#ifndef LV_ATTRIBUTE_IMG_SHIFT_0
#define LV_ATTRIBUTE_IMG_SHIFT_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_SHIFT_0 uint8_t shift_0_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#else
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#endif
    0x30, 0x48, 0x84, 0x30, 0x30, 0x30, // 6 bytes for 6x6
};

const lv_img_dsc_t shift_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = shift_0_map,
};

#ifndef LV_ATTRIBUTE_IMG_SHIFT_WHITE_0
#define LV_ATTRIBUTE_IMG_SHIFT_WHITE_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_SHIFT_WHITE_0 uint8_t shift_white_0_map[]
    = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
        0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#else
        0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#endif
        0x30, 0x48, 0x84, 0x30, 0x30, 0x30,
};

const lv_img_dsc_t shift_white_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = shift_white_0_map,
};

/* ==================== ALT (Windows) ==================== */
#ifndef LV_ATTRIBUTE_IMG_ALT_0
#define LV_ATTRIBUTE_IMG_ALT_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_ALT_0 uint8_t alt_0_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#else
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#endif
    0xCC, 0x18, 0x30, 0x60, 0xC0, 0x00, // 6 bytes for 6x6
};

const lv_img_dsc_t alt_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = alt_0_map,
};

#ifndef LV_ATTRIBUTE_IMG_ALT_WHITE_0
#define LV_ATTRIBUTE_IMG_ALT_WHITE_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_ALT_WHITE_0 uint8_t alt_white_0_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#else
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#endif
    0xCC, 0x18, 0x30, 0x60, 0xC0, 0x00,
};

const lv_img_dsc_t alt_white_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = alt_white_0_map,
};

/* ==================== WIN (Windows logo) ==================== */
#ifndef LV_ATTRIBUTE_IMG_WIN_0
#define LV_ATTRIBUTE_IMG_WIN_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_WIN_0 uint8_t win_0_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#else
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#endif
    0xD8, 0xD8, 0x00, 0xD8, 0xD8, 0x00, // 6 bytes for 6x6
};

const lv_img_dsc_t win_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = win_0_map,
};

#ifndef LV_ATTRIBUTE_IMG_WIN_WHITE_0
#define LV_ATTRIBUTE_IMG_WIN_WHITE_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_WIN_WHITE_0 uint8_t win_white_0_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#else
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#endif
    0xD8, 0xD8, 0x00, 0xD8, 0xD8, 0x00,
};

const lv_img_dsc_t win_white_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = win_white_0_map,
};

/* ==================== CMD (macOS Command) ==================== */
#ifndef LV_ATTRIBUTE_IMG_CMD_0
#define LV_ATTRIBUTE_IMG_CMD_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_CMD_0 uint8_t cmd_0_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#else
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#endif
    0x48, 0xB4, 0x48, 0x48, 0xB4, 0x48, // 6 bytes for 6x6
};

const lv_img_dsc_t cmd_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = cmd_0_map,
};

#ifndef LV_ATTRIBUTE_IMG_CMD_WHITE_0
#define LV_ATTRIBUTE_IMG_CMD_WHITE_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_CMD_WHITE_0 uint8_t cmd_white_0_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#else
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#endif
    0x48, 0xB4, 0x48, 0x48, 0xB4, 0x48,
};

const lv_img_dsc_t cmd_white_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = cmd_white_0_map,
};

/* ==================== OPT (macOS Option) ==================== */
#ifndef LV_ATTRIBUTE_IMG_OPT_0
#define LV_ATTRIBUTE_IMG_OPT_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_OPT_0 uint8_t opt_0_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#else
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#endif
    0xCC, 0x18, 0x30, 0x60, 0xC0, 0x00, // 6 bytes for 6x6
};

const lv_img_dsc_t opt_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = opt_0_map,
};

#ifndef LV_ATTRIBUTE_IMG_OPT_WHITE_0
#define LV_ATTRIBUTE_IMG_OPT_WHITE_0
#endif

static const LV_ATTRIBUTE_MEM_ALIGN LV_ATTRIBUTE_LARGE_CONST LV_ATTRIBUTE_IMG_OPT_WHITE_0 uint8_t opt_white_0_map[] = {
#if CONFIG_NICE_OLED_WIDGET_INVERTED
    0xff, 0xff, 0xff, 0xff, 0x00, 0x00, 0x00, 0xff,
#else
    0x00, 0x00, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff,
#endif
    0xCC, 0x18, 0x30, 0x60, 0xC0, 0x00,
};

const lv_img_dsc_t opt_white_0 = {
    .header.cf = LV_COLOR_FORMAT_I1,

    .header.w = 6,
    .header.h = 6,
    .data_size = 14,
    .data = opt_white_0_map,
};
