#!/bin/bash

function common_init() {
    arduino-cli config init
    arduino-cli core update-index
}

function install_dependence_lib() {
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

function check_install() {
    arduino-cli help > /dev/null 2>&1
    if [ $? -eq 0 ]; then
        echo "Arduino CLI already installed"
        return 1
    else
        echo "Arduino CLI not installed"
        return 0
    fi
}

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

function install() {
    OS=$(os_detect)
    echo $OS
    if [[ "$OS" == "pi" ]]; then
        ubuntu_install
    elif [[ "$OS" == "ubuntu" ]]; then
        ubuntu_install
    elif [[ "$OS" == "macOS" ]]; then
        macOS_install
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

function macOS_install() {
    check_install
    if [ $? -eq 0 ]; then
        echo "Installing Arduino CLI..."
        brew install arduino-cli
        common_init
    else
        exit 1
    fi

    list_config
    install_board
    install_dependence_lib
    cp -rf arduino/libraries/SoPIoT ~/Documents/Arduino/libraries
}

function ubuntu_install() {
    check_install
    if [ $? -eq 0 ]; then
        echo "Installing Arduino CLI..."
        curl -fsSL https://raw.githubusercontent.com/arduino/arduino-cli/master/install.sh | sh
        sudo mv bin/arduino-cli /usr/local/bin/
        rm -rf bin

        common_init

        list_config
        install_board
        install_dependence_lib
        cp -rf arduino/libraries/SoPIoT ~/Arduino/libraries
    else
        exit 1
    fi
}

install
