#!/bin/sh
# OpenTyrian on-device smoke: real drm_out present + ALSA audio via the bench
# build (auto-demo, self-exit). Stops the UI (frees DRM master), runs, samples
# RSS, restores the UI. No reboot.
cd /media/sd/opentyrian || exit 2
export HOME=/media/sd/opentyrian
export XDG_CONFIG_HOME=/media/sd/opentyrian/.config
export XDG_DATA_HOME=/media/sd/opentyrian/.local/share
mkdir -p "$XDG_CONFIG_HOME" "$XDG_DATA_HOME"

# Kill the 'erosq-ui || erosq-splash' wrapper first so the fallback can't refire.
for pid in $(ps w 2>/dev/null | grep 'erosq-ui ||' | grep -v grep | awk '{print $1}'); do
  kill -9 "$pid" 2>/dev/null
done
killall -9 erosq-ui erosq-splash 2>/dev/null
i=0
while [ "$(pidof erosq-ui erosq-splash 2>/dev/null | wc -w)" != 0 ] && [ $i -lt 10 ]; do
  killall -9 erosq-ui erosq-splash 2>/dev/null
  sleep 1; i=$((i + 1))
done
sleep 1
echo "UI_still_running=$(pidof erosq-ui erosq-splash 2>/dev/null | wc -w)"
echo "BASELINE_MemAvail_kB=$(awk '/MemAvailable/{print $2}' /proc/meminfo)"

SDL_AUDIODRIVER=alsa LD_LIBRARY_PATH=. ./opentyrian-bench >/tmp/ot.log 2>&1 &
P=$!
( sleep 45; kill -9 "$P" 2>/dev/null ) &
WD=$!
PEAK=0; MINAVAIL=999999
while kill -0 "$P" 2>/dev/null; do
  R=$(awk '/VmHWM/{print $2}' /proc/"$P"/status 2>/dev/null)
  [ -n "$R" ] && [ "$R" -gt "$PEAK" ] && PEAK=$R
  A=$(awk '/MemAvailable/{print $2}' /proc/meminfo)
  [ -n "$A" ] && [ "$A" -lt "$MINAVAIL" ] && MINAVAIL=$A
  sleep 1
done
wait "$P" 2>/dev/null; RC=$?
kill "$WD" 2>/dev/null

echo "PEAK_VmHWM_kB=$PEAK"
echo "MIN_MemAvail_kB=$MINAVAIL"
echo "EXIT_RC=$RC"
echo "===ot.log==="; cat /tmp/ot.log
echo "===restoring UI==="
/etc/init.d/S90erosq-ui start
sleep 3
echo "UI_back=$(pidof erosq-ui | wc -w)"
