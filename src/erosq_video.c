#include "erosq_video.h"
#include "erosq_blit.h"
#include "drm_out.h"
#include "palette.h"   /* rgb_palette[256], RGB565 once main_window_tex_format is RGB565 */
#include <string.h>

void erosq_present(SDL_Surface *src)
{
	int stride;
	uint16_t *fb = drm_pixels(&stride);
	if (!fb)
		return;

	static int bars_cleared = 0;
	int y_off = (DRM_OUT_H - src->h) / 2;   /* (240-200)/2 = 20 */

	if (!bars_cleared)
	{
		/* Zero the whole frame once so the letterbox bars stay black; the
		 * game area is overwritten every frame below. */
		for (int y = 0; y < DRM_OUT_H; ++y)
			memset(fb + (size_t)y * stride, 0, (size_t)DRM_OUT_W * 2);
		bars_cleared = 1;
	}

	erosq_expand565(fb, stride, (const uint8_t *)src->pixels,
	                src->w, src->h, y_off, rgb_palette);
	drm_present();
}
