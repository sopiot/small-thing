from digi.xbee.devices import XBeeDevice
from digi.xbee.util import utils
from func_timeout import func_timeout, FunctionTimedOut

# TODO: Replace with the serial port where your local module is connected to.
PORT = "/dev/ttyUSB0"
# TODO: Replace with the baud rate of your local module.
BAUD_RATE = 9600

PARAM_NODE_ID = "NI"
PARAM_PAN_ID = "ID"
PARAM_BAUD_RATE = "BD"
PARAM_DEST_ADDRESS_H = "DH"
PARAM_DEST_ADDRESS_L = "DL"

PARAM_VALUE_NODE_ID = "Yoda"
PARAM_VALUE_PAN_ID = utils.hex_string_to_bytes("1234")
PARAM_VALUE_DEST_ADDRESS_H = utils.hex_string_to_bytes("00")
PARAM_VALUE_DEST_ADDRESS_L = utils.hex_string_to_bytes("FFFF")


def get_xbee_device():
    for baud_rate in [9600, 19200, 38400, 57600, 115200, 230400, 460800, 921600]:
        try:
            device = XBeeDevice(PORT, baud_rate)
            func_timeout(timeout=0.3, func=device.open)
            print(f"Connected to xbee device with {baud_rate}")
            return device
        except Exception as e:
            device.close()
        except FunctionTimedOut as e:
            device.close()


def main():
    print(" +-----------------------------------------------+")
    print(" | XBee Python Library Set/Get parameters Sample |")
    print(" +-----------------------------------------------+\n")

    device = get_xbee_device()

    try:
        # Set parameters.
        device.set_parameter(PARAM_NODE_ID, bytearray('thsvkd_test', 'utf8'))
        device.set_parameter(PARAM_PAN_ID, utils.hex_string_to_bytes("12"))
        device.set_parameter(PARAM_BAUD_RATE, utils.int_to_bytes(7))
        device.close()
        device = get_xbee_device()

        # Get parameters.
        print("Node ID:                     %s" %
              device.get_parameter(PARAM_NODE_ID).decode())
        print("PAN ID:                      %s" %
              utils.hex_to_string(device.get_parameter(PARAM_PAN_ID)))
        print("BAUD RATE:                   %s" %
              utils.hex_to_string(device.get_parameter(PARAM_BAUD_RATE)))

        print("")
        print("All parameters were set correctly!")

    finally:
        if device is not None and device.is_open():
            device.close()


if __name__ == '__main__':
    main()
