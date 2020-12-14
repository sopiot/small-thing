#include <thing_client.h>

#define BOARD_SERIAL_IS_ONE (defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_SAMD_ZERO)) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_MBED) || defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_PROMICRO)

#define SEND_PIN 6

#define CLIENT_NAME "IR_3"

#define IR_TV_ONOFF_FUNCTION "ir_TV_onoff_switch"
#define IR_AIR_ON_FUNCTION "ir_air_on_switch"
#define IR_AIR_OFF_FUNCTION "ir_air_off_switch"
#define IR_BULB_ON_FUNCTION "ir_bulb_on_switch"
#define IR_BULB_OFF_FUNCTION "ir_bulb_off_switch"

#if BOARD_SERIAL_IS_ONE
ThingClient Client1(CLIENT_NAME, 3, Serial1);
#else
ThingClient Client1(CLIENT_NAME, 3, Serial);
#endif

unsigned int TV_ONOFF[] = {9080, 4420, 600, 516, 584, 524, 568, 1640, 576, 516, 588, 520, 580, 516, 576, 524, 576, 524, 576, 1644, 584, 1640, 580, 520, 576, 1644, 576, 1644, 584, 1644, 576, 1644, 576, 1640, 580, 520, 588, 516, 580, 520, 576, 1636, 584, 512, 584, 516, 584, 524, 576, 516, 580, 1640, 580, 1640, 588, 1640, 576, 524, 576, 1644, 576, 1640, 588, 1640, 580, 1640, 576, 40148, 9068, 2196, 584};
unsigned int Air_Conditioner_ON[] = {3108, 9848, 476, 1524, 584, 432, 568, 444, 496, 540, 456, 1556, 484, 524, 484, 524, 576, 424, 484, 504, 596, 432, 568, 396, 520, 512, 504, 512, 576, 424, 508, 500, 584, 440, 484, 1556, 504, 504, 592, 1496, 540, 416, 508, 516, 576, 432, 576, 452, 456, 1544, 484, 1564, 568, 464, 528, 1468, 568, 1468, 484};
unsigned int Air_Conditioner_OFF[] = {3160, 9760, 552, 1512, 524, 468, 544, 480, 524, 480, 524, 1504, 540, 480, 524, 480, 516, 488, 512, 1500, 544, 1500, 544, 480, 512, 492, 512, 492, 512, 472, 540, 492, 512, 472, 540, 480, 524, 488, 524, 480, 524, 480, 512, 492, 512, 1512, 524, 472, 532, 1524, 524, 472, 540, 492, 512, 492, 512, 1504, 540};
unsigned int Bulb_ON[] = {9084, 4528, 560, 552, 556, 564, 556, 560, 548, 556, 560, 560, 560, 560, 548, 572, 548, 560, 556, 1668, 560, 1672, 564, 1668, 552, 1672, 560, 1668, 548, 1676, 556, 1668, 552, 1672, 564, 1668, 552, 560, 556, 1676, 548, 560, 560, 560, 556, 560, 552, 1672, 564, 556, 552, 568, 548, 1676, 548, 560, 560, 1672, 548, 1672, 560, 1672, 548, 556, 556, 1672, 556};
unsigned int Bulb_OFF[] = {9112, 4520, 636, 480, 636, 488, 624, 492, 632, 520, 592, 480, 632, 492, 560, 552, 628, 496, 600, 1624, 632, 1592, 616, 1612, 612, 1616, 612, 1656, 592, 1604, 628, 1600, 624, 1604, 600, 1628, 628, 1596, 632, 1592, 616, 508, 624, 492, 608, 508, 632, 1596, 612, 504, 640, 484, 632, 484, 632, 488, 624, 1604, 632, 1592, 632, 1596, 628, 484, 640, 1592, 632};
int global_length = 0;

void custom_delay_usec(unsigned long uSecs)
{
    unsigned long Start = micros();
    unsigned long endMicros = Start + uSecs;
    if (endMicros < Start)
    { // Check if overflow
        while (micros() > Start)
        {
        }
    }
    while (micros() < endMicros)
    {
    }
}

void space(unsigned int Time)
{
    digitalWrite(SEND_PIN, LOW);
    if (Time > 0)
    {
        unsigned long Start = micros();
        unsigned long endMicros = Start + Time - 4;
        custom_delay_usec(Time);
    }
}

void mark(unsigned int Time)
{
    unsigned long Start = micros();
    unsigned long endMicros = Start + Time;
    int count = 0;

    while (micros() < endMicros)
    {
        digitalWrite(SEND_PIN, HIGH);
        custom_delay_usec(10);
        digitalWrite(SEND_PIN, LOW);
        custom_delay_usec(9);
        count++;
    }
}

void Send_IR(unsigned int *signal)
{
    digitalWrite(SEND_PIN, LOW);

    // FIX IT!!!
    // i < 72  ->  hard coding
    for (unsigned int i = 0; i < 72; i++)
    {
        unsigned long Start = micros();
        if (i & 1)
            space(signal[i]);
        else
            mark(signal[i]);
    }

    digitalWrite(SEND_PIN, LOW);
}

void ir_tv_onoff_function(void *pData)
{
    // int arg_value = 1, res;
    // res = GetIntArgumentByName(pData, ARG_TV_SWITCH, &arg_value);
    // if (res == -1)
    //     return;

    Send_IR(TV_ONOFF);
}

void ir_air_on_function(void *pData)
{
    // int arg_value = 1, res;
    // res = GetIntArgumentByName(pData, ARG_AIR_SWITCH, &arg_value);
    // if (res == -1)
    //     return;

    Send_IR(Air_Conditioner_ON);
}

void ir_air_off_function(void *pData)
{
    // int arg_value = 1, res;
    // res = GetIntArgumentByName(pData, ARG_AIR_SWITCH, &arg_value);
    // if (res == -1)
    //     return;

    Send_IR(Air_Conditioner_OFF);
}

void ir_bulb_on_function(void *pData)
{
    // int arg_value = 1, res;
    // res = GetIntArgumentByName(pData, ARG_BULB_SWITCH, &arg_value);
    // if (res == -1)
    //     return;

    Send_IR(Bulb_ON);
}

void ir_bulb_off_function(void *pData)
{
    // int arg_value = 1, res;
    // res = GetIntArgumentByName(pData, ARG_BULB_SWITCH, &arg_value);
    // if (res == -1)
    //     return;

    Send_IR(Bulb_OFF);
}

void init_pin()
{
#if BOARD_SERIAL_IS_ONE
    Serial1.begin(115200);
#else
    Serial.begin(115200);
#endif

    pinMode(SEND_PIN, OUTPUT);
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
    static Function IR_TV_ONOFF_Fucntion(IR_TV_ONOFF_FUNCTION, ir_tv_onoff_function, 0, 0);
    static Function IR_Air_ON_Fucntion(IR_AIR_ON_FUNCTION, ir_air_on_function, 0, 0);
    static Function IR_Bulb_ON_Fucntion(IR_BULB_ON_FUNCTION, ir_bulb_on_function, 0, 0);

    static Function IR_Air_OFF_Fucntion(IR_AIR_OFF_FUNCTION, ir_air_off_function, 0, 0);
    static Function IR_Bulb_OFF_Fucntion(IR_BULB_OFF_FUNCTION, ir_bulb_off_function, 0, 0);

    Client1.Add(IR_TV_ONOFF_Fucntion);
    Client1.Add(IR_Air_ON_Fucntion);
    Client1.Add(IR_Bulb_ON_Fucntion);

    Client1.Add(IR_Air_OFF_Fucntion);
    Client1.Add(IR_Bulb_OFF_Fucntion);
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
