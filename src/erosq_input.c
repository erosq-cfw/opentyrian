#include "erosq_input.h"
#include "erosq_keymap.h"
#include "keyboard.h"      /* keysactive[] */

#include <fcntl.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <linux/input.h>   /* struct input_event, EV_KEY, EV_REL, REL_X */

#define MAXFDS 10
static int fds[MAXFDS];
static int nfds;

/* The scroll wheel emits EV_REL/REL_X ticks with no release event, so we fake
 * a one-frame press: set the scancode this poll, clear it next poll. */
static SDL_Scancode wheel_pulse = SDL_SCANCODE_UNKNOWN;

void erosq_input_init(void)
{
	char path[32];
	for (int i = 0; i < MAXFDS; ++i)
	{
		snprintf(path, sizeof path, "/dev/input/event%d", i);
		int fd = open(path, O_RDONLY | O_NONBLOCK | O_CLOEXEC);
		if (fd >= 0)
			fds[nfds++] = fd;
	}
	if (getenv("EROSQ_INDBG"))
		fprintf(stderr, "erosq_input: opened %d event fds\n", nfds);
}

void erosq_input_poll(void)
{
	/* Release the previous frame's wheel pulse. ponytail: if a real d-pad key
	 * shares this scancode and is held, it drops for one frame; harmless for
	 * menu scrolling, revisit only if it bites in gameplay. */
	if (wheel_pulse != SDL_SCANCODE_UNKNOWN)
	{
		erosq_feed_key(wheel_pulse, SDLK_UNKNOWN, false);
		wheel_pulse = SDL_SCANCODE_UNKNOWN;
	}

	struct input_event ev;
	for (int i = 0; i < nfds; ++i)
	{
		while (read(fds[i], &ev, sizeof ev) == (ssize_t)sizeof ev)
		{
			if (getenv("EROSQ_INDBG") && ev.type != EV_SYN)
				fprintf(stderr, "erosq_input: fd%d type=%d code=%d val=%d -> sc=%d\n",
				        i, ev.type, ev.code, ev.value, erosq_map_key(ev.code));
			if (ev.type == EV_KEY)
			{
				if (ev.value == 2)   /* autorepeat */
					continue;
				/* sym=UNKNOWN: menus/fire dispatch on scancode; sym is only
				 * for typed cheat codes, which these buttons can't produce. */
				erosq_feed_key(erosq_map_key(ev.code), SDLK_UNKNOWN, ev.value != 0);
			}
			else if (ev.type == EV_REL && ev.code == REL_X)
			{
				SDL_Scancode sc = ev.value > 0 ? SDL_SCANCODE_DOWN : SDL_SCANCODE_UP;
				erosq_feed_key(sc, SDLK_UNKNOWN, true);
				wheel_pulse = sc;
			}
		}
	}
}
