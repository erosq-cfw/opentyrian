/* Host unit test for erosq_map_key (pure).
 * Build: cc -I<sysroot>/usr/include/SDL2 test/test_erosq_map.c src/erosq_keymap.c -o /tmp/tm && /tmp/tm */
#include "../src/erosq_keymap.h"
#include <assert.h>
#include <stdio.h>

int main(void)
{
	assert(erosq_map_key(115) == SDL_SCANCODE_UP);      /* vol+  */
	assert(erosq_map_key(114) == SDL_SCANCODE_DOWN);    /* vol-  */
	assert(erosq_map_key(165) == SDL_SCANCODE_LEFT);    /* prev  */
	assert(erosq_map_key(163) == SDL_SCANCODE_RIGHT);   /* next  */
	assert(erosq_map_key(164) == SDL_SCANCODE_RETURN);  /* play  */
	assert(erosq_map_key(139) == SDL_SCANCODE_LCTRL);   /* menu  */
	assert(erosq_map_key(158) == SDL_SCANCODE_ESCAPE);  /* back  */
	assert(erosq_map_key(116) == SDL_SCANCODE_UNKNOWN); /* power: left to OS */
	assert(erosq_map_key(0)   == SDL_SCANCODE_UNKNOWN);
	assert(erosq_map_key(9999) == SDL_SCANCODE_UNKNOWN);

	printf("test_erosq_map: PASS\n");
	return 0;
}
