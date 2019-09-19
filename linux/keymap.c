#include <stdarg.h>
#include QMK_KEYBOARD_H
#include "led.h"
#include "debug.h"
#include "action_layer.h"
#include "action_util.h"
#include "timer.h"
#include "keymap_plover.h"
#include "eeconfig.h"
#include "wait.h"
#include "version.h"
#include "print.h"

#define BASE 0   // default layer
#define SYMB 1   // symbol layer   -  NumPad and symbols
#define MOUS 2  // mouse layer - includes mouse buttons and function keys

enum custom_keycodes {
  PLACEHOLDER = SAFE_RANGE, // can always be here
  EPRM,
  VRSN,
  RGB_SLD,
};

// Sticky keys
enum {
  F_SFT_L = 0,
  F_SFT_R,
  F_ALT_L,
  F_ALT_R,
  F_GUI_L,
  F_GUI_R,
  F_CTRL_L,
  F_CTRL_R
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {

/* Keymap 0: Base Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |     \     | 1  ! | 2  @ | 3  # | 4  $ | 5  % |GUI+LT|           |GUI+RT| 6  ^ | 7  & | 8  * | 9  ( | 10 ) |   DEL     |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |     ~     |   Q  |   W  |   E  |   R  |   T  |OneSht|           |OneSht|   Y  |   U  |   I  |   O  |   P  |     -     |
 * |-----------+------+------+------+------+------|Mouse |           |Symbol|------+------+------+------+------+-----------|
 * |     =     |   A  |   S  |   D  |   F  |   G  |------|           |------|   H  |   J  |   K  |   L  |   ;  |     '     |
 * |-----------+------+------+------+------+------|SHIFT |           | SHIFT|------+------+------+------+------+-----------|
 * |    TAB    |   Z  |   X  |   C  |   V  |   B  |      |           |      |   N  |   M  |   ,  |   .  |   /  |   APPS    |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     | SF+CTL|CT+ALT| GUI  | Left | Right|                                       | Down |  Up  | GUI  |CT+ALT|SF+CTL |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         | CTRL | ALT  |           | ALT  | CTRL |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      | Home |           | PgUp |      |      |
 *                                  |  SPC | BKSP |------|           |------| ESC  |ENTER |
 *                                  |      |      |  End |           | PgDn |      |      |
 *                                  `--------------------'           `--------------------'
 */

  [BASE] = LAYOUT_ergodox(
            // left hand
            KC_BSLASH,      KC_1,           KC_2,      KC_3,    KC_4,    KC_5,       LGUI(KC_LEFT),
            KC_TILD,        KC_Q,           KC_W,      KC_E,    KC_R,    KC_T,       OSL(2),
            KC_EQUAL,       KC_A,           KC_S,      KC_D,    KC_F,    SFT_T(KC_G),
            KC_TAB,         KC_Z,           KC_X,      KC_C,    KC_V,    KC_B,       F(F_SFT_L),
            LSFT(KC_LCTRL), LALT(KC_LCTRL), KC_LGUI,   KC_LEFT, KC_RIGHT,

                                    // left thumb cluster

                                               KC_LCTRL, KC_LALT,
                                                         KC_HOME,
                                    KC_SPACE, KC_BSPACE, KC_END,

                                                    // right hand
                                                    RGUI(KC_RIGHT), KC_6,       KC_7,   KC_8,     KC_9,             KC_0,           KC_DELETE,
                                                    OSL(1),         KC_Y,       KC_U,   KC_I,     KC_O,             KC_P,           KC_MINUS,
                                                                    SFT_T(KC_H),KC_J,   KC_K,     KC_L,             KC_SCOLON,      KC_QUOTE,
                                                    F(F_SFT_L),     KC_N,       KC_M,   KC_COMMA, KC_DOT,           KC_SLASH,       KC_APPLICATION,
                                                                                KC_DOWN,KC_UP,    KC_LGUI,          RALT(KC_RCTRL), RSFT(KC_RCTRL),

                                                                    // right thumb cluster
                                                                    KC_RALT, KC_RCTRL,
                                                                    KC_PGUP,
                                                                    KC_PGDOWN, KC_ESCAPE, KC_ENTER
        ),

/* Keymap 1: Symbol Layer
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * | NUMLOCK   |  F1  |  F2  |  F3  |  F4  |  F5  |      |           |      |  F6  |  F7  |  F8  |  F9  |  F10 |    F11    |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |   `  |   @  |   {  |   }  |   |  |      |           |      |   *  |   7  |   8  |   9  |   &  |    F12    |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * | CTRL+INS  |   #  |   $  |   (  |   )  |   &  |------|           |------|   +  |   4  |   5  |   6  |   _  | SHIFT+INS |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |   %  |   ^  |   [  |   ]  |   "  |      |           |      |   -  |   1  |   2  |   3  |   /  |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |       |      |      |      |   \  |                                       |   =  |   0  |   .  |      |       |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |      |           |      |      |      |
 *                                  |      |      |------|           |------|      |      |
 *                                  |      |      |      |           |      |      |      |
 *                                  `--------------------'           `--------------------'
 */
  [SYMB] = LAYOUT_ergodox(
            // left hand
             KC_NUMLOCK,           KC_F1,          KC_F2,          KC_F3,          KC_F4,      KC_F5,      KC_TRANSPARENT,
             KC_TRANSPARENT,       KC_GRAVE,       KC_AT,          KC_LCBR,        KC_RCBR,    KC_PIPE,    KC_TRANSPARENT,
             LCTL(KC_INSERT),      KC_HASH,        KC_DLR,         KC_LPRN,        KC_RPRN,    KC_AMPR,
             KC_TRANSPARENT,       KC_PERC,        KC_CIRC,        KC_LBRACKET,    KC_RBRACKET,KC_DQUO,    KC_TRANSPARENT,
             KC_TRANSPARENT,       KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_BSLASH,

                    // left thumb cluster
                    KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,

                            // right hand
                            KC_TRANSPARENT,  KC_F6,          KC_F7,    KC_F8,      KC_F9,      KC_F10,           KC_F11,
                            KC_TRANSPARENT,  KC_ASTR,        KC_KP_7,  KC_KP_8,    KC_KP_9,    KC_AMPR,          KC_F12,
                                             KC_PLUS,        KC_KP_4,  KC_KP_5,    KC_KP_6,    KC_UNDS,          LSFT(KC_INSERT),
                            KC_TRANSPARENT,  KC_MINUS,       KC_KP_1,  KC_KP_2,    KC_KP_3,    KC_SLASH,         KC_TRANSPARENT,
                                                             KC_EQUAL, KC_KP_0,    KC_KP_DOT,  KC_TRANSPARENT,   KC_TRANSPARENT,

                                            // right thumb cluster
                                            KC_TRANSPARENT, KC_TRANSPARENT,
                                            KC_TRANSPARENT,
                                            KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT
            ),

/* Keymap 2:
 *
 * ,-----------------------------------------------------.           ,-----------------------------------------------------.
 * |           |      |      |      |      |      |      |           |      |      |      |      |      |      |           |
 * |-----------+------+------+------+------+-------------|           |------+------+------+------+------+------+-----------|
 * |           |      |      |      |      |      |      |           |      | WHUP | WHLT | MSUP | WHRT |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      | RGHT | MDDL | LEFT |      |------|           |------| WHDN | MSLT | MSDN | MSRT |      |           |
 * |-----------+------+------+------+------+------|      |           |      |------+------+------+------+------+-----------|
 * |           |      | ACCL2| ACCL1| ACCL0|      |      |           |      |      |      |      |      |      |           |
 * `-----------+------+------+------+------+-------------'           `-------------+------+------+------+------+-----------'
 *     |       |      |      |      |      |                                       |      |      |      |      |       |
 *     `-----------------------------------'                                       `-----------------------------------'
 *                                         ,-------------.           ,-------------.
 *                                         |      |      |           |      |      |
 *                                  ,------|------|------|           |------+------+------.
 *                                  |      |      |VOL UP|           |PREVTR|      |      |
 *                                  | MUTE |      |------|           |------|      |PLAY/ |
 *                                  |      |      |VOL DN|           |NEXTTR|      |PAUSE |
 *                                  `--------------------'           `--------------------'
 */
  [MOUS] = LAYOUT_ergodox(
            // left hand
            KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
            KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
            KC_TRANSPARENT, KC_MS_BTN2,     KC_MS_BTN3,     KC_MS_BTN1,     KC_TRANSPARENT, KC_TRANSPARENT,
            KC_TRANSPARENT, KC_MS_ACCEL2,   KC_MS_ACCEL1,   KC_MS_ACCEL0,   KC_TRANSPARENT, KC_TRANSPARENT,
            KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,

            // left thumb cluster
                            KC_TRANSPARENT, KC_TRANSPARENT,
                                            KC_AUDIO_VOL_UP,
            KC_AUDIO_MUTE,  KC_TRANSPARENT, KC_AUDIO_VOL_DOWN,


            // right hand
            KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
            KC_TRANSPARENT, KC_MS_WH_UP,    KC_MS_WH_LEFT,  KC_MS_UP,       KC_MS_WH_RIGHT, KC_TRANSPARENT, KC_TRANSPARENT,
                            KC_MS_WH_DOWN,  KC_MS_LEFT,     KC_MS_DOWN,     KC_MS_RIGHT,    KC_TRANSPARENT, KC_TRANSPARENT,
            KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,
                                            KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT, KC_TRANSPARENT,

            // right thumb cluster
            KC_TRANSPARENT, KC_TRANSPARENT,
            KC_MEDIA_PREV_TRACK,
            KC_MEDIA_NEXT_TRACK, KC_TRANSPARENT, KC_MEDIA_PLAY_PAUSE
            ),
};


const uint16_t PROGMEM fn_actions[] = {
  [F_SFT_L]  = ACTION_MODS_ONESHOT (MOD_LSFT)
  ,[F_SFT_R]  = ACTION_MODS_ONESHOT (MOD_RSFT)
  ,[F_GUI_L]  = ACTION_MODS_ONESHOT (MOD_LGUI)
  // There seems to be a bug here with MOD_RGUI, which acts like an ALT instead
  ,[F_GUI_R]  = ACTION_MODS_ONESHOT (MOD_RGUI)
  ,[F_ALT_L]  = ACTION_MODS_ONESHOT (MOD_LALT)
  ,[F_ALT_R]  = ACTION_MODS_ONESHOT (MOD_RALT)
  ,[F_CTRL_L] = ACTION_MODS_ONESHOT (MOD_LCTL)
  ,[F_CTRL_R] = ACTION_MODS_ONESHOT (MOD_RCTL)
};

// leaving this in place for compatibilty with old keymaps cloned and re-compiled.
const macro_t *action_get_macro(keyrecord_t *record, uint8_t id, uint8_t opt)
{
      switch(id) {
        case 0:
        if (record->event.pressed) {
          SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
        }
        break;
      }
    return MACRO_NONE;
};

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
  switch (keycode) {
    // dynamically generate these.
    case EPRM:
      if (record->event.pressed) {
        eeconfig_init();
      }
      return false;
      break;
    case VRSN:
      if (record->event.pressed) {
        SEND_STRING (QMK_KEYBOARD "/" QMK_KEYMAP " @ " QMK_VERSION);
      }
      return false;
      break;
    case RGB_SLD:
      if (record->event.pressed) {
        rgblight_mode(1);
      }
      return false;
      break;
    
  }
  return true;
}

void matrix_scan_user(void) {

    uint8_t layer = biton32(layer_state);

    ergodox_board_led_off();
    ergodox_right_led_1_off();
    ergodox_right_led_2_off();
    ergodox_right_led_3_off();

    // use green for layer 1, blue for layter 2
    switch (layer) {
        case 1:
            ergodox_right_led_1_on();
            ergodox_right_led_2_on();
            break;
        case 2:
            ergodox_right_led_2_on();
            ergodox_right_led_3_on();
            break;
        default:
            break;
    }

    if (keyboard_report->mods & MOD_BIT(KC_LSFT) || keyboard_report->mods & MOD_BIT(KC_RSFT) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LSFT) || get_oneshot_mods() & MOD_BIT(KC_RSFT)) && !has_oneshot_mods_timed_out())) {
            // LUKETODO: play around with brightness
      ergodox_right_led_1_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_1_on(); 
    }

    if (keyboard_report->mods & MOD_BIT(KC_LCTRL) || keyboard_report->mods & MOD_BIT(KC_RCTRL) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LCTRL) || get_oneshot_mods() & MOD_BIT(KC_RCTRL)) && !has_oneshot_mods_timed_out())) {
      ergodox_right_led_3_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_3_on(); 
    }

    if (keyboard_report->mods & MOD_BIT(KC_LALT) || keyboard_report->mods & MOD_BIT(KC_RALT) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LALT) || get_oneshot_mods() & MOD_BIT(KC_RALT)) && !has_oneshot_mods_timed_out())) { 
      ergodox_right_led_2_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_2_on();
    }


     if (keyboard_report->mods & MOD_BIT(KC_LGUI) || keyboard_report->mods & MOD_BIT(KC_RGUI) ||
        ((get_oneshot_mods() & MOD_BIT(KC_LGUI) || get_oneshot_mods() & MOD_BIT(KC_RGUI)) && !has_oneshot_mods_timed_out())) { 
      ergodox_right_led_1_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_1_on();
      ergodox_right_led_3_set (LED_BRIGHTNESS_HI);
      ergodox_right_led_3_on();
    }
};
