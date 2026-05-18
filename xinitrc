#!/bin/sh

# 1. Common setup: Runs no matter which environment you boot
xrandr --output DP-3 --rotate right

# Start Audio
pipewire &
pipewire-pulse &
wireplumber &

# 2. Session Switcher
# Defaults to "dwm" if the $DESKTOP variable is empty
SESSION=${DESKTOP:-dwm}

case "$SESSION" in
    kde|plasma)
        # KDE Plasma handles its own bars and widgets
        exec dbus-run-session startplasma-x11
        ;;
    dwm)
        # Only start the dwm statusbar if we are actually using dwm
        ~/.dwm/statusbar.sh &
        exec dwm
        ;;
    *)
        echo "Unknown session: $SESSION. Falling back to dwm."
        ~/.dwm/statusbar.sh &
        exec dwm
        ;;
esac
