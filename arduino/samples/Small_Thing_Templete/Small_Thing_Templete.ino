#include <thing_client.h>
#include <DHT.h>
#include <pm2008_i2c.h>

#define BOARD_SERIAL_IS_ONE (defined(ARDUINO_ARCH_SAMD) && !defined(ARDUINO_SAMD_ZERO)) || defined(ARDUINO_ARCH_SAM) || defined(ARDUINO_ARCH_MBED) || defined(__AVR_ATmega32U4__) || defined(ARDUINO_AVR_PROMICRO)

#define DHT_TYPE DHT11
//#define DHT_TYPE DHT22

#define LIGHT_PIN A0
#define SOUND_PIN A1
#define HUMID_PIN 2
#define DISTANCE_TRIGGER_PIN 3
#define DISTANCE_RECEIVE_PIN 4
#define RELAY_PIN 5
#define SEND_PIN 6

/* board names
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
Arduino_Zero_1
*/

#define CLIENT_NAME "Arduino_Name"
#define LIGHT_VALUE "light"
#define DISTANCE_VALUE "distance"
#define SOUND_VALUE "sound"
#define HUMID_VALUE "humid"
#define TEMP_VALUE "temp"
#define DUST_VALUE "dust"

#define IR_FUNCTION "ir_switch"
#define RELAY_FUNCTION "relay_switch"

#define ARG_IR_SWITCH "arg_ir_switch"
#define ARG_RELAY_SWITCH "arg_relay_switch"

#if BOARD_SERIAL_IS_ONE
ThingClient Client1(CLIENT_NAME, 3, Serial1);
#else
ThingClient Client1(CLIENT_NAME, 3, Serial);
#endif

DHT dht(HUMID_PIN, DHT_TYPE);
PM2008_I2C pm2008_i2c;

unsigned int TV_ONOFF[] = {9080, 4420, 600, 516, 584, 524, 568, 1640, 576, 516, 588, 520, 580, 516, 576, 524, 576, 524, 576, 1644, 584, 1640, 580, 520, 576, 1644, 576, 1644, 584, 1644, 576, 1644, 576, 1640, 580, 520, 588, 516, 580, 520, 576, 1636, 584, 512, 584, 516, 584, 524, 576, 516, 580, 1640, 580, 1640, 588, 1640, 576, 524, 576, 1644, 576, 1640, 588, 1640, 580, 1640, 576, 40148, 9068, 2196, 584};
unsigned int Air_Conditioner_ON[] = {3108, 9848, 476, 1524, 584, 432, 568, 444, 496, 540, 456, 1556, 484, 524, 484, 524, 576, 424, 484, 504, 596, 432, 568, 396, 520, 512, 504, 512, 576, 424, 508, 500, 584, 440, 484, 1556, 504, 504, 592, 1496, 540, 416, 508, 516, 576, 432, 576, 452, 456, 1544, 484, 1564, 568, 464, 528, 1468, 568, 1468, 484};
unsigned int Air_Conditioner_OFF[] = {3160, 9760, 552, 1512, 524, 468, 544, 480, 524, 480, 524, 1504, 540, 480, 524, 480, 516, 488, 512, 1500, 544, 1500, 544, 480, 512, 492, 512, 492, 512, 472, 540, 492, 512, 472, 540, 480, 524, 488, 524, 480, 524, 480, 512, 492, 512, 1512, 524, 472, 532, 1524, 524, 472, 540, 492, 512, 492, 512, 1504, 540};
unsigned int Bulb_ON[] = {9096, 4508, 576, 544, 576, 536, 580, 540, 568, 548, 568, 536, 580, 540, 572, 540, 576, 544, 572, 1644, 576, 1652, 580, 1652, 580, 1648, 576, 1648, 576, 1656, 576, 1652, 576, 1652, 580, 1652, 572, 532, 576, 1652, 576, 544, 576, 536, 580, 540, 576, 1652, 568, 536, 576, 544, 576, 1652, 576, 544, 576, 1652, 572, 1652, 576, 1652, 576, 544, 572, 1644, 584};
unsigned int Bulb_OFF[] = {9096, 4516, 572, 540, 580, 540, 568, 540, 580, 540, 576, 544, 576, 544, 568, 544, 576, 540, 576, 1652, 568, 1652, 580, 1652, 576, 1656, 576, 1644, 576, 1652, 576, 1648, 580, 1652, 564, 1656, 576, 1656, 572, 1652, 572, 536, 580, 540, 576, 540, 568, 1652, 576, 544, 576, 540, 576, 536, 576, 540, 580, 1652, 580, 1652, 576, 1644, 576, 544, 576, 1656, 576};
unsigned int Bulb_TIMER_OFF[] = {9096, 4504, 576, 540, 580, 536, 576, 544, 576, 544, 576, 536, 572, 544, 576, 540, 576, 540, 580, 1652, 580, 1652, 576, 1644, 576, 1656, 568, 1648, 580, 1652, 568, 1660, 568, 1656, 572, 544, 576, 1652, 576, 1648, 572, 536, 580, 540, 576, 540, 576, 1656, 576, 532, 576, 1652, 576, 540, 580, 536, 572, 1648, 576, 1656, 576, 1656, 576, 540, 568, 1648, 580};
unsigned int Bulb_NUM_1[] = {9092, 4504, 576, 540, 580, 532, 576, 536, 576, 544, 576, 544, 572, 532, 576, 536, 580, 540, 576, 1652, 568, 1652, 576, 1656, 576, 1644, 576, 1652, 576, 1652, 580, 1648, 576, 1648, 576, 540, 576, 532, 576, 1652, 568, 536, 580, 540, 576, 532, 576, 1644, 576, 544, 576, 1648, 580, 1652, 568, 540, 576, 1652, 576, 1644, 588, 1644, 576, 540, 580, 1652, 568};
unsigned int Bulb_NUM_2[] = {9096, 4504, 580, 540, 576, 540, 572, 544, 576, 540, 576, 532, 588, 532, 576, 540, 568, 540, 580, 1648, 580, 1652, 576, 1652, 576, 1648, 576, 1652, 580, 1652, 580, 1652, 576, 1656, 576, 544, 576, 532, 576, 540, 576, 540, 580, 540, 576, 540, 580, 1648, 576, 532, 588, 1644, 576, 1652, 580, 1652, 580, 1652, 576, 1656, 576, 1644, 576, 544, 572, 1648, 580};
unsigned int Bulb_NUM_3[] = {9088, 4504, 580, 540, 572, 540, 576, 544, 576, 532, 584, 532, 576, 548, 572, 536, 580, 540, 576, 1652, 568, 1644, 576, 1656, 576, 1652, 576, 1652, 568, 1652, 580, 1648, 576, 1656, 576, 1652, 576, 1652, 568, 1648, 580, 540, 572, 540, 576, 532, 584, 536, 576, 544, 576, 540, 576, 536, 580, 540, 568, 1652, 576, 1652, 576, 1644, 588, 1648, 572, 1652, 580};
unsigned int Bulb_NUM_4[] = {9088, 4516, 576, 544, 576, 532, 576, 540, 576, 540, 580, 536, 580, 540, 576, 540, 576, 544, 572, 1644, 576, 1660, 568, 1652, 580, 1648, 580, 1644, 576, 1652, 576, 1652, 580, 1652, 580, 1652, 568, 540, 584, 1644, 576, 544, 576, 1652, 576, 540, 580, 536, 580, 540, 576, 540, 576, 1656, 572, 532, 576, 1644, 580, 540, 576, 1652, 580, 1652, 572, 1652, 576, 40008, 9108, 2232, 576};
unsigned int Bulb_NUM_5[] = {9088, 4516, 576, 544, 568, 540, 576, 540, 576, 540, 580, 540, 580, 540, 568, 552, 568, 544, 576, 1652, 576, 1652, 580, 1648, 572, 1652, 572, 1656, 576, 1656, 576, 1648, 580, 1652, 568, 552, 572, 1652, 576, 1652, 576, 544, 576, 1652, 568, 540, 576, 540, 580, 536, 580, 1652, 576, 540, 576, 532, 576, 1664, 572, 540, 576, 1652, 580, 1648, 576, 1656, 576};
unsigned int Bulb_NUM_6[] = {9088, 4516, 596, 520, 576, 540, 576, 544, 576, 544, 568, 544, 600, 516, 572, 548, 568, 540, 576, 1648, 576, 1656, 604, 1624, 576, 1652, 576, 1652, 608, 1620, 576, 1656, 572, 1652, 576, 1652, 568, 548, 572, 548, 596, 1632, 592, 1624, 576, 540, 604, 512, 576, 540, 580, 536, 580, 1652, 568, 1660, 576, 532, 576, 544, 576, 1656, 576, 1652, 580, 1652, 568, 39988, 9096, 2232, 576};
unsigned int Bulb_NUM_7[] = {9108, 4508, 576, 544, 604, 512, 576, 544, 572, 540, 604, 520, 576, 540, 580, 536, 580, 540, 596, 1624, 576, 1656, 604, 1616, 576, 1652, 608, 1620, 580, 1648, 604, 1624, 576, 1648, 580, 540, 580, 540, 580, 1648, 572, 1656, 576, 540, 568, 544, 600, 512, 600, 516, 572, 1660, 568, 1652, 576, 544, 600, 512, 576, 1652, 576, 1652, 568, 1664, 572, 1652, 600};
unsigned int Bulb_NUM_8[] = {9116, 4468, 576, 540, 588, 536, 580, 540, 576, 540, 580, 536, 580, 548, 576, 544, 576, 540, 576, 1656, 576, 1652, 576, 1652, 580, 1648, 588, 1644, 584, 1644, 584, 1648, 580, 1652, 576, 540, 576, 544, 572, 540, 576, 1656, 576, 1648, 580, 540, 576, 540, 580, 540, 580, 1652, 584, 1656, 576, 1652, 580, 540, 576, 536, 580, 1652, 576, 1652, 576, 1656, 576};
unsigned int Bulb_HOUR_2[] = {9076, 4508, 584, 532, 572, 536, 580, 540, 576, 540, 568, 536, 576, 544, 576, 540, 576, 532, 576, 1656, 576, 1652, 568, 1652, 576, 1652, 576, 1656, 576, 1644, 576, 1652, 576, 1652, 568, 1652, 576, 1656, 576, 540, 568, 544, 576, 540, 580, 540, 580, 1640, 580, 540, 576, 540, 576, 544, 564, 1656, 576, 1652, 576, 1640, 576, 1648, 576, 544, 576, 1644, 576};
unsigned int Bulb_HOUR_4[] = {9096, 4504, 580, 536, 572, 548, 568, 540, 580, 536, 576, 544, 576, 532, 576, 544, 576, 540, 576, 1652, 568, 1652, 580, 1648, 576, 1648, 576, 1656, 576, 1652, 576, 1652, 568, 1648, 576, 1656, 576, 532, 576, 544, 576, 1652, 580, 540, 568, 540, 576, 540, 576, 544, 572, 532, 576, 1652, 576, 1652, 568, 540, 580, 1648, 580, 1652, 576, 1644, 576, 1656, 576};
unsigned int Bulb_HOUR_6[] = {9080, 4516, 568, 540, 576, 540, 576, 544, 576, 532, 576, 544, 576, 540, 580, 540, 572, 540, 576, 1652, 576, 1656, 572, 1648, 576, 1648, 580, 1652, 568, 1652, 576, 1656, 576, 1644, 576, 1652, 576, 540, 568, 1652, 580, 1648, 576, 544, 568, 540, 576, 544, 576, 536, 572, 536, 580, 1652, 580, 536, 576, 532, 576, 1656, 576, 1644, 576, 1652, 576, 1652, 568};
unsigned int Bulb_HOUR_8[] = {9084, 4500, 580, 540, 576, 540, 568, 536, 580, 540, 576, 540, 568, 540, 576, 544, 576, 532, 576, 1652, 576, 1652, 568, 1648, 576, 1656, 576, 1644, 576, 1652, 576, 1652, 568, 1648, 576, 544, 576, 1644, 576, 1652, 576, 1652, 568, 1652, 580, 540, 576, 1656, 568, 540, 576, 1652, 580, 540, 572, 540, 576, 540, 580, 536, 576, 1644, 576, 544, 576, 1644, 584, 39980, 9096, 2236, 576};
unsigned int Bulb_MODE_SW_LEFT[] = {9100, 4504, 580, 540, 576, 540, 576, 532, 576, 544, 576, 540, 576, 540, 580, 536, 572, 536, 580, 1652, 576, 1652, 576, 1648, 576, 1652, 580, 1652, 576, 1652, 576, 1644, 576, 1656, 576, 540, 576, 540, 568, 540, 580, 1648, 580, 540, 576, 540, 568, 544, 572, 544, 576, 1652, 568, 1652, 576, 1648, 580, 532, 576, 1644, 584, 1644, 576, 1652, 576, 1648, 580};
unsigned int Bulb_MODE_SW_RIGHT[] = {9096, 4508, 576, 544, 576, 532, 576, 540, 576, 544, 576, 540, 568, 548, 568, 536, 580, 540, 576, 1644, 576, 1644, 576, 1644, 576, 1652, 580, 1652, 568, 1652, 576, 1652, 576, 1660, 572, 536, 572, 1652, 576, 540, 580, 1644, 576, 1648, 576, 544, 576, 1652, 576, 540, 568, 1652, 568, 544, 576, 1652, 576, 532, 576, 544, 576, 1652, 576, 540, 568, 1652, 576};
unsigned int Bulb_BRIGHTNESS_UP[] = {9100, 4504, 580, 536, 576, 544, 576, 544, 576, 536, 576, 544, 576, 540, 576, 540, 580, 540, 572, 1652, 572, 1652, 576, 1644, 584, 1640, 576, 1652, 580, 1652, 576, 1644, 584, 1644, 576, 544, 572, 1648, 580, 540, 568, 540, 576, 532, 576, 544, 576, 1644, 584, 532, 576, 1652, 580, 536, 580, 1652, 576, 1644, 576, 1652, 576, 1652, 580, 536, 580, 1640, 580};
unsigned int Bulb_BRIGHTNESS_DOWN[] = {9088, 4504, 580, 540, 572, 540, 572, 540, 576, 544, 572, 532, 576, 540, 576, 540, 576, 536, 572, 1648, 580, 1652, 576, 1644, 576, 1656, 572, 1652, 580, 1652, 568, 1652, 576, 1652, 576, 532, 576, 1652, 580, 540, 580, 1644, 580, 540, 576, 540, 576, 1648, 576, 544, 576, 1652, 576, 540, 568, 1652, 580, 540, 576, 1648, 572, 1652, 576, 540, 576, 1648, 572};

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

    for (unsigned int i = 0; i < sizeof(signal) / sizeof(signal[0]); i++)
    {
        unsigned long Start = micros();
        if (i & 1)
            space(signal[i]);
        else
            mark(signal[i]);
    }

    digitalWrite(SEND_PIN, LOW);
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
    digitalWrite(DISTANCE_TRIGGER_PIN, HIGH);
    delayMicroseconds(10);
    digitalWrite(DISTANCE_TRIGGER_PIN, LOW);

    Duration = pulseIn(DISTANCE_RECEIVE_PIN, HIGH);
    int Distance_mm = ((Duration / 2.9) / 2);

    return Distance_mm;
}

int SoundSensor()
{
    return (int)analogRead(SOUND_PIN);
}

int HumidSensor()
{
    return (int)dht.readHumidity();
}

int TempSensor()
{
    return (int)dht.readTemperature();
}

int DustSensor()
{
    uint8_t ret = pm2008_i2c.read();
    int pm10 = 0;
    int pm2p0 = 0;
    int pm1p0 = 0;
    if (ret == 0)
    {
        Serial.print("PM 1.0 (GRIMM) : ");
        pm1p0 = pm2008_i2c.pm1p0_grimm;
        Serial.println(pm2008_i2c.pm1p0_grimm);
        Serial.print("PM 2.5 (GRIMM) : : ");
        pm2p0 = pm2008_i2c.pm2p5_grimm;
        Serial.println(pm2008_i2c.pm2p5_grimm);
        Serial.print("PM 10 (GRIMM) : : ");
        pm10 = pm2008_i2c.pm10_grimm;
        Serial.println(pm2008_i2c.pm10_grimm);
        Serial.print("PM 1.0 (TSI) : ");
        Serial.println(pm2008_i2c.pm1p0_tsi);
        Serial.print("PM 2.5 (TSI) : : ");
        Serial.println(pm2008_i2c.pm2p5_tsi);
        Serial.print("PM 10 (TSI) : : ");
        Serial.println(pm2008_i2c.pm10_tsi);
        Serial.print("Number of 0.3 um : ");
        Serial.println(pm2008_i2c.number_of_0p3_um);
        Serial.print("Number of 0.5 um : ");
        Serial.println(pm2008_i2c.number_of_0p5_um);
        Serial.print("Number of 1 um : ");
        Serial.println(pm2008_i2c.number_of_1_um);
        Serial.print("Number of 2.5 um : ");
        Serial.println(pm2008_i2c.number_of_2p5_um);
        Serial.print("Number of 5 um : ");
        Serial.println(pm2008_i2c.number_of_5_um);
        Serial.print("Number of 10 um : ");
        Serial.println(pm2008_i2c.number_of_10_um);
    }

    return pm2p0;
}

void ir_function(void *pData)
{
    int power = 1, res;
    res = GetIntArgumentByName(pData, ARG_IR_SWITCH, &power);
    if (res == -1)
        return;

    if (power == 0)
        Send_IR(OFF);
    else if (power == 1)
        Send_IR(ON);
}

void relay_function(void *pData)
{
    int power = 1, res;
    res = GetIntArgumentByName(pData, ARG_IR_SWITCH, &power);
    if (res == -1)
        return;

    if (power == 0)
        digitalWrite(RELAY_PIN, LOW);
    else if (power == 1)
        digitalWrite(RELAY_PIN, HIGH);
}

void init_pin()
{
#if BOARD_SERIAL_IS_ONE
    Serial1.begin(115200);
#else
    Serial.begin(115200);
#endif

    pinMode(HUMID_PIN, INPUT);
    pinMode(DISTANCE_TRIGGER_PIN, OUTPUT);
    pinMode(DISTANCE_RECEIVE_PIN, INPUT);
    pinMode(RELAY_PIN, OUTPUT);
    pinMode(SEND_PIN, OUTPUT);
    pinMode(SOUND_PIN, INPUT);
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
    pm2008_i2c.begin();
    pm2008_i2c.command();
    delay(100);
}

void init_Value()
{
    static Value lightValue(LIGHT_VALUE, LightSensor, 0, 2000, 3000);
    static Value distanceValue(DISTANCE_VALUE, DistanceSensor, 0, 30000, 3000);
    static Value soundValue(SOUND_VALUE, SoundSensor, 0, 2000, 3000);
    static Value humidValue(HUMID_VALUE, HumidSensor, 0, 200, 3000);
    static Value tempValue(TEMP_VALUE, TempSensor, 0, 30000, 3000);
    static Value dustValue(DUST_VALUE, DustSensor, 0, 30000, 3000);

    Client1.Add(lightValue);
    Client1.Add(distanceValue);
    Client1.Add(soundValue);
    Client1.Add(humidValue);
    Client1.Add(tempValue);
    Client1.Add(dustValue);
}

void init_Function()
{
    static Function IR_Fucntion(IR_FUNCTION, ir_function, 1, 1);
    static Argument Arg_IR_Switch(ARG_IR_SWITCH, 0, 4, INTEGER);
    static Attribute IR_Function_EnergyAttribute("Energy[J]", 50, DOUBLE);

    IR_Fucntion.Add_argument(Arg_IR_Switch);
    IR_Fucntion.Add_functionattribute(IR_Function_EnergyAttribute);

    Client1.Add(IR_Fucntion);
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
