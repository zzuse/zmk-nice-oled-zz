# Nice OLED Widget Layout Design

## Display Specs

- **Physical OLED**: 128 wide × 32 tall
- **Logical canvas buffer**: 68 wide × 160 tall (inside a 160×160 buffer)
- **Font**: `pixel_operator_mono_8` — line_height = **7px**

## Rotation: `rotate_canvas()`

```c
lv_canvas_transform(canvas, &img, 900, LV_IMG_ZOOM_NONE, -1, 0,
                    CANVAS_HEIGHT / 2, CANVAS_HEIGHT / 2, false);
```

The drawing functions paint into a vertical 68×160 logical buffer.  
`rotate_canvas()` rotates this 90° (CCW in LVGL) around pivot `(80, 80)` to produce the horizontal output.

**Coordinate transform (after rotation + offset):**

```
Physical_X = 159 − Logical_Y
Physical_Y = Logical_X
```

---

## Why offset is (−1, 0)

The rotation pivot is `(80, 80)` — center of the 160×160 buffer.  
Pure 90° rotation around this pivot yields:

```
Physical_X = 160 − Logical_Y
```

For content at **Logical Y = 0**, that gives `Physical_X = 160`.  
But the valid pixel range for a 160-wide buffer is **0 to 159**.  
Pixel 160 is **1 pixel off the right edge** — out of bounds.

The `offset_x = -1` shifts everything left by 1 pixel:

```
Physical_X = 160 − Logical_Y − 1 = 159 − Logical_Y
```

| Logical Y | Without offset | With offset (−1) |
|:----------|:---------------|:-----------------|
| 0         | 160 ❌ (OOB)    | 159 ✅            |
| 159       | 1              | 0 ✅              |

**TL;DR**: Classic fence-post correction. The 160×160 buffer has pixels 0–159,  
but pure rotation yields 1–160. The `−1` nudges it into the valid range.

---

## Why Layer starts at Logical Y = 32

The physical OLED is only **128 pixels wide** (Physical X: 0–127).  
Content must land within this range after rotation.

Using `Physical_X = 159 − Logical_Y`:

| Logical Y | Physical X | Visible? (0–127) |
|:----------|:-----------|:-----------------|
| 31        | 128        | ❌ Off-screen     |
| **32**    | **127**    | ✅ Rightmost pixel |
| 159       | 0          | ✅ Leftmost pixel  |

Anything at **Logical Y < 32** maps to Physical X > 127, which is **off the 128px display**.

Therefore the usable logical canvas is:
- **Logical X**: 0 to 31 → Physical Y: 0 to 31 (32px height)
- **Logical Y**: 32 to 159 → Physical X: 127 to 0 (128px width)

---

## Conversion Formulas

### For `lv_canvas_draw_text(canvas, LX, LY, MW, &dsc, text)` with font height `FH`:

```
Physical X start (left edge)  = 160 − LY − FH
Physical X end   (right edge) = 159 − LY
Physical Y start (top)        = LX
Physical Y end   (bottom)     = LX + MW − 1
Physical width                = FH
Physical height               = MW
```

**Shortcut** — to place text at a desired Physical X start with 7px font:

```
LY = 153 − Physical_X_start
```

### For `lv_canvas_draw_img(canvas, LX, LY, &img)` with image size `(W, H)`:

```
Physical X start = 160 − LY − H
Physical X end   = 159 − LY
Physical Y start = LX
Physical Y end   = LX + W − 1
Physical width   = H   (image height becomes physical width)
Physical height  = W   (image width becomes physical height)
```

### For LVGL object overlays (Luna, Crystal):

These are **NOT rotated**. Coordinates are already in physical space.

```c
lv_obj_align(obj, LV_ALIGN_TOP_LEFT, physical_x, physical_y);
```

---

## Asset Sizes

| Asset            | Logical W × H | Physical W × H (after rotation) | Source           |
|:-----------------|:---------------|:---------------------------------|:-----------------|
| `bolt`           | 5 × 9          | 9 × 5                           | `images.c`       |
| `bt`             | 12 × 15        | 15 × 12                         | `images.c`       |
| `bt_no_signal`   | 12 × 15        | 15 × 12                         | `images.c`       |
| `bt_unbonded`    | 22 × 15        | 15 × 22                         | `images.c`       |
| `usb`            | 20 × 11        | 11 × 20                         | `images.c`       |
| `dog_sit1_90`    | 24 × 32        | N/A (LVGL obj, not rotated)      | `luna_images.c`  |
| `crystal_XX`     | 69 × 68        | N/A (LVGL obj, not rotated)      | `crystal.c`      |

---

## Final Layout Design

### Physical view (128 × 32)

```
X:  0    7  9   23 24                  92 93  99100 106107 113114 120121 127
    ┌────┬──┬────┬──────────────────────┬──┬────┬────┬─────┬─────┬─────┐
    │BAT%│⚡│ BT │    CRYSTAL GEM       │  │ ID │KEY │ UP: │9999 │LAYER│
    │ 7  │  │ 15 │    69px overlay      │  │ 7  │ 7  │  7  │  7  │  7  │
    └────┴──┴────┴──────────────────────┴──┴────┴────┴─────┴─────┴─────┘
    ├─────────────────────────────────────┤├─────────────────────────────┤
              Left (status/art)                  Right (text info)
```

### Coordinate table

| Widget             | Phys X    | W  | Code                                          | Logical (LX, LY, MW)   |
|:-------------------|:----------|:---|:----------------------------------------------|:------------------------|
| Battery %          | 0–6       | 7  | `draw_text(0, 153, 26, &dsc_8, bat_text)`     | (0, 153, 26)            |
| Bolt ⚡ (charging)  | 0–8       | 9  | `draw_img(26, 151, &bolt)`                    | (26, 151)               |
| BT connected       | 9–23      | 15 | `draw_img(0, 136, &bt)`                       | (0, 136)                |
| BT disconnected    | 9–23      | 15 | `draw_img(0, 136, &bt_no_signal)`             | (0, 136)                |
| BT unbonded        | 9–23      | 15 | `draw_img(0, 136, &bt_unbonded)`              | (0, 136)                |
| USB                | 11–21     | 11 | `draw_img(0, 138, &usb)`                      | (0, 138)                |
| **Crystal GEM**    | 24–92     | 69 | `lv_obj_align(art, ..., 24, -18)`             | N/A (LVGL overlay)      |
| Keycode ID         | 93–99     | 7  | `draw_text(0, 60, 32, &dsc_8, id_text)`       | (0, 60, 32)             |
| Keycode Name       | 100–106   | 7  | `draw_text(0, 53, 32, &dsc_8, key_text)`      | (0, 53, 32)             |
| Uptime Label       | 107–113   | 7  | `draw_text(0, 46, 32, &dsc_8, "UP: ")`        | (0, 46, 32)             |
| Uptime Value       | 114–120   | 7  | `draw_text(0, 39, 32, &dsc_8, up_text)`       | (0, 39, 32)             |
| Layer Name         | 121–127   | 7  | `draw_text(0, 32, 32, &dsc_8, layer_text)`    | (0, 32, 32)             |

### Notes

- All text uses `pixel_operator_mono_8` (7px line height) to maximize available space.
- The Crystal GEM (69×68) is vertically centered with `y=-18`, showing the middle 32 rows.
- The right side packs 5 text bands (5 × 7 = 35px) perfectly into X:93–127.
- Battery text is limited to `MW=26` to leave room for the bolt icon below it.
- Text drawn in the pixel buffer appears **rotated 90°** on the physical screen (reads top-to-bottom).
- LVGL overlays (Crystal, Luna) use **pre-rotated** assets (`_90` suffix) and are positioned in physical coordinates.

### Conversion Formula Cheat Sheet
For any widget you want at Physical X start = PX using 7px font:
```
Logical_Y = 153 − PX
```
For an image with height H at Physical X start = PX:
```
Logical_Y = 160 − PX − H
```