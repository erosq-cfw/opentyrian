#ifndef EROSQ_VIDEO_H
#define EROSQ_VIDEO_H

/* EROS Q panel output: expand OpenTyrian's 8-bit indexed frame into the
 * drm_out RGB565 scanout, letterboxed 320x200 -> 320x240. Bypasses SDL video. */

#include "SDL.h"  /* SDL_Surface */

/* Present an 8-bit palettized surface to the panel (uses rgb_palette LUT). */
void erosq_present(SDL_Surface *src);

#endif
