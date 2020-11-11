#include <thing_client.h>
//#include <IRremote.h>

#define DHT_TYPE DHT11

#define LIGHT_PIN A0
#define DISTANCE_TRIGGER_PIN 2
#define DISTANCE_RECEIVE_PIN 3
#define RELAY_PIN 4
#define SEND_PIN 5

unsigned int ON[] = {9096, 4508, 576, 544, 576, 536, 580, 540, 568, 548, 568, 536, 580, 540, 572, 540, 576, 544, 572, 1644, 576, 1652, 580, 1652, 580, 1648, 576, 1648, 576, 1656, 576, 1652, 576, 1652, 580, 1652, 572, 532, 576, 1652, 576, 544, 576, 536, 580, 540, 576, 1652, 568, 536, 576, 544, 576, 1652, 576, 544, 576, 1652, 572, 1652, 576, 1652, 576, 544, 572, 1644, 584};
unsigned int OFF[] = {9096, 4516, 572, 540, 580, 540, 568, 540, 580, 540, 576, 544, 576, 544, 568, 544, 576, 540, 576, 1652, 568, 1652, 580, 1652, 576, 1656, 576, 1644, 576, 1652, 576, 1648, 580, 1652, 564, 1656, 576, 1656, 572, 1652, 572, 536, 580, 540, 576, 540, 568, 1652, 576, 544, 576, 540, 576, 536, 576, 540, 580, 1652, 580, 1652, 576, 1644, 576, 544, 576, 1656, 576};
unsigned int TIMER_OFF[] = {9096, 4504, 576, 540, 580, 536, 576, 544, 576, 544, 576, 536, 572, 544, 576, 540, 576, 540, 580, 1652, 580, 1652, 576, 1644, 576, 1656, 568, 1648, 580, 1652, 568, 1660, 568, 1656, 572, 544, 576, 1652, 576, 1648, 572, 536, 580, 540, 576, 540, 576, 1656, 576, 532, 576, 1652, 576, 540, 580, 536, 572, 1648, 576, 1656, 576, 1656, 576, 540, 568, 1648, 580};
unsigned int NUM_1[] = {9092, 4504, 576, 540, 580, 532, 576, 536, 576, 544, 576, 544, 572, 532, 576, 536, 580, 540, 576, 1652, 568, 1652, 576, 1656, 576, 1644, 576, 1652, 576, 1652, 580, 1648, 576, 1648, 576, 540, 576, 532, 576, 1652, 568, 536, 580, 540, 576, 532, 576, 1644, 576, 544, 576, 1648, 580, 1652, 568, 540, 576, 1652, 576, 1644, 588, 1644, 576, 540, 580, 1652, 568};
unsigned int NUM_2[] = {9096, 4504, 580, 540, 576, 540, 572, 544, 576, 540, 576, 532, 588, 532, 576, 540, 568, 540, 580, 1648, 580, 1652, 576, 1652, 576, 1648, 576, 1652, 580, 1652, 580, 1652, 576, 1656, 576, 544, 576, 532, 576, 540, 576, 540, 580, 540, 576, 540, 580, 1648, 576, 532, 588, 1644, 576, 1652, 580, 1652, 580, 1652, 576, 1656, 576, 1644, 576, 544, 572, 1648, 580};
unsigned int NUM_3[] = {9088, 4504, 580, 540, 572, 540, 576, 544, 576, 532, 584, 532, 576, 548, 572, 536, 580, 540, 576, 1652, 568, 1644, 576, 1656, 576, 1652, 576, 1652, 568, 1652, 580, 1648, 576, 1656, 576, 1652, 576, 1652, 568, 1648, 580, 540, 572, 540, 576, 532, 584, 536, 576, 544, 576, 540, 576, 536, 580, 540, 568, 1652, 576, 1652, 576, 1644, 588, 1648, 572, 1652, 580};
unsigned int NUM_4[] = {9088, 4516, 576, 544, 576, 532, 576, 540, 576, 540, 580, 536, 580, 540, 576, 540, 576, 544, 572, 1644, 576, 1660, 568, 1652, 580, 1648, 580, 1644, 576, 1652, 576, 1652, 580, 1652, 580, 1652, 568, 540, 584, 1644, 576, 544, 576, 1652, 576, 540, 580, 536, 580, 540, 576, 540, 576, 1656, 572, 532, 576, 1644, 580, 540, 576, 1652, 580, 1652, 572, 1652, 576, 40008, 9108, 2232, 576};
unsigned int NUM_5[] = {9088, 4516, 576, 544, 568, 540, 576, 540, 576, 540, 580, 540, 580, 540, 568, 552, 568, 544, 576, 1652, 576, 1652, 580, 1648, 572, 1652, 572, 1656, 576, 1656, 576, 1648, 580, 1652, 568, 552, 572, 1652, 576, 1652, 576, 544, 576, 1652, 568, 540, 576, 540, 580, 536, 580, 1652, 576, 540, 576, 532, 576, 1664, 572, 540, 576, 1652, 580, 1648, 576, 1656, 576};
unsigned int NUM_6[] = {9088, 4516, 596, 520, 576, 540, 576, 544, 576, 544, 568, 544, 600, 516, 572, 548, 568, 540, 576, 1648, 576, 1656, 604, 1624, 576, 1652, 576, 1652, 608, 1620, 576, 1656, 572, 1652, 576, 1652, 568, 548, 572, 548, 596, 1632, 592, 1624, 576, 540, 604, 512, 576, 540, 580, 536, 580, 1652, 568, 1660, 576, 532, 576, 544, 576, 1656, 576, 1652, 580, 1652, 568, 39988, 9096, 2232, 576};
unsigned int NUM_7[] = {9108, 4508, 576, 544, 604, 512, 576, 544, 572, 540, 604, 520, 576, 540, 580, 536, 580, 540, 596, 1624, 576, 1656, 604, 1616, 576, 1652, 608, 1620, 580, 1648, 604, 1624, 576, 1648, 580, 540, 580, 540, 580, 1648, 572, 1656, 576, 540, 568, 544, 600, 512, 600, 516, 572, 1660, 568, 1652, 576, 544, 600, 512, 576, 1652, 576, 1652, 568, 1664, 572, 1652, 600};
unsigned int NUM_8[] = {9116, 4468, 576, 540, 588, 536, 580, 540, 576, 540, 580, 536, 580, 548, 576, 544, 576, 540, 576, 1656, 576, 1652, 576, 1652, 580, 1648, 588, 1644, 584, 1644, 584, 1648, 580, 1652, 576, 540, 576, 544, 572, 540, 576, 1656, 576, 1648, 580, 540, 576, 540, 580, 540, 580, 1652, 584, 1656, 576, 1652, 580, 540, 576, 536, 580, 1652, 576, 1652, 576, 1656, 576};
unsigned int HOUR_2[] = {9076, 4508, 584, 532, 572, 536, 580, 540, 576, 540, 568, 536, 576, 544, 576, 540, 576, 532, 576, 1656, 576, 1652, 568, 1652, 576, 1652, 576, 1656, 576, 1644, 576, 1652, 576, 1652, 568, 1652, 576, 1656, 576, 540, 568, 544, 576, 540, 580, 540, 580, 1640, 580, 540, 576, 540, 576, 544, 564, 1656, 576, 1652, 576, 1640, 576, 1648, 576, 544, 576, 1644, 576};
unsigned int HOUR_4[] = {9096, 4504, 580, 536, 572, 548, 568, 540, 580, 536, 576, 544, 576, 532, 576, 544, 576, 540, 576, 1652, 568, 1652, 580, 1648, 576, 1648, 576, 1656, 576, 1652, 576, 1652, 568, 1648, 576, 1656, 576, 532, 576, 544, 576, 1652, 580, 540, 568, 540, 576, 540, 576, 544, 572, 532, 576, 1652, 576, 1652, 568, 540, 580, 1648, 580, 1652, 576, 1644, 576, 1656, 576};
unsigned int HOUR_6[] = {9080, 4516, 568, 540, 576, 540, 576, 544, 576, 532, 576, 544, 576, 540, 580, 540, 572, 540, 576, 1652, 576, 1656, 572, 1648, 576, 1648, 580, 1652, 568, 1652, 576, 1656, 576, 1644, 576, 1652, 576, 540, 568, 1652, 580, 1648, 576, 544, 568, 540, 576, 544, 576, 536, 572, 536, 580, 1652, 580, 536, 576, 532, 576, 1656, 576, 1644, 576, 1652, 576, 1652, 568};
unsigned int HOUR_8[] = {9084, 4500, 580, 540, 576, 540, 568, 536, 580, 540, 576, 540, 568, 540, 576, 544, 576, 532, 576, 1652, 576, 1652, 568, 1648, 576, 1656, 576, 1644, 576, 1652, 576, 1652, 568, 1648, 576, 544, 576, 1644, 576, 1652, 576, 1652, 568, 1652, 580, 540, 576, 1656, 568, 540, 576, 1652, 580, 540, 572, 540, 576, 540, 580, 536, 576, 1644, 576, 544, 576, 1644, 584, 39980, 9096, 2236, 576};
unsigned int MODE_SW_LEFT[] = {9100, 4504, 580, 540, 576, 540, 576, 532, 576, 544, 576, 540, 576, 540, 580, 536, 572, 536, 580, 1652, 576, 1652, 576, 1648, 576, 1652, 580, 1652, 576, 1652, 576, 1644, 576, 1656, 576, 540, 576, 540, 568, 540, 580, 1648, 580, 540, 576, 540, 568, 544, 572, 544, 576, 1652, 568, 1652, 576, 1648, 580, 532, 576, 1644, 584, 1644, 576, 1652, 576, 1648, 580};
unsigned int MODE_SW_RIGHT[] = {9096, 4508, 576, 544, 576, 532, 576, 540, 576, 544, 576, 540, 568, 548, 568, 536, 580, 540, 576, 1644, 576, 1644, 576, 1644, 576, 1652, 580, 1652, 568, 1652, 576, 1652, 576, 1660, 572, 536, 572, 1652, 576, 540, 580, 1644, 576, 1648, 576, 544, 576, 1652, 576, 540, 568, 1652, 568, 544, 576, 1652, 576, 532, 576, 544, 576, 1652, 576, 540, 568, 1652, 576};
unsigned int BRIGHTNESS_UP[] = {9100, 4504, 580, 536, 576, 544, 576, 544, 576, 536, 576, 544, 576, 540, 576, 540, 580, 540, 572, 1652, 572, 1652, 576, 1644, 584, 1640, 576, 1652, 580, 1652, 576, 1644, 584, 1644, 576, 544, 572, 1648, 580, 540, 568, 540, 576, 532, 576, 544, 576, 1644, 584, 532, 576, 1652, 580, 536, 580, 1652, 576, 1644, 576, 1652, 576, 1652, 580, 536, 580, 1640, 580};
unsigned int BRIGHTNESS_DOWN[] = {9088, 4504, 580, 540, 572, 540, 572, 540, 576, 544, 572, 532, 576, 540, 576, 540, 576, 536, 572, 1648, 580, 1652, 576, 1644, 576, 1656, 572, 1652, 580, 1652, 568, 1652, 576, 1652, 576, 532, 576, 1652, 580, 540, 580, 1644, 580, 540, 576, 540, 576, 1648, 576, 544, 576, 1652, 576, 540, 568, 1652, 580, 540, 576, 1648, 572, 1652, 576, 540, 576, 1648, 572};

void send_ir(unsigned int *signal)
{
    digitalWrite(SEND_PIN, LOW);

    for (unsigned int i = 0; i < sizeof(signal) / sizeof(signal[0]); i++)
    {
        unsigned long Start = micros();
        if (i & 1)
        {
            space(signal[i]);
        }
        else
        {
            mark(signal[i]);
        }
    }

    digitalWrite(SEND_PIN, LOW);

    delay(1000);
}

void custom_delay_usec(unsigned long uSecs)
{

    unsigned long Start = micros();
    unsigned long endMicros = Start + uSecs;
    if (endMicros < Start)
    { // Check if overflow
        while (micros() > Start)
        {
        } // wait until overflow
    }
    while (micros() < endMicros)
    {
    } // normal wait
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
    //Serial.println("count : " + count);
}

int getDistance(long time)
{
    // Calculates the Distance in mm
    // ((time)*(Speed of sound))/ toward and backward of object) * 10

    int DistanceCalc;                  // Calculation variable
    DistanceCalc = ((time / 2.9) / 2); // Actual calculation in mm
    //DistanceCalc = time / 74 / 2; // Actual calculation in inches

    return DistanceCalc; // return calculated value
}

int LightSensor()
{
    return (int)analogRead(LIGHT_PIN);
}

int DistanceSensor()
{
    long Duration = 0;
    digitalWrite(DISTANCE_TRIGGER_PIN, LOW);
    delayMicroseconds(2);
    digitalWrite(DISTANCE_TRIGGER_PIN, HIGH); // Trigger pin to HIGH
    delayMicroseconds(10);                    // 10us high
    digitalWrite(DISTANCE_TRIGGER_PIN, LOW);  // Trigger pin to HIGH

    Duration = pulseIn(DISTANCE_RECEIVE_PIN, HIGH); // Waits for the echo pin to get high
                                                    // returns the Duration in microseconds

    int Distance_mm = getDistance(Duration); // Use function to calculate the distance
    return Distance_mm;
}

//CLIENT_NAME format : Arduino_BOARDNAME_NUMBER,
//if board name have space, replace it to '_'(under bar)

/*
Arduino_Nano_1
Arduino_Micro_1
Arduino_Mega_1
Arduino_Mega_2
Arduino_Mega_3
Arduino_Mega_4
Arduino_MKR1000_1
Arduino_MKR_Zero_1
Arduino_MKR_WiFi_1010_1
Arduino_WAN_1310_1
Arduino_Nano_33_IoT_1
Arduino_Nano_33_BLE_Sense_1
Arduino_YUN_1
Arduino_DUE_1
*/

#define CLIENT_NAME "Arduino_Nano_33_BLE_Sense_1"
#define LIGHT_VALUE "light"
#define DISTANCE_VALUE "distance"
#define IR_RELAY_FUNCTION "ir_relay"
#define ARG_LIGHT_RELAY_SWITCH "light_relay_power"

void turn_on_relay_ir(void *pData)
{
    int power = 1, res;
    res = GetIntArgumentByName(pData, ARG_LIGHT_RELAY_SWITCH, &power);
    if (res == -1)
    {
        return;
    }

    if (power == 1)
        digitalWrite(RELAY_PIN, HIGH);
    else if (power == 0)
        digitalWrite(RELAY_PIN, LOW);
    else if (power == 3)
        send_ir(ON);
    else if (power == 2)
        send_ir(OFF);
}

#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_MBED)
ThingClient client_A3(CLIENT_NAME, 60, Serial1);
#elif
ThingClient client_A3(CLIENT_NAME, 60, Serial);
#endif

Value lightValue(LIGHT_VALUE, LightSensor, 0, 2000, 5000);
Value distanceValue(DISTANCE_VALUE, DistanceSensor, 0, 30000, 5000);
Function IR_Relay_Fucntion(IR_RELAY_FUNCTION, turn_on_relay_ir, 1, 1);
Argument argLightRelaySwitch(ARG_LIGHT_RELAY_SWITCH, 0, 4, INTEGER);
Attribute Ir_Relay_Function_EnergyAttribute("light_Energy[J]", 50, DOUBLE);

void setup()
{
#if defined(ARDUINO_ARCH_SAMD) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_MBED)
    Serial1.begin(115200);
#elif
    Serial.begin(115200);
#endif
    pinMode(SEND_PIN, OUTPUT);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(LED_BUILTIN, OUTPUT);

    for (int i = 0; i < 4; i++)
    {
        digitalWrite(LED_BUILTIN, HIGH);
        delay(100);
        digitalWrite(LED_BUILTIN, LOW);
        delay(100);
    }

    IR_Relay_Fucntion.Add_argument(argLightRelaySwitch);
    IR_Relay_Fucntion.Add_functionattribute(Ir_Relay_Function_EnergyAttribute);
    client_A3.Add(lightValue);
    client_A3.Add(distanceValue);
    client_A3.Add(IR_Relay_Fucntion);
    client_A3.Setting();
}

void loop()
{
    client_A3.DoLoop();
}