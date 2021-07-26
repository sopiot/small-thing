#include <thing.h>

// Module libraries
#include <DHT.h>
#include <GP2YDustSensor.h>
#include <pm2008_i2c.h>

// Pins
const int kGY10AnalogdustPin = A0;
const int khumidPin = 2;
const int kGY10DigitaldustPin = 3;

// #define DHT_TYPE DHT11
#define DHT_TYPE DHT22
DHT dht(khumidPin, DHT_TYPE);
PM2008_I2C pm2008_i2c;
// GP2YDustSensor dustSensor(GP2YDustSensorType::GP2Y1010AU0F, kdustledPin,
//                           kGY10AnalogdustPin);

Thing thing((const char *)"Air3", 60, SafeSerial);

int SenseHumidStatus() { return (int)dht.readHumidity(); }

int SenseTempStatus() { return (int)dht.readTemperature(); }

int SensePM2008DustStatus() {
  uint8_t ret = pm2008_i2c.read();
  if (ret == 0) {
    // Serial.print("PM 1.0 (GRIMM) : ");
    // Serial.println(pm2008_i2c.pm1p0_grimm);
    // Serial.print("PM 2.5 (GRIMM) : : ");
    // Serial.println(pm2008_i2c.pm2p5_grimm);
    // Serial.print("PM 10 (GRIMM) : : ");
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

    return (int)pm2008_i2c.number_of_1_um;
  }

  return -1;
}

int SenseGP2YDustStatus() {
  unsigned long pulse = 0;
  double value = 0.0;
  pulse = pulseIn(kGY10DigitaldustPin, LOW, 20000);
  value = (pulse - 1400) / 14.0;

  return (int)value;
}

Value humid_status((const char *)"humid_status", SenseHumidStatus, 0, 100,
                   30000);
Value temp_status((const char *)"temp_status", SenseTempStatus, 0, 100, 30000);
// Value dust_status((const char *)"dust_status", SenseGP2YDustStatus, 0, 500,
//                   30000);
Value dust_status((const char *)"dust_status", SensePM2008DustStatus, 0, 600,
                  30000);

void SetupSerial() { SafeSerial.begin(9600); }

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

//----------------------------------------
// Main
//----------------------------------------

void setup() {
  SetupSerial();
  SetupModules();
  SetupThing();
}

void loop() { thing.Loop(); }
