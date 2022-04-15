#!/bin/bash

function usage {
  echo "Usage: $0 [board_port] [app]"
  echo "  board_port: Arduino board port"
  echo "  app: Arduino application path"
}

if [ $# -lt 1 ]; then
  usage
else
  board_port=$1
  arduino-cli monitor -p $board_port
fi
