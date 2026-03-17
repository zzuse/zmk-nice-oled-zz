# Nice OLED Widget Layout Analysis

## How The Pipeline Works

### Step 1: Logical Canvas (68×160 vertical buffer)
All `draw_xxx()` functions paint into a **68-wide × 160-tall** vertical strip inside a 160×160 pixel buffer.

### Step 2: `rotate_canvas()` — 90° CCW Rotation
```c
lv_canvas_transform(canvas, &img, 900, LV_IMG_ZOOM_NONE, -1, 0,
                    CANVAS_HEIGHT / 2, CANVAS_HEIGHT / 2, false);
```
- Angle `900` = 90.0° (in 0.1° units) counter-clockwise in LVGL
- Pivot: `(80, 80)` — center of the 160×160 buffer
- Offset: `(-1, 0)`

**Resulting coordinate transform:**
- **Physical X = 159 − Logical Y**
- **Physical Y = Logical X**

The 68×160 strip maps perfectly onto the **160×68** visible screen.

### Step 3: LVGL Object Overlays
Luna and Crystal are `lv_obj_t` children of the canvas — they are **NOT** rotated. They use physical screen coordinates directly. Their image assets are **pre-rotated** (hence the `_90` suffix).

---

## Asset Sizes (from source code)

| Asset | Width | Height | Source |
|:---|:---|:---|:---|
| `bolt` | 5 | 9 | `images.c` |
| `bt` | 12 | 15 | `images.c` |
| `bt_no_signal` | 12 | 15 | `images.c` |
| `bt_unbonded` | 22 | 15 | `images.c` |
| `usb` | 20 | 11 | `images.c` |
| `dog_sit1_90` | 24 | 32 | `luna_images.c` |
| `crystal_XX` | 69 | 68 | `crystal.c` |
| Font line height | — | 13 | `pixel_operator_mono.c` |

---

## Coordinate Table

### Pixel Buffer Layer (drawn → rotated)

| Widget | Code Call | Logical (X, Y, W) | Physical (X, Y) | Physical Size |
|:---|:---|:---|:---|:---|
| **Layer Label** | `draw_text(0, 32, 68)` | (0, 32, w=68) | **(127, 0)** | 13w × 68h |
| **Keycode ID** | `draw_text(0, 50, 32)` | (0, 50, w=32) | **(109, 0)** | 13w × 32h |
| **Keycode Name** | `draw_text(0, 66, 32)` | (0, 66, w=32) | **(93, 0)** | 13w × 32h |
| **BT Connected** | `draw_img(4, 130, &bt)` | (4, 130) | **(29, 4)** | 15w × 12h |
| **BT Disconnected** | `draw_img(4, 130, &bt_no_signal)` | (4, 130) | **(29, 4)** | 15w × 12h |
| **BT Unbonded** | `draw_img(-1, 130, &bt_unbonded)` | (-1, 130) | **(29, -1)** | 15w × 22h |
| **USB** | `draw_img(0, 130, &usb)` | (0, 130) | **(29, 0)** | 11w × 20h |
| **Battery %** | `draw_text(0, 146, 42)` | (0, 146, w=42) | **(13, 0)** | 13w × 42h |
| **Battery % (chrg)** | `draw_text(0, 146, 35)` | (0, 146, w=35) | **(13, 0)** | 13w × 35h |
| **Bolt (chrg)** | `draw_img(25, 146, &bolt)` | (25, 146) | **(13, 25)** | 9w × 5h |

### LVGL Object Overlay Layer (NOT rotated)

| Widget | Code Call | Physical (X, Y) | Physical Size |
|:---|:---|:---|:---|
| **Luna (Dog)** | `lv_obj_align(..., 36, 0)` | **(36, 0)** | 24w × 32h |
| **Crystal Gem** | `lv_obj_align(art, ..., 18, -18)` | **(18, −18)** | 69w × 68h |

> [!IMPORTANT]
> After rotation, the **image width/height swap**: a logical image drawn at `(x, y)` with size `(w, h)` ends up at physical `(159-y, x)` with size `(h, w)`.
> Text drawn left-to-right in the logical canvas appears **top-to-bottom** on the physical screen.

---

## SVG: Logical Canvas (Pre-Rotation, 68×160)

This is what `draw_canvas()` paints BEFORE `rotate_canvas()` runs:

```xml
<svg viewBox="-2 -2 72 164" width="216" height="492" xmlns="http://www.w3.org/2000/svg" style="font-family: monospace;">
  <!-- Canvas boundary -->
  <rect x="0" y="0" width="68" height="160" fill="#111" stroke="#666" stroke-width="1"/>

  <!-- Y=0-32: Luna zone (right half) -->
  <rect x="36" y="0" width="32" height="32" fill="none" stroke="#888" stroke-dasharray="2"/>
  <text x="52" y="18" fill="#888" font-size="6" text-anchor="middle">(Luna lives</text>
  <text x="52" y="26" fill="#888" font-size="6" text-anchor="middle">as LVGL obj)</text>

  <!-- Y=32: Layer Label -->
  <rect x="0" y="32" width="68" height="13" fill="#1a3a1a" stroke="#4a4"/>
  <text x="34" y="42" fill="#4a4" font-size="8" text-anchor="middle">LAYER NAME</text>
  <text x="-1" y="40" fill="red" font-size="5">32</text>

  <!-- Y=50: Keycode ID -->
  <rect x="0" y="50" width="32" height="13" fill="#1a1a3a" stroke="#66f"/>
  <text x="16" y="60" fill="#66f" font-size="7" text-anchor="middle">KEY ID</text>
  <text x="-1" y="58" fill="red" font-size="5">50</text>

  <!-- Y=66: Keycode Name -->
  <rect x="0" y="66" width="32" height="13" fill="#1a1a3a" stroke="#88f"/>
  <text x="16" y="76" fill="#88f" font-size="7" text-anchor="middle">KEY NM</text>
  <text x="-1" y="74" fill="red" font-size="5">66</text>

  <!-- Y=82 to Y=130: EMPTY GAP -->
  <line x1="0" y1="82" x2="68" y2="82" stroke="#333" stroke-dasharray="1"/>
  <text x="34" y="108" fill="#333" font-size="7" text-anchor="middle">~ empty gap ~</text>
  <line x1="0" y1="130" x2="68" y2="130" stroke="#333" stroke-dasharray="1"/>

  <!-- Y=130: Output Icon (BT connected example) -->
  <rect x="4" y="130" width="12" height="15" fill="#0a0a2a" stroke="#44f"/>
  <text x="10" y="140" fill="#44f" font-size="6" text-anchor="middle">BT</text>
  <text x="-1" y="138" fill="red" font-size="5">130</text>

  <!-- Y=146: Battery % -->
  <rect x="0" y="146" width="42" height="13" fill="#2a1a0a" stroke="#fa0"/>
  <text x="12" y="156" fill="#fa0" font-size="8">80%</text>
  <text x="-1" y="154" fill="red" font-size="5">146</text>

  <!-- Y=146 + bolt at x=25 (charging) -->
  <rect x="25" y="146" width="5" height="9" fill="#ff0" opacity="0.4"/>
  <text x="27" y="153" fill="#ff0" font-size="4">⚡</text>

  <!-- Axis labels -->
  <text x="34" y="-1" fill="#aaa" font-size="5" text-anchor="middle">← 68px (CANVAS_WIDTH) →</text>
  <text x="70" y="80" fill="#aaa" font-size="5" transform="rotate(90, 70, 80)">← 160px (CANVAS_HEIGHT) →</text>
</svg>
```

---

## SVG: Physical Screen (Post-Rotation, 160×68, Final Result)

This is what you actually see on the nice!view OLED. Includes both the rotated pixel buffer AND LVGL object overlays.

> [!NOTE]
> After 90° CCW rotation, logical **Y-axis becomes inverted X-axis** (high logical Y → low physical X = left side).
> Text that was horizontal now reads **top-to-bottom** on the physical screen.

```xml
<svg viewBox="-2 -20 164 92" width="656" height="368" xmlns="http://www.w3.org/2000/svg" style="font-family: monospace;">
  <!-- Physical screen boundary 160x68 -->
  <rect x="0" y="0" width="160" height="68" fill="#111" stroke="#666" stroke-width="1"/>

  <!-- ═══════════════════════════════════════ -->
  <!-- LAYER 1: ROTATED PIXEL BUFFER           -->
  <!-- ═══════════════════════════════════════ -->

  <!-- Battery % text: logical(0,146,w=42) → physical(13, 0, 13w × 42h) -->
  <rect x="1" y="0" width="13" height="42" fill="#2a1a0a" stroke="#fa0" stroke-width="0.5"/>
  <text x="7" y="8" fill="#fa0" font-size="5" text-anchor="middle" writing-mode="tb">80%</text>

  <!-- Bolt icon: logical(25,146) → physical(13, 25, 9w × 5h) -->
  <rect x="9" y="25" width="9" height="5" fill="#ff0" opacity="0.5"/>
  <text x="13" y="29" fill="#ff0" font-size="4" text-anchor="middle">⚡</text>

  <!-- BT icon: logical(4,130,12w×15h) → physical(29, 4, 15w × 12h) -->
  <rect x="29" y="4" width="15" height="12" fill="#0a0a2a" stroke="#44f" stroke-width="0.5"/>
  <text x="36" y="12" fill="#44f" font-size="6" text-anchor="middle">BT</text>

  <!-- Keycode Name: logical(0,66,w=32) → physical(93, 0, 13w × 32h) -->
  <rect x="93" y="0" width="13" height="32" fill="#1a1a3a" stroke="#88f" stroke-width="0.5"/>
  <text x="99" y="6" fill="#88f" font-size="4" text-anchor="middle" writing-mode="tb">ENTER</text>

  <!-- Keycode ID: logical(0,50,w=32) → physical(109, 0, 13w × 32h) -->
  <rect x="109" y="0" width="13" height="32" fill="#1a1a3a" stroke="#66f" stroke-width="0.5"/>
  <text x="115" y="6" fill="#66f" font-size="4" text-anchor="middle" writing-mode="tb">40</text>

  <!-- Layer Label: logical(0,32,w=68) → physical(127, 0, 13w × 68h) -->
  <rect x="127" y="0" width="13" height="68" fill="#1a3a1a" stroke="#4a4" stroke-width="0.5"/>
  <text x="133" y="6" fill="#4a4" font-size="5" text-anchor="middle" writing-mode="tb">BASE</text>

  <!-- ═══════════════════════════════════════ -->
  <!-- LAYER 2: LVGL OBJECT OVERLAYS           -->
  <!-- (NOT rotated — physical coords)         -->
  <!-- ═══════════════════════════════════════ -->

  <!-- Crystal Gem: physical(18, -18, 69w × 68h) -->
  <rect x="18" y="-18" width="69" height="68" fill="rgba(0,255,200,0.1)" stroke="cyan" stroke-width="1" stroke-dasharray="3"/>
  <text x="52" y="16" fill="cyan" font-size="7" text-anchor="middle">CRYSTAL</text>
  <text x="52" y="24" fill="cyan" font-size="5" text-anchor="middle">69×68 anim</text>
  <text x="52" y="32" fill="cyan" font-size="5" text-anchor="middle">(18, −18)</text>

  <!-- Luna Dog: physical(36, 0, 24w × 32h) -->
  <rect x="36" y="0" width="24" height="32" fill="rgba(255,255,255,0.1)" stroke="white" stroke-width="1" stroke-dasharray="3"/>
  <text x="48" y="45" fill="white" font-size="6" text-anchor="middle">LUNA</text>
  <text x="48" y="52" fill="white" font-size="5" text-anchor="middle">24×32</text>

  <!-- ═══════════════════════════════════════ -->
  <!-- ANNOTATIONS                             -->
  <!-- ═══════════════════════════════════════ -->

  <!-- Physical X coordinate markers -->
  <line x1="0" y1="70" x2="0" y2="73" stroke="red" stroke-width="0.5"/>
  <text x="0" y="77" fill="red" font-size="4" text-anchor="middle">0</text>

  <line x1="13" y1="70" x2="13" y2="73" stroke="red" stroke-width="0.5"/>
  <text x="13" y="77" fill="red" font-size="4" text-anchor="middle">13</text>

  <line x1="29" y1="70" x2="29" y2="73" stroke="red" stroke-width="0.5"/>
  <text x="29" y="77" fill="red" font-size="4" text-anchor="middle">29</text>

  <line x1="44" y1="70" x2="44" y2="73" stroke="red" stroke-width="0.5"/>
  <text x="44" y="77" fill="red" font-size="4" text-anchor="middle">44</text>

  <line x1="60" y1="70" x2="60" y2="73" stroke="red" stroke-width="0.5"/>
  <text x="60" y="77" fill="red" font-size="4" text-anchor="middle">60</text>

  <line x1="87" y1="70" x2="87" y2="73" stroke="red" stroke-width="0.5"/>
  <text x="87" y="77" fill="red" font-size="4" text-anchor="middle">87</text>

  <line x1="93" y1="70" x2="93" y2="73" stroke="red" stroke-width="0.5"/>
  <text x="93" y="77" fill="red" font-size="4" text-anchor="middle">93</text>

  <line x1="109" y1="70" x2="109" y2="73" stroke="red" stroke-width="0.5"/>
  <text x="109" y="77" fill="red" font-size="4" text-anchor="middle">109</text>

  <line x1="127" y1="70" x2="127" y2="73" stroke="red" stroke-width="0.5"/>
  <text x="127" y="77" fill="red" font-size="4" text-anchor="middle">127</text>

  <line x1="160" y1="70" x2="160" y2="73" stroke="red" stroke-width="0.5"/>
  <text x="160" y="77" fill="red" font-size="4" text-anchor="middle">160</text>

  <!-- Screen dimension label -->
  <text x="80" y="85" fill="#aaa" font-size="5" text-anchor="middle">← 160px physical width →</text>
  <text x="-1" y="34" fill="#aaa" font-size="4" transform="rotate(-90, -1, 34)">68px</text>

  <!-- Overlap warning zone -->
  <rect x="13" y="0" width="47" height="50" fill="none" stroke="red" stroke-width="0.5" stroke-dasharray="2" opacity="0.6"/>
  <text x="36" y="62" fill="red" font-size="4" text-anchor="middle">⚠ OVERLAP ZONE</text>
</svg>
```

---

## Key Observations

1. **The Crystal Gem (69×68) almost covers the entire screen height** — it starts at Y=−18 and extends to Y=50, so on the visible 0–68 range it fills most of the vertical space from the left side (X=18 to X=87).

2. **Massive overlap on the left side (X=0–87)**:
   - Battery text (X=1–14) is **under** the Crystal
   - BT icon (X=29–44) is **under** the Crystal
   - Luna (X=36–60) is **under** the Crystal
   - These are all hidden when the Crystal animation is playing (charging mode)

3. **Clean right side (X=87–160)**:
   - Keycode Name (X=93–106) — partially overlaps with Crystal's right edge
   - Keycode ID (X=109–122) — clear
   - Layer Label (X=127–140) — clear

4. **Text orientation**: All text drawn in the pixel buffer appears **rotated 90°** on the physical screen (reads top-to-bottom).

5. **Coordinate**:	Value	Physical Mapping (160x68 Screen)
X-Offset	18	Positioned 18 pixels from the left edge of the physical screen.
Y-Offset	-18	Shifted "up" by 18 pixels. This is a common trick used to vertically center a large asset (e.g., a 90x90px or 128x128px crystal) on the narrow 68px height of the OLED.