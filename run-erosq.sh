#!/bin/sh
# HITL runner: stop the UI (free DRM master), launch the real OpenTyrian on the
# panel, and restore the UI when you quit the game (title menu -> Quit).
cd /media/sd/opentyrian || exit 2
export HOME=/media/sd/opentyrian
export XDG_CONFIG_HOME=/media/sd/opentyrian/.config
export XDG_DATA_HOME=/media/sd/opentyrian/.local/share
mkdir -p "$XDG_CONFIG_HOME" "$XDG_DATA_HOME"

for pid in $(ps w 2>/dev/null | grep 'erosq-ui ||' | grep -v grep | awk '{print $1}'); do
  kill -9 "$pid" 2>/dev/null
done
killall -9 erosq-ui erosq-splash 2>/dev/null
i=0
while [ "$(pidof erosq-ui erosq-splash 2>/dev/null | wc -w)" != 0 ] && [ $i -lt 10 ]; do
  killall -9 erosq-ui erosq-splash 2>/dev/null; sleep 1; i=$((i + 1))
done

SDL_AUDIODRIVER=alsa LD_LIBRARY_PATH=. ./opentyrian
RC=$?

echo "opentyrian exited rc=$RC; restoring UI"
/etc/init.d/S90erosq-ui start
