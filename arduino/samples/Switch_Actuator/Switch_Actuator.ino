#include <thing_client.h>
#include <Servo.h>

#define SERVO_PIN 8
#define SERVO_PIN2 9

#define CLIENT_NAME "SW_Act_1"

#define SERVO_FUNCTION_ON "servo_switch_on"
#define SERVO_FUNCTION_OFF "servo_switch_off"

#define SW_STATUS "sw_status"

Servo servo;
Servo servo2;

#if BOARD_SERIAL_IS_ONE
ThingClient Client1(CLIENT_NAME, 60, Serial1);
#else
ThingClient Client1(CLIENT_NAME, 60, Serial);
#endif

int sw_status;

void servo_function_on(void *pData)
{
    servo2.attach(SERVO_PIN2);
    int power = 1, res;
    int mid = 90, turn = 45;

    // res = GetIntArgumentByName(pData, ARG_SERVO_SWITCH, &power);
    // if (res == -1)
    //     return;

    //TODO:(thsvkd) - tuning servo.write Value to meetting room switch!!!
    servo2.write(mid - turn);
    delay(300);
    servo2.write(mid);
    sw_status = 1;
    servo2.detach();
}

void servo_function_off(void *pData)
{
    servo.attach(SERVO_PIN);
    int power = 1, res;
    int mid = 90, turn = -45;

    // res = GetIntArgumentByName(pData, ARG_SERVO_SWITCH, &power);
    // if (res == -1)
    //     return;

    //TODO:(thsvkd) - tuning servo.write Value to meetting room switch!!!
    servo.write(mid - turn);
    delay(300);
    servo.write(mid);
    sw_status = 0;
    servo.detach();
}

int get_sw_status()
{
    return sw_status;
}

void init_pin()
{
#if BOARD_SERIAL_IS_ONE
    Serial1.begin(115200);
#else
    Serial.begin(115200);
#endif

    pinMode(SERVO_PIN, OUTPUT);
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
    servo.attach(SERVO_PIN);
    servo2.attach(SERVO_PIN2);
}

void init_Value()
{
    static Value SW_status(SW_STATUS, get_sw_status, 0, 2, 1000);

    Client1.Add(SW_status);
}

void init_Function()
{
    static Function Servo_Fucntion_On(SERVO_FUNCTION_ON, servo_function_on, 0, 0);
    static Function Servo_Fucntion_Off(SERVO_FUNCTION_OFF, servo_function_off, 0, 0);

    Client1.Add(Servo_Fucntion_On);
    Client1.Add(Servo_Fucntion_Off);
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