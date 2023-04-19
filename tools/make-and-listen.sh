#!/bin/bash

# Script for simultaneous development on AVR128DB48 Curiosity nano
# (I2C target) and ESP32 (I2C controller). Opens a split terminal
# with tmux, builds/flashes both controllers and listens to them
# with the serial monitor.
#
# Usage:
#   - navigate to 'tools' folder in the git repo
#
#   - connect the two microcontrollers, scripts assume they are at the
#     standard /dev/ttyUSB0 and /dev/ttyACM0 directions
#
#   - run script ./make-and-listen.sh
#
#   - the build and listen commands will now be in bash history so you can
#     repeat them
#
# Dependencies:
#   - tmux
#   - platformio (compiler patched with build dependencies for AVR128DB48)
#
# IELET2111 - Mikrokontrollerprogrammering - Project 2023

# Relative path to git repository: tools/make-and-listen.sh

session="IELET2111"

tmux has-session -t $session 2>/dev/null

if [ $? != 0 ]; then
    # If session doesn't already exist, create it
    tmux new-session -d -s $session -x- -y-
    tmux rename-window -t 1 Build
    tmux split-window -t Build -h

    # ESP32
    tmux select-pane -t 2
    tmux send-keys 'cd esp32_i2c_master_usart_bridge' C-m

    tmux send-keys 'pio run -t upload --upload-port /dev/ttyUSB0' C-m
    tmux send-keys 'pio device monitor -p /dev/ttyUSB0' C-m

    # AVR128DB48 Curiosity Nano
    tmux select-pane -t 1
    tmux send-keys 'cd ..' C-m
    tmux send-keys 'pio run -t upload --upload-port /dev/ttyACM0' C-m
    tmux send-keys 'pio device monitor -p /dev/ttyACM0' C-m

    # Attach to the session
    tmux select-window -t Build
    tmux attach-session -t $session
else
    # Session already exists, attach to it
    tmux attach-session -t $session
fi
