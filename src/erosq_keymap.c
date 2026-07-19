#include "erosq_keymap.h"

/* EROS Q physical buttons -> SDL scancodes (approved control scheme).
 * Move on Prev/Next/Vol; Play=fire/confirm, Menu=change-rear-weapon,
 * Back=in-game menu/cancel. Power (116) is left to the OS. Numeric evdev
 * codes hardcoded to keep this file dependency-free for host testing. */
SDL_Scancode erosq_map_key(int evcode)
{
	switch (evcode)
	{
	case 115: return SDL_SCANCODE_UP;     /* KEY_VOLUMEUP    */
	case 114: return SDL_SCANCODE_DOWN;   /* KEY_VOLUMEDOWN  */
	case 165: return SDL_SCANCODE_LEFT;   /* KEY_PREVIOUSSONG */
	case 163: return SDL_SCANCODE_RIGHT;  /* KEY_NEXTSONG    */
	case 164: return SDL_SCANCODE_RETURN; /* KEY_PLAYPAUSE -> fire / confirm */
	case 139: return SDL_SCANCODE_LCTRL;  /* KEY_MENU -> change rear weapon  */
	case 158: return SDL_SCANCODE_ESCAPE; /* KEY_BACK -> in-game menu / cancel */
	default:  return SDL_SCANCODE_UNKNOWN;
	}
}
