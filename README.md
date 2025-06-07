# nice!oled ZMK Shield

[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](https://opensource.org/licenses/MIT)

## Project Overview
The nice!oled shield is a ZMK firmware module that provides a customizable OLED status screen for mechanical keyboards. It displays system information such as battery level, active layer, modifiers, output profile, WPM, and keycodes. The module is designed to be easily integrated into ZMK-based keyboard firmware.

## Features
- **Battery Status**: Shows current battery level and charging status
- **Layer Display**: Indicates the currently active keyboard layer
- **Modifier Keys**: Displays the state of modifier keys (Ctrl, Alt, Shift, etc.)
- **Output Profiles**: Shows the active output profile (BLE, USB) and connection status
- **WPM Counter**: Real-time words per minute tracking
- **Keycode Display**: Shows the last pressed keycode
- **Animations**: Optional animations (like Luna the dog) for visual feedback
- **Custom Fonts**: Supports custom fonts (including PixelOperatorMono in multiple sizes)
- **Inverted Color Scheme**: Configurable light/dark mode

## Installation
To use the nice!oled shield in your ZMK firmware:

1. Add this repository as a Zephyr module in your `zmk-config`:
   ```bash
   west.yml: 
     - name: zmk-nice-oled-zz
       url: https://github.com/zzuse/zmk-nice-oled-zz
       revision: main
   ```

2. Enable the shield in your `.conf` file:
   ```
   CONFIG_NICE_VIEW_WIDGET=y
   CONFIG_ZMK_DISPLAY=y
   ```

3. Add the shield to your keyboard's `.keymap` file

## Configuration
The following Kconfig options are available:
- `CONFIG_NICE_VIEW_WIDGET_INVERTED`: Invert display colors (default: n)
- `CONFIG_NICE_VIEW_WIDGET_STATUS`: Enable status screen (default: y)
- `CONFIG_NICE_VIEW_WIDGET_ANIMATION`: Enable animations (default: y)

## Customization
You can customize the display by:
- Adding custom fonts in `boards/shields/nice_oled/assets/`
- Adding custom images for animations
- Modifying the widget layouts in `boards/shields/nice_oled/widgets/`

## Widget Reference

### Keycode Widget
Displays last pressed keycode  
**File**: `widgets/keycode.[ch]`

### Battery Widget
Displays battery percentage and charging status  
**File**: `widgets/battery.[ch]`

### Layer Widget
Shows currently active layer  
**File**: `widgets/layer.[ch]`

### Output Widget
Displays active output profile (BLE/USB)  
**File**: `widgets/output.[ch]`

## License
This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.
## Contributors
- Marcos Chow Castro (Original Author)
- zzuse (Maintainer) Modify to suit my own needs