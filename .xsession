#!/bin/sh

# Start the MATE settings daemon to apply your themes
mate-settings-daemon &

# 1. Set your background image (e.g., with feh or nitrogen)
feh --bg-scale /home/ben/image.png &

# 2. Start the compositor for transparency
picom &

# 3. Start the status bar script from your home folder
/home/ben/.config/dwm/statusbar.sh &

# 4. Execute dwm (this MUST be the last command)
exec dwm
