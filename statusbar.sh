#!/bin/sh

get_cpu() {
    CPU_USAGE=$(top -bn1 \
                | awk '/Cpu\(s\)/{print 100-$8"%"}')
    echo " $CPU_USAGE"
}

get_mem() {
    MEM_USAGE=$(free -h \
                | awk '/^Mem:/ {print $3"/"$2}')
    echo " $MEM_USAGE"
}

get_vol() {
    DEFAULT_SINK=$(pactl info | awk -F': ' '/Default Sink/ {print $2}')
    VOLUME=$(pactl get-sink-volume "$DEFAULT_SINK" \
             | awk '/Volume:/ {print $5; exit}')
    MUTE=$(pactl get-sink-mute "$DEFAULT_SINK" \
           | awk -F': ' '/Mute:/ {print $2}')
    if [ "$MUTE" = "no" ]; then
        echo " $VOLUME"
    else
        echo " Muted"
    fi
}

get_date() {
    echo " $(date '+%Y-%m-%d (%a) %H:%M:%S')"
}


get_net() {
    IF="eth0"  # ← change to your interface name
    # read initial counters
    RX1=$(cat /sys/class/net/$IF/statistics/rx_bytes)
    TX1=$(cat /sys/class/net/$IF/statistics/tx_bytes)
    sleep 1
    # read again after 1s
    RX2=$(cat /sys/class/net/$IF/statistics/rx_bytes)
    TX2=$(cat /sys/class/net/$IF/statistics/tx_bytes)
    # calculate bytes per second
    RX_RATE=$((RX2 - RX1))
    TX_RATE=$((TX2 - TX1))
    # convert to KB/s
    RX_KB=$((RX_RATE / 1024))
    TX_KB=$((TX_RATE / 1024))
    # ↓ and ↑ are basic Unicode arrows—should work in most fonts
    echo "⬇ ${RX_KB}KB/s ⬆ ${TX_KB}KB/s"
}


while true; do
   STATUS="$(get_cpu) | $(get_mem) | $(get_vol) | $(get_date) | $(get_net)"
    xsetroot -name " $STATUS "
    sleep 2
done



