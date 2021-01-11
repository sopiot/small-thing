#include <thing_client.h>
#include <Servo.h>

#define SERVO_PIN1 9
#define SERVO_PIN2 10

Servo servo1; // servo1 is connected to pin9, and controls valve1
Servo servo2; // servo2 is connected to pin10, and controls valve2

enum valve_status {
    valve_close,
    valve_open
};

#if BOARD_SERIAL_IS_ONE
ThingClient Client1("Valve_Act1", 60, Serial1);
#else
ThingClient Client1("Valve_Act1", 60, Serial);
#endif

int valve1_status = 0;
int valve2_status = 0;

void openValve1(void *pData)
{
    servo1.attach(SERVO_PIN1);
    servo1.write(0);
    delay(1000);
    valve1_status = valve_open;
    servo1.detach();
}

void closeValve1(void *pData)
{
    servo1.attach(SERVO_PIN1);
    servo1.write(90);
    delay(1000);
    valve1_status = valve_close;
    servo1.detach();
}

int get_valve1_status()
{
    return valve1_status;
}



void openValve2(void *pData)
{
    servo2.attach(SERVO_PIN2);
    servo2.write(0);
    delay(1000);
    valve2_status = valve_open;
    servo2.detach();
}

void closeValve2(void *pData)
{
    servo2.attach(SERVO_PIN2);
    servo2.write(90);
    delay(1000);
    valve2_status = valve_close;
    servo2.detach();
}

int get_valve2_status()
{
    return valve2_status;
}



void init_pin()
{
#if BOARD_SERIAL_IS_ONE
    Serial1.begin(115200);
#else
    Serial.begin(115200);
#endif

    pinMode(SERVO_PIN1, OUTPUT);
    pinMode(SERVO_PIN2, OUTPUT);
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
    servo1.attach(SERVO_PIN1);
    servo2.attach(SERVO_PIN2);
}

void init_Value()
{
    static Value VLV1_status("valve1_status", get_valve1_status, 0, 2, 1000);
    static Value VLV2_status("valve2_status", get_valve2_status, 0, 2, 1000);
    Client1.Add(VLV1_status);
    Client1.Add(VLV2_status);
}

void init_Function()
{
    static Function fnOpenValve1("valve1_water_on", openValve1, 0, 0);
    static Function fnCloseValve1("valve1_water_off", closeValve1, 0, 0);
    static Function fnOpenValve2("valve2_water_on", openValve2, 0, 0);
    static Function fnCloseValve2("valve2_water_off", closeValve2, 0, 0);

    Client1.Add(fnOpenValve1);
    Client1.Add(fnCloseValve1);
    Client1.Add(fnOpenValve2);
    Client1.Add(fnCloseValve2);
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