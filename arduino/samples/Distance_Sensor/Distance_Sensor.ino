#include <thing_client.h>

#define DISTANCE_TRIGGER_PIN 3
#define DISTANCE_RECEIVE_PIN 4

#define CLIENT_NAME "Distance_2"

#define DISTANCE_VALUE "distance"

#if BOARD_SERIAL_IS_ONE
ThingClient Client1(CLIENT_NAME, 3, Serial1);
#else
ThingClient Client1(CLIENT_NAME, 3, Serial);
#endif

int DistanceSensor()
{
    long Duration = 0;
    digitalWrite(DISTANCE_TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(DISTANCE_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(DISTANCE_TRIGGER_PIN, LOW);

    Duration = pulseIn(DISTANCE_RECEIVE_PIN, HIGH);
    int Distance_mm = ((Duration / 2.9) / 2);

    return Distance_mm;
}

void init_pin()
{
#if BOARD_SERIAL_IS_ONE
    Serial1.begin(115200);
#else
    Serial.begin(115200);
#endif

    pinMode(DISTANCE_TRIGGER_PIN, OUTPUT);
    pinMode(DISTANCE_RECEIVE_PIN, INPUT);

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
    static Value distanceValue(DISTANCE_VALUE, DistanceSensor, 0, 30000, 3000);

    Client1.Add(distanceValue);
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
