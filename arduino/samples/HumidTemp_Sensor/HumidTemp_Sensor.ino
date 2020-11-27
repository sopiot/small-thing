#include <thing_client.h>
#include <DHT.h>

#define BOARD_SERIAL_IS_ONE (defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_SAMD_ZERO)) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_MBED) || defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_PROMICRO)

#define DHT_TYPE DHT11
//#define DHT_TYPE DHT22

#define HUMID_PIN 2

#define CLIENT_NAME "Humid_3"
#define HUMID_VALUE "humid"
#define TEMP_VALUE "temp"

#if BOARD_SERIAL_IS_ONE
ThingClient Client1(CLIENT_NAME, 3, Serial1);
#else
ThingClient Client1(CLIENT_NAME, 3, Serial);
#endif

DHT dht(HUMID_PIN, DHT_TYPE);

int HumidSensor()
{
    return (int)dht.readHumidity();
}

int TempSensor()
{
    return (int)dht.readTemperature();
}

void init_pin()
{
#if BOARD_SERIAL_IS_ONE
    Serial1.begin(115200);
#else
    Serial.begin(115200);
#endif

    pinMode(HUMID_PIN, INPUT);
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
    dht.begin();
}

void init_Value()
{
    static Value humidValue(HUMID_VALUE, HumidSensor, 0, 200, 3000);
    static Value tempValue(TEMP_VALUE, TempSensor, 0, 30000, 3000);

    Client1.Add(humidValue);
    Client1.Add(tempValue);
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
