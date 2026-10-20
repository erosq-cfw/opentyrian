#!/bin/sh
# erosq-ui Extras -> Games launcher for Tyrian. Install THIS script as
# /media/sd/opentyrian/opentyrian (the extras.tsv path) and the binary from
# build-erosq.sh beside it as opentyrian.elf. Never copy a binary over
# opentyrian: a 07-25 deploy did and silently dropped the mpd kill/relaunch.
# Kill mpd to free the DAC (no dmix) and its RAM/swap, run the game, then
# relaunch mpd -- it resumes the queue+position from its state_file (mpd.conf);
# do NOT `mpc stop` first (persists state=stop, kills the resume).
# Env/cwd are left as erosq-ui passes them (TYRIAN_DIR is compiled in).
was_up=; pidof mpd >/dev/null 2>&1 && was_up=1
[ "$was_up" ] && { killall mpd 2>/dev/null; sleep 1; }
/media/sd/opentyrian/opentyrian.elf
[ "$was_up" ] && mpd 2>/dev/null
