#include QMK_KEYBOARD_H

enum {
    TD_Z,
    TD_X,
    TD_C,
    TD_V,
    TD_A,
    TD_S,
    TD_D,
    TD_F,

};

uint16_t key_count = 0;

char wpm_str[21];
char kc_str[21];

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
	// Default Layer
	[0] = LAYOUT_split_3x6_3(

		KC_TAB, KC_Q, KC_W, KC_E, KC_R, KC_T, KC_Y, KC_U, KC_I, KC_O, KC_P, KC_BSPC,
		KC_LSPO, KC_A, KC_S, KC_D, KC_F, KC_G, KC_H, KC_J, KC_K, KC_L, KC_SCLN, KC_QUOT,
		KC_LCTL, KC_Z, KC_X, KC_C, KC_V, KC_B, KC_N, KC_M, KC_COMM, KC_DOT, KC_SLSH, KC_ENT,
		LALT_T(KC_LGUI), MO(2), KC_SPC, KC_RSPC, MO(1), MO(3)),

	// Navigation and Number Layer
	[1] = LAYOUT_split_3x6_3(

		KC_ESC, KC_NO, KC_7, KC_8, KC_9, KC_DOT, KC_PGUP, KC_HOME, KC_UP, KC_END, KC_NO, KC_BSPC,
		KC_LSFT, KC_NO, KC_4, KC_5, KC_6, KC_COMM, KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, KC_NO, KC_DEL,
		KC_LCTL, KC_NO, KC_1, KC_2, KC_3, KC_LGUI, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_ENT,
		KC_TRNS, KC_NO, KC_0, KC_NO, KC_TRNS, KC_NO),

	// Symbol and macro Layer
	[2] = LAYOUT_split_3x6_3(

		KC_ESC, KC_EXLM, KC_AT, KC_HASH, KC_DLR, KC_PERC, KC_CIRC, KC_AMPR, KC_LPRN, KC_RPRN, KC_ASTR, KC_BSPC,
		KC_LSFT, KC_NO, KC_NO, KC_NO, RCS(KC_F), KC_GRV, KC_MINS, KC_EQL, KC_LBRC, KC_RBRC, KC_PIPE, KC_GRV,
		KC_LCTL, KC_NO, KC_NO, KC_NO, RCS(KC_V), KC_LGUI, KC_UNDS, KC_PLUS, KC_LCBR, KC_RCBR, KC_BSLS, KC_TILD,
		KC_TRNS, KC_TRNS, KC_NO, KC_RSFT, KC_NO, KC_NO),

	// Function and mouse layer
	[3] = LAYOUT_split_3x6_3(

		KC_ESC, KC_F1, KC_F2, KC_F3, KC_F4, KC_F5, KC_WH_U, KC_BTN1, KC_MS_U, KC_BTN2, KC_NO, KC_NO,
		KC_LSFT, KC_F6, KC_F7, KC_F8, KC_F9, KC_F10, KC_WH_D, KC_MS_L, KC_MS_D, KC_MS_R, KC_NO, KC_NO,
		KC_LCTL, KC_F11, KC_F12, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO, KC_NO,
		KC_TRNS, KC_BTN2, KC_BTN1, KC_NO, KC_NO, KC_TRNS)

};


#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
  if (!is_master) {
    return OLED_ROTATION_180;  // flips the display 180 degrees if offhand
  }
  return rotation;
}

#define L_BASE 0
#define L_LOWER 2
#define L_RAISE 4
#define L_ADJUST 8

void oled_render_layer_state(void) {
    sprintf(wpm_str, "WPM: %03d\n", get_current_wpm());
    sprintf(kc_str, "Keys Pressed: %03d\n", key_count);
    switch (layer_state) {
        case L_BASE:
            oled_write_P(PSTR("LAYER: QWERTY\n"), false);
            oled_write(wpm_str, false);
            oled_write(kc_str, false);
            break;
        case L_LOWER:
            oled_write_P(PSTR("LAYER: NAVIGATION & NUMBER\n"), false);
            break;
        case L_RAISE:
            oled_write_P(PSTR("LAYER: SYMBOLS\n"), false);
            break;
        case L_ADJUST:
        case L_ADJUST|L_LOWER:
        case L_ADJUST|L_RAISE:
        case L_ADJUST|L_LOWER|L_RAISE:
            oled_write_P(PSTR("LAYER: MOUSE & FUNCTION\n"), false);
            break;
    }
}

void oled_render_logo(void) {
    static const char PROGMEM crkbd_logo[] = {
        0x80, 0x81, 0x82, 0x83, 0x84, 0x85, 0x86, 0x87, 0x88, 0x89, 0x8a, 0x8b, 0x8c, 0x8d, 0x8e, 0x8f, 0x90, 0x91, 0x92, 0x93, 0x94,
        0xa0, 0xa1, 0xa2, 0xa3, 0xa4, 0xa5, 0xa6, 0xa7, 0xa8, 0xa9, 0xaa, 0xab, 0xac, 0xad, 0xae, 0xaf, 0xb0, 0xb1, 0xb2, 0xb3, 0xb4,
        0xc0, 0xc1, 0xc2, 0xc3, 0xc4, 0xc5, 0xc6, 0xc7, 0xc8, 0xc9, 0xca, 0xcb, 0xcc, 0xcd, 0xce, 0xcf, 0xd0, 0xd1, 0xd2, 0xd3, 0xd4,
        0};
    oled_write_P(crkbd_logo, false);
}

void oled_task_user(void) {
    if (is_master) {
        oled_render_layer_state();
    } else {
        oled_render_logo();
    }
}

bool process_record_user(uint16_t keycode, keyrecord_t *record) {
    if (record->event.pressed) {
        key_count += 1;
    }
    return true;
}

#endif // OLED_DRIVER_ENABLE
