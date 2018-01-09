#ifndef KEYBOARDS_ERGODOX_CONFIG_H_
#define KEYBOARDS_ERGODOX_CONFIG_H_

#include QMK_KEYBOARD_CONFIG_H

#undef MOUSEKEY_TIME_TO_MAX
#undef MOUSEKEY_MAX_SPEED

#define MOUSEKEY_TIME_TO_MAX 1
#define MOUSEKEY_MAX_SPEED 2

#undef MOUSEKEY_DELAY
#define MOUSEKEY_DELAY 0

#undef MOUSEKEY_WHEEL_DELAY
#define MOUSEKEY_WHEEL_DELAY 0

#define ONESHOT_TAP_TOGGLE 2
#define ONESHOT_TIMEOUT 3000

#undef LOCKING_SUPPORT_ENABLE
#undef LOCKING_RESYNC_ENABLE

#undef LEADER_TIMEOUT
#define LEADER_TIMEOUT 1000

#define TAPPING_TOGGLE  1

#undef TAPPING_TERM
#define TAPPING_TERM    300
#define IGNORE_MOD_TAP_INTERRUPT // this makes it possible to do rolling combos (zx) with keys that convert to other keys on hold (z becomes ctrl when you hold it, and when this option isn't enabled, z rapidly followed by x actually sends Ctrl-x. That's bad.)

#endif
