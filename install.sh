#!/bin/bash

function common_init() {
    arduino-cli config init
    arduino-cli core update-index
}

function install_lib() {
    arduino-cli lib install ArduinoJson
    arduino-cli lib install "DHT sensor library"
    arduino-cli lib install "XBee-Arduino library"
    arduino-cli lib install "PM2008 I2C"
    # cp -rf arduino/libraries/SoPIoT ~/Documents/Arduino/libraries
}

function install_board() {
    arduino-cli core install arduino:samd
    arduino-cli core install arduino:sam
    arduino-cli core install arduino:mbed
}

function list_config() {
    arduino-cli board list
    arduino-cli core list
    arduino-cli lib list
}

function install() {
    if [[ "$OSTYPE" == "linux-gnu"* ]]; then
        os_release=$(cat /etc/os-release | grep "^PRETTY_NAME=")
        if [[ "$os_release" == *"Raspbian"* ]]; then
            echo "Raspberry Pi detected"
            ubuntu_install
        elif [[ "$os_release" == *"Ubuntu"* ]]; then
            echo "Ubuntu detected"
            ubuntu_install
        fi
    elif [[ "$OSTYPE" == "darwin"* ]]; then
        os_release=$(sw_vers)
        if [[ "$os_release" == *"macOS"* ]]; then
            echo "MacOS detected"
            macOS_install
        fi
    else
        echo "Unknown OS... install failed"
        exit 1
    fi
}

function check_install() {
    arduino-cli help
    if [[ $? -eq 0 ]]; then
        echo "Arduino CLI installed"
        return 1
    else
        echo "Arduino CLI not installed"
        return 0
    fi
}

function macOS_install() {
    if ! check_install; then
        echo "Installing Arduino CLI"
        brew install arduino-cli
    else
        echo "Upgrade Arduino CLI"
        brew upgrade arduino-cli
    fi

    common_init

    list_config
    install_board
    install_lib
    cp -rf arduino/libraries/SoPIoT ~/Documents/Arduino/libraries
}

function ubuntu_install() {
    curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
    sudo mv bin/arduino-cli /usr/local/bin/
    rm -rf bin

    common_init

    list_config
    install_board
    install_lib
    cp -rf arduino/libraries/SoPIoT ~/Arduino/libraries
}

install
