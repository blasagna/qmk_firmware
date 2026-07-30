# bob36 — Corne 36-key keymap

A 36-key (3x5+3) layout on `crkbd/rev1`, using the outer columns as `KC_NO`.
Home row mods on both hands, GACS/SCAG order:

```
left:   A(GUI)  S(Alt)  D(Ctrl)  F(Shift)
right:  J(Shift) K(Ctrl) L(Alt)   ;(GUI)
```

Build: `qmk compile -kb crkbd/rev1 -km bob36`

## Tap-hold configuration

Home row mods have two failure modes, and they share a root cause: nothing in
the config distinguishes "I'm typing" from "I'm holding a modifier."

1. **Accidental modifiers.** A rolled press resolves as a hold.
2. **Perceived input lag.** With an undecided mod-tap, the letter isn't emitted
   until you physically *release* the key.

`PERMISSIVE_HOLD` alone doesn't solve either, because it fires only on a nested
press-*and*-release. A normal roll (`a` down, `s` down, `a` up, `s` up) is not
nested, so the tap-hold key stays undecided for the full tapping term.

The current config layers three rules:

| Setting | File | Purpose |
| --- | --- | --- |
| `FLOW_TAP_TERM 150` | `config.h` | A mod-tap pressed within 150 ms of the previous alpha is sent **immediately** as a tap. Disables hold behavior during fast typing — this is the latency fix. |
| `CHORDAL_HOLD` | `config.h` | "Opposite hands" rule: a mod-tap chorded with a **same-hand** key inside the tapping term settles as a tap. Fixes same-hand rolls firing mods. |
| `PERMISSIVE_HOLD` | `config.h` | Governs the cross-hand case. Chordal Hold requires either this or `HOLD_ON_OTHER_KEY_PRESS`. |
| `TAPPING_TERM 200` | `config.h` | With the above doing the disambiguation, the term no longer has to out-wait fast rolls. |
| `QUICK_TAP_TERM 0` | `config.h` | Disables tap-then-hold auto-repeat, which otherwise defeats home row mods. |

### Handedness table

`keymap.c` defines `chordal_hold_layout` explicitly rather than relying on the
one QMK auto-generates. The generated table (visible at
`.build/obj_crkbd_rev1_bob36/src/default_keyboard.c` after a build) marks the
thumb keys `'L'`/`'R'`, which would break same-hand chords like
left-Ctrl + left-thumb Backspace (delete word). The six thumbs are `'*'`
(exempt from the opposite-hands rule) here instead.

### Known tradeoff

Chordal Hold means **same-hand shortcuts require deliberately holding past the
tapping term**. `Ctrl+A` is `LCTL_T(KC_D)` + `LGUI_T(KC_A)`, both left hand —
press-and-roll produces `da`; hold `D` for 200 ms first and it works as
intended. This is inherent to the opposite-hands rule and generally adapts away
within a day or two.

### Tuning

Roughly in the order worth reaching for:

- Still catching stray mods on fast rolls → raise `FLOW_TAP_TERM` toward 175–200.
- Letters dropped when you *meant* to hold → lower it toward 120.
- Want cross-hand shortcuts to resolve instantly → swap `PERMISSIVE_HOLD` for
  `HOLD_ON_OTHER_KEY_PRESS`. Not the default here: cross-hand rolls are constant
  in English (`th`, `he`), and outside the Flow Tap window that config turns
  them into modifier presses.

## Latency / scan configuration

- **`DEBOUNCE_TYPE = asym_eager_defer_pk`** (`rules.mk`) with **`DEBOUNCE 5`**
  (`config.h`). QMK's default is `sym_defer_g`, which delays *every* press by
  `DEBOUNCE` ms and restarts that timer on any other key's activity. The eager
  variant reports key-down immediately and debounces only the release, so
  `DEBOUNCE 5` here is both lower latency and better chatter protection than a
  lower value under the default algorithm.
- `LTO_ENABLE = yes` — flash size only, no latency effect. Build sits at
  ~22 KB / 28 KB (76%) on the ATmega32U4.
- USB polling is already 1 ms by default on this platform
  (`tmk_core/protocol/usb_descriptor.c`); nothing to gain there.

### Not enabled

`crkbd/info.json` sets `features.oled: true` and this keymap does not override
it, so OLED support is compiled in. If no OLEDs are attached, adding
`OLED_ENABLE = no` to `rules.mk` drops the per-scan I2C traffic and frees flash.

## Note on invalid options

`SQUEEZE_FAST_TYPING` was previously defined in `config.h`. **It is not a QMK
option** — no such symbol exists anywhere in the firmware tree, so it was a
silent no-op. Removed. Worth verifying any tap-hold define against
`docs/tap_hold.md` before adding it; unrecognized `#define`s fail silently.
