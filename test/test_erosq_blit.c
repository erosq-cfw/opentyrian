/* Host unit test for erosq_expand565 (pure, dep-free).
 * Build: cc test/test_erosq_blit.c src/erosq_blit.c -o /tmp/tb && /tmp/tb */
#include "../src/erosq_blit.h"
#include <assert.h>
#include <string.h>
#include <stdio.h>

int main(void)
{
	/* LUT: index i -> 0xF000 | i, so we can tell mapped pixels from raw. */
	uint32_t lut[256];
	for (int i = 0; i < 256; ++i)
		lut[i] = 0xF000u | (uint32_t)i;

	/* 3x2 source (odd width exercises the trailing single-pixel path). */
	const int W = 3, H = 2;
	uint8_t src[6] = { 1, 2, 3,
	                   4, 5, 6 };

	/* Destination 4 px wide (stride > width), 6 rows tall, y_off = 2. */
	const int DW = 4, DH = 6, YOFF = 2;
	uint16_t dst[DW * DH];
	memset(dst, 0, sizeof dst);   /* caller pre-clears (letterbox bars) */

	erosq_expand565(dst, DW, src, W, H, YOFF, lut);

	/* Active rows carry lut[index]. */
	for (int y = 0; y < H; ++y)
		for (int x = 0; x < W; ++x)
			assert(dst[(y + YOFF) * DW + x] == (uint16_t)(0xF000u | src[y * W + x]));

	/* Bars above (rows 0,1) and below (rows 4,5) stay 0. */
	for (int x = 0; x < DW; ++x)
	{
		assert(dst[0 * DW + x] == 0);
		assert(dst[1 * DW + x] == 0);
		assert(dst[4 * DW + x] == 0);
		assert(dst[5 * DW + x] == 0);
	}

	/* No write past src_w within an active row: col 3 (>= W) stays 0. */
	assert(dst[(0 + YOFF) * DW + 3] == 0);
	assert(dst[(1 + YOFF) * DW + 3] == 0);

	printf("test_erosq_blit: PASS\n");
	return 0;
}
