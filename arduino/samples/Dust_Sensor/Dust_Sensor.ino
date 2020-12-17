#include <thing_client.h>
#include <pm2008_i2c.h>

#define CLIENT_NAME "Sensor_Dust_1"

#define DUST_VALUE "dust"

#if BOARD_SERIAL_IS_ONE
ThingClient Client(CLIENT_NAME, 3, Serial1);
#else
ThingClient Client(CLIENT_NAME, 3, Serial);
#endif

PM2008_I2C pm2008_i2c;

int DustSensor()
{
    uint8_t ret = pm2008_i2c.read();
    int pm10 = 0;
    int pm2p0 = 0;
    int pm1p0 = 0;
    if (ret == 0)
    {
        // Serial.print("PM 1.0 (GRIMM) : ");
        // pm1p0 = pm2008_i2c.pm1p0_grimm;
        // Serial.println(pm2008_i2c.pm1p0_grimm);
        // Serial.print("PM 2.5 (GRIMM) : : ");
        // pm2p0 = pm2008_i2c.pm2p5_grimm;
        // Serial.println(pm2008_i2c.pm2p5_grimm);
        // Serial.print("PM 10 (GRIMM) : : ");
        pm10 = pm2008_i2c.pm10_grimm;
        // Serial.println(pm2008_i2c.pm10_grimm);
        // Serial.print("PM 1.0 (TSI) : ");
        // Serial.println(pm2008_i2c.pm1p0_tsi);
        // Serial.print("PM 2.5 (TSI) : : ");
        // Serial.println(pm2008_i2c.pm2p5_tsi);
        // Serial.print("PM 10 (TSI) : : ");
        // Serial.println(pm2008_i2c.pm10_tsi);
        // Serial.print("Number of 0.3 um : ");
        // Serial.println(pm2008_i2c.number_of_0p3_um);
        // Serial.print("Number of 0.5 um : ");
        // Serial.println(pm2008_i2c.number_of_0p5_um);
        // Serial.print("Number of 1 um : ");
        // Serial.println(pm2008_i2c.number_of_1_um);
        // Serial.print("Number of 2.5 um : ");
        // Serial.println(pm2008_i2c.number_of_2p5_um);
        // Serial.print("Number of 5 um : ");
        // Serial.println(pm2008_i2c.number_of_5_um);
        // Serial.print("Number of 10 um : ");
        // Serial.println(pm2008_i2c.number_of_10_um);
    }

    return pm10;
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
    pm2008_i2c.begin();
    pm2008_i2c.command();
}

void init_Value()
{
    static Value dustValue(DUST_VALUE, DustSensor, 0, 30000, 3000);

    Client.Add(dustValue);
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
