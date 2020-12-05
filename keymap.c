#include QMK_KEYBOARD_H
#include <stdio.h>
bool is_alt_tab_active = false;
uint16_t alt_tab_timer = 0;

char wpm_str[10];

enum layers {
    _QWERTY,
    _2WERTY, //pronounced "twerty," obviously
    _LOWER,
    _RAISE,
    _ADJUST,
};

const uint16_t PROGMEM keymaps[][MATRIX_ROWS][MATRIX_COLS] = {
/*
 * Base Layer: QWERTY
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |RAIS/ESC|   Q  |   W  |   E  |   R  |   T  |                              |   Y  |   U  |   I  |   O  |   P  |  BKSP  |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |Tab     |   A  |   S  |  D   |   F  |   G  |                              |   H  |   J  |   K  |   L  | ;  : |  ' "   |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * | CTRL   |   Z  |   X  |   C  |   V  |   B  |Enter |Ctrl  |  |Ctrl  |(2u)  |   N  |   M  | ,  < | . >  | /  ? |  - _   |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        | TG(1)| Alt  | Win  | Shift| Back |  | Enter| Space| Win  | Alt  |TG(3) |
 *                        |      |      |      | Lower| space|  | Lower| Raise|      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_QWERTY] = LAYOUT(
      KC_ESC,    KC_Q,    KC_W,   KC_E,   KC_R,   KC_T,                                             KC_Y,    KC_U,    KC_I,    KC_O,       KC_P, KC_BSPC,
      KC_TAB,    KC_A,    KC_S,   KC_D,   KC_F,   KC_G,                                             KC_H,    KC_J,    KC_K,    KC_L,    KC_SCLN, KC_QUOT,
      KC_LSFT,   KC_Z,    KC_X,   KC_C,   KC_V,   KC_B,    KC_EQL,  _______,    _______ ,KC_LBRC,  KC_N,    KC_M,   KC_COMM,  KC_DOT,  KC_SLSH, KC_MINS,
                                 TG(1), KC_LGUI,KC_LALT,KC_LCTL,  KC_SPC,      LT(3, KC_ENT),KC_RBRC,LT(2, KC_RGUI),  KC_RALT, TG(2)
    ),
/*
 * This layer exists to change encoder functions.
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_2WERTY] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
      _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 KC_TRNS, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
/*
 * Lower Layer: numpad and arrow
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |   7  |   8  |  9   |  0   |                              | PGUP |      |  UP  |      |      |   Del
   |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * | CAPS   |      |   4  |   5  |  6   | Tab  |                              | PGDN | Left | Down | Right|      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |   1  |  2   |  3   | Ent  |      |      |  |      |      |      | LBRC | RBRC |      |   |\ |  +=    |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      | Tab  | Ent  |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
     [_LOWER] = LAYOUT(
        _______, _______,    KC_1,    KC_2,    KC_3,  _______ ,                                            KC_PGUP, _______,   KC_UP, _______, _______,  KC_DEL,
        KC_CAPS, _______,    KC_4,    KC_5,    KC_6,  _______,                                            KC_PGDN, KC_LEFT, KC_DOWN, KC_RGHT, _______, _______,
        KC_LSFT,    KC_0,    KC_7,    KC_8,    KC_9,  KC_0,   _______,  KC_TRNS,       MO(4), _______, KC_LBRC, KC_RBRC, _______, _______, KC_BSLS,  KC_EQL,
                                   KC_TRNS,    KC_TRNS,  KC_TRNS, KC_TRNS,  KC_TRNS,     KC_TRNS, KC_TRNS, _______, _______, _______
    ),
/*
 * Raise Layer: F keys and media
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        |      |      |      |      |      |                              |  F9  | F10  |  F11 |  F12 |      |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        |      | Prev | Play | Next | VolUp|                              | F5   | F6   | F7   | F8   |      |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        |      |      |      | Mute | VolDn|      |      |  |      |      | F1   | F2   | F3   |  F4  |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_RAISE] = LAYOUT(
      _______, _______, _______, _______, _______, _______,                                       KC_F9,  KC_F10,  KC_F11, KC_F12, _______, _______,
      _______, _______, KC_MPRV, KC_MPLY, KC_MNXT, KC_VOLU,                                       KC_F5,   KC_F6,   KC_F7,  KC_F8, _______, _______,
      _______, _______, _______, _______, KC_MUTE, KC_VOLD, _______, _______, _______, _______,   KC_F1,   KC_F2,   KC_F3,  KC_F4, _______, _______,
                                 _______, _______, _______, _______, _______, _______, _______, _______, _______, KC_TRNS
    ),
/*
 * Adjust Layer: Function keys, RGB
 *
 * ,-------------------------------------------.                              ,-------------------------------------------.
 * |        | F1   |  F2  | F3   | F4   | F5   |                              | F6   | F7   |  F8  | F9   | F10  |        |
 * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
 * |        | VAI  | SAI  | HUI  | SPI  | MOD  |                              |      |      |      | F11  | F12  |        |
 * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
 * |        | VID  | SAD  | HUD  | SPD  | RMOD |      |      |  |      |      |      |      |      |      |      |        |
 * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        |      |      |      |      |      |  |      |      |      |      |      |
 *                        `----------------------------------'  `----------------------------------'
 */
    [_ADJUST] = LAYOUT(
      _______, KC_F1,   KC_F2,   KC_F3,   KC_F4,   KC_F5,                                       KC_F6,   KC_F7,   KC_F8,   KC_F9,   KC_F10,  _______,
      RGB_TOG, RGB_VAI, RGB_SAI, RGB_HUI, RGB_SPI, RGB_MOD,                                     _______, _______, _______, KC_F11,  KC_F12,  _______,
      _______, RGB_VAD, RGB_SAD, RGB_HUD, RGB_SPD, RGB_RMOD,_______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
                                 KC_TRNS, _______, _______, _______, _______, _______, _______, _______, _______, _______
    ),
// /*
//  * Layer template
//  *
//  * ,-------------------------------------------.                              ,-------------------------------------------.
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------|                              |------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |                              |      |      |      |      |      |        |
//  * |--------+------+------+------+------+------+-------------.  ,-------------+------+------+------+------+------+--------|
//  * |        |      |      |      |      |      |      |      |  |      |      |      |      |      |      |      |        |
//  * `----------------------+------+------+------+------+------|  |------+------+------+------+------+----------------------'
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        |      |      |      |      |      |  |      |      |      |      |      |
//  *                        `----------------------------------'  `----------------------------------'
//  */
//     [_LAYERINDEX] = LAYOUT(
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______,                                     _______, _______, _______, _______, _______, _______,
//       _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______, _______,
//                                  _______, _______, _______, _______, _______, _______, _______, _______, _______, _______
//     ),
};

layer_state_t layer_state_set_user(layer_state_t state) {
    return update_tri_layer_state(state, _LOWER, _RAISE, _ADJUST);
}


#ifdef OLED_DRIVER_ENABLE

oled_rotation_t oled_init_user(oled_rotation_t rotation) {
	return OLED_ROTATION_180;
}

static void render_status(void) {
    // Host Keyboard Layer Status
    oled_write_P(PSTR("  -----------------\n"), false);
    oled_write_P(PSTR("  |   B0RV3G4R4   |\n"), false);
    oled_write_P(PSTR("  -----------------\n"), false);
    oled_write_P(PSTR("\n"), false);
    

    switch (get_highest_layer(layer_state)) {
         case _QWERTY:
            oled_write_P(PSTR("    -> QWERTY"), false);
            break;
        case _2WERTY:
            oled_write_P(PSTR("    -> 2WERTY"), false);
            break;
        case _LOWER:
            oled_write_P(PSTR("    -> Numpad"), false);
            break;
        case _RAISE:
            oled_write_P(PSTR("    -> F Keys"), false);
            break;
        case _ADJUST:
            oled_write_P(PSTR("    -> RGB"), false);
            break;
        default:
            oled_write_P(PSTR("Undefined"), false);
    }
}


// WPM-responsive animation stuff here
#define IDLE_SPEED 10 // below this wpm value your animation will idle
#define TAP_SPEED 60 // above this wpm value typing animation to triggere
#define ANIM_FRAME_DURATION 200 // how long each frame lasts in ms

uint32_t anim_timer = 0;
uint32_t anim_sleep = 0;
uint8_t smallCloud_position = 20;
uint8_t smallCloud2_position = 0;

bool marioJumping = false;
uint8_t cointPosition = 0;


static void render_anim(void) {
    static const char PROGMEM floor[] = {
         171, 43, 35, 171, 137, 171, 43, 35, 1, 1, 0, 1, 1, 1, 1, 0
    };
    
    static const char PROGMEM cloud_top[] = {
       64,96,32,32,48,28,6,22,19,3,3,30,12,60,120,96,96,96,64
    };

    static const char PROGMEM cloud_bot[] = {
        4,14,26,49,32,32,96,80,192,160,224,112,80,144,160,160,64,65,65,64,107,62,22
    };

    static const char PROGMEM block_top[] = {
       252,252,251,255,255,15,15,231,239,255,31,31,251,248,0,0,0,0,0
    };

    static const char PROGMEM block_bot[] = {
        63,63,31,63,63,62,62,51,0,8,60,62,31,31,0,0,0,0,0
    };

    static const char PROGMEM mario_top[] = {
       0,56,188,223,223,199,199,255,190,54,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0
    };

    static const char PROGMEM mario_bot[] = {
        0,0,3,231,255,123,63,63,123,255,231,3,0,0
    };

    static const char PROGMEM coin[] = {
        0,120,252,134,250,254,252,48,0
    };


    void animateFloor(void){
        for(int i = 0; i< 21;i++ ){
                oled_set_cursor(i,7);
                oled_write_raw_P(floor,sizeof(floor));
            }
    }

    void animateClouds(void){
        oled_set_cursor(smallCloud_position,0);
        oled_write_raw_P(cloud_top,sizeof(cloud_top));
        oled_set_cursor(smallCloud_position,1);
        oled_write_raw_P(cloud_bot,sizeof(cloud_bot));
        smallCloud_position--;
        if(!smallCloud_position) smallCloud_position = 20;

        oled_set_cursor(smallCloud2_position,2);
        oled_write_raw_P(cloud_top,sizeof(cloud_top));
        oled_set_cursor(smallCloud2_position,3);
        oled_write_raw_P(cloud_bot,sizeof(cloud_bot));
        smallCloud2_position++;
        if(smallCloud2_position > 20) smallCloud2_position = 0;
    }

    void animateCoinBlock(void){
        oled_set_cursor(10,2);
        oled_write_raw_P(block_top,sizeof(block_top));
        oled_set_cursor(10,3);
        oled_write_raw_P(block_bot,sizeof(block_bot));
    }


    void animateMario(void){
         oled_set_cursor(10,5);
        oled_write_raw_P(mario_top,sizeof(mario_top));
        oled_set_cursor(10,6);
        oled_write_raw_P(mario_bot,sizeof(mario_bot));
    }

    void animateMarioOnFire(bool onFire){
        //Jump on
        if(!marioJumping){
            oled_set_cursor(10,4);
            oled_write_raw_P(mario_top,sizeof(mario_top));
            oled_set_cursor(10,5);
            oled_write_raw_P(mario_bot,sizeof(mario_bot));
            marioJumping = !marioJumping;
        }else{
            oled_set_cursor(10,5);
            oled_write_raw_P(mario_top,sizeof(mario_top));
            oled_set_cursor(10,6);
            oled_write_raw_P(mario_bot,sizeof(mario_bot));
            marioJumping = !marioJumping;
        }
        if(onFire){
        switch(cointPosition){
            case 0:
                break;
            case 1:
                oled_set_cursor(11,1);
                oled_write_raw_P(coin,sizeof(coin));
                break;
            case 2:
              oled_set_cursor(11,0);
                oled_write_raw_P(coin,sizeof(coin));
                break;
            case 3:
                oled_set_cursor(13,1);
                oled_write_raw_P(coin,sizeof(coin));
                break; 
            case 4: 
                oled_set_cursor(13,3);
                oled_write_raw_P(coin,sizeof(coin));
                break;   
            case 5:
                oled_set_cursor(13,5);
                oled_write_raw_P(coin,sizeof(coin));
                break;   
        }
         cointPosition+=1;
        if(cointPosition > 5) cointPosition = 0;
        }
    }

    //assumes 1 frame prep stage
    void animation_phase(void) {
        animateFloor();
        animateClouds();
        animateCoinBlock();

        if(get_current_wpm() <=IDLE_SPEED){
            animateMario();
         }
         if(get_current_wpm() >IDLE_SPEED && get_current_wpm() <TAP_SPEED){
              animateMarioOnFire(false);
         }
         if(get_current_wpm() >=TAP_SPEED){
             animateMarioOnFire(true);
         }
    }


    
    if(get_current_wpm() != 000) {
        oled_on(); // not essential but turns on animation OLED with any alpha keypress
        if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
            anim_timer = timer_read32();
            oled_clear();
            animation_phase();
        }
        anim_sleep = timer_read32();
    } else {
        if(timer_elapsed32(anim_sleep) > OLED_TIMEOUT) {
            oled_off();
        } else {
            if(timer_elapsed32(anim_timer) > ANIM_FRAME_DURATION) {
                anim_timer = timer_read32();
                oled_clear();
                animation_phase();
            }
        }
    }
}

void oled_task_user(void) {
    if (is_keyboard_master()) {
        render_status();
    } else {
        render_anim();
        oled_set_cursor(0,6);
        sprintf(wpm_str, "WPM: %03d", get_current_wpm());
        oled_write(wpm_str, false);
    }
}
#endif

#ifdef ENCODER_ENABLE
void encoder_update_user(uint8_t index, bool clockwise) {
    switch(biton32(layer_state)){
        case 1:
            if (clockwise) {
                tap_code16(C(KC_TAB));
            } else {
                tap_code16(S(C(KC_TAB)));
            }
            break;
        default:
            if (clockwise) {
                if (!is_alt_tab_active) {
                    is_alt_tab_active = true;
                    register_code(KC_LALT);
                }
                alt_tab_timer = timer_read();
                tap_code16(KC_TAB);
            } else {
                tap_code16(S(KC_TAB));
                }
            break;
    }
}
void matrix_scan_user(void) {
    if (is_alt_tab_active) {
        if (timer_elapsed(alt_tab_timer) > 1250) {
            unregister_code(KC_LALT);
            is_alt_tab_active = false;
            }
        }
}

#endif
