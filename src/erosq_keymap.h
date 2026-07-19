#ifndef EROSQ_KEYMAP_H
#define EROSQ_KEYMAP_H
#include "SDL_scancode.h"

/* Map a Linux evdev key code (EV_KEY) to the SDL scancode OpenTyrian polls,
 * or SDL_SCANCODE_UNKNOWN if unmapped. Pure -> host-testable. */
SDL_Scancode erosq_map_key(int evcode);

#endif
