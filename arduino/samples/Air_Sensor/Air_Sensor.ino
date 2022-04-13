// #include "ota.h"
#include "thing.h"

#define DEVICE_NAME "Air3"

#include <DHT.h>
// #include <GP2YDustSensor.h>
#include <pm2008_i2c.h>

const int kGY10AnalogdustPin = A0;
const int khumidPin = 2;
const int kGY10DigitaldustPin = 3;

// #define DHT_TYPE DHT11
#define DHT_TYPE DHT22
DHT dht(khumidPin, DHT_TYPE);
PM2008_I2C pm2008_i2c;
/************************
red    : 5V
black  : GND0
white  : A4 (SDA)
green  : A5 (SCL)
yellow : jumper (connect to GND -> i2c, connect to VCC or floating -> uart)
************************/

// GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, kdustledPin,
//                           kGY10AnalogdustPin);

Thing thing((const char *)DEVICE_NAME, 10, SafeSerial);

int SenseHumidStatus() {
  int humid = dht.readHumidity();
  if (humid > 100) {
    return 100;
  } else {
    return humid;
  }
}

int SenseTempStatus() {
  int temp = dht.readTemperature();
  if (temp > 100) {
    return 100;
  } else {
    return temp;
  }
}

int SensePM2008DustStatus() {
  uint8_t ret = pm2008_i2c.read();
  static int dust_num_2p5;
  if (ret == 0) {
    // pm2008_i2c.pm + {1p0|2p5|p10} + {_grimm|_tsi}
    // pm2008_i2c.number_of_ + {0p3|0p5|1|2p5|5|10} + _um

    dust_num_2p5 = (int)pm2008_i2c.number_of_2p5_um;
  }
  return dust_num_2p5;
}

int SenseGP2YDustStatus() {
  unsigned long pulse = 0;
  double value = 0.0;
  pulse = pulseIn(kGY10DigitaldustPin, LOW, 20000);
  value = (pulse - 1400) / 14.0;

  return (int)value;
}

Value humid_status((const char *)"humid", SenseHumidStatus, 0, 100, 30000);
Value temp_status((const char *)"temp", SenseTempStatus, 0, 100, 30000);
// Value dust_status((const char *)"dust", SenseGP2YDustStatus, 0, 500,
//                   30000);
Value dust_status((const char *)"dust", SensePM2008DustStatus, 0, 600, 30000);

void SetupSerial() { SafeSerial.begin(115200); }

void SetupModules() {
  // Setup Pin mode
  pinMode(khumidPin, INPUT);
  pinMode(kGY10DigitaldustPin, INPUT);

  // Attach modules
  dht.begin();
  pm2008_i2c.begin();
  pm2008_i2c.command();
  // dustSensor.setBaseline(
  //     0.4);  // set no dust voltage according to your own experiments
  // dustSensor.setCalibrationFactor(
  //     1.7);  // calibrate against precision instrument
  // dustSensor.begin();
  delay(100);
}

void SetupThing() {
  // Setup Functions

  // Setup Values
  thing.Add(humid_status);
  thing.Add(temp_status);
  thing.Add(dust_status);

  // Setup Thing
  thing.Setup();
}

void setup() {
  SetupSerial();
  SetupModules();
  // WiFi_Setup("SoPIoT_2.4G", "/PeaCE/#1", DEVICE_NAME, "/PeaCE/");
  SetupThing();
}

void loop() {
  // SOPOTA();
  thing.Loop();
}
