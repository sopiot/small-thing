#include <thing_client.h>

#define BOARD_SERIAL_IS_ONE (defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_SAMD_ZERO)) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_MBED) || defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_PROMICRO)

#define LIGHT_PIN A0

#define CLIENT_NAME "Light_1"
#define LIGHT_VALUE "light"

#if BOARD_SERIAL_IS_ONE
ThingClient Client(CLIENT_NAME, 3, Serial1);
#else
ThingClient Client(CLIENT_NAME, 3, Serial);
#endif

int LightSensor()
{
    return (int)analogRead(LIGHT_PIN);
}

void init_pin()
{
#if BOARD_SERIAL_IS_ONE
    Serial1.begin(115200);
#else
    Serial.begin(115200);
#endif

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
    static Value lightValue(LIGHT_VALUE, LightSensor, 0, 2000, 3000);

    Client.Add(lightValue);
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

    Client.Setting();
}

void loop()
{
    Client.DoLoop();
}
