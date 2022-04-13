#!/bin/bash

function os_detect() {
  if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    os_release=$(cat /etc/os-release | grep "^PRETTY_NAME=")
    if [[ "$os_release" == *"Raspbian"* ]] || [[ "$os_release" == *"Debian"* ]]; then
      echo "pi"
    elif [[ "$os_release" == *"Ubuntu"* ]]; then
      echo "ubuntu"
    else
      echo "not linux-gnu"
    fi
  elif [[ "$OSTYPE" == "darwin"* ]]; then
    os_release=$(sw_vers)
    if [[ "$os_release" == *"macOS"* ]]; then
      echo "macOS"
    else
      echo "not macOS"
    fi
  elif [[ "$OSTYPE" == "cygwin" ]]; then
    echo "cygwin"
  elif [[ "$OSTYPE" == "msys" ]]; then
    echo "msys"
  elif [[ "$OSTYPE" == "win32" ]]; then
    echo "windows"
  elif [[ "$OSTYPE" == "freebsd"* ]]; then
    echo "freebsd"
  else
    echo "unknown"
  fi
}

function ubuntu_upload {
  board_port=$1
  app=$2
  cp -rf ../libraries/SoPIoT/ ~/Arduino/libraries

  arduino-cli compile --fqbn arduino:samd:nano_33_iot $app -v
  arduino-cli upload -p $board_port --fqbn arduino:samd:nano_33_iot $app
  arduino-cli monitor -p $board_port
}

function macOS_upload {
  board_port=$1
  app=$2
  cp -rf ../libraries/SoPIoT/ ~/Documents/Arduino/libraries

  arduino-cli compile --fqbn arduino:samd:nano_33_iot $app -v
  arduino-cli upload -p $board_port --fqbn arduino:samd:nano_33_iot $app
  arduino-cli monitor -p $board_port
}

function upload() {
  board_port=$1
  app=$2

  OS=$(os_detect)
  if [[ "$OS" == "pi" ]]; then
    ubuntu_upload $board_port $app
  elif [[ "$OS" == "ubuntu" ]]; then
    ubuntu_upload $board_port $app
  elif [[ "$OS" == "macOS" ]]; then
    macOS_upload $board_port $app
  elif [[ "$OS" == "cygwin" ]]; then
    echo "not supported OS"
  elif [[ "$OS" == "msys" ]]; then
    echo "not supported OS"
  elif [[ "$OS" == "windows" ]]; then
    echo "not supported OS"
  elif [[ "$OS" == "freebsd" ]]; then
    echo "not supported OS"
  elif [[ "$OS" == "unknown" ]]; then
    echo "not supported OS"
  else
    echo "OS detect failed..."
  fi
}

function usage {
  echo "Usage: $0 [board_port] [app]"
  echo "  board_port: Arduino board port"
  echo "  app: Arduino application path"
}

if [ $# -lt 2 ]; then
  usage
else
  upload $1 $2
fi
