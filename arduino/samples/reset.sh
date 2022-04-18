#!/bin/bash

pin=$1
sleep_time=0.1
gpio -g mode $pin output
gpio -g write $pin 1
sleep $sleep_time
gpio -g write $pin 0
sleep $sleep_time
gpio -g write $pin 1
