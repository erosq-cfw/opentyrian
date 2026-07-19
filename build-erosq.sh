#!/bin/sh
# Build OpenTyrian for the EROS Q (Ingenic X1000). Produces a stripped binary
# that presents via drm_out and reads DAP buttons via evdev. Data dir is
# compiled in (launcher gives no argv/chdir).
#
#   ./build-erosq.sh          -> deployable binary
#   ./build-erosq.sh bench    -> same + SPIKE_BENCH (auto-demo, self-exit) smoke build
set -e

BR=${BR:-buildroot-2026.05}
CC="$BR/output/host/bin/mipsel-buildroot-linux-musl-gcc"
PKG="$BR/output/host/bin/pkg-config"
STRIP="$BR/output/host/bin/mipsel-buildroot-linux-musl-strip"
READELF="$BR/output/host/bin/mipsel-buildroot-linux-musl-readelf"

DATA_DIR='/media/sd/opentyrian/data'   # Makefile wraps this in a string literal
CFLAGS="-O2 -fomit-frame-pointer -DTARGET_EROSQ"
OUT=opentyrian
if [ "$1" = "bench" ]; then
	CFLAGS="$CFLAGS -DSPIKE_BENCH"
	OUT=opentyrian-bench
fi

make clean >/dev/null 2>&1 || true
make -j"$(nproc)" CC="$CC" PKG_CONFIG="$PKG" PLATFORM=UNIX WITH_NETWORK=false \
	TYRIAN_DIR="$DATA_DIR" CFLAGS="$CFLAGS"
"$STRIP" opentyrian
[ "$OUT" != opentyrian ] && mv opentyrian "$OUT"

echo "=== $OUT ==="
ls -la "$OUT"
echo "NEEDED libs (expect only libSDL2 + libc):"
"$READELF" -d "$OUT" | grep NEEDED
