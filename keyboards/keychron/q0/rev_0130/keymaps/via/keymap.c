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

#include QMK_KEYBOARD_H


enum layers { _BASE, _FN1, _RESERVED1, _RESERVED2 };

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
    [_BASE] = LAYOUT_numpad_6x4(
        MO(_FN1), KC_ESC,  KC_BSPC,   KC_TAB,
        KC_NUM,   KC_PSLS, KC_PAST,   KC_PMNS,
        KC_P7,    KC_P8,   KC_P9,
        KC_P4,    KC_P5,   KC_P6,     KC_PPLS,
        KC_P1,    KC_P2,   KC_P3,
        KC_P0,             KC_PDOT,   KC_PENT),

    [_FN1] = LAYOUT_numpad_6x4(
        _______,    KC_MUTE, KC_VOLD,   KC_VOLU,
        RGB_MOD,    RGB_VAI, RGB_HUI,   KC_DEL,
        RGB_RMOD,   RGB_VAD, RGB_HUD,
        RGB_SAI,    RGB_SPI, KC_MPRV,   _______,
        RGB_SAD,    RGB_SPD, KC_MPLY,
        RGB_TOG,             KC_MNXT,   _______),

    [_RESERVED1] = LAYOUT_numpad_6x4(
        _______,    _______, _______,   _______,
        _______,    _______, _______,   _______,
        _______,    _______, _______,
        _______,    _______, _______,   _______,
        _______,    _______, _______,
        _______,             _______,   _______),

    [_RESERVED2] = LAYOUT_numpad_6x4(
        _______,    _______, _______,   _______,
        _______,    _______, _______,   _______,
        _______,    _______, _______,
        _______,    _______, _______,   _______,
        _______,    _______, _______,
        _______,             _______,   _______)
};


//https://gist.github.com/aashreys/01cb34605a290a7cfb94a856bdabc94c

static uint16_t key_timer; // timer to track the last keyboard activity
static void refresh_rgb(void); // refreshes the activity timer and RGB, invoke whenever activity happens
static void check_rgb_timeout(void); // checks if enough time has passed for RGB to timeout
bool is_rgb_timeout = false; // store if RGB has timed out or not in a boolean



void refresh_rgb() {
  key_timer = timer_read(); // store time of last refresh
  if (is_rgb_timeout) { // only do something if rgb has timed out
    print("Activity detected, removing timeout\n");
    is_rgb_timeout = false;
    //rgblight_wakeup();
        print("Activity detected: rgblight_wakeup()\n");
        rgblight_enable();
  }
};


void check_rgb_timeout() {
  if (!is_rgb_timeout && timer_elapsed(key_timer) > RGBLIGHT_TIMEOUT) {
    print("NO Activity detected, go to sleep little RGBs\n");
    rgblight_disable();
    is_rgb_timeout = true;
  }
};


/* Then, call the above functions from QMK's built in post processing functions like so */

/* Runs at the end of each scan loop, check if RGB timeout has occured */
void housekeeping_task_user(void) {
  #ifdef RGBLIGHT_TIMEOUT
  check_rgb_timeout();
  #endif

  /* rest of the function code here */
};

/* Runs after each key press, check if activity occurred */
void post_process_record_user(uint16_t keycode, keyrecord_t *record) {
  #ifdef RGBLIGHT_TIMEOUT
  if (record->event.pressed) refresh_rgb();
  #endif

  /* rest of the function code here */
};

/* Runs after each encoder tick, check if activity occurred */
void post_encoder_update_user(uint8_t index, bool clockwise) {
  #ifdef RGBLIGHT_TIMEOUT
  refresh_rgb();
  #endif

  /* rest of the function code here */
};
