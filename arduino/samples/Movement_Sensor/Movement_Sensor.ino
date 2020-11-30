#include <thing_client.h>

#define BOARD_SERIAL_IS_ONE (defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_SAMD_ZERO)) || (defined(ARDUINO_ARCH_SAM) && !defined(ARDUINO_SAM_DUE)) || defined(ARDUINO_ARCH_MBED) || defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_PROMICRO)

#define MOVEMENT_PIN 7

#define CLIENT_NAME "Arduino_Name"

#define MOVEMENT_VALUE "movement"

#if BOARD_SERIAL_IS_ONE
ThingClient Client1(CLIENT_NAME, 3, Serial1);
#else
ThingClient Client1(CLIENT_NAME, 3, Serial);
#endif

int MovementSensor()
{
    return (int)digitalRead(MOVEMENT_PIN);
}

void init_pin()
{
#if BOARD_SERIAL_IS_ONE
    Serial1.begin(115200);
#else
    Serial.begin(115200);
#endif

    pinMode(MOVEMENT_PIN, INPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    for (int i = 0; i < 4; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
    }
}

void init_sensor()
{
}

void init_Value()
{
    static Value movementValue(MOVEMENT_VALUE, MovementSensor, 0, 30000, 3000);

    Client1.Add(movementValue);
}

void init_Function()
{
}

void setup()
{
    init_pin();
    init_sensor();

    init_Value();
    init_Function();

    Client1.Setting();
}

void loop()
{
    Client1.DoLoop();
}
