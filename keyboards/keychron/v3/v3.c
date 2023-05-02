/* Copyright 2022 @ Keychron (https://www.keychron.com)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 2 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "quantum.h"

// clang-format off

const matrix_row_t matrix_mask[] = {
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111111111,
    0b1111111111101111,
};

// clang-format on

#ifdef DIP_SWITCH_ENABLE

    bool dip_switch_update_kb(uint8_t index, bool active) {
        if (!dip_switch_update_user(index, active)) {
            return false;
        }
        if (index == 0) {
            default_layer_set(1UL << (active ? 0 : 2));
        }
        return true;
    }

#endif // DIP_SWITCH_ENABLE

#if defined(RGB_MATRIX_ENABLE) && defined(CAPS_LOCK_LED_INDEX)

bool process_record_kb(uint16_t keycode, keyrecord_t *record) {
    if (!process_record_user(keycode, record)) { return false; }
    switch (keycode) {
        #ifdef RGB_MATRIX_ENABLE
                case RGB_TOG:
                    if (record->event.pressed) {
                        switch (rgb_matrix_get_flags()) {
                            case LED_FLAG_ALL: {
                                rgb_matrix_set_flags(LED_FLAG_NONE);
                                rgb_matrix_set_color_all(0, 0, 0);
                            } break;
                            default: {
                                rgb_matrix_set_flags(LED_FLAG_ALL);
                            } break;
                        }
                    }
                    if (!rgb_matrix_is_enabled()) {
                        rgb_matrix_set_flags(LED_FLAG_ALL);
                        rgb_matrix_enable();
                    }
                    return false;
        #endif
    }
    return true;
}

bool rgb_matrix_indicators_advanced_kb(uint8_t led_min, uint8_t led_max) {
    if (!rgb_matrix_indicators_advanced_user(led_min, led_max)) { return false; }
    // RGB_MATRIX_INDICATOR_SET_COLOR(index, red, green, blue);

    if (host_keyboard_led_state().caps_lock) {
        for (uint8_t i = led_min; i < led_max; i++) {
            if (g_led_config.flags[i] & LED_FLAG_KEYLIGHT) {
                // rgb_matrix_set_color(i, RGB_GOLD);
                RGB_MATRIX_INDICATOR_SET_COLOR(i, 80, 80, 80);
            }
        }
    } else {
        if (!rgb_matrix_get_flags()) {
           RGB_MATRIX_INDICATOR_SET_COLOR(CAPS_LOCK_LED_INDEX, 0, 0, 0);
        }
    }
    return true;
}

#endif // CAPS_LOCK_LED_INDEX
