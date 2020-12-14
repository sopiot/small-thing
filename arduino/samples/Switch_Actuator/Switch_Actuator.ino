#include <thing_client.h>
#include <Servo.h>

#define BOARD_SERIAL_IS_ONE (defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_SAMD_ZERO)) || (defined(ARDUINO_ARCH_SAM) && !defined(ARDUINO_SAM_DUE)) || defined(ARDUINO_ARCH_MBED) || defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_PROMICRO)

#define SERVO_PIN 8

#define CLIENT_NAME "SW_Act_1"

#define SERVO_FUNCTION_ON "servo_switch_on"
#define SERVO_FUNCTION_OFF "servo_switch_off"

Servo servo;

#if BOARD_SERIAL_IS_ONE
ThingClient Client1(CLIENT_NAME, 3, Serial1);
#else
ThingClient Client1(CLIENT_NAME, 3, Serial);
#endif

void servo_function_on(void *pData)
{
    int power = 1, res;
    int stop = 90, right_rotate = 200, left_rotate = 0;

    // res = GetIntArgumentByName(pData, ARG_SERVO_SWITCH, &power);
    // if (res == -1)
    //     return;  

    //TODO(thsvkd) - tuning servo.write Value to meetting room switch!!!
    servo.write(left_rotate);
    delay(300);
    servo.write(stop);
}

void servo_function_off(void *pData)
{
    int power = 1, res;
    int stop = 90, right_rotate = 200, left_rotate = 0;

    // res = GetIntArgumentByName(pData, ARG_SERVO_SWITCH, &power);
    // if (res == -1)
    //     return;

    //TODO(thsvkd) - tuning servo.write Value to meetting room switch!!!
    servo.write(right_rotate);
    delay(300);
    servo.write(stop);
}

void init_pin()
{
#if BOARD_SERIAL_IS_ONE
    Serial1.begin(115200);
#else
    Serial.begin(115200);
#endif

    pinMode(SERVO_PIN, OUTPUT);
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
}

void init_Value()
{
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
