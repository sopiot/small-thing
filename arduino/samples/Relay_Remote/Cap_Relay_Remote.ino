#include <thing_client.h>

#define BOARD_SERIAL_IS_ONE (defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_SAMD_ZERO)) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_MBED) || defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_PROMICRO)

#define RELAY_PIN 5

char CLIENT_NAME[16] = "R2";

#define RELAY_ON_FUNCTION "relay_on_switch"
#define RELAY_OFF_FUNCTION "relay_off_switch"

#define ARG_RELAY_SWITCH "arg_relay_switch"

#if BOARD_SERIAL_IS_ONE
ThingClient Client1(CLIENT_NAME, 3, Serial1);
#else
ThingClient Client1(CLIENT_NAME, 3, Serial);
#endif

int Switch = HIGH;
//void relay_on_function(void *pData)
//{
//    digitalWrite(RELAY_PIN, HIGH);
//}
//
//void relay_off_function(void *pData)
//{
//    digitalWrite(RELAY_PIN, LOW);
//}

void relay_function(void *pData)
{
    Serial.print("Switch = ");
    Serial.println(Switch);
    digitalWrite(RELAY_PIN, Switch);
    Switch = !Switch;
}

void init_pin()
{
#if BOARD_SERIAL_IS_ONE
    Serial1.begin(115200);
#else
    Serial.begin(115200);
#endif

    pinMode(RELAY_PIN, OUTPUT);
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
}

void init_Function()
{
    static Function Relay_Function("Rfunc", relay_function, 0, 0);

    Client1.Add(Relay_Function);
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
