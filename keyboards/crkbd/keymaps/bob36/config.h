/*
This is the c configuration file for the keymap

Copyright 2012 Jun Wako <wakojun@gmail.com>
Copyright 2015 Jack Humbert

This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

#pragma once

//#define USE_MATRIX_I2C

#define TAPPING_TERM 200
#define QUICK_TAP_TERM 0  // value 0 disables quick tap term
#define PERMISSIVE_HOLD

// --- Home row mod disambiguation ---
// Opposite-hands rule: a mod-tap chorded with a key on the SAME hand within the
// tapping term settles as a tap. Kills same-hand rolls firing modifiers.
#define CHORDAL_HOLD
// During fast typing, a mod-tap pressed within 150ms of the previous alpha is
// sent immediately as a tap. Removes the release-delay on rolls.
#define FLOW_TAP_TERM 150

// --- Hardware Optimization ---
// Paired with DEBOUNCE_TYPE = asym_eager_defer_pk in rules.mk: key-down is
// reported immediately, so this only gates key-up / chatter suppression.
#define DEBOUNCE 5

#ifdef RGBLIGHT_ENABLE
    #define RGBLIGHT_EFFECT_BREATHING
    #define RGBLIGHT_EFFECT_RAINBOW_MOOD
    #define RGBLIGHT_EFFECT_RAINBOW_SWIRL
    #define RGBLIGHT_EFFECT_SNAKE
    #define RGBLIGHT_EFFECT_KNIGHT
    #define RGBLIGHT_EFFECT_CHRISTMAS
    #define RGBLIGHT_EFFECT_STATIC_GRADIENT
    #define RGBLIGHT_EFFECT_RGB_TEST
    #define RGBLIGHT_EFFECT_ALTERNATING
    #define RGBLIGHT_EFFECT_TWINKLE
    #define RGBLIGHT_LIMIT_VAL 120
    #define RGBLIGHT_HUE_STEP 10
    #define RGBLIGHT_SAT_STEP 17
    #define RGBLIGHT_VAL_STEP 17
#endif
